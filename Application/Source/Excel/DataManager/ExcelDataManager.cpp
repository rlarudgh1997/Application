#include "ExcelDataManager.h"

#include "CommonEnum.h"
#include "ConfigSetting.h"
#include "ExcelUtil.h"
#include "ExcelData.h"
#include "SignalDataManager.h"

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

    reloadExcelData();
}

QVariantList ExcelDataManager::isSheetDataInfo(const int& sheetIndex, const bool& appendMergeInfo) {
    QMap<int, QStringList> excelSheetData = isConvertedExcelData(sheetIndex, appendMergeInfo);
    int rowMax = 0;
    for (auto iter = excelSheetData.cbegin(); iter != excelSheetData.cend(); ++iter) {
        rowMax = (rowMax > iter.value().size()) ? (rowMax) : (iter.value().size());
    }

    QList<QStringList> tmpSheetData;
    tmpSheetData.reserve(rowMax);
    for (int index = 0; index < rowMax; ++index) {
        QStringList rowData;
        for (const auto& columnData : excelSheetData) {
            if (index < columnData.size()) {
                rowData.append(columnData.at(index));
            }
        }
        tmpSheetData.append(rowData);
        // qDebug() << index << ". RowData :" << rowData;
    }

    QVariantList sheetData;
    for (auto& data : tmpSheetData) {
        sheetData.append(data);
    }

    // qDebug() << "isSheetDataInfo :" << sheetIndex << excelSheetData.size() << rowMax << tmpSheetData.size();
    return sheetData;
}

