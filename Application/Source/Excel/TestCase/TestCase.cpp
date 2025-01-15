#include "TestCase.h"
#include "ExcelUtil.h"
#include "ControlExcel.h"  // 임시방편임. 사라질 예정
#include "ExcelDataManger.h"
#include "SignalDataManger.h"
#include "TestCaseWriter.h"

const bool TCNAME_CHECK_OPTION_DEACTIVATE = true;

const QString JSON_CASES_NAME = QString("cases");
const QString JSON_CASE_SIZE_NAME = QString("CaseSize");
const QString JSON_ALL_CASE_SIZE_NAME = QString("AllCaseSize");
const QString JSON_OTHER_CASE_SIZE_NAME = QString("OtherCaseSize");

const QString GEN_TYPE_DEFAULT = QString("Default");
const QString GEN_TYPE_NEGATIVE_AND_POSITIVE = QString("NegativeAndPositive");
const QString GEN_TYPE_NEGATIVE = QString("Negative");
const QString GEN_TYPE_POSITIVE = QString("Positive");
const QString TEXT_OTHERS = QString("Others");
const QString TEXT_OTHER = QString("Other");
const QString TEXT_SHEET = QString("Sheet");
const QString TEXT_COLLECT = QString("Collect");
const QString TEXT_VALUE_ENUM = QString("ValueEnum");
const QString TEXT_EMPTY = QString("[Empty]");
const QString TEXT_INPUT_SIGNAL_LIST = QString("InputSignalList");
const QString TEXT_INPUT_DATA = QString("InputData");
const QString TEXT_PRECONDITION = QString("Precondition");
const QString TEXT_DATA_TYPE = QString("DataType");
const QString TEXT_IGN = QString("SFC.Private.IGNElapsed.Elapsed");

QSharedPointer<TestCase>& TestCase::instance() {
    static QSharedPointer<TestCase> gInstance;
    if (gInstance.isNull()) {
        gInstance = QSharedPointer<TestCase>(new TestCase());
    }
    return gInstance;
}

TestCase::TestCase() {
}

void TestCase::excuteTestCase(const int& type) {
    mIntermediateDefaultJson = QJsonObject();
    mAllCaseJson = QJsonObject();
    mCaseSizeMap = QMap<QString, int>();

    switch (type) {
        case ExcuteTypeGenTC: {
            qDebug() << "excuteTestCase()";
            ivis::common::CheckTimer checkTimer;
            genCase();
            checkTimer.check("genCase()");
            if (!mAllCaseJson.empty()) {
                TestCaseWriter testCaseFile;
                testCaseFile.genTestCaseFile(mAllCaseJson);
            }
            printCaseSize(GEN_TYPE_DEFAULT);

            break;
        }
        case ExcuteTypeRunTC: {
            break;
        }
        default: {
            break;
        }
    }
    setExcuteType(type);
}

