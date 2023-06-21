#ifndef GUI_MAIN_H
#define GUI_MAIN_H

#include <QWidget>

class AbstractHandler;

class GuiMain : public QWidget {
public:
    static QSharedPointer<GuiMain> instance(AbstractHandler* handler = nullptr);


private:
    explicit GuiMain(AbstractHandler* handler = nullptr);
    bool createSignal(const int& type, const QVariant& value);
    void drawDisplayMain();


public slots:
    void slotPropertyChanged(const int& type, const QVariant& value);


private:
    AbstractHandler* mHandler = nullptr;
    QWidget* mScreen = nullptr;
};

#endif    // GUI_MAIN_H
