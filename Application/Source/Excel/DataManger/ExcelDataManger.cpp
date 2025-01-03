#include "ExcelDataManger.h"

#include "CommonEnum.h"
#include "ConfigSetting.h"
#include "ExcelUtil.h"

QSharedPointer<ExcelDataManger>& ExcelDataManger::instance() {
    static QSharedPointer<ExcelDataManger> gManger;
    if (gManger.isNull()) {
        gManger = QSharedPointer<ExcelDataManger>(new ExcelDataManger());
    }
    return gManger;
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

#if defined(USE_SHEET_COLUMN_OLD)
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

QStringList ExcelDataManger::isExcelDataOther(const int& columnIndex) {
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
        sheetData = getExcelDataOther(columnIndex);
    }

    return sheetData;
}

QStringList ExcelDataManger::isExcelDataConfig(const int& columnIndex) {
    QStringList sheetData = getExcelDataConfig(columnIndex);
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
    const QStringList tcNameData = isExcelDataOther(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
    const QStringList resultData = isExcelDataOther(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
    const QStringList caseData = isExcelDataOther(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case));

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

QStringList ExcelDataManger::isTCNameDataList(const bool& all) {
    const QStringList currentData = isExcelDataOther(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));

    QStringList tcNameList;
    for (const auto& tcName : isParsingDataList(currentData, true)) {
        if (all) {
            tcNameList.append(tcName);
        } else {
            if (isCheckData(tcName)) {
                tcNameList.append(tcName);
            }
        }
    }

    // qDebug() << "isTCNameDataList :" << all;
    // qDebug() << "\t Info :" << tcNameList.size() << tcNameList;
    // qDebug() << "\n";

    return tcNameList;
}

bool ExcelDataManger::isCheckData(const QString& tcName) {
    const QStringList currentData = isExcelDataOther(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check));
    const QPair<int, int> rowInfo = isRowIndexInfo(tcName, QString(), QString());

    QStringList list;
    for (int rowIndex = rowInfo.first; rowIndex <= rowInfo.second; ++rowIndex) {
        QString text = currentData.at(rowIndex);
        list.append(text);
        // qDebug() << "\t Result[" << rowIndex << "] :" << text;
    }
    QStringList checkList = isParsingDataList(list, true);
    bool check = ((checkList.size() > 0) && (checkList.at(0).compare("") != false));

    // qDebug() << "isCheckData :" << tcName;
    // qDebug() << "\t Info :" << check << checkList.size() << checkList;
    // qDebug() << "\n";

    return check;
}

QString ExcelDataManger::isGenTypeData(const QString& tcName) {
    const QStringList currentData = isExcelDataOther(static_cast<int>(ivis::common::ExcelSheetTitle::Other::GenType));
    const QPair<int, int> rowInfo = isRowIndexInfo(tcName, QString(), QString());

    QStringList list;
    for (int rowIndex = rowInfo.first; rowIndex <= rowInfo.second; ++rowIndex) {
        QString text = currentData.at(rowIndex);
        list.append(text);
        // qDebug() << "\t Result[" << rowIndex << "] :" << text;
    }
    QStringList genTypeList = isParsingDataList(list, true);
    QString genType = (genTypeList.size() > 0) ? (genTypeList.at(0)) : (QString());

    // qDebug() << "isGenTypeData :" << tcName;
    // qDebug() << "\t Info :" << genTypeList.size() << genType;
    // qDebug() << "\n";

    return genType;
}

QString ExcelDataManger::isVehicleTypeData(const QString& tcName) {
    const QStringList currentData = isExcelDataOther(static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType));
    const QPair<int, int> rowInfo = isRowIndexInfo(tcName, QString(), QString());

    QStringList list;
    for (int rowIndex = rowInfo.first; rowIndex <= rowInfo.second; ++rowIndex) {
        QString text = currentData.at(rowIndex);
        list.append(text);
        // qDebug() << "\t Result[" << rowIndex << "] :" << text;
    }
    QStringList vehicleTypeList = isParsingDataList(list, true);
    QString vehicleType = (vehicleTypeList.size() > 0) ? (vehicleTypeList.at(0)) : (QString());

    // qDebug() << "isVehicleTypeData";
    // qDebug() << "\t Info :" << vehicleType.size() << vehicleType;
    // qDebug() << "\n";

    return vehicleType;
}

QString ExcelDataManger::isConfigData(const QString& tcName) {
    const QStringList currentData = isExcelDataOther(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config));
    const QPair<int, int> rowInfo = isRowIndexInfo(tcName, QString(), QString());

    QStringList list;
    for (int rowIndex = rowInfo.first; rowIndex <= rowInfo.second; ++rowIndex) {
        QString text = currentData.at(rowIndex);
        list.append(text);
        // qDebug() << "\t Result[" << rowIndex << "] :" << text;
    }
    QStringList configList = isParsingDataList(list, true);
    QString config = (configList.size() > 0) ? (configList.at(0)) : (QString());

    // qDebug() << "isConfigDataList";
    // qDebug() << "\t Info :" << config.size() << config;
    // qDebug() << "\n";

    return config;
}

