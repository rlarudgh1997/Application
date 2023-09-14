#ifndef GUI_MENU_H
#define GUI_MENU_H

#include "AbstractGui.h"

#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>


class GuiMenu : public AbstractGui {
    Q_OBJECT

private:
    enum class MainType {
        File = 0,
        Edit,
        View,
        Setting,
        Report,
        Run,
        Help,
        Max,
    };


public:
    static QSharedPointer<GuiMenu>& instance(AbstractHandler* handler = nullptr);


private:
    explicit GuiMenu(AbstractHandler* handler = nullptr);

    virtual void drawDisplayDepth0();
    virtual void drawDisplayDepth1();
    virtual void drawDisplayDepth2();
    virtual void updateDisplaySize();
    virtual void updateDisplayVisible();

    void drawMenuFile();
    void drawMenuEdit();
    void drawMenuView();
    void drawMenuSetting();
    void drawMenuReport();
    void drawMenuRun();
    void drawMenuHelp();
    void drawMenuEtc();
    void updateDisplayDefaultPath();


public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value);


private:
    QMainWindow* mMainView = nullptr;
    QMap<MainType, QMenu*> mMenu = QMap<MainType, QMenu*>();
    QMap<MainType, QToolBar*> mToolBar = QMap<MainType, QToolBar*>();
};

#endif    // GUI_MENU_H
