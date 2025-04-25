#include "SignalDataManager.h"

#include "CommonEnum.h"
#include "ConfigSetting.h"
#include "ExcelUtil.h"

#include <QRegularExpression>
#include <QRandomGenerator>

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
    } else if (signalName.trimmed().startsWith("SFC_Common_Enum.")) {
        signalType = static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcCommon);
    } else if (signalName.trimmed().startsWith("SFC_Common_Enum_CV.")) {
        signalType = static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcCommonCV);
    } else {
    }
    return signalType;
}

QString SignalDataManager::isSfcFileInfo(const QString& signalName) {
    int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    bool appModeCV = (appMode == ivis::common::AppModeEnum::AppModeTypeCV);
    int signalType = isSignalType(signalName);
    QString sfcModelPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcModelPath).toString();
    QStringList signalSplit = signalName.split(".");
    QString moduleName;
    QStringList fileList;

    switch (signalType) {
        case static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcExternal): {
            fileList.append(QString("%1/SFC/extension/external.yml").arg(sfcModelPath));
            break;
        }
        case static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcEvent): {
            fileList.append(QString("%1/SFC/extension/Event.yml").arg(sfcModelPath));
            break;
        }
        case static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcCommon): {
            fileList.append(QString("%1/SFC/extension/SFC_Common_Enum.yml").arg(sfcModelPath));
            break;
        }
        case static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcCommonCV): {
            fileList.append(QString("%1/SFC/extension/SFC_Common_Enum_CV.yml").arg(sfcModelPath));
            break;
        }
        case static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcException): {
            // signalName : SFC.Extension.{MODULE_NAME}.* | SFC.Private.{MODULE_NAME}.*
            moduleName = signalSplit.at(2);
            break;
        }
        case static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Sfc): {
            if (signalSplit.size() > 1) {
                moduleName = signalSplit.at(1);  // signalName : SFC.{MODULE_NAME}.*
            } else {
                moduleName = signalSplit.at(0).toUpper();  // signalName : ABS_CV ->  Description 자동완성 참조 파일(대문자 변경)
            }
            break;
        }
        default: {
            break;
        }
    }

    if (moduleName.size() > 0) {
        QStringList sfcTypeList = QStringList();

        if (appModeCV) {
            sfcTypeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcSpecTypeCV).toStringList();
            moduleName.replace("_New", "_CV");  // ModuleName : ADAS_Driving_New -> ADAS_Driving_CV
        } else {
            sfcTypeList = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeSfcSpecTypePV).toStringList();
        }

        for (const auto& type : sfcTypeList) {
            fileList.append(QString("%1/SFC/%2/%3/%4.yml").arg(sfcModelPath).arg(type).arg(moduleName).arg(moduleName));
            if (appModeCV) {
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
            continue;  // 파일이 존재하지 않으면 리스트 파일 사용 하지 않음
        }
        ymlFile = file;
        // qDebug() << "\t Yml :" << ymlFile;

        if (appModeCV == false) {
            break;  // PV : fileList.at(0) 파일 사용
        }
        if (file.contains("_CV")) {
            break;  // CV : fileList.at(0 ~ max) 파일중 "_CV" 가 있는 파일 사용
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

#if 0
    qDebug() << "isSignalFileList :" << signalName << vehicleType;
    for (const auto& key : fileList.keys()) {
        qDebug() << "\t File[" << key << "] :" << fileList[key];
    }
#endif

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

        // ./Model/SFC/AV/Navigation_TBT/Navigation_TBT.yml
        // - SFC.Navigation_TBT.Constant.NaviDistToTurn1.Stat
        //   valueEnum: SFC_Common_Enum.Off_Value_EnumValue

        // "valueEnum:" 지우고, 공백(" ")을 지우고 SFC_Common_Enum.Off_Value_EnumValue 사용

        commonEnum = exceptionValueEnum[1].remove(" ");
        // qDebug() << "isSFCCommonEnum :" << commonEnum;
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
    } else if ((signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcCommon)) ||
               (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::SfcCommonCV))) {
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

    if (valueEnum.size() == 1) {
        if ((valueEnum.at(0).trimmed().startsWith("SFC_Common_Enum.")) ||
            (valueEnum.at(0).trimmed().startsWith("SFC_Common_Enum_CV."))) {
            sfcCommonSignal = valueEnum.at(0);
        }
    }
    if (sfcCommonSignal.size() > 0) {
        fileList = isSignalFileList(sfcCommonSignal, vehicleType);
        QMap<int, QStringList> sfcCommonDataInfo = isParsingFileDataInfo(sfcCommonSignal, inputData, fileList, dataType);
        QStringList sfcCommonValueEnum = sfcCommonDataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum];
        sfcDataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum] = sfcCommonValueEnum;
#if 0
        qDebug() << "=================================================================================================";
        qDebug() << "isSignalDataList";
        qDebug() << "\t Signal     :" << signalName << "->" << sfcCommonSignal;
        qDebug() << "\t DataTyp    :" << dataType;
        qDebug() << "\t FileList   :" << fileList;
        qDebug() << "\t ValueEnum  :" << valueEnum;
        qDebug() << "\t CommonEnum :" << sfcCommonValueEnum;
        qDebug() << "=================================================================================================\n\n";
#endif
    }
    sfcDataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum] = isValidValueEnum(signalName, sfcDataInfo);

    return sfcDataInfo;
}

QStringList SignalDataManager::isValidValueEnum(const QString& signalName, const QMap<int, QStringList>& dataInfo) {
    const int signalType = isSignalType(signalName);
    QStringList valueEnum = dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum];

    if (valueEnum.size() == 0) {
        return valueEnum;
    }
    if ((signalType != static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Vehicle)) &&
        (signalType != static_cast<int>(ivis::common::SignalTypeEnum::SignalType::VehicleSystem))) {
        return valueEnum;
    }

    // Signal : Vehicle
    // (ValueEnum != MatchingTable) -> MatchingTable 에 존재하는 ValueEnum 만 사용 하도록 예외처리

    // QString matchingValue;
    // QStringList valueEnumHex = isConvertedSignalDataValueEnum(false, signalName, valueEnum, matchingValue);
    QStringList matchingTableHex = isConvertedSignalDataMatchingTable(true, signalName, dataInfo);

    if ((valueEnum.size() != matchingTableHex.size()) && (matchingTableHex.size() > 0)) {
        valueEnum.clear();

        for (const auto& matching : matchingTableHex) {
            for (const auto& value : dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum]) {
                if (value.contains(matching)) {
                    valueEnum.append(value);
                    break;
                }
            }
        }
#if 0
        qDebug() << "=================================================================================================";
        qDebug() << "isValidValueEnum";
        qDebug() << "\t Signal            :" << signalName;
        qDebug() << "\t ValueEnum         :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum];
        qDebug() << "\t MatchingTableICV  :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableICV];
        qDebug() << "\t MatchingTableEV   :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableEV];
        qDebug() << "\t MatchingTableFCEV :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableFCEV];
        qDebug() << "\t MatchingTablePHEV :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTablePHEV];
        qDebug() << "\t MatchingTableHEV  :" << dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableHEV];
        qDebug() << "\t MatchingTableHex  :" << matchingTableHex;
        qDebug() << "\t ValueEnum         :" << valueEnum;
        qDebug() << "=================================================================================================\n\n";
