#include "ExcelDataManger.h"

#include "CommonEnum.h"
#include "ConfigSetting.h"

QSharedPointer<ExcelDataManger>& ExcelDataManger::instance() {
    static QSharedPointer<ExcelDataManger> gControl;
    if (gControl.isNull()) {
        gControl = QSharedPointer<ExcelDataManger>(new ExcelDataManger());
    }
    return gControl;
}

ExcelDataManger::ExcelDataManger() {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});

    setMergeStart(mergeStart);
    setMerge(merge);
    setMergeEnd(mergeEnd);
    setMergeInfos(QStringList({mergeStart, merge, mergeEnd}));
}

QList<QStringList> ExcelDataManger::isSheetDataInfo() {
    QMap<int, QStringList> excelSheetData = isConvertedExcelData();
    int rowMax = 0;
    // int colummMax = static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max);
    int colummMax = (static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData) + 1);
    QStringList previousData;
    for (int columnIndex = 0; columnIndex < colummMax; ++columnIndex) {
        QStringList currentData = excelSheetData[columnIndex];
        excelSheetData[columnIndex] = isContainsMergeData(columnIndex, qMakePair(previousData, currentData));
        previousData = currentData;
        rowMax = (rowMax > excelSheetData[columnIndex].size()) ? (rowMax) : (excelSheetData[columnIndex].size());
    }

    QList<QStringList> sheetData;
    for (int index = 0; index < rowMax; ++index) {
        QStringList rowData;
        for (auto iter = excelSheetData.cbegin(); iter != excelSheetData.cend(); ++iter) {
            QStringList columnData = iter.value();
            if (index < columnData.size()) {
                rowData.append(columnData.at(index));
            }
        }
        sheetData.append(rowData);
    }

    qDebug() << "isSheetDataInfo :" << sheetData.size();

#if 0
    int index = 0;
    for (const auto& rowDataList : sheetData) {
        qDebug() << "RowData[" << index++ << "] :" << rowDataList.size()
                 << rowDataList;
                //  << rowDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal)]
                //  << rowDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)]
                //  << rowDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)];
                //  << rowDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)]
                //  << rowDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)]
                //  << rowDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal)]
                //  << rowDataList[static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue)];
    }
    qDebug() << "\n\n\n";
#endif

    return sheetData;
}

QMap<int, QStringList> ExcelDataManger::isConvertedExcelData() {
    // Read Type
    setReadStateNewData(true);

    // InsertData to ExcelData
    QMap<int, QStringList> excelSheetData;
    for (int columnIndex = 0; columnIndex < static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max); ++columnIndex) {
        excelSheetData[columnIndex] = isExcelSheetData(columnIndex);
    }

#if 0
    qDebug() << "\n\n\n\n";
    qDebug() << "**********************************************************************************************";
    int outputSize = 0;
    for (const auto& tcName : isTCNameDataList()) {
        for (const auto& resultName : isResultDataList(tcName)) {
            for (const auto& caseName : isCaseDataList(tcName, resultName)) {
                QPair<QStringList, QStringList> inputList = isInputDataList(tcName, resultName, caseName);
                QList<QStringList> outputList = isOutputDataList(tcName, resultName);
                qDebug() << "[DataTest]";
                qDebug() << "\t TCName     :" << tcName;
                qDebug() << "\t Result     :" << resultName;
                qDebug() << "\t Case       :" << caseName;
                qDebug() << "\t InputList  :" << inputList.first.size() << inputList.first;
                qDebug() << "\t OutputList :" << outputSize << outputList.size() << outputList.at(0);
                qDebug() << "\n";

                outputSize += outputList.size();
            }
        }
    }
    qDebug() << "OutputSize     :" << outputSize;
    qDebug() << "**********************************************************************************************";
    qDebug() << "\n\n\n\n";

