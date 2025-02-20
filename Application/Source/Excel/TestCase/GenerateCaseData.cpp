#include "GenerateCaseData.h"

#include "ExcelData.h"
#include "ExcelUtil.h"
#include "ExcelDataManager.h"
#include "SignalDataManager.h"
#include "CaseDataWriter.h"

const bool TCNAME_CHECK_OPTION_DEACTIVATE = false;

const QString JSON_CASES_NAME = QString("cases");
const QString JSON_CASE_SIZE_NAME = QString("CaseSize");
const QString JSON_INIT_CASE_NAME = QString("InitCase");
const QString JSON_ALL_CASE_SIZE_NAME = QString("AllCaseSize");
const QString JSON_OTHER_CASE_SIZE_NAME = QString("OtherCaseSize");

const QString GEN_TYPE_DEFAULT = QString("Default");
const QString GEN_TYPE_NEGATIVE_AND_POSITIVE = QString("Negative/Positive");
const QString GEN_TYPE_NEGATIVE = QString("Negative");
const QString GEN_TYPE_POSITIVE = QString("Positive");
const QString TEXT_OTHERS = QString("Others");
const QString TEXT_OTHER = QString("Other");
const QString TEXT_SHEET = QString("Sheet");
const QString TEXT_COLLECT = QString("Collect");
const QString TEXT_VALUE_ENUM = QString("ValueEnum");
const QString TEXT_EMPTY = QString("[Empty]");
const QString TEXT_PRECONDITION_ORDER = QString("PreconditionOrder");
const QString TEXT_INPUT_SIGNAL_LIST = QString("InputSignalList");
const QString TEXT_INPUT_SIGNAL_NAME = QString("SignalName");
const QString TEXT_INPUT_DATA = QString("InputData");
const QString TEXT_PRECONDITION = QString("Precondition");
const QString TEXT_DATA_TYPE = QString("DataType");
const QString TEXT_IGN = QString("SFC.Private.IGNElapsed.Elapsed");

QSharedPointer<GenerateCaseData>& GenerateCaseData::instance() {
    static QSharedPointer<GenerateCaseData> gInstance;
    if (gInstance.isNull()) {
        gInstance = QSharedPointer<GenerateCaseData>(new GenerateCaseData());
    }
    return gInstance;
}

GenerateCaseData::GenerateCaseData() {
}

bool GenerateCaseData::excuteGenerateCaseData() {
    bool result = false;
    mIntermediateDefaultJson = QJsonObject();
    mAllCaseJson = QJsonObject();
    mCaseSizeMap = QMap<QString, int>();

    qDebug() << "GenerateCaseData()";
    ivis::common::CheckTimer checkTimer;
    genCase();
    checkTimer.check("genCase()");
    printCaseSize(GEN_TYPE_DEFAULT);
    if (!mAllCaseJson.empty()) {
        CaseDataWriter testCaseFile;
        QString fileName = testCaseFile.genTestCaseFile(mAllCaseJson, mTotalTestCaseCount);
        result = (fileName.size() > 0);
        qDebug() << "GenerateCaseData TC File :" << fileName;
    }
    return result;
}

QString GenerateCaseData::genCase() {
    const int convertStart = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription;
    const int convertEnd = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax;

#if !defined(USE_EXCEL_DATA_MANAGER_OLD)
    ExcelDataManager::instance().data()->resetExcelData(true);
#endif

    for (int sheetIndex = convertStart; sheetIndex < convertEnd; ++sheetIndex) {
        if ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription) ||
            (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetPrivates) ||
            (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConfigs)) {
            // 모듈 이름정보가 필요할 경우 해당 부분에서 구현
            // Private Sheet 도 필요한 경우도 해당 부분에서 구현
            // qDebug() << "Not support sheet :" << sheetIndex;
            continue;
        }

        // Manager 내부의 data 컨테이너를 해당 sheetIndex 에 해당하는 데이터로 업데이트
#if defined(USE_EXCEL_DATA_MANAGER_OLD)
        QVariantList sheetData = ExcelData::instance().data()->getSheetData(sheetIndex).toList();
        ExcelDataManager::instance().data()->updateExcelData(sheetIndex, sheetData);
#endif

        // Json 파일 내부에서 순서 보장을 위한 Index 할당
        int caseCnt = 0;
        int resultCnt = 0;
        int tcNameCnt = 0;

        // TCName 순회
        for (const auto& tcName :
                ExcelDataManager::instance().data()->isTCNameDataList(sheetIndex, TCNAME_CHECK_OPTION_DEACTIVATE)) {
            QString genType;
            ExcelDataManager::instance().data()->isGenTypeData(sheetIndex, tcName, genType);
            QString vehicleType = ExcelDataManager::instance().data()->isVehicleTypeData(sheetIndex, tcName);
            QString config = ExcelDataManager::instance().data()->isConfigData(sheetIndex, tcName);
            // Result 순회
            for (const auto& resultName : ExcelDataManager::instance().data()->isResultDataList(sheetIndex, tcName)) {
                // Case 순회
                for (const auto& caseName : ExcelDataManager::instance().data()->isCaseDataList(sheetIndex, tcName, resultName)) {
                    auto inputList = ExcelDataManager::instance().data()->isInputDataList(sheetIndex, tcName, resultName,
                                                                                          caseName, true);
                    if (inputList.first.isEmpty() && inputList.second.isEmpty()) {
                        if (genType == GEN_TYPE_DEFAULT) {
                            // Other case 처리
                            appendOtherCaseJson(mIntermediateDefaultJson, caseName, caseCnt, resultName, resultCnt, vehicleType,
                                                tcName, tcNameCnt, config, sheetIndex);
                        }
                    } else {
                        // 일반 case 처리
                        appendCase(genType, caseName, caseCnt, resultName, resultCnt, vehicleType, tcName, tcNameCnt, config,
                                   sheetIndex);
                        if (genType == GEN_TYPE_NEGATIVE_AND_POSITIVE) {
                            caseCnt++;  // Negative 와 Positive 를 둘 다 생성하게 되면 cnt 가 증가하므로 추가 증가시켜줌
                        }
                    }
                    caseCnt++;
                }  // Case 순회 종료
                resultCnt++;
            }  // Result 순회 종료
            tcNameCnt++;
        }  // TCName 순회 종료

        // 동일 result 의 다른 case 에서 Result 단위로 Positive 와 negative 생성값이 동일한 TC 는 삭제하는 부분
        caseCnt = 0;
        resultCnt = 0;
        tcNameCnt = 0;
        // TCName 순회
        for (const auto& tcName :
                ExcelDataManager::instance().data()->isTCNameDataList(sheetIndex, TCNAME_CHECK_OPTION_DEACTIVATE)) {
            QString genType;
            ExcelDataManager::instance().data()->isGenTypeData(sheetIndex, tcName, genType);
            // Result 순회
            for (const auto& resultName : ExcelDataManager::instance().data()->isResultDataList(sheetIndex, tcName)) {
                // Case 순회
                for (const auto& caseName : ExcelDataManager::instance().data()->isCaseDataList(sheetIndex, tcName, resultName)) {
                    auto inputList = ExcelDataManager::instance().data()->isInputDataList(sheetIndex, tcName, resultName,
                                                                                          caseName, true);
                    if (!inputList.first.isEmpty() && !inputList.second.isEmpty() && genType == GEN_TYPE_NEGATIVE_AND_POSITIVE) {
                        caseCnt++;
                        // Other 가 아니고 GEN_TYPE_NEGATIVE_AND_POSITIVE 에서만 동작
                        auto negCaseName = caseName + " [Negative]";
                        checkNegativeAndPositive(GEN_TYPE_NEGATIVE, negCaseName, caseCnt, resultName, resultCnt, tcName,
                                                 tcNameCnt, sheetIndex);
                    }
                    caseCnt++;
                }  // Case 순회 종료
                resultCnt++;
            }  // Result 순회 종료
            tcNameCnt++;
        }  // TCName 순회 종료
    }

    saveHistory();
    return "";
}