#endif
    }

    return valueEnum;
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
    QStringList signalData = QStringList();
    int dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::Invalid);
    QMap<int, QStringList> dataInfo = isSignalDataList(signalName, signalData, QString(), dataType);
    QStringList valueEnum = dataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum];

    QString matchingValue;  // not used
    QStringList convertValueEnum = isConvertedSignalDataValueEnum(toEnum, signalName, valueEnum, matchingValue);
    // qDebug() << "isSignalValueEnum :" << toEnum << signalName << convertValueEnum;
    return convertValueEnum;
}

QStringList SignalDataManager::isConvertedSignalDataValueEnum(const bool& toEnum, const QString& signalName,
                                                              const QStringList& valueEnum, QString& matchingValue) {
    const int signalType = isSignalType(signalName);
    const bool vehicleState = ((signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::Vehicle)) ||
                               (signalType == static_cast<int>(ivis::common::SignalTypeEnum::SignalType::VehicleSystem)));
    const int enumIndex = (vehicleState) ? (1) : (0);
    const int valueIndex = (vehicleState) ? (0) : (1);

    QStringList convertDataInfo;
    QString compareMatchingValue = matchingValue;
    matchingValue.clear();
    for (const auto& v : valueEnum) {
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

QStringList SignalDataManager::isConvertedSignalDataMatchingTable(const bool& toHex, const QString& signalName,
                                                                  const QMap<int, QStringList>& dataInfo) {
    QMap<int, QStringList> matchingTableData;
    for (const auto& key : dataInfo.keys()) {
        if ((key == ivis::common::InputDataTypeEnum::InputDataTypeValueEnum) ||
            (key >= ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableSystem)) {
            continue;
        }
        if (dataInfo[key].size() == 0) {
            continue;
        }

        for (const auto& data : dataInfo[key]) {
            // - ADAS_PARKING_CV.Input_IbuPdwSystemFailureStatus:
            //   type: sensor
            //   signalName: IBU_PDW_SysFlrSta
            //   dataType: HUInt64
            //   abstractionName: Input_IbuPdwSystemFailureStatus
            //   valueEnum:
            //     0x0: "OFF"
            //     0x1: "ULTRASONIC_SENSOR_BLOCKAGE"
            //     0x2: "PDW_FAILURE"
            //   matchingTable:
            //     0x2: 0x2
            //     ELSE: 0x0
            // -> Use Value Enum = 0x0: "OFF", 0x2: "PDW_FAILURE"
            QStringList splitData = data.split(":", Qt::SkipEmptyParts);
            if (splitData.size() != 2) {
                continue;
            }
            matchingTableData[key].append((toHex) ? (splitData.at(1)) : (splitData.at(0)));
        }
        // qDebug() << "\t MatchingTable[" << key << "] :" << dataInfo[key] << "->" << matchingTableData[key];
    }

    QStringList matchingTable;
    for (const auto& key : matchingTableData.keys()) {
        if (matchingTable.size() == 0) {
            matchingTable = matchingTableData[key];
            continue;
        }
        if (matchingTable != matchingTableData[key]) {
            qDebug() << "\t Matching table data does not match :" << key << ". The first matching table is used.";
            break;
        }
    }
    matchingTable.sort();

#if 0
    if (matchingTable.size() == 0) {
        qDebug() << "\t There is no matching table data :" << signalName;
        // for (const auto& key : dataInfo.keys()) {
        //     qDebug() << "\t\t MatchingTable[" << key << "] :" << dataInfo[key] << "->" << matchingTableData[key];
        // }
    }
#endif

    return matchingTable;
}

QList<QStringList> SignalDataManager::parsingKeywordBlocks(const QStringList& originData) {
    QRegularExpression keywordStartRegex(R"(\[Custom\w+\]\[?)");
    QList<QStringList> keywordBlocks;
    QStringList currentBlock;

    for (const QString& d : originData) {
        QString trimmed = d.trimmed();
        if (trimmed.contains(keywordStartRegex)) {
            if (currentBlock.size() > 0) {
                keywordBlocks.append(currentBlock);
                currentBlock.clear();
            }
        }
        currentBlock.append(trimmed);
    }

    if (currentBlock.size() > 0) {
        keywordBlocks.append(currentBlock);
    }

    // for (const auto& block : keywordBlocks) {
    //     qDebug() << "\t Keyword :" << block;
    // }
    return keywordBlocks;
}

QMap<int, QStringList> SignalDataManager::parsingKeywordData(const QStringList& originData) {
    QRegularExpression regex(R"(\[([^\[\]]+)\])");  // 정규식 : 대괄호
    QStringList keywordList;

    // originData : "[CustomMoreThanEqual][1279]", "[1280]", "[CustomUnder][1280]", "[1279]"
    for (const QString& d : originData) {
        QRegularExpressionMatchIterator it = regex.globalMatch(d);
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            QString captured = match.captured(1);
            if (captured.contains(QRegularExpression("[A-Za-z]"))) {  // 정규식 : 문자
                keywordList.append(QString("[%1]").arg(captured));    // [CustomMoreThanEqual], [CustomUnder] ....
            }
        }
    }

    QList<QPair<QString, int>> keywordPattern =
        ExcelUtil::instance().data()->isKeywordPatternInfo(static_cast<int>(ivis::common::ExcelSheetTitle::Other::Max));
    QList<QStringList> keywordBlocks = parsingKeywordBlocks(originData);
    QMap<int, QStringList> keywordDataMap;

    for (const auto& keyword : keywordList) {
        for (auto blocks : keywordBlocks) {
            if (blocks.size() == 0) {
                continue;
            }
            int keywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid);
            for (const auto& pattern : keywordPattern) {
                if (keyword == pattern.first) {
                    keywordType = static_cast<int>(pattern.second);
                    break;
                }
            }
            if (blocks.at(0).startsWith(keyword)) {
                blocks[0].remove(keyword);
                for (auto& b : blocks) {
                    b.replace("][", "], [");    // OriginData  : QList("[CustomOver][100][101]") 인 경우 변경
                }
                keywordDataMap[keywordType] = blocks;
                break;
            }
        }
    }

    // for (const auto& key : keywordDataMap.keys()) {
    //     qDebug() << "\t KeywordDataMap[" << key << "] :" << keywordDataMap[key];
    // }
    return keywordDataMap;
}

