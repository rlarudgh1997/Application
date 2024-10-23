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

int TestCase::isKeywordType(const QString& signalName) {
    QSharedPointer<SignalDataInfo> dataInfo = mSignalDataInfo[signalName];
    return dataInfo->getKeywordType();
}

int TestCase::isDataType(QMap<int, QStringList>& dataInfo, const int& keywordType) {
    SignalDataInfo info = SignalDataInfo(dataInfo, keywordType, QString());
    int dataType = info.isDataType();
    dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum] = info.getValueEnum();
    dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableICV] = info.getMatchingTableICV();
    dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableEV] = info.getMatchingTableEV();
    dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableFCEV] = info.getMatchingTableFCEV();
    dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTablePHEV] = info.getMatchingTablePHEV();
    dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableHEV] = info.getMatchingTableHEV();
    dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableSystem] = info.getMatchingTableSystem();
    dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeInputData] = info.getInputData();
    return dataType;
}

void TestCase::clearSignalDataInfo(const QString& signalName) {
    qDebug() << "clearSignalDataInfo :" << mSignalDataInfo.size() << signalName;

    if (signalName.size() == 0) {
        mSignalDataInfo.clear();
    } else {
        mSignalDataInfo.remove(signalName);
    }
}

void TestCase::setSignalDataInfo(const QString& signalName, const QMap<int, QStringList>& dataInfo, const int& keywordType,
                                 const QString& dataType) {
    if (dataInfo.size() == 0) {
        return;
    }

    mSignalDataInfo[signalName] = QSharedPointer<SignalDataInfo>::create(dataInfo, keywordType, dataType);
#if 0
    qDebug() << "=============================================================================================";
    for (auto iter = mSignalDataInfo.cbegin(); iter != mSignalDataInfo.cend(); ++iter) {
        QString signal = iter.key();
        if (signal.compare(signalName) != false) {
            continue;
        }

        QSharedPointer<SignalDataInfo> signalInfo = iter.value();
        if (signalInfo) {
            qDebug() << "\n\t [Write] :" << signalName.toLatin1().data()
                     << "\n\t\t KeywordType         :" << signalInfo->getKeywordType()
                     << "\n\t\t DataType            :" << signalInfo->getDataType()
                     << "\n\t\t InputData           :" << signalInfo->getInputData()
                     << "\n\t\t ValueEnum           :" << signalInfo->getValueEnum()
                     << "\n\t\t MatchingTableICV    :" << signalInfo->getMatchingTableICV()
                     << "\n\t\t MatchingTableEV     :" << signalInfo->getMatchingTableEV()
                     << "\n\t\t MatchingTableFCEV   :" << signalInfo->getMatchingTableFCEV()
                     << "\n\t\t MatchingTablePHEV   :" << signalInfo->getMatchingTablePHEV()
                     << "\n\t\t MatchingTableHEV    :" << signalInfo->getMatchingTableHEV()
                     << "\n\t\t MatchingTableSystem :" << signalInfo->getMatchingTableSystem();
        }
    }
    qDebug() << "=============================================================================================\n\n";
#endif
}

QMap<int, QStringList> TestCase::getSignalDataInfo(const QString& signalName, int& keywordType, QString& dataType) {
    QMap<int, QStringList> signalInfo = QMap<int, QStringList>();
    QSharedPointer<SignalDataInfo> dataInfo = mSignalDataInfo[signalName];

    if (dataInfo.isNull()) {
        qDebug() << "Fail to read signal data(invalid) info : " << signalName;
        return signalInfo;
    }

    dataType = dataInfo->getDataType();
    keywordType = dataInfo->getKeywordType();
    signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeInputData] = dataInfo->getInputData();
    signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum] = dataInfo->getValueEnum();
    signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableICV] = dataInfo->getMatchingTableICV();
    signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableEV] = dataInfo->getMatchingTableEV();
    signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableFCEV] = dataInfo->getMatchingTableFCEV();
    signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTablePHEV] = dataInfo->getMatchingTablePHEV();
    signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableHEV] = dataInfo->getMatchingTableHEV();
    signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableSystem] = dataInfo->getMatchingTableSystem();