void GenerateCaseData::saveHistory() {
    // if (!mIntermediateDefaultJson.empty()) {
    //     QString _filePath = QString("%1/TcGenHistoryDefault.json").arg(ivis::common::APP_PWD());
    //     saveJsonToFile(mIntermediateDefaultJson, _filePath);
    // }
    if (!mAllCaseJson.empty()) {
        QString _filePath = QString("%1/TcGenHistory.json").arg(ivis::common::APP_PWD());
        saveJsonToFile(mAllCaseJson, _filePath);
    }
}

void GenerateCaseData::eraseMergeTag(QString& str) {
    const auto excelMergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const auto excelMergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const auto excelMerge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    QStringList eraseTaglist = {excelMergeStart, excelMergeEnd, excelMerge};
    str = ivis::common::getRemoved(str, eraseTaglist);
}

void GenerateCaseData::appendCase(const QString& genType, const QString& caseName, const int& caseNumber,
                                  const QString& resultName, const int& resultNumber, const QString& vehicleType,
                                  const QString& tcName, const int& tcNameNumber, const QString& config,
                                  const int& sheetNumber) {
    QPair<QString, QString> sendStrPair = getSignalInfoString(genType, sheetNumber, tcName, resultName, caseName, false);
    QString sendStr1 = sendStrPair.first;
    callPython(sendStr1);
    QJsonObject caseJson1 = readJson();

    QString sendStr2 = sendStrPair.second;
    callPython(sendStr2);
    QJsonObject caseJson2 = readJson();
#if 0
    qDebug() << "caseName: " << caseName;
#endif
    auto negCaseName = caseName + " [Negative]";
    auto posiCaseName = caseName + " [Positive]";

    if (genType == GEN_TYPE_DEFAULT) {
        // Default case 추가
        QJsonObject newCaseJson = getCaseInfoJson(genType, tcName, config, caseJson1, false);
        appendCaseJson(mAllCaseJson, newCaseJson, caseName, caseNumber, resultName, resultNumber, vehicleType, tcName,
                       tcNameNumber, sheetNumber, genType);
        // Other 연산을 위한 mIntermediateDefaultJson 에 정보 추가 (TC 생성과 무관)
        appendCaseJson(mIntermediateDefaultJson, caseJson2, caseName, caseNumber, resultName, resultNumber, vehicleType, tcName,
                       tcNameNumber, sheetNumber, genType);
    } else if (genType == GEN_TYPE_NEGATIVE_AND_POSITIVE) {
        // Positive case 추가
        QJsonObject posiNewCaseJson = getCaseInfoJson(GEN_TYPE_POSITIVE, tcName, config, caseJson1, false);
        appendCaseJson(mAllCaseJson, posiNewCaseJson, posiCaseName, caseNumber, resultName, resultNumber, vehicleType, tcName,
                       tcNameNumber, sheetNumber, GEN_TYPE_POSITIVE);
        // Negative case 추가
        QJsonObject negNewCaseJson = getCaseInfoJson(GEN_TYPE_NEGATIVE, tcName, config, caseJson1, false);
        appendCaseJson(mAllCaseJson, negNewCaseJson, negCaseName, caseNumber + 1, resultName, resultNumber, vehicleType, tcName,
                       tcNameNumber, sheetNumber, GEN_TYPE_NEGATIVE);
    } else if (genType == GEN_TYPE_POSITIVE) {
        // Positive case 만 추가
        QJsonObject newCaseJson = getCaseInfoJson(genType, tcName, config, caseJson1, false);
        appendCaseJson(mAllCaseJson, newCaseJson, posiCaseName, caseNumber, resultName, resultNumber, vehicleType, tcName,
                       tcNameNumber, sheetNumber, genType);
    } else {
        // no operation
    }
}

