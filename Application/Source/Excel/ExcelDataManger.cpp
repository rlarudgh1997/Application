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
}

QPair<QStringList, QStringList> ExcelDataManger::isCaseInputList(const QString& tcName, const QString& resultName,
                                                                 const QString& caseName) {
    QPair<QStringList, QStringList> caseInputData;
    int caseIndex = isCaseIndex(caseName, true);

    // QMap<int, QPair<QString, QPair<QStringList, QStringList>>> mCaseRelationship;
    QString currentCaseName = mCaseRelationship[caseIndex].first;

    if (currentCaseName.compare(caseName) == false) {
        caseInputData = mCaseRelationship[caseIndex].second;
    }

    qDebug() << "isCaseInputList :" << caseIndex << tcName << resultName << caseName;
    for (int index = 0; index < caseInputData.first.size(); ++index) {
        qDebug() << "\t Input :" << caseInputData.first[index] << caseInputData.second[index];
    }

    clear(true);
    return caseInputData;
}

QList<QStringList> ExcelDataManger::isSheetDataInfo() {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});

    QList<QStringList> sheetData;
    QMap<int, QStringList> convertExcelData;
    for (const auto& data : mExcelData) {
        QPair<QStringList, QStringList> inputList = data.isInputList();
        for (int index = 0; index < inputList.first.size(); ++index) {
            QString inputSignal = inputList.first[index];
            QString inputData = inputList.second[index];
            convertExcelData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)].append(data.isTCName());
            convertExcelData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)].append(data.isResultName());
            convertExcelData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)].append(data.isCaseName());
            convertExcelData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal)].append(inputSignal);
            convertExcelData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)].append(inputData);
        }
    }

    QStringList tcName;
    QStringList resultName;
    QStringList caseName;
    QStringList inputSignal;
    QStringList inptuData;
    int rowMax = 0;

    for (auto iter = convertExcelData.cbegin(); iter != convertExcelData.cend(); ++iter) {
        int columnIndex = iter.key();
        QStringList tempList = iter.value();
        QStringList newList;

        if (columnIndex <= static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)) {
            for (int index = 0; index < tempList.size(); ++index) {
                QString text = tempList[index];
                int startIndex = tempList.indexOf(text);
                int endIndex = tempList.lastIndexOf(text);

                if (startIndex == endIndex) {
                    text = text;
                } else if (index == startIndex) {
                    text.prepend(mergeStart);
                } else if (index == endIndex) {
                    text.prepend(mergeEnd);
                } else {
                    text.prepend(merge);
                }
                newList.append(text);
            }
        } else {
            newList = tempList;
        }

        if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)) {
            tcName = newList;
        } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)) {
            resultName = newList;
        } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)) {
            caseName = newList;
        } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal)) {
            inputSignal = newList;
        } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)) {
            inptuData = newList;
        }
        rowMax = (rowMax > newList.size()) ? (rowMax) : (newList.size());

        // qDebug() << "------------------------------------------------------------------------------------------------------";
        // qDebug() << columnIndex << ". Origin :" << rowMax << tempList.size() << tempList;
        // qDebug() << columnIndex << ". New    :" << rowMax << newList.size() << newList;
        // qDebug() << "\n";
    }

    for (int index = 0; index < rowMax; ++index) {
#if 1
        QStringList rowData;
        if (index < tcName.size()) {
            rowData.append(tcName[index]);
        }
        if (index < resultName.size()) {
            rowData.append(resultName[index]);
        }
        if (index < caseName.size()) {
            rowData.append(caseName[index]);
        }
        if (index < inputSignal.size()) {
            rowData.append(inputSignal[index]);
        }
        if (index < inptuData.size()) {
            rowData.append(inptuData[index]);
        }
#else
        QStringList rowData(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max));

        if (index < tcName.size()) {
            rowData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)] = tcName[index];
        }
        if (index < resultName.size()) {
            rowData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)] = resultName[index];
        }
        if (index < caseName.size()) {
            rowData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)] = caseName[index];
        }
        if (index < inputSignal.size()) {
            rowData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal)] = inputSignal[index];
        }
        if (index < inptuData.size()) {
            rowData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)] = inptuData[index];
        }
#endif
        sheetData.append(rowData);
    }

    for (const auto& rowData : sheetData) {
        qDebug() << "RowData :" << rowData.size() <<rowData;
    }

    return sheetData;
}