#if 0
    qDebug() << "=============================================================================================";
    qDebug() << "\n\t [Read] :" << signalName.toLatin1().data() << "\n\t\t KeywordType          :" << keywordType
             << "\n\t\t DataType            :" << dataType
             << "\n\t\t InputData           :" << signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeInputData]
             << "\n\t\t ValueEnum           :" << signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum]
             << "\n\t\t MatchingTableICV    :" << signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableICV]
             << "\n\t\t MatchingTableEV     :" << signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableEV]
             << "\n\t\t MatchingTableFCEV   :" << signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableFCEV]
             << "\n\t\t MatchingTablePHEV   :" << signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTablePHEV]
             << "\n\t\t MatchingTableHEV    :" << signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableHEV]
             << "\n\t\t MatchingTableSystem :" << signalInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableSystem];
    qDebug() << "=============================================================================================\n\n";
#endif

    return signalInfo;
}

QString TestCase::getSignalInfoString(const QMap<QString, QMap<QString, QString>>& singleCase) {
    QString ret;

    for (const auto& TCName_Result_Case : singleCase.keys()) {
        for (const auto& inputSig : singleCase[TCName_Result_Case].keys()) {
            if (mSignalDataInfo.contains(inputSig)) {
                auto sigPtr = mSignalDataInfo[inputSig];
                if (inputSig.contains("SFC.Private.IGNElapsed.Elapsed")) {
                    ret += "InputSignalName   : " + inputSig + "\n";
                    ret += "InputDataType     : Invalid\n";
                    if (inputSig.contains("On")) {
                        ret += "InputData         : ON\n";
                    } else {
                        ret += "InputData         : OFF\n";
                    }
                    ret += "InputValueEnum    : 0x0:OFF, 0x1:ON\n";
                } else {
                    ret += "InputSignalName   : " + inputSig + "\n";
                    int dataType = sigPtr->isDataType();
                    if (dataType == static_cast<int>(ivis::common::DataTypeEnum::DataType::HUInt64)) {
                        ret += "InputDataType     : HUInt64\n";
                    } else if (dataType == static_cast<int>(ivis::common::DataTypeEnum::DataType::HInt64)) {
                        ret += "InputDataType     : HInt64\n";
                    } else if (dataType == static_cast<int>(ivis::common::DataTypeEnum::DataType::HString)) {
                        ret += "InputDataType     : HString\n";
                    } else {
                        ret += "InputDataType     : Invalid\n";
                    }
                    ret += "InputData         : " + singleCase[TCName_Result_Case][inputSig] + "\n";
                    ret += "InputValueEnum    : " + sigPtr->getValueEnum().join(", ").replace("\\", "").replace("\"", "") + "\n";
                }
            } else {
                qDebug() << "error";
            }
        }
    }
#if 0
    qDebug() << "getSignalInfoString ret: " << ret << Qt::endl;
#endif
    return ret;
}

