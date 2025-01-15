#include "ExcelUtil.h"

#include "CommonEnum.h"
#include "ConfigSetting.h"
#include "ExcelData.h"

QSharedPointer<ExcelUtil>& ExcelUtil::instance() {
    static QSharedPointer<ExcelUtil> gUtil;
    if (gUtil.isNull()) {
        gUtil = QSharedPointer<ExcelUtil>(new ExcelUtil());
    }
    return gUtil;
}

ExcelUtil::ExcelUtil() {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});

    setMergeStart(mergeStart);
    setMerge(merge);
    setMergeEnd(mergeEnd);
    setMergeInfos(QStringList({mergeStart, merge, mergeEnd}));
}

QStringList ExcelUtil::isDescriptionDataInfo() {
    const int descriptSheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
    // const int descriptSheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription;

    QStringList dataInfo(static_cast<int>(ivis::common::ExcelSheetTitle::Description::Max));
    int rowIndex = 0;
    for (const auto& rowDataList : ExcelData::instance()->getSheetData(descriptSheetIndex).toList()) {
        QStringList rowData = rowDataList.toStringList();
        if (rowData.size() < static_cast<int>(ivis::common::ExcelSheetTitle::Description::Max)) {
            // qDebug() << "Fail to sheet data list size :" << rowData.size();
            continue;
        }

        for (int columnIndex = 0; columnIndex < dataInfo.size(); ++columnIndex) {
            dataInfo[columnIndex] = rowData[columnIndex];
        }

        rowIndex++;
    }
    return dataInfo;
}

QList<QPair<QString, int>> ExcelUtil::isKeywordPatternInfo(const int& columnIndex) {
    QList<QPair<QString, int>> keywordPattern;

    if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case)) {
        keywordPattern = {
            // qMakePair(QString("Other"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Other)),
            qMakePair(QString("others"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Other)),
        };
    } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config)) {
        keywordPattern = {
            qMakePair(QString("Config"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Config)),
        };
    } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal)) {
        keywordPattern = {
            qMakePair(QString("[Sheet]"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Sheet)),
            qMakePair(QString("[Dependent_On]"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::DependentOn)),
            qMakePair(QString("[Not_Trigger]"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::NotTrigger)),
            qMakePair(QString("[Preset]"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Preset)),
        };
    } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)) {
        keywordPattern = {
            qMakePair(QString("valuechanged"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::ValueChanged)),
            // qMakePair(QString("Value Changed"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::ValueChanged)),
            qMakePair(QString("~"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Range)),
            // qMakePair(QString("="), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Equal)),
            qMakePair(QString("<=>"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::TwoWay)),
            qMakePair(QString(">="), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::MoreThanEqual)),
            qMakePair(QString("<="), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::LessThanEqual)),
            qMakePair(QString("=>"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Flow)),
            qMakePair(QString(">"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Over)),
            qMakePair(QString("<"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Under)),
            qMakePair(QString("!"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Not)),
            // qMakePair(QString("timeout"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Timeout)),
            qMakePair(QString("MESSAGE_TIMEOUT"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Timeout)),
            // qMakePair(QString("Time out"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Timeout)),
            qMakePair(QString("CRC_ERROR"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Crc)),
            // qMakePair(QString("crc"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Crc)),
            qMakePair(QString("[Not_Trigger]"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::NotTrigger)),
            // qMakePair(QString("D’"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::DontCare)),
            // qMakePair(QString("D`"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::DontCare)),
            qMakePair(QString("D'"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::DontCare)),
        };
    } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal)) {
        keywordPattern = {
            qMakePair(QString("[Sheet]"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Sheet)),
            qMakePair(QString("collect"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Collect)),
        };
    } else if (columnIndex == static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue)) {
        keywordPattern = {
            qMakePair(QString("[Cal]"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Cal)),
        };
    } else {
        keywordPattern = {
            qMakePair(QString("[CustomNotTrigger]"),
                      static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotTrigger)),
            qMakePair(QString("[CustomOver]"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomOver)),
            qMakePair(QString("[CustomUnder]"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomUnder)),
            qMakePair(QString("[CustomRange]"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomRange)),
            qMakePair(QString("[CustomTwoWay]"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomTwoWay)),
            qMakePair(QString("[CustomIgn]"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomIgn)),
            qMakePair(QString("[CustomFlow]"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomFlow)),
            qMakePair(QString("[CustomMoreThanEqual]"),
                      static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomMoreThanEqual)),
            qMakePair(QString("[CustomLessThanEqual]"),
                      static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomLessThanEqual)),
            qMakePair(QString("[CustomConfig]"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomConfig)),
            qMakePair(QString("[CustomNotDefined]"),
                      static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotDefined)),
        };
    }
    return keywordPattern;
}