QMap<int, QStringList> ExcelDataManager::isConvertedExcelData(const int& sheetIndex, const bool& appendMergeInfo) {
    // Read Type
    setReadStateNewData(true);
    // InsertData to ExcelData
    QMap<int, QStringList> excelSheetData;
    QMap<QPair<QString, QString>, QList<QStringList>> outputListInfo;
    QMap<int, QList<QPair<int, int>>> mergeIndex;
    QString previousTCName;
    QString previousResult;
    QString previousCase;

    const auto currSheetData = getInsertSheetData(sheetIndex);
    for (const auto& data : currSheetData) {
        QString tcName = data.getTCName();
        QString check = data.getCheck();
        QString genType = data.getGenType();
        QString vehicleType = data.getVehicleType();
        QString config = data.getConfig();
        QString resultName = data.getResultName();
        QString caseName = data.getCaseName();
        QPairStrList inputList = data.getInputList();
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
        }
    }

    if (appendMergeInfo) {
        for (auto iter = mergeIndex.cbegin(); iter != mergeIndex.cend(); ++iter) {
            int columnIndex = iter.key();
            QStringList& columnData = excelSheetData[columnIndex];
            for (const auto& merge : iter.value()) {
                int start = merge.first;
                int end = merge.second;
                int count = end - start + 1;

                if (count == 1) {
                    continue;
                }
                for (int index = start; index <= end; ++index) {
                    if (index >= columnData.size()) {
                        continue;
                    }
                    // for (const auto& merge : getMergeInfos()) {
                    //     columnData[index].remove(merge);
                    // }
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

QStringList ExcelDataManager::isOriginSheetData(const int& sheetIndex, const int& columnIndex) {
    // ivis::common::CheckTimer checkTimer;
    // static int totalElapsed = 0;

    QMapIntStrList excelSheetData = getExcelSheetData(sheetIndex);
    QStringList columnData = excelSheetData[columnIndex];

    // totalElapsed += checkTimer.check("\t isOriginSheetData");
    // qDebug() << "\t\t Total - isOriginSheetData :" << totalElapsed << "\n";

    return columnData;
}

QStringList ExcelDataManager::isInsertSheetData(const int& sheetIndex, const int& columnIndex) {
    const auto currSheetData = getInsertSheetData(sheetIndex);

    QMap<int, QStringList> sheetData;
    for (const auto& data : currSheetData) {
        QString tcName = data.getTCName();
        QString check = data.getCheck();
        QString genType = data.getGenType();
        QString vehicleTYpe = data.getVehicleType();
        QString config = data.getConfig();
        QString resultName = data.getResultName();
        QString caseName = data.getCaseName();
        QPairStrList inputList = data.getInputList();
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

QStringList ExcelDataManager::isExcelSheetData(const int& sheetIndex, const int& columnIndex, const bool& origin,
                                               const bool& log) {
    // ivis::common::CheckTimer checkTimer;
    // static int totalElapsed = 0;

    QStringList dataList;

    if (origin) {
        dataList = isOriginSheetData(sheetIndex, columnIndex);
    } else {
        dataList = isInsertSheetData(sheetIndex, columnIndex);
    }

    if (log) {
        qDebug() << "isExcelSheetData :" << sheetIndex << columnIndex << origin;
        for (const auto& data : dataList) {
            qDebug() << "\t Data :" << data;
        }
    }

    // totalElapsed += checkTimer.check(QString("\t isExcelSheetData : %1").arg(origin));
    // qDebug() << "\t\t Total - isExcelSheetData :" << totalElapsed << "\n";

    return dataList;
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
                                                 const QString& caseName, const bool& origin, const bool& log) {
    const int columnIndex = ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetManualTC)
                                 ? (static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::TCName))
                                 : (static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)));
    const QStringList tcNameData = isExcelSheetData(sheetIndex, columnIndex, origin);
    const QStringList resultData =
        isExcelSheetData(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result), origin);
    const QStringList caseData =
        isExcelSheetData(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case), origin);

    if (log) {
        qDebug() << "isRowIndexInfo :" << sheetIndex << tcName << resultName << caseName << origin;
        qDebug() << "\t TCName :" << tcNameData;
        qDebug() << "\t Result :" << resultData;
        qDebug() << "\t Case   :" << caseData;
    }

    QPair<int, int> rowInfo(1, 0);
    QPair<int, int> foundIndex(1, 0);
    QString logInfo("Default");

    // TCName Index
    if (tcName.size() > 0) {
        foundIndex = isIndexOf(tcNameData, tcName);
        if (foundIndex == qMakePair(1, 0)) {
            // qDebug() << "Not found tcName :" << tcName;
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
            // qDebug() << "Not found result :" << resultName;
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
            // qDebug() << "Not found case :" << caseName;
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
    const int columnIndex = ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetManualTC)
                                 ? (static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::TCName))
                                 : (static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)));
    const auto currentData = isExcelSheetData(sheetIndex, columnIndex, true);
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
    const QStringList currentData =
        isExcelSheetData(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check), true);
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

    QStringList genTypeNames("");  // 0: Invalid, 1: Default, 2: Negative/Positive, 3: Positive
    genTypeNames.append(ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeGenTypeList).toStringList());
    int genType = ivis::common::GenTypeEnum::GenTypeInvalid;

    if (cliGenType == ivis::common::GenTypeEnum::GenTypeInvalid) {
        const QStringList currentData =
            isExcelSheetData(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::GenType), true);
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
        isExcelSheetData(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType), true);
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
    const QStringList currentData =
        isExcelSheetData(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config), true);
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
    // ivis::common::CheckTimer checkTimer;
    // static int totalElapsed = 0;

    const QStringList currentData =
        isExcelSheetData(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result), true);
    const QPair<int, int> rowInfo = isRowIndexInfo(sheetIndex, tcName, QString(), QString(), true);

    QStringList list;
    for (int rowIndex = rowInfo.first; rowIndex <= rowInfo.second; ++rowIndex) {
        QString text = currentData.at(rowIndex);
        list.append(text);
        // qDebug() << "\t Result[" << rowIndex << "] :" << text;
    }
    QStringList resultList = isParsingDataList(list, true);

    // totalElapsed += checkTimer.check("isResultDataList");
    // qDebug() << "\t Total - isResultDataList :" << totalElapsed << "\n";

    // qDebug() << "isResultDataList :" << tcName;
    // qDebug() << "\t Info :" << resultList.size() << resultList;
    // qDebug() << "\n";

    return resultList;
}

