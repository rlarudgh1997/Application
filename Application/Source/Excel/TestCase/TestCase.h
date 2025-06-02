#ifndef TEST_CASE_H
#define TEST_CASE_H

#include <QObject>

#include "CommonUtil.h"

#define USE_TEST_CASE_THREAD
#if defined (USE_TEST_CASE_THREAD)
#include <QThread>
#include <QWaitCondition>
#endif

using QPairString = QPair<QString, QString>;
class TestCase : public QObject {
    Q_OBJECT

    REGISTER_WRITABLE_VALUE(int, ExcuteType, 0)
    REGISTER_WRITABLE_VALUE(QStringList, Arguments, QStringList())
    REGISTER_WRITABLE_VALUE(bool, AllModuleSelect, false)
    REGISTER_WRITABLE_VALUE(int, SelectAppMode, 0)
    REGISTER_WRITABLE_VALUE(QStringList, SelectModules, QStringList())
    REGISTER_WRITABLE_VALUE(QStringList, RemainingModules, QStringList())
    REGISTER_WRITABLE_VALUE(QString, EditingModule, QString())
    REGISTER_WRITABLE_CONTAINER(QMap, QString, QString, ModuleList)
    REGISTER_WRITABLE_VALUE(QStringList, GenTCInfo, QStringList())
    REGISTER_WRITABLE_VALUE(bool, GenTCResult, false)
    REGISTER_WRITABLE_VALUE(bool, ThreadRunState, true)
    REGISTER_WRITABLE_VALUE(QFileInfo, OpenFile, QFileInfo())
    REGISTER_WRITABLE_CONTAINER(QMap, int, QPairString, GenTCResultInfo)

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
        ExcuteTypeGenTCComplted,

        ExcuteTypeHelpMode = 40,
        ExcuteTypeInvalidSelectItem,
        ExcuteTypeManualInput,

        ExcuteTypeCompleted = 100,
        ExcuteTypeStop,
        ExcuteTypeFailed = 200,
        ExcuteTypeExit,
    };

public:
    static QSharedPointer<TestCase>& instance();
    ~TestCase();

    bool start(const QStringList& arguments = QStringList(), const QFileInfo& fileInfo = QFileInfo());
    void stop(const bool& appExit = false);

private:
    explicit TestCase();

#if defined (USE_TEST_CASE_THREAD)
    void runThread();
    void controlThread(QThread* thread, QWaitCondition& waitCondition, QMutex& mutex, const int& type);
#endif
    int excuteTestCase(const int& excuteType);
    void updateTestCaseExcuteInfo(const int& excuteType, const QString& text);
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
    void signalTestCaseCompleted(const int& type, const bool& sheetReload);
    void signalGenTCInfo(const int& resultType, const int& current, const int& total, const QStringList& info);

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
