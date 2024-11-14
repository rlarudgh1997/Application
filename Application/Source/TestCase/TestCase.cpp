#include "TestCase.h"

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
    if (getExcuteType() == type) {
        qDebug() << "Running excute tc type :" << type;
        return;
    }

    switch (type) {
        case ExcuteTypeGenTC: {
            qDebug() << "excuteTestCase()";
            genCase();

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
        // SheetConfigurationInfo sheetDataList = ControlExcel::instance().data()->isSheetConfigurationInfo(sheetIndex);

        // for (const auto& sheetData : sheetDataList.isDataInfo()) {
        //     QString tcName = sheetData.first;
        //     QString result = sheetData.second.first;
        //     QStringList caseList = sheetData.second.second;

        //     qDebug() << "TCName :" << tcName << result << caseList;
        // }
        QVariantList sheetdata = getSheetData(sheetIndex).toList();
        // qDebug() << "sheetdata[" << sheetIndex << "].size(): " << sheetdata.size();
        if (sheetIndex == convertStart || sheetIndex == convertStart + 1) {
            // 모듈 이름정보가 필요할 경우 해당 부분에서 구현
            // Private Sheet 도 필요한 경우도 해당 부분에서 구현
            continue;
        }
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

            // 추후, isRowData 와 isInputDataInfo 로 수정 예정
            QString tmpInputSignal = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal));
            QString tmpInputData = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData));

            if (it == sheetdata.begin()) {
                currentCase = tmpCase;
                currentResult = tmpResult;
                currentTCName = tmpTCName;
            }

            if (currentCase != tmpCase && (currentCase != "Others" && currentCase != "Other")) {
                QString sendStr = getSignalInfoString(sheetIndex, currentTCName, currentResult, currentCase);
                qDebug() << "TCName:Result:Case == " << currentTCName << ":" << currentResult << ":" << currentCase;
                callPython(sendStr);
                QJsonObject caseJson = readJson("path");
                int caseSize = 0;
                appendCaseJson(mFileJson, caseJson, currentCase, caseCnt, currentResult, resultCnt, currentVehicleType,
                               currentTCName, tcNameCnt, sheetIndex);
                caseCnt++;
                currentCase = tmpCase;
            } else if (currentCase != tmpCase && (currentCase == "Others" || currentCase == "Other")) {
                appendOtherCaseJson(mFileJson, currentCase, caseCnt, currentResult, resultCnt, currentVehicleType, currentTCName,
                                    tcNameCnt, sheetIndex);
                caseCnt++;
                currentCase = tmpCase;
            }

            if (currentResult != tmpResult) {
                resultCnt++;
                currentResult = tmpResult;
            }

            if (currentTCName != tmpTCName) {
                tcNameCnt++;
                currentTCName = tmpTCName;
            }

            if (std::next(it) == sheetdata.end() && (currentCase != "Others" && currentCase != "Other")) {
                QString sendStr = getSignalInfoString(sheetIndex, currentTCName, currentResult, currentCase);
                callPython(sendStr);
                QJsonObject caseJson = readJson("path");
                appendCaseJson(mFileJson, caseJson, currentCase, caseCnt, currentResult, resultCnt, currentVehicleType,
                               currentTCName, tcNameCnt, sheetIndex);
            } else if (std::next(it) == sheetdata.end() && (currentCase == "Others" || currentCase == "Other")) {
                appendOtherCaseJson(mFileJson, currentCase, caseCnt, currentResult, resultCnt, currentVehicleType, currentTCName,
                                    tcNameCnt, sheetIndex);
            }
        }
    }
    QString _filePath = QString("%1/TcGenHistory.json").arg(ivis::common::APP_PWD());
    saveJsonToFile(mFileJson, _filePath);
    return "";
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

