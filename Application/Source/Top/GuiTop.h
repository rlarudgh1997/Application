#ifndef GUI_TOP_H
#define GUI_TOP_H

#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QTabWidget>


class AbstractHandler;


class GuiTop : public QWidget {
private:
    enum class MainType {
        File = 0,
        Edit,
        Setting,
        Help,
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
    void updateDisplay(const bool& first);


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

#endif    // GUI_TOP_H