QStringList ExcelDataManager::isCaseDataList(const int& sheetIndex, const QString& tcName, const QString& resultName) {
    // ivis::common::CheckTimer checkTimer;
    // static int totalElapsed = 0;

    const QStringList currentData =
        isExcelSheetData(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case), true);
    const QPair<int, int> rowInfo = isRowIndexInfo(sheetIndex, tcName, resultName, QString(), true);

    QStringList list;
    for (int rowIndex = rowInfo.first; rowIndex <= rowInfo.second; ++rowIndex) {
        QString text = currentData.at(rowIndex);
        list.append(text);
        // qDebug() << "\t Case[" << rowIndex << "] :" << text;
    }

    QStringList caseList = isParsingDataList(list, true);

    // totalElapsed += checkTimer.check("isCaseDataList");
    // qDebug() << "\t Total - isCaseDataList :" << totalElapsed << "\n";

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
    // ivis::common::CheckTimer checkTimer;
    // static int totalElapsed = 0;

    const QStringList inputSignal =
        isExcelSheetData(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal), true);
    const QStringList inputData =
        isExcelSheetData(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData), true);
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

    // totalElapsed += checkTimer.check("isInputDataList");
    // qDebug() << "\t Total - isInputDataList :" << totalElapsed << "\n";

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
#if 1  // Parsing 오류
       // removeDuplicates() 함수 동작시 리스트 중복(4294967296) 삭제
       // In  : QList("[CustomRange][4294967296", "4294967296", "4294967296", "4294967296]", "[1", "2", "99", "100]")
       // Out : QList("[CustomRange][4294967296", "4294967296", "4294967296]", "[1", "2", "99", "100]")
        allInputInfo[signal].append(data);
#else
        QStringList dataInfo = data.remove(" ").split(",");
        allInputInfo[signal].append(dataInfo);
#endif
    }
    for (const auto& signal : caseInputSignalList) {
        if (ivis::common::isContainsString(signal, SFC_IGN_ELAPSED)) {
            caseIgnSignalContains = true;
            break;
        }
    }

    QPair<QStringList, QStringList> inputList;
    for (const auto& key : allInputInfo.keys()) {
        QString signal = key;
#if 1  // Parsing 오류
        QStringList dataList = allInputInfo[key];
        dataList.removeAll("");
        dataList.removeDuplicates();

        QStringList dataInfo;
        for (auto d : dataList) {
            dataInfo.append(d.remove(" ").split(","));
        }
#else
        QStringList dataInfo = allInputInfo[key];
        dataInfo.removeAll("");
        dataInfo.removeDuplicates();
#endif

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

    // qDebug() << "==========================================================================================================";
    // qDebug() << "isInputDataWithoutCaseList";
    // qDebug() << "\t TcName :" << tcName;
    // qDebug() << "\t Result :" << resultName;
    // qDebug() << "\t Case   :" << caseName;
    // qDebug() << "\t Signal :" << inputList.first.size() << inputList.first;
    // qDebug() << "\t Data   :" << inputList.second.size() << inputList.second;
    // qDebug() << "\n\n";

    return inputList;
}

QList<QStringList> ExcelDataManager::isOutputDataList(const int& sheetIndex, const QString& tcName, const QString& resultName) {
    // ivis::common::CheckTimer checkTimer;
    // static int totalElapsed = 0;

    const QStringList outputSignal =
        isExcelSheetData(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal), true);
    const QStringList outputInit =
        isExcelSheetData(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::IsInitialize), true);
    const QStringList outputData =
        isExcelSheetData(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue), true);
    const QPair<int, int> rowInfo = isRowIndexInfo(sheetIndex, tcName, resultName, QString(), true);

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

    // totalElapsed += checkTimer.check("isOutputDataList");
    // qDebug() << "\t Total - isOutputDataList :" << totalElapsed << "\n";

    // qDebug() << "isOutputDataList :" << sheetIndex << tcName << resultName;
    // qDebug() << "\t Row  :" << rowInfo;
    // qDebug() << "\t Info :" << outputList.size() << outputList;
    // qDebug() << "\n";

    return outputList;
}

QList<QStringList> ExcelDataManager::isConfigDataList(const QString& configName, const bool& allData, const bool& removeMerge) {
    const int sheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs;
    const QStringList configNameList =
        isExcelSheetData(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Config::ConfigName), true);
    const QStringList andGroupList =
        isExcelSheetData(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Config::AndGroup), true);
    const QStringList inputSignalList =
        isExcelSheetData(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputSignal), true);
    const QStringList inputDataList =
        isExcelSheetData(sheetIndex, static_cast<int>(ivis::common::ExcelSheetTitle::Config::InputData), true);

    QStringList tempConfigList;
    for (auto configInfo : configNameList) {
        ivis::common::getRemoved(configInfo, getMergeInfos());
        tempConfigList.append(configInfo);
    }

    QList<QStringList> dataInfo;
    QPair<int, int> foundIndex = isIndexOf(tempConfigList, (configName.size() == 0) ? (QString("Default")) : (configName));

    if (foundIndex == qMakePair(1, 0)) {
        // qDebug() << "Not found configName :" << configName;
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
        if (removeMerge) {
            for (auto& d : data) {
                ivis::common::getRemoved(d, getMergeInfos());
            }
        }
        dataInfo.append(data);
    }

    // qDebug() << "isConfigDataList :" << configName << allData << removeMerge << foundIndex;
    // for (const auto& info : dataInfo) {
    //     qDebug() << "\t Info :" << info;
    // }
    // qDebug() << "\n";

    return dataInfo;
}

