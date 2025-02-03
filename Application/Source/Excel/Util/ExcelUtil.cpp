#include "ExcelUtil.h"

#include "CommonEnum.h"
#include "ConfigSetting.h"
#include "ExcelData.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

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

QStringList ExcelUtil::isModuleListFromJson(const int& appMode, const bool& toUpper) {
    bool appModeCV = (appMode == ivis::common::AppModeEnum::AppModeTypeCV);
    QString sfcModelPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcModelPath).toString();
    QString jsonFile = QString("%1/SFC/config/%2.json").arg(sfcModelPath).arg((appModeCV) ? ("CV") : ("platform"));
    QByteArray jsonData = ivis::common::FileInfo::readFileByteArray(jsonFile);

    // Json Parsing
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &error);
    if (error.error != QJsonParseError::NoError) {
        qDebug() << "Fail to json parsing error :" << error.errorString();
        return QStringList();
    }
    if (jsonDoc.isObject() == false) {
        qDebug() << "Fail to json format invalid";
        return QStringList();
    }

    // Read : Json Data
    QStringList tempModuleList = QStringList();
    QJsonObject jsonObj = jsonDoc.object();
    if (jsonObj.contains("SFCConfiguration") && jsonObj["SFCConfiguration"].isObject()) {  // Read : SFCConfiguration
        QJsonObject sfcConfig = jsonObj["SFCConfiguration"].toObject();
        // if (sfcConfig.contains("Version") && sfcConfig["Version"].isString()) {    // Read : Version
        //     QString version = sfcConfig["Version"].toString();
        // }
        // if (sfcConfig.contains("Description") && sfcConfig["Description"].isString()) {   // Read : Description
        //     QString description = sfcConfig["Description"].toString();
        // }
        if (sfcConfig.contains("SFCs") && sfcConfig["SFCs"].isArray()) {  // Read : SFCs
            for (const QJsonValue& module : sfcConfig["SFCs"].toArray()) {
                if (module.isString()) {
                    // qDebug() << "\t Module Json :" << module.toString();
                    tempModuleList.append(module.toString());
                }
            }
        }
    }
    if (appModeCV) {
        QDir commonModuleDir(QString("%1/SFC/CV/Common_Module").arg(sfcModelPath));
        for (const auto& commonModule : commonModuleDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            tempModuleList.append(commonModule);
            // qDebug() << "\t Module  Dir :" << commonModule;
        }
    }
    tempModuleList.sort();
    tempModuleList.removeDuplicates();

    QStringList moduleList;
    if (toUpper) {
        for (const auto& module : tempModuleList) {
            moduleList.append(module.toUpper());
        }
    } else {
        moduleList = tempModuleList;
    }

    qDebug() << "ModuelList :" << jsonFile << moduleList.size();
    // for (const auto& module : moduleList) {
    //     qDebug() << "\t Module :" << module;
    // }

    return moduleList;
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

int ExcelUtil::isDataType(const QString& dataTypeStr) {
    int dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::Invalid);

    if (dataTypeStr.compare("HUInt64") == 0) {
        dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::HUInt64);
    } else if (dataTypeStr.compare("HInt64") == 0) {
        dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::HInt64);
    } else if (dataTypeStr.compare("HString") == 0) {
        dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::HString);
    } else {
        // qDebug() << "isDataType -> DataType is incorrect :" << dataTypeStr;
    }
    return dataType;
}

QPair<int, int> ExcelUtil::isIGNElapsedType(const QString& singalName) {
    QPair<int, int> ignInfo;
    if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOn0ms") == 0) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOn500ms") == 0) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn500ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOn3000ms") == 0) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn3000ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOn3500ms") == 0) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn3500ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOn4000ms") == 0) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn4000ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOn10s") == 0) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn10s),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOff0ms") == 0) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff0ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOff500ms") == 0) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff500ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOff700ms") == 0) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff700ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOff1000ms") == 0) {
        ignInfo = QPair<int, int>(static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOff1000ms),
                                  static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms));
    } else if (singalName.compare("SFC.Private.IGNElapsed.ElapsedOff1500ms") == 0) {
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

bool ExcelUtil::isCheckPythonLibrary() {
#if defined(USE_PYTHON_LIB_CHECK_READ_WRITE)
    bool openpyxl = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeCheckLibOpenpyxl).toBool();
    bool pandas = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeCheckLibPandas).toBool();
    bool checkLib = ((openpyxl) && (pandas));

    if (checkLib == false) {
        ivis::common::PopupButton button = ivis::common::PopupButton::Invalid;
        QVariantList text =
            QVariantList({STRING_POPUP_LIB, STRING_POPUP_LIB_INSTALL_TIP, STRING_POPUP_INSTALL, STRING_POPUP_CANCEL});
        QVariant popupData = QVariant();
        button = ivis::common::Popup::drawPopup(ivis::common::PopupType::NoInstallLib, isHandler(), popupData, QVariant(text));
        if (button == ivis::common::PopupButton::Install) {
            mProcess.data()->setCommandInfo(QString("pip install openpyxl pandas"));
            mProcess.data()->start();
            connect(mProcess.data(), &ivis::common::ExcuteProgramThread::signalExcuteProgramCompleted, [&](const bool& result) {
                ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeCheckLibOpenpyxl, true);
                ConfigSetting::instance().data()->writeConfig(ConfigInfo::ConfigTypeCheckLibPandas, true);
                ivis::common::Popup::drawPopup(ivis::common::PopupType::InstallComplete, isHandler(), popupData,
                                               QVariant(QVariantList({STRING_POPUP_LIB, STRING_POPUP_NOW_INSTALLING_TIP})));
            });
            ivis::common::Popup::drawPopup(ivis::common::PopupType::NowInstalling, isHandler(), popupData,
                                           QVariant(QVariantList({STRING_POPUP_LIB, STRING_POPUP_INSTALL_COMPLETE_TIP})));
        }
    }
    qDebug() << "Check lib - openpyxl :" << openpyxl << ", pandas :" << pandas;
    return checkLib;
