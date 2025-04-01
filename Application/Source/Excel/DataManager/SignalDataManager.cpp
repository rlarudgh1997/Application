#include "SignalDataManager.h"

#include "CommonEnum.h"
#include "ConfigSetting.h"
#include "ExcelUtil.h"

#include <QRegularExpression>

const QString VEHICLE_TYPE_ICV = QString("ICV");
const QString VEHICLE_TYPE_EV = QString("EV");
const QString VEHICLE_TYPE_FCEV = QString("FCEV");
const QString VEHICLE_TYPE_PHEV = QString("PHEV");
const QString VEHICLE_TYPE_HEV = QString("HEV");
const QString SFC_IGN_ELAPSED = QString("SFC.Private.IGNElapsed.Elapsed");
const QString SFC_DONT_CARE = QString("D'");

QSharedPointer<SignalDataManager>& SignalDataManager::instance() {
    static QSharedPointer<SignalDataManager> gManager;
    if (gManager.isNull()) {
        gManager = QSharedPointer<SignalDataManager>(new SignalDataManager());
    }
    return gManager;
}

SignalDataManager::SignalDataManager() {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});

    setMergeStart(mergeStart);
    setMerge(merge);
    setMergeEnd(mergeEnd);
    setMergeInfos(QStringList({mergeStart, merge, mergeEnd}));
}

int SignalDataManager::isDataType(const QString& dataTypeStr) {
    int dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::Invalid);

    if (dataTypeStr.compare("HUInt64") == 0) {
        dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::HUInt64);
    } else if (dataTypeStr.compare("HInt64") == 0) {
        dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::HInt64);
    } else if (dataTypeStr.compare("HDouble") == 0) {
        dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::HDouble);
    } else if (dataTypeStr.compare("HString") == 0) {
        dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::HString);
    } else if (dataTypeStr.compare("HBool") == 0) {
        dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::HBool);
    } else {
        // qDebug() << "isDataType -> DataType is incorrect :" << dataTypeStr;
    }
    return dataType;
}

int SignalDataManager::isSignalType(const QString& signalName) {
    int signalType = static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Invalid);
    if (signalName.trimmed().startsWith("SFC.")) {
        if (signalName.trimmed().startsWith("SFC.Event.")) {
            signalType = static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcEvent);
        } else if ((signalName.trimmed().startsWith("SFC.Extension.")) || (signalName.trimmed().startsWith("SFC.Private."))) {
            signalType = static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcException);
        } else {
            signalType = static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Sfc);
        }
    } else if (signalName.trimmed().startsWith("Vehicle.")) {
        if (signalName.trimmed().startsWith("Vehicle.System.")) {
            signalType = static_cast<int>(ivis::common::SignalTypeEnum::SignalType::VehicleSystem);
        } else {
            signalType = static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Vehicle);
        }
    } else if (signalName.trimmed().startsWith("SFCFuel.Fuel_System.")) {
        signalType = static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcExternal);
    } else if (signalName.trimmed().startsWith("SFC_Common_Enum")) {
        signalType = static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcCommon);
    } else {
    }
    return signalType;
}

QString SignalDataManager::isSfcFileInfo(const QString& signalName) {
    int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    bool appModePV = (appMode == ivis::common::AppModeEnum::AppModeTypePV);
    int signalType = isSignalType(signalName);
    QString sfcModelPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcModelPath).toString();
    QStringList signalSplit = signalName.split(".");
    QString moduleName;
    QStringList fileList;

    if (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcExternal)) {
        fileList.append(QString("%1/SFC/extension/external.yml").arg(sfcModelPath));
    } else if (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcEvent)) {
        fileList.append(QString("%1/SFC/extension/Event.yml").arg(sfcModelPath));
    } else if (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcCommon)) {
        QString sfcCommonEnum = QString("SFC_Common_Enum%1").arg((appModePV) ? ("") : ("_CV"));
        fileList.append(QString("%1/SFC/extension/%2.yml").arg(sfcModelPath).arg(sfcCommonEnum));
    } else if (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcException)) {
        moduleName = signalSplit.at(2);  // signalName : SFC.Extension.{MODULE_NAME}.* | SFC.Private.{MODULE_NAME}.*
    } else if (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Sfc)) {
        if (signalSplit.size() > 1) {
            moduleName = signalSplit.at(1);  // signalName : SFC.{MODULE_NAME}.*
        } else {
            moduleName = signalSplit.at(0).toUpper();  // signalName : ABS_CV ->  Description 자동완성 참조 파일(대문자 변경)
        }
    } else {
    }

    if (moduleName.size() > 0) {
        QStringList sfcTypeList = QStringList();

        if (appModePV) {
            sfcTypeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcSpecTypePV).toStringList();
        } else {
            sfcTypeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcSpecTypeCV).toStringList();
            moduleName.replace("_New", "_CV");  // ModuleName : ADAS_Driving_New -> ADAS_Driving_CV
        }

        for (const auto& type : sfcTypeList) {
            fileList.append(QString("%1/SFC/%2/%3/%4.yml").arg(sfcModelPath).arg(type).arg(moduleName).arg(moduleName));
            if (appModePV == false) {
                fileList.append(QString("%1/SFC/%2/%3_CV/%4_CV.yml").arg(sfcModelPath).arg(type).arg(moduleName).arg(moduleName));
            }
        }
    }

    // qDebug() << "========================================================================================================";
    // qDebug() << "isSfcFileInfo";
    // qDebug() << "\t Info :" << signalName << signalType << moduleName;

    QString ymlFile;
    for (const auto& file : fileList) {
        if (QFile::exists(file) == false) {
            continue;
        }
        // PV : list.at(0) 파일을 YML 로 사용
        // CV : list.at(0 ~ max) 파일중 "_CV" 가 있는 첫번째 파일 사용, 아닌 경우 마지막 파일을 사용
        ymlFile = file;
        // qDebug() << "\t Yml :" << ymlFile;
        if ((appModePV) || (file.contains("_CV"))) {
            break;
        }
    }
    // qDebug() << "\t Yml :" << (ymlFile.size() > 0) << ymlFile;

    return ymlFile;
}

QStringList SignalDataManager::isVsmFileInfo(const QString& vehicleName, const QStringList& specType) {
    QStringList fileName = QStringList();
    QString vsmPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcModelPath).toString();
    vsmPath.append("/VSM");
    QString fileNameBase = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVsmFileNameBaseCV).toString();
    int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    if (appMode == ivis::common::AppModeEnum::AppModeTypePV) {
        fileNameBase = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVsmFileNameBasePV).toString();
    }

    for (const auto& spec : specType) {
        if (vehicleName.compare("System") == 0) {
            fileName.append(QString("%1/%2.%3.vsm").arg(vsmPath).arg(vehicleName).arg(spec));
        } else {
            fileName.append(QString("%1/%2").arg(vsmPath).arg(fileNameBase.arg(vehicleName).arg(spec)));
        }
    }
    return fileName;
}

QMap<int, QStringList> SignalDataManager::isSignalFileList(const QString& signalName, const QString& vehicleType) {
    QMap<int, QStringList> fileList = QMap<int, QStringList>();
    int signalType = isSignalType(signalName);

    if ((signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Vehicle)) ||
        (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::VehicleSystem))) {
        QStringList typeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcSpecTypeCV).toStringList();
        int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
        if (appMode == ivis::common::AppModeEnum::AppModeTypePV) {
            typeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcSpecTypePV).toStringList();
        }

        // ValueEnum
        fileList[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum] = isVsmFileInfo(QString("SKEL"), typeList);

        // MatchinigTable : CV(ICV, EV, FCEV)        PV(ICV, EV, FCEV, PHEV, HEV)
        for (const auto& vehicle : vehicleType.split(", ")) {
            int inputDataType = 0;
            if (vehicle.compare(VEHICLE_TYPE_ICV) == 0) {
                inputDataType = ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableICV;
            } else if (vehicle.compare(VEHICLE_TYPE_EV) == 0) {
                inputDataType = ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableEV;
            } else if (vehicle.compare(VEHICLE_TYPE_FCEV) == 0) {
                inputDataType = ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableFCEV;
            } else if (vehicle.compare(VEHICLE_TYPE_PHEV) == 0) {
                inputDataType = ivis::common::InputDataTypeEnum::InputDataTypeMatchingTablePHEV;
            } else if (vehicle.compare(VEHICLE_TYPE_HEV) == 0) {
                inputDataType = ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableHEV;
            } else {
                continue;
            }
            fileList[inputDataType] = isVsmFileInfo(vehicle, typeList);
        }

        // MatchinigTable : System
        // typeList = QStringList({"Config", "Engineering", "Extra", "Gateway", "HardWire", "Micom", "TP", "Undefined"});
        typeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSystemTypePV).toStringList();
        fileList[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableSystem] = isVsmFileInfo(QString("System"), typeList);
    } else {
        fileList[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum].append(isSfcFileInfo(signalName));
    }

    // qDebug() << "isSignalFileList :" << signalName << vehicleType << fileList;
    return fileList;
}

