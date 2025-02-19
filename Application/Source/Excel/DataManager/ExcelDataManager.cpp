#include "ExcelDataManager.h"

#include "CommonEnum.h"
#include "ConfigSetting.h"
#include "ExcelUtil.h"
#include "ExcelData.h"

#define USE_EXCEL_DATA_MANAGER_OLD

const QString SFC_IGN_ELAPSED = QString("SFC.Private.IGNElapsed.Elapsed");

QSharedPointer<ExcelDataManager>& ExcelDataManager::instance() {
    static QSharedPointer<ExcelDataManager> gManager;
    if (gManager.isNull()) {
        gManager = QSharedPointer<ExcelDataManager>(new ExcelDataManager());
    }
    return gManager;
}

ExcelDataManager::ExcelDataManager() {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});

    setMergeStart(mergeStart);
    setMerge(merge);
    setMergeEnd(mergeEnd);
    setMergeInfos(QStringList({mergeStart, merge, mergeEnd}));
}

QList<QStringList> ExcelDataManager::isSheetDataInfo(const int& sheetIndex) {
    QMap<int, QStringList> excelSheetData = isConvertedExcelData(sheetIndex);
    int rowMax = 0;
    for (auto iter = excelSheetData.cbegin(); iter != excelSheetData.cend(); ++iter) {
        rowMax = (rowMax > iter.value().size()) ? (rowMax) : (iter.value().size());
    }

    QList<QStringList> sheetData;
    sheetData.reserve(rowMax);
    for (int index = 0; index < rowMax; ++index) {
        QStringList rowData;
        for (const auto& columnData : excelSheetData) {
            if (index < columnData.size()) {
                rowData.append(columnData.at(index));
            }
        }
        sheetData.append(rowData);
    }
    qDebug() << "isSheetDataInfo :" << sheetIndex << excelSheetData.size() << rowMax << sheetData.size();
    return sheetData;
}

QMap<int, QStringList> ExcelDataManager::isConvertedExcelData(const int& sheetIndex) {
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
        QString tcName = data.getTCName();
        QString check = data.getCheck();
        QString genType = data.getGenType();
        QString vehicleType = data.getVehicleType();
        QString config = data.getConfig();
        QString resultName = data.getResultName();
        QString caseName = data.getCaseName();
        QPairStringList inputList = data.getInputList();
        QPair<int, int> rowIndex;

        if (previousTCName != tcName) {
            rowIndex = isRowIndexInfo(sheetIndex, tcName, QString(), QString(), false);
            mergeIndex[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check)].append(rowIndex);
            mergeIndex[static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)].append(rowIndex);
            mergeIndex[static_cast<int>(ivis::common::ExcelSheetTitle::Other::GenType)].append(rowIndex);
            mergeIndex[static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)].append(rowIndex);
            mergeIndex[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config)].append(rowIndex);
            previousResult.clear();
        }
        if (previousResult != resultName) {
            rowIndex = isRowIndexInfo(sheetIndex, tcName, resultName, QString(), false);
            mergeIndex[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)].append(rowIndex);
            previousCase.clear();

            QList<QStringList> outputList = data.getOutputList();
            if (outputList.size() > 0) {
                outputListInfo[qMakePair(tcName, resultName)] = outputList;
            }
        }
        if (previousCase != caseName) {
            rowIndex = isRowIndexInfo(sheetIndex, tcName, resultName, caseName, false);
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
        QPair<int, int> rowIndex = isRowIndexInfo(sheetIndex, iter.key().first, iter.key().second, QString(), false);
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

QStringList ExcelDataManager::isExcelDataOther(const int& sheetIndex, const int& columnIndex) {
    QStringList sheetData;
    if (getReadStateNewData()) {
        QMap<int, QStringList> newSheetData;
        for (const auto& data : readNewSheetData()) {
            QString tcName = data.getTCName();
            QString check = data.getCheck();
            QString genType = data.getGenType();
            QString vehicleTYpe = data.getVehicleType();
            QString config = data.getConfig();
            QString resultName = data.getResultName();
            QString caseName = data.getCaseName();
            QPairStringList inputList = data.getInputList();
            QStringList signalList = inputList.first;
            QStringList dataList = inputList.second;

            for (int index = 0; index < signalList.size(); ++index) {
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check)].append(check);
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)].append(tcName);
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::GenType)].append(genType);
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)].append(vehicleTYpe);
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config)].append(config);
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)].append(resultName);
                newSheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)].append(caseName);

                QString inputSignal = signalList.at(index);
                QString inputData = (index < dataList.size()) ? (dataList.at(index)) : (QString());
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

