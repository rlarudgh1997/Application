#include "SignalDataManger.h"

#include "CommonEnum.h"
#include "ConfigSetting.h"
#include "ExcelUtil.h"

QSharedPointer<SignalDataManger>& SignalDataManger::instance() {
    static QSharedPointer<SignalDataManger> gManger;
    if (gManger.isNull()) {
        gManger = QSharedPointer<SignalDataManger>(new SignalDataManger());
    }
    return gManger;
}

SignalDataManger::SignalDataManger() {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});

    setMergeStart(mergeStart);
    setMerge(merge);
    setMergeEnd(mergeEnd);
    setMergeInfos(QStringList({mergeStart, merge, mergeEnd}));
}

QMap<QString, SignalData> SignalDataManger::isSignalDataInfo(const QPair<QStringList, QStringList>& list) {
    QMap<QString, SignalData> signalDataInfo;

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

    const int appMode = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAppMode).toInt();
    const int configVehicleType = (appMode == ivis::common::AppModeEnum::AppModeTypePV) ? (ConfigInfo::ConfigTypeVehicleTypePV)
                                                                                       : (ConfigInfo::ConfigTypeVehicleTypeCV);
    const QStringList vehicleList = ConfigSetting::instance().data()->readConfig(configVehicleType).toStringList();
    const QString vehicleType = vehicleList.join(", ");

    const QStringList singals = list.first;
    const QStringList datas = list.second;


    qDebug() << "isSignalDataInfo :" << singals.size() << datas.size();
    qDebug() << "\t Info :" << vehicleType;
    qDebug() << "\t Info :" << keywordString;
    int index = 0;
    for (const auto& singal : singals) {
        qDebug() << "\t Signal[" << index++ << "] :" << singal;
    }
    index = 0;
    for (const auto& data : datas) {
        qDebug() << "\t Data[" << index++ << "]   :" << data;
    }




    return signalDataInfo;
}