QString SignalDataManager::isSFCCommonEnum(const QString& info, const QString& prefix) {
    QString commonEnum;

    QStringList exceptionValueEnum = info.split(prefix);
    QString exceptionSignal = (exceptionValueEnum.size() == 2) ? (exceptionValueEnum.at(1)) : ("");
    if (exceptionSignal.size() > 0) {
        // ./Model/SFC/CV/ABS_CV/ABS_CV.yml
        // - SFC.ABS_CV.Telltale.ABS_CV.Stat:
        //   valueEnum: SFC_Common_Enum_CV.Telltale.Stat

        // ./Model/SFC/CV/Settings_CV/Settings_CV.yml
        // - SFC.Extension.Settings.Inter_WelcomeSoundSetStatus:
        //   valueEnum: SFC_Common_Enum_CV.EnumValue0

        // ./Model/SFC/CD/Settings/Settings.yml
        // - SFC.Extension.Settings.Inter_WelcomeSoundSetStatus:
        //   valueEnum: SFC_Common_Enum.Disable_Enable_EnumValue

        commonEnum = exceptionValueEnum[1].remove(" ");
        qDebug() << "isSFCCommonEnum :" << commonEnum;
    }

    return commonEnum;
}

QMap<int, QStringList> SignalDataManager::isParsingFileDataInfo(const QString& signalName, const QStringList& inputData,
                                                                const QMap<int, QStringList>& fileList, int& dataType) {
    const QString PREFIX_TYPE = QString("type:");
    const QString PREFIX_SIGNAL_NAME = QString("signalName:");
    const QString PREFIX_DATA_TYPE = QString("dataType:");
    const QString PREFIX_DESCRIPTION = QString("description:");
    const QString PREFIX_ABSTRACTION_NAME = QString("abstractionName:");
    const QString PREFIX_VALUE_ENUM = QString("valueEnum:");
    const QString PREFIX_MATCHING_TABLE = QString("matchingTable:");
    const QString PREFIX_CODE_COMMENTING = QString("# ");
    const QString PREFIX_HYPHEN = QString("-");
    const QString PREFIX_DOT = QString(".");
    const QString PREFIX_COLON = QString(":");
    const QString PREFIX_SPACE = QString(" ");
    // const QString PREFIX_MCAN = QString("_MCAN");
    // const QString PREFIX_CCAN = QString("_CCAN");

    const int signalType = isSignalType(signalName);
    if (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Invalid)) {
        qDebug() << "Input signal type is invalid";
        return QMap<int, QStringList>();
    }

    QString signal = QString();
    int index = 0;
    int startAppendIndex = 2;
    if (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::VehicleSystem)) {
        startAppendIndex = 3;
    } else if (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcCommon)) {
        startAppendIndex = 1;
    } else {
    }
    for (const auto& splitText : signalName.split(PREFIX_DOT)) {
        if (index >= startAppendIndex) {
            signal.append((signal.size() > 0) ? (".") : (""));
            signal.append(splitText);
        }
        index++;
    }
    if (signal.size() == 0) {
        qDebug() << "Signal name is null";
        return QMap<int, QStringList>();
    }

    // qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    // qDebug() << "Input  Signal      :" << signalName;
    // qDebug() << "Parser Signal      :" << signal;

    QMap<int, QStringList> sfcDataInfo = QMap<int, QStringList>();
    QString dataTypeStr = QString();
    QMapIterator<int, QStringList> iter(fileList);
    while (iter.hasNext()) {
        iter.next();
        int inputDataType = iter.key();
        QStringList fileName = iter.value();
        // qDebug() << inputDataType << ". Size :" << fileName.size() << ", File :" << fileName;
        QStringList valueEunm = QStringList();
        QStringList matchingTable = QStringList();
        for (const auto& file : fileName) {
            if (QFile::exists(file) == false) {
                // qDebug() << "\t\t Fail to file not exists :" << file;
                continue;
            }
            QStringList readData = ivis::common::FileInfo::readFile(file);
            QStringList vsmInfo = QStringList();
            bool foundSignal = false;
            for (QString lineStr : readData) {
                if (foundSignal) {
                    if ((lineStr.trimmed().startsWith(PREFIX_HYPHEN)) && (lineStr.trimmed().endsWith(PREFIX_COLON))) {
                        // if ((lineStr.contains(PREFIX_HYPHEN)) && (lineStr.contains(PREFIX_COLON))) {
                        // qDebug() << "\t Next  Signal[" << inputDataType << "] :" << lineStr;
                        break;
                    } else if ((lineStr.size() == 0) || (lineStr.contains(PREFIX_TYPE)) ||
                               // (lineStr.contains(PREFIX_DATA_TYPE)) ||   // Skip : dataType
                               (lineStr.contains(PREFIX_SIGNAL_NAME)) || (lineStr.contains(PREFIX_ABSTRACTION_NAME)) ||
                               (lineStr.contains(PREFIX_DESCRIPTION)) || (lineStr.contains(PREFIX_CODE_COMMENTING))) {
                        // Skip : type, signalName, description, abstractionName, #(주석)
                        continue;
                    } else {
                        // Append : ValueEnum, MatchingTable
                        lineStr.remove(PREFIX_SPACE);
                        vsmInfo.append(lineStr);
                    }
                } else {
                    if ((lineStr.contains(signal)) && (lineStr.contains(PREFIX_HYPHEN)) && (lineStr.contains(PREFIX_COLON))) {
                        // Input Signal : (Vehicle.Speed_Gauge.Output_DisplaySpeedUnit)
                        // Read  Signal : (- Speed_Gauge.Output_DisplaySpeedUnit:)
                        lineStr.remove(PREFIX_HYPHEN);
                        lineStr.remove(PREFIX_COLON);
                        lineStr.remove(PREFIX_SPACE);

                        foundSignal = true;
                        if ((signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Vehicle)) ||
                            (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::VehicleSystem))) {
                            foundSignal = (lineStr.compare(signal) == 0);
                        }
                        // qDebug() << ((foundSignal) ? ("\t Found") : ("\t Skip ")) << "Signal[" << inputDataType
                        //          << "] :" << lineStr;
                    }
                }
            }

            bool foundValueEnum = false;
            bool foundMatchingTable = false;
            bool foundDataType = false;
            for (const auto& info : vsmInfo) {
                if (info.contains(PREFIX_DATA_TYPE)) {
                    dataTypeStr = info;
                    dataTypeStr.remove(PREFIX_DATA_TYPE);
                    continue;
                }
                if (foundMatchingTable) {
                    foundValueEnum = false;
                    matchingTable.append(info);
                    // qDebug() << "\t MatchingTable :" << info;
                } else {
                    foundMatchingTable = (info.contains(PREFIX_MATCHING_TABLE));
                }

                if ((signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::VehicleSystem)) ||
                    (inputDataType == ivis::common::InputDataTypeEnum::InputDataTypeValueEnum)) {
                    if (foundValueEnum) {
                        if (info.contains(PREFIX_MATCHING_TABLE)) {
                            continue;
                        }
                        valueEunm.append(info);
                        // qDebug() << "\t ValueEnum     :" << info;
                    } else {
                        QString commonEnum = isSFCCommonEnum(info, PREFIX_VALUE_ENUM);
                        if (commonEnum.size() > 0) {
                            valueEunm.append(commonEnum);
                        } else {
                            foundValueEnum = info.contains(PREFIX_VALUE_ENUM);
                        }
                    }
                }
            }
        }

        if (inputDataType == ivis::common::InputDataTypeEnum::InputDataTypeValueEnum) {
            sfcDataInfo[inputDataType] = valueEunm;
            // qDebug() << "\t Value    Size :" << valueEunm.size();
        } else {
            sfcDataInfo[inputDataType] = matchingTable;
            // qDebug() << "\t Matching Size :" << matchingTable.size();
            if ((signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::VehicleSystem)) &&
                (valueEunm.size() > 0)) {
                sfcDataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum] = valueEunm;
                // qDebug() << "\t Value    Size :" << valueEunm.size();
            }
        }
    }

    if (inputData.size() > 0) {
        sfcDataInfo[ivis::common::InputDataTypeEnum::InputDataTypeInputData] = inputData;
    }
    dataType = isDataType(dataTypeStr);
    // qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";

    return sfcDataInfo;
}