QStringList ExcelDataManager::isExcelDataConfig(const int& sheetIndex, const int& columnIndex) {
    QStringList sheetData = getExcelDataConfig(columnIndex);
    return sheetData;
}

QStringList ExcelDataManager::isExcelSheetData(const int& sheetIndex, const int& columnIndex) {
    QMapIntString excelSheetData = getExcelSheetData(sheetIndex);
    QStringList columnData = excelSheetData[columnIndex];

    return columnData;
}

QStringList ExcelDataManager::isInsertSheetData(const int& sheetIndex, const int& columnIndex) {
    QMap<int, QStringList> sheetData;
    for (const auto& data : readNewSheetData()) {
        QString tcName = data.getTCName();
        QString check = data.getCheck();
        QString genType = data.getGenType();
        QString vehicleTYpe = data.getVehicleType();
        QString config = data.getConfig();
        QString resultName = data.getResultName();
        QString caseName = data.getCaseName();
        QPairStringList inputList = data.getInputList();
        QStringList signalList = inputList.first;
        QStringList dataList = inputList.second;

        for (int index = 0; index < signalList.size(); ++index) {
            sheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check)].append(check);
            sheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)].append(tcName);
            sheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::GenType)].append(genType);
            sheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)].append(vehicleTYpe);
            sheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config)].append(config);
            sheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)].append(resultName);
            sheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)].append(caseName);

            QString inputSignal = signalList.at(index);
            QString inputData = (index < dataList.size()) ? (dataList.at(index)) : (QString());
            sheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal)].append(inputSignal);
            sheetData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)].append(inputData);
        }
    }
    QStringList columnData = sheetData[columnIndex];

    return columnData;
}

QPair<int, int> ExcelDataManager::isIndexOf(const QStringList& dataList, const QString& data) {
    QPair<int, int> foundIndex(1, 0);
    int startIndex = dataList.indexOf(data);
    int endIndex = dataList.lastIndexOf(data);

    if ((data.size() > 0) && (startIndex >= 0) && (endIndex >= 0)) {
        foundIndex = qMakePair(startIndex, endIndex);
    }
    // qDebug() << "isIndexOf :" << foundIndex << data;
    return foundIndex;
}

QStringList ExcelDataManager::isParsingDataList(const QStringList& data, const bool& removeWhitespace) {
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

QPair<int, int> ExcelDataManager::isRowIndexInfo(const int& sheetIndex, const QString& tcName, const QString& resultName,
                                                 const QString& caseName, const bool& origin) {
    const QStringList tcNameData = isExcelDataOther(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
    const QStringList resultData = isExcelDataOther(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
    const QStringList caseData = isExcelDataOther(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case));

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
    // qDebug() << "\t isRowIndexInfo :" << sheetIndex << tcName << resultName << caseName << origin;
    // qDebug() << "\t\t RowInfo :" << rowInfo.first << rowInfo.second << ", Type :" << logInfo;
    // qDebug() << "\t <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";

    return rowInfo;
}

QStringList ExcelDataManager::isTCNameDataList(const int& sheetIndex, const bool& all) {
    const QStringList currentData = isExcelDataOther(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName));
    const bool cliTCCheck = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeCLIModeTCCheck).toBool();
    const bool appendAll = ((all) || (cliTCCheck));
    // const bool appendAll = all;

    QStringList tcNameList;
    for (const auto& tcName : isParsingDataList(currentData, true)) {
        if (appendAll) {
            tcNameList.append(tcName);
        } else {
            if (isCheckData(sheetIndex, tcName)) {
                tcNameList.append(tcName);
            }
        }
    }

    // qDebug() << "isTCNameDataList :" << all << appendAll;
    // qDebug() << "\t Info :" << tcNameList.size() << tcNameList;
    // qDebug() << "\n";

    return tcNameList;
}