QString ExcelUtil::isKeywordString(const int keywordType) {
    QMap<int, QString> keywordPatternInfo;
    QStringList keywordString;
    for (int index = 0; index <= static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max); ++index) {
        for (const auto& infoPair : isKeywordPatternInfo(index)) {
            keywordPatternInfo[infoPair.second] = infoPair.first;
            keywordString.append(infoPair.first);
        }
    }
    return keywordPatternInfo[keywordType];
}

int ExcelUtil::isKeywordType(const int& columnIndex, QString& inputData) {
    int keywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid);
    QString tempInputData = inputData;

    QList<QPair<QString, int>> keywordPatternList = isKeywordPatternInfo(columnIndex);
    QStringList compareKeywordList = QStringList({
        isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Timeout)),
        isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Crc)),
        isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Collect)),
        isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::ValueChanged)),
        isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::DontCare)),
        isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Other)),
    });

    // [입력된 inputData 중 아래에 텍스트에 해당 하는 경우 처리 방법]
    //     [방법 1] 대소문자 구분 없이 비교 (compare(str, Qt::CaseInsensitive))
    //     [방법 2] 선언된 키워드 패턴 스트링으로 변경
    //         1. "Value Changed"  =>> "ValueChanged"
    //         2. "CRC", "crc"  =>> "Crc"
    //         3. "Time out", "TimeOut"  =>> "Timeout"
    //         4. "COLLECT", "collect", "" =>> "Collect"
    //         5. ""D’", "D`", "" =>> "D'"
    //         6. "Others", "Other" | input_Signal 없으면 => "Others"

    for (const auto& pair : keywordPatternList) {
        QString currentKeyword = pair.first;
        int currentKeywordType = pair.second;
        bool compareState = false;

        for (const auto& str : compareKeywordList) {
            if (currentKeyword.compare(str, Qt::CaseInsensitive) == 0) {
                compareState = true;
                break;
            }
        }

        if (compareState) {
            if (tempInputData.compare(currentKeyword, Qt::CaseInsensitive) == 0) {
                keywordType = currentKeywordType;
                tempInputData = isKeywordString(keywordType);
            }
        } else {
            if (tempInputData.contains(currentKeyword, Qt::CaseInsensitive) == true) {
                if ((currentKeywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Range)) ||
                    (currentKeywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::TwoWay)) ||
                    (currentKeywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Flow))) {
                    tempInputData.remove(" ");
                    tempInputData.replace(currentKeyword, ", ");
                } else {
                    // no operation
                }
                keywordType = currentKeywordType;
                tempInputData.remove(currentKeyword);
            }
        }
    }

    if (keywordType != static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid)) {
        // qDebug() << "isKeywordType :" << keywordType << ", InputData :" << inputData << "->" << tempInputData;
        inputData = tempInputData;
    } else {
        QList<QPair<QString, int>> nonKeywordPatternList = {
            qMakePair(QString("D’"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::DontCare)),
            qMakePair(QString("D`"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::DontCare)),
            qMakePair(QString("Value Changed"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::ValueChanged)),
            qMakePair(QString("Other"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Other)),
            qMakePair(QString("Time Out"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Timeout)),
            // qMakePair(QString("MESSAGE_TIMEOUT"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Timeout)),
            qMakePair(QString("timeout"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Timeout)),
            qMakePair(QString("Crc Error"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Crc)),
            qMakePair(QString("crc"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Crc)),
            // qMakePair(QString("CRC_ERROR"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Crc)),
        };
        for (const auto& pair : nonKeywordPatternList) {
            QString currentKeyword = pair.first;
            int currentKeywordType = pair.second;
            QString tmpValue = tempInputData;
            QStringList enumValueList = tmpValue.remove(" ").split(",");
            for (int i = 0; i < enumValueList.size(); ++i) {
                QString val = enumValueList.at(i);
                if (val.compare(currentKeyword, Qt::CaseInsensitive) == 0) {
                    keywordType = currentKeywordType;
                    enumValueList[i] = isKeywordString(keywordType);
                }
            }
            inputData = enumValueList.join(", ");
            if (keywordType != static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid)) {
                break;
            }
        }
    }

    // qDebug() << "isKeywordType :" << keywordType << ", InputData :" << inputData << "->" << tempInputData;
    return keywordType;
}