QMap<int, QStringList> SignalDataManager::isSignalDataList(const QString& signalName, const QStringList& inputData,
                                                           const QString& vehicleType, int& dataType) {
    const int signalType = isSignalType(signalName);
    if (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Invalid)) {
        // ex) collected
        // qDebug() << "\t Signal :" << signalName << "-> SignalType::Invalid";
        return QMap<int, QStringList>();
    }

    QMap<int, QStringList> fileList = isSignalFileList(signalName, vehicleType);
    QMap<int, QStringList> sfcDataInfo = isParsingFileDataInfo(signalName, inputData, fileList, dataType);
    QStringList valueEnum = sfcDataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum];
    QString sfcCommonSignal;

#if 0
    sfcCommonSignal =
        ((signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Sfc)) && (valueEnum.size() == 1))
            ? (valueEnum.at(0)) : ("");
#else
    if (valueEnum.size() == 1) {
        if ((valueEnum.at(0).trimmed().startsWith("SFC_Common_Enum.")) ||
            (valueEnum.at(0).trimmed().startsWith("SFC_Common_Enum_CV."))) {
            sfcCommonSignal = valueEnum.at(0);
        }
    }
#endif

    if (sfcCommonSignal.size() > 0) {
        fileList = isSignalFileList(sfcCommonSignal, vehicleType);
        QMap<int, QStringList> sfcCommonDataInfo = isParsingFileDataInfo(sfcCommonSignal, inputData, fileList, dataType);
        QStringList sfcCommonValueEnum = sfcCommonDataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum];
        sfcDataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum] = sfcCommonValueEnum;
#if 0
        qDebug() << "=================================================================================================";
        qDebug() << "isSignalDataList";
        qDebug() << "\t Signal      :" << signalName << "->" << sfcCommonSignal;
        qDebug() << "\t DataTyp     :" << dataType;
        qDebug() << "\t FileList    :" << fileList;
        qDebug() << "\t 1 ValueEnum :" << valueEnum;
        qDebug() << "\t 2 ValueEnum :" << sfcCommonValueEnum;
        qDebug() << "=================================================================================================\n\n";
#endif
    }

    return sfcDataInfo;
}

QString SignalDataManager::isSignalValueEnum(const QString& signalName, const QString& value) {
    QString matchingValue;  // not used
    QStringList currentEnum = isSignalValueEnum(true, signalName);
    QStringList currentValue = isSignalValueEnum(false, signalName);
    bool toEnum = value.toLower().trimmed().startsWith("0x");
    int index = (toEnum) ? (currentValue.indexOf(value)) : (currentEnum.indexOf(value));

    QString resultValue;

    if (index >= 0) {
        QStringList currentList = (toEnum) ? (currentEnum) : (currentValue);
        if (index < currentList.size()) {
            resultValue = currentList.at(index);
        }
    }
    // qDebug() << "isSignalValueEnum :" << signalName << "," << value << "->" << resultValue;
    return resultValue;
}

QStringList SignalDataManager::isSignalValueEnum(const bool& toEnum, const QString& signalName) {
    QString matchingValue;  // not used
    QStringList valueEnum = isConvertedSignalData(toEnum, signalName, QStringList(), matchingValue);
    // qDebug() << "isSignalValueEnum :" << toEnum << signalName << valueEnum;
    return valueEnum;
}

QStringList SignalDataManager::isConvertedSignalData(const bool& toEnum, const QString& signalName, const QStringList& valueEnum,
                                                     QString& matchingValue) {
    int signalType = isSignalType(signalName);
    bool vehicleState = ((signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Vehicle)) ||
                         (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::VehicleSystem)));
    int enumIndex = (vehicleState) ? (1) : (0);
    int valueIndex = (vehicleState) ? (0) : (1);

    QStringList currentValueEnum = valueEnum;
    if (currentValueEnum.size() == 0) {
        QStringList signalData = QStringList();
        int dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::Invalid);
        QMap<int, QStringList> dataInfo = isSignalDataList(signalName, signalData, QString(), dataType);
        currentValueEnum = dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum];
    }

    QStringList convertDataInfo;
    QString compareMatchingValue = matchingValue;
    matchingValue.clear();
    for (const auto& v : currentValueEnum) {
        QString tempValueEnum = v;
        QStringList splitData = tempValueEnum.remove("\"").split(":");
        if (splitData.size() < 2) {
            continue;
        }
        QString data = ((toEnum) ? (splitData.at(enumIndex)) : (splitData.at(valueIndex)));
        convertDataInfo.append(data);
        if (compareMatchingValue.size() > 0) {
            QString matchingData = ((toEnum) ? (splitData.at(valueIndex)) : (splitData.at(enumIndex)));
            // qDebug() << "Matching :" << matchingData << compareMatchingValue << data;
            if (matchingData.compare(compareMatchingValue) == 0) {
                matchingValue = data;
            }
        }
    }
    return convertDataInfo;
}

void SignalDataManager::isConvertedExceptionData(const QString& signalName, const QMap<int, QStringList>& dataInfo,
                                                 QStringList& checkDataList) {
    const auto checkInfo = QMap<ivis::common::KeywordTypeEnum::KeywordType, QPair<QString, QString>>({
        {ivis::common::KeywordTypeEnum::KeywordType::Timeout, qMakePair("TIMEOUT", "timeout")},
        {ivis::common::KeywordTypeEnum::KeywordType::Crc, qMakePair("CRCERROR", "crc")},
    });
    const auto valueEnum = dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum];
    const auto matchingTableList = QList<QStringList>({
        dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableICV],
        dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableEV],
        dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableFCEV],
        dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTablePHEV],
        dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableHEV],
        dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableSystem],
    });

    QString checkStrMain;
    QString checkStrSub;
    QString changeStr;
    int foundIndex = (-1);
    // ConvertData : [MESSAGE_TIMEOUT, CRC_ERROR] 유무 확인 후 정보 구성 -> (checkStr, changeStr, foundIndex)
    for (const auto& key : checkInfo.keys()) {
        // MESSAGE_TIMEOUT or CRC_ERROR
        checkStrMain = ExcelUtil::instance().data()->isKeywordString(static_cast<int>(key));
        if (checkDataList.filter(checkStrMain).isEmpty() == false) {
            QPair<QString, QString> info = checkInfo[key];
            checkStrSub = info.first;
            changeStr = info.second;
            foundIndex = checkDataList.indexOf(checkStrMain);
            foundIndex = (foundIndex < checkDataList.size()) ? (foundIndex) : (-1);
            break;
        }
    }
    if (foundIndex < 0) {
        // ConvertData 에 [MESSAGE_TIMEOUT, CRC_ERROR] 존재하지 않음
        return;
    }

#if 0
    qDebug() << "========================================================================================";
    qDebug() << "[isConvertedExceptionData] : " << checkDataList;
    for (const auto& key : dataInfo.keys()) {
        if (key >= ivis::common::InputDataTypeEnum::InputDataTypeInputData) {
            continue;
        }
        qDebug() << "\t DataInfo[" << key << "] :" << dataInfo[key];
    }
#endif

    const QStringList tempDataList = checkDataList;
    bool foundState = false;
    for (const auto& dataList : matchingTableList) {
        // MatchingTable : [TIMEOUT, CRCERROR], ELSE 유무 확인
        if ((dataList.filter(checkStrSub).isEmpty() == false) || (dataList.filter(QString("ELSE")).isEmpty() == false)) {
            foundState = true;
            break;
        }
    }

    if (foundState == false) {
        QString valueEnumHex;
        // ValueEnum : [MESSAGE_TIMEOUT, CRC_ERROR] 유무 확인
        for (const auto& data : valueEnum) {
            if (data.contains(checkStrMain)) {
                valueEnumHex = data;
                valueEnumHex.remove(checkStrMain);
                valueEnumHex.remove(QRegularExpression(R"([":\s])"));  // Delete - ":", "\"", " "
                break;
            }
        }

        bool replaceState = false;
        if (valueEnumHex.size() > 0) {
            for (const auto& matchingTable : matchingTableList) {
                for (const auto& data : matchingTable) {
                    if (data.contains(valueEnumHex)) {
                        replaceState = true;
                        break;
                    }
                }
                if (replaceState) {
                    break;
                }
            }
        }

        if (replaceState == false) {
            // ConvertData : [MESSAGE_TIMEOUT, CRC_ERROR] 삭제
            checkDataList.removeAll(checkStrMain);
            qDebug() << "1. Exception - Remove :" << tempDataList << "->" << checkDataList;
        } else {
            // ConvertData : [MESSAGE_TIMEOUT, CRC_ERROR] 유지
            checkDataList[foundIndex] = checkStrMain;
            qDebug() << "2. Exception - Keep   :" << tempDataList << "->" << checkDataList;
        }
    } else {
        // ConvertData : [MESSAGE_TIMEOUT, CRC_ERROR] 변환 [timeout, crc]
        checkDataList[foundIndex] = changeStr;
        // qDebug() << "3. Exception - Change :" << tempDataList << "->" << checkDataList;
    }
}