QString TestCase::genCase() {
    const int convertStart = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription;
    const int convertEnd = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax;

    // ControlExcel::instance().data()->isInputDataInfo();
    for (int sheetIndex = convertStart; sheetIndex < convertEnd; ++sheetIndex) {
        QVariantList sheetdata = instance().data()->getSheetData(sheetIndex).toList();
        // qDebug() << "sheetdata[" << sheetIndex << "].size(): " << sheetdata.size();
        if (sheetIndex == convertStart || sheetIndex == convertStart + 1) {
            // 모듈 이름정보가 필요할 경우 해당 부분에서 구현
            // Private Sheet 도 필요한 경우도 해당 부분에서 구현
            continue;
        }
        QString currentTCName;
        QString currentCase;
        // QMap<"TCName:Result:Case", QMap<input_signal, input_data>>
        QMap<QString, QMap<QString, QString>> tmpSignalMap;
        // // QMap<InputSignal, InputSignalIdx>
        // QMap<QString, int> allInputList;
        QJsonObject sumJson;
        int caseCnt = 0;
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

            if (tmpSignalMap.isEmpty() == false && currentCase != tmpCase) {
#if 0  /* Test debug */
                    for (const auto& TCName_Result_Case : tmpSignalMap.keys()) {
                        qDebug() << "TC Key: " << TCName_Result_Case;
                        for (const auto& inputSig : tmpSignalMap[TCName_Result_Case].keys()) {
                            qDebug() << "\tinputSig (Key, Value): "
                                     << "(" << inputSig << ", " << tmpSignalMap[TCName_Result_Case][inputSig] << ")";
                        }
                    }
#endif /* Test debug End*/

                // Do python CaseGen
                QString sendStr = getSignalInfoString(tmpSignalMap);
                callPython(sendStr);
                QJsonObject tmpJson = readJson("path");
                QString firstKey = tmpSignalMap.firstKey();
                QStringList keyParts = firstKey.split(':');
                tmpJson["TCName"] = keyParts[0];
                tmpJson["VehicleType"] = keyParts[1];
                tmpJson["Result"] = keyParts[2];
                tmpJson["Case"] = keyParts[3];
                // printJson(tmpJson);
                appendCaseJson(sumJson, tmpJson, caseCnt);
                tmpSignalMap.clear();
                caseCnt++;
            }

            if (tmpCase == "Others") {
                // do other logic(T.B.D)
            } else {
                // tmpSignalMap 에 signal 정보 등록
                QString keyStr = tmpTCName + ":" + tmpVehicleType + ":" + tmpResult + ":" + tmpCase;
                if (tmpSignalMap.contains(keyStr)) {
                    // 바깥 QMap의 키가 존재하면, 내부 QMap에서 키가 존재하는지 확인
                    if (tmpSignalMap[keyStr].contains(tmpInputSignal)) {
                        // 이미 존재하는 경우, 에러 처리도 가능
                        qDebug() << "Key " << tmpInputSignal << " already exists in " << keyStr;
                    } else {
                        // 존재하지 않는 경우, 값을 추가
                        tmpSignalMap[keyStr][tmpInputSignal] = tmpInputData;
                    }
                } else {
                    // 바깥쪽 QMap에 키가 없으면 새롭게 추가
                    tmpSignalMap[keyStr][tmpInputSignal] = tmpInputData;
                }

                // // 전체 신호 목록에 신호 추가
                // if (allInputList, )
                //     allInputList.append(tmpInputSignal)
            }

            if (tmpSignalMap.isEmpty() == false && std::next(it) == sheetdata.end()) {
                // Do python CaseGen
                QString sendStr = getSignalInfoString(tmpSignalMap);
                callPython(sendStr);
                QJsonObject tmpJson = readJson("");
                // printJson(tmpJson);
                appendCaseJson(sumJson, tmpJson, caseCnt);
                tmpSignalMap.clear();
                caseCnt++;
            }

            // Json 전달
            if (sumJson.isEmpty() == false && (currentTCName != tmpTCName || std::next(it) == sheetdata.end())) {
                qDebug() << "TCName data send: " << currentTCName;
                // getOtherCase(sumJson);
                // addTcStr(sumJson);

                QString _filePath = QString("%1/../Python/CaseGen/tests/ItertoolsHistory.json").arg(ivis::common::APP_PWD());
                // saveJsonToFile(sumJson, _filePath);
                appendTCNameJson(currentTCName, sumJson, _filePath);

                sumJson = QJsonObject();  // 초기화
            }
            currentTCName = tmpTCName;
            currentCase = tmpCase;
        }
    }
    return "";
}

void TestCase::eraseMergeTag(QString& str) {
    const QVariant excelMergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart);
    const QVariant excelMergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd);
    const QVariant excelMerge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge);
    str = ivis::common::getRemoved(str, excelMergeStart.toString());
    str = ivis::common::getRemoved(str, excelMergeEnd.toString());
    str = ivis::common::getRemoved(str, excelMerge.toString());
}

void TestCase::callPython(const QString& str) {
    qDebug() << "Python call...";
    QString cmd = QString("PYTHONPATH=$(pwd)/../Python/CaseGen/ python $(pwd)/../Python/CaseGen/tests/run.py \"%2\"").arg(str);
    ivis::common::ExcuteProgram process(false);
    QStringList log;
    bool result = process.start(cmd, log);

    if (result) {
        qDebug() << "Success(TestCase::callPython): ";
    } else {
        qDebug() << "fail!: " << log;
    }
}