bool ExcelDataManager::isCheckData(const int& sheetIndex, const QString& tcName) {
    const QStringList currentData = isExcelDataOther(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check));
    const QPair<int, int> rowInfo = isRowIndexInfo(sheetIndex, tcName, QString(), QString(), true);

    QStringList list;
    for (int rowIndex = rowInfo.first; rowIndex <= rowInfo.second; ++rowIndex) {
        QString text = currentData.at(rowIndex);
        list.append(text);
        // qDebug() << "\t Result[" << rowIndex << "] :" << text;
    }
    QStringList checkList = isParsingDataList(list, true);
    bool check = ((checkList.size() > 0) && (checkList.at(0).compare("") != 0));

    // qDebug() << "isCheckData :" << sheetIndex << tcName;
    // qDebug() << "\t Info :" << check << checkList.size() << checkList;
    // qDebug() << "\n";

    return check;
}

int ExcelDataManager::isGenTypeData(const int& sheetIndex, const QString& tcName, QString& genTypeStr) {
    const int cliGenType = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeCLIModeGenType).toInt();

    QStringList genTypeNames("");    // 0: Invalid, 1: Default, 2: Negative/Positive, 3: Positive
    genTypeNames.append(ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeGenTypeList).toStringList());
    int genType = ivis::common::GenTypeEnum::GenTypeInvalid;

    if (cliGenType == ivis::common::GenTypeEnum::GenTypeInvalid) {
        const QStringList currentData =
            isExcelDataOther(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::GenType));
        const QPair<int, int> rowInfo = isRowIndexInfo(sheetIndex, tcName, QString(), QString(), true);

        QStringList list;
        for (int rowIndex = rowInfo.first; rowIndex <= rowInfo.second; ++rowIndex) {
            QString text = currentData.at(rowIndex);
            list.append(text);
            // qDebug() << "\t Result[" << rowIndex << "] :" << text;
        }
        QStringList genTypeList = isParsingDataList(list, true);
        genTypeStr = (genTypeList.size() > 0) ? (genTypeList.at(0)) : (QString());

        genType = genTypeNames.indexOf(genTypeStr);

        if (genType == ivis::common::GenTypeEnum::GenTypeInvalid) {
            if (genTypeStr.size() == 0) {
                genType = ivis::common::GenTypeEnum::GenTypeDefault;
                genTypeStr = (genType < genTypeNames.size()) ? (genTypeNames.at(genType)) : ("Default");
            } else {
                qDebug() << "Fail to gen type(invaild) :" << genTypeStr;
            }
        }
    } else {
        genTypeStr = (cliGenType < genTypeNames.size()) ? (genTypeNames.at(cliGenType)) : ("");
        genType = cliGenType;
    }

    // qDebug() << "isGenTypeData :" << sheetIndex << tcName;
    // qDebug() << "\t Info :" << genType << genTypeStr;
    // qDebug() << "\n";

    return genType;
}

QString ExcelDataManager::isVehicleTypeData(const int& sheetIndex, const QString& tcName) {
    const QStringList currentData =
        isExcelDataOther(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType));
    const QPair<int, int> rowInfo = isRowIndexInfo(sheetIndex, tcName, QString(), QString(), true);

    QStringList list;
    for (int rowIndex = rowInfo.first; rowIndex <= rowInfo.second; ++rowIndex) {
        QString text = currentData.at(rowIndex);
        list.append(text);
        // qDebug() << "\t Result[" << rowIndex << "] :" << text;
    }
    QStringList vehicleTypeList = isParsingDataList(list, true);
    QString vehicleType = (vehicleTypeList.size() > 0) ? (vehicleTypeList.at(0)) : (QString());

    // qDebug() << "isVehicleTypeData :" << sheetIndex << tcName;
    // qDebug() << "\t Info :" << vehicleType.size() << vehicleType;
    // qDebug() << "\n";

    return vehicleType;
}

QString ExcelDataManager::isConfigData(const int& sheetIndex, const QString& tcName) {
    const QStringList currentData = isExcelDataOther(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config));
    const QPair<int, int> rowInfo = isRowIndexInfo(sheetIndex, tcName, QString(), QString(), true);

    QStringList list;
    for (int rowIndex = rowInfo.first; rowIndex <= rowInfo.second; ++rowIndex) {
        QString text = currentData.at(rowIndex);
        list.append(text);
        // qDebug() << "\t Result[" << rowIndex << "] :" << text;
    }
    QStringList configList = isParsingDataList(list, true);
    QString config = (configList.size() > 0) ? (configList.at(0)) : (QString());

    // qDebug() << "isConfigData :" << sheetIndex << tcName;
    // qDebug() << "\t Info :" << config.size() << config;
    // qDebug() << "\n";

    return config;
}