#else
    QStringList tcNameData = excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)];
    QStringList resultData = excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)];
    QStringList inputSignal = excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal)];
    QMap<int, QPair<QString, QString>> tcNamResultInfo;
    QMap<int, QPair<int, int>> resultIndexInfo;

    QStringList tcNameList = isParsingDataList(tcNameData);
    for (const auto& tcName : tcNameList) {
        QPair<int, int> tcNameIndex = isIndexOf(tcNameData, tcName);
        if (tcNameIndex == qMakePair(1, 0)) {
            continue;
        }

        QStringList resultList;
        for (int rowIndex = tcNameIndex.first; rowIndex <= tcNameIndex.second; ++rowIndex) {
            QString text = resultData.at(rowIndex);
            resultList.append(text);
        }
        QStringList resultListTemp = resultList;
        resultListTemp.removeDuplicates();

        qDebug() << "**********************************************************************************************";
        qDebug() << "TCName :" << tcName;

        for (const auto& resultName : resultListTemp) {
            QPair<int, int> resultIndex = isIndexOf(resultList, resultName);
            if (resultIndex == qMakePair(1, 0)) {
                continue;
            }
            resultIndex = qMakePair((tcNameIndex.first + resultIndex.first), (tcNameIndex.first + resultIndex.second));
            int index = tcNamResultInfo.size();
            tcNamResultInfo[index] = qMakePair(tcName, resultName);
            resultIndexInfo[index] = qMakePair(resultIndex.first, resultIndex.second + 1);

            qDebug() << "\t Result :" << resultIndex << resultName;
        }
    }


    qDebug() << "\n\n";

    setReadStateNewData(false);
    for (int index = 0; index < tcNamResultInfo.size(); ++index) {
        QString tcName = tcNamResultInfo[index].first;
        QString result = tcNamResultInfo[index].second;
        QPair<int, int> resultRowInfo = resultIndexInfo[index];
        QList<QStringList> outputList = isOutputDataList(tcName, result);

        qDebug() << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++";
        qDebug() << "Info :" << resultRowInfo.first << resultRowInfo.second << tcName << result;
        qDebug() << "\t Output :" << outputList.size() << outputList;

        const int rowEnd = resultRowInfo.second - resultRowInfo.first;
        for (int rowIndex = 0; rowIndex < rowEnd; ++rowIndex) {
            bool state = (rowIndex < outputList.size());
            QString outputSignal = (state) ? (outputList.at(rowIndex).at(0)) : (QString());
            QString outputInit = (state) ? (outputList.at(rowIndex).at(1)) : (QString());
            QString outputData = (state) ? (outputList.at(rowIndex).at(2)) : (QString());

            excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal)].append(outputSignal);
            excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::IsInitialize)].append(outputInit);
            excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue)].append(outputData);

            excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::ConfigSignal)].append(QString());
            excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Data)].append(QString());
            excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::NegativeTest)].append(QString());

            qDebug() << "\t\t OutputList[" << rowIndex << "] :" << outputSignal;
        }
    }

#if 1
    qDebug() << "==============================================================================================";
    qDebug() << "DataSize :" << tcNameData.size() << resultData.size() << inputSignal.size()
             << excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal)].size();
    int index = 0;
    for (const auto& data : excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal)]) {
        qDebug() << "\t OutputInfo[" << index++ << "] :" << data;
    }
    qDebug() << "\n\n";
#endif
#endif

    return excelSheetData;
}