QString TestCase::genCase() {
    const int convertStart = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription;
    const int convertEnd = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax;

    for (int sheetIndex = convertStart; sheetIndex < convertEnd; ++sheetIndex) {
        if ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription) ||
            (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetPrivates) ||
            (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConfigs)) {
            // 모듈 이름정보가 필요할 경우 해당 부분에서 구현
            // Private Sheet 도 필요한 경우도 해당 부분에서 구현
            // qDebug() << "Not support sheet :" << sheetIndex;
            continue;
        }

        // Manger 내부의 data 컨테이너를 해당 sheetIndex 에 해당하는 데이터로 업데이트
        QVariantList sheetData = ControlExcel::instance().data()->getData(sheetIndex).toList();
        ExcelDataManger::instance().data()->updateExcelData(sheetIndex, sheetData);

        // Json 파일 내부에서 순서 보장을 위한 Index 할당
        int caseCnt = 0;
        int resultCnt = 0;
        int tcNameCnt = 0;

        // TCName 순회
        for (const auto& tcName : ExcelDataManger::instance().data()->isTCNameDataList(TCNAME_CHECK_OPTION_DEACTIVATE)) {
            QString genType = getGenType();  // int ExcelDataManger::isGenTypeData(const QString& tcName) 로 수정 예정
            QString vehicleType = ExcelDataManger::instance().data()->isVehicleTypeData(tcName);
            // Result 순회
            for (const auto& resultName : ExcelDataManger::instance().data()->isResultDataList(tcName)) {
                // Case 순회
                for (const auto& caseName : ExcelDataManger::instance().data()->isCaseDataList(tcName, resultName)) {
                    auto inputList = ExcelDataManger::instance().data()->isInputDataList(tcName, resultName, caseName, true);
                    if (inputList.first.isEmpty() && inputList.second.isEmpty()) {
                        // Other case 처리
                        appendOtherCaseJson(mIntermediateDefaultJson, caseName, caseCnt, resultName, resultCnt, vehicleType,
                                            tcName, tcNameCnt, sheetIndex);
                    } else {
                        // 일반 case 처리
                        appendCase(genType, caseName, caseCnt, resultName, resultCnt, vehicleType, tcName, tcNameCnt, sheetIndex);
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

// 열을 하나씩 읽는 방식의 구현
#if 0

        QVariantList sheetdata = getSheetData(sheetIndex).toList();
        // QVariantList sheetdata = ExcelData::instance().data()->getSheetData(propertyType);

        // qDebug() << "sheetdata[" << sheetIndex << "].size(): " << sheetdata.size();
        bool otherFlag = false;
        QString currentTCName;
        QString currentVehicleType;
        QString currentResult;
        QString currentCase;
        int caseCnt = 0;
        int resultCnt = 0;
        int tcNameCnt = 0;
        for (auto it = sheetdata.begin(); it != sheetdata.end(); ++it) {
            const auto& rowDataQVariant = *it;
            QStringList rowData = rowDataQVariant.toStringList();
            QString tmpTCName = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
            QString tmpVehicleType = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType));
            QString tmpResult = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
            QString tmpCase = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case));
            eraseMergeTag(tmpTCName);
            eraseMergeTag(tmpVehicleType);
            eraseMergeTag(tmpResult);
            eraseMergeTag(tmpCase);
            QString genType = getGenType();

            // 추후, isRowData 와 isInputDataInfo 로 수정 예정
            QString tmpInputSignal = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal));
            QString tmpInputData = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData));

            if (it == sheetdata.begin()) {
                currentCase = tmpCase;
                currentResult = tmpResult;
                currentVehicleType = tmpVehicleType;
                currentTCName = tmpTCName;
            }

            if (currentCase != tmpCase && (currentCase.toCaseFolded() != QString(TEXT_OTHERS).toCaseFolded() &&
                                           currentCase.toCaseFolded() != QString(TEXT_OTHERS).toCaseFolded())) {
                appendCase(genType, currentCase, caseCnt, currentResult, resultCnt, currentVehicleType, currentTCName, tcNameCnt,
                           sheetIndex);
                if (genType == GEN_TYPE_NEGATIVE_AND_POSITIVE) {
                    caseCnt++;
                }
                caseCnt++;
                currentCase = tmpCase;
            } else if (currentCase != tmpCase && (currentCase.toCaseFolded() == QString(TEXT_OTHERS).toCaseFolded() ||
                                                  currentCase.toCaseFolded() == QString(TEXT_OTHERS).toCaseFolded())) {
                if (genType == GEN_TYPE_DEFAULT) {
                    appendOtherCaseJson(mIntermediateDefaultJson, currentCase, caseCnt, currentResult, resultCnt, currentVehicleType,
                                        currentTCName, tcNameCnt, sheetIndex);
                }
                caseCnt++;
                currentCase = tmpCase;
            } else {
                // no operation
            }

            if (currentResult != tmpResult) {
                resultCnt++;
                currentResult = tmpResult;
            }

            if (currentTCName != tmpTCName) {
                tcNameCnt++;
                currentVehicleType = tmpVehicleType;
                currentTCName = tmpTCName;
            }

            if (std::next(it) == sheetdata.end() && (currentCase.toCaseFolded() != QString(TEXT_OTHERS).toCaseFolded() &&
                                                     currentCase.toCaseFolded() != QString(TEXT_OTHERS).toCaseFolded())) {
                appendCase(genType, currentCase, caseCnt, currentResult, resultCnt, currentVehicleType, currentTCName, tcNameCnt,
                           sheetIndex);
            } else if (std::next(it) == sheetdata.end() && (currentCase.toCaseFolded() == QString(TEXT_OTHERS).toCaseFolded() ||
                                                            currentCase.toCaseFolded() == QString(TEXT_OTHERS).toCaseFolded())) {
                if (genType == GEN_TYPE_DEFAULT) {
                    appendOtherCaseJson(mIntermediateDefaultJson, currentCase, caseCnt, currentResult, resultCnt, currentVehicleType,
                                        currentTCName, tcNameCnt, sheetIndex);
                }
            } else {
                // no operation
            }
        }