QStringList ExcelDataManager::isResultDataList(const int& sheetIndex, const QString& tcName) {
    const QStringList currentData = isExcelDataOther(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result));
    const QPair<int, int> rowInfo = isRowIndexInfo(sheetIndex, tcName, QString(), QString(), true);

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

QStringList ExcelDataManager::isCaseDataList(const int& sheetIndex, const QString& tcName, const QString& resultName) {
    const QStringList currentData = isExcelDataOther(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case));
    const QPair<int, int> rowInfo = isRowIndexInfo(sheetIndex, tcName, resultName, QString(), true);

    QStringList list;
    for (int rowIndex = rowInfo.first; rowIndex <= rowInfo.second; ++rowIndex) {
        QString text = currentData.at(rowIndex);
        list.append(text);
        // qDebug() << "\t Case[" << rowIndex << "] :" << text;
    }

    QStringList caseList = isParsingDataList(list, true);

    // qDebug() << "isCaseDataList :" << tcName << resultName;
    // // qDebug() << "\t Info :" << caseList.size() << caseList;
    // qDebug() << "\t Size :" << caseList.size();
    // for (const auto& info : caseList) {
    //     qDebug() << "\t Case :" << info;
    // }
    // qDebug() << "\n";

    return caseList;
}

QPair<QStringList, QStringList> ExcelDataManager::isInputDataList(const int& sheetIndex, const QString& tcName,
                                                                  const QString& resultName, const QString& caseName,
                                                                  const bool& removeWhitespace, const bool& checkOthers) {
    const QStringList inputSignal =
        isExcelDataOther(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal));
    const QStringList inputData = isExcelDataOther(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData));
    const QPair<int, int> rowInfo = isRowIndexInfo(sheetIndex, tcName, resultName, caseName, true);

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

    if (checkOthers) {
        QString others =
            ExcelUtil::instance()->isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Other));
        bool tcNameBaseState = (tcName.size() > 0) && (resultName.size() == 0) && (caseName.size() == 0);
        if (tcNameBaseState) {
            QStringList caseList = isCaseDataList(sheetIndex, tcName, QString());
            if (caseList.contains(others) == false) {
                // qDebug() << "Found others case in case list : 0";
                inputList = QPair<QStringList, QStringList>();
            }
        } else {
            if ((caseName.compare(others) == 0) && (inputList.first.size() == 1) && (inputList.first.at(0).size() == 0)) {
                // case(others) 인 경우 inputList 가 있는 경우 일반적인 others 키워드로 동작 하지 않음
                // qDebug() << "case(others) and if there is an inputList, it does not work as the general others keyword.";
                inputList = QPair<QStringList, QStringList>();
            }
        }
    }

    // qDebug() << "isInputDataList :" << sheetIndex << tcName << resultName << caseName << removeWhitespace;
    // qDebug() << "\t Signal :" << inputList.first.size() << inputList.first;
    // qDebug() << "\t Data   :" << inputList.second.size() << inputList.second;
    // qDebug() << "\n";

    return inputList;
}