QStringList ExcelDataManger::isContainsMergeData(const int& columnIndex, const QPair<QStringList, QStringList> dataList) {
    const QString mergeStart = getMergeStart();
    const QString merge = getMerge();
    const QString mergeEnd = getMergeEnd();
    const QStringList mergeInfos = getMergeInfos();
    const QStringList previousData = dataList.first;
    const QStringList currentData = dataList.second;

    // qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    // qDebug() << "[isContainsMergeData] :" << columnIndex << currentData.size() << previousData.size();

    QStringList data;
    switch (columnIndex) {
        case static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal):
        case static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData): {
            data = currentData;
            break;
        }
        case static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal):
        case static_cast<int>(ivis::common::ExcelSheetTitle::Other::IsInitialize):
        case static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue): {
            // QList<QStringList> outputList = isOutputDataList(tcName, resultName);
            break;
        }
        case static_cast<int>(ivis::common::ExcelSheetTitle::Other::ConfigSignal):
        case static_cast<int>(ivis::common::ExcelSheetTitle::Other::Data):
        case static_cast<int>(ivis::common::ExcelSheetTitle::Other::NegativeTest): {
            break;
        }
        default: {
            for (int rowIndex = 0; rowIndex < currentData.size(); ++rowIndex) {
                QString text = currentData[rowIndex];
                int startIndex = currentData.indexOf(text);
                int endIndex = currentData.lastIndexOf(text);

                if (startIndex == endIndex) {
                    text = text;
                } else if (rowIndex == startIndex) {
                    text.prepend(mergeStart);
                } else if (rowIndex == endIndex) {
                    text.prepend(mergeEnd);
                } else {
                    text.prepend(merge);
                }
                data.append(text);
                // qDebug() << "\t MergeData[" << columnIndex << "][" << rowIndex << "] :" << startIndex << endIndex << text;
            }
            break;
        }
    }
    // qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";

    return data;
}

QStringList ExcelDataManger::isExcelSheetData(const int& columnIndex) {
    QStringList sheetData;
    if (getReadStateNewData()) {
        QMap<int, QStringList> newSheetData;
        for (const auto& data : readNewSheetData()) {
            QString tcName = data.isTCName();
            QString vehicleTYpe = data.isVehicleType();
            QString config = data.isConfig();
            QString resultName = data.isResultName();
            QString caseName = data.isCaseName();
            QPair<QStringList, QStringList> inputList = data.isInputList();

            for (int index = 0; index < inputList.first.size(); ++index) {
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)].append(tcName);
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)].append(vehicleTYpe);
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config)].append(config);
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)].append(resultName);
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)].append(caseName);

                QString inputSignal = inputList.first[index];
                QString inputData = inputList.second[index];
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal)].append(inputSignal);
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)].append(inputData);

                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal)].append(QString());
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::IsInitialize)].append(QString());
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue)].append(QString());
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::ConfigSignal)].append(QString());
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Data)].append(QString());
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::NegativeTest)].append(QString());
            }
        }
        sheetData = newSheetData[columnIndex];
    } else {
        sheetData = getExcelSheetData(columnIndex);
    }
    return sheetData;
}


QPair<int, int> ExcelDataManger::isIndexOf(const QStringList& dataList, const QString& data) {
    QPair<int, int> foundIndex(1, 0);
    int startIndex = dataList.indexOf(data);
    int endIndex = dataList.lastIndexOf(data);

    if ((data.size() > 0) && (startIndex >= 0) && (endIndex >= 0)) {
        foundIndex = qMakePair(startIndex, endIndex);
    }
    // qDebug() << "isIndexOf :" << foundIndex << data;
    return foundIndex;
}

QStringList ExcelDataManger::isParsingDataList(const QStringList& data, const bool& removeWhitespace) {
    QStringList parsingData;

    for (const auto& text : data) {
        if ((removeWhitespace) && (text.size() == 0)) {
            continue;
        }
        if (parsingData.contains(text)) {
            continue;
        }
        parsingData.append(text);
    }
    parsingData.removeDuplicates();

    return parsingData;
}

