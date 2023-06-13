#ifndef GUI_MAIN_H
#define GUI_MAIN_H

#include <QWidget>
// #include <QSharedPointer>


class GuiMain : public QWidget {
public:
#if defined(USE_GUI_INSTANCE_SINGTONE)
    static QSharedPointer<HandlerMain> instance(QWidget* parent);

private:
    explicit GuiMain(QWidget* parent);
#else
    explicit GuiMain(QWidget* parent = nullptr);
    ~GuiMain();
#endif

    void updateGuiScreen(QWidget* parent);
    void updateGui(const int& dataType, const QVariant& value);
    void drawDisplayMain();

private:
    QWidget* mGuiScreen = nullptr;
};

#endif    // GUI_MAIN_H
