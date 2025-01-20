#ifndef EXCEL_DATA_H
#define EXCEL_DATA_H

#include <QObject>
#include <QSharedPointer>

#include "CommonDefine.h"
#include "CommonUtil.h"

class ExcelData : public QObject {
    Q_OBJECT

    REGISTER_WRITABLE_PROPERTY(QString, MergeStart, QString(), false)
    REGISTER_WRITABLE_PROPERTY(QString, Merge, QString(), false)
    REGISTER_WRITABLE_PROPERTY(QString, MergeEnd, QString(), false)
    REGISTER_WRITABLE_PROPERTY(QStringList, MergeInfos, QStringList(), false)
    REGISTER_WRITABLE_PROPERTY(bool, ReadStateNewData, true, false)
    REGISTER_WRITABLE_PROPERTY_CONTAINER(QMap, int, QVariant, SheetData, false)

public:
    static QSharedPointer<ExcelData>& instance() {
        static QSharedPointer<ExcelData> gData;
        if (gData.isNull()) {
            gData = QSharedPointer<ExcelData>(new ExcelData());
        }
        return gData;
    }

private:
    explicit ExcelData() = default;
};

#endif  // EXCEL_DATA_H
