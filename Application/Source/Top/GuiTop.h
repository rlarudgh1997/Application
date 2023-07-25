#ifndef GUI_TOP_H
#define GUI_TOP_H

#include "AbstractGui.h"

#include <QWidget>
#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QKeySequence>


class GuiTop : public AbstractGui {
private:
    enum class MainType {
        File = 0,
        Edit,
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
    virtual void updateDisplay(const bool& first, const int& type = 0);
    virtual void updateDisplaySize();
    virtual void updateDisplayVisible();


public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value);


private:
    QMainWindow* mMainWindow = new QMainWindow();
    QMap<MainType, QMenu*> mMenu = QMap<MainType, QMenu*>();
    QMap<MainType, QToolBar*> mToolBar = QMap<MainType, QToolBar*>();
};

#endif    // GUI_TOP_H