QMap<int, QStringList> SignalDataManager::isCustomValueInfo(const QStringList& originData, const bool& validValue) {
    QMap<int, QStringList> keywordDataMap = parsingKeywordData(originData);
    int keywordGroup = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordGroup::Normal);
    QMap<int, QStringList> dataInfo;

    switch (keywordDataMap.size()) {
        case 0: {
            // qDebug() << "\t Keyword Group - Invalid";
            keywordGroup = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordGroup::Normal);
            break;
        }
        case 1: {
            // qDebug() << "\t Keyword Group - Single";
            keywordGroup = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordGroup::Single);
            break;
        }
        default: {
            // qDebug() << "\t Keyword Group - Multi";
            keywordGroup = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordGroup::Multi);
            break;
        }
    }
    if (keywordGroup == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordGroup::Normal)) {
        dataInfo[static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid)] = originData;
        return dataInfo;
    }

    for (const auto& keywordType : keywordDataMap.keys()) {
        const QStringList values = keywordDataMap[keywordType];
        const QString joined = values.join(", ");

        QStringList splitValue = joined.split("], [");
        QStringList keywordData;
        QPair<QString, QString> validData;

        QString frontValue;
        QString rearValue;
        int index = 0;
        for (auto d : splitValue) {
            d.remove('[').trimmed();
            d.remove(']').trimmed();

            if (validValue) {
                if (index == 0) {
                    frontValue = d;
                } else if (index == 1) {
                    rearValue = d;
                } else {
                    qDebug() << "No valid value found :" << splitValue.size();
                    frontValue.clear();
                    rearValue.clear();
                    break;
                }
            } else {
                keywordData.append(d);
            }
            index++;
        }

        QStringList removeStr({"[", "]"});
        QStringList removeMaxValue({
            QString("%1").arg(static_cast<quint64>(UINT32_MAX) + 1),
            QString("%1.0").arg(static_cast<quint64>(UINT32_MAX) + 1),
        });

        if (validValue) {
            int frontValueSize = frontValue.split(",", Qt::SkipEmptyParts).size();
            int rearValueSize = rearValue.split(",", Qt::SkipEmptyParts).size();
            bool append = ((frontValueSize > 0) && (frontValueSize == rearValueSize));

            switch (keywordType) {
                case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomOver):
                case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomUnder):
                case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomMoreThanEqual):
                case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomLessThanEqual): {
                    if (append) {
                        keywordData.append(frontValue);
                    }
                    break;
                }
                case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomFlow):
                case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomTwoWay): {
                    if (append) {
                        keywordData.append(frontValue);
                        keywordData.append(rearValue);
                    }
                    break;
                }
                case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomRange): {
                    if (append) {
                        keywordData.append(frontValue);
                        keywordData.append(rearValue);
                        // removeStr.append(removeMaxValue);
                    }
                    break;
                }
                default: {
                    append = false;
                    break;
                }
            }
        } else {
            removeStr.append(removeMaxValue);
        }

        for (auto& data : keywordData) {
            ivis::common::getRemoved(data, removeStr);
        }

        for (const auto& data : keywordData) {
            if (data.contains(",")) {
                for (auto d : data.split(",", Qt::SkipEmptyParts)) {
                    dataInfo[keywordType].append(d.trimmed());
                }
            } else {
                dataInfo[keywordType].append(data);
            }
        }
        dataInfo[keywordType].removeAll("");
        dataInfo[keywordType].removeDuplicates();
        dataInfo[keywordType].sort();

#if 1
        qDebug() << "\t >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
        qDebug() << "\t CustomValue :" << validValue << values;
        qDebug() << "\t\t Joined      :" << joined;
        qDebug() << "\t\t SplitValue  :" << splitValue;
        qDebug() << "\t\t KeywordType :" << ExcelUtil::instance().data()->isKeywordString(keywordType) << keywordType;
        if (validValue) {
            qDebug() << "\t\t FrontValue  :" << frontValue;
            qDebug() << "\t\t RearValue   :" << rearValue;
        }
        qDebug() << "\t\t KeywordData :" << keywordData.size() << keywordData;
        qDebug() << "\t\t             :" << dataInfo[keywordType];
        qDebug() << "\t <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
#endif
    }

    return dataInfo;
}

QPair<int, int> SignalDataManager::isMinMaxValue(const QStringList& inputList, const bool& maxSkip) {
    qint64 minValue = 0;
    qint64 maxValue = 0;

    QList<qint64> numberList;
    QStringList removeMaxValue({
        QString("%1").arg(static_cast<quint64>(UINT32_MAX) + 1),
        QString("%1.0").arg(static_cast<quint64>(UINT32_MAX) + 1),
    });

    for (auto input : inputList) {
        if (maxSkip) {
            bool skip = false;
            for (auto removeValue : removeMaxValue) {
                if (input.compare(removeValue) == false) {
                    skip = true;
                    break;
                }
            }
            if (skip) {
                continue;
            }
        }
        bool ok = false;
        qint64 value = input.toLongLong(&ok);
        if (ok) {
            numberList.append(value);
        }
    }

    if (numberList.isEmpty() == false) {
        if (numberList.size() == 1) {
            qint64 value = numberList.first();
            minValue = (value < 0) ? (value) : (0);
            maxValue = (value < 0) ? (0) : (value);
        } else {
            // minValue = *std::min_element(numberList.begin(), numberList.end());
            // maxValue = *std::max_element(numberList.begin(), numberList.end());
            minValue = numberList.first();
            maxValue = numberList.first();
            for (const qint64& value : numberList) {
                minValue = qMin(minValue, value);
                maxValue = qMax(maxValue, value);
            }
        }
    }

    // qDebug() << "isMinMaxValue :" << minValue << maxValue;

    return qMakePair(minValue, maxValue);
}