QPair<QStringList, QStringList> ExcelDataManager::isDependentDataList(const QString& tcName, const QString& resultName) {
    const int startIndex = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription;
    const int endIndex = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax;

    QPair<int, int> tcNameIndex;
    QPair<int, int> resultIndex;
    int foundSheetIndex = 0;
    int signalColumnIndex = 0;
    int dataColumnIndex = 0;

    for (int sheetIndex = startIndex; sheetIndex < endIndex; ++sheetIndex) {
        int tcNameColumnIndex = 0;
        int resultColumnIndex = 0;
        if ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription) ||
            (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConfigs) ||
            (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetManualTC)) {
            continue;
        } else if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDependentOn) {
            tcNameColumnIndex = static_cast<int>(ivis::common::ExcelSheetTitle::DependentOn::TCName);
            resultColumnIndex = static_cast<int>(ivis::common::ExcelSheetTitle::DependentOn::Result);
            signalColumnIndex = static_cast<int>(ivis::common::ExcelSheetTitle::DependentOn::InputSignal);
            dataColumnIndex = static_cast<int>(ivis::common::ExcelSheetTitle::DependentOn::InputData);
        } else {
            tcNameColumnIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName);
            resultColumnIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result);
            signalColumnIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal);
            dataColumnIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData);
        }

        QStringList tcNameList = isExcelSheetData(sheetIndex, tcNameColumnIndex, true);
        QStringList resultList = isExcelSheetData(sheetIndex, resultColumnIndex, true);
        if ((tcNameList.size() == 0) || (resultList.size() == 0)) {
            continue;
        }

        tcNameIndex = isIndexOf(tcNameList, tcName);
        resultIndex = isIndexOf(resultList, resultName);

        if ((resultIndex.first >= tcNameIndex.first) && (resultIndex.second <= tcNameIndex.second) &&
            (resultIndex.first <= resultIndex.second)) {
            foundSheetIndex = sheetIndex;
            break;
        }
    }

    if (foundSheetIndex == 0) {
        qDebug() << "Fail to found tcName :" << tcName << ", result :" << resultName << "\n";
        return QPair<QStringList, QStringList>();
    }

    const QStringList inputSignalList = isExcelSheetData(foundSheetIndex, signalColumnIndex, true);
    const QStringList inputDataList = isExcelSheetData(foundSheetIndex, dataColumnIndex, true);

    QStringList signalList;
    QStringList dataList;
    for (int index = resultIndex.first; index <= resultIndex.second; ++index) {
        auto signal = (index < inputSignalList.size()) ? (inputSignalList.at(index)) : (QString());
        auto data = (index < inputDataList.size()) ? (inputDataList.at(index)) : (QString());
        signalList.append(signal);
        dataList.append(data);
    }
    QPair<QStringList, QStringList> dataInfo = qMakePair(signalList, dataList);

#if 0
    qDebug() << "==========================================================================================================";
    qDebug() << "isDependentDataList :"  << foundSheetIndex << tcName << resultName;
    qDebug() << "\t Index :" << tcNameIndex << resultIndex;
    for (int index = 0; index < dataInfo.first.size(); ++index) {
        auto signal = dataInfo.first.at(index);
        auto data = (index < dataInfo.second.size()) ? (dataInfo.second.at(index)) : (QString());
        qDebug() << "\t Signal :" << signal;
        qDebug() << "\t Data   :" << data << "\n";
    }
    qDebug() << "\n";
#endif

    return dataInfo;
}

