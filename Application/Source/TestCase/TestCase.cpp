#include "TestCase.h"
#include "TestCaseWriter.h"

const QString JSON_CASES_NAME = QString("cases");
const QString JSON_CASE_SIZE_NAME = QString("CaseSize");
const QString JSON_ALL_CASE_SIZE_NAME = QString("AllCaseSize");
const QString JSON_OTHER_CASE_SIZE_NAME = QString("OtherCaseSize");

const QString GEN_TYPE_DEFAULT = QString("Default");
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
    // if (getExcuteType() == type) {
    //     qDebug() << "Running excute tc type :" << type;
    //     return;
    // }

    switch (type) {
        case ExcuteTypeGenTC: {
            qDebug() << "excuteTestCase()";
            genCase();
            if (!mDefaultFileJson.empty()) {
                genTestCaseFile(mDefaultFileJson);
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
        if (sheetIndex == convertStart || sheetIndex == convertStart + 1) {
            // 모듈 이름정보가 필요할 경우 해당 부분에서 구현
            // Private Sheet 도 필요한 경우도 해당 부분에서 구현
            continue;
        }
        if ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription) ||
            (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConfigs)) {
            qDebug() << "Not support sheet :" << sheetIndex;
            continue;
        }
// API를 사용한 구현
#if 0
        auto sheetDataList = ControlExcel::instance().data()->isSheetConfigurationInfo(sheetIndex).isDataInfo();

        QString currentTCName = "";
        QString currentResultName = "";
        QString sendStr = "";
        int caseCnt = 0;
        int resultCnt = 0;
        int tcNameCnt = 0;
        QString currentVehicleType = "";

        for (auto it = sheetDataList.cbegin(); it != sheetDataList.cend(); ++it) {
            const auto& sheetData = *it;
            QString tcName = sheetData.first;
            QString result = sheetData.second.first;
            QStringList caseList = sheetData.second.second;

            if (it == sheetDataList.cbegin()) {
                currentTCName = tcName;
                currentResultName = result;
            } else {
                if (currentTCName != tcName) {
                    currentTCName = tcName;
                    tcNameCnt++;
                }

                if (currentResultName != result) {
                    currentResultName = result;
                    resultCnt++;
                }
            }

            qDebug() << "TCName:Result:Case == " << tcName << result << caseList;

            for (const auto& singleCase : caseList) {
                // add case string to buf
                qDebug() << "caseName: " << singleCase;
                if (getGenType() == GEN_TYPE_DEFAULT) {
                    if (singleCase != TEXT_OTHERS || singleCase != TEXT_OTHERS) {
                        QString sendStr = getSignalInfoString(genType, sheetIndex, tcName, result, singleCase);
                        callPython(sendStr);
                        QJsonObject caseJson = readJson();
                        appendCaseJson(mFileJson, caseJson, singleCase, caseCnt, result, resultCnt, currentVehicleType, tcName,
                                       tcNameCnt, sheetIndex);
                        caseCnt++;
                    }
                } else {
                    // QString sendStr = getSignalInfoString(genType, sheetIndex, tcName, result, singleCase);
                }
            }

            if (tcName != currentTCName || it + 1 == sheetDataList.cend()) {
                // send buf to python
            }
        }
#endif

// 열을 하나씩 읽는 방식의 구현
#if 1

        QVariantList sheetdata = getSheetData(sheetIndex).toList();
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
                currentTCName = tmpTCName;
            }

            if (currentCase != tmpCase && (currentCase.toCaseFolded() != QString(TEXT_OTHERS).toCaseFolded() &&
                                           currentCase.toCaseFolded() != QString(TEXT_OTHERS).toCaseFolded())) {
                appendCase(genType, currentCase, caseCnt, currentResult, resultCnt, currentVehicleType, currentTCName, tcNameCnt,
                           sheetIndex);
                caseCnt++;
                currentCase = tmpCase;
            } else if (currentCase != tmpCase && (currentCase.toCaseFolded() == QString(TEXT_OTHERS).toCaseFolded() ||
                                                  currentCase.toCaseFolded() == QString(TEXT_OTHERS).toCaseFolded())) {
                if (genType == GEN_TYPE_DEFAULT) {
                    appendOtherCaseJson(mDefaultFileJson, currentCase, caseCnt, currentResult, resultCnt, currentVehicleType,
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
                currentTCName = tmpTCName;
            }

            if (std::next(it) == sheetdata.end() && (currentCase.toCaseFolded() != QString(TEXT_OTHERS).toCaseFolded() &&
                                                     currentCase.toCaseFolded() != QString(TEXT_OTHERS).toCaseFolded())) {
                appendCase(genType, currentCase, caseCnt, currentResult, resultCnt, currentVehicleType, currentTCName, tcNameCnt,
                           sheetIndex);
            } else if (std::next(it) == sheetdata.end() && (currentCase.toCaseFolded() == QString(TEXT_OTHERS).toCaseFolded() ||
                                                            currentCase.toCaseFolded() == QString(TEXT_OTHERS).toCaseFolded())) {
                if (genType == GEN_TYPE_DEFAULT) {
                    appendOtherCaseJson(mDefaultFileJson, currentCase, caseCnt, currentResult, resultCnt, currentVehicleType,
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
    if (!mDefaultFileJson.empty()) {
        QString _filePath = QString("%1/TcGenHistoryDefault.json").arg(ivis::common::APP_PWD());
        saveJsonToFile(mDefaultFileJson, _filePath);
        mAllFileJson.insert(GEN_TYPE_DEFAULT, mDefaultFileJson);
    }

    if (!mNegativeFileJson.empty()) {
        QString _negFilePath = QString("%1/TcGenHistoryNeg.json").arg(ivis::common::APP_PWD());
        saveJsonToFile(mNegativeFileJson, _negFilePath);
        mAllFileJson.insert(GEN_TYPE_NEGATIVE, mNegativeFileJson);
    }

    if (!mPositiveFileJson.empty()) {
        QString _posiFilePath = QString("%1/TcGenHistoryPosi.json").arg(ivis::common::APP_PWD());
        saveJsonToFile(mPositiveFileJson, _posiFilePath);
        mAllFileJson.insert(GEN_TYPE_POSITIVE, mPositiveFileJson);
    }

    if (!mAllFileJson.empty()) {
        QString _filePath = QString("%1/TcGenHistory.json").arg(ivis::common::APP_PWD());
        saveJsonToFile(mAllFileJson, _filePath);
    }
}

void TestCase::printCase(const QMap<QString, QMap<QString, QString>>& tmpSignalMap) {
    for (const auto& TCName_Result_Case : tmpSignalMap.keys()) {
        qDebug() << "TC Key: " << TCName_Result_Case;
        for (const auto& inputSig : tmpSignalMap[TCName_Result_Case].keys()) {
            qDebug() << "\tinputSig (Key, Value): "
                     << "(" << inputSig << ", " << tmpSignalMap[TCName_Result_Case][inputSig] << ")";
        }
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
    QString sendStr = getSignalInfoString(genType, sheetNumber, tcName, resultName, caseName);
    callPython(sendStr);
    QJsonObject caseJson = readJson();
    if (genType == GEN_TYPE_DEFAULT) {
        appendCaseJson(mDefaultFileJson, caseJson, caseName, caseNumber, resultName, resultNumber, vehicleType, tcName,
                       tcNameNumber, sheetNumber, genType);
    } else if (genType == GEN_TYPE_NEGATIVE) {
        appendCaseJson(mNegativeFileJson, caseJson, caseName, caseNumber, resultName, resultNumber, vehicleType, tcName,
                       tcNameNumber, sheetNumber, genType);
    } else if (genType == GEN_TYPE_POSITIVE) {
        appendCaseJson(mPositiveFileJson, caseJson, caseName, caseNumber, resultName, resultNumber, vehicleType, tcName,
                       tcNameNumber, sheetNumber, genType);
    } else {
        // No operation
    }
}

QString TestCase::getSignalInfoString(const QString& genType, const int& sheetNum, const QString& tcName,
                                      const QString& resultName, const QString& caseName) {
    QString ret;
    auto sigDataInfoMap =
        ControlExcel::instance().data()->isInputSignalDataInfo(sheetNum, QStringList({tcName, resultName, caseName}), false);

    ret += QString("TcGenType   : ") + genType + QString("\n");

    for (const auto& sig : sigDataInfoMap.keys()) {
        auto sigDataInfo = sigDataInfoMap[sig];
        ret += QString("InputSignalName   : ") + sig + QString("\n");
        ret += QString("InputDataType   : ") + QString::number(sigDataInfo.isDataType()) + QString("\n");
        ret += QString("InputKeywordType   : ") + QString::number(sigDataInfo.isKeywordType()) + QString("\n");
        // if (resultName == "" && caseName == "" &&
        //     sigDataInfo.isDataType() == static_cast<int>(ivis::common::DataTypeEnum::DataType::HUInt64) &&
        //     sigDataInfo.isValueEnum().isEmpty() == false) {
        //     QStringList allEnumList;
        //     for (const auto& enumString : sigDataInfo.isValueEnum()) {
        //         QStringList enumSplitList = enumString.split(":");
        //         for (const auto& enumSplit : enumSplitList) {
        //             if (enumSplit.contains("0x") == false) {
        //                 allEnumList.append(enumSplit);
        //                 break;
        //             }
        //         }
        //     }
        //     ret += QString("InputData   : ") + allEnumList.join(", ") + QString("\n");
        // } else {
        //     ret += QString("InputData   : ") + sigDataInfo.isConvertData().join(", ") + QString("\n");
        // }
        ret += QString("InputData   : ") + sigDataInfo.isConvertData().join(", ") + QString("\n");
        ret += QString("InputPrecondition   : ") + sigDataInfo.isPrecondition().join(", ") + QString("\n");
        ret += QString("InputValueEnum   : ") + sigDataInfo.isValueEnum().join(", ") + QString("\n");
        ret += "\n\n";
    }
#if 0
    qDebug() << "getSignalInfoString: " << ret << Qt::endl;
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
    QString titleGenType = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::NegativeTest));
    QString titleConfigSig = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::ConfigSignal));
    QString titleConfigSigData = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Data));
    QString titleResult = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
    QString titleOutputSignal = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal));
    QString titleIsInitialize = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::IsInitialize));
    QString titleOutputValue = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue));
    QString titleCase = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case));
    QString titleSheet = TEXT_SHEET;

    QString sheetKey = QString("%1[%2]").arg(titleSheet).arg(sheetNumber - sheetIdxStart);
    QString sheetName = sheetList[sheetNumber - sheetIdxStart];
    QString tcNameKey = QString("%1[%2]").arg(titleTcName).arg(tcNameNumber);
    QString resultKey = QString("%1[%2]").arg(titleResult).arg(resultNumber);
    QString caseKey = QString("%1[%2]").arg(titleCase).arg(caseNumber);

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
        tcNameJson[titleGenType] = genType;
        if (!tcNameJson.contains(titleConfigSig)) {
            tcNameJson[titleConfigSig] = getConfigSig(sheetNumber, QStringList({tcName, "", ""}));
        }
        // tcNameJson[titleConfigSigData] = getConfigData(sheetNumber, QStringList({tcName, "", ""}));
    }

    // 3. Result 확인
    if (!tcNameJson.contains(resultKey)) {
        tcNameJson[resultKey] = QJsonObject();
    }
    QJsonObject resultJson = tcNameJson[resultKey].toObject();
    if (!resultJson.contains(titleResult)) {
        resultJson[titleResult] = resultName;
        if (!resultJson.contains(titleOutputSignal)) {
            resultJson[titleOutputSignal] = getOutputSig(sheetNumber, QStringList({tcName, resultName, ""}));
        }
    }

    // 4. Case 확인
    if (!caseJson.contains(titleCase)) {
        caseJson[titleCase] = caseName;
    }

    if (caseJson.contains(JSON_ALL_CASE_SIZE_NAME) && caseJson.contains(JSON_OTHER_CASE_SIZE_NAME)) {
        mCaseSizeMap.insert(QString(genType + ":" + sheetKey + ":" + tcNameKey + ":" + JSON_ALL_CASE_SIZE_NAME),
                            caseJson[JSON_ALL_CASE_SIZE_NAME].toInt());
        mCaseSizeMap.insert(QString(genType + ":" + sheetKey + ":" + tcNameKey + ":" + JSON_OTHER_CASE_SIZE_NAME),
                            caseJson[JSON_OTHER_CASE_SIZE_NAME].toInt());
    } else {
        if (caseJson.contains(JSON_CASE_SIZE_NAME)) {
            mCaseSizeMap.insert(QString(genType + ":" + sheetKey + ":" + tcNameKey + ":" + resultKey + ":" + caseKey),
                                caseJson[JSON_CASE_SIZE_NAME].toInt());
        }
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
    // QString ret = GEN_TYPE_NEGATIVE;
    return ret;
}

