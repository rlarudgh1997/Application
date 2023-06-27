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
        Widget = 0,
        MainWindow,
        TabeWidget,
    };


public:
    static QSharedPointer<GuiCenter> instance(AbstractHandler* handler = nullptr);
    virtual void initItem();
    virtual QWidget* isItem(const int& type);
    virtual bool createSignal(const int& type, const QVariant& value);
    virtual void drawDisplayDepth0();
    virtual void drawDisplayDepth1();
    virtual void drawDisplayDepth2();


private:
    explicit GuiCenter(AbstractHandler* handler = nullptr);



public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value);


private:
    QSharedPointer<QMap<ItemType, QWidget*>> mItem = QSharedPointer<QMap<ItemType, QWidget*>>();
};

#endif    // GUI_CENTER_H