int ExcelDataManger::isTCNameIndex(const QString& tcName, const bool& relationship) {
    QMap<int, QPair<QString, QStringList>> tcNameData = (relationship) ? (mTCNameRelationship) : (mTCNameData);
    int tcNameIndex = tcNameData.size();

    for (auto iter = tcNameData.cbegin(); iter != tcNameData.cend(); ++iter) {
        QString currentTCName = iter.value().first;
        // qDebug() << "\t\t TCName :" << tcName.size() << currentTCName << "->" << tcName;
        if (tcName.compare(currentTCName) == false) {
            tcNameIndex = iter.key();
            break;
        }
    }
    // qDebug() << "isTCNameIndex :" << tcNameIndex << tcName;

    return tcNameIndex;
}

int ExcelDataManger::isResultIndex(const QString& tcName, const QString& resultName, const bool& relationship) {
    int tcNameIndex = isTCNameIndex(tcName, relationship);
    QMap<int, QPair<QString, QStringList>> tcNameData = (relationship) ? (mTCNameRelationship) : (mTCNameData);
    QStringList resultList = tcNameData[tcNameIndex].second;

    QMap<int, QPair<QString, QStringList>> resultData = (relationship) ? (mResultRelationship) : (mResultData);
    int resultIndex = resultData.size();

    for (auto iter = resultData.cbegin(); iter != resultData.cend(); ++iter) {
        QString currentResultName = iter.value().first;
        if (resultName.compare(currentResultName) == false) {
            resultIndex = iter.key();
            break;
        }
    }

    // int index = resultList.indexOf(resultName);
    // index =  (index < 0) ? (resultData.size()) : (index);
    // QString temp = resultData[index].first;
    // if (resultName.compare(temp) == false) {
    //     resultIndex = index;
    // }
    // qDebug() << "resultList :" << resultList;
    // qDebug() << "isResultIndex :" << index << resultIndex << resultName << temp;

    // qDebug() << "isResultIndex :" << resultIndex << resultName;

    return resultIndex;
}

int ExcelDataManger::isCaseIndex(const QString& caseName, const bool& relationship) {
    QMap<int, QPair<QString, QPair<QStringList, QStringList>>> caseData = (relationship) ? (mCaseRelationship) : (mCaseData);
    int caseIndex = caseData.size();

    for (auto iter = caseData.cbegin(); iter != caseData.cend(); ++iter) {
        QString currentCaseName = iter.value().first;
        if (caseName.compare(currentCaseName) == false) {
            caseIndex = iter.key();
            break;
        }
    }
    // qDebug() << "isCaseIndex :" << caseIndex << caseName;

    return caseIndex;
}

void ExcelDataManger::updateExcelData(const QVariantList& sheetData) {
    const int columnMax = static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData) + 1;
    QMap<int, QStringList> excelData;

    for (const auto& rowDataList : sheetData.toList()) {
        QStringList rowData = rowDataList.toStringList();
        // columnMax = rowData.size();
        for (int columnIndex = 0; columnIndex < columnMax; ++columnIndex) {
            QString readColumnText = rowData.at(columnIndex);
            excelData[columnIndex].append(readColumnText);
        }
    }

    int rowMax = sheetData.size();
    updateRelatonshipTCName(rowMax, excelData);
    updateRelationshipResult(rowMax, excelData);
    updateRelationshipCase(rowMax, excelData);

    setSheetData(sheetData);
}


void ExcelDataManger::updateCaseDataInfo(const QString& tcName, const QString& resultName, const QString& caseName,
                                         const QPair<QStringList, QStringList>& inputList) {
    qDebug() << "[updateCaseDataInfo]";
    qDebug() << "\t Info  :" << tcName << resultName << caseName << inputList.first.size();

#if 1
    mExcelData.append(InsertData(tcName, resultName, caseName, inputList));
#else
    int tcNameIndex = isTCNameIndex(tcName, false);
    int resultIndex = isResultIndex(tcName, resultName, false);
    int caseIndex = isCaseIndex(caseName, false);

    qDebug() << "\t Index :" << tcNameIndex << resultIndex << caseIndex;

    QStringList resultList = mTCNameData[tcNameIndex].second;
    resultList.append(resultName);
    resultList.removeDuplicates();
    if (resultList.size() != mTCNameData[tcNameIndex].second.size()) {
        mTCNameData[tcNameIndex] = QPair<QString, QStringList>(tcName, resultList);
    }

    QStringList caseList = mResultData[resultIndex].second;
    caseList.append(caseName);
    // caseList.removeDuplicates();
    if (caseList.size() != mResultData[resultIndex].second.size()) {
        mResultData[resultIndex] = QPair<QString, QStringList>(resultName, caseList);
    }

    QPair<QStringList, QStringList> inputInfoList = mCaseData[caseIndex].second;
    inputInfoList.first.append(inputList.first);
    // inputInfoList.first.removeDuplicates();
    inputInfoList.second.append(inputList.second);
    // inputInfoList.second.removeDuplicates();
    if (inputInfoList.first.size() != mCaseData[caseIndex].second.first.size()) {
        mCaseData[caseIndex] = QPair<QString, QPair<QStringList, QStringList>>(caseName, inputList);
    }
    printData();
#endif
}