QStringList ExcelDataManger::isResultDataList(const QString& tcName) {
    const QStringList currentData = isExcelDataOther(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
    const QPair<int, int> rowInfo = isRowIndexInfo(tcName, QString(), QString());

    QStringList list;
    for (int rowIndex = rowInfo.first; rowIndex <= rowInfo.second; ++rowIndex) {
        QString text = currentData.at(rowIndex);
        list.append(text);
        // qDebug() << "\t Result[" << rowIndex << "] :" << text;
    }
    QStringList resultList = isParsingDataList(list, true);

    // qDebug() << "isResultDataList :" << tcName;
    // qDebug() << "\t Info :" << resultList.size() << resultList;
    // qDebug() << "\n";

    return resultList;
}

QStringList ExcelDataManger::isCaseDataList(const QString& tcName, const QString& resultName) {
    const QStringList currentData = isExcelDataOther(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case));
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
                                                                 const QString& caseName, const bool& removeWhitespace) {
    const QStringList inputSignal = isExcelDataOther(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal));
    const QStringList inputData = isExcelDataOther(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData));
    const QPair<int, int> rowInfo = isRowIndexInfo(tcName, resultName, caseName);

    QStringList signalInfo;
    QStringList dataInfo;
    for (int rowIndex = rowInfo.first; rowIndex <= rowInfo.second; ++rowIndex) {
        if ((rowIndex >= inputSignal.size()) || (rowIndex >= inputData.size())) {
            continue;
        }
        QString signal = inputSignal.at(rowIndex);
        QString data = inputData.at(rowIndex);
        bool appendState = (removeWhitespace) ? (signal.size() > 0) : (true);
        if (appendState) {
            signalInfo.append(signal);
            dataInfo.append(data);
        }
    }
    QPair<QStringList, QStringList> inputList = qMakePair(signalInfo, dataInfo);
    QString others = ExcelUtil::instance()->isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Other));
    bool tcNameBaseState = (tcName.size() > 0) && (resultName.size() == 0) && (caseName.size() == 0);
    if (tcNameBaseState) {
        QStringList caseList = isCaseDataList(tcName, QString());
        if (caseList.contains(others)) {
            inputList = QPair<QStringList, QStringList>();
            // qDebug() << "Found others case in case list :" << others << inputList.first.size();
        }
    } else {
        // case(others) 인 경우 inputList 가 있는 경우 일반적인 others 키워드로 동작 하지 않음
        if ((caseName.compare(others) == false) && (inputList.first.size() == 1)) {
            if (inputList.first.at(0).size() == 0) {
                inputList.first.clear();
                inputList.second.clear();
            }
        }
    }

    // qDebug() << "isInputDataList :" << tcName << resultName << caseName << removeWhitespace;
    // qDebug() << "\t Info :" << inputList.first.size() << inputList.first << inputList.second.size() << inputList.second;
    // qDebug() << "\n";

    return inputList;
}

QList<QStringList> ExcelDataManger::isOutputDataList(const QString& tcName, const QString& resultName) {
    const bool readState = getReadStateNewData();
    setReadStateNewData(false);
    const QStringList outputSignal = isExcelDataOther(static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal));
    const QStringList outputInit = isExcelDataOther(static_cast<int>(ivis::common::ExcelSheetTitle::Other::IsInitialize));
    const QStringList outputData = isExcelDataOther(static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue));
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

QList<QStringList> ExcelDataManger::isConfigDataList(const QString& configName, const bool& allData) {
    const QStringList configNameList = isExcelDataConfig(static_cast<int>(ivis::common::ExcelSheetTitle::Config::ConfigName));
    const QStringList andGroupList = isExcelDataConfig(static_cast<int>(ivis::common::ExcelSheetTitle::Config::AndGroup));
    const QStringList inputSignalList = isExcelDataConfig(static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputSignal));
    const QStringList inputDataList = isExcelDataConfig(static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputData));

    QStringList tempConfigNameList;
    for (auto configInfo : configNameList) {
        ivis::common::getRemoved(configInfo, getMergeInfos());
        tempConfigNameList.append(configInfo);
    }

    QList<QStringList> dataInfo;
    QPair<int, int> foundIndex = isIndexOf(tempConfigNameList, (configName.size() == 0) ? (QString("Default")) : (configName));

    if (foundIndex == qMakePair(1, 0)) {
        qDebug() << "Not found configName :" << configName;
        return dataInfo;
    }

    for (int index = foundIndex.first; index <= foundIndex.second; ++index) {
        QStringList data;
        if (allData) {
            data.append((index < configNameList.size()) ? (configNameList.at(index)) : (QString()));
            data.append((index < andGroupList.size()) ? (andGroupList.at(index)) : (QString()));
        }
        data.append((index < inputSignalList.size()) ? (inputSignalList.at(index)) : (QString()));
        data.append((index < inputDataList.size()) ? (inputDataList.at(index)) : (QString()));
        dataInfo.append(data);
    }

    // qDebug() << "isConfigDataList :" << configName << foundIndex;
    // for (const auto& info : dataInfo) {
    //     qDebug() << "\t Info :" << info;
    // }
    // qDebug() << "\n";

    return dataInfo;
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