QPair<QStringList, QStringList> SignalDataManager::isValidValueList(const bool& notTrigger, const QStringList& allData,
                                                                    const QMap<int, QStringList>& dataInfo) {
    if (dataInfo.size() != 1) {
        return QPair<QStringList, QStringList>();
    }
    const int keywordType = dataInfo.firstKey();
    const QStringList inputData = dataInfo[keywordType];
    const QPair<int, int> minMaxValue = isMinMaxValue(inputData, true);
    const qint64 minValue = minMaxValue.first;
    const qint64 maxValue = minMaxValue.second;

    auto toNumber = [](const QStringList& numberStrList) {
        QList<qint64> numberList;
        for (const auto& val : numberStrList) {
            bool ok = false;
            qint64 number = val.toLongLong(&ok);
            if (ok) {
                numberList.append(number);
            }
        }
        return numberList;
    };
    auto sortNumber = [](QList<qint64> numberList) {
        std::sort(numberList.begin(), numberList.end());  // 오름차순
        // std::sort(numberList.begin(), numberList.end(), std::greater<qint64>());  // 내림차순 정렬
        QStringList sortList;
        sortList.reserve(numberList.size());
        for (const auto& number : numberList) {
            sortList.append(QString::number(number));
        }
        return sortList;
    };
    auto isNormalValue = [&](const bool& toValid, const QStringList& allList, const QStringList& inputList) {
        QList<qint64> numberList = toNumber(inputList);
        if (toValid == false) {
            QList<qint64> tempList;
            for (const auto& number : toNumber(allList)) {
                if (numberList.contains(number) == false) {
                    tempList.append(number);
                }
            }
            numberList = tempList;
        }
        return sortNumber(numberList);
    };
    auto isRangeValue = [&](const bool& toValid, const QStringList& allList, const qint64& min, const qint64& max) {
        QList<qint64> numberList;
        for (const auto& number : toNumber(allList)) {
            bool inRange = ((number >= min) && (number <= max));
            if (((toValid) && (inRange)) || ((toValid == false) && (inRange == false))) {
                numberList.append(number);
            }
        }
        return sortNumber(numberList);
    };
    auto isOverlValue = [&](const bool& toValid, const QStringList& allList, const qint64& base) {
        QList<qint64> numberList;
        for (const auto& number : toNumber(allList)) {
            bool append = ((toValid) ? (number >= base) : (number < base));
            if (append) {
                numberList.append(number);
            }
        }
        return sortNumber(numberList);
    };
    auto isUnderlValue = [&](const bool& toValid, const QStringList& allList, const qint64& base) {
        QList<qint64> numberList;
        for (const auto& number : toNumber(allList)) {
            bool append = ((toValid) ? (number <= base) : (number > base));
            if (append) {
                numberList.append(number);
            }
        }
        return sortNumber(numberList);
    };

    QStringList invalidData;
    QStringList validData;

    switch (keywordType) {
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid): {
            validData = isNormalValue(true, allData, inputData);
            invalidData = isNormalValue(false, allData, inputData);
            break;
        }
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomOver): {
            validData = isOverlValue(true, allData, maxValue);
            invalidData = isOverlValue(false, allData, maxValue);
            break;
        }
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomMoreThanEqual): {
            validData = isOverlValue(true, allData, maxValue);
            invalidData = isOverlValue(false, allData, maxValue);
            break;
        }
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomUnder): {
            validData = isUnderlValue(true, allData, minValue);
            invalidData = isUnderlValue(false, allData, minValue);
            break;
        }
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomLessThanEqual): {
            validData = isUnderlValue(true, allData, minValue);
            invalidData = isUnderlValue(false, allData, minValue);
            break;
        }
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomFlow):
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomTwoWay):
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomRange): {
            validData = isRangeValue(true, allData, minValue, maxValue);
            invalidData = isRangeValue(false, allData, minValue, maxValue);
            break;
        }
        default: {
            break;
        }
    }

    QPair<QStringList, QStringList> valueInfo = qMakePair(validData, invalidData);

    qDebug() << "\n";
    qDebug() << "\tisValidValueList :" << notTrigger;
    qDebug() << "\t\t Keyword :" << keywordType << ExcelUtil::instance().data()->isKeywordString(keywordType);
    qDebug() << "\t\t MinMax  :" << minMaxValue.first << minMaxValue.second;
    qDebug() << "\t\t AllData :" << allData;
    qDebug() << "\t\t Value   :" << valueInfo.first;
    qDebug() << "\t\t         :" << valueInfo.second;
    qDebug() << "\n";

    return valueInfo;
}

QStringList SignalDataManager::isValidUniqueValue(const int& dataType, const QMap<int, QStringList>& dataInfo) {
    if (dataInfo.size() != 1) {
        return QStringList();
    }

    const int gapValue = 1;
    const int keywordType = dataInfo.firstKey();
    const QStringList inputList = dataInfo[keywordType];
    const QPair<int, int> minMaxValue = isMinMaxValue(inputList, true);
    const qint64 minValue = minMaxValue.first;
    const qint64 maxValue = minMaxValue.second;
    const QSet<QString> existing(inputList.begin(), inputList.end());

    bool result = true;
    qint64 validValue = 0;

    switch (keywordType) {
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid):
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomFlow):
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomTwoWay): {
            validValue = (maxValue + gapValue);
            break;
        }
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomOver):
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomMoreThanEqual): {
            validValue = (minValue - gapValue);
            if (dataType == static_cast<int>(ivis::common::DataTypeEnum::DataType::HUInt64)) {
                if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomMoreThanEqual)) {
                    result = (validValue >= 0);  // 만족 하는 조건이 없으면 데이터 구성 안함
                } else {  // if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomOver)) {
                    validValue = (validValue >= 0) ? (validValue) : (0);
                }
            }
            break;
        }
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomUnder):
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomLessThanEqual): {
            validValue = (maxValue + gapValue);
            break;
        }
        case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomRange): {
            validValue = (maxValue + gapValue);
            break;
        }
        default: {
            result = (dataType == static_cast<int>(ivis::common::DataTypeEnum::DataType::HString));
            break;
        }
    }

    QStringList validData;
    if (result) {
        if (dataType == static_cast<int>(ivis::common::DataTypeEnum::DataType::HString)) {
            validData.append("[NotUsedString]");
        } else if (dataType == static_cast<int>(ivis::common::DataTypeEnum::DataType::HDouble)) {
            validData.append(QString("%1.0").arg(validValue));
        } else {
            validData.append(QString::number(validValue));
        }
    } else {
        qDebug() << "Fail to - Data cleared as no conditions were met.";
        qDebug() << "\t DataType    : " << dataType;
        qDebug() << "\t KeywordType : " << keywordType;
        qDebug() << "\t Min         : " << minValue;
        qDebug() << "\t Max         : " << maxValue;
    }

#if 0
    qDebug() << "\t >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    for (const auto& key : dataInfo.keys()) {
        qDebug() << "\t KeywordInfo[" << ExcelUtil::instance().data()->isKeywordString(key) << "] :" << dataInfo[key];
    }
    qDebug() << "\t isValidUniqueValue :" << result;
    qDebug() << "\t\t DataType    : " << dataType;
    qDebug() << "\t\t KeywordType : " << keywordType;
    qDebug() << "\t\t InputData   : " << inputList.size() << inputList;
    qDebug() << "\t\t ValueList   : " << valueList.size() << valueList;
    qDebug() << "\t\t Min         : " << minValue;
    qDebug() << "\t\t Max         : " << maxValue;
    qDebug() << "\t\t ValidData   : " << validData;
    qDebug() << "\t <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
#endif

    return validData;
}

