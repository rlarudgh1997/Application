#ifndef GUI_TOP_H
#define GUI_TOP_H

#include "AbstractGui.h"

#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>


class GuiTop : public AbstractGui {
    Q_OBJECT

private:
    enum class MainType {
        File = 0,
        Edit,
        View,
        Setting,
        Help,
        Max,
    };


public:
    static QSharedPointer<GuiTop>& instance(AbstractHandler* handler = nullptr);


private:
    explicit GuiTop(AbstractHandler* handler = nullptr);

    virtual void drawDisplayDepth0();
    virtual void drawDisplayDepth1();
    virtual void drawDisplayDepth2();
    virtual void updateDisplaySize();
    virtual void updateDisplayVisible();

    void drawMenuFile();
    void drawMenuEdit();
    void drawMenuView();
    void drawMenuSetting();
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

#endif    // GUI_TOP_H