#if !defined(USE_CODE_BEFORE_CLASS_SPLIT)
QList<KeywordInfo> ExcelUtil::isKeywordTypeInfo(const QVariantList& sheetData, const QList<int>& inputColumnList) {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const QList<int> columnList = inputColumnList;

    QList<KeywordInfo> keywordTypeInfo;
    int rowIndex = 0;
    QList<QPair<int, int>> caseRowInfo;
    QPair<int, int> rowInfo = QPair<int, int>((-1), (-1));

    for (const auto& rowDataList : sheetData) {
        QStringList rowData = rowDataList.toStringList();
        if (rowData.size() < (static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max))) {
            // qDebug() << "Fail to sheet data list size :" << rowData.size();
            continue;
        }

        for (const auto& columnIndex : columnList) {
            QString text = rowData.at(columnIndex);
            int keywordType = isKeywordType(columnIndex, text);
            QString data = QString();

            if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid)) {
                continue;
            }

            if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Sheet)) {
                data = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData));
            } else if ((keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Range)) ||
                       (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Not))) {
                data = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal));
            } else if ((keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::DontCare)) ||
                       (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::ValueChanged)) ||
                       (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Other)) ||
                       (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Crc)) ||
                       (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Timeout))) {
                data = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal));
            } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Collect)) {
                data = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue));
            } else {
                // no operation
            }
            keywordTypeInfo.append(KeywordInfo(rowIndex, columnIndex, text, keywordType, data));
        }

        QString caseText = rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case));

        if (caseText.contains(mergeStart)) {
            rowInfo = QPair<int, int>(rowIndex, (-1));
        } else if (caseText.contains(mergeEnd)) {
            rowInfo = QPair<int, int>(rowInfo.first, rowIndex);
        } else {
        }

        if ((rowInfo.first >= 0) && (rowInfo.second >= 0)) {
            caseRowInfo.append(rowInfo);
            rowInfo = QPair<int, int>((-1), (-1));
        }

        rowIndex++;
    }

    for (auto& keyword : keywordTypeInfo) {
        // if ((keyword.isKeyword() & static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Sheet)) == false) {
        //     continue;
        // }

        QPair<int, int> rowInfo = QPair<int, int>((-1), (-1));
        for (const auto& row : caseRowInfo) {
            if ((keyword.isRow() < row.first) || (keyword.isRow() > row.second)) {
                continue;
            }
            rowInfo = row;
            break;
        }

        QList<QStringList> rowData;
        int getRowIndex = 0;
        // Keyword 에 해당하는 Row 데이터 전체 구성
        for (const auto& rowDataList : sheetData) {
            if ((getRowIndex >= rowInfo.first) && (getRowIndex <= rowInfo.second)) {
                QStringList rowDataInfo = rowDataList.toStringList();
                QString inputSignalInfo = rowDataInfo.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal));
                if (inputSignalInfo.contains(keyword.isText()) == false) {
                    QStringList dataInfo(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max));
                    for (int index = 0; index < static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal); ++index) {
                        dataInfo[index] = rowDataInfo.at(index);
                    }
                    rowData.append(dataInfo);
                }
            }
            getRowIndex++;
        }
        keyword.updateRowData(rowData);
    }

#if 0
    for (const auto& keyword : keywordTypeInfo) {
        qDebug() << "-----------------------------------------------------------------------------------------";
        qDebug() << "Keyword[" << sheetIndex << "]";
        qDebug() << "\t Info        :" << keyword.isRow() << keyword.isColumn() << keyword.isKeyword() << keyword.isText();
        qDebug() << "\t Data        :" << keyword.isData();
        qDebug() << "\t RowData     :" << keyword.isRowData();
        qDebug() << "\t ConvertData :" << keyword.isConvertData();
    }
#endif

    return keywordTypeInfo;
}
#endif

int ExcelUtil::isDataType(const QString& dataTypeStr) {
    int dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::Invalid);

    if (dataTypeStr.compare("HUInt64") == false) {
        dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::HUInt64);
    } else if (dataTypeStr.compare("HInt64") == false) {
        dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::HInt64);
    } else if (dataTypeStr.compare("HString") == false) {
        dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::HString);
    } else {
        // qDebug() << "isDataType -> DataType is incorrect :" << dataTypeStr;
    }
    return dataType;
}

