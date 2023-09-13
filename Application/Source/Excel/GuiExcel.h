#ifndef GUI_EXCEL_H
#define GUI_EXCEL_H

#include "AbstractGui.h"

#include <QWidget>
#include <QTabWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QCompleter>
#include <QStringListModel>
#include <QTableWidgetItem>
#include <QMenu>




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


class ExcelSheet {
public:
    ExcelSheet() {
        clear();
    }
    bool isCellStateMerge(const int& columnIndex, const int& rowStart, const int& rowEnd) {
        if (mMergeInfo.contains(columnIndex)) {
            foreach(const auto& v, mMergeInfo[columnIndex]) {
                if ((v.first == rowStart) && (v.second == rowEnd)) {
                    // qDebug() << "\t\t Is Cell State Merge :" << columnIndex << v.first << v.second;
                    return true;
                }
            }
        }
        return false;
    }
    QMap<int, QList<QPair<int, int>>> isMergeInfo() {
        return mMergeInfo;
    }
    void insertMergeInfo(const int& columnIndex, const int& rowStart, const int& rowEnd) {
        // qDebug() << "\t\t\t Insert Merge Cell :" << columnIndex << rowStart << rowEnd;
        mMergeInfo[columnIndex].append(QPair<int, int>(rowStart, rowEnd));
    }
    void updateMergeInfo(const int& columnIndex, const int& rowStart, const int& rowEnd) {
        QMap<int, QList<QPair<int, int>>> mergeInfo = mMergeInfo;
        QMapIterator<int, QList<QPair<int, int>>> iter(mergeInfo);
        clear();
        while (iter.hasNext()) {
            iter.next();
            int currColumnIndex = iter.key();
            foreach(const auto& v, iter.value()) {
                if ((currColumnIndex == columnIndex) && (rowStart == v.first) && (rowEnd == v.second)) {
                    // qDebug() << "\t\t 2. Erase Merge Cell :" << currColumnIndex << v.first << v.second;
                    continue;
                }
                // qDebug() << "\t\t 1. Update Merge Cell :" << currColumnIndex << v.first << v.second;
                insertMergeInfo(currColumnIndex, v.first, v.second);
            }
        }
    }

private:
    void clear(const bool& all = true) {
        mMergeInfo.clear();
    }


private:
    QMap<int, QList<QPair<int, int>>> mMergeInfo = QMap<int, QList<QPair<int, int>>>();
};


class GuiExcel : public AbstractGui {
    Q_OBJECT

private:
    enum class MenuItemRight {
        Invalid = 0,
        Insert,
        Delete,
        MergeSplit,
    };
    enum class AutoComplete {
        Draw = 0,
        Update,
        Show,
        Hide,
        Cancel,
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

    void controlConnect(const bool& state = true);

    void updateDisplayNodeAddress(const AutoComplete& type, QTableWidget* sheet, QTableWidgetItem* item);
#if defined(USE_EXCEL_FUNCTION_NEW)
    QVariantList readExcelSheet(const int& sheetIndex);
    void readAllExcelSheet();
    void updateDisplayMergeCell(const int& sheetIndex);
    void updateDisplayCellInfo(const int& sheetIndex, const QVariantList& mergeInfo, const QMap<int, QVariantList>& sheetData);
    void updateDisplayExcelSheet();
#else
    void updateDisplaySheetInfo(const int& type);
    void readExcelInfo();
#endif


public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value);


private:
    QTabWidget* mMainView = nullptr;
#if defined(USE_EXCEL_FUNCTION_NEW)
    QMap<int, QTableWidget*> mExcelSheet = QMap<int, QTableWidget*>();
    QMap<int, ExcelSheet> mCellInfo = QMap<int, ExcelSheet>();
    QMap<int, QMap<int, QPair<int, int>>> mMergeInfo = QMap<int, QMap<int, QPair<int, int>>>();
#else
    QMap<int, QTableWidget*> mExcelSheet = QMap<int, QTableWidget*>();
    QMap<int, QList<CellInfo>> mExcelCellInfo = QMap<int, QList<CellInfo>>();
#endif
    QMenu* mMenuRight = nullptr;
    QMap<MenuItemRight, QAction*> mMenuActionItem = QMap<MenuItemRight, QAction*>();

    int mCurrentSheetIndex = 0;
    QLineEdit* mInputNodeAddress = nullptr;
    QCompleter* mAutoComplete = nullptr;
    QStringListModel mNodeAddressListModel = QStringListModel();
    QTableWidget* mCurrentSheet = nullptr;
    QTableWidgetItem* mCurrentCellItem = nullptr;
};

#endif    // GUI_EXCEL_H
