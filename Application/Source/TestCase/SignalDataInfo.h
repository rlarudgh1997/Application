#ifndef SIGNAL_DATA_INFO_H
#define SIGNAL_DATA_INFO_H

#include <QObject>

#include "CommonEnum.h"
#include "CommonUtil.h"

class SignalDataInfo : public QObject {
    Q_OBJECT

    REGISTER_WRITABLE_PROPERTY(QString, DataType, QString(), false)
    REGISTER_WRITABLE_PROPERTY(QStringList, ValueEnum, QStringList(), false)
    REGISTER_WRITABLE_PROPERTY(QStringList, MatchingTableICV, QStringList(), false)
    REGISTER_WRITABLE_PROPERTY(QStringList, MatchingTableEV, QStringList(), false)
    REGISTER_WRITABLE_PROPERTY(QStringList, MatchingTableFCEV, QStringList(), false)
    REGISTER_WRITABLE_PROPERTY(QStringList, MatchingTablePHEV, QStringList(), false)
    REGISTER_WRITABLE_PROPERTY(QStringList, MatchingTableHEV, QStringList(), false)
    REGISTER_WRITABLE_PROPERTY(QStringList, MatchingTableSystem, QStringList(), false)

public:
    SignalDataInfo() {
    }
    explicit SignalDataInfo(const QVariantList& dataInfo, const QString& dataType) {
        if (dataInfo.size() == (ivis::common::InputDataTypeEnum::InputDataTypeMax - 1)) {
            QMap<int, QStringList> info;
            info[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum] = dataInfo.at(0).toStringList();
            info[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableICV] = dataInfo.at(1).toStringList();
            info[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableEV] = dataInfo.at(2).toStringList();
            info[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableFCEV] = dataInfo.at(3).toStringList();
            info[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTablePHEV] = dataInfo.at(4).toStringList();
            info[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableHEV] = dataInfo.at(5).toStringList();
            info[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableSystem] = dataInfo.at(6).toStringList();
            updateSignalDataInfo(info, dataType);
        }
    }
    explicit SignalDataInfo(const QMap<int, QStringList>& dataInfo, const QString& dataType) {
        updateSignalDataInfo(dataInfo, dataType);
    }
    int isDataType() {
        int dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::Invalid);
        QString isDataType = getDataType();

        if (isDataType.compare("HUInt64") == false) {
            dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::HUInt64);
        } else if (isDataType.compare("HInt64") == false) {
            dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::HInt64);
        } else if (isDataType.compare("HString") == false) {
            dataType = static_cast<int>(ivis::common::DataTypeEnum::DataType::HString);
        } else {
        }

        return dataType;
    }

private:
    void updateSignalDataInfo(const QMap<int, QStringList>& dataInfo, const QString& dataType) {
        QMap<int, QStringList> info = dataInfo;

        if (info.size() > 0) {
            // DataType Update : dataType or ValueEnum[0]
            if (dataType.size() == 0) {
                auto valueEnum = info[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum];
                if (valueEnum.size() > 0) {
                    setDataType(valueEnum.at(0));
                }
            } else {
                setDataType(dataType);
            }

            // DataType Remove : MatchingTable*[0] -> Not Used
            for (int index = ivis::common::InputDataTypeEnum::InputDataTypeValueEnum;
                 index < ivis::common::InputDataTypeEnum::InputDataTypeMax; ++index) {
                if (info[index].size() > 0) {
                    info[index].removeAt(0);
                }
            }
        }
        setValueEnum(info[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum]);
        setMatchingTableICV(info[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableICV]);
        setMatchingTableEV(info[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableEV]);
        setMatchingTableFCEV(info[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableFCEV]);
        setMatchingTablePHEV(info[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTablePHEV]);
        setMatchingTableHEV(info[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableHEV]);
        setMatchingTableSystem(info[ivis::common::InputDataTypeEnum::InputDataTypeMatchingTableSystem]);
    }
};

#endif  // SIGNAL_DATA_INFO_H
