#ifndef GUI_MAIN_H
#define GUI_MAIN_H

#include <QWidget>
#if defined(USE_INTANCE_SINGLETON_GUI)
#include <QSharedPointer>
#endif

#include "CommonDefine.h"

class GuiMain : public QWidget {
public:
#if defined(USE_INTANCE_SINGLETON_GUI)
    static QSharedPointer<HandlerMain> instance(QWidget* parent);

private:
    explicit GuiMain(QWidget* parent);
#else
    explicit GuiMain(QWidget* parent = nullptr);
    ~GuiMain();
#endif

    void updateGuiScreen(QWidget* parent);
    void updateGuiProperty(const int& dataType, const QVariant& value);
    void drawDisplayMain();

private:
    QWidget* mGuiScreen = nullptr;
};

#endif    // GUI_MAIN_H
