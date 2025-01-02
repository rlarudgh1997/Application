#ifndef EXCEL_UTIL_H
#define EXCEL_UTIL_H

#include <QObject>
#include <QSharedPointer>

#include "CommonDefine.h"
#include "CommonUtil.h"

class ExcelUtil : public QObject {
    Q_OBJECT

    REGISTER_WRITABLE_PROPERTY(QString, MergeStart, QString(), false)
    REGISTER_WRITABLE_PROPERTY(QString, Merge, QString(), false)
    REGISTER_WRITABLE_PROPERTY(QString, MergeEnd, QString(), false)
    REGISTER_WRITABLE_PROPERTY(QStringList, MergeInfos, QStringList(), false)

public:
    static QSharedPointer<ExcelUtil>& instance();

    QStringList isDescriptionDataInfo();
    QList<QPair<QString, int>> isKeywordPatternInfo(const int& columnIndex);
    QString isKeywordString(const int keywordType);
    int isDataType(const QString& dataTypeStr);
    QPair<int, int> isIGNElapsedType(const QString& singalName);
    QString isIGNElapsedName(const int& ignType);
    QPair<QStringList, QStringList> isConvertedIGNElapsedInfo(const QStringList& ignOriginData);
    int isConvertedKeywordType(const bool& toCustom, const int& keywordType);

private:
    explicit ExcelUtil();
};


#endif  // EXCEL_UTIL_H