QPair<int, int> ExcelDataManger::isRowIndexInfo(const QString& tcName, const QString& resultName, const QString& caseName) {
    const QStringList tcNameData = isExcelSheetData(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
    const QStringList resultData = isExcelSheetData(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
    const QStringList caseData = isExcelSheetData(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case));

    QPair<int, int> rowInfo(1, 0);
    QPair<int, int> foundIndex(1, 0);
    QString logInfo("Default");

    // TCName Index
    if (tcName.size() > 0) {
        foundIndex = isIndexOf(tcNameData, tcName);
        if (foundIndex == qMakePair(1, 0)) {
            qDebug() << "Not found tcName :" << tcName;
            return foundIndex;
        }
        rowInfo = foundIndex;
        logInfo = QString("TCName");
    }

    // Reusult List & Index
    if (resultName.size() > 0) {
        QStringList resultList;
        for (int rowIndex = rowInfo.first; rowIndex <= rowInfo.second; ++rowIndex) {
            QString text = resultData.at(rowIndex);
            resultList.append(text);
        }
        foundIndex = isIndexOf(resultList, resultName);
        if (foundIndex == qMakePair(1, 0)) {
            qDebug() << "Not found result :" << resultName;
            return foundIndex;
        }
        rowInfo = qMakePair((foundIndex.first + rowInfo.first), (foundIndex.second + rowInfo.first));
        logInfo = QString("Result");
    }

    // Case List & Index
    if (caseName.size() > 0) {
        QStringList caseList;
        for (int rowIndex = rowInfo.first; rowIndex <= rowInfo.second; ++rowIndex) {
            QString text = caseData.at(rowIndex);
            caseList.append(text);
        }
        foundIndex = isIndexOf(caseList, caseName);
        if (foundIndex == qMakePair(1, 0)) {
            qDebug() << "Not found case :" << caseName;
            return foundIndex;
        }
        rowInfo = qMakePair((foundIndex.first + rowInfo.first), (foundIndex.second + rowInfo.first));
        logInfo = QString("Case");
    }

    // qDebug() << "\t >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    // qDebug() << "\t isRowIndexInfo :" << tcName << resultName << caseName;
    // qDebug() << "\t\t RowInfo :" << rowInfo.first << rowInfo.second << ", Type :" << logInfo;
    // qDebug() << "\t <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";

    return rowInfo;
}

QStringList ExcelDataManger::isTCNameDataList() {
    const QStringList currentData = isExcelSheetData(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));

    QStringList tcNameList = isParsingDataList(currentData);

    // qDebug() << "isTCNameDataList";
    // qDebug() << "\t Info :" << tcNameList;
    // qDebug() << "\n";

    return tcNameList;
}

QString ExcelDataManger::isVehicleTypeData(const QString& tcName) {
    const QStringList currentData = isExcelSheetData(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType));
    const QPair<int, int> rowInfo = isRowIndexInfo(tcName, QString(), QString());

    QStringList list;
    for (int rowIndex = rowInfo.first; rowIndex <= rowInfo.second; ++rowIndex) {
        QString text = currentData.at(rowIndex);
        list.append(text);
        // qDebug() << "\t Result[" << rowIndex << "] :" << text;
    }
    QStringList vehicleTypeList = isParsingDataList(list);
    QString vehicleType = (vehicleTypeList.size() > 0) ? (vehicleTypeList.at(0)) : (QString());

    // qDebug() << "isVehicleTypeData";
    // qDebug() << "\t Info :" << vehicleType.size() << vehicleType;
    // qDebug() << "\n";

    return vehicleType;
}

QString ExcelDataManger::isConfigData(const QString& tcName) {
    const QStringList currentData = isExcelSheetData(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config));
    const QPair<int, int> rowInfo = isRowIndexInfo(tcName, QString(), QString());

    QStringList list;
    for (int rowIndex = rowInfo.first; rowIndex <= rowInfo.second; ++rowIndex) {
        QString text = currentData.at(rowIndex);
        list.append(text);
        // qDebug() << "\t Result[" << rowIndex << "] :" << text;
    }
    QStringList configList = isParsingDataList(list);
    QString config = (configList.size() > 0) ? (configList.at(0)) : (QString());

    // qDebug() << "isConfigDataList";
    // qDebug() << "\t Info :" << config.size() << config;
    // qDebug() << "\n";

    return config;
}