QPair<QStringList, QStringList> ExcelDataManager::isConvertedToEnum(const QStringList& signalList, const QStringList& dataList) {
    QStringList dataHexList;

    for (int index = 0; index < signalList.size(); ++index) {
        if (index >= dataList.size()) {
            continue;
        }
        auto signal = signalList.at(index);
        auto data = dataList.at(index);
        auto enumList = SignalDataManager::instance().data()->isSignalValueEnum(true, signal);
        auto hexList = SignalDataManager::instance().data()->isSignalValueEnum(false, signal);

        QStringList tempHexList;
        for (const auto& d : data.remove(" ").split(",")) {
            int validIndex = enumList.indexOf(d);
            if (validIndex >= hexList.size()) {
                continue;
            }
            QString value = ((validIndex >= 0) && (validIndex < hexList.size())) ? (hexList.at(validIndex)) : (d);
            // qDebug() << "\t Info :" << signal << validIndex << hexList.size() << d << "->" << value;
            tempHexList.append(value);
        }
        dataHexList.append(tempHexList.join(", "));
    }

#if 0
    qDebug() << "isConvertedToEnum";
    qDebug() << "\t Signal :" << signalList.size() << signalList;
    qDebug() << "\t Data   :" << dataList.size() << dataList;
    qDebug() << "\t Hex    :" << dataHexList.size() << dataHexList << "\n";
#endif

    return qMakePair(signalList, dataHexList);
}

QList<ManualData> ExcelDataManager::isManualDataList() {
    const auto sheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetManualTC;
    reloadExcelData(sheetIndex);

    QMap<QString, QPair<int, int>> tcNameRowInfo;
    QMap<int, QStringList> readManualData;

    for (const auto& tcName : isTCNameDataList(sheetIndex, true)) {
        tcNameRowInfo[tcName] = isRowIndexInfo(sheetIndex, tcName, QString(), QString(), true);
        for (int columnIndex = 0; columnIndex < static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::Max); ++columnIndex) {
            readManualData[columnIndex] = isExcelSheetData(sheetIndex, columnIndex, true);
        }
    }

    QList<ManualData> manualDataList;

    for (const auto& key : tcNameRowInfo.keys()) {
        int start = tcNameRowInfo[key].first;
        int end = (tcNameRowInfo[key].second + 1);
        // qDebug() << "\t RowInfo[" << key << "] :" << start << "~" << end;

        ManualData manualData;
        for (const auto& columnIndex : readManualData.keys()) {
            QStringList value = readManualData[columnIndex].mid(start, end);
            for (auto& v : value) {
                v.remove(getMergeStart());
                v.remove(getMerge());
                v.remove(getMergeEnd());
            }
            value.removeAll("");
            value.removeDuplicates();

            QString toString = (value.size() > 0) ? (value.at(0)) : (QString());
            bool toBool = (value.size() > 0) ? (value.at(0).size() > 0) : (false);
            bool ok = false;
            int toInt = (value.size() > 0) ? (value.at(0).toInt(&ok)) : (0);
            toInt = (ok) ? (toInt) : (0);

            // qDebug() << "\t\t value :" << value.size() << value << "->" << toString << toBool << toInt << "\n";

            switch (columnIndex) {
                case static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::TCName): {
                    manualData.setTCName(toString);
                    break;
                }
                case static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::VehicleType): {
                    manualData.setVehicleType(toString);
                    break;
                }
                case static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::RunnableOpt): {
                    manualData.setRunnableOpt(toBool);
                    break;
                }
                case static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::ConfigOpt): {
                    manualData.setConfigOpt(toBool);
                    break;
                }
                case static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::CycleOption): {
                    manualData.setCycleOption(toBool);
                    break;
                }
                case static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::CyclePeriod): {
                    manualData.setCyclePeriod(toInt);
                    break;
                }
                case static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::CycleDelta): {
                    manualData.setCycleDelta(toInt);
                    break;
                }
                case static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::CycleDuration): {
                    manualData.setCycleDuration(toInt);
                    break;
                }
                case static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::CycleMode): {
                    manualData.setCycleMode(toString);
                    break;
                }
                case static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::PreconditionSignal): {
                    manualData.setPreconditionList(qMakePair(value, QStringList()));
                    break;
                }
                case static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::PreconditionValue): {
                    manualData.setPreconditionList(isConvertedToEnum(manualData.getPreconditionList().first, value));
                    break;
                }
                case static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::InputSignal): {
                    manualData.setInputList(qMakePair(value, QStringList()));
                    break;
                }
                case static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::InputValue): {
                    manualData.setInputList(isConvertedToEnum(manualData.getInputList().first, value));
                    break;
                }
                case static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::InitSignal): {
                    manualData.setInitList(qMakePair(value, QStringList()));
                    break;
                }
                case static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::InitValue): {
                    manualData.setInitList(isConvertedToEnum(manualData.getInitList().first, value));
                    break;
                }
                case static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::OutputSignal): {
                    manualData.setOutputList(qMakePair(value, QStringList()));
                    break;
                }
                case static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::OutputValue): {
                    manualData.setOutputList(isConvertedToEnum(manualData.getOutputList().first, value));
                    break;
                }
                default: {
                    break;
                }
            }
        }
        manualDataList.append(manualData);
    }