#if 0
QString SignalDataManager::isCheckBothExceptionValue(const QMap<int, QStringList>& dataInfo, const QString& originStr,
                                                     const QString& checkStr) {
    const auto valueEnumData = dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum];

    QString exceptionValue;

    // 1. Check : (ValueEnum : MESSAGE_TIMEOUT) && (MatchingTable : TIMEOUT)  =>> timeout
    // 2. Check : (ValueEnum : CRC_ERROR)       && (MatchingTable : CRCERROR) =>> crc
    if (valueEnumData.filter(originStr).isEmpty() == false) {
        for (const auto& key : dataInfo.keys()) {
            if ((key <= ivis::common::InputDataTypeEnum::InputDataTypeValueEnum) ||
                (key >= ivis::common::InputDataTypeEnum::InputDataTypeInputData)) {
                continue;
            }

            auto matchingTableData = dataInfo[key];
            // qDebug() << "\t\t ContainString :" << matchingTableData << ">>" << checkStr;
            if (matchingTableData.filter(checkStr).isEmpty() == false) {
                exceptionValue = checkStr.toLower();
                break;
            }
        }
    }

    // qDebug() << "isCheckBothExceptionValue :" << originStr << checkStr;
    // qDebug() << "\t ValueEnum :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum];
    // qDebug() << "\t MatchingTableICV  :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableICV];
    // qDebug() << "\t MatchingTableEV   :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableEV];
    // qDebug() << "\t MatchingTableFCEV :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableFCEV];
    // qDebug() << "\t MatchingTablePHEV :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTablePHEV];
    // qDebug() << "\t MatchingTableHEV  :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableHEV];
    // qDebug() << "\t Found             :" << foundState << exceptionValue;

    return exceptionValue;
}

QPair<QStringList, QStringList> SignalDataManager::isCheckExceptionValueEnum(const QString& signalName,
                                                                             const QMap<int, QStringList>& dataInfo) {
    const QString originTimeOut =
        ExcelUtil::instance().data()->isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Timeout));
    const QString checkTimeOut = QString("TIMEOUT");
    const QStringList valueEnum = dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum];
    QStringList originData = dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeInputData];
    QPair<QStringList, QStringList> exceptionData(originData, originData);

    int signalType = isSignalType(signalName);

    // qDebug() << "isCheckExceptionValueEnum :" << signalName << signalType << originData;

    if ((signalType > static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Invalid)) &&
        (signalType < static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Vehicle))) {
        // qDebug() << "not support sfc signal :" << signalName;
        return exceptionData;
    }

    for (const auto& data : originData) {
        if (data.contains(originTimeOut) == false) {
            // qDebug() << "origindata does not contain MESSAGE_TIMEOUT :" << signalName;
            return exceptionData;
        }
    }

    QString exceptionValue = isCheckBothExceptionValue(dataInfo, originTimeOut, checkTimeOut);
    if (exceptionValue.size() > 0) {  // Chceck MatchingTable : TIMEOUT
        // qDebug() << "both value contains is ValueEnum(MESSAGE_TIMEOUT), MatchingTable(TIMEOUT) :" << signalName;
        exceptionData = QPair<QStringList, QStringList>(QStringList(), QStringList({exceptionValue}));
        return exceptionData;
    }

    for (auto value : valueEnum) {
        if (value.contains(originTimeOut)) {
            originData.removeAll(originTimeOut);
            exceptionData = QPair<QStringList, QStringList>(QStringList({originTimeOut}), originData);
            // qDebug() << "other exception data :" << signalName;
            return exceptionData;
        }
    }

    // 예외처리
    const int startIndex = ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableICV;
    const QStringList keywordList = QStringList({"TIMEOUT:", "CRCERROR:", "ELSE:"});
    QString matchingValue;

    for (const auto& keyword : keywordList) {
        for (int index = startIndex; index < ivis::common::InputDataTypeEnum::InputDataTypeMax; ++index) {
            for (auto& matchingTable : dataInfo[index]) {
                if (matchingTable.contains(keyword)) {
                    matchingValue = matchingTable.remove(keyword);
                    break;
                }
            }
        }
    }

    QString matchingEnum = matchingValue;
    isConvertedSignalData(true, signalName, valueEnum, matchingEnum);
    if (matchingEnum.size() > 0) {
        originData.removeAll(originTimeOut);
        originData.removeAll(checkTimeOut.toLower());
        originData.removeAll(matchingEnum);
        matchingEnum = checkTimeOut.toLower();
    } else {
        for (auto& origin : originData) {
            if (origin.contains(originTimeOut)) {
                origin.replace(originTimeOut, checkTimeOut.toLower());
                // qDebug() << signalName << ":" << originTimeOut << "->" << checkTimeOut.toLower();
            }
        }
    }

    exceptionData = QPair<QStringList, QStringList>(QStringList({matchingEnum}), originData);

#if 0
    qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    qDebug() << "isCheckExceptionValueEnum :" << signalName;
    qDebug() << "\t Matching        :" << matchingValue << matchingEnum;
    qDebug() << "\t InputData       :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeInputData];
    qDebug() << "\t OriginData      :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeInputData];
    qDebug() << "\t ExceptionFirst  :" << exceptionData.first;
    qDebug() << "\t ExceptionSecond :" << exceptionData.second;
    qDebug() << "\t ValueEnum       :" << valueEnum;
    // qDebug() << "\t 1 MatchingTable :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableICV];
    // qDebug() << "\t 2 MatchingTable :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableEV];
    // qDebug() << "\t 3 MatchingTable :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableFCEV];
    // qDebug() << "\t 4 MatchingTable :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTablePHEV];
    // qDebug() << "\t 5 MatchingTable :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableHEV];
    // qDebug() << "\t 6 MatchingTable :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableSystem];
    qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
#endif

    return exceptionData;
}
#endif

bool SignalDataManager::isExceptionSignal(const QString& signalName) {
    if (ivis::common::isCompareString(signalName, QString("collect"))) {
        return true;
    }
    return false;
}

bool SignalDataManager::isMultiKeywordData(const QStringList& dataList) {
    QRegularExpression regex(R"(\[([^\[\]]+)\])");    // 정규식 : 대괄호
    QStringList keywordList;

    // DataList : "[CustomMoreThanEqual][1279]", "[1280]", "[CustomUnder][1280]", "[1279]"
    for (const QString& item : dataList) {
        QRegularExpressionMatchIterator it = regex.globalMatch(item);
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            QString captured = match.captured(1);
            if (captured.contains(QRegularExpression("[A-Za-z]"))) {    // 정규식 : 문자
                keywordList.append(QString("[%1]").arg(captured));    // [CustomMoreThanEqual], [CustomUnder]
            }
        }
    }

    int multiCount = 0;
    if (keywordList.size() > 1) {
        QList<QPair<QString, int>> customKeywordPatternList =
            ExcelUtil::instance().data()->isKeywordPatternInfo(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max));
        for (const auto& keyword : keywordList) {
            for (const auto& pattern : customKeywordPatternList) {
                if (keyword == pattern.first) {
                    multiCount++;
                }
            }

            if (multiCount > 1) {
                break;
            }
        }
    }

    // qDebug() << "isMultiKeywordData :" << KeywordList << "<-" << dataList;

    return (multiCount > 1);
}