#endif
    }

    saveHistory();
    return "";
}

void TestCase::saveHistory() {
    // if (!mIntermediateDefaultJson.empty()) {
    //     QString _filePath = QString("%1/TcGenHistoryDefault.json").arg(ivis::common::APP_PWD());
    //     saveJsonToFile(mIntermediateDefaultJson, _filePath);
    // }
    if (!mAllCaseJson.empty()) {
        QString _filePath = QString("%1/TcGenHistory.json").arg(ivis::common::APP_PWD());
        saveJsonToFile(mAllCaseJson, _filePath);
    }
}

void TestCase::eraseMergeTag(QString& str) {
    const auto excelMergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const auto excelMergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const auto excelMerge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    QStringList eraseTaglist = {excelMergeStart, excelMergeEnd, excelMerge};
    str = ivis::common::getRemoved(str, eraseTaglist);
}

void TestCase::appendCase(const QString& genType, const QString& caseName, const int& caseNumber, const QString& resultName,
                          const int& resultNumber, const QString& vehicleType, const QString& tcName, const int& tcNameNumber,
                          const int& sheetNumber) {
    QString sendStr = getSignalInfoString(genType, sheetNumber, tcName, resultName, caseName, false);
    callPython(sendStr);
    QJsonObject caseJson = readJson();
#if 0
    qDebug() << "caseName: " << caseName;
#endif

    if (genType == GEN_TYPE_DEFAULT) {
        // Default case 추가
        QJsonObject newCaseJson = getCaseInfoJson(genType, tcName, caseJson, false);
        appendCaseJson(mAllCaseJson, newCaseJson, caseName, caseNumber, resultName, resultNumber, vehicleType, tcName,
                       tcNameNumber, sheetNumber, genType);
        // Other 연산을 위한 mIntermediateDefaultJson 에 정보 추가 (TC 생성과 무관)
        appendCaseJson(mIntermediateDefaultJson, caseJson, caseName, caseNumber, resultName, resultNumber, vehicleType, tcName,
                       tcNameNumber, sheetNumber, genType);
    } else if (genType == GEN_TYPE_NEGATIVE_AND_POSITIVE) {
        // Negative case 추가
        QJsonObject negNewCaseJson = getCaseInfoJson(GEN_TYPE_NEGATIVE, tcName, caseJson, false);
        auto negCaseName = caseName + "_Negative";
        appendCaseJson(mAllCaseJson, negNewCaseJson, negCaseName, caseNumber, resultName, resultNumber, vehicleType, tcName,
                       tcNameNumber, sheetNumber, GEN_TYPE_NEGATIVE);
        // Positive case 추가
        QJsonObject posiNewCaseJson = getCaseInfoJson(GEN_TYPE_POSITIVE, tcName, caseJson, false);
        auto posiCaseName = caseName + "_Positive";
        appendCaseJson(mAllCaseJson, posiNewCaseJson, posiCaseName, caseNumber + 1, resultName, resultNumber, vehicleType, tcName,
                       tcNameNumber, sheetNumber, GEN_TYPE_POSITIVE);
    } else if (genType == GEN_TYPE_NEGATIVE) {
        // Negative case 추가
        QJsonObject newCaseJson = getCaseInfoJson(genType, tcName, caseJson, false);
        appendCaseJson(mAllCaseJson, newCaseJson, caseName, caseNumber, resultName, resultNumber, vehicleType, tcName,
                       tcNameNumber, sheetNumber, genType);
    } else {
        // no operation
    }
}