QPair<QString, QString> GenerateCaseData::getSignalInfoString(const QString& genType, const int& sheetNum, const QString& tcName,
                                                              const QString& resultName, const QString& caseName,
                                                              const bool& isOther) {
    QPair<QString, QString> ret;
    QMap<int, QPair<QString, SignalData>> signalDataList;
    QMap<QString, SignalData> sigDataInfoMap;

    if (isOther) {
        auto otherInputList = ExcelDataManager::instance().data()->isInputDataList(sheetNum, tcName, QString(), QString(), true);
        signalDataList = SignalDataManager::instance().data()->isOtherInputSignalDataInfo(otherInputList, sigDataInfoMap);
    } else {
        auto inputList = ExcelDataManager::instance().data()->isInputDataList(sheetNum, tcName, resultName, caseName, true);
        signalDataList = SignalDataManager::instance().data()->isTestCaseInputSignalDataInfo(inputList, sigDataInfoMap);
    }

    // 입력 순서가 보장되는 Signal list
    QStringList tmpListFirst;
    tmpListFirst << QString("TcGenType   : ") + genType;

    for (const auto& mapKey : signalDataList.keys()) {
        auto sig1 = signalDataList[mapKey].first;
        auto sigDataInfo1 = signalDataList[mapKey].second;
        tmpListFirst << QString("InputSignalName   : ") + sig1;
        tmpListFirst << QString("InputDataType   : ") + QString::number(sigDataInfo1.getDataType());
        tmpListFirst << QString("InputKeywordType   : ") + QString::number(sigDataInfo1.getKeywordType());
        tmpListFirst << QString("InputData   : ") + sigDataInfo1.getConvertData().join(", ");
        tmpListFirst << QString("InputPrecondition   : ") + sigDataInfo1.getPrecondition().join(", ");
        tmpListFirst << QString("InputValueEnum   : ") + sigDataInfo1.getValueEnum().join(", ");
        tmpListFirst << "\n";
    }
    ret.first = tmpListFirst.join("\n");

    // 입력 순서가 보장되지 않고 알파벳 순서로 재배열한 Signal list
    QStringList tmpListSecond;
    tmpListSecond << QString("TcGenType   : ") + genType;

    for (const auto& sig2 : sigDataInfoMap.keys()) {
        auto sigDataInfo2 = sigDataInfoMap[sig2];
        tmpListSecond << QString("InputSignalName   : ") + sig2;
        tmpListSecond << QString("InputDataType   : ") + QString::number(sigDataInfo2.getDataType());
        tmpListSecond << QString("InputKeywordType   : ") + QString::number(sigDataInfo2.getKeywordType());
        tmpListSecond << QString("InputData   : ") + sigDataInfo2.getConvertData().join(", ");
        tmpListSecond << QString("InputPrecondition   : ") + sigDataInfo2.getPrecondition().join(", ");
        tmpListSecond << QString("InputValueEnum   : ") + sigDataInfo2.getValueEnum().join(", ");
        tmpListSecond << "\n";
    }
    ret.second = tmpListSecond.join("\n");
#if 0
    qDebug().noquote() << "getSignalInfoString" << Qt::endl << ret << Qt::endl;
#endif
    return ret;
}

void GenerateCaseData::callPython(const QString& str) {
#if 0
    QString cmd = QString("PYTHONPATH=%1/CaseGen/ python %1/CaseGen/tests/run.py \"%2\"").arg(ivis::common::APP_PWD()).arg(str);
#else
    QString cmd =
        QString("PYTHONPATH=%1/CaseGen/ python -O %1/CaseGen/tests/run.py \"%2\"").arg(ivis::common::APP_PWD()).arg(str);
#endif
    ivis::common::ExcuteProgram process(false);
    QStringList log;
    bool result = process.start(cmd, log);
#if 0
    qDebug() << "Python call...";
    if (result) {
        qDebug() << "Success(GenerateCaseData::callPython)";
    } else {
        qDebug() << "Fail(GenerateCaseData::callPython): " << log;
    }
    qDebug() << "=================================================================================================\n\n";
#endif
}

QJsonObject GenerateCaseData::readJson(const QString& filePath) {
    // If you wanna change the path, Use the following linie
    // QString _filePath = filePath;
    QString _filePath = filePath;
    if (filePath.isEmpty() == true) {
        _filePath = QString("%1/CaseResult.json").arg(ivis::common::APP_PWD());
    }
    QFile file(_filePath);

    // 파일 열기
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file:" << filePath;
        return {};
    }

    // 파일의 내용을 읽고 JSON 문서로 변환
    QByteArray fileData = file.readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData);

    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        qWarning() << "Invalid JSON format in file:" << filePath;
        return {};
    }

    // 최상위 JSON 객체 반환
    return jsonDoc.object();
}

void GenerateCaseData::printJson(const QJsonObject& jsonObj) {
    QJsonDocument doc(jsonObj);
    QString jsonString = doc.toJson(QJsonDocument::Indented);
    qDebug().noquote() << jsonString;
}

void GenerateCaseData::appendCaseJson(QJsonObject& fileJson, QJsonObject& caseJson, const QString& caseName,
                                      const int& caseNumber, const QString& resultName, const int& resultNumber,
                                      const QString& vehicleType, const QString& tcName, const int& tcNameNumber,
                                      const int& sheetNumber, const QString& genType) {
    const int sheetIdxStart = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription;
    // QJsonObject tmpJson = readJson();
    // Create keys based on the provided numbers
    QStringList sheetList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toStringList();
    QStringList columnTitleList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeOtherTitle).toStringList();
    QString titleTcName = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
    QString titleVehicleType = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType));
    QString titleGenType = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::GenType));
#if defined(USE_SHEET_COLUMN_OLD)
    QString titleConfigSig = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::ConfigSignal));
    QString titleConfigSigData = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Data));
#else
    QString titleConfigSig;
    QString titleConfigSigData;
