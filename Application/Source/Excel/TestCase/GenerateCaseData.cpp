#include "GenerateCaseData.h"

#include "ExcelData.h"
#include "ExcelUtil.h"
#include "ExcelDataManager.h"
#include "SignalDataManager.h"
#include "CaseDataWriter.h"

const bool TCNAME_CHECK_OPTION_DEACTIVATE = false;

const QString JSON_CASES_NAME = QString("cases");
const QString JSON_NOT_TRIGGER_PRECONDITION_SET_NAME = QString("notTriggerPreconditionSet");
const QString JSON_FINAL_STATE_CASES_NAME = QString("finalStateCases");
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
const QString TEXT_INPUT_EMPTY_LIST = QString("InputEmptyList");
const QString TEXT_INPUT_SIGNAL_LIST = QString("InputSignalList");
const QString TEXT_INPUT_SIGNAL_NAME = QString("SignalName");
const QString TEXT_INPUT_DATA = QString("InputData");
const QString TEXT_PRECONDITION = QString("Precondition");
const QString TEXT_DATA_TYPE = QString("DataType");
const QString TEXT_TRIGGER_DATA = QString("TriggerData");
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
    mIntermediateDefaultJson = QJsonObject();
    mAllCaseJson = QJsonObject();
    mCaseSizeMap = QMap<QString, int>();

    qDebug() << "GenerateCaseData()";
    ivis::common::CheckTimer checkTimer;
    bool result = genCase();

    if (result) {
        checkTimer.check("genCase()");
        printCaseSize(GEN_TYPE_DEFAULT);
        if (!mAllCaseJson.empty()) {
            CaseDataWriter testCaseFile;
            QString fileName = testCaseFile.genTestCaseFile(mAllCaseJson, mTotalTestCaseCount);
            result = (fileName.size() > 0);
            qDebug() << "GenerateCaseData TC File :" << fileName;
        }
    }

    qDebug() << "excuteGenerateCaseData :" << ((result) ? ("Success") : ("Fail"));
    qDebug() << (QString(120, '*')) << "\n\n\n\n\n\n";

    return result;
}

