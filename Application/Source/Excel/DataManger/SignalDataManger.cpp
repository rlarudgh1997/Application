#include "SignalDataManger.h"

#include "CommonEnum.h"
#include "ConfigSetting.h"

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

QMap<QString, SignalDataInfo> SignalDataManger::isMatchingSignalDataInfo(const int& dataInfoType, const int& sheetIndex,
                                                                         const QStringList& columnDataInfo) {
    if (columnDataInfo.size() != 3) {
        qDebug() << "Fail to column data info size :" << columnDataInfo.size();
        return QMap<QString, SignalDataInfo>();
    }
    QString tcNameInfo = columnDataInfo.at(0);
    QString resultInfo = columnDataInfo.at(1);
    QString caseInfo = columnDataInfo.at(2);

    QMap<QString, SignalDataInfo> signalDataInfo;


    return signalDataInfo;
}