QJsonObject TestCase::readJson(const QString& filePath) {
    // If you wanna change the path, Use the following linie
    // QString _filePath = filePath;
    QString _filePath = QString("%1/../Python/CaseGen/tests/ItertoolsTest.json").arg(ivis::common::APP_PWD());
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

void TestCase::printJson(const QJsonObject& jsonObj, int indent) {
    const QString indentStr(indent, ' ');  // 들여쓰기용 문자열

    for (auto it = jsonObj.constBegin(); it != jsonObj.constEnd(); ++it) {
        QString key = it.key();
        QJsonValue value = it.value();

        if (value.isObject()) {
            qDebug().noquote() << indentStr + key + ": {";
            printJson(value.toObject(), indent + 4);  // 객체 재귀 호출
            qDebug().noquote() << indentStr + "}";
        } else if (value.isArray()) {
            qDebug().noquote() << indentStr + key + ": [";
            printArray(value.toArray(), indent + 4);  // 배열을 별도로 처리
            qDebug().noquote() << indentStr + "]";
        } else {
            qDebug().noquote() << indentStr + key + ": " + value.toVariant().toString();
        }
    }
}

void TestCase::printArray(const QJsonArray& jsonArray, int indent) {
    const QString indentStr(indent, ' ');  // 들여쓰기용 문자열

    for (const auto& element : jsonArray) {
        if (element.isObject()) {
            qDebug().noquote() << indentStr + "{";
            printJson(element.toObject(), indent + 4);  // 객체 재귀 호출
            qDebug().noquote() << indentStr + "}";
        } else if (element.isArray()) {
            qDebug().noquote() << indentStr + "[";
            printArray(element.toArray(), indent + 4);  // 배열 재귀 호출
            qDebug().noquote() << indentStr + "]";
        } else {
            qDebug().noquote() << indentStr + element.toVariant().toString();
        }
    }
}

void TestCase::appendCaseJson(QJsonObject& sumJson, const QJsonObject& newJson, int caseNumber) {
    QString key = QString("Case%1").arg(caseNumber);
    sumJson.insert(key, newJson);
}

void TestCase::addTcStr(const QJsonObject& TcNameJson) {
    // Do something...
    // for (const QString& caseKey : rootObject.keys()) {
    //     QJsonObject caseObject = rootObject.value(caseKey).toObject();
    //     QString caseName = caseObject.value("Case").toString();
    //     qDebug() << "Case Name:" << caseName;

    //     QJsonArray casesArray = caseObject.value("Cases").toArray();
    //     QJsonArray SignalList = caseObject.value("Input_Signal_List").toArray();
    //     for (const QJsonValue& caseValue : casesArray) {
    //         QJsonArray innerArray = caseValue.toArray();
    //         QStringList caseData;
    //         if (innerArray.size() == SignalList.size()) {
    //             for (int i = 0; i < innerArray.size(); i++) {
    //                 QString val = innerArray[i];
    //                 QString sig = SignalList[i];
    //                 if (mSignalDataInfo.contains(sig)) {
    //                     QStringList otherEnumList = mSignalDataInfo[sig]->getValueEnum();
    //                     if (otherEnumList.contains(val)) {
    //                         otherEnumList.removeAll(val);
    //                     } else {
    //                         qDebug() << "Error";
    //                     }

    //                 } else {
    //                     qDebug() << "Error";
    //                 }
    //                 otherEnumList
    //             }
    //         } else {
    //             qDebug() << "Error";
    //         }

    //         // for (const QJsonValue& value : innerArray) {
    //         //     caseData << value.toString();
    //         // }
    //         qDebug() << "Case Data:" << caseData;
    //     }
    // }
}

void TestCase::appendTCNameJson(const QString& newKey, const QJsonObject& newData, const QString& filePath) {
    QFile file(filePath);
    QJsonObject rootObject;

    // 기존 JSON 파일 열기 (읽기 모드)
    if (file.exists()) {
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Failed to open existing file for reading:" << file.errorString();
            return;
        }
        // 기존 JSON 읽기
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        rootObject = doc.object();
        file.close();
    } else {
        qDebug() << "File does not exist. A new file will be created.";
    }

    // 새 키와 데이터를 최상위에 추가
    rootObject[newKey] = newData;

    saveJsonToFile(rootObject, filePath);
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

// void TestCase::getOtherCase(const QJsonObject& json) {
//     callPython();
// }