#else
    return true;
#endif
}

QString ExcelUtil::systemCall(const bool& readFile, const QVariant& filePath) {
    QString cmdType = ((readFile) ? ("read") : ("write"));
    QStringList fileInfo = filePath.toString().split("/");

    qDebug() << "systemCall() ->" << cmdType << "," << filePath;

    if (fileInfo.size() == 0) {
        qDebug() << "Fail to input file path (size : 0)";
    }

    QString dirPath = QString();
    for (int index = 0; index < (fileInfo.size() - 1); index++) {
        dirPath.append(fileInfo[index]);
        dirPath.append("/");
    }

    QString fileName = fileInfo[fileInfo.size() - 1];
    if ((fileName.contains(".xlsx", Qt::CaseInsensitive) == false) || (fileName.contains(".xls", Qt::CaseInsensitive) == false)) {
        fileName.append(".xlsx");
    }

    QString cmd =
        QString("python3 %1/ExcelParser.py %2 %3 %4").arg(ivis::common::APP_PWD()).arg(dirPath).arg(fileName).arg(cmdType);
    ivis::common::ExcuteProgram process(false);
    QStringList log;
    bool result = process.start(cmd, log);

    if (result) {
        dirPath.append("TC");
    } else {
        dirPath.clear();
    }

    qDebug() << "*************************************************************************************************";
    qDebug() << "PWD      :" << ivis::common::APP_PWD();
    qDebug() << "System   :" << ((result) ? ("<Success>") : ("<fail>")) << cmd;
    qDebug() << "FilePath :" << filePath;
    qDebug() << "DirPath  :" << dirPath;
    for (const auto& d : log) {
        qDebug() << "LogData  :" << d;
    }
    qDebug() << "*************************************************************************************************\n";

    return dirPath;
}

bool ExcelUtil::writeExcelSheet(const QVariant& filePath, const bool& convert) {
    // Set Path : file, directory
    QStringList fileInfo = filePath.toString().split("/");
    QString writePath = QString();
    for (int index = 0; index < (fileInfo.size() - 1); index++) {
        writePath.append(fileInfo[index]);
        writePath.append("/");
    }
    writePath.append("TC");

    QDir dir(writePath);
    if (dir.exists() == false) {
        dir.mkdir(writePath);
    }

    QStringList sheetName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toStringList();
    QStringList descTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDescTitle).toStringList();
    QStringList configTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeConfigTitle).toStringList();
    QStringList otherTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeOtherTitle).toStringList();

    int writeSize = 0;
    int sheetIndex = 0;
    int propertyType = (convert) ? (ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription)
                                 : (ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription);
    for (const auto& sheet : sheetName) {
        QString file = QString("%1_%2.toExcel").arg(sheetIndex++).arg(sheet);
        QString writeData = QString();
        QVariantList sheetData = QVariantList();

        // Title - Append
        QStringList contentTitle;
        if ((propertyType == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) ||
            (propertyType == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription)) {
            contentTitle = descTitle;
        } else if ((propertyType == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) ||
                   (propertyType == ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetConfigs)) {
            contentTitle = configTitle;
        } else {
            contentTitle = otherTitle;
        }
        sheetData.append(contentTitle);

        // Data - Append
        if (propertyType >= ivis::common::PropertyTypeEnum::PropertyTypeConvertSheetDescription) {
            sheetData.append(ExcelData::instance().data()->getSheetData(propertyType).toList());
        } else {
            sheetData.append(ExcelData::instance().data()->getSheetData(propertyType).toList());
        }
        propertyType++;

        for (const auto& dataInfo : sheetData) {
            QString rowData = QString();
            int count = 0;
            for (QVariant info : dataInfo.toList()) {
                rowData.append(info.toString());
                if (count++ < (dataInfo.toList().size() - 1)) {
                    rowData.append("\t");
                }
            }
            rowData.append("\n");
            writeData.append(rowData);
            // qDebug() << "RowData :" << rowData;
        }

        if (writeData.size() > 0) {
            QString writeFielPath = QString("%1/%2").arg(writePath).arg(file);
            int size = ivis::common::FileInfo::writeFile(writeFielPath, writeData, false);
            writeSize += size;
            if (size == 0) {
                qDebug() << "Fail to write size : 0, filePath :" << writeFielPath;
            }
        }
    }
    return (writeSize > 0);
}

