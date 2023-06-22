#ifndef GUI_MAIN_H
#define GUI_MAIN_H

#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QTabWidget>


class AbstractHandler;


class GuiMain : public QWidget {
private:
    enum class MainType {
        File = 0,
        Edit,
        Setting,
        Help,
        Max,
    };

public:
    static QSharedPointer<GuiMain> instance(AbstractHandler* handler = nullptr);


private:
    explicit GuiMain(AbstractHandler* handler = nullptr);
    bool createSignal(const int& type, const QVariant& value);
    void drawDisplayMain();
    void drawDisplayDepth1();
    void drawDisplayDepth2();


public slots:
    void slotPropertyChanged(const int& type, const QVariant& value);


private:
    AbstractHandler* mHandler = nullptr;
    QWidget* mScreen = nullptr;
    QMainWindow* mMainWindow = nullptr;
    QTabWidget* mTabWidget = nullptr;
    QMap<MainType, QMenu*> mMenu = QMap<MainType, QMenu*>();
    QMap<MainType, QToolBar*> mToolBar = QMap<MainType, QToolBar*>();
};

#endif    // GUI_MAIN_H