#if 0
    qDebug() << "isManualDataList :" << manualDataList.size();
    for (const auto& manualData : manualDataList) {
        qDebug() << "\t TCName        :" << manualData.getTCName();
        qDebug() << "\t VehicleType   :" << manualData.getVehicleType();
        qDebug() << "\t RunnableOpt   :" << manualData.getRunnableOpt();
        qDebug() << "\t ConfigOpt     :" << manualData.getConfigOpt();
        qDebug() << "\t CycleOption   :" << manualData.getCycleOption();
        qDebug() << "\t CyclePeriod   :" << manualData.getCyclePeriod();
        qDebug() << "\t CycleDelta    :" << manualData.getCycleDelta();
        qDebug() << "\t CycleDuration :" << manualData.getCycleDuration();
        qDebug() << "\t CycleMode     :" << manualData.getCycleMode();
        for (int index = 0; index < manualData.getPreconditionList().first.size(); ++index) {
            qDebug() << "\t Precondition  :" << index << manualData.getPreconditionList().first.at(index)
                                             << "->" << manualData.getPreconditionList().second.at(index);
        }
        for (int index = 0; index < manualData.getInputList().first.size(); ++index) {
            qDebug() << "\t Input         :" << index << manualData.getInputList().first.at(index)
                                             << "->" << manualData.getInputList().second.at(index);
        }
        for (int index = 0; index < manualData.getInitList().first.size(); ++index) {
            qDebug() << "\t Init          :" << index << manualData.getInitList().first.at(index)
                                             << "->" << manualData.getInitList().second.at(index);
        }
        for (int index = 0; index < manualData.getOutputList().first.size(); ++index) {
            qDebug() << "\t Output        :" << index << manualData.getOutputList().first.at(index)
                                             << "->" << manualData.getOutputList().second.at(index);
        }
        qDebug() << "\n";
    }
#endif
    return manualDataList;
}