QString TestCase::getSignalInfoString(const QString& genType, const int& sheetNum, const QString& tcName,
                                      const QString& resultName, const QString& caseName, const bool& isOther) {
    QString ret;
    // auto sigDataInfoMap =
    // ControlExcel::instance().data()->isInputSignalDataInfo(sheetNum, QStringList({tcName, resultName, caseName}), false);
    QMap<int, QPair<QString, SignalData>> sigDataInfoMap;

    if (isOther) {
        auto otherInputList = ExcelDataManger::instance().data()->isInputDataList(tcName, QString(), QString(), true);
        sigDataInfoMap = SignalDataManger::instance().data()->isOtherInputSignalDataInfo(otherInputList);
    } else {
        auto inputList = ExcelDataManger::instance().data()->isInputDataList(tcName, resultName, caseName, true);
        sigDataInfoMap = SignalDataManger::instance().data()->isTestCaseInputSignalDataInfo(inputList);
    }

    QStringList tmpList;
    tmpList << QString("TcGenType   : ") + genType;

    for (const auto& mapKey : sigDataInfoMap.keys()) {
        auto sig = sigDataInfoMap[mapKey].first;
        auto sigDataInfo = sigDataInfoMap[mapKey].second;
        tmpList << QString("InputSignalName   : ") + sig;
        tmpList << QString("InputDataType   : ") + QString::number(sigDataInfo.isDataType());
        tmpList << QString("InputKeywordType   : ") + QString::number(sigDataInfo.isKeywordType());
        tmpList << QString("InputData   : ") + sigDataInfo.isConvertData().join(", ");
        tmpList << QString("InputPrecondition   : ") + sigDataInfo.isPrecondition().join(", ");
        tmpList << QString("InputValueEnum   : ") + sigDataInfo.isValueEnum().join(", ");
        tmpList << "\n";
    }
    ret = tmpList.join("\n");
#if 0
    qDebug().noquote() << "getSignalInfoString" << Qt::endl << ret << Qt::endl;
#endif
    return ret;
}

void TestCase::callPython(const QString& str) {
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
        qDebug() << "Success(TestCase::callPython)";
    } else {
        qDebug() << "Fail(TestCase::callPython): " << log;
    }
    qDebug() << "=================================================================================================\n\n";
#endif
}

QJsonObject TestCase::readJson(const QString& filePath) {
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

void TestCase::printJson(const QJsonObject& jsonObj) {
    QJsonDocument doc(jsonObj);
    QString jsonString = doc.toJson(QJsonDocument::Indented);
    qDebug().noquote() << jsonString;
}

void TestCase::appendCaseJson(QJsonObject& fileJson, QJsonObject& caseJson, const QString& caseName, const int& caseNumber,
                              const QString& resultName, const int& resultNumber, const QString& vehicleType,
                              const QString& tcName, const int& tcNameNumber, const int& sheetNumber, const QString& genType) {
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

    QString sheetKey = QString("_%1[%2]").arg(titleSheet).arg(sheetNumber - sheetIdxStart);
    QString sheetName = sheetList[sheetNumber - sheetIdxStart];
    QString tcNameKey = QString("_%1[%2]").arg(titleTcName).arg(tcNameNumber);
    QString resultKey = QString("_%1[%2]").arg(titleResult).arg(resultNumber);
    QString caseKey = QString("_%1[%2]").arg(titleCase).arg(caseNumber);

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
QJsonArray TestCase::toJsonArray(const QList<T>& list) {
    QJsonArray jsonArray;
    for (const auto& item : list) {
        jsonArray.append(QJsonValue::fromVariant(QVariant(item)));
    }
    return jsonArray;
}

QString TestCase::getGenType() {
    // API가 만들어지면 나중에 구현 필요
    QString ret = GEN_TYPE_DEFAULT;
    // QString ret = GEN_TYPE_NEGATIVE_AND_POSITIVE;
    // QString ret = GEN_TYPE_NEGATIVE;
    return ret;
}

QJsonObject TestCase::getConfigSig(const int& sheetIdx, const QStringList& strList) {
    QJsonObject ret;
    QStringList columnTitleList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeOtherTitle).toStringList();
#if defined(USE_SHEET_COLUMN_OLD)
    QString titleConfigSigData = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Data));
