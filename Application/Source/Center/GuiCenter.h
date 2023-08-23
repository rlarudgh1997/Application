#ifndef GUI_CENTER_H
#define GUI_CENTER_H

#include "AbstractGui.h"

#include <QWidget>
#include <QTabWidget>
#include <QTableWidget>

class CellInfo {
public:
    CellInfo() : mRowStart(0), mColumnStart(0), mRowEnd(0) {
    }
    CellInfo(const int& rowStart, const int& columnStart, const int& rowEnd)
        : mRowStart(rowStart), mColumnStart(columnStart), mRowEnd(rowEnd) {
    }
    void isCellInfo(int& rowStart, int& columnStart, int& rowEnd) {
        rowStart = mRowStart;
        columnStart = mColumnStart;
        rowEnd = mRowEnd;
    }
    bool isMergeCell(const int& rowStart, const int& columnStart, const int& rowEnd) {
        if ((mRowStart == rowStart) && (mColumnStart == columnStart) && (mRowEnd == rowEnd)) {
            return true;
        }
        return false;
    }

private:
    int mRowStart = 0;
    int mColumnStart = 0;
    int mRowEnd = 0;
};

class GuiCenter : public AbstractGui {
private:
    enum class MenuItemRight {
        Invalid = 0,
        RowInsert,
        RowDelete,
        CellMergeSplit,
    };


public:
    static QSharedPointer<GuiCenter>& instance(AbstractHandler* handler = nullptr);


private:
    explicit GuiCenter(AbstractHandler* handler = nullptr);

    virtual void drawDisplayDepth0();
    virtual void drawDisplayDepth1();
    virtual void drawDisplayDepth2();
    virtual void updateDisplaySize();
    virtual void updateDisplayVisible();

    void updateDisplaySheetInfo(const int& type);


public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value);


private:
    QWidget* mMainView = nullptr;
    QTabWidget* mExcelView = nullptr;
    QMap<int, QTableWidget*> mExcelSheet = QMap<int, QTableWidget*>();
    QMap<int, QList<CellInfo>> mExcelCellInfo = QMap<int, QList<CellInfo>>();
};

#endif    // GUI_CENTER_H
