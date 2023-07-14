#ifndef GUI_CENTER_H
#define GUI_CENTER_H

#include "AbstractGui.h"

#include <QWidget>
#include <QTabWidget>
#include <QTableWidget>


// class ItemTypeEnum;

class GuiCenter : public AbstractGui {
private:
    enum ItemType {
        Widget = 0,
        MainWindow,
        TabeWidget,
    };


public:
    static QSharedPointer<GuiCenter>& instance(AbstractHandler* handler = nullptr);


protected:
    virtual void initItem();
    virtual QWidget* isItem(const int& type);
    virtual bool createSignal(const int& type, const QVariant& value);
    virtual void drawDisplayDepth0();
    virtual void drawDisplayDepth1();
    virtual void drawDisplayDepth2();
    virtual void updateDisplay(const bool& first, const int& type = 0);


private:
    explicit GuiCenter(AbstractHandler* handler = nullptr);



public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value);


private:
    QSharedPointer<QMap<ItemType, QWidget*>> mItem = QSharedPointer<QMap<ItemType, QWidget*>>();
    QMap<int, QTableWidget*> mTableWidgets = QMap<int, QTableWidget*>();
    // ItemTypeEnum::ItemType mItemType;
};

#endif    // GUI_CENTER_H
