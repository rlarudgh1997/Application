#ifndef GUI_CENTER_H
#define GUI_CENTER_H

#include <QWidget>
#include <QMainWindow>
#include <QTabWidget>

#include "AbstractGui.h"


class AbstractHandler;


template <typename T1, typename T2>
inline T1* Q_CAST(T1, T2 base) {
    return qobject_cast<T1*>(base);
}

class GuiCenter : public AbstractGui {
private:
    enum ItemType {
        Screen = 0,
        MainWindow,
        TabeWidget,
    };


public:
    static QSharedPointer<GuiCenter> instance(AbstractHandler* handler = nullptr);

private:
    explicit GuiCenter(AbstractHandler* handler = nullptr);
    virtual AbstractHandler* isHandler();
    virtual QWidget* isItem(const int& type);
    virtual bool createSignal(const int& type, const QVariant& value);
    virtual void drawDisplay(const int& depth);

    // QWidget* isItem(const ItemType& type);
    // bool createSignal(const int& type, const QVariant& value);

    void drawDisplayDepth0();
    void drawDisplayDepth1();
    void drawDisplayDepth2();

public slots:
    void slotPropertyChanged(const int& type, const QVariant& value);


private:
//    AbstractHandler* mHandler = nullptr;
    QSharedPointer<QMap<ItemType, QWidget*>> mItem = QSharedPointer<QMap<ItemType, QWidget*>>();
};

#endif    // GUI_CENTER_H
