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
    for (auto iter = excelSheetData.cbegin(); iter != excelSheetData.cend(); ++iter) {
        rowMax = (rowMax > iter.value().size()) ? (rowMax) : (iter.value().size());
    }

    QList<QStringList> sheetData;
    sheetData.reserve(rowMax);
    for (int index = 0; index < rowMax; ++index) {
        QStringList rowData;
        for (const auto &columnData : excelSheetData) {
            if (index < columnData.size()) {
                rowData.append(columnData.at(index));
            }
        }
        sheetData.append(rowData);
    }
    qDebug() << "isSheetDataInfo :" << excelSheetData.size() << rowMax << sheetData.size();
    return sheetData;
}

QMap<int, QStringList> ExcelDataManger::isConvertedExcelData() {
    // Read Type
    setReadStateNewData(true);
    // InsertData to ExcelData
    QMap<int, QStringList> excelSheetData;
    QMap<QPair<QString, QString>, QList<QStringList>> outputListInfo;
    QMap<int, QList<QPair<int, int>>> mergeIndex;
    QString previousTCName;
    QString previousResult;
    QString previousCase;

    for (const auto& data : readNewSheetData()) {
        QString tcName = data.isTCName();
        QString check = data.isCheck();
        QString genType = data.isGenType();
        QString vehicleType = data.isVehicleType();
        QString config = data.isConfig();
        QString resultName = data.isResultName();
        QString caseName = data.isCaseName();
        QPair<QStringList, QStringList> inputList = data.isInputList();
        QPair<int, int> rowIndex;

        if (previousTCName != tcName) {
            rowIndex = isRowIndexInfo(tcName, QString(), QString());
            mergeIndex[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check)].append(rowIndex);
            mergeIndex[static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)].append(rowIndex);
            mergeIndex[static_cast<int>(ivis::common::ExcelSheetTitle::Other::GenType)].append(rowIndex);
            mergeIndex[static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)].append(rowIndex);
            mergeIndex[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config)].append(rowIndex);
            previousResult.clear();
        }
        if (previousResult != resultName) {
            rowIndex = isRowIndexInfo(tcName, resultName, QString());
            mergeIndex[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)].append(rowIndex);
            previousCase.clear();

            QList<QStringList> outputList = data.isOutputList();
            if (outputList.size() > 0) {
                outputListInfo[qMakePair(tcName, resultName)] = outputList;
            }
        }
        if (previousCase != caseName) {
            rowIndex = isRowIndexInfo(tcName, resultName, caseName);
            mergeIndex[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)].append(rowIndex);
        }
        previousTCName = tcName;
        previousResult = resultName;
        previousCase = caseName;

        for (int index = 0; index < inputList.first.size(); ++index) {
            excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check)].append(check);
            excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)].append(tcName);
            excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::GenType)].append(genType);
            excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)].append(vehicleType);
            excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config)].append(config);
            excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)].append(resultName);
            excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)].append(caseName);

            QString inputSignal = inputList.first[index];
            QString inputData = inputList.second[index];
            excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal)].append(inputSignal);
            excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)].append(inputData);

            excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal)].append(QString());
            excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::IsInitialize)].append(QString());
            excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue)].append(QString());

#if 0    // USE_APPEND_SHEET_COLUMN
            excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::ConfigSignal)].append(QString());
            excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Data)].append(QString());
            excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::NegativeTest)].append(QString());
