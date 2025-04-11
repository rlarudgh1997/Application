#ifndef TEST_CASE_H
#define TEST_CASE_H

#include <QObject>

#include "CommonUtil.h"

#define USE_TEST_CASE_THREAD
#if defined (USE_TEST_CASE_THREAD)
#include <QThread>
#include <QWaitCondition>
#endif

class TestCase : public QObject {
    Q_OBJECT

    REGISTER_WRITABLE_VALUE(int, ExcuteType, 0)
    REGISTER_WRITABLE_VALUE(QStringList, Arguments, QStringList())
    REGISTER_WRITABLE_VALUE(int, SelectAppMode, 0)
    REGISTER_WRITABLE_VALUE(QStringList, SelectModules, QStringList())
    REGISTER_WRITABLE_VALUE(QStringList, RemainingModules, QStringList())
    REGISTER_WRITABLE_VALUE(QString, EditingModule, QString())
    REGISTER_WRITABLE_CONTAINER(QMap, QString, QString, ModuleList)

public:
    enum {
        ExcuteTypeInvalid = 0,
        ExcuteTypeStart,
        ExcuteTypeParsingAppMode,
        ExcuteTypeParsingModule,

        ExcuteTypeExcelOpen = 10,
        ExcuteTypeGenConvertData,

        ExcuteTypeGenTC = 20,
        ExcuteTypeRunTC,

        ExcuteTypeHelpMode = 40,
        ExcuteTypeInvalidSelectItem,
        ExcuteTypeManualInput,

        ExcuteTypeCompleted = 100,
        ExcuteTypeStop = 100,
        ExcuteTypeFailed = 200,
        ExcuteTypeExit,
    };

public:
    static QSharedPointer<TestCase>& instance();
    ~TestCase();

    bool start(const QStringList& arguments = QStringList());
    void stop();

private:
    explicit TestCase();

#if defined (USE_TEST_CASE_THREAD)
    void runThread();
    void controlThread(QThread* thread, QWaitCondition& waitCondition, QMutex& mutex, const int& type);
#endif
    int excuteTestCase(const int& excuteType);
    bool parsingOptions(const QStringList& arguments);
    QStringList parsingAppMode(const QStringList& arguments);
    QStringList parsingModules(const QStringList& arguments);
    bool parsingInputArguments(const int& excuteType);
    void terminateApplicaton();
    void drawTerminalMenu(const int& excuteType, const QStringList& itemList);
    QStringList selectMultipleOptionsWithNumbers(const int& excuteType, const QStringList& options);
    QStringList isModuleList();
    QList<QVariantList> readSheetData(const bool& editingModule = false);
    void writeSheetData(const QList<QVariantList>& sheetDataList);
    bool openExcelFile();

signals:
    void signalTestCaseCompleted(const int& type, const bool& result);
    void signalGenTCInfo(const bool& result, const int& current, const int& total, const QString& text);

private:
    const QString mStrExit = QString("Exit");
    const int mNumExit = 0;
    const int mNumSelectAll = 800;
    const int mNumManualInput = 900;

#if defined (USE_TEST_CASE_THREAD)
    QSharedPointer<QThread> mThread;
    QWaitCondition mWaitCondition;
    QMutex mMutex;
#endif
};

#endif  // TEST_CASE_H