QStringList SignalDataManager::isMultiValidUniqueValue(const int& dataType, const QMap<int, QStringList>& dataInfo) {
    const int maxTryCount = 1000;
    QStringList validData;

    if (dataInfo.isEmpty()) {
        return validData;
    }

    bool isUnsigned = (dataType == static_cast<int>(ivis::common::DataTypeEnum::DataType::HUInt64));
    bool isSigned = (dataType == static_cast<int>(ivis::common::DataTypeEnum::DataType::HInt64));

    if ((isUnsigned == false) && (isSigned == false)) {
        // qDebug() << "Error - isMultiValidUniqueValue : 1";
        return validData;
    }

    // 모든 값 모으기
    QSet<QString> allExisting;
    QList<qint64> allIntValues;
    bool hasNegativeInput = false;

    for (const auto& list : dataInfo.values()) {
        allExisting.unite(QSet<QString>(list.begin(), list.end()));

        for (const QString& s : list) {
            bool ok = false;
            qint64 val = s.toLongLong(&ok);
            if (ok) {
                allIntValues.append(val);
                if (val < 0)
                    hasNegativeInput = true;
            }
        }
    }

    if (allIntValues.isEmpty()) {
        // qDebug() << "Error - isMultiValidUniqueValue : 2";
        return validData;
    }

    qint64 minVal = *std::min_element(allIntValues.begin(), allIntValues.end());
    qint64 maxVal = *std::max_element(allIntValues.begin(), allIntValues.end());

    // 생성 범위 계산
    qint64 minRange = minVal - 1000;
    qint64 maxRange = maxVal + 1000;

    // unsigned 타입이면 반드시 양수만
    if ((isUnsigned) || (isSigned && (hasNegativeInput == false))) {
        minRange = std::max(minRange, static_cast<qint64>(0));
    }

    for (int index = 0; index < maxTryCount; ++index) {
        qint64 candidateInt = QRandomGenerator::global()->bounded(minRange, maxRange + 1);
        QString candidateStr = QString::number(candidateInt);
        if (allExisting.contains(candidateStr)) {
            continue;
        }

        bool isValid = true;

        for (auto it = dataInfo.constBegin(); it != dataInfo.constEnd(); ++it) {
            int keywordType = it.key();
            QStringList refList = it.value();

            QList<qint64> refVals;
            for (const QString& s : refList) {
                bool ok;
                qint64 val = s.toLongLong(&ok);
                if (ok)
                    refVals.append(val);
            }

            if (refVals.isEmpty()) {
                isValid = false;
                break;
            }

            qint64 refMin = *std::min_element(refVals.begin(), refVals.end());
            qint64 refMax = *std::max_element(refVals.begin(), refVals.end());

            if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomOver) ||
                keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomMoreThanEqual)) {
                if (!(candidateInt < refMin)) {
                    isValid = false;
                    break;
                }
            } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomUnder) ||
                       keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomLessThanEqual)) {
                if (!(candidateInt > refMax)) {
                    isValid = false;
                    break;
                }
            } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomFlow) ||
                       keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomTwoWay)) {
                if (refVals.contains(candidateInt)) {
                    isValid = false;
                    break;
                }
            } else if (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomRange)) {
                if (candidateInt >= refMin && candidateInt <= refMax) {
                    isValid = false;
                    break;
                }
            }
        }

        if (isValid) {
            validData.append(candidateStr);
            break;
        }
    }

#if 0
    qDebug() << "\t >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    qDebug() << "\t [isMultiValidUniqueValue]";
    qDebug() << "\t\t DataType          :" << dataType;
    qDebug() << "\t\t HasNegativeInput  :" << hasNegativeInput;
    qDebug() << "\t\t Min/MaxRange      :" << minRange << " ~ " << maxRange;
    qDebug() << "\t\t InputData         :" << dataInfo;
    qDebug() << "\t\t OutputData        :" << validData;
    qDebug() << "\t <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
#endif

    return validData;
}

QPair<QStringList, QStringList> SignalDataManager::isConvertedValue(const QString& signalName, const int& dataType,
                                                                    const QStringList& originData,
                                                                    const QStringList& otherAllData) {
    if (signalName.trimmed().startsWith(SFC_IGN_ELAPSED)) {
        // signalName = "SFC.Private.IGNElapsed.Elapsed"
        // qDebug() << "\t isConvertedSignalDataValue - Skip Signal :" << signalName;
        return QPair<QStringList, QStringList>();
    }

    const QList<int> checkKeyword = {
        static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Timeout),
        static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Crc),
    };
    if (originData.size() == 1) {
        // originData = "MESSAGE_TIMEOUT" or "CRC_ERROR"
        for (const auto& keyword : checkKeyword) {
            if (originData.contains(ExcelUtil::instance().data()->isKeywordString(keyword))) {
                return QPair<QStringList, QStringList>();
            }
        }
    }

    QList<int> removeKeyword = {
        static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotTrigger),
        static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotTriggerSheet),
        // static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::DontCare),
    };
    removeKeyword.append(checkKeyword);
    QStringList currOriginData = originData;
    bool customNotTrigger = false;
    for (auto& d : currOriginData) {
        for (const auto& keyword : removeKeyword) {
            QString keywordStr = ExcelUtil::instance().data()->isKeywordString(keyword);
            if ((customNotTrigger == false) && (d.contains(keywordStr))) {
                customNotTrigger = true;
            }
            d.remove(keywordStr);
        }
    }

    const bool normal = (otherAllData.size() == 0);
    QMap<int, QStringList> dataInfo = isCustomValueInfo(currOriginData, (normal) ? (false) : (true));
    QPair<QStringList, QStringList> valueInfo;

#if 1
    if (normal == false) {
        qDebug() << "*************************************************************************************************";
        qDebug() << "isConvertedValue :" << signalName;
        qDebug() << "\t DataType     :" << dataType;
        qDebug() << "\t NotTrigger   :" << customNotTrigger;
        qDebug() << "\t OriginData   :" << originData.size() << originData;
        qDebug() << "\t CurrOrigin   :" << currOriginData.size() <<currOriginData;
        qDebug() << "\t OtherAllData :" << otherAllData;
        qDebug() << "\n";
    }
#endif

    if (normal) {
        if (dataInfo.size() > 1) {  // Keyword Group : Multi
            valueInfo = qMakePair(isMultiValidUniqueValue(dataType, dataInfo), QStringList());
        } else {  // Keyword Group : Normal, Single
            valueInfo = qMakePair(isValidUniqueValue(dataType, dataInfo), QStringList());
        }
    } else {
        valueInfo = isValidValueList(customNotTrigger, otherAllData, dataInfo);
    }

#if 1
    if (normal == false) {
        qDebug() << "\n";
        qDebug() << "\t ValueInfo    :" << valueInfo.first;
        qDebug() << "\t              :" << valueInfo.second;
        qDebug() << "*************************************************************************************************\n\n";
    }
#endif

    return valueInfo;
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

#if 0
    if (signalName == "Vehicle.CV.ADAS_Driving_CV.Input_MvLeftLineOffsetValue") {
        qDebug() << "========================================================================================";
        qDebug() << "[isConvertedExceptionData] : " << signalName;
        qDebug() << "\t FoundIndex    :" << foundIndex;
        qDebug() << "\t CheckData     :" << checkDataList;
        for (const auto& key : dataInfo.keys()) {
            qDebug() << "\t DataInfo[" << key << "] :" << dataInfo[key];
        }
    }
