#ifndef EXCEL_UTIL_H
#define EXCEL_UTIL_H

#include <QObject>
#include <QSharedPointer>

#include "CommonDefine.h"
#include "CommonUtil.h"

#if !defined(USE_CODE_BEFORE_CLASS_SPLIT)
class KeywordInfo {
public:
    KeywordInfo(const int& row, const int& column, const QString& text, const int& keyword, const QString& data)
        : mRow(row), mColumn(column), mText(text), mKeyword(keyword), mData(data) {
    }
    KeywordInfo() = default;
    KeywordInfo(const KeywordInfo& other) = default;
    KeywordInfo& operator=(const KeywordInfo& other) = default;

    bool operator==(const KeywordInfo& other) const {
        return ((mRow == other.mRow) && (mColumn == other.mColumn) && (mText == other.mText) && (mKeyword == other.mKeyword) &&
                (mData == other.mData));
    }
    bool operator!=(const KeywordInfo& other) const {
        return !(*this == other);
    }

    int isRow() const {
        return mRow;
    }
    int isColumn() const {
        return mColumn;
    }
    QString isText() const {
        return mText;
    }
    int isKeyword() const {
        return mKeyword;
    }
    QString isData() const {
        return mData;
    }
    QList<QStringList> isRowData() const {
        return mRowData;
    }
    void updateRowData(const QList<QStringList>& rowData) {
        mRowData = rowData;
    }
    QList<QStringList> isConvertData() const {
        return mConvertData;
    }
    void updateConvertData(const QList<QStringList>& convertData) {
        mConvertData = convertData;
    }

private:
    int mRow = 0;
    int mColumn = 0;
    QString mText = QString();
    int mKeyword = 0;
    QString mData = QString();
    QList<QStringList> mConvertData = QList<QStringList>();
    QList<QStringList> mRowData = QList<QStringList>();
};
#endif

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
    int isKeywordType(const int& columnIndex, QString& inputData);
#if !defined(USE_CODE_BEFORE_CLASS_SPLIT)
    QList<KeywordInfo> isKeywordTypeInfo(const QVariantList& sheetData, const QList<int>& inputColumnList);
#endif
    int isDataType(const QString& dataTypeStr);
    QPair<int, int> isIGNElapsedType(const QString& singalName);
    QString isIGNElapsedName(const int& ignType);
    QPair<QStringList, QStringList> isConvertedIGNElapsedInfo(const QStringList& ignOriginData);
    int isConvertedKeywordType(const bool& toCustom, const int& keywordType);
    QString isPreconditionMaxValue(const QString& signalName, const int& dataType, const int& keywordType,
                                   const QStringList& inputData, const QStringList& valueEnum);

private:
    explicit ExcelUtil();
};


#endif  // EXCEL_UTIL_H