QStringList ExcelDataManger::isResultDataList(const QString& tcName) {
    const QStringList currentData = isExcelSheetData(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
    const QPair<int, int> rowInfo = isRowIndexInfo(tcName, QString(), QString());

    QStringList list;
    for (int rowIndex = rowInfo.first; rowIndex <= rowInfo.second; ++rowIndex) {
        QString text = currentData.at(rowIndex);
        list.append(text);
        // qDebug() << "\t Result[" << rowIndex << "] :" << text;
    }
    QStringList resultList = isParsingDataList(list);

    // qDebug() << "isResultDataList :" << tcName;
    // qDebug() << "\t Info :" << resultList.size() << resultList;
    // qDebug() << "\n";

    return resultList;
}

QStringList ExcelDataManger::isCaseDataList(const QString& tcName, const QString& resultName) {
    const QStringList currentData = isExcelSheetData(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case));
    const QPair<int, int> rowInfo = isRowIndexInfo(tcName, resultName, QString());

    QStringList list;
    for (int rowIndex = rowInfo.first; rowIndex <= rowInfo.second; ++rowIndex) {
        QString text = currentData.at(rowIndex);
        list.append(text);
        // qDebug() << "\t Case[" << rowIndex << "] :" << text;
    }

    QStringList caseList = isParsingDataList(list, false);

    // qDebug() << "isCaseDataList :" << tcName << resultName;
    // qDebug() << "\t Info :" << caseList.size() << caseList;
    // qDebug() << "\n";

    return caseList;
}

QPair<QStringList, QStringList> ExcelDataManger::isInputDataList(const QString& tcName, const QString& resultName,
                                                                 const QString& caseName) {
    const QStringList inputSignal = isExcelSheetData(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal));
    const QStringList inputData = isExcelSheetData(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData));

    const QPair<int, int> rowInfo = isRowIndexInfo(tcName, resultName, caseName);

    QStringList signalInfo;
    QStringList dataInfo;
    for (int rowIndex = rowInfo.first; rowIndex <= rowInfo.second; ++rowIndex) {
        signalInfo.append(inputSignal.at(rowIndex));
        dataInfo.append(inputData.at(rowIndex));
    }
    QPair<QStringList, QStringList> inputList = qMakePair(signalInfo, dataInfo);
    // case(others) 인 경우 inputList 가 있는 경우 일반적인 others 키워드로 동작 하지 않음
    if ((caseName.compare("others") == false) && (inputList.first.size() == 1)) {
        if (inputList.first.at(0).size() == 0) {
            inputList.first.clear();
            inputList.second.clear();
        }
    }

    // qDebug() << "isInputDataList :" << tcName << resultName << caseName;
    // qDebug() << "\t Info :" << inputList.first.size() << inputList.first << inputList.second.size() << inputList.second;
    // qDebug() << "\n";

    return inputList;
}

QList<QStringList> ExcelDataManger::isOutputDataList(const QString& tcName, const QString& resultName) {
    const QStringList outputSignal = isExcelSheetData(static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal));
    const QStringList outputInit = isExcelSheetData(static_cast<int>(ivis::common::ExcelSheetTitle::Other::IsInitialize));
    const QStringList outputData = isExcelSheetData(static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue));
    const QPair<int, int> rowInfo = isRowIndexInfo(tcName, resultName, QString());

    QList<QStringList> outputList;
    for (int rowIndex = rowInfo.first; rowIndex <= rowInfo.second; ++rowIndex) {
        outputList.append({outputSignal.at(rowIndex), outputInit.at(rowIndex), outputData.at(rowIndex)});
    }

    qDebug() << "isOutputDataList :" << tcName << resultName;
    qDebug() << "\t Info :" << outputList.size() << outputList;
    qDebug() << "\n";

    return outputList;
}

int ExcelDataManger::isCaseIndex(const QString& tcName, const QString& resultName, const QString& caseName) {
    int caseIndex = 0;
    for (const auto& data : readNewSheetData()) {
        QString currentTCName = data.isTCName();
        QString currentResult = data.isResultName();
        QString currentCase = data.isCaseName();

        ivis::common::getRemoved(currentTCName, getMergeInfos());
        ivis::common::getRemoved(currentResult, getMergeInfos());
        ivis::common::getRemoved(currentCase, getMergeInfos());

        if ((currentTCName.compare(tcName) == false) && (currentResult.compare(resultName) == false) &&
            (currentCase.compare(caseName) == false)) {
            break;
        }
        caseIndex++;
    }
    return caseIndex;
}

