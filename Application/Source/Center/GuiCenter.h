#ifndef GUI_CENTER_H
#define GUI_CENTER_H

#include "AbstractGui.h"

#include <QWidget>
#include <QTabWidget>
#include <QTableWidget>


// class ItemTypeEnum;

class CellInfo {
public:
    CellInfo() {}
    CellInfo(const int& startRow, const int& startColumn, const int& endRow, const int& endColumn)
        : mStartRow(startRow), mStartColumn(startColumn), mEndRow(endRow), mEndColumn(endColumn) {}
    explicit CellInfo(const QRect& mergeInfo)
        : mStartRow(mergeInfo.x()), mStartColumn(mergeInfo.y()), mEndRow(mergeInfo.width()), mEndColumn(mergeInfo.height()) {}
    CellInfo(const int& row, const int& column) : mRow(row), mColumn(column) {}
    int isCellInfoRow() const { return mRow; }
    int isCellInfoColumn() const { return mColumn; }
    const QRect isCellInfo() {
        QRect info;
        info.setX(mStartRow);
        info.setY(mStartColumn);
        info.setWidth(mEndColumn);
        info.setHeight(mStartRow);
        return info;
    }

private:
    int mStartRow = 0;
    int mStartColumn = 0;
    int mEndRow = 0;
    int mEndColumn = 0;

    int mRow = 0;
    int mColumn = 0;
};

class GuiCenter : public AbstractGui {
public:
    static QSharedPointer<GuiCenter>& instance(AbstractHandler* handler = nullptr);


private:
    explicit GuiCenter(AbstractHandler* handler = nullptr);
    virtual bool createSignal(const int& type, const QVariant& value);
    virtual void drawDisplay(const QVariant& depth);
    virtual void drawDisplayDepth0();
    virtual void drawDisplayDepth1();
    virtual void drawDisplayDepth2();
    virtual void updateDisplay(const bool& first, const int& type = 0);

    void updateDisplaySize();
    void updateDisplayVisible();
    void updateDisplaySheetInfo();


public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value);


private:
    QTabWidget* mTabWidget = nullptr;
    QMap<int, QTableWidget*> mTableWidgets = QMap<int, QTableWidget*>();
    const int mMarginPosY = 120;

};

#endif    // GUI_CENTER_H