#endif
        }
    }

    for (auto iter = mergeIndex.cbegin(); iter != mergeIndex.cend(); ++iter) {
        int columnIndex = iter.key();
        QStringList& columnData = excelSheetData[columnIndex];
        for (const auto& merge : iter.value()) {
            int start = merge.first;
            int end = merge.second;
            int cout = end - start + 1;

            if (cout == 1) {
                continue;
            }
            for (int index = start; index <= end; ++index) {
                if (index >= columnData.size()) {
                    continue;
                }
                if (index == start) {
                    columnData[index].prepend(getMergeStart());
                } else if (index == end) {
                    columnData[index].prepend(getMergeEnd());
                } else {
                    columnData[index].prepend(getMerge());
                }
            }
        }
    }

    QStringList& outputSignal = excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal)];
    QStringList& outputInit = excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::IsInitialize)];
    QStringList& outputData = excelSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue)];
    for (auto iter = outputListInfo.cbegin(); iter != outputListInfo.cend(); ++iter) {
        QPair<int, int> rowIndex = isRowIndexInfo(iter.key().first, iter.key().second, QString());
        int start = rowIndex.first;
        // int end = rowIndex.second;
        for (const auto& data : iter.value()) {
            outputSignal[start] = data.at(0);
            outputInit[start] = data.at(1);
            outputData[start] = data.at(2);
            start++;
        }
    }

    return excelSheetData;
}

QStringList ExcelDataManger::isExcelSheetData(const int& columnIndex) {
    QStringList sheetData;
    if (getReadStateNewData()) {
        QMap<int, QStringList> newSheetData;
        for (const auto& data : readNewSheetData()) {
            QString tcName = data.isTCName();
            QString check = data.isCheck();
            QString genType = data.isGenType();
            QString vehicleTYpe = data.isVehicleType();
            QString config = data.isConfig();
            QString resultName = data.isResultName();
            QString caseName = data.isCaseName();
            QPair<QStringList, QStringList> inputList = data.isInputList();
            for (int index = 0; index < inputList.first.size(); ++index) {
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check)].append(check);
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)].append(tcName);
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::GenType)].append(genType);
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)].append(vehicleTYpe);
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config)].append(config);
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)].append(resultName);
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)].append(caseName);

                QString inputSignal = inputList.first[index];
                QString inputData = inputList.second[index];
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal)].append(inputSignal);
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)].append(inputData);
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

QString ExcelDataManger::isCheckData(const QString& tcName) {
    const QStringList currentData = isExcelSheetData(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check));
    const QPair<int, int> rowInfo = isRowIndexInfo(tcName, QString(), QString());

    QStringList list;
    for (int rowIndex = rowInfo.first; rowIndex <= rowInfo.second; ++rowIndex) {
        QString text = currentData.at(rowIndex);
        list.append(text);
        // qDebug() << "\t Result[" << rowIndex << "] :" << text;
    }
    QStringList checkList = isParsingDataList(list);
    QString check = (checkList.size() > 0) ? (checkList.at(0)) : (QString());

    // qDebug() << "isCheckData";
    // qDebug() << "\t Info :" << checkList.size() << check;
    // qDebug() << "\n";

    return check;
}

QString ExcelDataManger::isGenTypeData(const QString& tcName) {
    const QStringList currentData = isExcelSheetData(static_cast<int>(ivis::common::ExcelSheetTitle::Other::GenType));
    const QPair<int, int> rowInfo = isRowIndexInfo(tcName, QString(), QString());

    QStringList list;
    for (int rowIndex = rowInfo.first; rowIndex <= rowInfo.second; ++rowIndex) {
        QString text = currentData.at(rowIndex);
        list.append(text);
        // qDebug() << "\t Result[" << rowIndex << "] :" << text;
    }
    QStringList genTypeList = isParsingDataList(list);
    QString genType = (genTypeList.size() > 0) ? (genTypeList.at(0)) : (QString());

    // qDebug() << "isGenTypeData";
    // qDebug() << "\t Info :" << genTypeList.size() << genType;
    // qDebug() << "\n";

    return genType;
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
        if ((rowIndex >= inputSignal.size()) || (rowIndex >= inputData.size())) {
            continue;
        }
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
    const bool readState = getReadStateNewData();
    setReadStateNewData(false);
    const QStringList outputSignal = isExcelSheetData(static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal));
    const QStringList outputInit = isExcelSheetData(static_cast<int>(ivis::common::ExcelSheetTitle::Other::IsInitialize));
    const QStringList outputData = isExcelSheetData(static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue));
    const QPair<int, int> rowInfo = isRowIndexInfo(tcName, resultName, QString());
    setReadStateNewData(readState);

    QList<QStringList> outputList;
    for (int rowIndex = rowInfo.first; rowIndex <= rowInfo.second; ++rowIndex) {
        if ((rowIndex >= outputSignal.size()) || (rowIndex >= outputInit.size()) || (rowIndex >= outputData.size())) {
            continue;
        }
        QString signal = outputSignal.at(rowIndex);
        QString init = outputInit.at(rowIndex);
        QString data = outputData.at(rowIndex);
        if ((signal.size() == 0) || (data.size() == 0)) {
            continue;
        }
        outputList.append({signal, init, data});
    }

    // qDebug() << "isOutputDataList :" << tcName << resultName;
    // qDebug() << "\t Info :" << outputList.size() << outputList;
    // qDebug() << "\n";

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