void ExcelDataManger::updateRelatonshipTCName(const int& rowMax, const QMap<int, QStringList>& excelData) {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});

    // QMap<TCNameIndex, QPair<TCName, ResultNameList>>
    QMap<int, QPair<QString, QStringList>> tcNameRelationship;
    // QMap<TCNameIndex, QPair<VehicleType, Config>>
    QMap<int, QPair<QString, QString>> configRelationship;
    int tcNameSize = excelData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)].size();
    int vehicleTypeSize = excelData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)].size();
    int configSize = excelData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config)].size();
    if ((rowMax == tcNameSize) && (rowMax == vehicleTypeSize) && (rowMax == configSize)) {
        QString foundTCName;
        QPair<QString, QString> fountConfig;
        QStringList resultNameList;
        // int count = 0;

        for (int index = 0; index < rowMax; ++index) {
            QString resultName = excelData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)].at(index);
            bool appendResult = ((ivis::common::isContainsString(resultName, merge) == false) &&
                                 (ivis::common::isContainsString(resultName, mergeEnd) == false));
            if (appendResult) {
                ivis::common::getRemoved(resultName, mergeInfos);
                resultNameList.append(resultName);
            }

            QString tcName = excelData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName)].at(index);
            bool foundState = false;
            bool appendRelationship = false;
            if (ivis::common::isContainsString(tcName, mergeStart)) {
                foundState = true;
            } else if (ivis::common::isContainsString(tcName, mergeEnd)) {
                appendRelationship = true;
            } else {
                if (ivis::common::isContainsString(tcName, merge) == false) {
                    foundState = true;
                    appendRelationship = true;
                }
            }

            if (foundState) {
                foundTCName = tcName;
                ivis::common::getRemoved(foundTCName, mergeInfos);
            }
            if (appendRelationship) {
                // int index = 0;
                // for (auto& resultInfo : resultNameList) {
                //     resultInfo.prepend(QString("%1.").arg(count + index));
                //     index++;
                // }
                // count = resultNameList.size();

                tcNameRelationship[tcNameRelationship.size()] = qMakePair(foundTCName, resultNameList);
                foundTCName.clear();
                resultNameList.clear();

                QString vehicleType =
                    excelData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)].at(index);
                QString config = excelData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config)].at(index);

                ivis::common::getRemoved(vehicleType, mergeInfos);
                ivis::common::getRemoved(config, mergeInfos);
                configRelationship[configRelationship.size()] = qMakePair(vehicleType, config);
            }
        }
    }

    mTCNameRelationship = tcNameRelationship;
    mConfigRelationship = configRelationship;
}

void ExcelDataManger::updateRelationshipResult(const int& rowMax, const QMap<int, QStringList>& excelData) {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});

    // QMap<ResultIndex, QPair<ResultName, CaseNameList>>
    QMap<int, QPair<QString, QStringList>> resultRelationship;
    int resultSize = excelData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)].size();
    int caseSize = excelData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)].size();
    if ((rowMax == resultSize) && (rowMax == caseSize)) {
        QString foundResultName;
        QStringList caseNameList;
        int count = 0;

        for (int index = 0; index < rowMax; ++index) {
            QString caseName = excelData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)].at(index);
            bool appendCase = ((ivis::common::isContainsString(caseName, merge) == false) &&
                               (ivis::common::isContainsString(caseName, mergeEnd) == false));
            if (appendCase) {
                ivis::common::getRemoved(caseName, mergeInfos);
                caseNameList.append(caseName);
            }

            QString resultName = excelData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result)].at(index);
            bool foundState = false;
            bool appendRelationship = false;
            if (ivis::common::isContainsString(resultName, mergeStart)) {
                foundState = true;
            } else if (ivis::common::isContainsString(resultName, mergeEnd)) {
                appendRelationship = true;
            } else {
                if (ivis::common::isContainsString(resultName, merge) == false) {
                    foundState = true;
                    appendRelationship = true;
                }
            }

            if (foundState) {
                foundResultName = resultName;
                ivis::common::getRemoved(foundResultName, mergeInfos);
            }
            if (appendRelationship) {
                // int index = 0;
                // for (auto& caseInfo : caseNameList) {
                //     caseInfo.prepend(QString("%1.").arg(count + index));
                //     index++;
                // }
                resultRelationship[resultRelationship.size()] = qMakePair(foundResultName, caseNameList);
                foundResultName.clear();
                caseNameList.clear();
            }
        }
    }

    mResultRelationship = resultRelationship;
}