QMap<QString, SignalData> SignalDataManager::isSignalDataInfo(const QStringList& signalList, const QStringList& dataList,
                                                              QMap<QString, QMap<int, QStringList>>& dataInfo) {
    QMap<QString, SignalData> signalDataInfo;

    const int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    const int configVehicleType = (appMode == ivis::common::AppModeEnum::AppModeTypePV) ? (ConfigInfo::ConfigTypeVehicleTypePV)
                                                                                        : (ConfigInfo::ConfigTypeVehicleTypeCV);
    const QStringList vehicleList = ConfigSetting::instance().data()->readConfig(configVehicleType).toStringList();
    const QString vehicleType = vehicleList.join(", ");

    // Custom Keyword String : 예외 처리 코드 추가
    QStringList keywordString;
    for (int index = 0; index <= static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max); ++index) {
        for (const auto& infoPair : ExcelUtil::instance().data()->isKeywordPatternInfo(index)) {
            keywordString.append(infoPair.first);
        }
        keywordString.removeDuplicates();
    }
    QStringList exceptionKeyword({
        ExcelUtil::instance().data()->isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Timeout)),
        ExcelUtil::instance().data()->isKeywordString(static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Crc)),
    });
    for (const auto& exception : exceptionKeyword) {
        keywordString.removeAll(exception);
    }

    // qDebug() << "isSignalDataInfo :" << signalList.size() << dataList.size();
    // qDebug() << "\t VehicleType :" << vehicleType;
    // qDebug() << "\t Keyword     :" << keywordString << "\n";

    QMap<QString, QStringList> allConvertData;

    for (int index = 0; index < signalList.size(); ++index) {
        int dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::Invalid);
        QString signalName = signalList[index];
        QString tempData = dataList[index];
        QStringList signalData = tempData.remove(" ").split(",");
        QMap<int, QStringList> currDataInfo = isSignalDataList(signalName, signalData, vehicleType, dataType);

        int keywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid);
        QStringList originData;
        QStringList convertData;
        QStringList valueEnum;
        QStringList notUsedEnum;
        QStringList precondition;

        if (dataType == static_cast<int>(ivis::common::DataTypeEnum::DataType::Invalid)) {
            if (isExceptionSignal(signalName)) {
                dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::HUInt64);
                originData = signalData;
                convertData = originData;
            }
        } else {
            QString keywordContainStr = (signalData.size() == 0) ? (QString()) : (signalData.at(0));  // 0 : 키워드 포함 데이터
            keywordType = ExcelUtil::instance().data()->isKeywordType(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max),
                                                                      keywordContainStr);

            originData = currDataInfo[ivis::common::InputDataTypeEnum::InputDataTypeInputData];
            convertData = originData;
            valueEnum = currDataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum];

            QString matchingValue;
            notUsedEnum = isConvertedSignalData(true, signalName, valueEnum, matchingValue);

            QStringList temp;
            for (auto& data : convertData) {
                keywordString.append("[");
                keywordString.append("]");
                ivis::common::getRemoved(data, keywordString);  // convertData 에서 키워드 정보만 삭제
                notUsedEnum.removeAll(data);

                if (data.size() > 0) {
                    temp.append(data);
                }
            }

            if (temp.size() > 0) {
                allConvertData[signalName].append(temp);
            }
        }

        if (isMultiKeywordData(originData)) {
            qDebug() << "[Multi-keyword data deletes the origin data]";
            qDebug() << "\t Signal     :" << signalName;
            qDebug() << "\t OriginData :" << originData;
            originData.clear();
        }

        if (dataType != static_cast<int>(ivis::common::DataTypeEnum::DataType::Invalid)) {
            dataInfo[signalName] = currDataInfo;
            signalDataInfo[signalName] = SignalData(signalName, dataType, false, keywordType, originData, convertData, valueEnum,
                                                    notUsedEnum, precondition);
        }
    }

    // Update : All Convert Data
    for (const auto& key : allConvertData.keys()) {
        auto data = allConvertData[key];
        data.removeDuplicates();
        // data.sort();
        signalDataInfo[key].setAllConvertData(data);
    }

    return signalDataInfo;
}

QMap<int, QPair<QString, SignalData>> SignalDataManager::isSortingInputSignalList(const QMap<QString, SignalData>& dataInfo,
                                                                                  const QStringList& signalList) {
    QMap<int, QPair<QString, SignalData>> signalDataInfo;
    QStringList newSignalList;

    for (const auto& signalName : signalList) {
        if (ivis::common::isContainsString(signalName, SFC_IGN_ELAPSED)) {
            // qDebug() << "\t Found ign signal :" << signalName;
            newSignalList.append(SFC_IGN_ELAPSED);
        } else {
            newSignalList.append(signalName);
        }
    }
    newSignalList.removeDuplicates();

    for (const auto& signalName : newSignalList) {
        // qDebug() << "Signal :" << signalName;
        if (dataInfo.contains(signalName) == false) {
            // qDebug() << "\t Not found signal :" << signalName;
            continue;
        }
        signalDataInfo[signalDataInfo.size()] = qMakePair(signalName, dataInfo[signalName]);
    }

    return signalDataInfo;
}

QMap<int, QPair<QString, SignalData>> SignalDataManager::isNormalInputSignalDataInfo(
    const QPair<QStringList, QStringList>& list) {
    QStringList signalList = list.first;
    QStringList dataList = list.second;
    signalList.removeAll("");
    dataList.removeAll("");

    QMap<QString, QMap<int, QStringList>> dataInfo;
    QMap<QString, SignalData> currentSignalDataInfo = isSignalDataInfo(signalList, dataList, dataInfo);
    QMap<QString, SignalData> newSignalDataInfo;

    for (const auto& signalName : currentSignalDataInfo.keys()) {
        if ((ivis::common::isContainsString(signalName, SFC_IGN_ELAPSED)) &&
            (ivis::common::isCompareString(signalName, SFC_IGN_ELAPSED) == false)) {
            // qDebug() << "\t Skip IgnElapsed Signal :" << signalName;
            continue;
        }
        newSignalDataInfo[signalName] = currentSignalDataInfo[signalName];
    }
    QMap<int, QPair<QString, SignalData>> signalDataInfo = isSortingInputSignalList(newSignalDataInfo, signalList);

#if 0
    qDebug() << "*************************************************************************************************";
    qDebug() << "1. isNormalInputSignalDataInfo :" << signalList.size() << dataList.size() << signalDataInfo.size();
    qDebug() << "=================================================================================================";
    for (const auto& mapKey : signalDataInfo.keys()) {
        auto signalName = signalDataInfo[mapKey].first;
        auto signalData = signalDataInfo[mapKey].second;
        qDebug() << "[" << signalName << "]";
        qDebug() << "\t DataType       :" << signalData.getDataType();
        qDebug() << "\t Initialize     :" << signalData.getInitialize();
        qDebug() << "\t KeywordType    :" << signalData.getKeywordType();
        qDebug() << "\t ValueEnum      :" << signalData.getValueEnum().size() << signalData.getValueEnum();
        qDebug() << "\t OriginData     :" << signalData.getOriginData().size() << signalData.getOriginData();
        qDebug() << "\t ConvertData    :" << signalData.getConvertData().size() << signalData.getConvertData();
        qDebug() << "\t NotUsedEnum    :" << signalData.getNotUsedEnum().size() << signalData.getNotUsedEnum();
        qDebug() << "\t Precondition   :" << signalData.getPrecondition().size() << signalData.getPrecondition();
        qDebug() << "\t AllConvertData :" << signalData.getAllConvertData().size() << signalData.getAllConvertData();
        qDebug() << "\n\n";
    }
    qDebug() << "*************************************************************************************************\n\n";
#endif

    return signalDataInfo;
}