#endif
    QString titleResult = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
    QString titleOutputSignal = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal));
    QString titleIsInitialize = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::IsInitialize));
    QString titleOutputValue = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue));
    QString titleCase = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case));
    QString titleSheet = TEXT_SHEET;

    QString sheetKey = QString("_%1[%2]").arg(titleSheet).arg(sheetNumber - sheetIdxStart, 3, 10, QChar('0'));
    QString sheetName = sheetList[sheetNumber - sheetIdxStart];
    QString tcNameKey = QString("_%1[%2]").arg(titleTcName).arg(tcNameNumber, 3, 10, QChar('0'));
    QString resultKey = QString("_%1[%2]").arg(titleResult).arg(resultNumber, 3, 10, QChar('0'));
    QString caseKey = QString("_%1[%2]").arg(titleCase).arg(caseNumber, 3, 10, QChar('0'));

    // 1. SheetName 확인
    if (!fileJson.contains(sheetKey)) {
        fileJson[sheetKey] = QJsonObject();
    }
    QJsonObject sheetJson = fileJson[sheetKey].toObject();
    if (!sheetJson.contains(titleSheet)) {
        sheetJson[titleSheet] = sheetName;
    }

    // 2. TCName 확인
    if (!sheetJson.contains(tcNameKey)) {
        sheetJson[tcNameKey] = QJsonObject();
    }
    QJsonObject tcNameJson = sheetJson[tcNameKey].toObject();
    if (!tcNameJson.contains(titleTcName)) {
        tcNameJson[titleTcName] = tcName;
        tcNameJson[titleVehicleType] = vehicleType;
#if 0
        if (!tcNameJson.contains(titleConfigSig)) {
            tcNameJson[titleConfigSig] = getConfigSig(sheetNumber, QStringList({tcName, "", ""}));
        }
#endif
    }

    // 3. Result 확인
    if (!tcNameJson.contains(resultKey)) {
        tcNameJson[resultKey] = QJsonObject();
    }
    QJsonObject resultJson = tcNameJson[resultKey].toObject();
    if (!resultJson.contains(titleResult)) {
        resultJson[titleResult] = resultName;
        if (!resultJson.contains(titleOutputSignal)) {
            resultJson[titleOutputSignal] = getOutputSig(sheetNumber, tcName, resultName);
        }
    }

    // 4. Case 확인
    if (!caseJson.contains(titleCase)) {
        caseJson[titleCase] = caseName;
        caseJson[titleGenType] = genType;
    }

    if (fileJson == mAllCaseJson) {
        QStringList nameIdentifierList;
        nameIdentifierList << sheetName << tcName << genType << resultName << caseName;
        QString nameIdentifier = nameIdentifierList.join("@");
        mCaseSizeMap.insert(nameIdentifier, caseJson[JSON_CASE_SIZE_NAME].toInt());
    }

    // 변경된 JsonObject를 원래 object로 재할당
    resultJson[caseKey] = caseJson;

    // Update the JSON objects back into the hierarchy
    tcNameJson[resultKey] = resultJson;
    sheetJson[tcNameKey] = tcNameJson;
    fileJson[sheetKey] = sheetJson;
}

template <typename T>
QJsonArray GenerateCaseData::toJsonArray(const QList<T>& list) {
    QJsonArray jsonArray;
    for (const auto& item : list) {
        jsonArray.append(QJsonValue::fromVariant(QVariant(item)));
    }
    return jsonArray;
}

QString GenerateCaseData::getGenType() {
    // API가 만들어지면 나중에 구현 필요
    QString ret = GEN_TYPE_DEFAULT;
    // QString ret = GEN_TYPE_NEGATIVE_AND_POSITIVE;
    // QString ret = GEN_TYPE_NEGATIVE;
    return ret;
}

QJsonObject GenerateCaseData::getConfigSig(const int& sheetIdx, const QStringList& strList) {
    QJsonObject ret;
    QStringList columnTitleList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeOtherTitle).toStringList();
#if defined(USE_SHEET_COLUMN_OLD)
    QString titleConfigSigData = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Data));
#else
    QString titleConfigSigData;
#endif
    QMap<QString, SignalData> configSigMap;  // = SignalDataManager::instance().data()->isConfigSignalDataInfo(sheetIdx, strList);

    for (const auto& configSigKey : configSigMap.keys()) {
        auto tmpSignalDataInfo = configSigMap[configSigKey];
        auto tmpInputDataList = tmpSignalDataInfo.getConvertData();
        // 1. ret에서 QJsonObject를 가져오기
        QJsonObject configObj = ret[configSigKey].toObject();
        // 2. 가져온 객체에 데이터 추가
        if (!configObj.contains(titleConfigSigData)) {
            configObj[titleConfigSigData] = toJsonArray(tmpInputDataList);
        }
        // 3. 수정된 객체를 다시 ret에 설정
        ret[configSigKey] = configObj;
    }
    return ret;
}

QJsonObject GenerateCaseData::getOutputSig(const int& sheetIdx, const QString& tcName, const QString& resultName) {
    QJsonObject ret;
    QStringList columnTitleList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeOtherTitle).toStringList();
    QString titleIsInitialize = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::IsInitialize));
    QString titleOutputValue = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue));
    auto outputList = ExcelDataManager::instance().data()->isOutputDataList(sheetIdx, tcName, resultName);
    auto outputSigMap = SignalDataManager::instance().data()->isOutputSignalDataInfo(outputList);
    for (const auto& mapKey : outputSigMap.keys()) {
        auto outputSigKey = outputSigMap[mapKey].first;
        auto tmpSignalDataInfo = outputSigMap[mapKey].second;
        auto tmpInputDataList = tmpSignalDataInfo.getConvertData();
        auto tmpValueEnum = tmpSignalDataInfo.getValueEnum();
        auto tmpIsInitialize = tmpSignalDataInfo.getInitialize();
        auto tmpOutputSigKey = outputSigKey;
        if (tmpOutputSigKey == ExcelUtil::instance().data()->isKeywordString(
                                   static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Collect))) {
            tmpOutputSigKey = TEXT_COLLECT;
        }
        // 1. ret에서 QJsonObject를 가져오기
        QJsonObject outputObj = ret[tmpOutputSigKey].toObject();
        // 3. 수정된 객체를 다시 ret에 설정
        // 2. 가져온 객체에 데이터 추가
        if (!outputObj.contains(titleOutputValue)) {
            if (tmpInputDataList.size() > 0 && tmpInputDataList[0] != "") {
                outputObj[titleOutputValue] = tmpInputDataList[0];
            } else {
                qDebug() << "No Output!!!!!";
            }
        }
        if (!outputObj.contains(titleIsInitialize)) {
            outputObj[titleIsInitialize] = tmpIsInitialize;
        }
        if (!outputObj.contains(TEXT_VALUE_ENUM)) {
            QJsonArray ValueEnumjsonArray;
            for (auto str : tmpValueEnum) {
                ValueEnumjsonArray.append(str);
            }
            outputObj[TEXT_VALUE_ENUM] = ValueEnumjsonArray;
        }
        ret[tmpOutputSigKey] = outputObj;
    }
    return ret;
}