void ExcelDataManger::updateExcelData(const QVariantList& sheetData) {
    const int rowMax = sheetData.size();
    // const int columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue) + 1;
    const int columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max);

    QMap<int, QStringList> excelSheetData;
    for (const auto& rowDataList : sheetData.toList()) {
        QStringList rowData = rowDataList.toStringList();
        if (rowData.size() != (static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max))) {
            continue;
        }

        for (int columnIndex = 0; columnIndex < columnMax; ++columnIndex) {
            QString readColumnText = rowData.at(columnIndex);
            ivis::common::getRemoved(readColumnText, getMergeInfos());
            excelSheetData[columnIndex].append(readColumnText);
        }
    }

    clear();
    setReadStateNewData(false);

    if (excelSheetData.size() == 0) {
        qDebug() << "Not support sheet data : check sheet(description or config)";
        return;
    }

#if 1
    qDebug() << "updateExcelData :" << sheetData.size();
    // for (auto iter = excelSheetData.cbegin(); iter != excelSheetData.cend(); ++iter) {
    //     qDebug() << "\t ExcelData[" << iter.key() << "] :" << iter.value().size();
    // }
    qDebug() << "\n\n";
#endif

    writeExcelSheetData(excelSheetData);
}

void ExcelDataManger::updateCaseDataInfo(const QString& tcName, const QString& resultName, const QString& caseName,
                                         const QPair<QStringList, QStringList>& inputList) {
    if ((tcName.size() == 0) || (resultName.size() == 0) ||(caseName.size() == 0)) {
        qDebug() << "Fail to update info size :" << tcName.size() << resultName.size() << caseName.size();
        return;
    }

    QString vehicleType = isVehicleTypeData(tcName);
    QString config = isConfigData(tcName);
    InsertData insertData = InsertData(tcName, vehicleType, config, resultName, caseName, inputList);
    setNewSheetData(isSizeNewSheetData(), insertData);

#if 1
    qDebug() << "[updateCaseDataInfo]";
    qDebug() << "\t TCName    :" << tcName;
    qDebug() << "\t Result    :" << resultName;
    qDebug() << "\t Case      :" << caseName;
    qDebug() << "\t InputList :" << inputList.first.size() << inputList.first;
    qDebug() << "\n";
#endif
}

void ExcelDataManger::insertCaseDataInfo(const QString& tcName, const QString& resultName, const QString& caseName,
                                         const QPair<QStringList, QStringList>& inputList, const QString& baseCaseName,
                                         const bool& insertBefore) {
    if ((tcName.size() == 0) || (resultName.size() == 0) ||(caseName.size() == 0)) {
        qDebug() << "Fail to update info size :" << tcName.size() << resultName.size() << caseName.size();
        return;
    }

    QString vehicleType = isVehicleTypeData(tcName);
    QString config = isConfigData(tcName);
    InsertData insertData(tcName, vehicleType, config, resultName, caseName, inputList);
    int caseIndex = isCaseIndex(tcName, resultName, baseCaseName);
    caseIndex = (insertBefore) ? (caseIndex) : (caseIndex + 1);
    setNewSheetData(caseIndex, InsertData(tcName, vehicleType, config, resultName, caseName, inputList));

#if 0
    qDebug() << "[updateCaseDataInfo]";
    qDebug() << "\t caseIndex :" << caseIndex;
    qDebug() << "\t TCName    :" << tcName;
    qDebug() << "\t Result    :" << resultName;
    qDebug() << "\t Case      :" << caseName;
    qDebug() << "\t InputList :" << inputList.first.size() << inputList.first;
    qDebug() << "\n";
#endif
}

void ExcelDataManger::clear() {
    clearNewSheetData();
    clearExcelSheetData();
}
