#ifndef EXCEL_DATA_H
#define EXCEL_DATA_H

#include <QObject>
#include <QSharedPointer>

#include "CommonDefine.h"
#include "CommonUtil.h"

class ExcelData : public QObject {
    Q_OBJECT

    // REGISTER_WRITABLE_VALUE(QString, MergeStart, QString())
    // REGISTER_WRITABLE_VALUE(QString, Merge, QString())
    // REGISTER_WRITABLE_VALUE(QString, MergeEnd, QString())
    // REGISTER_WRITABLE_VALUE(QStringList, MergeInfos, QStringList())
    // REGISTER_WRITABLE_VALUE(bool, ReadStateNewData, true)
    REGISTER_WRITABLE_CONTAINER(QMap, int, QVariant, SheetData)
    REGISTER_WRITABLE_CONTAINER(QMap, int, QVariant, EditingSheetData)    // 엑셀 편집중인 경우 데이터 저장 -> TestCase 에서 사용

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