bool GenerateCaseData::genCase() {
    ivis::common::CheckTimer checkTimer;
    const int convertStart = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription;
    const int convertEnd = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax;

    ExcelDataManager::instance().data()->reloadExcelData();

    for (int sheetIndex = convertStart; sheetIndex < convertEnd; ++sheetIndex) {
        if (isGenerateStop("genCase 1")) {
            return false;
        }

        if ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription) ||
            (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConfigs)) {
            // 모듈 이름정보가 필요할 경우 해당 부분에서 구현
            // Private Sheet 도 필요한 경우도 해당 부분에서 구현
            // qDebug() << "Not support sheet :" << sheetIndex;
            continue;
        }

        // Json 파일 내부에서 순서 보장을 위한 Index 할당
        int tcNameCnt = 0;
        int resultCnt = 0;
        int caseCnt = 0;
        // TCName 순회
        for (const auto& tcName :
             ExcelDataManager::instance().data()->isTCNameDataList(sheetIndex, TCNAME_CHECK_OPTION_DEACTIVATE)) {
            if (isGenerateStop("genCase 2")) {
                return false;
            }

            QString genType;
            ExcelDataManager::instance().data()->isGenTypeData(sheetIndex, tcName, genType);
            QString vehicleType = ExcelDataManager::instance().data()->isVehicleTypeData(sheetIndex, tcName);
            QString config = ExcelDataManager::instance().data()->isConfigData(sheetIndex, tcName);
            if (genType == GEN_TYPE_DEFAULT && sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetPrivates) {
                continue;
            }
            resultCnt = 0;
            // Result 순회
            for (const auto& resultName : ExcelDataManager::instance().data()->isResultDataList(sheetIndex, tcName)) {
                caseCnt = 0;
                // Case 순회
                for (const auto& caseName : ExcelDataManager::instance().data()->isCaseDataList(sheetIndex, tcName, resultName)) {
                    auto inputList =
                        ExcelDataManager::instance().data()->isInputDataList(sheetIndex, tcName, resultName, caseName, true);
                    if (inputList.first.isEmpty() && inputList.second.isEmpty()) {
                        if (genType == GEN_TYPE_DEFAULT) {
                            // Other case 처리
                            appendOtherCaseJson(mIntermediateDefaultJson, caseName, caseCnt, resultName, resultCnt, vehicleType,
                                                tcName, tcNameCnt, config, sheetIndex);
                        }
                        checkTimer.check("sheetIndex:tcNameCnt:resultCnt:caseCnt = " + QString::number(sheetIndex) + ":" +
                                         QString::number(tcNameCnt) + ":" + QString::number(resultCnt) + ":" +
                                         QString::number(caseCnt));
                    } else {
                        // 일반 case 처리
                        appendCase(genType, caseName, caseCnt, resultName, resultCnt, vehicleType, tcName, tcNameCnt, config,
                                   sheetIndex);
                        checkTimer.check("sheetIndex:tcNameCnt:resultCnt:caseCnt = " + QString::number(sheetIndex) + ":" +
                                         QString::number(tcNameCnt) + ":" + QString::number(resultCnt) + ":" +
                                         QString::number(caseCnt));
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
        tcNameCnt = 0;
        resultCnt = 0;
        caseCnt = 0;
        // Negative에서 생성된 case 중 동일 result 의 다른 case 의 positive 를 만족하는 것을 제거하는 부분
        // TCName 순회
        for (const auto& tcName :
             ExcelDataManager::instance().data()->isTCNameDataList(sheetIndex, TCNAME_CHECK_OPTION_DEACTIVATE)) {
            if (isGenerateStop("genCase 3")) {
                return false;
            }

            QString genType;
            ExcelDataManager::instance().data()->isGenTypeData(sheetIndex, tcName, genType);
            if (genType == GEN_TYPE_DEFAULT) {
                continue;
            }
            resultCnt = 0;
            // Result 순회
            for (const auto& resultName : ExcelDataManager::instance().data()->isResultDataList(sheetIndex, tcName)) {
                caseCnt = 0;
                // Case 순회
                for (const auto& caseName : ExcelDataManager::instance().data()->isCaseDataList(sheetIndex, tcName, resultName)) {
                    auto inputList =
                        ExcelDataManager::instance().data()->isInputDataList(sheetIndex, tcName, resultName, caseName, true);
                    if (!inputList.first.isEmpty() && !inputList.second.isEmpty() && genType == GEN_TYPE_NEGATIVE_AND_POSITIVE) {
                        caseCnt++;
                        // Other 가 아니고 GEN_TYPE_NEGATIVE_AND_POSITIVE 에서만 동작
                        auto negCaseName = caseName + " [Negative]";
                        checkNegativeAndPositive(GEN_TYPE_NEGATIVE, negCaseName, caseCnt, resultName, resultCnt, tcName,
                                                 tcNameCnt, sheetIndex);
                    }
                    checkTimer.check("checkNegativeAndPositive");
                    caseCnt++;
                }  // Case 순회 종료
                resultCnt++;
            }  // Result 순회 종료
            tcNameCnt++;
        }  // TCName 순회 종료

        // 만들어진 TC들 중에서 유효한 개수의 신호가 아닌 것들은 삭제하는 방식의 구현을 함.
        // TCName 순회
        // if (cnt != 0)
        tcNameCnt = 0;
        resultCnt = 0;
        caseCnt = 0;
        int maxTriggerCnt = ExcelUtil::instance().data()->isDescriptionValueCount();
        if (maxTriggerCnt > 0) {
            for (const auto& tcName :
                 ExcelDataManager::instance().data()->isTCNameDataList(sheetIndex, TCNAME_CHECK_OPTION_DEACTIVATE)) {
                if (isGenerateStop("genCase 4")) {
                    return false;
                }

                QString genType;
                ExcelDataManager::instance().data()->isGenTypeData(sheetIndex, tcName, genType);
                if (genType == GEN_TYPE_DEFAULT) {
                    continue;
                }
                // Result 순회
                resultCnt = 0;
                for (const auto& resultName : ExcelDataManager::instance().data()->isResultDataList(sheetIndex, tcName)) {
                    caseCnt = 0;
                    // Case 순회
                    for (const auto& caseName :
                         ExcelDataManager::instance().data()->isCaseDataList(sheetIndex, tcName, resultName)) {
                        auto inputList =
                            ExcelDataManager::instance().data()->isInputDataList(sheetIndex, tcName, resultName, caseName, true);
                        if (!inputList.first.isEmpty() || !inputList.second.isEmpty()) {
                            if (genType == GEN_TYPE_NEGATIVE_AND_POSITIVE) {
                                auto posiCaseName = caseName + " [Positive]";
                                eraseNotUsefulTC(GEN_TYPE_POSITIVE, posiCaseName, caseCnt, resultName, resultCnt, tcName,
                                                 tcNameCnt, sheetIndex, maxTriggerCnt);
                                caseCnt++;
                                auto negCaseName = caseName + " [Negative]";
                                eraseNotUsefulTC(GEN_TYPE_NEGATIVE, negCaseName, caseCnt, resultName, resultCnt, tcName,
                                                 tcNameCnt, sheetIndex, maxTriggerCnt);
                                caseCnt++;
                            } else if (genType == GEN_TYPE_POSITIVE) {
                                auto posiCaseName = caseName + " [Positive]";
                                eraseNotUsefulTC(GEN_TYPE_POSITIVE, posiCaseName, caseCnt, resultName, resultCnt, tcName,
                                                 tcNameCnt, sheetIndex, maxTriggerCnt);
                                caseCnt++;
                            } else {
                                // no operation
                            }
                        }
                        checkTimer.check("eraseNotUsefulTC");
                    }  // Case 순회 종료
                    resultCnt++;
                }  // Result 순회 종료
                tcNameCnt++;
            }  // TCName 순회 종료
        }

        // 만들어진 TC들에 input_data 가 비어있는 경우, precondition_data를 보고 쓸 데이터가 있다면 케이스의 개수를 증가시켜줌
        // "0, 0x0, 0x2, 65534, 0x0, [Empty], 0x0, 0x4 => inputA: 0x0" 에서 [Empty] 가 있으므로 해당 신호의 precondition 이 있다면
        // 다음과 같이 case 를 늘려줌
        // "0, 0x0, 0x2, 65534, 0x0, 0x0, 0x0, 0x4 => inputA: 0x0"
        // "0, 0x0, 0x2, 65534, 0x0, 0x1, 0x0, 0x4 => inputA: 0x0"
        // "0, 0x0, 0x2, 65534, 0x0, 0x2, 0x0, 0x4 => inputA: 0x0"
        tcNameCnt = 0;
        resultCnt = 0;
        caseCnt = 0;
        // TCName 순회
        for (const auto& tcName :
             ExcelDataManager::instance().data()->isTCNameDataList(sheetIndex, TCNAME_CHECK_OPTION_DEACTIVATE)) {
            if (isGenerateStop("genCase 5")) {
                return false;
            }

            QString genType;
            ExcelDataManager::instance().data()->isGenTypeData(sheetIndex, tcName, genType);
            QString vehicleType = ExcelDataManager::instance().data()->isVehicleTypeData(sheetIndex, tcName);
            QString config = ExcelDataManager::instance().data()->isConfigData(sheetIndex, tcName);
            if (genType == GEN_TYPE_DEFAULT && sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetPrivates) {
                continue;
            }
            resultCnt = 0;
            // Result 순회
            for (const auto& resultName : ExcelDataManager::instance().data()->isResultDataList(sheetIndex, tcName)) {
                caseCnt = 0;
                // Case 순회
                for (const auto& caseName : ExcelDataManager::instance().data()->isCaseDataList(sheetIndex, tcName, resultName)) {
                    auto inputList =
                        ExcelDataManager::instance().data()->isInputDataList(sheetIndex, tcName, resultName, caseName, true);
                    if (inputList.first.isEmpty() && inputList.second.isEmpty()) {
                        // Other case 처리는 필요 없음
                    } else {
                        // 일반 case 처리
                        if (genType == GEN_TYPE_DEFAULT) {
                            setPreconditionValues(GEN_TYPE_DEFAULT, caseName, caseCnt, resultName, resultCnt, tcName, tcNameCnt,
                                                  sheetIndex);
                        } else if (genType == GEN_TYPE_NEGATIVE_AND_POSITIVE) {
                            auto positiveCaseName = caseName + " [Positive]";
                            setPreconditionValues(GEN_TYPE_POSITIVE, positiveCaseName, caseCnt, resultName, resultCnt, tcName,
                                                  tcNameCnt, sheetIndex);
                            caseCnt++;
                            auto negativeCaseName = caseName + " [Negative]";
                            setPreconditionValues(GEN_TYPE_NEGATIVE, negativeCaseName, caseCnt, resultName, resultCnt, tcName,
                                                  tcNameCnt, sheetIndex);
                        } else if (genType == GEN_TYPE_POSITIVE) {
                            auto positiveCaseName = caseName + " [Positive]";
                            setPreconditionValues(GEN_TYPE_POSITIVE, positiveCaseName, caseCnt, resultName, resultCnt, tcName,
                                                  tcNameCnt, sheetIndex);
                        } else {
                            // no operation
                        }
                    }
                    checkTimer.check("setPreconditionValues");
                    caseCnt++;
                }  // Case 순회 종료
                resultCnt++;
            }  // Result 순회 종료
            tcNameCnt++;
        }  // TCName 순회 종료
    }
    saveHistory();
    return true;
}

bool GenerateCaseData::isGenerateStop(const QString& info) {
    if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfitTypeGenerateStart).toBool()) {
        return false;
    }
    qDebug() << "isGenerateStop :" << info;
    return true;
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
                                  const QString& tcName, const int& tcNameNumber, const QString& config, const int& sheetNumber) {
    QPair<QString, QString> sendStrPair = getSignalInfoString(genType, sheetNumber, tcName, resultName, caseName, false);
    QString sendStr1 = sendStrPair.first;
    callPython(sendStr1);
    QJsonObject caseJson1 = readJson();

    // QString sendStr2 = sendStrPair.second;
    // callPython(sendStr2);
    // QJsonObject caseJson2 = readJson();
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
        appendCaseJson(mIntermediateDefaultJson, caseJson1, caseName, caseNumber, resultName, resultNumber, vehicleType, tcName,
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
    if (isGenerateStop("callPython")) {
        return;
    }

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
    // inputSignalList: 시그널의 리스트와 시느널에 대한 정보들
    QJsonObject inputSignalList = caseJsonObject[TEXT_INPUT_SIGNAL_LIST].toObject();
    // preconditionOrder: 시그널이 엑셀에 입력된 순서를 나타내는 리스트, 후에 tc 를 생성하면서 해당 순서를 기반으로 순서를 재벼열
    auto preconditionOrder = caseJsonObject[TEXT_PRECONDITION_ORDER];
    // inputEmptyList: 시그널의 InputData 가 비어있으면 true 를 갖고 비어있지 않으면 false 를 갖는 배열
    auto inputEmptyList = caseJsonObject[TEXT_INPUT_EMPTY_LIST];
    // cases: 해당 case 의 itertools 결과물
    QJsonObject cases = caseJsonObject[JSON_CASES_NAME].toObject();
    // notTriggerPreconditionSet: 해당 case 에서 사용한 신호들중 notTrigger 신호들의 Precondition값의 조합들
    QJsonObject notTriggerPreconditionSet = caseJsonObject[JSON_NOT_TRIGGER_PRECONDITION_SET_NAME].toObject();

    // newCaseJsonObject: TC 파일에 입력될 정보를 저장하고 있는 Json Object
    QJsonObject newCaseJsonObject;
    // newCases: itertools 결과물을 precondition 을 이용하여 다음과 같은 형식으로 변환한 Json Object
    // 형식: "precondition_val_0, ..., precondition_val_N, => input_name_M: Input_value_M": "input_signal_idx"
    // 예시: "0x1, 0x2, 0x3, 0x4 => SigName_0: 0x2" : "0"
    QJsonObject newCases = newCaseJsonObject[JSON_CASES_NAME].toObject();
    newCaseJsonObject[TEXT_INPUT_SIGNAL_LIST] = inputSignalList;
    newCaseJsonObject[TEXT_PRECONDITION_ORDER] = preconditionOrder;
    newCaseJsonObject[TEXT_INPUT_EMPTY_LIST] = inputEmptyList;
    newCaseJsonObject[JSON_NOT_TRIGGER_PRECONDITION_SET_NAME] = notTriggerPreconditionSet;

    // finalStateCases: input 값이 적용된 후, 마지막 상태를 key 로 가지고 원래 TC line 정보를 value 로 가진다.
    // 형식: "Idx[TcCnt], final_state_val_0, ..., final_state_val_N": "tc_line"
    // 예시:
    QJsonObject finalStateCases = newCaseJsonObject[JSON_FINAL_STATE_CASES_NAME].toObject();

    QMap<QString, int> flowIdxMap;
    // other 에서는 flow keyword 판단 안함.
    if (isOther == false) {
        // flow 키워드를 타입으로 가지는 신호가 있는 index 를 map 에 모두 저장
        flowIdxMap = getFlowKeywordIdxMap(inputSignalList);
    }

    // 한 case 에서 flow 키워드가 여러개이면 TC 를 만들 수 없음
    if (flowIdxMap.size() > 1) {
        qDebug() << "Flow Keyword Error: too much flow keyword in a case...";
        return QJsonObject();
    }

    // config 키워드를 타입으로 가지는 신호가 있는 index 를 map 에 모두 저장
    QMap<QString, int> configIdxMap = getConfigIdxMap(inputSignalList);
    QMap<QString, QMap<QString, QString>> configHexEnumMap = getCongigSigHexEnumMap(inputSignalList);

    // 전체 생성된 TC 의 개수 : tcCnt
    long unsigned int tcCnt = 0;
    long unsigned int uInt32Max = (long unsigned int)-1;
    // 각 case를 처리
    for (auto it = cases.begin(); it != cases.end(); ++it) {  // cases 순회
        if (tcCnt == uInt32Max) {
            qDebug() << "Cannot generate test cases more than 2^64";
            break;
        }
        // caseKey 와 originCaseValues 는 같은 값을 나타내지만 1개는 QString 으로, 1개는 QJsonArray 로 나타냄
        QString caseKey = it.key();
        QJsonArray originCaseValues = it.value().toArray();

        // 해당 case 를 참으로 만드는 값들의 조합이 caseValues 로 할당됨
        // Default 와 positive 는 caseValues 값들중 trigger 할 값을 1개 고르고 해당 값을 input 으로 보내고 해당 값의
        // precondition 값 1개를 가져와서 precondition 으로 만듬 -> (line or 함수 써주기)
        QStringList caseValues = qJsonArrayToQStringList(originCaseValues);

        // InputSignalList에서 특정 Idx 의 신호 정보를 가져옴
        int triggerSigIndexOrigin = 0;
        // inputSignalList 순회
        for (auto sigItr = inputSignalList.begin(); sigItr != inputSignalList.end(); ++sigItr, ++triggerSigIndexOrigin) {
            int triggerSigIndex = 0;
            QJsonObject::iterator sigIt;
            QString sigIdxPadded = sigItr.key();
            // Flow keyword 판단 부분
            if (genType == GEN_TYPE_NEGATIVE) {
                // Negative 생성 시에는 해당 flow 키워드로 trigger 하는 tc 를 제외하고 모두 만들면 됨.
                if (flowIdxMap.size() > 0 && flowIdxMap.contains(sigIdxPadded) == true) {
                    continue;
                } else {
                    sigIt = sigItr;
                    triggerSigIndex = triggerSigIndexOrigin;
                }
            } else {
                // Default 와 positive 생성시에는 해당 flow 키워드로 trigger 하는 tc 만 만들면 됨.
                if (flowIdxMap.size() > 0 && flowIdxMap.contains(sigIdxPadded) == false) {
                    continue;
                } else {
                    sigIt = sigItr;
                    triggerSigIndex = triggerSigIndexOrigin;
                }
            }
            // Flow keyword 판단 부분 End

            // signalData: 시그널의 input value 와 precodition 등 상위단에서 제공된 정보를 모두 저장하고 있는 변수
            QJsonObject signalData = sigIt.value().toObject();
            QString signalName = signalData[TEXT_INPUT_SIGNAL_NAME].toString();
            QStringList inputDataArray = qJsonArrayToQStringList(signalData[TEXT_INPUT_DATA].toArray());
            QStringList preconditionDataArray = qJsonArrayToQStringList(signalData[TEXT_PRECONDITION].toArray());

            // triggerSigIndex로 접근할 때, 에러방지
            if (triggerSigIndex >= caseValues.size()) {
                break;  // caseValues 범위를 벗어나지 않도록
            }

            // caseValue: 현재 triggerSigIndex 에 대응된 시그널이 input 에서 trigger 될 때, 사용할 값을 나타냄
            // 사용 예:
            /*
            input:
                - trigger_signal_name: caseValue
            */
            QString caseValue = caseValues[triggerSigIndex];

            // IGN 신호는 다른 신호와는 다른 처리가 필요하기 때문에 분기하여 처리함.
            if (signalName == TEXT_IGN) {
                // ignIdx: inputDataArray내에 값중 몇 번째 값이 현재 caseValue 로 지정되어 있는지 나타냄
                // triggerSigIndex 와 ignIdx 는 다름
                int ignIdx = 0;
                for (const QString& value : inputDataArray) {
                    if (value == caseValue) {
                        break;
                    } else {
                        ignIdx++;
                    }
                }
                if (ignIdx < inputDataArray.size() && ignIdx < preconditionDataArray.size() && caseValue != "[Empty]") {
                    QString tag, input, precondition, tcLine, finalState;
                    if (genType == GEN_TYPE_NEGATIVE) {
                        tag = getConfigTagStr(isOther, tcName, config, configIdxMap, configHexEnumMap, caseValues,
                                              triggerSigIndex, preconditionDataArray[ignIdx]);
                        precondition = getPreconditionStr(caseValues);
                        input = getInputStr(signalName, preconditionDataArray[ignIdx]);
                        finalState = getFinalStateStr(caseValues, triggerSigIndex, preconditionDataArray[ignIdx]);
                    } else {
                        tag = getConfigTagStr(isOther, tcName, config, configIdxMap, configHexEnumMap, caseValues,
                                              triggerSigIndex, caseValue);
                        precondition = getPreconditionStr(caseValues, triggerSigIndex, preconditionDataArray[ignIdx]);
                        input = getInputStr(signalName, caseValue);
                        finalState = getFinalStateStr(caseValues);
                    }
                    tcLine = getTcLine(tag, precondition, input);
                    newCases.insert(tcLine, QString::number(triggerSigIndex));
                    if (genType != GEN_TYPE_DEFAULT) {
                        finalStateCases.insert(getFinalStateLine(tcCnt, finalState), tcLine);
                    }
                    tcCnt++;
                } else {
                    qDebug() << "IGN Index Error";
                }
            } else {
                QStringList loopArray = preconditionDataArray;
                if (isOther) {
                    loopArray = inputDataArray;
                }
                // loopArray 를 순회하면서 Precondition Str 을 만들어냄
                for (const QString& preconditionValue : loopArray) {  // loopArray 순회
                    if (preconditionValue != caseValue && caseValue != "[Empty]" && preconditionValue != "[Empty]") {
                        QString tag, input, precondition, tcLine, finalState;
                        if (genType == GEN_TYPE_NEGATIVE) {
                            tag = getConfigTagStr(isOther, tcName, config, configIdxMap, configHexEnumMap, caseValues,
                                                  triggerSigIndex, preconditionValue);
                            precondition = getPreconditionStr(caseValues);
                            input = getInputStr(signalName, preconditionValue);
                            finalState = getFinalStateStr(caseValues, triggerSigIndex, preconditionValue);
                        } else {
                            tag = getConfigTagStr(isOther, tcName, config, configIdxMap, configHexEnumMap, caseValues,
                                                  triggerSigIndex, caseValue);
                            precondition = getPreconditionStr(caseValues, triggerSigIndex, preconditionValue);
                            input = getInputStr(signalName, caseValue);
                            finalState = getFinalStateStr(caseValues);
                        }
                        tcLine = getTcLine(tag, precondition, input);
                        newCases.insert(tcLine, QString::number(triggerSigIndex));
                        if (genType != GEN_TYPE_DEFAULT) {
                            finalStateCases.insert(getFinalStateLine(tcCnt, finalState), tcLine);
                        }
                        tcCnt++;
                    }
                }  // loopArray 순회
            }
        }  // inputSignalList 순회 End
    }      // cases 순회 End

    // 해당 case에서 사용했던 모든 신호를 precondition 값으로 초기화해줌
    if (genType == GEN_TYPE_NEGATIVE || genType == GEN_TYPE_POSITIVE) {
        QString initStr = getInitStr(inputSignalList);
        QJsonObject initJson = newCaseJsonObject[JSON_INIT_CASE_NAME].toObject();
        initJson[initStr] = "0";
        newCaseJsonObject[JSON_INIT_CASE_NAME] = initJson;
    }

    // 생성된 Cases를 새로운 Object 에 삽입
    newCaseJsonObject[JSON_CASES_NAME] = newCases;
    newCaseJsonObject[JSON_CASE_SIZE_NAME] = newCases.size();
    newCaseJsonObject[JSON_FINAL_STATE_CASES_NAME] = finalStateCases;
    return newCaseJsonObject;
}

QStringList GenerateCaseData::qJsonArrayToQStringList(const QJsonArray& jsonArray) {
    QStringList stringList;
    stringList.reserve(jsonArray.size());
    for (const QJsonValue& value : jsonArray) {
        stringList.append(value.toString());
    }
    return stringList;
}

QMap<QString, int> GenerateCaseData::getFlowKeywordIdxMap(const QJsonObject& inputSignalList) {
    QMap<QString, int> flowIdxMap;
    int flowIdx = 0;
    for (auto sigItr = inputSignalList.begin(); sigItr != inputSignalList.end(); ++sigItr, ++flowIdx) {
        QString sigIdxPadded = sigItr.key();
        QJsonObject sigInfo = inputSignalList[sigIdxPadded].toObject();
        if (sigInfo.contains("KeywordType")) {
            if (sigInfo["KeywordType"].toInt() == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Flow)) {
                flowIdxMap.insert(sigIdxPadded, flowIdx);
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
                QJsonObject tmpJson = sigInfo["ValueEnum"].toObject();
                for (const QString& key : tmpJson.keys()) {
                    tmpMap.insert(key, tmpJson.value(key).toString());
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
    return configTag;
}

QStringList GenerateCaseData::getReplaceValueAtList(const QStringList& originList, const int& idx, const QString& value) {
    QStringList ret;
    for (int i = 0; i < originList.size(); i++) {
        if (i != idx) {
            ret << originList[i];
        } else {
            ret << value;
        }
    }
    return ret;
}

QString GenerateCaseData::getPreconditionStr(const QStringList& preconditionList, const int& triggerSigIndex,
                                             const QString& preconditionValue) {
    QString preconditionStr = getReplaceValueAtList(preconditionList, triggerSigIndex, preconditionValue).join(", ");
    return preconditionStr;
}

QString GenerateCaseData::getFinalStateStr(const QStringList& preconditionList, const int& triggerSigIndex,
                                           const QString& inputValue) {
    QString finalStateStr = getReplaceValueAtList(preconditionList, triggerSigIndex, inputValue).join(", ");
    return finalStateStr;
}

QString GenerateCaseData::getInputStr(const QString& triggerSigName, const QString& caseValue) {
    QString inputStr = QString(triggerSigName + ": " + caseValue);
    return inputStr;
}

QString GenerateCaseData::getTcLine(const QString& tag, const QString& precondition, const QString& input) {
    return QString(tag + precondition + ", =>" + input);
}

QString GenerateCaseData::getFinalStateLine(const long long unsigned int& cnt, const QString& finalStateValues) {
    QString cntStr = QString::number(cnt, 10).rightJustified(10, '0');
    return QString("Idx[" + cntStr + "], " + finalStateValues);
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
                        if (_caseJson.contains(JSON_FINAL_STATE_CASES_NAME) && _caseJson[titleGenType] == GEN_TYPE_POSITIVE) {
                            // qDebug() << "_caseJson: " << _caseJson["Case"];
                            QJsonObject positiveFinalStateCases = _caseJson[JSON_FINAL_STATE_CASES_NAME].toObject();
                            QJsonArray inputEmptyList = _caseJson[TEXT_INPUT_EMPTY_LIST].toArray();
                            removeMatchingKeysNegative(caseJson, positiveFinalStateCases, inputEmptyList);
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

void GenerateCaseData::removeMatchingKeysNegative(QJsonObject& caseJson, const QJsonObject& positiveFinalStateCases,
                                                  const QJsonArray& inputEmptyList) {
    // Cases 객체 가져오기
    QJsonObject casesArray = caseJson[JSON_CASES_NAME].toObject();
    QJsonObject finalStateArray = caseJson[JSON_FINAL_STATE_CASES_NAME].toObject();
    QStringList inputEmptyListStr = qJsonArrayToQStringList(inputEmptyList);
    inputEmptyListStr.prepend("");

    for (const QString& caseKey : finalStateArray.keys()) {
        if (casesArray.contains(finalStateArray[caseKey].toString()) == false) {
            continue;
        }
        QStringList keyParts = caseKey.split(", ");
        QString pattern = "^";
        bool isWildcardPartExist = false;

        if (inputEmptyListStr.size() != keyParts.size()) {
            qDebug() << "Size error!(" << inputEmptyListStr.size() << "!=" << keyParts.size() << ")";
            return;
        } else if (keyParts.size() == 0) {
            qDebug() << "Size error!(keyParts1.size() == 0)";
            return;
        } else {
            // no operation
        }

        // 각 부분이 비어있다면 정규표현식에서 와일드카드로 매칭되도록 함
        for (int i = 0; i < keyParts.size(); i++) {
            if (keyParts[i] == TEXT_EMPTY || inputEmptyListStr[i] == "EmptyInputSignal") {
                // if (keyParts[i] != TEXT_EMPTY && inputEmptyListStr[i] == "EmptyInputSignal") {
                //     qDebug() << "inputEmptyListStr[i]: " << inputEmptyListStr[i];
                // }
                pattern += "[^,]+, ";  // 와일드카드 처리 (콤마로 구분되는 모든 값을 포함)
                isWildcardPartExist = true;
            } else if (keyParts[i].startsWith("Idx[") && keyParts[i].endsWith("]")) {
                // index 를 나태내는 부분도 와일드카드오 매칭되도록 함
                pattern += "\\bIdx\\[[A-Fa-f0-9]+\\], ";  // "Idx[00000000000000000000]" 와일드카드 패턴
                isWildcardPartExist = true;
            } else {
                pattern += QRegularExpression::escape(keyParts[i]) + ", ";
            }
        }
        // 마지막 콤마 제거 후 끝에 $ 추가
        pattern.chop(2);
        pattern += "$";

        // 정규표현식으로 패턴 생성
        QRegularExpression regexPattern(pattern);

        if (isWildcardPartExist == false) {
            qDebug() << "Undetermined format";
            return;
        }

        for (const QString& positiveKey : positiveFinalStateCases.keys()) {
            if (regexPattern.match(positiveKey).hasMatch()) {
                QString value = finalStateArray[caseKey].toString();
                if (casesArray.contains(value)) {
                    casesArray.remove(value);
                    break;
                }
            }
        }
    }

    // 수정된 casesArray를 caseJson에 다시 설정
    caseJson[JSON_CASES_NAME] = casesArray;
}

void GenerateCaseData::eraseNotUsefulTC(const QString& genType, const QString& caseName, const int& caseNumber,
                                        const QString& resultName, const int& resultNumber, const QString& tcName,
                                        const int& tcNameNumber, const int& sheetNumber, const int& maxTriggerCnt) {
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
        qDebug() << "No TCName Error! " << tcNameKey;
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

    if (caseJson.contains(titleGenType) && caseJson.contains(JSON_CASES_NAME)) {
        removeTcByCnt(caseJson, titleGenType, maxTriggerCnt);
        // 개수 변경 반영
        if (caseJson[titleGenType] == GEN_TYPE_POSITIVE || caseJson[titleGenType] == GEN_TYPE_NEGATIVE) {
            if (caseJson.contains(JSON_CASE_SIZE_NAME) &&
                caseJson[JSON_CASE_SIZE_NAME].toInt() != caseJson[JSON_CASES_NAME].toObject().size()) {
                caseJson[JSON_CASE_SIZE_NAME] = caseJson[JSON_CASES_NAME].toObject().size();
                QStringList nameIdentifierList;
                nameIdentifierList << sheetName << tcName << genType << resultName << caseName;
                QString nameIdentifier = nameIdentifierList.join("@");
                mCaseSizeMap[nameIdentifier] = caseJson[JSON_CASE_SIZE_NAME].toInt();
            }
        }
    }

    if (caseJson.contains(JSON_FINAL_STATE_CASES_NAME)) {
        caseJson.remove(JSON_FINAL_STATE_CASES_NAME);
    }

    // 변경된 JsonObject를 원래 object로 재할당
    resultJson[caseKey] = caseJson;

    // Update the JSON objects back into the hierarchy
    tcNameJson[resultKey] = resultJson;
    sheetJson[tcNameKey] = tcNameJson;
    mAllCaseJson[sheetKey] = sheetJson;
}

void GenerateCaseData::removeTcByCnt(QJsonObject& caseJson, const QString& titleGenType, const int& maxTriggerCnt) {
    // Cases 객체 가져오기
    QJsonObject casesArray = caseJson[JSON_CASES_NAME].toObject();
    QJsonObject inputSignalList = caseJson[TEXT_INPUT_SIGNAL_LIST].toObject();
    QString combinationListName = "";
    if (caseJson[titleGenType] == GEN_TYPE_POSITIVE) {
        combinationListName = TEXT_INPUT_DATA;
    } else if (caseJson[titleGenType] == GEN_TYPE_NEGATIVE) {
        combinationListName = TEXT_PRECONDITION;
    } else {
        qDebug() << "Not valid genType: " << caseJson[titleGenType].toString();
        return;
    }

    for (auto sigItr = inputSignalList.begin(); sigItr != inputSignalList.end(); ++sigItr) {
        QString sigIdx = sigItr.key();
        QJsonObject sigInfo = inputSignalList[sigIdx].toObject();
        sigInfo[TEXT_TRIGGER_DATA] = copyQJsonArrayUpToIndex(sigInfo[combinationListName].toArray(), maxTriggerCnt);
        inputSignalList[sigIdx] = sigInfo;
    }
    caseJson[TEXT_INPUT_SIGNAL_LIST] = inputSignalList;

    // cases 를 돈다
    for (const QString& caseKey : casesArray.keys()) {
        // 어 해당 케이스에서 trigger 하는 신호가 몇번째 신호인지 파악
        int triggerSigIdx = casesArray[caseKey].toString().toInt();
        // 신호 리스트를 돔
        for (const QString& sigIdx2 : inputSignalList.keys()) {
            // 신호 index 가 trigger 하는 신호의 index 와 다르면 걍 continue
            if (triggerSigIdx != sigIdx2.toInt()) {
                continue;
            }
            // 신호 index 가 trigger 하는 신호의 index 와 같으면 확인
            QJsonObject sigInfo2 = inputSignalList[sigIdx2].toObject();
            if (sigInfo2.contains(combinationListName) && sigInfo2.contains(TEXT_TRIGGER_DATA) &&
                sigInfo2[combinationListName].toArray().size() <= sigInfo2[TEXT_TRIGGER_DATA].toArray().size()) {
                // 조합을 생성한 list 의 크기가 내가 원하는 maxTriggerCnt 보다 작으면 다음 key 로 넘어감
                break;
            } else {
                // key 에서 trigger 한 값을 찾음
                QStringList parts = caseKey.split("=>");
                QString triggerValue = "";
                if (parts.size() > 1) {
                    QString valuePart = parts.at(1).trimmed();
                    QStringList parts2 = valuePart.split(":");
                    if (parts2.size() > 1) {
                        triggerValue = parts2.at(1).trimmed();
                    } else {
                        qDebug() << "Size error1: removeTcByCnt";
                        break;
                    }
                } else {
                    qDebug() << "Size error2: removeTcByCnt";
                    break;
                }

                if (triggerValue != "" && sigInfo2[TEXT_TRIGGER_DATA].toArray().contains(QJsonValue(triggerValue)) == false) {
                    casesArray.remove(caseKey);
                }
            }
        }
    }

    // 수정된 casesArray를 caseJson에 다시 설정
    caseJson[JSON_CASES_NAME] = casesArray;
}

QJsonArray GenerateCaseData::copyQJsonArrayUpToIndex(const QJsonArray& array, const int& maxCnt) {
    QJsonArray jsonArray;
    for (int i = 0; i < array.size(); ++i) {
        if (i < maxCnt) {
            jsonArray.append(array.at(i));
        } else {
            break;
        }
    }
    return jsonArray;
}

void GenerateCaseData::setPreconditionValues(const QString& genType, const QString& caseName, const int& caseNumber,
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
        qDebug() << "No TCName Error! " << tcNameKey;
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

    if (caseJson.contains(titleGenType) && caseJson.contains(JSON_CASES_NAME) &&
        caseJson.contains(JSON_NOT_TRIGGER_PRECONDITION_SET_NAME)) {
        applyPrecondition(caseJson);
        // 개수 변경 반영
        if (caseJson.contains(JSON_CASE_SIZE_NAME) &&
            caseJson[JSON_CASE_SIZE_NAME].toInt() != caseJson[JSON_CASES_NAME].toObject().size()) {
            caseJson[JSON_CASE_SIZE_NAME] = caseJson[JSON_CASES_NAME].toObject().size();
            QStringList nameIdentifierList;
            nameIdentifierList << sheetName << tcName << genType << resultName << caseName;
            QString nameIdentifier = nameIdentifierList.join("@");
            mCaseSizeMap[nameIdentifier] = caseJson[JSON_CASE_SIZE_NAME].toInt();
        }
    }

    // 변경된 JsonObject를 원래 object로 재할당
    resultJson[caseKey] = caseJson;

    // Update the JSON objects back into the hierarchy
    tcNameJson[resultKey] = resultJson;
    sheetJson[tcNameKey] = tcNameJson;
    mAllCaseJson[sheetKey] = sheetJson;
}

void GenerateCaseData::applyPrecondition(QJsonObject& caseJson) {
    // Cases 객체 가져오기
    QJsonObject casesArray = caseJson[JSON_CASES_NAME].toObject();
    // QJsonObject inputSignalList = caseJson[TEXT_INPUT_SIGNAL_LIST].toObject();
    QJsonObject notTriggerPreconditionArray = caseJson[JSON_NOT_TRIGGER_PRECONDITION_SET_NAME].toObject();
    if (notTriggerPreconditionArray.size() == 0) {
        return;
    }

    QJsonObject newCases;

    for (const QString& caseKey : casesArray.keys()) {
        auto itr = notTriggerPreconditionArray.begin();
        QString triggerIdx = casesArray[caseKey].toString();
        QStringList parts = caseKey.split("=>");
        QStringList preconditionListOrigin;
        QString configInfo = "";
        QString triggerInfo = "";
        if (parts.size() > 1) {
            QString keyPart = parts.at(0).trimmed();
            triggerInfo = parts.at(1);
            QStringList parts2 = keyPart.split(",");
            for (QString& item : parts2) {
                item = item.trimmed();
            }
            if (parts2.size() > 1) {
                QString firstValue = parts2.at(0);
                if (firstValue.contains("[config:")) {
                    configInfo = firstValue;
                    preconditionListOrigin = parts2.mid(1, parts2.size() - 2);
                } else {
                    preconditionListOrigin = parts2.mid(0, parts2.size() - 1);
                }
            } else {
                qDebug() << "Size error1: applyPrecondition";
                break;
            }
        } else {
            qDebug() << "Size error2: applyPrecondition";
            break;
        }

        // 현재는 1개만 돌리기 위해서 itr == notTriggerPreconditionArray.begin() 가 추가됨.
        // 만일 모든 precondition을 다 적용하고 싶다면 itr != notTriggerPreconditionArray.end() 를 추가하고
        // itr == notTriggerPreconditionArray.begin() 을 삭제하면 됨.
        while (itr == notTriggerPreconditionArray.begin()) {
            if (notTriggerPreconditionArray.isEmpty() == true) {
                break;
            }
            QStringList preconditionSet;
            QStringList notTriggerPreconditionSet = qJsonArrayToQStringList(itr.value().toArray());
            if (notTriggerPreconditionSet.size() != preconditionListOrigin.size()) {
                qDebug() << "Size error3: applyPrecondition";
                qDebug() << "notTriggerPreconditionSet.size(): " << notTriggerPreconditionSet.size();
                qDebug() << "notTriggerPreconditionSet: " << notTriggerPreconditionSet;
                qDebug() << "preconditionListOrigin.size(): " << preconditionListOrigin.size();
                qDebug() << "preconditionListOrigin: " << preconditionListOrigin;
                break;
            }

            for (int i = 0; i < notTriggerPreconditionSet.size(); i++) {
                if (preconditionListOrigin[i] != TEXT_EMPTY) {
                    preconditionSet.append(preconditionListOrigin[i]);
                } else {
                    if (notTriggerPreconditionSet[i] != TEXT_EMPTY) {
                        preconditionSet.append(notTriggerPreconditionSet[i]);
                    } else {
                        preconditionSet.append(TEXT_EMPTY);
                    }
                }
            }
            preconditionSet.append("=>");
            if (configInfo != "") {
                preconditionSet.prepend(configInfo);
            }
            QString newKey = preconditionSet.join(", ") + triggerInfo;
            // qDebug() << "newKey: " << newKey;
            newCases[newKey] = triggerIdx;
            itr++;
        }
    }

    if (caseJson.contains(JSON_NOT_TRIGGER_PRECONDITION_SET_NAME)) {
        caseJson.remove(JSON_NOT_TRIGGER_PRECONDITION_SET_NAME);
    }

    caseJson[JSON_CASES_NAME] = newCases;
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
        qDebug().nospace() << "Case[" << QString::number(idx).rightJustified(2, ' ').toLatin1().data()
                           << "] : " << QString::number(mCaseSizeMap[key]).rightJustified(5, ' ').toLatin1().data() << ",  "
                           << key.toLatin1().data();
        idx++;
    }
    mTotalTestCaseCount = sum;

    qDebug() << (QString(120, '-').toLatin1().data());
    qDebug() << (QString("Sum of cases => ") + QString::number(sum)).toLatin1().data();
    qDebug() << (QString(120, '=').toLatin1().data());
    qDebug() << "\n\n\033[0m";
}
