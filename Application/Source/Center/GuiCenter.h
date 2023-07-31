#ifndef GUI_CENTER_H
#define GUI_CENTER_H

#include "AbstractGui.h"

#include <QWidget>
#include <QTabWidget>
#include <QTableWidget>


class GuiCenter : public AbstractGui {
public:
    static QSharedPointer<GuiCenter>& instance(AbstractHandler* handler = nullptr);


private:
    explicit GuiCenter(AbstractHandler* handler = nullptr);

    virtual void drawDisplayDepth0();
    virtual void drawDisplayDepth1();
    virtual void drawDisplayDepth2();
    virtual void updateDisplay(const bool& first, const int& type = 0);
    virtual void updateDisplaySize();
    virtual void updateDisplayVisible();

    void updateDisplaySheetInfo(const int& type);


public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value);


private:
    QTabWidget* mTabWidget = nullptr;
    QMap<int, QTableWidget*> mTableWidgets = QMap<int, QTableWidget*>();
};

#endif    // GUI_CENTER_H