#endif

    if (foundIndex < 0) {
        // ConvertData 에 [MESSAGE_TIMEOUT, CRC_ERROR] 존재하지 않음
        return;
    }

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
            checkDataList.removeAll(checkStrMain);  // ConvertData : [MESSAGE_TIMEOUT, CRC_ERROR] 삭제
            // qDebug() << "1. Exception - Remove :" << tempDataList << "->" << checkDataList;
        } else {
            checkDataList[foundIndex] = checkStrMain;  // ConvertData : [MESSAGE_TIMEOUT, CRC_ERROR] 유지
            // qDebug() << "2. Exception - Keep   :" << tempDataList << "->" << checkDataList;
        }
    } else {
        checkDataList[foundIndex] = changeStr;  // ConvertData : [MESSAGE_TIMEOUT, CRC_ERROR] 변환 [timeout, crc]
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
    isConvertedSignalDataValueEnum(true, signalName, valueEnum, matchingEnum);
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
            notUsedEnum = isConvertedSignalDataValueEnum(true, signalName, valueEnum, matchingValue);

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

QMap<int, QPair<QString, SignalData>> SignalDataManager::isNormalInputSignalDataInfo(const int& sheetIndex,
                                                                                  const QPair<QStringList, QStringList>& list) {
    QStringList signalList = list.first;
    QStringList dataList = list.second;
    signalList.removeAll("");
    dataList.removeAll("");

    // for (int index = 0; index < signalList.size(); ++index) {
    //     for (const auto& d : dataList) {
    //         if (d.contains("[CustomNotTrigger]")) {
    //             qDebug() << index << ". " << signalList.at(index) << ":" << dataList.at(index);
    //             break;
    //         }
    //     }
    // }

    QMap<QString, QMap<int, QStringList>> dataInfo;
    QMap<QString, SignalData> currentSignalDataInfo = isSignalDataInfo(signalList, dataList, dataInfo);
    QMap<QString, SignalData> newSignalDataInfo;

    for (const auto& signalName : currentSignalDataInfo.keys()) {
        bool ignState = ivis::common::isContainsString(signalName, SFC_IGN_ELAPSED);

        if ((ignState) && (ivis::common::isCompareString(signalName, SFC_IGN_ELAPSED) == false)) {
            // SignalName : SFC_IGN_ELAPSED* 인 경우 예외 처리
            // qDebug() << "\t Skip IgnElapsed Signal :" << signalName;

            // signalList : SFC_IGN_ELAPSED 이걸로 오던지
            // signalList : SFC.Private.IGNElapsed.ElapsedOn*, SFC.Private.IGNElapsed.ElapsedOff* 오는지 확인해서
            // signalList : On 으로만 구성 되어 있으면 notUsedEnum = Off0ms 를 데이터로 사용
            // signalList : Off 으로만 구성 되어 있으면 notUsedEnum = On0ms 를 데이터로 사용
            // signalList : On, Off 중복으로 있으면 notUsedEnum = 공백("") 를 데이터로 사용
            // 이 경우 SignalName -> SFC_IGN_ELAPSED

            continue;
        }

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

        if ((ignState == false) && (valueEnum.size() == 0) && (notUsedEnum.size() == 0)) {
            auto valueInfo = isConvertedValue(signalName, dataType, originData, QStringList());
            notUsedEnum = valueInfo.first;
        }

        newSignalDataInfo[signalName] = SignalData(signalName, dataType, init, keywordType, originData, convertData, valueEnum,
                                                   notUsedEnum, precondition, allConvertData);
    }
    QMap<int, QPair<QString, SignalData>> signalDataInfo = isSortingInputSignalList(newSignalDataInfo, signalList);

#if 0
    qDebug() << "*************************************************************************************************";
    qDebug() << "1. isNormalInputSignalDataInfo :" << sheetIndex << signalList.size() << dataList.size()
             << signalDataInfo.size();
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

QMap<int, QPair<QString, SignalData>> SignalDataManager::isTestCaseInputSignalDataInfo(const int& sheetIndex,
                                                                                const QPair<QStringList, QStringList>& allList,
                                                                                const QPair<QStringList, QStringList>& list,
                                                                                QMap<QString, SignalData>& newSignalDataInfo) {
    QStringList signalList = list.first;
    QStringList dataList = list.second;
    signalList.removeAll("");
    dataList.removeAll("");

    qDebug() << "*************************************************************************************************";
    qDebug() << "*************************************************************************************************";
    qDebug() << "*************************************************************************************************";
    // for (int index = 0; index < signalList.size(); ++index) {
    //     qDebug() << index << ". Case  :" << signalList.at(index) << ":" << dataList.at(index);
    // }
    // qDebug() << "-------------------------------------------------------------------------------------------------";
    // for (int index = 0; index < allList.first.size(); ++index) {
    //     qDebug() << index << ". Other :" << allList.first.at(index) << ":" << allList.second.at(index);
    // }
    //     qDebug() << "Size :" << signalList.size() << allList.first.size();
    // qDebug() << "-------------------------------------------------------------------------------------------------";

    QMap<QString, QMap<int, QStringList>> dataInfo;
    QMap<QString, SignalData> currentSignalDataInfo = isSignalDataInfo(signalList, dataList, dataInfo);
    bool log = false;
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

        bool ignState = ivis::common::isContainsString(signalName, SFC_IGN_ELAPSED);
        bool valueState = ((allConvertData.size() > 0) && (valueEnum.size() == 0));
        QString maxValueState = ExcelUtil::instance().data()->isPreconditionMaxValue(signalName, dataType, keywordType,
                                                                                     convertData, valueEnum);
        bool enumState = (valueEnum.size() > 0);

        // Signal, Data 처리 순서 보장 필요 : ignState -> valueState -> maxValueState -> enumState
        if (ignState) {
            QPair<int, int> ignInfo = ExcelUtil::instance().data()->isIGNElapsedType(signalName);
            signalName = SFC_IGN_ELAPSED;
            if (ignInfo.first >= static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms)) {
                convertData = QStringList({QString("%1").arg(ignInfo.first)});
                precondition = QStringList({QString("%1").arg(ignInfo.second)});
            }
            if ((keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotTrigger)) ||
                (keywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotTriggerSheet))) {
                convertData.clear();
                precondition.clear();
            }
#if 1
        } else if (valueState) {
            QStringList otherSignalList;
            QStringList otherDataList;
            for (int index = 0; index < allList.first.size(); ++index) {
                QString currentSignal(allList.first.at(index));
                if ((signalName != currentSignal) || (index >= allList.second.size())) {
                    continue;
                }
                otherSignalList.append(currentSignal);
                otherDataList.append({allList.second.at(index)});
            }

            QPair<QStringList, QStringList> otherList = qMakePair(otherSignalList, otherDataList);
            QMap<QString, SignalData> otherSignalDataInfo;
            isOtherInputSignalDataInfo(sheetIndex, otherList, otherSignalDataInfo);
            QStringList otherAllConvertData;

            if (otherSignalDataInfo.size() == 1) {
                QString key = otherSignalDataInfo.firstKey();
                QStringList otherAllConvertData = otherSignalDataInfo[key].getAllConvertData();
                auto valueInfo = isConvertedValue(signalName, dataType, originData, otherAllConvertData);

                qDebug() << "\t OriginData          :" << originData;
                qDebug() << "\t AllConvertData      :" << allConvertData;
                qDebug() << "\t OtherListSize       :" << otherList.first.size() << otherList.second.size();
                qDebug() << "\t OtherAllConvertData :" << otherAllConvertData;
                qDebug() << "\t ValueList           :" << valueInfo.first;
                qDebug() << "\t                     :" << valueInfo.second;
            } else {
                qDebug() << "Fail to other all convert data size :" << otherSignalDataInfo.size();
            }


        } else if (maxValueState.size() > 0) {
            precondition = QStringList({maxValueState});
        } else if (enumState) {
            const int customNotTriggerIndex = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotTriggerSheet);
            const QString customNotTriggerKeyword = ExcelUtil::instance().data()->isKeywordString(customNotTriggerIndex);
            QStringList originDataTemp = originData;

            for (auto& data : originDataTemp) {
                if (data.contains(customNotTriggerKeyword)) {
                    keywordType = customNotTriggerIndex;
                    data.remove(customNotTriggerKeyword);
                    break;
                }
            }

            switch (keywordType) {
                case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotUsed): {
                    convertData.clear();
                    notUsedEnum.clear();
                    precondition.clear();
                    break;
                }
                case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotTrigger): {
                    precondition = convertData;
                    convertData.clear();
                    notUsedEnum.clear();
                    break;
                }
                case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotTriggerSheet): {
                    // Value 인 경우에 대한 처리만 동작함
                    // Enum 인 경우 처리 확인 필요
                    int dataKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid);
                    QMap<int, QStringList> dataInfo = isCustomValueInfo(originDataTemp, true);

                    if (dataInfo.size() == 1) {
                        dataKeywordType = dataInfo.firstKey();
                    }
                    if (dataKeywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid)) {
                        precondition = convertData;
                    } else {
                        precondition = dataInfo[dataKeywordType];
                    }
                    convertData.clear();
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
                    precondition = notUsedEnum;
                    notUsedEnum.clear();
                    break;
                }
            }
            keywordType = ExcelUtil::instance().data()->isConvertedKeywordType(false, keywordType);

            isConvertedExceptionData(signalName, dataInfo[signalName], convertData);
            isConvertedExceptionData(signalName, dataInfo[signalName], precondition);