int ExcelDataManager::isCaseIndex(const int& sheetIndex, const QString& tcName, const QString& resultName,
                                  const QString& caseName) {
    const auto currSheetData = getInsertSheetData(sheetIndex);

    int caseIndex = 0;
    for (const auto& data : currSheetData) {
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

void ExcelDataManager::reloadExcelData(const int& sheetIndex) {
    const int startIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;  // Origin
    const int endIndex = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax;           // Convert
    const bool allState = ((sheetIndex < startIndex) || (sheetIndex >= endIndex));

    QList<int> sheetIndexList;
    if (allState) {
        clearInsertSheetData();
        clearExcelSheetData();
        for (int index = startIndex; index < endIndex; ++index) {
            if (index == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax) {
                continue;
            }
            sheetIndexList.append(index);
        }
    } else {
        setInsertSheetData(sheetIndex, QList<InsertData>());
        setExcelSheetData(sheetIndex, QMap<int, QStringList>());
        sheetIndexList.append(sheetIndex);
    }

    QMap<int, QMap<int, QStringList>> excelData;
    for (const auto& index : sheetIndexList) {
        auto sheetData = ExcelData::instance().data()->getSheetData(index).toList();
        auto excelSheetData = updateParsingExcelData(index, sheetData);
        setExcelSheetData(index, excelSheetData);

        if (index <= ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax) {
            continue;
        }
        excelData[index] = excelSheetData;
    }

#if 0
    qDebug() << (QString(100, '*').toLatin1().data());
    qDebug() << "reloadExcelData :" << sheetIndex << allState;
    qDebug() << "\t" << (QString(100, '=').toLatin1().data()) << "\n";

    for (const auto& sheetKey : excelData.keys()) {
        auto sheetData = excelData[sheetKey];
        if (sheetData.size() == 0) {
            continue;
        }

        int rowMax = 0;
        for (auto iter = sheetData.cbegin(); iter != sheetData.cend(); ++iter) {
            rowMax = (rowMax > iter.value().size()) ? (rowMax) : (iter.value().size());
        }

        qDebug() << "\t" << (QString(100, '>').toLatin1().data());
        qDebug() << "\t [[SheetIndex]] :" << sheetKey << rowMax << "\n";

        for (int index = 0; index < rowMax; ++index) {
            QStringList rowData;
            for (const auto& columnData : sheetData) {
                if (index < columnData.size()) {
                    rowData.append(columnData.at(index));
                }
            }
            qDebug() << "\t Data[" << index << "] :" << rowData;
            qDebug() << "\t" << (QString(100, '<').toLatin1().data());
        }
        qDebug() << "\n";
    }
#endif
}

void ExcelDataManager::writeExcelSheetData(const int& sheetIndex) {
    const int startIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;  // Origin
    const int endIndex = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetMax;           // Convert
    const bool allState = ((sheetIndex < startIndex) || (sheetIndex >= endIndex));

    QList<int> sheetIndexList;

    if (allState) {
        for (int index = startIndex; index < endIndex; ++index) {
            if (index == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax) {
                continue;
            }
            sheetIndexList.append(index);
        }
    } else {
        sheetIndexList.append(sheetIndex);
    }

    for (const auto& index : sheetIndexList) {
        auto sheetData = isSheetDataInfo(index);
        ExcelData::instance().data()->setSheetData(index, sheetData);
    }
}

QMapIntStrList ExcelDataManager::updateParsingExcelData(const int& sheetIndex, const QVariantList& sheetData) {
    int columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max);
    bool removeMergeState = true;
    switch (sheetIndex) {
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription:
        case ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription: {
            columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::Description::Max);
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs:
        case ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConfigs: {
            columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::Config::Max);
            removeMergeState = false;
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDependentOn:
        case ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDependentOn: {
            columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::DependentOn::Max);
            // removeMergeState = false;
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetManualTC:
        case ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetManualTC: {
            columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::ManualTC::Max);
            // removeMergeState = false;
            break;
        }
        default: {
            break;
        }
    }

    QMapIntStrList excelSheetData;
    for (const auto& rowDataList : sheetData.toList()) {
        QStringList rowData = rowDataList.toStringList();
        if (rowData.size() != columnMax) {
            continue;
        }

        for (int columnIndex = 0; columnIndex < columnMax; ++columnIndex) {
            QString readColumnText = rowData.at(columnIndex);
            // qDebug() << sheetIndex << ". Data :" << readColumnText;
            if (removeMergeState) {
                ivis::common::getRemoved(readColumnText, getMergeInfos());
            }
            // qDebug() << "\t " << readColumnText;
            excelSheetData[columnIndex].append(readColumnText);
        }
    }

    // qDebug() << "updateParsingExcelData :" << sheetIndex << sheetData.size() << excelSheetData.size();

    return excelSheetData;
}

void ExcelDataManager::updateInputDataInfo(const int& sheetIndex, const QString& tcName, const QString& resultName,
                                           const QString& caseName, const QPair<QStringList, QStringList>& inputList,
                                           const QString& baseCaseName, const bool& insertBefore) {
    if ((tcName.size() == 0) || (resultName.size() == 0) || (caseName.size() == 0)) {
        qDebug() << "Fail to update info size :" << sheetIndex << tcName.size() << resultName.size() << caseName.size();
        return;
    }

    QString check = (isCheckData(sheetIndex, tcName)) ? (QString("O")) : (QString());
    QString genType;
    int genTypeIndex = isGenTypeData(sheetIndex, tcName, genType);
    QString vehicleType = isVehicleTypeData(sheetIndex, tcName);
    QString config = isConfigData(sheetIndex, tcName);
    QList<QStringList> outputList = isOutputDataList(sheetIndex, tcName, resultName);
    QPair<QStringList, QStringList> currInputList = reconstructInputData(inputList, outputList);
    QList<InsertData> currSheetData = getInsertSheetData(sheetIndex);
    int caseIndex = currSheetData.size();

    if (baseCaseName.size() > 0) {
        caseIndex = isCaseIndex(sheetIndex, tcName, resultName, baseCaseName);
        caseIndex = (insertBefore) ? (caseIndex) : (caseIndex + 1);
    }

    if (caseIndex >= 0) {
        InsertData insertData(sheetIndex, caseIndex, tcName, check, genType, vehicleType, config, resultName, caseName,
                              currInputList, outputList);
        currSheetData.insert(caseIndex, insertData);
        setInsertSheetData(sheetIndex, currSheetData);
    }

#if 0
    qDebug() << "[updateInputDataInfo] :" << sheetIndex;
    qDebug() << "\t TCName      :" << tcName;
    qDebug() << "\t Result      :" << resultName;
    qDebug() << "\t Case        :" << caseIndex << caseName;
    qDebug() << "\t InputSignal  :" << currInputList.first.size() << currInputList.first;
    qDebug() << "\t InputData    :" << currInputList.second.size() << currInputList.second;
    if (outputList.size() == 3) {
        qDebug() << "\t OutputSignal :" << outputList.at(0).size() << outputList.at(0);
        qDebug() << "\t OutputInit   :" << outputList.at(1).size() << outputList.at(1);
        qDebug() << "\t OutputData   :" << outputList.at(2).size() << outputList.at(2);
    }
    qDebug() << "\n";
#endif
}