#else
    QString titleConfigSigData;
#endif
#if defined(USE_CODE_BEFORE_CLASS_SPLIT)
    auto configSigMap = ControlExcel::instance().data()->isConfigSignalDataInfo(sheetIdx, strList);
#else
    QMap<QString, SignalData> configSigMap;  // = SignalDataManger::instance().data()->isConfigSignalDataInfo(sheetIdx, strList);
#endif

    for (const auto& configSigKey : configSigMap.keys()) {
        auto tmpSignalDataInfo = configSigMap[configSigKey];
        auto tmpInputDataList = tmpSignalDataInfo.isConvertData();
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

QJsonObject TestCase::getOutputSig(const int& sheetIdx, const QString& tcName, const QString& resultName) {
    QJsonObject ret;
    QStringList columnTitleList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeOtherTitle).toStringList();
    QString titleIsInitialize = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::IsInitialize));
    QString titleOutputValue = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue));
    auto outputList = ExcelDataManger::instance().data()->isOutputDataList(tcName, resultName);
    auto outputSigMap = SignalDataManger::instance().data()->isOutputSignalDataInfo(outputList);
    // auto outputSigMap = ControlExcel::instance().data()->isOutputSignalDataInfo(sheetIdx, strList);
    for (const auto& mapKey : outputSigMap.keys()) {
        auto outputSigKey = outputSigMap[mapKey].first;
        auto tmpSignalDataInfo = outputSigMap[mapKey].second;
        auto tmpInputDataList = tmpSignalDataInfo.isConvertData();
        auto tmpValueEnum = tmpSignalDataInfo.isValueEnum();
        auto tmpIsInitialize = tmpSignalDataInfo.isInitialize();
        auto tmpOutputSigKey = outputSigKey;
#if defined(USE_CODE_BEFORE_CLASS_SPLIT)
        if (tmpOutputSigKey == ControlExcel::instance().data()->isKeywordString(
                                   static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Collect))) {
            tmpOutputSigKey = TEXT_COLLECT;
        }
#else
        if (tmpOutputSigKey == ExcelUtil::instance().data()->isKeywordString(
                                   static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Collect))) {
            tmpOutputSigKey = TEXT_COLLECT;
        }
#endif
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

void TestCase::saveJsonToFile(const QJsonObject& json, const QString& filePath) {
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

void TestCase::appendOtherCaseJson(QJsonObject& fileJson, const QString& caseName, const int& caseNumber,
                                   const QString& resultName, const int& resultNumber, const QString& vehicleType,
                                   const QString& tcName, const int& tcNameNumber, const int& sheetNumber) {
    ivis::common::CheckTimer checkTimer;

    QStringList sheetList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toStringList();
    const int sheetIdxStart = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription;
    QStringList columnTitleList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeOtherTitle).toStringList();

    QString titleTcName = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
    QString titleResult = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
    QString titleCase = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case));
    QString titleSheet = TEXT_SHEET;

    QString sheetKey = QString("_%1[%2]").arg(titleSheet).arg(sheetNumber - sheetIdxStart);
    QString tcNameKey = QString("_%1[%2]").arg(titleTcName).arg(tcNameNumber);

    QString genType = getGenType();
    QString sendStr = getSignalInfoString(genType, sheetNumber, tcName, QString(), QString(), true);
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

    QJsonObject newOtherJson = getCaseInfoJson(genType, tcName, otherCase, true);
