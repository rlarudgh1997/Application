#ifndef GUI_EXCEL_H
#define GUI_EXCEL_H

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

class GuiExcel : public AbstractGui {
    Q_OBJECT

private:
    enum class MenuItemRight {
        Invalid = 0,
        RowInsert,
        RowDelete,
        CellMergeSplit,
    };


public:
    static QSharedPointer<GuiExcel>& instance(AbstractHandler* handler = nullptr);


private:
    explicit GuiExcel(AbstractHandler* handler = nullptr);

    virtual void drawDisplayDepth0();
    virtual void drawDisplayDepth1();
    virtual void drawDisplayDepth2();
    virtual void updateDisplaySize();
    virtual void updateDisplayVisible();

    void updateDisplaySheetInfo(const int& type);


public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value);


private:
    QTabWidget* mMainView = nullptr;
    QMap<int, QTableWidget*> mExcelSheet = QMap<int, QTableWidget*>();
    QMap<int, QList<CellInfo>> mExcelCellInfo = QMap<int, QList<CellInfo>>();
};

#endif    // GUI_EXCEL_H