QMap<int, QPair<QString, SignalData>> SignalDataManager::isTestCaseInputSignalDataInfo(
    const QPair<QStringList, QStringList>& list, QMap<QString, SignalData>& newSignalDataInfo) {
    QStringList signalList = list.first;
    QStringList dataList = list.second;
    signalList.removeAll("");
    dataList.removeAll("");

    QMap<QString, QMap<int, QStringList>> dataInfo;
    QMap<QString, SignalData> currentSignalDataInfo = isSignalDataInfo(signalList, dataList, dataInfo);

    for (auto signalName : currentSignalDataInfo.keys()) {
        SignalData signalData = currentSignalDataInfo[signalName];
        int dataType = signalData.getDataType();
        bool init = false;
        int keywordType = signalData.getKeywordType();
        QStringList originData = signalData.getOriginData();
        QStringList convertData = signalData.getConvertData();
        QStringList valueEnum = signalData.getValueEnum();
        QStringList notUsedEnum = signalData.getNotUsedEnum();
        QStringList precondition = signalData.getPrecondition();

        bool ignElaspedSingal = ivis::common::isContainsString(signalName, SFC_IGN_ELAPSED);

        if (ignElaspedSingal) {
            QPair<int, int> ignInfo = ExcelUtil::instance().data()->isIGNElapsedType(signalName);
            signalName = SFC_IGN_ELAPSED;
            if (ignInfo.first >= static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms)) {
                convertData = QStringList({QString("%1").arg(ignInfo.first)});
                precondition = QStringList({QString("%1").arg(ignInfo.second)});
            }
            if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotTrigger)) {
                convertData.clear();
                precondition.clear();
            }
        } else {
            QString preconditionMaxValue =
                ExcelUtil::instance().data()->isPreconditionMaxValue(signalName, dataType, keywordType, convertData, valueEnum);

            if (preconditionMaxValue.size() > 0) {
                precondition = QStringList({preconditionMaxValue});
            } else {
                // bool checkExceptionValueEnum = true;
                switch (keywordType) {
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotUsed): {
                        convertData.clear();
                        notUsedEnum.clear();
                        precondition.clear();
                        break;
                    }
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotTrigger): {
                        // convertData = convertData;
                        precondition.clear();
                        notUsedEnum.clear();
                        break;
                    }
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomOver):
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomUnder):
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomRange):
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomFlow):
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomTwoWay):
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomMoreThanEqual):
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomLessThanEqual):
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotDefined): {
                        int splitSize = convertData.size() * 0.5;
                        precondition = convertData.mid(0, splitSize);
                        convertData = convertData.mid(splitSize, convertData.size());
                        notUsedEnum.clear();
                        break;
                    }
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomIgn): {
                        break;
                    }
                    default: {
                        // checkExceptionValueEnum = false;
                        precondition = notUsedEnum;
                        notUsedEnum.clear();
                        break;
                    }
                }
                keywordType = ExcelUtil::instance().data()->isConvertedKeywordType(true, keywordType);

#if 1
                isConvertedExceptionData(signalName, dataInfo[signalName], convertData);
                isConvertedExceptionData(signalName, dataInfo[signalName], precondition);
#else
                if (checkExceptionValueEnum) {
                    // Data Check : MESSAGE_TIMEOUT, timeout
                    QPair<QStringList, QStringList> exceptionData = isCheckExceptionValueEnum(signalName, dataInfo[signalName]);
                    if (exceptionData.first != exceptionData.second) {
                        convertData = exceptionData.first;
                        precondition = exceptionData.second;
                    }
                } else {
                    QString originTimeOut = ExcelUtil::instance().data()->isKeywordString(
                        static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Timeout));

                    // qMakePair(QString("CRC_ERROR"), static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Crc)),

                    QString checkTimeOut = QString("TIMEOUT");
                    auto currDataInfo = dataInfo[signalName];

                    for (auto& data : convertData) {  // ConvertData Check : MESSAGE_TIMEOUT, CRC_ERROR 이 있는 경우 변경
                        if (data.contains(originTimeOut)) {
                            data = isCheckBothExceptionValue(currDataInfo, originTimeOut, checkTimeOut);
                        }
                    }
                    for (auto& data : precondition) {  // Precondition Check : MESSAGE_TIMEOUT, CRC_ERROR 이 있는 경우 변경
                        if (data.contains(originTimeOut)) {
                            data = isCheckBothExceptionValue(currDataInfo, originTimeOut, checkTimeOut);
                        }
                    }
                }
#endif
            }
        }

        convertData.removeDuplicates();
        precondition.removeDuplicates();

        newSignalDataInfo[signalName] =
            SignalData(signalName, dataType, init, keywordType, originData, convertData, valueEnum, notUsedEnum, precondition);
    }
    QMap<int, QPair<QString, SignalData>> signalDataInfo = isSortingInputSignalList(newSignalDataInfo, signalList);

#if 0
    qDebug() << "*************************************************************************************************";
    qDebug() << "2. isTestCaseInputSignalDataInfo :" << signalList.size() << dataList.size() << signalDataInfo.size();
    qDebug() << "=================================================================================================";
    for (const auto& mapKey : signalDataInfo.keys()) {
        auto signalName = signalDataInfo[mapKey].first;
        auto signalData = signalDataInfo[mapKey].second;
        qDebug() << "[" << signalName << "]";
        qDebug() << "\t DataType       :" << signalData.getDataType();
        qDebug() << "\t Initialize     :" << signalData.getInitialize();
        qDebug() << "\t KeywordType    :" << signalData.getKeywordType();
        qDebug() << "\t ValueEnum      :" << signalData.getValueEnum().size() << signalData.getValueEnum();
        qDebug() << "\t OriginData     :" << signalData.getOriginData().size() << signalData.getOriginData();
        qDebug() << "\t ConvertData    :" << signalData.getConvertData().size() << signalData.getConvertData();
        qDebug() << "\t NotUsedEnum    :" << signalData.getNotUsedEnum().size() << signalData.getNotUsedEnum();
        qDebug() << "\t Precondition   :" << signalData.getPrecondition().size() << signalData.getPrecondition();
        qDebug() << "\t AllConvertData :" << signalData.getAllConvertData().size() << signalData.getAllConvertData();
        qDebug() << "\n\n";
    }
    qDebug() << "*************************************************************************************************\n\n";
#endif

    return signalDataInfo;
}

QMap<int, QPair<QString, SignalData>> SignalDataManager::isOtherInputSignalDataInfo(
    const QPair<QStringList, QStringList>& list, QMap<QString, SignalData>& newSignalDataInfo) {
    QStringList signalList = list.first;
    QStringList dataList = list.second;
    signalList.removeAll("");
    dataList.removeAll("");

    QMap<QString, QMap<int, QStringList>> dataInfo;
    QMap<QString, SignalData> currentSignalDataInfo = isSignalDataInfo(signalList, dataList, dataInfo);

    QStringList ignOriginData;
    for (auto signalName : currentSignalDataInfo.keys()) {
        SignalData signalData = currentSignalDataInfo[signalName];
        int dataType = signalData.getDataType();
        bool init = false;
        int keywordType = signalData.getKeywordType();
        QStringList originData = signalData.getOriginData();
        QStringList convertData = signalData.getConvertData();
        QStringList valueEnum = signalData.getValueEnum();
        QStringList notUsedEnum = signalData.getNotUsedEnum();
        QStringList precondition = signalData.getPrecondition();
        QStringList allConvertData = signalData.getAllConvertData();

        bool ignElaspedSingal = ivis::common::isContainsString(signalName, SFC_IGN_ELAPSED);

        if (ignElaspedSingal) {
            QPair<int, int> ignInfo = ExcelUtil::instance().data()->isIGNElapsedType(signalName);
            if (ignInfo.first >= static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms)) {
                ignOriginData.append(QString("%1").arg(ignInfo.first));
            }
            continue;
        } else {
            QString preconditionMaxValue =
                ExcelUtil::instance().data()->isPreconditionMaxValue(signalName, dataType, keywordType, convertData, valueEnum);

            if (preconditionMaxValue.size() > 0) {
                precondition = QStringList({preconditionMaxValue});
            } else {
                if (valueEnum.size() == 0) {
                    if (allConvertData.size() > 0) {
                        convertData = allConvertData;
                    }
                } else {
                    QString tempMatchingValue;  // not used
                    convertData = isConvertedSignalData(true, signalName, valueEnum, tempMatchingValue);
#if 1
                    isConvertedExceptionData(signalName, dataInfo[signalName], convertData);
#else
                    QString originTimeOut = ExcelUtil::instance().data()->isKeywordString(
                        static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Timeout));
                    QString checkTimeOut = QString("TIMEOUT");
                    for (auto& data : convertData) {
                        if (data.compare(originTimeOut) == 0) {
                            data.replace(originTimeOut, checkTimeOut.toLower());
                            break;
                        }
                    }
#endif
                }
                keywordType = ExcelUtil::instance().data()->isConvertedKeywordType(true, keywordType);

                notUsedEnum.clear();
                precondition.clear();
            }
        }

        convertData.removeDuplicates();
        precondition.removeDuplicates();

        newSignalDataInfo[signalName] =
            SignalData(signalName, dataType, init, keywordType, originData, convertData, valueEnum, notUsedEnum, precondition);
    }

    if (ignOriginData.size() > 0) {
        // 전체 시그널에서 ign 시그널 만 별도로 변환한 값을 저장 후 전체 ign 시그널에 대하여 시그널 변경 및 데이터 구성 동작함
        int ignDataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::HUInt64);
        int ignKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid);
        QPair<QStringList, QStringList> ingDataIfo = ExcelUtil::instance().data()->isConvertedIGNElapsedInfo(ignOriginData);
        QStringList ignConvertData = ingDataIfo.first;
        QStringList ignPrecondition = ingDataIfo.second;
        newSignalDataInfo[SFC_IGN_ELAPSED] = SignalData(SFC_IGN_ELAPSED, ignDataType, false, ignKeywordType, ignOriginData,
                                                        ignConvertData, QStringList(), QStringList(), ignPrecondition);
    }
    QMap<int, QPair<QString, SignalData>> signalDataInfo = isSortingInputSignalList(newSignalDataInfo, signalList);