#if defined(USE_CODE_BEFORE_CLASS_SPLIT)
    newOtherJson[titleCase] =
        ControlExcel::instance().data()->isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Other));
#else
    newOtherJson[titleCase] =
        ExcelUtil::instance().data()->isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Other));
#endif
    appendCaseJson(mAllCaseJson, newOtherJson, caseName, caseNumber, resultName, resultNumber, vehicleType, tcName, tcNameNumber,
                   sheetNumber, GEN_TYPE_DEFAULT);

    checkTimer.check("appendOtherCaseJson");
}

void TestCase::removeMatchingKeys(QJsonObject& otherJson, const QJsonObject& validArray) {
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
                    qDebug() << "regexPattern: " << regexPattern;
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
                qDebug() << "No Matching key:" << key;
            }
        }
    }

    // 수정된 casesArray를 otherJson에 다시 설정
    otherJson[JSON_CASES_NAME] = casesArray;
}

QJsonObject TestCase::getCaseInfoJson(const QString& genType, const QString& tcName, const QJsonObject& caseJsonObject,
                                      const bool& isOther) {
    QJsonObject inputSignalList = caseJsonObject[TEXT_INPUT_SIGNAL_LIST].toObject();
    QJsonObject cases = caseJsonObject[JSON_CASES_NAME].toObject();
    QJsonObject newCaseJsonObject;
    QJsonArray newCases = newCaseJsonObject[JSON_CASES_NAME].toArray();
    newCaseJsonObject[TEXT_INPUT_SIGNAL_LIST] = inputSignalList;

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
            QString sigName = sigItr.key();
            // Flow keyword 판단 부분
            if (genType == GEN_TYPE_NEGATIVE) {
                if (flowIdxMap.size() > 0 && flowIdxMap.contains(sigName) == true) {
                    continue;
                } else {
                    sigIt = sigItr;
                    triggerSigIndex = triggerSigIndexOrigin;
                }
            } else {
                if (flowIdxMap.size() > 0 && flowIdxMap.contains(sigName) == false) {
                    continue;
                } else {
                    sigIt = sigItr;
                    triggerSigIndex = triggerSigIndexOrigin;
                }
            }
            // Flow keyword 판단 부분 End

            QString signalKey = sigIt.key();
            QJsonObject signalData = sigIt.value().toObject();

            if (triggerSigIndex >= caseValues.size()) {
                break;  // caseValues 범위를 벗어나지 않도록
            }

            // 현재 신호의 InputData 배열과 caseValues 비교
            QJsonArray inputDataArray = signalData[TEXT_INPUT_DATA].toArray();
            QJsonArray preconditionDataArray = signalData[TEXT_PRECONDITION].toArray();
            QString caseValue = caseValues[triggerSigIndex].toString();

            if (signalKey == TEXT_IGN) {
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
                        tag = getConfigTagStr(isOther, tcName, configIdxMap, preconditionList, triggerSigIndex,
                                              preconditionDataArray[ignIdx].toString());
                        precondition = getPreconditionStr(preconditionList);
                        input = getInputStr(signalKey, preconditionDataArray[ignIdx].toString());
                    } else {
                        tag = getConfigTagStr(isOther, tcName, configIdxMap, preconditionList, triggerSigIndex, caseValue);
                        precondition = getPreconditionStr(preconditionList, triggerSigIndex, preconditionDataArray[ignIdx]);
                        input = getInputStr(signalKey, caseValue);
                    }
                    newCases.append(QJsonValue(getTcLine(tag, precondition, input)));
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
                            tag = getConfigTagStr(isOther, tcName, configIdxMap, preconditionList, triggerSigIndex,
                                                  preconditionValue.toString());
                            precondition = getPreconditionStr(preconditionList);
                            input = getInputStr(signalKey, preconditionValue.toString());
                        } else {
                            tag = getConfigTagStr(isOther, tcName, configIdxMap, preconditionList, triggerSigIndex, caseValue);
                            precondition = getPreconditionStr(preconditionList, triggerSigIndex, preconditionValue);
                            input = getInputStr(signalKey, caseValue);
                        }
                        newCases.append(QJsonValue(getTcLine(tag, precondition, input)));
                    }
                }
            }
        }
    }
    newCaseJsonObject[JSON_CASES_NAME] = newCases;
    newCaseJsonObject[JSON_CASE_SIZE_NAME] = newCases.size();
    return newCaseJsonObject;
}

