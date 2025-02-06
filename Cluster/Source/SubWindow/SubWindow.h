#ifndef SUB_WINDOW_H
#define SUB_WINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTimer>
#include <QRect>
#include <QListWidgetItem>
#include <QMultiMap>
#include <QTextEdit>

#include "CommonUtil.h"

#define USE_FILE_WATCHER_QT
#if !defined(USE_FILE_WATCHER_QT)
#include "LogWatcher.h"
#endif

QT_BEGIN_NAMESPACE
namespace Ui {
class SubWindow;
}
QT_END_NAMESPACE

class SubWindow : public QMainWindow {
    Q_OBJECT

    REGISTER_WRITABLE_PROPERTY(bool, Init, false, false)
    REGISTER_WRITABLE_PROPERTY(int, DisplayType, 0, false)
    REGISTER_WRITABLE_PROPERTY(QString, TavPath, QString(), false)
    REGISTER_WRITABLE_PROPERTY(QString, SelectFile, QString(), false)
    REGISTER_WRITABLE_PROPERTY(QString, TavData, QString(), false)
    REGISTER_WRITABLE_PROPERTY(int, ScriptStart, 0, false)
    REGISTER_WRITABLE_PROPERTY(bool, TavSave, false, false)
    REGISTER_WRITABLE_PROPERTY(int, ListType, 0, false)
    REGISTER_WRITABLE_PROPERTY(QStringList, OriginalData, QStringList(), false)
    REGISTER_WRITABLE_PROPERTY(QStringList, DeleteFileList, QStringList(), false)
    REGISTER_WRITABLE_PROPERTY(QStringList, AltonListenData, QStringList(), false)
    REGISTER_WRITABLE_PROPERTY(QStringList, AltonServiceData, QStringList(), false)
    REGISTER_WRITABLE_PROPERTY_CONTAINER(QMap, int, QString, AllDetailInfo, false)
    REGISTER_WRITABLE_PROPERTY(QString, CheckSfcSignal, QString(), false)

private:
    // TO_ENUM(DisplayType, ListMain, ListSub, ListAltonService, ListHmi, ETC)
    enum DisplayType {
        DisplayTypeInvalid = 0,
        DisplayTypeMain,
        DisplayTypeViewTav,
        DisplayTypeViewScript,
        DisplayTypeViewScriptLog,
    };
    enum ListType {
        ListTypeNormal = 0,
        ListTypeCheck,
        ListTypeUpdateCheck,
    };
    enum TavDisplayType {
        TavDisplayTypeInvalid = 0,
        TavDisplayTypeList,
        TavDisplayTypeDetail,
        TavDisplayTypeLog,
    };
    enum ViewType {
        ViewTypeInvalid = 0,
        ViewTypeTAV,
        ViewTypeScript,
        ViewTypeRedrawTAV,
    };
    enum StartScriptType {
        StartScriptTypeStop = 0,
        StartScriptTypeStart,
        StartScriptTypeMenuStop,
        StartScriptTypeMenuStart,
    };
    enum DetailInfo {
        DetailInfoInvalid = 0,
        DetailInfoDescription,
        DetailInfoPowerTrain,
        DetailInfoPrecondition,
        DetailInfoListen,
        DetailInfoStep,
        DetailInfoExpectedResult,
        DetailInfoMax,
    };
    enum DetailSubInfo {
        DetailSubInfoNormal,
        DetailSubInfoGroup,
        DetailSubInfoPeriod,
        DetailSubInfoPeriodGroup,
    };
    enum PathType {
        PathTypeVSM,
        PathTypeTAV,
        PathTypeAltonClient,
    };
    enum DeleteType {
        DeleteTypeSelectTAV,
        DeleteTypeTAV,
        DeleteTypeScript,
        DeleteTypeInfo,
    };
    enum FileWatcherType {
        FileWatcherTypeAltonListen,
        FileWatcherTypeAltonService,
        FileWatcherTypeHmi,
    };

public:
    explicit SubWindow(QWidget* parent = nullptr);
    ~SubWindow();

private:
    void init();
    void controlConnect(const bool& state = true);
    void drawDisplay(const int& type, const QString& text = QString());
    void updateFileList(const int& type, QListWidgetItem* updateItem);
    void updateDetailFileInfo(const int& viewType, const QString& info);
    void updateDetailContent(const QString& content);
    void updateDetailDataInfo(const QString& filePath);
    void updateAltonClientLog(const QString& log);
    void updateAltonServiceLog(const QString& log);
    void updateHmiLog(const QString& log);
    void writeOriginalData(const QString& filePath, const QStringList& saveData);
    bool isDetailInfo(const int& type, QPair<QString, QStringList>& detailInfo);
    void isSubDetailInfo(const QStringList& inputStr, QMap<int, QStringList>& subDetialInfo);
    QString isDetailSignalInfo(const int& type, const QString& inputStr, QString& convertStr);
    QString isToScriptInfo(const int& type, QStringList& infoList, const QString& file = QString());
    QString createToScript(const QString& file, QStringList& scriptFileList);
    void excuteScript(const bool& start, const QString& file, const QStringList& scriptFile);
    void startProcess(const QString& command, const QString& log);
    void stopProcess();
    void startWatcherFile(const int& type, const QString& watcherFile);
    void stopWatcherFile(const int& type);
    bool isDateTimeValid(const QString& base, const QString& input);
    QStringList isVsmFileInfo(const QStringList& powerTrainList, const QStringList& signalList);
    QList<QPair<QString, QString>> isReplaceSignal(const QStringList& abstractionList, const QStringList& vsmFileList);
    QString getDataType(const QString& value);
    void settingPath(const int& pathType);
    QString openTavFile();
    bool deleteFile(const int& deleteType);
    void controlScript(const int& scriptStart, const QString& fileName);

    void controlFileWatcher(const bool& start);
    void onFileDataChanged(const std::string& data);

private:
    Ui::SubWindow* mGui;
    QTimer* mTimerTouch = nullptr;
    QSharedPointer<ivis::common::ExcuteProgramThread> mProcess = nullptr;
#if defined(USE_FILE_WATCHER_QT)
    QSharedPointer<ivis::common::FileSystemWatcherThread> mFileWatcher = nullptr;
    QMap<int, QSharedPointer<ivis::common::FileSystemWatcherThread>> mLogFileWatcher =
        QMap<int, QSharedPointer<ivis::common::FileSystemWatcherThread>>();
#else
    QSharedPointer<LogWatcher> mFileWatcher = nullptr;
#endif
};

#endif  // SUB_WINDOW_H
