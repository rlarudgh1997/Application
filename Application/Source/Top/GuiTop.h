#ifndef GUI_TOP_H
#define GUI_TOP_H

#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QTabWidget>
#include <QKeySequence>


class AbstractHandler;

class ActionInfo {
public:
    ActionInfo() {}
    ActionInfo(const int& eventID, const QString& name, const QIcon& icon, const QString& title,
                const QString& tip, const QKeySequence::StandardKey& shortcuts, QObject* parent = nullptr)
    : mEventID(eventID), mName(name), mIcon(icon), mTitle(title)
    , mTip(tip), mShortcuts(shortcuts), mParent(parent) {}


private:
    int mEventID = 0;
    QString mName = QString();
    QIcon mIcon = QIcon();
    QString mTitle = QString();
    QString mTip = QString();
    QKeySequence::StandardKey mShortcuts;
    QObject* mParent = nullptr;
};

class GuiTop : public QWidget {
private:
    enum class MainType {
        File = 0,
        Edit,
        Setting,
        Help,
        Max,
    };
    enum class ActionType {
        FileNew = 0,
        FileOpen,
        FileSave,
        FileSaveAs,
        FileExit,
        EditCut,
        EditCopy,
        EditPaste,
        SettingPath,
        SettingReport,
        SettingResult,
        SettingCoverage,
        HelpAbout,
        HelpAboutQt,
        Max,
    };

public:
    static QSharedPointer<GuiTop> instance(AbstractHandler* handler = nullptr);


private:
    explicit GuiTop(AbstractHandler* handler = nullptr);
    bool createSignal(const int& type, const QVariant& value);
    void drawDisplayDepth0();
    void drawDisplayDepth1();
    void drawDisplayDepth2();
    void updateDisplay(const bool& first, const int& type = 0);


public slots:
    void slotPropertyChanged(const int& type, const QVariant& value);


private:
    AbstractHandler* mHandler = nullptr;
    QWidget* mScreen = nullptr;
    QMainWindow* mMainWindow = nullptr;
    QTabWidget* mTabWidget = nullptr;
    QMap<MainType, QMenu*> mMenu = QMap<MainType, QMenu*>();
    QMap<MainType, QToolBar*> mToolBar = QMap<MainType, QToolBar*>();
    QMap<ActionType, ActionInfo> mActionInfo = QMap<ActionType, ActionInfo>();
};

#endif    // GUI_TOP_H