QList<QVariantList> ExcelUtil::openExcelFile(const QString& filePath) {
    if (isCheckPythonLibrary() == false) {
        return QList<QVariantList>();
    }

    QString dirPath = systemCall(true, filePath);
    if (dirPath.size() == 0) {
        qDebug() << "Fail to dir path :" << filePath;
        return QList<QVariantList>();
    }

    const QString mergeStart = getMergeStart();
    const QString merge = getMergeStart();
    const QString mergeEnd = getMergeStart();
    const QStringList sheetName = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSheetName).toStringList();
    const QVariant descTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDescTitle);
    const QVariant configTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeConfigTitle);
    const QVariant otherTitle = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeOtherTitle);

    QList<QVariantList> sheetDataList;

    for (int sheetIndex = 0; sheetIndex < sheetName.size(); ++sheetIndex) {
        QString currSheetName = sheetName.at(sheetIndex);
        QString filePath = QString("%1/%2_%3.fromExcel").arg(dirPath).arg(sheetIndex).arg(currSheetName);
        QStringList readData = ivis::common::FileInfo::readFile(filePath);
        QStringList titleList;
        bool checkTitle = false;

        int properytType = sheetIndex + ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription;
        if (properytType == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetDescription) {
            titleList = descTitle.toStringList();
        } else if (properytType == ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetConfigs) {
            titleList = configTitle.toStringList();
        } else {
            titleList = otherTitle.toStringList();
            checkTitle = true;
        }

        QVariantList sheetData;
        int columnMax = titleList.size();
        QList<int> notSameTitleIndex;
        QList<int> readTitleIndex;
        for (int rowIndex = 0; rowIndex < readData.size(); rowIndex++) {
            QStringList rowData = readData[rowIndex].split("\t");
            if (rowIndex < 2) {
                // RowIndex[0] : column 인덱스(0, 1, 2, 3....) 정보

#if !defined(USE_SHEET_COLUMN_OLD)
                if ((rowIndex == 1) && (checkTitle)) {  // RowIndex[1] : title(desc, other) 정보
                    for (const auto& title : titleList) {
                        if (rowData.contains(title) == false) {
                            notSameTitleIndex.append(titleList.indexOf(title));
                        }
                    }
                    if (notSameTitleIndex.size() > 0) {
                        std::reverse(notSameTitleIndex.begin(), notSameTitleIndex.end());
                        qDebug() << "The title list is not the same :" << notSameTitleIndex << currSheetName;
                    }
                }
#endif
                continue;
            }

#if !defined(USE_SHEET_COLUMN_OLD)
            if (notSameTitleIndex.size() > 0) {
                QStringList temp = rowData;
                for (const auto& index : notSameTitleIndex) {
                    QString appendText;
                    int insertIndex = 0;
                    if (index == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Check)) {
                        insertIndex = 0;             // Befor index : TCName
                        appendText = rowData.at(0);  // Read index : TCName
                    } else if (index == static_cast<int>(ivis::common::ExcelSheetTitle::Other::GenType)) {
                        insertIndex = 1;             // After index : TCName
                        appendText = rowData.at(0);  // Read index : TCName
                    } else if (index == static_cast<int>(ivis::common::ExcelSheetTitle::Other::Config)) {
                        insertIndex = 2;             // Befor index : Result
                        appendText = rowData.at(0);  // Read index : Result
                    } else {
                        continue;
                    }

                    if (appendText.contains(mergeStart)) {
                        appendText = mergeStart;
                    } else if (appendText.contains(mergeEnd)) {
                        appendText = mergeEnd;
                    } else if (appendText.contains(merge)) {
                        appendText = merge;
                    } else {
                        appendText.clear();
                    }
                    temp.insert(insertIndex, appendText);
                }
                rowData = temp;
            }
#endif

            // 최대 사이즈 기준으로 정렬
            if (rowData.size() > columnMax) {
                // qDebug() << "The row data sizes are not same :" << properytType << rowData.size() << columnMax;
                rowData.resize(columnMax);
            }

            sheetData.append(rowData);
        }
        sheetDataList.append(sheetData);
        // qDebug() << "File Open :" << filePath << ", Length :" << sheetData.size();
        // qDebug() << currSheetName << ":" << sheetData;
        // qDebug() << "==================================================================================================\n";
    }

    // Delete : Folder(TC)
    if (ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDeleteFileTC).toBool()) {
        QStringList log;
        ivis::common::ExcuteProgram process(false);
        process.start(QString("rm -rf %1").arg(dirPath), log);  // Delete : /TC/*.fromExcel
    }

    return sheetDataList;
}