QString TestCase::getSignalInfoString(const int& sheetNum, const QString& tcName, const QString& resultName,
                                      const QString& caseName) {
    QString ret;
    auto sigDataInfoMap =
        ControlExcel::instance().data()->isInputSignalDataInfo(sheetNum, QStringList({tcName, resultName, caseName}), false);

    for (const auto& sig : sigDataInfoMap.keys()) {
        auto sigDataInfo = sigDataInfoMap[sig];
        ret += QString("InputSignalName   : ") + sig + QString("\n");
        ret += QString("InputDataType   : ") + QString::number(sigDataInfo.isDataType()) + QString("\n");
        ret += QString("InputKeywordType   : ") + QString::number(sigDataInfo.isKeywordType()) + QString("\n");
        if (resultName == "" && caseName == "" &&
            sigDataInfo.isDataType() == static_cast<int>(ivis::common::DataTypeEnum::DataType::HUInt64) &&
            sigDataInfo.isValueEnum().isEmpty() == false) {
            QStringList allEnumList;
            for (const auto& enumString : sigDataInfo.isValueEnum()) {
                QStringList enumSplitList = enumString.split(":");
                for (const auto& enumSplit : enumSplitList) {
                    if (enumSplit.contains("0x") == false) {
                        allEnumList.append(enumSplit);
                        break;
                    }
                }
            }
            ret += QString("InputData   : ") + allEnumList.join(", ") + QString("\n");
        } else {
            ret += QString("InputData   : ") + sigDataInfo.isConvertData().join(", ") + QString("\n");
        }
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
    qDebug() << "Python call...";
    QString cmd = QString("PYTHONPATH=$(pwd)/../Python/CaseGen/ python $(pwd)/../Python/CaseGen/tests/run.py \"%1\"").arg(str);
    ivis::common::ExcuteProgram process(false);
    QStringList log;
    bool result = process.start(cmd, log);

    if (result) {
        qDebug() << "Success(TestCase::callPython)";
    } else {
        qDebug() << "Fail(TestCase::callPython): " << log;
    }
    qDebug() << "=================================================================================================\n\n";
}

QJsonObject TestCase::readJson(const QString& filePath) {
    // If you wanna change the path, Use the following linie
    // QString _filePath = filePath;
    QString _filePath = QString("%1/CaseResult.json").arg(ivis::common::APP_PWD());
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
                              const QString& tcName, const int& tcNameNumber, const int& sheetNumber) {
    const int sheetIdxStart = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription;
    // QJsonObject tmpJson = readJson("path");
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
    QString titleSheet = "Sheet";

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
        tcNameJson[titleGenType] = getGenType();
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
    QString ret = "Default";
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
        auto tmpIsInitialize = tmpSignalDataInfo.isInitialize();
        // 1. ret에서 QJsonObject를 가져오기
        QJsonObject outputObj = ret[outputSigKey].toObject();
        // 2. 가져온 객체에 데이터 추가
        if (!outputObj.contains(titleOutputValue)) {
            if (tmpInputDataList.size() > 0) {
                outputObj[titleOutputValue] = tmpInputDataList[0];
            } else {
                qDebug() << "No Output!!!!!";
            }
        }
        if (!outputObj.contains(titleIsInitialize)) {
            outputObj[titleIsInitialize] = tmpIsInitialize;
        }
        // 3. 수정된 객체를 다시 ret에 설정
        ret[outputSigKey] = outputObj;
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
    QString titleSheet = "Sheet";

    QString sheetKey = QString("%1[%2]").arg(titleSheet).arg(sheetNumber - sheetIdxStart);
    QString tcNameKey = QString("%1[%2]").arg(titleTcName).arg(tcNameNumber);

    QString sendStr = getSignalInfoString(sheetNumber, tcName, "", "");
    callPython(sendStr);
    QJsonObject otherCase = readJson("path");
    otherCase[titleCase] = "Others";
    int tcNameAllCaseSize = 0;
    if (otherCase.contains("Cases")) {
        tcNameAllCaseSize = otherCase["Cases"].toObject().size();
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
                    if (caseJson.contains("Cases")) {
                        QJsonObject cases = caseJson["Cases"].toObject();
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
    if (otherCase.contains("Cases")) {
        tcNameOtherCaseSize = otherCase["Cases"].toObject().size();
    } else {
        qDebug() << "Other Case Key Error";
    }
    qDebug() << "TCName Result====================================================================================";
    qDebug() << "tcName             : " << tcName;
    qDebug() << "tcNameAllCaseSize  : " << tcNameAllCaseSize;
    qDebug() << "tcNameOtherCaseSize: " << tcNameOtherCaseSize;
    qDebug() << "=================================================================================================\n\n";
    appendCaseJson(fileJson, otherCase, caseName, caseNumber, resultName, resultNumber, vehicleType, tcName, tcNameNumber,
                   sheetNumber);
    checkTimer.check("appendOtherCaseJson");
}

void TestCase::removeMatchingKeys(QJsonObject& otherJson, const QJsonObject& validArray) {
    // Cases 객체 가져오기
    QJsonObject casesArray = otherJson["Cases"].toObject();

    // validArray 모든 키를 확인하며, 해당 키가 와일드카드 패턴일 경우 다른 키를 삭제
    for (const QString& key : validArray.keys()) {
        // 콤마로 분리된 각 부분의 조건을 설정
        QStringList keyParts = key.split(", ");
        QString pattern = "^";
        bool isEmptyPartExist = false;

        // 각 부분이 비어있다면 정규표현식에서 와일드카드로 매칭되도록 함
        for (const QString& part : keyParts) {
            if (part == "[Empty]") {
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
            }
        }
    }

    // 수정된 casesArray를 otherJson에 다시 설정
    otherJson["Cases"] = casesArray;
}