void ExcelDataManger::updateExcelData(const int& sheetIndex, const QVariantList& sheetData) {
    qDebug() << "updateExcelData :" << sheetIndex << sheetData.size();

    if ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) ||
        (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConfigs)) {
        updateExcelDataConfig(sheetData);
    } else if ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) ||
               (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription)) {
        // do nothing
    } else {
        updateExcelDataOther(sheetData);
    }
}

void ExcelDataManger::updateExcelDataOther(const QVariantList& sheetData) {
    const int columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max);

    QMap<int, QStringList> excelSheetData;
    for (const auto& rowDataList : sheetData.toList()) {
        QStringList rowData = rowDataList.toStringList();
        if (rowData.size() != columnMax) {
            continue;
        }

        for (int columnIndex = 0; columnIndex < columnMax; ++columnIndex) {
            QString readColumnText = rowData.at(columnIndex);
            ivis::common::getRemoved(readColumnText, getMergeInfos());
            excelSheetData[columnIndex].append(readColumnText);
        }
    }

    setReadStateNewData(false);
    clearNewSheetData();
    clearExcelDataOther();

    if (excelSheetData.size() == 0) {
        qDebug() << "Fail to other sheet data size : 0";
        return;
    }

    writeExcelDataOther(excelSheetData);
}

void ExcelDataManger::updateExcelDataConfig(const QVariantList& sheetData) {
    const int columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::Config::Max);

    QMap<int, QStringList> excelSheetData;
    for (const auto& rowDataList : sheetData.toList()) {
        QStringList rowData = rowDataList.toStringList();
        if (rowData.size() != columnMax) {
            continue;
        }

        for (int columnIndex = 0; columnIndex < columnMax; ++columnIndex) {
            QString readColumnText = rowData.at(columnIndex);
            // ivis::common::getRemoved(readColumnText, getMergeInfos());
            excelSheetData[columnIndex].append(readColumnText);
        }
    }

    if (excelSheetData.size() == 0) {
        qDebug() << "Fail to config sheet data size : 0";
        return;
    }

    writeExcelDataConfig(excelSheetData);
}

void ExcelDataManger::updateCaseDataInfo(const QString& tcName, const QString& resultName, const QString& caseName,
                                         const QPair<QStringList, QStringList>& inputList) {
    if ((tcName.size() == 0) || (resultName.size() == 0) ||(caseName.size() == 0)) {
        qDebug() << "Fail to update info size :" << tcName.size() << resultName.size() << caseName.size();
        return;
    }

    QString check = (isCheckData(tcName)) ? (QString("O")) : (QString());
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

    QString check = (isCheckData(tcName)) ? (QString("O")) : (QString());
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

bool ExcelDataManger::isValidConfigCheck(const bool& other, const QString& configName, const QMap<QString, QString>& inputList) {
    bool result = true;
    QList<QStringList> configList = isConfigDataList(configName, other);

    QStringList inputSignal;
    QStringList inputDatas;

    QMap<int, QPair<QStringList, QStringList>> inputMap;

    for (const auto& config : configList) {
        if (config.size() == 2) {
            inputSignal.append(config.at(0));
            inputDatas.append(config.at(1));
        } else if (config.size() == 4) {
            QString andGroup = config.at(1);
            inputSignal.append(config.at(2));
            inputDatas.append(config.at(3));

            if ((andGroup.compare(getMergeStart())) && (andGroup.compare(getMerge()))) {
                inputMap[inputMap.size()] = qMakePair(inputSignal, inputDatas);
                inputSignal.clear();
                inputDatas.clear();
            }
        } else {
        }
    }

    if (other) {
        for (auto iter = inputMap.cbegin(); iter != inputMap.cend(); ++iter) {
            qDebug() << "InputData :" << iter.key() << iter.value().first << iter.value().second;
        }

        // // Additional condition processing when other is true
        // for (auto iter = inputList.cbegin(); iter != inputList.cend(); ++iter) {
        //     QString signal = iter.key();
        //     QString data = iter.value();
        //     bool found = false;

        //     for (const auto& entry : inputMap) {
        //         if (entry.first.contains(signal) && entry.second.contains(data)) {
        //             found = true;
        //             break;
        //         }
        //     }

        //     if (!found) {
        //         result = false;
        //         break;
        //     }
        // }
    } else {
        for (auto iter = inputList.cbegin(); iter != inputList.cend(); ++iter) {
            QString signal = iter.key();
            QString data = iter.value();
            if (inputSignal.contains(signal) == false) {
                result = false;
                break;
            }
            int signalIndex = inputSignal.indexOf(signal);
            if (data != inputDatas.at(signalIndex)) {
                result = false;
                break;
            }
        }
    }

    qDebug() << "isValidConfigCheck :" << result;

    return result;
}