void GenerateCaseData::saveJsonToFile(const QJsonObject& json, const QString& filePath) {
    QJsonDocument doc(json);  // QJsonObject를 QJsonDocument로 변환
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {  // 파일을 이어쓰기 모드로 엽니다.
        qDebug() << "Failed to open file for writing:" << file.errorString();
        return;
    }

    file.write(doc.toJson(QJsonDocument::Indented));  // JSON 데이터를 파일에 기록
    file.close();                                     // 파일 닫기
    qDebug() << "JSON saved to:" << filePath;
}

void GenerateCaseData::appendOtherCaseJson(QJsonObject& fileJson, const QString& caseName, const int& caseNumber,
                                           const QString& resultName, const int& resultNumber, const QString& vehicleType,
                                           const QString& tcName, const int& tcNameNumber, const QString& config,
                                           const int& sheetNumber) {
    ivis::common::CheckTimer checkTimer;

    QStringList sheetList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toStringList();
    const int sheetIdxStart = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription;
    QStringList columnTitleList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeOtherTitle).toStringList();

    QString titleTcName = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
    QString titleResult = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
    QString titleCase = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case));
    QString titleSheet = TEXT_SHEET;

    QString sheetKey = QString("_%1[%2]").arg(titleSheet).arg(sheetNumber - sheetIdxStart, 3, 10, QChar('0'));
    QString tcNameKey = QString("_%1[%2]").arg(titleTcName).arg(tcNameNumber, 3, 10, QChar('0'));

    QString genType = GEN_TYPE_DEFAULT;
    QString sendStr = getSignalInfoString(genType, sheetNumber, tcName, QString(), QString(), true).second;
    callPython(sendStr);
    QJsonObject otherCase = readJson();

    if (!fileJson.contains(sheetKey)) {
        qWarning() << "Key error : fileJson[" << sheetKey << "]";
        return;
    }
    QJsonObject sheetJson = fileJson[sheetKey].toObject();

    if (!sheetJson.contains(tcNameKey)) {
        qWarning() << "Key error : sheetJson[" << tcNameKey << "]";
        return;
    }
    QJsonObject tcNameJson = sheetJson[tcNameKey].toObject();

    // 나중에 최적화 해야할 것 같음.... 너무 가독성이 떨어짐
    for (const QString& _key : tcNameJson.keys()) {
        if (_key.contains(titleResult)) {
            QJsonObject resultJson = tcNameJson[_key].toObject();
            for (const QString& __key : resultJson.keys()) {
                if (__key.contains(titleCase)) {
                    QJsonObject caseJson = resultJson[__key].toObject();
                    if (caseJson.contains(JSON_CASES_NAME)) {
                        QJsonObject cases = caseJson[JSON_CASES_NAME].toObject();
#if 0
                        qDebug() << _key << ":" << __key << " Size: " << cases.size();
#endif
                        removeMatchingKeys(otherCase, cases);
                    }
                }
            }
        }
    }
    appendCaseJson(fileJson, otherCase, caseName, caseNumber, resultName, resultNumber, vehicleType, tcName, tcNameNumber,
                   sheetNumber, GEN_TYPE_DEFAULT);

    QJsonObject newOtherJson = getCaseInfoJson(genType, tcName, config, otherCase, true);
    newOtherJson[titleCase] =
        ExcelUtil::instance().data()->isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Other));
    appendCaseJson(mAllCaseJson, newOtherJson, caseName, caseNumber, resultName, resultNumber, vehicleType, tcName, tcNameNumber,
                   sheetNumber, GEN_TYPE_DEFAULT);

    checkTimer.check("appendOtherCaseJson");
}

void GenerateCaseData::removeMatchingKeys(QJsonObject& otherJson, const QJsonObject& validArray) {
    // Cases 객체 가져오기
    QJsonObject casesArray = otherJson[JSON_CASES_NAME].toObject();

    // validArray 모든 키를 확인하며, 해당 키가 와일드카드 패턴일 경우 다른 키를 삭제
    for (const QString& key : validArray.keys()) {
        // 콤마로 분리된 각 부분의 조건을 설정
        QStringList keyParts = key.split(", ");
        QString pattern = "^";
        bool isEmptyPartExist = false;

        // 각 부분이 비어있다면 정규표현식에서 와일드카드로 매칭되도록 함
        for (const QString& part : keyParts) {
            if (part == TEXT_EMPTY) {
                pattern += "[^,]+, ";  // 와일드카드 처리 (콤마로 구분되는 모든 값을 포함)
                isEmptyPartExist = true;
            } else {
                pattern += QRegularExpression::escape(part) + ", ";
            }
        }
        // 마지막 콤마 제거 후 끝에 $ 추가
        pattern.chop(2);
        pattern += "$";

        // 정규표현식으로 패턴 생성
        QRegularExpression regexPattern(pattern);

        if (isEmptyPartExist) {
            // casesArray의 키들을 탐색하면서 패턴에 맞는 것 삭제
            for (const QString& caseKey : casesArray.keys()) {
                if (regexPattern.match(caseKey).hasMatch()) {
                    casesArray.remove(caseKey);
#if 0
                    qDebug() << "key: " << key;
                    // qDebug() << "regexPattern: " << regexPattern;
                    qDebug() << "match caseKey key: " << caseKey << "\n";
#endif
                }
            }
        } else {
            if (casesArray.contains(key)) {
#if 0
                qDebug() << "origin key" << key;
#endif
                casesArray.remove(key);
            } else {
#if 0
                qDebug() << "No Matching key:" << key;
#endif
            }
        }
    }

    // 수정된 casesArray를 otherJson에 다시 설정
    otherJson[JSON_CASES_NAME] = casesArray;
}