QJsonObject TestCase::getConfigSig(const int& sheetIdx, const QStringList& strList) {
    QJsonObject ret;
    QStringList columnTitleList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeOtherTitle).toStringList();
    QString titleConfigSigData = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Data));
    auto configSigMap = ControlExcel::instance().data()->isConfigSignalDataInfo(sheetIdx, strList);
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

QJsonObject TestCase::getOutputSig(const int& sheetIdx, const QStringList& strList) {
    QJsonObject ret;
    QStringList columnTitleList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeOtherTitle).toStringList();
    QString titleIsInitialize = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::IsInitialize));
    QString titleOutputValue = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue));
    auto outputSigMap = ControlExcel::instance().data()->isOutputSignalDataInfo(sheetIdx, strList);
    for (const auto& outputSigKey : outputSigMap.keys()) {
        auto tmpSignalDataInfo = outputSigMap[outputSigKey];
        auto tmpInputDataList = tmpSignalDataInfo.isConvertData();
        auto tmpValueEnum = tmpSignalDataInfo.isValueEnum();
        auto tmpIsInitialize = tmpSignalDataInfo.isInitialize();
        auto tmpOutputSigKey = outputSigKey;
        if (tmpOutputSigKey == ControlExcel::instance().data()->isKeywordString(
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

    const int sheetIdxStart = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription;
    QStringList columnTitleList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeOtherTitle).toStringList();

    QString titleTcName = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
    QString titleResult = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
    QString titleCase = columnTitleList.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case));
    QString titleSheet = TEXT_SHEET;

    QString sheetKey = QString("%1[%2]").arg(titleSheet).arg(sheetNumber - sheetIdxStart);
    QString tcNameKey = QString("%1[%2]").arg(titleTcName).arg(tcNameNumber);

    QString genType = getGenType();
    QString sendStr = getSignalInfoString(genType, sheetNumber, tcName, "", "");
    qDebug() << "send buffer: " << sendStr;
    callPython(sendStr);
    QJsonObject otherCase = readJson();
    otherCase[titleCase] =
        ControlExcel::instance().data()->isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Other));
    int tcNameAllCaseSize = 0;
    if (otherCase.contains(JSON_CASE_SIZE_NAME)) {
        tcNameAllCaseSize = otherCase[JSON_CASE_SIZE_NAME].toInt();
        if (!otherCase.contains(JSON_ALL_CASE_SIZE_NAME)) {
            otherCase[JSON_ALL_CASE_SIZE_NAME] = tcNameAllCaseSize;
        }
    } else {
        qDebug() << "All Case Key Error";
    }

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

    int OnConditionCnt = 0;
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
    int tcNameOtherCaseSize = 0;
    if (otherCase.contains(JSON_CASES_NAME) && !otherCase.contains(JSON_OTHER_CASE_SIZE_NAME)) {
        tcNameOtherCaseSize = otherCase[JSON_CASES_NAME].toObject().size();
        otherCase[JSON_OTHER_CASE_SIZE_NAME] = tcNameOtherCaseSize;
    } else {
        qDebug() << "Other Case Key Error";
    }

    makeOtherTcInfo(otherCase, mOtherInfo, tcName);