QPair<QStringList, QStringList> ExcelDataManager::isInputDataWithoutCaseList(const int& sheetIndex, const QString& tcName,
                                                                             const QString& resultName, const QString& caseName) {
    const QPair<QStringList, QStringList> allInputList = isInputDataList(sheetIndex, tcName, QString(), QString(), true, false);
    const QStringList caseInputSignalList = isInputDataList(sheetIndex, tcName, resultName, caseName, true).first;

    bool allIgnSignalContains = false;
    bool caseIgnSignalContains = false;

    QMap<QString, QStringList> allInputInfo;
    for (int index = 0; index < allInputList.first.size(); ++index) {
        QString signal = allInputList.first.at(index);
        if (signal.size() == 0) {
            continue;
        }
        if (ivis::common::isContainsString(signal, SFC_IGN_ELAPSED)) {
            allIgnSignalContains = true;
        }
        QString data = allInputList.second.at(index);
        QStringList dataInfo = data.remove(" ").split(",");
        allInputInfo[signal].append(dataInfo);
    }
    for (const auto& signal : caseInputSignalList) {
        if (ivis::common::isContainsString(signal, SFC_IGN_ELAPSED)) {
            caseIgnSignalContains = true;
            break;
        }
    }

    QPair<QStringList, QStringList> inputList;
    for (auto iter = allInputInfo.cbegin(); iter != allInputInfo.cend(); ++iter) {
        QString signal = iter.key();
        QStringList dataInfo = iter.value();
        dataInfo.removeAll("");
        dataInfo.removeDuplicates();

        for (const auto& removeSignal : caseInputSignalList) {
            if (removeSignal.compare(signal) == 0) {
                signal.clear();
                break;
            }
        }
        if (signal.size() > 0) {
            inputList.first.append(signal);
            inputList.second.append(dataInfo.join(", "));
        }
    }

    if ((allIgnSignalContains) && (caseIgnSignalContains == false)) {
        QString dontCare =
            ExcelUtil::instance().data()->isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::DontCare));
        inputList.first.append(SFC_IGN_ELAPSED);
        inputList.second.append(dontCare);
    }

    // qDebug() << "isInputDataWithoutCaseList :" << tcName << resultName << caseName;
    // qDebug() << "\t Signal  :" << inputList.first.size() << inputList.first;
    // qDebug() << "\t Data    :" << inputList.second.size() << inputList.second;
    // qDebug() << "\n";

    return inputList;
}

QList<QStringList> ExcelDataManager::isOutputDataList(const int& sheetIndex, const QString& tcName, const QString& resultName) {
    const bool readState = getReadStateNewData();
    setReadStateNewData(false);
    const QStringList outputSignal =
        isExcelDataOther(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal));
    const QStringList outputInit =
        isExcelDataOther(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::IsInitialize));
    const QStringList outputData =
        isExcelDataOther(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue));
    const QPair<int, int> rowInfo = isRowIndexInfo(sheetIndex, tcName, resultName, QString(), false);
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

    // qDebug() << "isOutputDataList :" << sheetIndex << tcName << resultName;
    // qDebug() << "\t Info :" << outputList.size() << outputList;
    // qDebug() << "\n";

    return outputList;
}

QList<QStringList> ExcelDataManager::isConfigDataList(const QString& configName, const bool& allData) {
    const int sheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs;

#if defined(USE_EXCEL_DATA_MANAGER_OLD)
    const QStringList configNameList =
        isExcelDataConfig(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Config::ConfigName));
    const QStringList andGroupList =
        isExcelDataConfig(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Config::AndGroup));
    const QStringList inputSignalList =
        isExcelDataConfig(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputSignal));
    const QStringList inputDataList =
        isExcelDataConfig(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputData));
#else
    const QStringList configNameList =
        isExcelSheetData(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Config::ConfigName));
    const QStringList andGroupList =
        isExcelSheetData(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Config::AndGroup));
    const QStringList inputSignalList =
        isExcelSheetData(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputSignal));
    const QStringList inputDataList =
        isExcelSheetData(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputData));
#endif

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

int ExcelDataManager::isCaseIndex(const QString& tcName, const QString& resultName, const QString& caseName) {
    int caseIndex = 0;
    for (const auto& data : readNewSheetData()) {
        QString currentTCName = data.getTCName();
        QString currentResult = data.getResultName();
        QString currentCase = data.getCaseName();

        ivis::common::getRemoved(currentTCName, getMergeInfos());
        ivis::common::getRemoved(currentResult, getMergeInfos());
        ivis::common::getRemoved(currentCase, getMergeInfos());

        if ((currentTCName.compare(tcName) == 0) && (currentResult.compare(resultName) == 0) &&
            (currentCase.compare(caseName) == 0)) {
            break;
        }
        caseIndex++;
    }
    return caseIndex;
}

void ExcelDataManager::resetExcelData(const bool& convertState) {
    int startIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    int endIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax;

    if (convertState) {
        startIndex = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription;
        endIndex = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax;
    }
    clearExcelSheetData();

    for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
        auto sheetData = ExcelData::instance().data()->getSheetData(sheetIndex).toList();
        updateParsingExcelData(sheetIndex, sheetData);
    }
}

void ExcelDataManager::updateParsingExcelData(const int& sheetIndex, const QVariantList& sheetData) {
    int columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max);
    switch (sheetIndex) {
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription:
        case ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription: {
            columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::Description::Max);
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs:
        case ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConfigs: {
            columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::Config::Max);
            break;
        }
        default: {
            break;
        }
    }

    QMapIntString excelSheetData;
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

    setExcelSheetData(sheetIndex, excelSheetData);
}