void ExcelDataManger::updateRelationshipCase(const int& rowMax, const QMap<int, QStringList>& excelData) {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});

    // QMap<CaseIndex, QPair<CaseName, QPair<InputSignal, InputData>>>
    QMap<int, QPair<QString, QPair<QStringList, QStringList>>> caseRelationship;
    int inputSignalSize = excelData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal)].size();
    int inputDataSize = excelData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)].size();
    if ((rowMax == inputSignalSize) && (rowMax == inputDataSize)) {
        QString foundCaseName;
        QStringList inputSignalList;
        QStringList inputDataList;

        for (int index = 0; index < rowMax; ++index) {
            QString inputSignal = excelData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal)].at(index);
            QString inputData = excelData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)].at(index);
            inputSignalList.append(inputSignal);
            inputDataList.append(inputData);

            QString caseName = excelData[static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)].at(index);
            bool foundState = false;
            bool appendRelationship = false;
            if (ivis::common::isContainsString(caseName, mergeStart)) {
                foundState = true;
            } else if (ivis::common::isContainsString(caseName, mergeEnd)) {
                appendRelationship = true;
            } else {
                if (ivis::common::isContainsString(caseName, merge) == false) {
                    foundState = true;
                    appendRelationship = true;
                }
            }

            if (foundState) {
                foundCaseName = caseName;
                ivis::common::getRemoved(foundCaseName, mergeInfos);
            }
            if (appendRelationship) {
                QPair<QStringList, QStringList> inputList = qMakePair(inputSignalList, inputDataList);
                caseRelationship[caseRelationship.size()] = qMakePair(foundCaseName, inputList);
                foundCaseName.clear();
                inputSignalList.clear();
                inputDataList.clear();
            }
        }
    }

    mCaseRelationship = caseRelationship;
}

void ExcelDataManger::clear(const bool& all) {
    if (all) {
        mTCNameRelationship.clear();
        mConfigRelationship.clear();
        mResultRelationship.clear();
        mCaseRelationship.clear();
    }
    mTCNameData.clear();
    mConfigData.clear();
    mResultData.clear();
    mCaseData.clear();
}

void ExcelDataManger::printData(const bool& relationship) {
    qDebug() << "\t **************************************************************************************";

    QMap<int, QPair<QString, QStringList>> tcNameData = (relationship) ? (mTCNameRelationship) : (mTCNameData);
    for (auto iter = tcNameData.cbegin(); iter != tcNameData.cend(); ++iter) {
        qDebug() << "\t TcName[" << iter.key() << "] :" << iter.value().first;
        for (const auto& data : iter.value().second) {
            qDebug() << "\t\t Result :" << data;
        }
    }
    qDebug() << "\t --------------------------------------------------------------------------------------\n";
    // QMap<int, QPair<QString, QString>> configData = (relationship) ? (mConfigRelationship) : (mConfigData);
    // for (auto iter = configData.cbegin(); iter != configData.cend(); ++iter) {
    //     qDebug() << "\t Config[" << iter.key() << "] :" << iter.value();
    // }
    // qDebug() << "\t --------------------------------------------------------------------------------------\n";

    QMap<int, QPair<QString, QStringList>> resultData = (relationship) ? (mResultRelationship) : (mResultData);
    for (auto iter = resultData.cbegin(); iter != resultData.cend(); ++iter) {
        qDebug() << "\t Result[" << iter.key() << "] :" << iter.value().first;
        for (const auto& data : iter.value().second) {
            qDebug() << "\t\t Case :" << data;
        }
    }
    qDebug() << "\t --------------------------------------------------------------------------------------\n";

    QMap<int, QPair<QString, QPair<QStringList, QStringList>>> caseData = (relationship) ? (mCaseRelationship) : (mCaseData);
    for (auto iter = caseData.cbegin(); iter != caseData.cend(); ++iter) {
        qDebug() << "\t Case[" << iter.key() << "]   :" << iter.value().first;
        QPair<QStringList, QStringList> inputList = iter.value().second;
        for (const auto& data : inputList.first) {
            qDebug() << "\t\t Input :" << data;
        }
    }
    qDebug() << "\t --------------------------------------------------------------------------------------\n\n";
}