QJsonObject GenerateCaseData::getCaseInfoJson(const QString& genType, const QString& tcName, const QString& config,
                                              const QJsonObject& caseJsonObject, const bool& isOther) {
    QJsonObject inputSignalList = caseJsonObject[TEXT_INPUT_SIGNAL_LIST].toObject();
    auto preconditionOrder = caseJsonObject[TEXT_PRECONDITION_ORDER];
    QJsonObject cases = caseJsonObject[JSON_CASES_NAME].toObject();
    QJsonObject newCaseJsonObject;
    QJsonObject newCases = newCaseJsonObject[JSON_CASES_NAME].toObject();
    newCaseJsonObject[TEXT_INPUT_SIGNAL_LIST] = inputSignalList;
    newCaseJsonObject[TEXT_PRECONDITION_ORDER] = preconditionOrder;

    QMap<QString, int> flowIdxMap;
    // other 에서는 flow keyword 판단 안함.
    if (isOther == false) {
        flowIdxMap = getFlowKeywordIdxMap(inputSignalList);
    }

    if (flowIdxMap.size() > 1) {
        qDebug() << "Flow Keyword Error: too much flow keyword in a case...";
        return QJsonObject();
    }
    QMap<QString, int> configIdxMap = getConfigIdxMap(inputSignalList);
    QMap<QString, QMap<QString, QString>> configHexEnumMap = getCongigSigHexEnumMap(inputSignalList);

    // 각 case를 처리
    for (auto it = cases.begin(); it != cases.end(); ++it) {
        QString caseKey = it.key();                    // 현재 case의 키
        QJsonArray caseValues = it.value().toArray();  // 현재 case의 값 리스트

        QStringList preconditionList = getPreconditionList(inputSignalList, caseValues);

        // caseValues를 반복하여 InputSignalList에서 해당 신호 정보 가져오기
        int triggerSigIndexOrigin = 0;
        for (auto sigItr = inputSignalList.begin(); sigItr != inputSignalList.end(); ++sigItr, ++triggerSigIndexOrigin) {
            int triggerSigIndex = 0;
            QJsonObject::iterator sigIt;
            QString sigKey = sigItr.key();
            // Flow keyword 판단 부분
            if (genType == GEN_TYPE_NEGATIVE) {
                if (flowIdxMap.size() > 0 && flowIdxMap.contains(sigKey) == true) {
                    continue;
                } else {
                    sigIt = sigItr;
                    triggerSigIndex = triggerSigIndexOrigin;
                }
            } else {
                if (flowIdxMap.size() > 0 && flowIdxMap.contains(sigKey) == false) {
                    continue;
                } else {
                    sigIt = sigItr;
                    triggerSigIndex = triggerSigIndexOrigin;
                }
            }
            // Flow keyword 판단 부분 End
            auto signalKey = sigIt.key();
            QJsonObject signalData = sigIt.value().toObject();
            QString signalName = signalData[TEXT_INPUT_SIGNAL_NAME].toString();

            if (triggerSigIndex >= caseValues.size()) {
                break;  // caseValues 범위를 벗어나지 않도록
            }

            // 현재 신호의 InputData 배열과 caseValues 비교
            QJsonArray inputDataArray = signalData[TEXT_INPUT_DATA].toArray();
            QJsonArray preconditionDataArray = signalData[TEXT_PRECONDITION].toArray();
            QString caseValue = caseValues[triggerSigIndex].toString();

            if (signalName == TEXT_IGN) {
                int ignIdx = 0;
                for (const QJsonValue& value : inputDataArray) {
                    if (value.toString() == caseValue) {
                        break;
                    } else {
                        ignIdx++;
                    }
                }
                if (ignIdx < inputDataArray.size() && ignIdx < preconditionDataArray.size() && caseValue != "[Empty]") {
                    QString tag, input, precondition;
                    if (genType == GEN_TYPE_NEGATIVE) {
                        tag = getConfigTagStr(isOther, tcName, config, configIdxMap, configHexEnumMap, preconditionList,
                                              triggerSigIndex, preconditionDataArray[ignIdx].toString());
                        precondition = getPreconditionStr(preconditionList);
                        input = getInputStr(signalName, preconditionDataArray[ignIdx].toString());
                    } else {
                        tag = getConfigTagStr(isOther, tcName, config, configIdxMap, configHexEnumMap, preconditionList,
                                              triggerSigIndex, caseValue);
                        precondition = getPreconditionStr(preconditionList, triggerSigIndex, preconditionDataArray[ignIdx]);
                        input = getInputStr(signalName, caseValue);
                    }
                    newCases.insert(getTcLine(tag, precondition, input), "0");
                } else {
                    qDebug() << "IGN Index Error";
                }
            } else {
                QJsonArray loopArray = preconditionDataArray;
                if (isOther) {
                    loopArray = inputDataArray;
                }
                // loopArray 를 순회하면서 Precondition Str 을 만들어냄
                for (const QJsonValue& preconditionValue : loopArray) {
                    if (preconditionValue.toString() != caseValue && caseValue != "[Empty]" && preconditionValue != "[Empty]") {
                        QString tag, input, precondition;
                        if (genType == GEN_TYPE_NEGATIVE) {
                            tag = getConfigTagStr(isOther, tcName, config, configIdxMap, configHexEnumMap, preconditionList,
                                                  triggerSigIndex, preconditionValue.toString());
                            precondition = getPreconditionStr(preconditionList);
                            input = getInputStr(signalName, preconditionValue.toString());
                        } else {
                            tag = getConfigTagStr(isOther, tcName, config, configIdxMap, configHexEnumMap, preconditionList,
                                                  triggerSigIndex, caseValue);
                            precondition = getPreconditionStr(preconditionList, triggerSigIndex, preconditionValue);
                            input = getInputStr(signalName, caseValue);
                        }
                        newCases.insert(getTcLine(tag, precondition, input), "0");
                    }
                }
            }
        }
    }

    // 해당 case에서 사용했던 모든 신호를 precondition 값으로 초기화해줌
    if (genType == GEN_TYPE_NEGATIVE) {
        QString initStr = getInitStr(inputSignalList);
        QJsonObject initJson = newCaseJsonObject[JSON_INIT_CASE_NAME].toObject();
        initJson[initStr] = "0";
        newCaseJsonObject[JSON_INIT_CASE_NAME] = initJson;
    }

    // 생성된 Cases를 새로운 Object 에 삽입
    newCaseJsonObject[JSON_CASES_NAME] = newCases;
    newCaseJsonObject[JSON_CASE_SIZE_NAME] = newCases.size();
    return newCaseJsonObject;
}