void ExcelDataManager::updateExcelData(const int& sheetIndex, const QVariantList& sheetData) {
    qDebug() << "updateExcelData :" << sheetIndex << sheetData.size();

    if ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) ||
        (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConfigs)) {
        updateExcelDataConfig(sheetData);
    } else if ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) ||
               (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription)) {
        // do nothing
    } else {
        updateExcelDataOther(sheetIndex, sheetData);
    }
}

void ExcelDataManager::updateExcelDataOther(const int& sheetIndex, const QVariantList& sheetData) {
    const int columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max);

#if defined(USE_EXCEL_DATA_MANAGER_OLD)
    QMap<int, QStringList> excelSheetData;
#else
    QMapIntString excelSheetData;
#endif

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
#if defined(USE_EXCEL_DATA_MANAGER_OLD)
    setReadStateNewData(false);
    clearNewSheetData();
    clearExcelDataOther();
#else
    setExcelSheetData(sheetIndex, excelSheetData);
#endif

    if (excelSheetData.size() == 0) {
        qDebug() << "Fail to other sheet data size : 0";
        return;
    }

    writeExcelDataOther(excelSheetData);
}

void ExcelDataManager::updateExcelDataConfig(const QVariantList& sheetData) {
    const int columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::Config::Max);

    QMap<int, QStringList> excelSheetData;
    for (const auto& rowDataList : sheetData.toList()) {
        QStringList rowData = rowDataList.toStringList();
        if (rowData.size() != columnMax) {
            continue;
        }

        for (int columnIndex = 0; columnIndex < columnMax; ++columnIndex) {
            QString readColumnText = rowData.at(columnIndex);
            // ivis::common::getRemoved(readColumnText, isMergeInfos());
            excelSheetData[columnIndex].append(readColumnText);
        }
    }

    clearExcelDataConfig();

    if (excelSheetData.size() == 0) {
        qDebug() << "Fail to config sheet data size : 0";
        return;
    }

    writeExcelDataConfig(excelSheetData);
}

void ExcelDataManager::updateInputDataInfo(const int& sheetIndex, const QString& tcName, const QString& resultName,
                                           const QString& caseName, const QPair<QStringList, QStringList>& inputList,
                                           const QString& baseCaseName, const bool& insertBefore) {
    if ((tcName.size() == 0) || (resultName.size() == 0) || (caseName.size() == 0)) {
        qDebug() << "Fail to update info size :" << sheetIndex << tcName.size() << resultName.size() << caseName.size();
        return;
    }

    QString check = (isCheckData(sheetIndex, tcName)) ? (QString("O")) : (QString());
    QString genTypeStr;
    int genType = isGenTypeData(sheetIndex, tcName, genTypeStr);
    QString vehicleType = isVehicleTypeData(sheetIndex, tcName);
    QString config = isConfigData(sheetIndex, tcName);

    QStringList inputSignalList;
    QStringList inputDataList;
    for (const auto& signal : inputList.first) {
        // if (signal.size() == 0) {    // Output 없는 경우에 Others Case 만 있는 경우 시그널, 데이터가 null 이여서 문제 생김
        //     continue;
        // }
        int index = inputList.first.indexOf(signal);
        QString data = (index < inputList.second.size()) ? (inputList.second.at(index)) : (QString());
        inputSignalList.append(signal);
        inputDataList.append(data);
    }

    QList<QStringList> outputList = isOutputDataList(sheetIndex, tcName, resultName);
    int listSizeGap = outputList.size() - inputSignalList.size();
    listSizeGap = (listSizeGap > 0) ? (listSizeGap) : (0);
    for (int index = 0; index < listSizeGap; ++index) {
        inputSignalList.append("");
        inputDataList.append("");
    }

    QPair<QStringList, QStringList> currInputList = qMakePair(inputSignalList, inputDataList);
    int caseIndex = isSizeNewSheetData();
    if (baseCaseName.size() > 0) {
        caseIndex = isCaseIndex(tcName, resultName, baseCaseName);
        caseIndex = (insertBefore) ? (caseIndex) : (caseIndex + 1);
    }

    if (caseIndex >= 0) {
        InsertData insertData(tcName, check, genTypeStr, vehicleType, config, resultName, caseName, currInputList, outputList);
        setNewSheetData(caseIndex, insertData);
    }

#if 0
    qDebug() << "[updateInputDataInfo] :" << inputSignalList.size() << inputDataList.size();
    qDebug() << "\t TCName      :" << tcName;
    qDebug() << "\t Result      :" << resultName;
    qDebug() << "\t Case        :" << caseIndex << caseName;
    qDebug() << "\t InputSignal :" << inputSignalList.size() << inputSignalList;
    qDebug() << "\t InputData   :" << inputDataList.size() << inputDataList;
    qDebug() << "\t OutputList  :" << outputList.size() << outputList;
    qDebug() << "\n";
#endif
}

