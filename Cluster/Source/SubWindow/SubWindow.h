#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTimer>
#include <QRect>
#include <QStandardItemModel>
#include <QListWidgetItem>

#include "CommonUtil.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class SubWindow;
}
QT_END_NAMESPACE

class SubWindow : public QMainWindow {
    Q_OBJECT

    REGISTER_WRITABLE_PROPERTY(bool, Init, false, false)
    REGISTER_WRITABLE_PROPERTY(int, CurrentDisplay, 0, false)
    REGISTER_WRITABLE_PROPERTY(QString, CurrentTavPath, QString(), false)
    REGISTER_WRITABLE_PROPERTY(QString, SelectFile, QString(), false)
    REGISTER_WRITABLE_PROPERTY(QString, PreviousTavData, QString(), false)
    REGISTER_WRITABLE_PROPERTY(bool, ScriptStart, false, false)
    REGISTER_WRITABLE_PROPERTY(bool, TavSave, false, false)
    REGISTER_WRITABLE_PROPERTY(int, ListType, 0, false)

private:
    enum DisplayType {
        DisplayTypeInvalid = 0,
        DisplayTypeListMain,
        DisplayTypeListSub,
        DisplayTypeListAltonService,
        DisplayTypeListHmi,
        DisplayTypeETC,
    };
    enum ListType {
        ListTypeNormal = 0,
        ListTypeCheck,
        ListTypeUpdateCheck,
    };
    enum ViewType {
        ViewTypeInvalid = 0,
        ViewTypeTAV,
        ViewTypeScript,
        ViewTypeRedrawTAV,
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
    };

public:
    SubWindow(QWidget* parent = nullptr);
    ~SubWindow();

private:
    void init();
    void controlConnect(const bool& state = true);
    void drawDisplay(const int& type, const QString& text = QString());
    void updateFileList(const int& type, QListWidgetItem* updateItem);
    void updateDetailFileInfo(const int& viewType, const QString& info);
    void updateDetailDataInfo(const QString& filePath);
    void writeOriginalData(const QString& filePath, const QStringList& saveData);
    bool isDetailInfo(const int& type, QPair<QString, QStringList>& detailInfo);
    void isSubDetailInfo(const QStringList& inputStr, QMap<int, QStringList>& subDetialInfo);
    QString isDetailSignalInfo(const int& type, const QString& inputStr, QString& convertStr);
    QString isToScriptInfo(const int& type, QStringList& infoList);
    QString createToScript(const QString& file, QStringList& scriptFileList);
    void excuteScript(const bool& start, const QString& file, const QStringList& scriptFile);
    void startProcess(const QString& command, const QString& arg);
    void stopProcess();
    void startWatcherFile(const int& type, const QString& watcherFile);
    void stopWatcherFile(const int& type);
    QStringList isVsmFileInfo(const QStringList& powerTrainList, const QStringList& signalList);
    QList<QPair<QString, QString>> isReplaceSignal(const QStringList& abstractionList, const QStringList& vsmFileList);
    QString isDataType(const QString& value);
    void settingPath(const int& pathType);
    QString openTavFile();
    bool deleteFile(const int& deleteType);

private:
    Ui::SubWindow* mGui;
    QTimer* mTimerTouch = nullptr;
    QStandardItemModel mModel = QStandardItemModel();
    QMap<int, QString> mDetailInfo = QMap<int, QString>();
    QStringList mOriginalData = QStringList();
    QStringList mScriptFileList = QStringList();
    QStringList mDeleteFileList = QStringList();
};

#endif  // MAINWINDOW_H