QPair<int, int> ExcelUtil::isIGNElapsedType(const QString& singalName) {
    QPair<int, int> ignInfo;
    if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOn0ms") == false) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOn500ms") == false) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn500ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOn3000ms") == false) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn3000ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOn3500ms") == false) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn3500ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOn4000ms") == false) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn4000ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOn10s") == false) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn10s),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOff0ms") == false) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOff500ms") == false) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff500ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOff700ms") == false) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff700ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOff1000ms") == false) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff1000ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOff1500ms") == false) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff1500ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms));
    } else {
        ignInfo = QPair<int, int>(-1, -1);
    }
    return ignInfo;
}

QString ExcelUtil::isIGNElapsedName(const int& ignType) {
    QString signalName;
    if (ignType == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms)) {
        signalName = QString("SFC.Private.IGNElapsed.ElapsedOn0ms");
    } else if (ignType == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn500ms)) {
        signalName = QString("SFC.Private.IGNElapsed.ElapsedOn500ms");
    } else if (ignType == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn3000ms)) {
        signalName = QString("SFC.Private.IGNElapsed.ElapsedOn3000ms");
    } else if (ignType == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn3500ms)) {
        signalName = QString("SFC.Private.IGNElapsed.ElapsedOn3500ms");
    } else if (ignType == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn4000ms)) {
        signalName = QString("SFC.Private.IGNElapsed.ElapsedOn4000ms");
    } else if (ignType == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn10s)) {
        signalName = QString("SFC.Private.IGNElapsed.ElapsedOn10s");
    } else if (ignType == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms)) {
        signalName = QString("SFC.Private.IGNElapsed.ElapsedOff0ms");
    } else if (ignType == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff500ms)) {
        signalName = QString("SFC.Private.IGNElapsed.ElapsedOff500ms");
    } else if (ignType == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff700ms)) {
        signalName = QString("SFC.Private.IGNElapsed.ElapsedOff700ms");
    } else if (ignType == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff1000ms)) {
        signalName = QString("SFC.Private.IGNElapsed.ElapsedOff1000ms");
    } else if (ignType == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff1500ms)) {
        signalName = QString("SFC.Private.IGNElapsed.ElapsedOff1500ms");
    } else {
        signalName = QString("IGNElapsed type error");
    }
    return signalName;
}

QPair<QStringList, QStringList> ExcelUtil::isConvertedIGNElapsedInfo(const QStringList& ignOriginData) {
    int ignValue = 0;
    bool foundOn = false;
    bool foundOn0ms = false;
    bool foundOff = false;
    bool foundOff0ms = false;

    for (const auto& ign : ignOriginData) {
        ignValue = ign.toInt();
        if (ignValue < static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms)) {
            if (foundOn == false) {
                foundOn = true;
            }
            if (ignValue == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms)) {
                foundOn0ms = true;
            }
        } else {
            if (foundOff == false) {
                foundOff = true;
            }
            if (ignValue == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms)) {
                foundOff0ms = true;
            }
        }
    }

    QList<int> ignAppend;
    bool appendOnOms = ((foundOn == false) && (foundOff0ms == false)) || ((foundOn == true) && (foundOff0ms == true));
    bool appendOffOms = ((foundOn0ms == false) && (foundOff == false)) || ((foundOn0ms == true) && (foundOff == true));
    bool appendOn0msOffOms = ((foundOn == true) && (foundOn0ms == false) && (foundOff == true) && (foundOff0ms == false));

    // Append - ElapsedOn0ms
    if ((appendOnOms) || (appendOn0msOffOms)) {
        ignAppend.append(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms));
    }
    // Append - ElapsedOff0ms
    if ((appendOffOms) || (appendOn0msOffOms)) {
        ignAppend.append(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms));
    }

    // qDebug() << "Ign Append Info :" << ignAppend.size() << appendOnOms << appendOffOms << appendOn0msOffOms;

    // Update - ConvertData
    QStringList ignConvertData = ignOriginData;
    for (const auto& ign : ignAppend) {
        QString ignStr = QString("%1").arg(ign);
        if (ignConvertData.contains(ignStr) == false) {
            ignConvertData.append(QString("%1").arg(ignStr));
        }
    }

    // Update - Precondition
    QStringList ignPrecondition;
    ignAppend.clear();
    for (const auto& ign : ignConvertData) {
        ignValue = ign.toInt();
        if (ignValue < static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms)) {
            ignAppend.append(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms));
        } else {
            ignAppend.append(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms));
        }
    }
    for (const auto& ign : ignAppend) {
        ignPrecondition.append(QString("%1").arg(ign));
    }

    if ((ignConvertData.size() == 1) && (ignPrecondition.size() == 1)) {
        // TCName 기준 Others 포함 Signal 구성시 On0ms / Off0ms  인 경우에
        // ignConvertData, ignPrecondition 데이터 하나씩만 구성되는 이슈
        // On0ms  : ConvertData  : 1 QList("0")       / Precondition : 1 QList("10")
        //       -> ConvertData  : 2 QList("0", "10") / Precondition : 2 QList("10", "0")
        // Off0ms : ConvertData  : 1 QList("10")      / Precondition : 1 QList("0")
        //       -> ConvertData  : 2 QList("10", "0") / Precondition : 2 QList("0", "10")

        // qDebug() << "\t ConvertData  :" << ignConvertData;
        // qDebug() << "\t Precondition :" << ignPrecondition;

        int covertValue = ignConvertData.at(0).toInt();
        if (covertValue == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms)) {
            covertValue = static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms);
        } else if (covertValue == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms)) {
            covertValue = static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms);
        } else {
        }

        int preconditionValue = ignPrecondition.at(0).toInt();
        if (preconditionValue == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms)) {
            preconditionValue = static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms);
        } else if (preconditionValue == static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms)) {
            preconditionValue = static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms);
        } else {
        }

        if (covertValue >= static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms)) {
            ignConvertData.append(QString("%1").arg(covertValue));
        }
        if (preconditionValue >= static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms)) {
            ignPrecondition.append(QString("%1").arg(preconditionValue));
        }
    }

