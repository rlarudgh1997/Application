#ifndef TEST_CASE_H
#define TEST_CASE_H

#include <QObject>

#include "CommonUtil.h"

class TestCase : public QObject {
    Q_OBJECT

    REGISTER_WRITABLE_VALUE(int, ExcuteType, 0)
    REGISTER_WRITABLE_VALUE(QStringList, ExcuteData, QStringList())
    REGISTER_WRITABLE_VALUE(QString, SelectAppMode, QString())
    REGISTER_WRITABLE_VALUE(QStringList, SelectModules, QStringList())
    REGISTER_WRITABLE_PROPERTY_CONTAINER(QMap, QString, QString, ModuleList, false)

public:
    enum {
        ExcuteTypeInvalid = 0,
        ExcuteTypeStart,
        ExcuteTypeParsingMode,
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

    void start(const QStringList& data = QStringList());

signals:
    void signalTestCaseCompleted(const int& type, const bool& result);

private:
    explicit TestCase();

    int excuteTestCase(const int& excuteType);
    bool inputArguments(const int& excuteType);
    void terminateApplicaton();
    void drawTerminalMenu(const int& excuteType, const QStringList& itemList, const bool& clear = true);
    QStringList selectMultipleOptionsWithNumbers(const int& excuteType, const QStringList& options);
    QStringList isModuleList();
    bool openExcelFile();

private:
    const int mNumExit = 0;
    const int mNumSelectAll = 800;
    const int mNumManualInput = 900;
};

#endif  // TEST_CASE_H
