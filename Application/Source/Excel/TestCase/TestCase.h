#ifndef TEST_CASE_H
#define TEST_CASE_H

#include <QObject>

#include "CommonUtil.h"

class TestCase : public QObject {
    Q_OBJECT

    REGISTER_WRITABLE_VALUE(int, ExcuteType, 0)
    REGISTER_WRITABLE_VALUE(QStringList, Arguments, QStringList())
    REGISTER_WRITABLE_VALUE(int, SelectAppMode, 0)
    REGISTER_WRITABLE_VALUE(QStringList, SelectModules, QStringList())
    REGISTER_WRITABLE_CONTAINER(QMap, QString, QString, ModuleList)

public:
    enum {
        ExcuteTypeInvalid = 0,
        ExcuteTypeStart,
        ExcuteTypeParsingAppMode,
        ExcuteTypeParsingModule,
        ExcuteTypeExcelOpen,
        ExcuteTypeGenConvertData,
        ExcuteTypeGenTC,
        ExcuteTypeRunTC,

        ExcuteTypeCompleted = 100,
        ExcuteTypeFailed,

        ExcuteTypeInvalidSelectItem = 200,
        ExcuteTypeManualInput,
    };

public:
    static QSharedPointer<TestCase>& instance();

    void start(const QStringList& arguments = QStringList());

signals:
    void signalTestCaseCompleted(const int& type, const bool& result);

private:
    explicit TestCase();

    int excuteTestCase(const int& excuteType);
    void parsingOptions(const QStringList& arguments);
    QStringList parsingAppMode(const QStringList& arguments);
    QStringList parsingModules(const QStringList& arguments);
    bool parsingInputArguments(const int& excuteType);
    void terminateApplicaton();
    void drawTerminalMenu(const int& excuteType, const QStringList& itemList);
    QStringList selectMultipleOptionsWithNumbers(const int& excuteType, const QStringList& options);
    QStringList isModuleList();
    QList<QVariantList> isSheetData();
    void updateSheetData(const QList<QVariantList>& sheetDataList);
    bool openExcelFile();

private:
    const QString mStrExit = QString("Exit");
    const int mNumExit = 0;
    const int mNumSelectAll = 800;
    const int mNumManualInput = 900;
};

#endif  // TEST_CASE_H