QStringList GenerateCaseData::getPreconditionList(const QJsonObject& inputSignalList, const QJsonArray& caseValues) {
    QStringList preconditionList;
    int caseSigIndex = 0;
    for (auto sigItr = inputSignalList.begin(); sigItr != inputSignalList.end(); ++sigItr, ++caseSigIndex) {
        QString caseValue = caseValues[caseSigIndex].toString();
        QJsonObject signalData = sigItr.value().toObject();
        QJsonArray preconditionDataArray = signalData[TEXT_PRECONDITION].toArray();
        if (caseValue == "[Empty]" && preconditionDataArray.size() > 0) {
            preconditionList << preconditionDataArray[0].toString();
        } else {
            preconditionList << caseValue;
        }
    }
    return preconditionList;
}

QMap<QString, int> GenerateCaseData::getFlowKeywordIdxMap(const QJsonObject& inputSignalList) {
    QMap<QString, int> flowIdxMap;
    int flowIdx = 0;
    for (auto sigItr = inputSignalList.begin(); sigItr != inputSignalList.end(); ++sigItr, ++flowIdx) {
        QString signalName = sigItr.key();
        QJsonObject sigInfo = inputSignalList[signalName].toObject();
        if (sigInfo.contains("KeywordType")) {
            if (sigInfo["KeywordType"].toInt() == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Flow)) {
                flowIdxMap.insert(signalName, flowIdx);
            }
        }
    }
    return flowIdxMap;
}

QMap<QString, int> GenerateCaseData::getConfigIdxMap(const QJsonObject& inputSignalList) {
    QMap<QString, int> configIdxMap;
    int configIdx = 0;
    for (auto sigItr = inputSignalList.begin(); sigItr != inputSignalList.end(); ++sigItr, ++configIdx) {
        QString signalKey = sigItr.key();
        QJsonObject sigInfo = inputSignalList[signalKey].toObject();
        if (sigInfo.contains(TEXT_INPUT_SIGNAL_NAME) && sigInfo.contains("KeywordType")) {
            QString signalName = sigInfo[TEXT_INPUT_SIGNAL_NAME].toString();
            if (sigInfo["KeywordType"].toInt() == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Config)) {
                configIdxMap.insert(signalName, configIdx);
            }
        }
    }
    return configIdxMap;
}

QMap<QString, QMap<QString, QString>> GenerateCaseData::getCongigSigHexEnumMap(const QJsonObject& inputSignalList) {
    QMap<QString, QMap<QString, QString>> congigSigHexEnumMap;
    int configIdx = 0;
    for (auto sigItr = inputSignalList.begin(); sigItr != inputSignalList.end(); ++sigItr, ++configIdx) {
        QString signalKey = sigItr.key();
        QJsonObject sigInfo = inputSignalList[signalKey].toObject();
        if (sigInfo.contains(TEXT_INPUT_SIGNAL_NAME) && sigInfo.contains("KeywordType")) {
            QString signalName = sigInfo[TEXT_INPUT_SIGNAL_NAME].toString();
            if (sigInfo["KeywordType"].toInt() == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Config) &&
                sigInfo.contains("ValueEnum")) {
                QMap<QString, QString> tmpMap;
                QJsonObject tepJson = sigInfo["ValueEnum"].toObject();
                for (const QString& key : tepJson.keys()) {
                    tmpMap.insert(key, tepJson.value(key).toString());
                }
                congigSigHexEnumMap.insert(signalName, tmpMap);
            }
        }
    }
    return congigSigHexEnumMap;
}

QString GenerateCaseData::getConfigTagStr(const bool& isOther, const QString& tcName, const QString& config,
                                          const QMap<QString, int>& configIdxMap,
                                          const QMap<QString, QMap<QString, QString>>& configHexEnumMap,
                                          const QStringList& preconditionList, const int& triggerSigIndex,
                                          const QString& triggerSigValue) {
    QMap<QString, QString> configSigValueMap;
    QString configTag = "";
    if (configIdxMap.size() == 0 || configIdxMap.size() > preconditionList.size()) {
        return configTag;
    }

    for (const auto& key : configIdxMap.keys()) {
        QString signalValue = (configIdxMap[key] == triggerSigIndex) ? (triggerSigValue) : (preconditionList[configIdxMap[key]]);
        // enumValue = SignalDataManager::instance().data()->isSignalValueEnum(key, signalName);
        QString enumValue;
        if (configHexEnumMap.contains(key)) {
            if (configHexEnumMap[key].contains(signalValue)) {
                enumValue = configHexEnumMap[key][signalValue];
            } else {
                qDebug() << "No Config Enum@";
            }
        } else {
            qDebug() << "No Config Enum@@";
        }
        if (enumValue.isEmpty() == false) {
            configSigValueMap.insert(key, enumValue);
        }
    }

    bool isConfigTrue = ExcelDataManager::instance().data()->isValidConfigCheck(isOther, config, configSigValueMap);

    if (isConfigTrue == true) {
        configTag += "[config: true], ";
    } else {
        configTag += "[config: false], ";
    }
#if 0
    qDebug() << "\n";
    qDebug() << "isConfigTrue: " << isConfigTrue;
    qDebug() << configTag;
    for (auto it = configSigValueMap.cbegin(); it != configSigValueMap.cend(); ++it) {
        qDebug() << it.key() << ":" << it.value();
    }
    qDebug() << "\n";
#endif
    return configTag;
}

// QString GenerateCaseData::getPreconditionStr(const QStringList& preconditionList) {
//     QStringList preconditionStrList;
//     for (int caseValueIdx = 0; caseValueIdx < preconditionList.size(); caseValueIdx++) {
//         preconditionStrList << preconditionList[caseValueIdx];
//     }
//     QString preconditionStr = preconditionStrList.join(", ");
//     return preconditionStr;
// }

QString GenerateCaseData::getPreconditionStr(const QStringList& preconditionList, const int& triggerSigIndex,
                                             const QJsonValue& preconditionValue) {
    QStringList preconditionStrList;
    for (int caseValueIdx = 0; caseValueIdx < preconditionList.size(); caseValueIdx++) {
        if (triggerSigIndex != caseValueIdx) {
            preconditionStrList << preconditionList[caseValueIdx];
        } else {
            preconditionStrList << preconditionValue.toString();
        }
    }
    QString preconditionStr = preconditionStrList.join(", ");
    return preconditionStr;
}