#if 1
    qDebug() << "updateExcelData :" << sheetData.size() << excelSheetData.size();
    // for (auto iter = excelSheetData.cbegin(); iter != excelSheetData.cend(); ++iter) {
    //     qDebug() << "\t ExcelData[" << iter.key() << "] :" << iter.value().size();
    // }
    qDebug() << "\n\n";
#endif

    clear();
    setReadStateNewData(false);

    if (excelSheetData.size() == 0) {
        qDebug() << "Not support sheet(description or config) data : 0";
        return;
    }

    writeExcelSheetData(excelSheetData);
}

void ExcelDataManger::updateCaseDataInfo(const QString& tcName, const QString& resultName, const QString& caseName,
                                         const QPair<QStringList, QStringList>& inputList) {
    if ((tcName.size() == 0) || (resultName.size() == 0) ||(caseName.size() == 0)) {
        qDebug() << "Fail to update info size :" << tcName.size() << resultName.size() << caseName.size();
        return;
    }

    QString check = isCheckData(tcName);
    QString genType = isGenTypeData(tcName);
    QString vehicleType = isVehicleTypeData(tcName);
    QString config = isConfigData(tcName);
    QList<QStringList> outputList = isOutputDataList(tcName, resultName);
    int caseIndex = isSizeNewSheetData();
    InsertData insertData(tcName, check, genType, vehicleType, config, resultName, caseName, inputList, outputList);
    setNewSheetData(caseIndex, insertData);

#if 0
    qDebug() << "[updateCaseDataInfo]";
    // qDebug() << "\t caseIndex   :" << caseIndex;
    qDebug() << "\t TCName      :" << tcName;
    qDebug() << "\t Result      :" << resultName;
    qDebug() << "\t Case        :" << caseName;
    // qDebug() << "\t InputList   :" << inputList.first.size() << inputList.first;
    qDebug() << "\t InputSignal :" << inputList.first.size() << inputList.first;
    qDebug() << "\t OutputList  :" << outputList.size() << outputList;
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

    QString check = isCheckData(tcName);
    QString genType = isGenTypeData(tcName);
    QString vehicleType = isVehicleTypeData(tcName);
    QString config = isConfigData(tcName);
    QList<QStringList> outputList = isOutputDataList(tcName, resultName);
    int caseIndex = isCaseIndex(tcName, resultName, baseCaseName);
    caseIndex = (insertBefore) ? (caseIndex) : (caseIndex + 1);
    InsertData insertData(tcName, check, genType, vehicleType, config, resultName, caseName, inputList, outputList);
    setNewSheetData(caseIndex, insertData);

#if 0
    qDebug() << "[insertCaseDataInfo]";
    // qDebug() << "\t caseIndex   :" << caseIndex;
    qDebug() << "\t TCName      :" << tcName;
    qDebug() << "\t Result      :" << resultName;
    qDebug() << "\t Case        :" << caseName;
    // qDebug() << "\t InputList   :" << inputList.first.size() << inputList.first;
    qDebug() << "\t InputSignal :" << inputList.first.size() << inputList.first;
    qDebug() << "\t OutputList  :" << outputList.size() << outputList;
    qDebug() << "\n";
#endif
}

void ExcelDataManger::clear() {
    clearNewSheetData();
    clearExcelSheetData();
}