#if 0
    qDebug() << "*************************************************************************************************";
    qDebug() << "3. isOtherInputSignalDataInfo :" << signalList.size() << dataList.size() << signalDataInfo.size();
    qDebug() << "=================================================================================================";
    for (const auto& mapKey : signalDataInfo.keys()) {
        auto signalName = signalDataInfo[mapKey].first;
        auto signalData = signalDataInfo[mapKey].second;
        // for (const auto& key : newSignalDataInfo.keys()) {
        //     auto signalName = key;
        //     auto signalData = newSignalDataInfo[key];
        qDebug() << "[" << signalName << "]";
        qDebug() << "\t DataType       :" << signalData.getDataType();
        qDebug() << "\t Initialize     :" << signalData.getInitialize();
        qDebug() << "\t KeywordType    :" << signalData.getKeywordType();
        qDebug() << "\t ValueEnum      :" << signalData.getValueEnum().size() << signalData.getValueEnum();
        qDebug() << "\t OriginData     :" << signalData.getOriginData().size() << signalData.getOriginData();
        qDebug() << "\t ConvertData    :" << signalData.getConvertData().size() << signalData.getConvertData();
        qDebug() << "\t NotUsedEnum    :" << signalData.getNotUsedEnum().size() << signalData.getNotUsedEnum();
        qDebug() << "\t Precondition   :" << signalData.getPrecondition().size() << signalData.getPrecondition();
        qDebug() << "\t AllConvertData :" << signalData.getAllConvertData().size() << signalData.getAllConvertData();
        qDebug() << "\n\n";
    }
    qDebug() << "*************************************************************************************************\n\n";
#endif

    return signalDataInfo;
}

QMap<int, QPair<QString, SignalData>> SignalDataManager::isOutputSignalDataInfo(const QList<QStringList>& list) {
    QStringList signalList;
    QStringList dataList;
    QMap<QString, QString> initList;

    for (const auto& info : list) {
        if (info.size() != 3) {  // Output : Signal, Init, Data
            continue;
        }
        QString signal = info.at(0);
        QString init = info.at(1);
        QString data = info.at(2);

        signalList.append(signal);
        dataList.append(data);
        initList[signal] = init;
    }

    signalList.removeAll("");
    dataList.removeAll("");

    QMap<QString, QMap<int, QStringList>> dataInfo;
    QMap<QString, SignalData> currentSignalDataInfo = isSignalDataInfo(signalList, dataList, dataInfo);
    QMap<QString, SignalData> newSignalDataInfo;

    for (const auto& signalName : currentSignalDataInfo.keys()) {
        SignalData signalData = currentSignalDataInfo[signalName];
        int dataType = signalData.getDataType();
        bool init = (initList[signalName].size() > 0);
        int keywordType = signalData.getKeywordType();
        QStringList originData = signalData.getOriginData();
        QStringList convertData = signalData.getConvertData();
        QStringList valueEnum = signalData.getValueEnum();
        QStringList notUsedEnum = signalData.getNotUsedEnum();
        QStringList precondition = signalData.getPrecondition();

        newSignalDataInfo[signalName] =
            SignalData(signalName, dataType, init, keywordType, originData, convertData, valueEnum, notUsedEnum, precondition);
    }

    QMap<int, QPair<QString, SignalData>> signalDataInfo;
    for (const auto& signalName : signalList) {
        signalDataInfo[signalDataInfo.size()] = qMakePair(signalName, newSignalDataInfo[signalName]);
    }

#if 0
    qDebug() << "*************************************************************************************************";
    qDebug() << "4. isOutputSignalDataInfo :" << signalList.size() << dataList.size() << signalDataInfo.size();
    qDebug() << "=================================================================================================";
    for (const auto& mapKey : signalDataInfo.keys()) {
        auto signalName = signalDataInfo[mapKey].first;
        auto signalData = signalDataInfo[mapKey].second;
        qDebug() << "[" << signalName << "]";
        qDebug() << "\t DataType       :" << signalData.getDataType();
        qDebug() << "\t Initialize     :" << signalData.getInitialize();
        qDebug() << "\t KeywordType    :" << signalData.getKeywordType();
        qDebug() << "\t ValueEnum      :" << signalData.getValueEnum().size() << signalData.getValueEnum();
        qDebug() << "\t OriginData     :" << signalData.getOriginData().size() << signalData.getOriginData();
        qDebug() << "\t ConvertData    :" << signalData.getConvertData().size() << signalData.getConvertData();
        qDebug() << "\t NotUsedEnum    :" << signalData.getNotUsedEnum().size() << signalData.getNotUsedEnum();
        qDebug() << "\t Precondition   :" << signalData.getPrecondition().size() << signalData.getPrecondition();
        qDebug() << "\t AllConvertData :" << signalData.getAllConvertData().size() << signalData.getAllConvertData();
        qDebug() << "\n\n";
    }
    qDebug() << "*************************************************************************************************\n\n";
#endif

    return signalDataInfo;
}

QMap<int, QPair<QString, SignalData>> SignalDataManager::isDependSignalDataInfo(const QList<QStringList>& list) {
    QStringList signalList;
    QStringList dataList;
    QMap<QString, QString> initList;

    for (const auto& info : list) {
        if (info.size() != 3) {  // Output : Signal, Init, Data
            continue;
        }
        QString signal = info.at(0);
        QString init = info.at(1);
        QString data = info.at(2);

        signalList.append(signal);
        dataList.append(data);
        initList[signal] = init;
    }

    signalList.removeAll("");
    dataList.removeAll("");

    QMap<QString, QMap<int, QStringList>> dataInfo;
    QMap<QString, SignalData> currentSignalDataInfo = isSignalDataInfo(signalList, dataList, dataInfo);
    QMap<QString, SignalData> newSignalDataInfo;

    for (const auto& signalName : currentSignalDataInfo.keys()) {
    }

    QMap<int, QPair<QString, SignalData>> signalDataInfo;
    for (const auto& signalName : signalList) {
        signalDataInfo[signalDataInfo.size()] = qMakePair(signalName, newSignalDataInfo[signalName]);
    }

#if 0
    qDebug() << "*************************************************************************************************";
    qDebug() << "5. isDependSignalDataInfo :" << signalList.size() << dataList.size() << signalDataInfo.size();
    qDebug() << "=================================================================================================";
    for (const auto& mapKey : signalDataInfo.keys()) {
        auto signalName = signalDataInfo[mapKey].first;
        auto signalData = signalDataInfo[mapKey].second;
        qDebug() << "[" << signalName << "]";
        qDebug() << "\t DataType       :" << signalData.getDataType();
        qDebug() << "\t Initialize     :" << signalData.getInitialize();
        qDebug() << "\t KeywordType    :" << signalData.getKeywordType();
        qDebug() << "\t ValueEnum      :" << signalData.getValueEnum().size() << signalData.getValueEnum();
        qDebug() << "\t OriginData     :" << signalData.getOriginData().size() << signalData.getOriginData();
        qDebug() << "\t ConvertData    :" << signalData.getConvertData().size() << signalData.getConvertData();
        qDebug() << "\t NotUsedEnum    :" << signalData.getNotUsedEnum().size() << signalData.getNotUsedEnum();
        qDebug() << "\t Precondition   :" << signalData.getPrecondition().size() << signalData.getPrecondition();
        qDebug() << "\t AllConvertData :" << signalData.getAllConvertData().size() << signalData.getAllConvertData();
        qDebug() << "\n\n";
    }
    qDebug() << "*************************************************************************************************\n\n";
#endif

    return signalDataInfo;
}