QString GenerateCaseData::getInputStr(const QString& triggerSigName, const QString& caseValue) {
    QString inputStr = QString(triggerSigName + ": " + caseValue);
    return inputStr;
}

QString GenerateCaseData::getTcLine(const QString& tag, const QString& precondition, const QString& input) {
    return QString(tag + precondition + ", =>" + input);
}

QString GenerateCaseData::getInitStr(const QJsonObject& inputSignalList) {
    QStringList preconditionStrList;
    for (auto sigItr = inputSignalList.begin(); sigItr != inputSignalList.end(); ++sigItr) {
        QJsonObject signalData = sigItr.value().toObject();
        QJsonArray preconditionDataArray = signalData[TEXT_PRECONDITION].toArray();
        if (preconditionDataArray.size() <= 0) {
            qDebug() << "Precondition Error";
            return QString();
        }
        preconditionStrList << preconditionDataArray[0].toString();
    }
    QString preconditionStr = preconditionStrList.join(", ");
    return getTcLine("", preconditionStr, "Init");
}

void GenerateCaseData::checkNegativeAndPositive(const QString& genType, const QString& caseName, const int& caseNumber,
                                                const QString& resultName, const int& resultNumber, const QString& tcName,
                                                const int& tcNameNumber, const int& sheetNumber) {
    const int sheetIdxStart = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription;
    QStringList sheetList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toStringList();
    QStringList columnTitleList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeOtherTitle).toStringList();
    QString titleTcName = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
    QString titleGenType = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::GenType));
    QString titleResult = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
    QString titleCase = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case));
    QString titleSheet = TEXT_SHEET;

    QString sheetKey = QString("_%1[%2]").arg(titleSheet).arg(sheetNumber - sheetIdxStart, 3, 10, QChar('0'));
    QString sheetName = sheetList[sheetNumber - sheetIdxStart];
    QString tcNameKey = QString("_%1[%2]").arg(titleTcName).arg(tcNameNumber, 3, 10, QChar('0'));
    QString resultKey = QString("_%1[%2]").arg(titleResult).arg(resultNumber, 3, 10, QChar('0'));
    QString caseKey = QString("_%1[%2]").arg(titleCase).arg(caseNumber, 3, 10, QChar('0'));

    // 1. SheetName 확인
    if (!mAllCaseJson.contains(sheetKey)) {
        qDebug() << "No Sheet Error!";
        return;
    }
    QJsonObject sheetJson = mAllCaseJson[sheetKey].toObject();

    // 2. TCName 확인
    if (!sheetJson.contains(tcNameKey)) {
        qDebug() << "No TCName Error!";
        return;
    }
    QJsonObject tcNameJson = sheetJson[tcNameKey].toObject();

    // 3. Result 확인
    if (!tcNameJson.contains(resultKey)) {
        qDebug() << "No Result Error!";
        return;
    }
    QJsonObject resultJson = tcNameJson[resultKey].toObject();

    // 4. Case 확인
    if (!resultJson.contains(caseKey)) {
        qDebug() << "No Case Error!";
        return;
    }

    QJsonObject caseJson = resultJson[caseKey].toObject();
#if 0
    qDebug() << "caseName: " << caseName;
#endif

    // Negative caseJson 에 대해서 Positive 와 중복된 tc 가 있는지 확인함.
    if (caseJson.contains(titleGenType) && caseJson.contains(JSON_CASES_NAME)) {
        if (caseJson[titleGenType] == GEN_TYPE_NEGATIVE) {
            // 동일 result 내의 case 들 중 Positive만을 추출하여 Negative 와 비교
            for (const QString& _key : resultJson.keys()) {
                if (_key.contains(titleCase) && _key != caseKey) {
                    QJsonObject _caseJson = resultJson[_key].toObject();
                    if (_caseJson.contains(titleGenType)) {
                        if (_caseJson.contains(JSON_CASES_NAME) && _caseJson[titleGenType] == GEN_TYPE_POSITIVE) {
                            // qDebug() << "_caseJson: " << _caseJson["Case"];
                            QJsonObject positiveCases = _caseJson[JSON_CASES_NAME].toObject();
                            removeMatchingKeys(caseJson, positiveCases);
                        }
                    }
                }
            }
            // 개수 변경 반영
            if (caseJson[JSON_CASE_SIZE_NAME].toInt() != caseJson[JSON_CASES_NAME].toObject().size()) {
                caseJson[JSON_CASE_SIZE_NAME] = caseJson[JSON_CASES_NAME].toObject().size();
                QStringList nameIdentifierList;
                nameIdentifierList << sheetName << tcName << genType << resultName << caseName;
                QString nameIdentifier = nameIdentifierList.join("@");
                mCaseSizeMap[nameIdentifier] = caseJson[JSON_CASE_SIZE_NAME].toInt();
            }
        }
    }

    // 변경된 JsonObject를 원래 object로 재할당
    resultJson[caseKey] = caseJson;

    // Update the JSON objects back into the hierarchy
    tcNameJson[resultKey] = resultJson;
    sheetJson[tcNameKey] = tcNameJson;
    mAllCaseJson[sheetKey] = sheetJson;
}

void GenerateCaseData::printCaseSize(const QString& genType) {
    qDebug() << "\n\033[31m";
    qDebug() << (QString(120, '=').toLatin1().data());
    qDebug() << "[GenerateCaseData Result] :" << genType.toLatin1().data();
    qDebug() << (QString(120, '-').toLatin1().data());

    int sum = 0;
    int idx = 0;
    QString allCasekey;
    for (const auto& key : mCaseSizeMap.keys()) {
        sum += mCaseSizeMap[key];
        qDebug().nospace() << "Case[" << QString::number(idx).rightJustified(2, ' ').toLatin1().data() << "] : "
                        << QString::number(mCaseSizeMap[key]).rightJustified(5, ' ').toLatin1().data() << ",  "
                        << key.toLatin1().data();
        idx++;
    }
    mTotalTestCaseCount = sum;

    qDebug() << (QString(120, '-').toLatin1().data());
    qDebug() << (QString("Sum of cases => ") + QString::number(sum)).toLatin1().data();
    qDebug() << (QString(120, '=').toLatin1().data());
    qDebug() << "\n\n\033[0m";
}