#if 0
    qDebug() << "[isConvertedIGNElapsedInfo]";
    qDebug() << "\t On           :" << foundOn << ", On0ms  :" << foundOn0ms;
    qDebug() << "\t Off          :" << foundOff<< ", Off0ms :"  << foundOff0ms;
    qDebug() << "\t On0msOffOms  :" << appendOn0msOffOms;
    qDebug() << "\t OriginData   :" << ignOriginData;
    qDebug() << "\t ConvertData  :" << ignConvertData;
    qDebug() << "\t Precondition :" << ignPrecondition;
#endif

    return QPair<QStringList, QStringList>(ignConvertData, ignPrecondition);
}

int ExcelUtil::isConvertedKeywordType(const bool& toCustom, const int& keywordType) {
    int convertKeywordType = keywordType;
    if (toCustom) {
        if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotTrigger)) {
            convertKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::NotTrigger);
        } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomOver)) {
            convertKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Over);
        } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomUnder)) {
            convertKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Under);
        } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomRange)) {
            convertKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Range);
        } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomTwoWay)) {
            convertKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::TwoWay);
        } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomFlow)) {
            convertKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Flow);
        } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomConfig)) {
            convertKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Config);
        } else {
        }
    } else {
        if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::NotTrigger)) {
            convertKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotTrigger);
        } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Over)) {
            convertKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomOver);
        } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Under)) {
            convertKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomUnder);
        } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Range)) {
            convertKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomRange);
        } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::TwoWay)) {
            convertKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomTwoWay);
        } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Flow)) {
            convertKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomFlow);
        } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Config)) {
            convertKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomConfig);
        } else {
        }
    }
    return convertKeywordType;
}

QString ExcelUtil::isPreconditionMaxValue(const QString& signalName, const int& dataType, const int& keywordType,
                                          const QStringList& inputData, const QStringList& valueEnum) {
    const QString SFC_IGN_ELAPSED = QString("SFC.Private.IGNElapsed.Elapsed");

    // qDebug() << "\t isPreconditionMaxValue :" << dataType << keywordType << inputData.size() << valueEnum.size();
    if (signalName.trimmed().startsWith(SFC_IGN_ELAPSED)) {
        return QString();
    }

    if ((dataType != static_cast<int>(ivis::common::DataTypeEnum::DataType::HUInt64)) &&
        (dataType != static_cast<int>(ivis::common::DataTypeEnum::DataType::HInt64))) {
        return QString();
    }

    if (keywordType != static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid)) {
        return QString();
    }

    if (inputData.size() != 1) {
        return QString();
    }

    if (valueEnum.size() > 0) {
        return QString();
    }

    return QString("%1").arg(static_cast<quint64>(UINT32_MAX) + 1);
}
