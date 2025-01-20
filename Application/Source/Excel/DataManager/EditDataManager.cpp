#include "EditDataManager.h"

#include "CommonEnum.h"
#include "ConfigSetting.h"
#include "ExcelUtil.h"

QSharedPointer<EditDataManager>& EditDataManager::instance() {
    static QSharedPointer<EditDataManager> gManager;
    if (gManager.isNull()) {
        gManager = QSharedPointer<EditDataManager>(new EditDataManager());
    }
    return gManager;
}

EditDataManager::EditDataManager() {
    const QString mergeStart = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeStart).toString();
    const QString merge = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMerge).toString();
    const QString mergeEnd = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeExcelMergeEnd).toString();
    const QStringList mergeInfos = QStringList({mergeStart, merge, mergeEnd});

    setMergeStart(mergeStart);
    setMerge(merge);
    setMergeEnd(mergeEnd);
    setMergeInfos(QStringList({mergeStart, merge, mergeEnd}));
}
