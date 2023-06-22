#ifndef GUI_CENTER_H
#define GUI_CENTER_H

#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QTabWidget>


class AbstractHandler;


class GuiCenter : public QWidget {
private:
    enum class MainType {
        File = 0,
        Edit,
        Setting,
        Help,
        Max,
    };

public:
    static QSharedPointer<GuiCenter> instance(AbstractHandler* handler = nullptr);


private:
    explicit GuiCenter(AbstractHandler* handler = nullptr);
    bool createSignal(const int& type, const QVariant& value);
    void drawDisplayDepth0();
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

#endif    // GUI_CENTER_H
