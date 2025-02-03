#ifndef TEST_CASE_H
#define TEST_CASE_H

#include <QObject>

#include "CommonUtil.h"

class TestCase : public QObject {
    Q_OBJECT

    REGISTER_WRITABLE_PROPERTY(int, ExcuteType, 0, false)
    REGISTER_WRITABLE_PROPERTY(QStringList, ExcuteData, QStringList(), false)
    REGISTER_WRITABLE_PROPERTY(QStringList, ModuleList, QStringList(), false)
    REGISTER_WRITABLE_PROPERTY(QString, SelectAppMode, QString(), false)
    REGISTER_WRITABLE_PROPERTY(QStringList, SelectModules, QStringList(), false)

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
    void drawTerminalMenu(const int& excuteType, const QStringList& itemList);
    QStringList selectMultipleOptionsWithNumbers(const int& excuteType, const QStringList& options);
    bool openExcelFile();

private:
};

#endif  // TEST_CASE_H