#else
        } else {
            if (maxValueState.size() > 0) {
                precondition = QStringList({maxValueState});
            } else {
                const int customNotTriggerIndex =
                    static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotTriggerSheet);
                const QString customNotTriggerKeyword = ExcelUtil::instance().data()->isKeywordString(customNotTriggerIndex);
                QStringList originDataTemp = originData;

                for (auto& data : originDataTemp) {
                    if (data.contains(customNotTriggerKeyword)) {
                        keywordType = customNotTriggerIndex;
                        data.remove(customNotTriggerKeyword);
                        break;
                    }
                }

                switch (keywordType) {
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotUsed): {
                        convertData.clear();
                        notUsedEnum.clear();
                        precondition.clear();
                        break;
                    }
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotTrigger):
                    case static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomNotTriggerSheet): {
                        int dataKeywordType = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid);
                        QMap<int, QStringList> dataInfo = isCustomValueInfo(originDataTemp, true);

                        if (dataInfo.size() == 1) {
                            dataKeywordType = dataInfo.firstKey();
                        }

                        if (dataKeywordType == static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::Invalid)) {
                            precondition = convertData;
                        } else {
                            precondition = dataInfo[dataKeywordType];
                        }
                        convertData.clear();
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
                        precondition = notUsedEnum;
                        notUsedEnum.clear();
                        break;
                    }
                }
                keywordType = ExcelUtil::instance().data()->isConvertedKeywordType(false, keywordType);
            }
            isConvertedExceptionData(signalName, dataInfo[signalName], convertData);
            isConvertedExceptionData(signalName, dataInfo[signalName], precondition);
#endif
        }

        convertData.removeDuplicates();
        precondition.removeDuplicates();

        newSignalDataInfo[signalName] = SignalData(signalName, dataType, init, keywordType, originData, convertData, valueEnum,
                                                   notUsedEnum, precondition, allConvertData);
    }
    QMap<int, QPair<QString, SignalData>> signalDataInfo = isSortingInputSignalList(newSignalDataInfo, signalList);

#if 1
    if (log) {
    qDebug() << "*************************************************************************************************";
    qDebug() << "2. isTestCaseInputSignalDataInfo :" << sheetIndex << signalList.size() << dataList.size()
             << signalDataInfo.size();
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
    }
#endif

    return signalDataInfo;
}