void ExcelDataManager::updateOutputDataInfo(const QString& tcName, const QString& resultName, const QList<QStringList>& list) {
    if ((tcName.size() == 0) || (resultName.size() == 0)) {
        qDebug() << "Fail to update info size :" << tcName.size() << resultName.size();
        return;
    }

    #if 0
    qDebug() << "[updateOutputDataInfo] :" << inputSignalList.size() << inputDataList.size();
    qDebug() << "\t TCName      :" << tcName;
    qDebug() << "\t Result      :" << resultName;
    qDebug() << "\t OutputSignal :" << inputSignalList.size() << inputSignalList;
    qDebug() << "\t OutputData   :" << inputDataList.size() << inputDataList;
    qDebug() << "\n";
#endif
}

bool ExcelDataManager::isValidConfigCheck(const bool& other, const QString& configName, const QMap<QString, QString>& inputList) {
    QList<QStringList> configList = isConfigDataList(configName, other);
    QMap<int, QPair<QStringList, QStringList>> inputMap;
    QStringList signalList;
    QStringList dataList;

    // other = false
    // configList : 시그널 리스트에서 시그널, 데이터 별로 구성
    // inputList : 시그널과 데이터가 있는지 확인 후 전체가 시그널에 대한 결과를 만족해야 true

    // other = true
    // configList : 시그널 리스트에서 andGroup 의 묶음으로 시그널, 데이터 구성
    // inputList : configList 와 동일하게 시그널 구성 되어 있으며 andGroup 별로 시그널, 데이터 하나만 만족해도 결과 true

    for (const auto& config : configList) {
        if (config.size() == 2) {
            inputMap[0].first.append(config.at(0));
            inputMap[0].second.append(config.at(1));
        } else if (config.size() == 4) {
            QString andGroup = config.at(1);
            signalList.append(config.at(2));
            dataList.append(config.at(3));

            if ((andGroup.compare(getMergeStart()) != 0) && (andGroup.compare(getMerge()) != 0)) {
                inputMap[inputMap.size()] = qMakePair(signalList, dataList);
                signalList.clear();
                dataList.clear();
            }
        } else {
        }
    }

    QMap<QString, bool> resultMap;
    for (const auto& input : inputMap) {
        signalList = input.first;
        dataList = input.second;

        for (auto iter = inputList.cbegin(); iter != inputList.cend(); ++iter) {
            QString signal = iter.key();
            QString data = iter.value();
            bool found = signalList.contains(signal);
            if (found) {
                int signalIndex = signalList.indexOf(signal);
                if (signalIndex >= 0) {
                    QString signalData = dataList.at(signalIndex);
                    QStringList signalDataList = signalData.remove(" ").split(",");
                    found = (signalDataList.contains(data));
                }
            }
            resultMap[signal] = (resultMap[signal]) ? (true) : (found);
        }
    }
    bool result = false;
    for (int index = 0; index < inputMap.size(); ++index) {
        QStringList checkSignalList = (other) ? (inputMap[index].first) : (inputList.keys());
        for (const auto& signal : checkSignalList) {
            result = resultMap[signal];
            if (result == false) {
                break;
            }
        }
        if (result) {
            break;
        }
    }

    // qDebug() << "isValidConfigCheck :" << result;
    // qDebug() << "\t inputMap  :" << inputMap;
    // qDebug() << "\t inputList :" << inputList;
    // for (const auto& resultKey : resultMap.keys()) {
    //     qDebug() << "\t ResultMap :" << resultKey << resultMap[resultKey];
    // }
    // qDebug() << "\n";

    return result;
}