bool SignalDataManager::isExcelDataValidation() {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const int originStart = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetPrivates;
    const int originEnd = ivis::common::PropertyTypeEnum::PropertyTypeOriginSheetMax;

    bool validCheck = true;
    QMap<int, QStringList> checkDataList;

    // [동작 조건]
    // 1. TCName 은 중복 되면 안됨
    // 2. TCName 기준으로 Result 는 중복 되면 안됨
    // 3. Reuslt 기준으로 Case 는 중복 되면 안됨
    // 4. 현재 동작 기준 : TCName, Result, Case 별 중복 되는지만 체크 하고 있음
    // 5. 추후 함수 동작 조건 수정 필요함
    QVariantList temp;
    for (int sheetIndex = originStart; sheetIndex < originEnd; ++sheetIndex) {
        for (const auto& rowDataList : temp) {
            // for (const auto& rowDataList : getData(sheetIndex).toList()) {
            QStringList rowData = rowDataList.toStringList();
            if (rowData.size() < (static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max))) {
                // qDebug() << "Fail to sheet data list size :" << rowData.size();
                continue;
            }

            QMap<int, QString> infoText =
                QMap<int, QString>({{static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName),
                                     rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::TCName))},
                                    {static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result),
                                     rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Result))},
                                    {static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case),
                                     rowData.at(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Case))}});

            for (auto iter = infoText.cbegin(); iter != infoText.cend(); ++iter) {
                int key = iter.key();
                QString text = iter.value();
                if ((text.contains(merge) == false) && (text.contains(mergeEnd) == false)) {
                    text.remove(mergeStart);
                    text = text.toUpper();
                    checkDataList[key].append(text);
                }
            }
        }
    }

    for (auto iterCheck = checkDataList.cbegin(); iterCheck != checkDataList.cend(); ++iterCheck) {
        int key = iterCheck.key();
        QStringList textList = iterCheck.value();

        textList.removeAll("");
        int duplicateCount = textList.removeDuplicates();

        // 임시로 주석 처리함 -> 추후 체크 조건에 맞게 수정 필요
        // if ((duplicateCount > 0) || (textList.size() == 0)) {
        //     qDebug() << "Fail to excel data validation[" << key << "] :" << textList.size() << duplicateCount;
        //     validCheck = false;
        //     break;
        // }
    }

    if (validCheck == false) {
        qDebug() << "Fail to excel data validation.";
    }
    return validCheck;
}

QStringList SignalDataManager::extractMatchingSignal(const QString& filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text) == false) {
        qDebug() << "Fail to open file :" << filePath;
        return QStringList();
    }

    QStringList signalList;
    QRegularExpression signalRegex(R"(^\s*-\s*([^:]+):\s*(.*)$)");    // 시작 : "- "    끝 ":" 확인인
    QRegularExpression signalNameRegex(R"(signalName:)");    // "signalName:" 포함 확인
    QRegularExpression abstractionNameRegex(R"(abstractionName:)");    // " :" 포함 확인

    QString foundSignal;
    bool foundSignalName = false;
    bool foundAbstractionName = false;

    QTextStream in(&file);
    while (in.atEnd() == false) {
        QString line = in.readLine().trimmed();
        QRegularExpressionMatch match = signalRegex.match(line);

        if ((match.hasMatch()) || in.atEnd()) {
            if (foundSignal.size() == 0) {
                foundSignal = match.captured(1).trimmed() + " " + match.captured(2).trimmed();
                // qDebug() << "\t\t FoundSignal :" << foundSignal;
            }
        }

        if (foundSignal.size() > 0) {
            if ((foundSignalName == false) && (signalNameRegex.match(line).hasMatch())) {
                foundSignalName = true;
            }
            if ((foundAbstractionName == false) && (abstractionNameRegex.match(line).hasMatch())) {
                foundAbstractionName = true;
            }

            if ((foundSignalName) && (foundAbstractionName)) {
                signalList.append(foundSignal);
                foundSignal.clear();
                foundSignalName = false;
                foundAbstractionName = false;
            }
        }
    }
    file.close();

#if 0
    qDebug() << "=======================================================================================================";
    int index = 0;
    for (const auto& signal : signalList) {
        qDebug() << "\t SignalList[" << index++ << "] :" << signal;
    }
    qDebug() << "extractMatchingSignal :" << signalList.size() << file.fileName();
    qDebug() << "=======================================================================================================\n\n";
#endif

    return signalList;
}

QStringList SignalDataManager::isSignalListInfo(const bool& sfcSignal) {
    ivis::common::CheckTimer checkTimer;

    QString sfcModelPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcModelPath).toString();
    int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    QStringList typeList;
    QStringList vehicleTypeList;
    QString fileNameBase;

#if 0
    ConfigTypeVsmFileNameBasePV    CLU_VSM_%1.Vehicle.%2.vsm
    ConfigTypeVsmFileNameBaseCV    CLU_VSM_CV_%1.Vehicle.%2.vsm

    ConfigTypeVehicleTypePV        ICV, EV, FCEV, PHEV, HEV
    ConfigTypeVehicleTypeCV        ICV, EV, FCEV

    ConfigTypeSfcSpecTypePV        AD, AV, CD, CH, EC, HD, PT, ETC, extension
    ConfigTypeSfcSpecTypeCV        AV, CD, CV, EC, PT, ETC, extension

    ConfigTypeVsmSpecTypePV        AD, AV, CD, CH, EC, HD, PT, CS
    ConfigTypeVsmSpecTypeCV        CV

    ConfigTypeSystemTypePV         Config, Engineering, Extra, Gateway, HardWire, Micom, TP, Undefined

    isVsmFileInfo(const QString& vehicleName, const QStringList& specType) {
        QStringList fileName = QStringList();
        QString vsmPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcModelPath).toString();
        vsmPath.append("/VSM");
        QString fileNameBase = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVsmFileNameBaseCV).toString();
        int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
        if (appMode == ivis::common::AppModeEnum::AppModeTypePV) {
            fileNameBase = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVsmFileNameBasePV).toString();
        }

        for (const auto& spec : specType) {
            if (vehicleName.compare("System") == 0) {
                fileName.append(QString("%1/%2.%3.vsm").arg(vsmPath).arg(vehicleName).arg(spec));
            } else {
                fileName.append(QString("%1/%2").arg(vsmPath).arg(fileNameBase.arg(vehicleName).arg(spec)));
            }
        }
    }
#endif

    if (appMode == ivis::common::AppModeEnum::AppModeTypePV) {
        typeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcSpecTypePV).toStringList();
        vehicleTypeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVehicleTypePV).toStringList();
        fileNameBase = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVsmFileNameBasePV).toString();
    } else {
        typeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcSpecTypeCV).toStringList();
        vehicleTypeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVehicleTypeCV).toStringList();
        fileNameBase = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVsmFileNameBaseCV).toString();
    }


    // isSignalFileList();

    if (sfcSignal) {
    } else {
        for (const auto& vehicleType : vehicleTypeList) {
        }
    }


    QStringList fileList;

    if (sfcSignal) {
        sfcModelPath.append("/SFC/CV");
        fileList.append(sfcModelPath + "/ABS_CV/ABS_CV.yml");
        fileList.append(sfcModelPath + "/ABS_NO_ABS_Trailer/ABS_NO_ABS_Trailer.yml");
        fileList.append(sfcModelPath + "/ADAS_Driving_CV/ADAS_Driving_CV.yml");
        fileList.append(sfcModelPath + "/ADAS_PARKING_CV/ADAS_PARKING_CV.yml");
    } else {
        sfcModelPath.append("/VSM");
        fileList.append(sfcModelPath + "/CLU_VSM_CV_EV.Vehicle.CV.vsm");
        fileList.append(sfcModelPath + "/CLU_VSM_CV_FCEV.Vehicle.CV.vsm");
        fileList.append(sfcModelPath + "/CLU_VSM_CV_ICV.Vehicle.CV.vsm");
        fileList.append(sfcModelPath + "/CLU_VSM_CV_SKEL.Vehicle.CV.vsm");
        fileList.append(sfcModelPath + "/CLU_VSM_CV_System.Vehicle.CV.vsm");
    }


    auto moduleInfo = ExcelUtil::instance().data()->isModuleListFromJson(appMode, false);
    QStringList moduleList = moduleInfo.keys();






    // QString vehicleType = vehicleTypeList.toStringList().join(", ");



    QStringList signalList;
#if 0
    for (const auto& file : fileList) {
        signalList.append(extractMatchingSignal(file));
    }
    checkTimer.check("isSignalListInfo");
#endif

    return signalList;
}
