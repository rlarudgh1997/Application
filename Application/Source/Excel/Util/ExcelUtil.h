#ifndef EXCEL_UTIL_H
#define EXCEL_UTIL_H

#include <QObject>
#include <QSharedPointer>
#include <QJsonObject>

#include "CommonDefine.h"
#include "CommonUtil.h"

using QMapStrStr = QMap<QString, QString>;

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

class ExcelUtil : public QObject {
    Q_OBJECT

    REGISTER_WRITABLE_VALUE(QString, MergeStart, QString())
    REGISTER_WRITABLE_VALUE(QString, Merge, QString())
    REGISTER_WRITABLE_VALUE(QString, MergeEnd, QString())
    REGISTER_WRITABLE_VALUE(QStringList, MergeInfos, QStringList())
    REGISTER_WRITABLE_VALUE(bool, InitPreset, false)
    REGISTER_WRITABLE_VALUE(QJsonObject, JsonRoot, QJsonObject())
    REGISTER_WRITABLE_VALUE(QStringList, JsonSectionInfo, QStringList())
    REGISTER_WRITABLE_CONTAINER(QMap, QString, QMapStrStr, PresetInfo)

public:
    static QSharedPointer<ExcelUtil>& instance();

    QString isModuleFilePath(const QString& path, const QString& module, const QString& fileExtension);
    QMap<QString, QPair<QString, QString>> isModuleListFromJson(const int& appMode, const bool& yml = false,
                                                                const bool& toUpper = false);
    QStringList isDescriptionDataInfo();
    int isDescriptionValueCount();
    int isDescriptionPreconditionCount();
    QString isCurrentCellText(const int& sheetIndex, const int& rowIndex, const int& columnIndex);
    QList<QPair<QString, int>> isKeywordPatternInfo(const int& columnIndex);
    QString isKeywordString(const int keywordType);
    int isKeywordType(const int& columnIndex, QString& inputData);
    // QList<KeywordInfo> isKeywordTypeInfo(const QVariantList& sheetData, const QList<int>& inputColumnList);
    int isDataType(const QString& dataTypeStr);
    QPair<int, int> isIGNElapsedType(const QString& singalName);
    QString isIGNElapsedName(const int& ignType);
    QPair<QStringList, QStringList> isConvertedIGNElapsedInfo(const QStringList& ignOriginData);
    int isConvertedKeywordType(const bool& toCustom, const int& keywordType);
    QString isMaxValue(const bool& doubleType);
    QString isValidMaxValue(const QString& signalName, const int& dataType, const int& keywordType, const QStringList& inputData,
                       const QStringList& valueEnum);
    // bool isExistsExcelSheet();
    bool isCheckPythonLibrary();
    QString pythonCall(const bool& readFile, const QString& filePath);
    void writeExcelSheet(const QString& filePath, const bool& convert);
    QList<QVariantList> openExcelFile(const QString& filePath);
    QString readPreset(const QString& key);

private:
    explicit ExcelUtil();

    void initPresetInfo();
};

#endif  // EXCEL_UTIL_H