QStringList TestCase::getPreconditionList(const QJsonObject& inputSignalList, const QJsonArray& caseValues) {
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

QMap<QString, int> TestCase::getFlowKeywordIdxMap(const QJsonObject& inputSignalList) {
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

QMap<QString, int> TestCase::getConfigIdxMap(const QJsonObject& inputSignalList) {
    QMap<QString, int> configIdxMap;
    int configIdx = 0;
    for (auto sigItr = inputSignalList.begin(); sigItr != inputSignalList.end(); ++sigItr, ++configIdx) {
        QString signalName = sigItr.key();
        QJsonObject sigInfo = inputSignalList[signalName].toObject();
        if (sigInfo.contains("KeywordType")) {
            if (sigInfo["KeywordType"].toInt() == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Config)) {
                configIdxMap.insert(signalName, configIdx);
            }
        }
    }
    return configIdxMap;
}

QString TestCase::getConfigTagStr(const bool& isOther, const QString& tcName, const QMap<QString, int>& configIdxMap,
                                  const QStringList& preconditionList, const int& triggerSigIndex,
                                  const QString& triggerSigValue) {
    QMap<QString, QString> configSigValueMap;
    QString configTag = "";
    if (configIdxMap.size() == 0 || configIdxMap.size() > preconditionList.size()) {
        return configTag;
    }

    for (const auto& key : configIdxMap.keys()) {
        QString enumValue;
        QString signalName = (configIdxMap[key] == triggerSigIndex) ? (triggerSigValue) : (preconditionList[configIdxMap[key]]);
#if defined(USE_CODE_BEFORE_CLASS_SPLIT)
        enumValue = ControlExcel::instance().data()->isSignalValueEnum(key, signalName);
#else
        enumValue = SignalDataManger::instance().data()->isSignalValueEnum(key, signalName);
#endif
        configSigValueMap.insert(key, enumValue);
    }

    bool isConfigTrue = ExcelDataManger::instance().data()->isValidConfigCheck(
        isOther, ExcelDataManger::instance().data()->isConfigData(tcName), configSigValueMap);

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

QString TestCase::getPreconditionStr(const QStringList& preconditionList) {
    QStringList preconditionStrList;
    for (int caseValueIdx = 0; caseValueIdx < preconditionList.size(); caseValueIdx++) {
        preconditionStrList << preconditionList[caseValueIdx];
    }
    QString preconditionStr = preconditionStrList.join(", ");
    return preconditionStr;
}

QString TestCase::getPreconditionStr(const QStringList& preconditionList, const int& triggerSigIndex,
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

QString TestCase::getInputStr(const QString& triggerSigName, const QString& caseValue) {
    QString inputStr = QString(triggerSigName + ": " + caseValue);
    return inputStr;
}

QString TestCase::getTcLine(const QString& tag, const QString& precondition, const QString& input) {
    return QString(tag + precondition + "=>" + input);
}

void TestCase::printCaseSize(const QString& genType) {
    qDebug() << genType + QString(" Result==============================================================================");
    int sum = 0;
    int idx = 0;
    QString allCasekey;
    for (const auto& key : mCaseSizeMap.keys()) {
        sum += mCaseSizeMap[key];
        qDebug() << "Case[" << idx << "], "
                 << "Identifier: '" << key << "', Size: " << mCaseSizeMap[key] << Qt::endl;
        idx++;
    }
    qDebug() << QString("Sum of cases => ") + QString::number(sum);
    qDebug() << QString("============================================================================================") << "\n\n";
}