#if 0
    for (auto otherMapKey : mOtherInfo[tcName].keys()) {
        qDebug() << "mOtherInfo"
                 << "[" << tcName << "]: " << otherMapKey << "/// originKey: " << mOtherInfo[tcName][otherMapKey];
    }
#endif

    appendCaseJson(fileJson, otherCase, caseName, caseNumber, resultName, resultNumber, vehicleType, tcName, tcNameNumber,
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

void TestCase::makeOtherTcInfo(const QJsonObject& jsonObject, QMap<QString, QMap<QString, QString>>& OtherInfo,
                               const QString& tcName) {
    if (!OtherInfo.contains(tcName)) {
        // tcName 이 없으면 빈 QMap을 추가
        OtherInfo[tcName] = QMap<QString, QString>();
    } else {
        qDebug() << "Error: " << tcName << "Other data already exits...";
        return;
    }
    QJsonObject inputSignalList = jsonObject[TEXT_INPUT_SIGNAL_LIST].toObject();
    QJsonObject cases = jsonObject[JSON_CASES_NAME].toObject();

    // 각 case를 처리
    for (auto it = cases.begin(); it != cases.end(); ++it) {
        QString caseKey = it.key();                    // 현재 case의 키
        QJsonArray caseValues = it.value().toArray();  // 현재 case의 값 리스트

        // caseValues를 반복하여 InputSignalList에서 해당 신호 정보 가져오기
        int index = 0;
        for (auto sigIt = inputSignalList.begin(); sigIt != inputSignalList.end(); ++sigIt, ++index) {
            QString signalKey = sigIt.key();
            QJsonObject signalData = sigIt.value().toObject();

            if (index >= caseValues.size()) {
                break;  // caseValues 범위를 벗어나지 않도록
            }

            // 현재 신호의 InputData 배열과 caseValues 비교
            QJsonArray inputDataArray = signalData[TEXT_INPUT_DATA].toArray();
            QJsonArray preconditionDataArray = signalData[TEXT_PRECONDITION].toArray();
            QString caseValue = caseValues[index].toString();

            if (signalKey == TEXT_IGN) {
                int ignIdx = 0;
                for (const QJsonValue& value : inputDataArray) {
                    if (value.toString() == caseValue) {
                        break;
                    } else {
                        ignIdx++;
                    }
                }
                if (ignIdx < inputDataArray.size() && ignIdx < preconditionDataArray.size()) {
                    QString triggerInfo = getTriggerInfo(caseValues, signalKey, index, preconditionDataArray[ignIdx]);
                    // qDebug() << "triggerInfo: " << triggerInfo;
                    insertTriggerInfo(OtherInfo, tcName, triggerInfo, caseKey);
                } else {
                    qDebug() << "IGN Index Error";
                }
            } else {
                // InputData와 caseValue 비교 후 다른 값들을 Precondition으로 설정
                for (const QJsonValue& value : inputDataArray) {
                    if (value.toString() != caseValue) {
                        QString triggerInfo = getTriggerInfo(caseValues, signalKey, index, value);
                        // qDebug() << "triggerInfo: " << triggerInfo;
                        insertTriggerInfo(OtherInfo, tcName, triggerInfo, caseKey);
                    }
                }
            }
        }
    }
}

QString TestCase::getTriggerInfo(const QJsonArray& caseValues, const QString& triggerSigName, const int& triggerSigIndex,
                                 const QJsonValue& preconditionValue) {
    QStringList preconditionStrList;
    for (int caseValueIdx = 0; caseValueIdx < caseValues.size(); caseValueIdx++) {
        if (triggerSigIndex != caseValueIdx) {
            preconditionStrList << caseValues[caseValueIdx].toString();
        } else {
            preconditionStrList << preconditionValue.toString();
        }
    }
    QString preconditionStr = preconditionStrList.join(", ");
    QString inputStr = QString("=>" + triggerSigName + ":" + caseValues[triggerSigIndex].toString());
    return preconditionStr + inputStr;
}

void TestCase::insertTriggerInfo(QMap<QString, QMap<QString, QString>>& OtherInfo, const QString& tcName,
                                 const QString& triggerInfo, const QString& originCasesKey) {
    if (!OtherInfo.contains(tcName)) {
        qDebug() << "Error: "
                 << "You Cannot insert to OtherInfo[" << tcName << "]";
        return;
    }

    if (OtherInfo[tcName].contains(triggerInfo)) {
        qDebug() << "Redundancy error";
        return;
    } else {
        OtherInfo[tcName].insert(triggerInfo, originCasesKey);
    }

    // Redundancy 확인해보려면 아래 라인처럼 구현하면 됨
    // if (OtherInfo[tcName].contains(triggerInfo)) {
    //     int redundancy = OtherInfo[tcName][triggerInfo].toInt();
    //     redundancy++;
    //     OtherInfo[tcName].insert(triggerInfo, QString::Number(redundancy));
    // } else {
    //     int redundancy = 1;
    //     OtherInfo[tcName].insert(triggerInfo, QString::Number(redundancy));
    // }
}

void TestCase::printCaseSize(const QString& genType) {
    qDebug() << genType + QString(" Result==============================================================================");
    int sum = 0;
    QString allCasekey;
    for (const auto& key : mCaseSizeMap.keys()) {
        if (!key.contains(QString("AllCaseSize"), Qt::CaseInsensitive) && key.contains(genType, Qt::CaseInsensitive)) {
            sum += mCaseSizeMap[key];
            qDebug() << key << " => " << mCaseSizeMap[key];
        } else if (key.contains(QString("AllCaseSize"), Qt::CaseInsensitive) && key.contains(genType, Qt::CaseInsensitive)) {
            allCasekey = key;
        } else {
            // no operation
        }
    }
    qDebug() << QString("Sum of cases => ") + QString::number(sum);
    if (genType == GEN_TYPE_DEFAULT && mCaseSizeMap.contains(allCasekey)) {
        qDebug() << QString("Theoretical Number of All Cases => ") + QString::number(mCaseSizeMap[allCasekey]);
    }
    qDebug() << QString("============================================================================================") << "\n\n";
}

void TestCase::genTestCaseFile(const QJsonObject& json) {
    TestCaseWriter testCaseFile;
    // TestCaseWriter testCaseFile("../deploy_x86");

    int ignCount = 0;
    int testCaseCount = 1;
    auto quoteIfNotNumeric = [](const QString& value) {
        return (!value.isEmpty() && !value[0].isDigit() && !value.contains("timeout") && !value.contains("crc"))
                   ? "\"" + value + "\""
                   : value;
    };
    for (const QString& sheet : json.keys()) {
        qDebug().noquote() << QString("■ %1").arg(sheet);
        QJsonObject sectionObj = json[sheet].toObject();

        for (auto tcIt = sectionObj.begin(); tcIt != sectionObj.end(); ++tcIt) {
            QJsonObject tcObj = tcIt.value().toObject();
            QString tcName = tcObj["TCName"].toString();
            QString vehicleType = tcObj["VehicleType"].toString();

            if (!tcName.isEmpty()) {
                QString othersResultName;
                QStringList othersInputSignalList;
                QVector<std::tuple<QString, int, int, QJsonArray, QJsonObject, QJsonArray>> othersSignalListInfo;
                QStringList othersOutputSignal;
                QStringList othersOutputValue;
                QVector<bool> othersIsInitialize;

                qDebug().noquote() << QString("  ├ TCName: %1").arg(tcName);
                qDebug().noquote() << QString("  ├ VehicleType: %1").arg(vehicleType);

                qDebug().noquote() << QString("  ├ Config_Signal:");
                for (const QJsonValue& signal : tcObj["Config_Signal"].toArray()) {
                    qDebug().noquote() << QString("  │  ├ %1").arg(signal.toString());
                }
                qDebug().noquote() << QString("  ├ Data:");
                for (const QJsonValue& value : tcObj["Data"].toArray()) {
                    qDebug().noquote() << QString("  │  ├ %1").arg(value.toString());
                }
                qDebug().noquote() << QString("  ├ Negative Test: %1").arg(tcObj["Negative Test"].toString());

                for (const QString& key : tcObj.keys()) {
                    if (key.startsWith("Result[")) {
                        qDebug().noquote() << QString("  │  ├ %1").arg(key);
                        QJsonObject resultObj = tcObj[key].toObject();
                        QString resultName = resultObj["Result"].toString();
                        QStringList outputSignal;
                        QStringList outputValue;
                        QVector<bool> isInitialize;

                        qDebug().noquote() << QString("  │  │  ├ Result: %1").arg(resultName);

                        QJsonObject outputSignalList = resultObj["Output_Signal"].toObject();
                        qDebug().noquote() << QString("  │  │  ├ Output_Signal:");
                        QJsonObject signalObj;
                        for (auto sigIt = outputSignalList.begin(); sigIt != outputSignalList.end(); ++sigIt) {
                            signalObj = sigIt.value().toObject();
                            outputSignal << sigIt.key();
                            outputValue << signalObj["Output_Value"].toString();
                            isInitialize << signalObj["isInitialize"].toBool();
                            qDebug().noquote() << QString("  │  │  │  ├ %1:").arg(sigIt.key());
                            qDebug().noquote()
                                << QString("  │  │  │  │  ├ Output_Value: %1").arg(signalObj["Output_Value"].toString());
                            qDebug().noquote()
                                << QString("  │  │  │  │  ├ isInitialize: %1").arg(signalObj["isInitialize"].toBool());
                        }
                        for (const QString& caseKey : resultObj.keys()) {
                            if (caseKey.startsWith("Case[")) {
                                QVector<std::tuple<QString, int, int, QJsonArray, QJsonObject, QJsonArray>> signalList;
                                QJsonObject caseObj = resultObj[caseKey].toObject();
                                QString caseName = caseObj["Case"].toString();
                                QStringList inputSignalList =
                                    resultObj[caseKey][QString(TEXT_INPUT_SIGNAL_LIST)].toObject().keys();
#if 0
                                QJsonObject enumObj;
                                qDebug().noquote() << QString("  │  │  ├ %1").arg(caseKey);
                                qDebug().noquote() << QString("  │  │  │  ├ Case: %1").arg(caseName);
                                qDebug().noquote() << QString("  │  │  │  ├ InputSignalList:");
#endif
                                for (const QString& inputSignal : inputSignalList) {
#if 0
                                    qDebug().noquote() << QString("  │  │  │  │  ├ %1:").arg(inputSignal);
                                    qDebug().noquote() << QString("  │  │  │  │  │  ├ DataType: %1")
                                                              .arg(resultObj[caseKey][QString(TEXT_INPUT_SIGNAL_LIST)][inputSignal]
                                                                            [QString(TEXT_DATA_TYPE)]
                                                                                .toInt());
                                    qDebug().noquote() << QString("  │  │  │  │  │  ├ KeywordType: %1")
                                                              .arg(resultObj[caseKey][QString(TEXT_INPUT_SIGNAL_LIST)][inputSignal]
                                                                            [QString("KeywordType")]
                                                                                .toInt());
                                    qDebug().noquote() << QString("  │  │  │  │  │  ├ Precondition:");
                                    QJsonArray preconditionArray =
                                        resultObj[caseKey][QString(TEXT_INPUT_SIGNAL_LIST)][inputSignal][QString(TEXT_PRECONDITION)]
                                            .toArray();
                                    if (!(preconditionArray.size() == 1 && preconditionArray.first().toString().isEmpty())) {
                                        for (const QJsonValue& enumValue : preconditionArray) {
                                            if (!enumValue.toString().isEmpty()) {
                                                qDebug().noquote()
                                                    << QString("  │  │  │  │  │  │  ├ %1").arg(enumValue.toString());
                                            }
                                        }
                                    }
                                    qDebug().noquote() << QString("  │  │  │  │  │  ├ ValueEnum:");
                                    enumObj = resultObj[caseKey][QString(TEXT_INPUT_SIGNAL_LIST)][inputSignal][QString(TEXT_VALUE_ENUM)]
                                                  .toObject();
                                    for (auto it = enumObj.constBegin(); it != enumObj.constEnd(); ++it) {
                                        qDebug().noquote() << QString("  │  │  │  │  │  │  ├ %1: %2")
                                                                  .arg(it.key(), -6)
                                                                  .arg(it.value().toString());
                                    }
#endif
                                    signalList.emplace_back(std::make_tuple(
                                        inputSignal,
                                        resultObj[caseKey][QString(TEXT_INPUT_SIGNAL_LIST)][inputSignal][QString(TEXT_DATA_TYPE)]
                                            .toInt(),
                                        resultObj[caseKey][QString(TEXT_INPUT_SIGNAL_LIST)][inputSignal][QString("KeywordType")]
                                            .toInt(),
                                        resultObj[caseKey][QString(TEXT_INPUT_SIGNAL_LIST)][inputSignal]
                                                 [QString(TEXT_PRECONDITION)]
                                                     .toArray(),
                                        resultObj[caseKey][QString(TEXT_INPUT_SIGNAL_LIST)][inputSignal][QString(TEXT_VALUE_ENUM)]
                                            .toObject(),
                                        resultObj[caseKey][QString(TEXT_INPUT_SIGNAL_LIST)][inputSignal][QString(TEXT_INPUT_DATA)]
                                            .toArray()));
                                }
#if 0
                                qDebug().noquote() << QString("  │  │  │  ├ Cases:");
                                for (const QString& subArrayValue : caseObj["Cases"].toObject().keys()) {
                                    qDebug().noquote() << QString("  │  │  │  │  ├ [%1]").arg(subArrayValue);
                                }
#endif
                                if (caseName == ControlExcel::instance().data()->isKeywordString(
                                                    static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Other))) {
                                    othersResultName = resultName;
                                    othersInputSignalList = inputSignalList;
                                    othersSignalListInfo = signalList;
                                    othersOutputSignal = outputSignal;
                                    othersOutputValue = outputValue;
                                    othersIsInitialize = isInitialize;
                                } else {
                                    QString signalName;
                                    QString signalValue;
                                    QString enumString;
                                    QString triggerSignalName;
                                    QString triggerSignalValue;
                                    QString triggerSignalValueComment;
                                    // NOTE: Case의 시그널 갯수
                                    for (int i = 0; i < inputSignalList.size(); i++) {
                                        // NOTE: inputSignal이 NotTrigger가 아니고 Empty가 아닐 경우에만 TC Case를 생성한다.
                                        if (std::get<2>(signalList[i]) !=
                                                static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::NotTrigger) &&
                                            std::get<3>(signalList[i]).contains(TEXT_EMPTY) == false) {
                                            // NOTE: inputSignal Precondition 갯수 만큼 순회 하며 Trigger TC 생성.
                                            for (int j = 0; j < std::get<3>(signalList[i]).size(); j++) {
                                                for (const QString& subArrayValue : caseObj[JSON_CASES_NAME].toObject().keys()) {
                                                    QStringList list = subArrayValue.split(',', Qt::SkipEmptyParts);
                                                    // NOTE: Case의 각 Value를 순회 하며 Precondition과 Input값을 생성.
                                                    for (int k = 0; k < list.size(); k++) {
                                                        if (k == 0) {
                                                            testCaseFile.write("- name: " + tcName + ", " + resultName + ", " +
                                                                                   caseName + " " +
                                                                                   QString::number(testCaseCount++),
                                                                               1);
                                                            if (vehicleType.isEmpty() == false) {
                                                                testCaseFile.write("tag: " + vehicleType, 2, 1);
                                                            }
                                                            testCaseFile.write("precondition:", 2, 1);
                                                        }

                                                        signalName = std::get<0>(signalList[k]);
                                                        signalValue = list[k].trimmed();
                                                        enumString = std::get<4>(signalList[k])[list[k].trimmed()].toString();

                                                        // NOTE: Case의 개별 요소가 Empty나 Unknown일 경우 해당 시그널 TC 생성제외
                                                        if (signalValue.contains(TEXT_EMPTY) == false &&
                                                            signalValue.contains("Unknown") == false) {
                                                            if (i == k) {
                                                                triggerSignalName = signalName;
                                                                triggerSignalValue = signalValue;
                                                                const auto comment =
                                                                    std::get<4>(signalList[k])[list[k].trimmed()].toString();
                                                                triggerSignalValueComment =
                                                                    comment.isEmpty() ? QString() : QString(" # %1").arg(comment);
                                                                if (signalName.contains("SFC.Private.IGNElapsed")) {
                                                                    signalName =
                                                                        ControlExcel::instance().data()->isIGNElapsedName(
                                                                            std::get<3>(signalList[i])[j].toString().toInt());
                                                                    signalValue =
                                                                        "0x" + QString::number(++ignCount, 16).toUpper();
                                                                    // qDebug()
                                                                    //     << "@@@@ " << signalName << ": " <<
                                                                    //     signalValue;
                                                                } else {
                                                                    signalName = std::get<0>(signalList[i]);
                                                                    signalValue = std::get<3>(signalList[i])[j].toString();
                                                                    enumString =
                                                                        std::get<4>(signalList[i])[signalValue].toString();
                                                                }
                                                            } else {
                                                                if (signalName.contains("SFC.Private.IGNElapsed")) {
                                                                    signalName =
                                                                        ControlExcel::instance().data()->isIGNElapsedName(
                                                                            signalValue.toInt());
                                                                    signalValue =
                                                                        "0x" + QString::number(++ignCount, 16).toUpper();
                                                                }
                                                            }
                                                            enumString = enumString.isEmpty() ? QString()
                                                                                              : QString(" # %1").arg(enumString);
                                                            testCaseFile.write("- " + signalName + ": " +
                                                                                   quoteIfNotNumeric(signalValue) + enumString,
                                                                               3, 1);
                                                        }
                                                    }
                                                    if (!triggerSignalValue.contains(TEXT_EMPTY)) {
                                                        // NOTE: isInitialize값이 true 일 경우 초기화를 위한 SFC Output 구문 생성.
                                                        if (isInitialize.contains(true)) {
                                                            testCaseFile.write("init:", 2, 1);
                                                            for (int initCnt = 0; initCnt < isInitialize.size(); initCnt++) {
                                                                if (isInitialize[initCnt] == true) {
                                                                    if (outputValue[initCnt][0].isDigit() == true) {
                                                                        if (outputValue[initCnt].contains("0x")) {
                                                                            testCaseFile.write(
                                                                                "- " + outputSignal[initCnt] + ": 0x0", 3);
                                                                        } else {
                                                                            testCaseFile.write(
                                                                                "- " + outputSignal[initCnt] + ": 0", 3);
                                                                        }
                                                                    } else {
                                                                        testCaseFile.write(
                                                                            "- " + outputSignal[initCnt] + ": " +
                                                                                quoteIfNotNumeric(outputValue[initCnt]),
                                                                            3);
                                                                    }
                                                                }
                                                            }
                                                        }
                                                        testCaseFile.write("input:", 2, 1);
                                                        if (triggerSignalName.contains("SFC.Private.IGNElapsed")) {
                                                            triggerSignalName = ControlExcel::instance().data()->isIGNElapsedName(
                                                                triggerSignalValue.toInt());
                                                            triggerSignalValue = "0x" + QString::number(++ignCount, 16).toUpper();
                                                        }
                                                        testCaseFile.write("- " + triggerSignalName + ": " +
                                                                               quoteIfNotNumeric(triggerSignalValue) +
                                                                               triggerSignalValueComment,
                                                                           3);
                                                        testCaseFile.write("output:", 2);
                                                        for (int outputCnt = 0; outputCnt < outputSignal.size(); outputCnt++) {
                                                            QString tempOutputSignal = outputSignal[outputCnt];
                                                            if (tempOutputSignal.contains(TEXT_COLLECT)) {
                                                                tempOutputSignal = "collect";
                                                            }
                                                            (outputCnt == outputSignal.size() - 1)
                                                                ? testCaseFile.write(
                                                                      "- " + tempOutputSignal + ": " +
                                                                          quoteIfNotNumeric(outputValue[outputCnt]),
                                                                      3, 2)
                                                                : testCaseFile.write(
                                                                      "- " + tempOutputSignal + ": " +
                                                                          quoteIfNotNumeric(outputValue[outputCnt]),
                                                                      3);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if (mOtherInfo[tcName].isEmpty() == false) {
                    int signalCount = 0;
                    QStringList parts;
                    QString signalName;
                    QString signalValue;
                    QStringList triggerInfo;
                    QString triggerSignal;  // Vehicle.System.Undefined.Inter_ConfigEBS
                    QString triggerValue;
                    QString enumString;
                    QString triggerEnumString;
                    // NOTE: Others TC Case 생성
                    // NOTE: othersResultName
                    // NOTE: othersInputSignalList
                    // NOTE: othersSignalListInfo
                    // NOTE: othersOutputSignal
                    // NOTE: othersOutputValue
                    // NOTE: othersIsInitialize
                    for (auto otherMapKey : mOtherInfo[tcName].keys()) {
                        // NOTE: => 를 중심으로 앞의 데이터는 Prcondition 뒤는 input
                        // NOTE: 분리된 input을 다시 :를 중심으로 name과 value로 분리.
                        // NOTE: "10, 0x2, 0x1, 0x0, 0x1, 0x4=>Vehicle.System.Undefined.Inter_ConfigEBS:0x2"
                        // NOTE: 1. => 기준으로 문자열 분리
                        parts = otherMapKey.split("=>");

                        if (parts.size() == 2) {
                            // NOTE: 앞쪽 문자열 처리 (쉼표로 구분된 리스트)

                            testCaseFile.write("- name: " + tcName + ", " + othersResultName + ", " + TEXT_OTHERS + " " +
                                                   QString::number(testCaseCount++),
                                               1);
                            if (vehicleType.isEmpty() == false) {
                                testCaseFile.write("tag: " + vehicleType, 2, 1);
                            }
                            testCaseFile.write("precondition:", 2, 1);

                            // NOTE: 뒷쪽 문자열 처리 (:로 구분)
                            triggerInfo = parts[1].split(":");

                            // NOTE: 공백 제거 및 트리밍
                            // NOTE: 여기서 ,로 분리된 value에 맞는 signal을 붙여서 precondition생성
                            signalCount = 0;
                            for (QString& number : parts[0].split(",", Qt::SkipEmptyParts)) {
                                number = number.trimmed();
                                enumString = std::get<4>(othersSignalListInfo[signalCount])[number].toString();
                                if (triggerInfo[0] == std::get<0>(othersSignalListInfo[signalCount])) {
                                    triggerEnumString = std::get<4>(othersSignalListInfo[signalCount])[triggerInfo[1]].toString();
                                }
                                if (std::get<0>(othersSignalListInfo[signalCount]).contains("SFC.Private.IGNElapsed")) {
                                    signalName = ControlExcel::instance().data()->isIGNElapsedName(number.toInt());
                                    signalValue = "0x" + QString::number(++ignCount, 16).toUpper();
                                } else {
                                    signalName = std::get<0>(othersSignalListInfo[signalCount]);
                                    signalValue = number;
                                }
                                enumString = enumString.isEmpty() ? QString() : QString(" # %1").arg(enumString);
                                // testCaseFile.write("- " + signalName + ": " + signalValue + enumString, 3);

                                testCaseFile.write("- " + signalName + ": " + quoteIfNotNumeric(signalValue) + enumString, 3);
                                signalCount++;
                            }

                            if (othersIsInitialize.contains(true)) {
                                testCaseFile.write("init:", 2, 1);
                                for (int initCnt = 0; initCnt < othersIsInitialize.size(); initCnt++) {
                                    if (othersIsInitialize[initCnt] == true) {
                                        if (othersOutputValue[initCnt][0].isDigit() == true) {
                                            if (othersOutputValue[initCnt].contains("0x")) {
                                                testCaseFile.write("- " + othersOutputSignal[initCnt] + ": 0x0", 3);
                                            } else {
                                                testCaseFile.write("- " + othersOutputSignal[initCnt] + ": 0", 3);
                                            }
                                        } else {
                                            testCaseFile.write("- " + othersOutputSignal[initCnt] + ": " +
                                                                   quoteIfNotNumeric(othersOutputValue[initCnt]),
                                                               3);
                                        }
                                    }
                                }
                            }

                            if (parts[1].split(":").size() == 2) {
                                testCaseFile.write("input:", 2, 1);
                                if (triggerInfo[0].contains("SFC.Private.IGNElapsed")) {
                                    triggerSignal = ControlExcel::instance().data()->isIGNElapsedName(triggerInfo[1].toInt());
                                    triggerValue = "0x" + QString::number(++ignCount, 16).toUpper();
                                } else {
                                    triggerSignal = triggerInfo[0];
                                    triggerValue = triggerInfo[1];
                                }
                                triggerEnumString =
                                    triggerEnumString.isEmpty() ? QString() : QString(" # %1").arg(triggerEnumString);
                                testCaseFile.write(
                                    "- " + triggerSignal + ": " + quoteIfNotNumeric(triggerValue) + triggerEnumString, 3);
                            }
                            testCaseFile.write("output:", 2);
                            for (int outputCnt = 0; outputCnt < othersOutputSignal.size(); outputCnt++) {
                                QString tempOutputSignal = othersOutputSignal[outputCnt];
                                if (tempOutputSignal.contains(TEXT_COLLECT)) {
                                    tempOutputSignal = "collect";
                                }
                                (outputCnt == othersOutputSignal.size() - 1)
                                    ? testCaseFile.write(
                                          "- " + tempOutputSignal + ": " + quoteIfNotNumeric(othersOutputValue[outputCnt]), 3, 2)
                                    : testCaseFile.write(
                                          "- " + tempOutputSignal + ": " + quoteIfNotNumeric(othersOutputValue[outputCnt]), 3);
                            }
                        }
                    }
                }
            }
            qDebug().noquote() << QString("  │");
        }
    }
}