void ExcelDataManager::updateOutputDataInfo(const int& sheetIndex, const QString& tcName, const QString& resultName,
                                            const QList<QStringList>& outputList) {
    if ((tcName.size() == 0) || (resultName.size() == 0)) {
        qDebug() << "Fail to update info size :" << tcName.size() << resultName.size();
        return;
    }

    QList<InsertData> currSheetData;
    for (auto& currData : getInsertSheetData(sheetIndex)) {
        if ((tcName == currData.getTCName()) && (resultName == currData.getResultName())) {
            QPairStrList inputList = reconstructInputData(currData.getInputList(), outputList);
            currData.setInputList(inputList);
            currData.setOutputList(outputList);
        }
        currSheetData.append(currData);
    }
    setInsertSheetData(sheetIndex, currSheetData);

#if 0
    qDebug() << "[updateOutputDataInfo] :" << sheetIndex;
    qDebug() << "\t TCName      :" << tcName;
    qDebug() << "\t Result      :" << resultName;
    qDebug() << "\t InputSignal  :" << inputList.first.size() << inputList.first;
    qDebug() << "\t InputData    :" << inputList.second.size() << inputList.second;
    if (outputList.size() == 3) {
        qDebug() << "\t OutputSignal :" << outputList.at(0).size() << outputList.at(0);
        qDebug() << "\t OutputInit   :" << outputList.at(1).size() << outputList.at(1);
        qDebug() << "\t OutputData   :" << outputList.at(2).size() << outputList.at(2);
    }
    qDebug() << "\n";
#endif
}

QPair<QStringList, QStringList> ExcelDataManager::reconstructInputData(const QPair<QStringList, QStringList>& inputList,
                                                                       const QList<QStringList>& outputList) {
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

    int resizeMax = outputList.size() - inputSignalList.size();
    resizeMax = (resizeMax > 0) ? (resizeMax) : (0);
    for (int index = 0; index < resizeMax; ++index) {
        inputSignalList.append("");
        inputDataList.append("");
    }

    return qMakePair(inputSignalList, inputDataList);
}

bool ExcelDataManager::isValidConfigCheck(const QString& configName, const QMap<QString, QString>& inputList) {
    bool allData = true;  // true : Config 컬럼 전체 데이터, false : InputSingnal, InputData 컬럼 데이터만 구성
    QList<QStringList> configList = isConfigDataList(configName, allData, false);
    QMap<int, QPair<QStringList, QStringList>> inputMap;
    QStringList signalList;
    QStringList dataList;

    // allData = false
    // configList : 시그널 리스트에서 시그널, 데이터 별로 구성
    // inputList : 시그널과 데이터가 있는지 확인 후 전체가 시그널에 대한 결과를 만족해야 true

    // allData = true
    // configList : 시그널 리스트에서 andGroup 의 묶음으로 시그널, 데이터 구성
    // inputList : configList 와 동일하게 시그널 구성 되어 있으며 andGroup 별로 시그널, 데이터 하나만 만족해도 결과 true

    for (const auto& config : configList) {
#if 0
        // allData = false 조건을 같이 사용하려면 해당 코드 활성화화
        if (config.size() == 2) {  // allData = false
            inputMap[0].first.append(config.at(0));
            inputMap[0].second.append(config.at(1));
        } else if (config.size() == 4) {  // allData = true
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
#else
        if (config.size() != 4) {
            continue;
        }
        QString andGroup = config.at(1);
        signalList.append(config.at(2));
        dataList.append(config.at(3));

        if ((andGroup.compare(getMergeStart()) != 0) && (andGroup.compare(getMerge()) != 0)) {
            inputMap[inputMap.size()] = qMakePair(signalList, dataList);
            signalList.clear();
            dataList.clear();
        }
#endif
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
        QStringList checkSignalList = (allData) ? (inputMap[index].first) : (inputList.keys());
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