QMap<int, QPair<QString, SignalData>> SignalDataManager::isOtherInputSignalDataInfo(const int& sheetIndex,
                                                                                const QPair<QStringList, QStringList>& list,
                                                                                QMap<QString, SignalData>& newSignalDataInfo) {
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

        bool ignState = ivis::common::isContainsString(signalName, SFC_IGN_ELAPSED);
        bool valueState = ((allConvertData.size() > 0) && (valueEnum.size() == 0));
        QString maxValueState = ExcelUtil::instance().data()->isPreconditionMaxValue(signalName, dataType, keywordType,
                                                                                     convertData, valueEnum);
        bool enumState = (valueEnum.size() > 0);

        // Signal, Data 처리 순서 보장 필요 : ignState -> valueState -> maxValueState -> enumState
        if (ignState) {
            QPair<int, int> ignInfo = ExcelUtil::instance().data()->isIGNElapsedType(signalName);
            if (ignInfo.first >= static_cast<int>(ivis::common::IGNElapsedTypeEnum::IGNElapsedType::ElapsedOn0ms)) {
                ignOriginData.append(QString("%1").arg(ignInfo.first));
            }
            continue;
        } else if (valueState) {
            // 이슈 : value 로 구성된 데이터에서 other 정보 구성시 convertData = maxValueState 만 구성 되는 문제 수정
            // valueState, maxValueState 2가지 조건중 valueState 조건을 먼저 처리해야함
            // valueEnum.size() == 0  때문에 maxValueState 조건이 처리 되어서 이슈 발생
            convertData = allConvertData;
            keywordType = ExcelUtil::instance().data()->isConvertedKeywordType(false, keywordType);
            notUsedEnum.clear();
            precondition.clear();
        } else if (maxValueState.size() > 0) {
            precondition = QStringList({maxValueState});
        } else if (enumState) {
            const int configIndex = static_cast<int>(ivis::common::KeywordTypeEnum::KeywordType::CustomConfig);
            const QString configKeyword = ExcelUtil::instance().data()->isKeywordString(configIndex);
            // 원본 입력 시그널, 데이터에서 [CustomConfig] 가 있었으면 변경되어 나온 데이터 타입을 Config 로 변경
            // Data : [CustomNotTrigger]OFF -> [CustomConfig]OFF => 이건 변경 할지 결정 필요
            for (int index = 0; index < signalList.size(); ++index) {
                if (signalName == signalList.at(index)) {
                    QString data = dataList.at(index);
                    if (data.contains(configKeyword) == false) {
                        continue;
                    }
                    keywordType = configIndex;
                    // qDebug() << "[" << signalName << "] Data continas :" << configKeyword;
                    break;
                }
            }

            QString tempMatchingValue;  // not used
            convertData = isConvertedSignalDataValueEnum(true, signalName, valueEnum, tempMatchingValue);
            isConvertedExceptionData(signalName, dataInfo[signalName], convertData);

            keywordType = ExcelUtil::instance().data()->isConvertedKeywordType(false, keywordType);
            notUsedEnum.clear();
            precondition.clear();
        } else {
            keywordType = ExcelUtil::instance().data()->isConvertedKeywordType(false, keywordType);
            notUsedEnum.clear();
            precondition.clear();
        }

        convertData.removeDuplicates();
        precondition.removeDuplicates();

        newSignalDataInfo[signalName] = SignalData(signalName, dataType, init, keywordType, originData, convertData, valueEnum,
                                                   notUsedEnum, precondition, allConvertData);
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
    qDebug() << "3. isOtherInputSignalDataInfo :" << sheetIndex << signalList.size() << dataList.size()
             << signalDataInfo.size();
    qDebug() << "=================================================================================================";
    for (const auto& mapKey : signalDataInfo.keys()) {
        auto signalName = signalDataInfo[mapKey].first;
        auto signalData = signalDataInfo[mapKey].second;
        // for (const auto& key : newSignalDataInfo.keys()) {
        //     auto signalName = key;
        //     auto signalData = newSignalDataInfo[key];
        // }
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

QMap<int, QPair<QString, SignalData>> SignalDataManager::isOutputSignalDataInfo(const int& sheetIndex,
                                                                                const QList<QStringList>& list) {
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
        QStringList allConvertData = signalData.getAllConvertData();

        newSignalDataInfo[signalName] = SignalData(signalName, dataType, init, keywordType, originData, convertData, valueEnum,
                                                   notUsedEnum, precondition, allConvertData);
    }

    QMap<int, QPair<QString, SignalData>> signalDataInfo;
    for (const auto& signalName : signalList) {
        signalDataInfo[signalDataInfo.size()] = qMakePair(signalName, newSignalDataInfo[signalName]);
    }

#if 0
    qDebug() << "*************************************************************************************************";
    qDebug() << "4. isOutputSignalDataInfo :" << sheetIndex << signalList.size() << dataList.size()
             << signalDataInfo.size();
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

QMap<int, QPair<QString, SignalData>> SignalDataManager::isDependSignalDataInfo(const int& sheetIndex,
                                                                                const QList<QStringList>& list) {
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
    qDebug() << "5. isDependSignalDataInfo :" << sheetIndex << signalList.size() << dataList.size()
             << signalDataInfo.size();
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
    QRegularExpression signalRegex(R"(^\s*-\s*([^:]+):\s*(.*)$)");   // 시작 : "- "    끝 ":" 확인인
    QRegularExpression signalNameRegex(R"(signalName:)");            // "signalName:" 포함 확인
    QRegularExpression abstractionNameRegex(R"(abstractionName:)");  // " :" 포함 확인

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

void SignalDataManager::testCode(const QVariantList& arg) {
    if (arg.size() == 0) {
        const static QMap<int, QStringList> customData = {
            {0, QStringList({"[CustomOver][0]", "[1]"})},                                       // >0    ->  0 이하(HInt64), 공백
            {1, QStringList({"[CustomMoreThanEqual][-1]", "[0]"})},                             // >=0   -> -1 이하(HInt64), 공백
            {2, QStringList({"[CustomOver][10]", "[11]"})},                                     // >10   ->  9 이하 (min 이하)
            {3, QStringList({"[CustomMoreThanEqual][9]", "[10]"})},                             // >=10  ->  9 이하 (min 이하)
            {4, QStringList({"[CustomUnder][0]", "[-1]"})},                                     // <0    ->  0 이상 (max 이상)
            {5, QStringList({"[CustomLessThanEqual][1]", "[0]"})},                              // <=0   ->  0 이상 (max 이상)
            {6, QStringList({"[CustomUnder][10]", "[9]"})},                                     // <10   -> 10 이상 (max 이상)
            {7, QStringList({"[CustomLessThanEqual][11]", "[10]"})},                            // <=10  -> 10 이상 (max 이상)
            {8, QStringList({"[CustomFlow][1]", "[2]"})},                                       // 1=>2  ->  3 이상 (max 이상)
            {9, QStringList({"[CustomFlow][1, 2]", "[2, 1]"})},                                 // 1<=>2 ->  3 이상 (max 이상)
            {10, QStringList({"[CustomRange][4294967296, 4294967296, 4294967296, 4294967296]",  // 1~10  -> 11 이상 (max 이상)
                            "[1, 2, 9, 10]"})},
            {11, QStringList({"[CustomTwoWay][1, 1, 1, 1, 2, 4, 8, 10]",
                            "[2, 4, 8, 10, 1, 1, 1, 1]"})},
            {12, QStringList({"0", "[CustomRange][4294967296]",
                            "[1]", "[2]", "[44]", "[45]", "83"})},
            {13, QStringList({"[CustomLessThanEqual][91]", "[90]", "91", "92", "100"})},
            {14, QStringList({"[CustomTwoWay][1, 1, 1, 1, 2, 4, 8, 10]",
                            "[2, 4, 8, 10, 1, 1, 1, 1]"})},
            {15, QStringList({"[CustomMoreThanEqual][149]", "[150]",
                            "[CustomLessThanEqual][151]", "[150]"})},
            {16, QStringList({"-200", "300", "100"})},
            {17, QStringList({"200", "300", "100"})},
            {18, QStringList({"[CustomOver][1]", "[2]"})},                                      // >1    -> 1 이하(HInt64), 공백
            {19, QStringList({"[CustomMoreThanEqual][149]", "[150]",
                            "[CustomLessThanEqual][151]", "[150]",
                            "[CustomRange][4294967296, 4294967296, 4294967296, 4294967296]",
                            "[1, 2, 9, 10]"})},
            {20, QStringList({"[CustomLessThanEqual][151]", "[150]",
                            "[CustomRange][4294967296, 4294967296, 4294967296, 4294967296]",
                            "[1, 2, 9, 10]"})},
            {21, QStringList({"[CustomNotTrigger][CustomOver][100]", "[101]"})},
            {22, QStringList({"[CustomNotTrigger][CustomOver][100][101]"})},
        };
        static int size = customData.size();
        static int index = 0;
        int dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::HUInt64);
        // int dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::HInt64);
        // int dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::HDouble);
        isConvertedValue(QString("Signal.Test"), dataType, customData[index], QStringList());
        ivis::common::REVOLVE_P(index, 1, 0, size);
    }
}
