#ifndef EDIT_DATA_MANAGER_H
#define EDIT_DATA_MANAGER_H

#include <QObject>
#include <QSharedPointer>

#include "CommonDefine.h"
#include "CommonUtil.h"

class EditDataManager : public QObject {
    Q_OBJECT

    REGISTER_WRITABLE_PROPERTY(QString, MergeStart, QString(), false)
    REGISTER_WRITABLE_PROPERTY(QString, Merge, QString(), false)
    REGISTER_WRITABLE_PROPERTY(QString, MergeEnd, QString(), false)
    REGISTER_WRITABLE_PROPERTY(QStringList, MergeInfos, QStringList(), false)

public:
    static QSharedPointer<EditDataManager>& instance();

private:
    explicit EditDataManager();
};

#endif  // EDIT_DATA_MANAGER_H
