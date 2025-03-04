#ifndef GUI_EXCEL_H
#define GUI_EXCEL_H

#include "AbstractGui.h"
#include "CommonUtil.h"

#include <QWidget>
#include <QTabWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QCompleter>
#include <QStringListModel>
#include <QTableWidgetItem>
#include <QMenu>

#include <QDialog>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "Dialog.h"
#include "ui_GuiExcel.h"

class ExcelSheet {
public:
    ExcelSheet() {
        clear();
    }
    void clear() {
        mInfo.clear();
    }
    QMap<int, QList<QPair<int, int>>> isMergeInfo() {
        return mInfo;
    }
    bool isContains(const int& columnIndex, const int& rowStart, const int& rowEnd) {
        if (mInfo.contains(columnIndex)) {
            for (const auto& v : mInfo[columnIndex]) {
                if ((v.first == rowStart) && (v.second == rowEnd)) {
                    return true;
                }
            }
        }
        return false;
    }
    bool isContains(const int& columnIndex, const int& rowStart) {
        if (mInfo.contains(columnIndex)) {
            for (const auto& v : mInfo[columnIndex]) {
                if (v.first == rowStart) {
                    return true;
                }
            }
        }
        return false;
    }
    int isMergeCellCount(const int& columnIndex, const int& rowStart) {
        if (mInfo.contains(columnIndex)) {
            for (const auto& v : mInfo[columnIndex]) {
                if (v.first == rowStart) {
                    return v.second;
                }
            }
        }
        return 0;
    }
    void insert(const int& columnIndex, const int& rowStart, const int& rowEnd) {
        mInfo[columnIndex].append(QPair<int, int>(rowStart, rowEnd));
    }
    void erase(const int& columnIndex, const int& rowStart, const int& rowEnd) {
        QMap<int, QList<QPair<int, int>>> mergeInfo = mInfo;
        QMapIterator<int, QList<QPair<int, int>>> iter(mergeInfo);
        mInfo.clear();
        while (iter.hasNext()) {
            iter.next();
            int currColumnIndex = iter.key();
            for (const auto& v : iter.value()) {
                if ((currColumnIndex == columnIndex) && (rowStart == v.first) && (rowEnd == v.second)) {
                    // qDebug() << "\t\t 3. Erase Merge Cell :" << currColumnIndex << rowStart<< rowEnd;
                    continue;
                }
                // qDebug() << "\t\t 4. Append Merge Cell :" << currColumnIndex << v.first<< v.second;
                insert(currColumnIndex, v.first, v.second);
            }
        }
    }

private:
    QMap<int, QList<QPair<int, int>>> mInfo = QMap<int, QList<QPair<int, int>>>();
};

class CellSelectedInfo {
    REGISTER_WRITABLE_VALUE(int, RowStart, 0)
    REGISTER_WRITABLE_VALUE(int, RowEnd, 0)
    REGISTER_WRITABLE_VALUE(int, ColumnStart, 0)
    REGISTER_WRITABLE_VALUE(int, ColumnEnd, 0)
    REGISTER_WRITABLE_VALUE(QModelIndexList, ModelIndexs, QModelIndexList())

public:
    CellSelectedInfo(const int& rowStart, const int& rowEnd, const int& columnStart, const int& columnEnd) {
        setRowStart(rowStart);
        setRowEnd(rowEnd);
        setColumnStart(columnStart);
        setColumnEnd(columnEnd);
    }
    explicit CellSelectedInfo(const QModelIndexList& modelIndexs) {
        if (modelIndexs.isEmpty()) {
            qDebug() << "Selected Cell is empty";
            return;
        }

        int rowStart = modelIndexs.first().row();
        int rowEnd = rowStart;
        int columnStart = modelIndexs.first().column();
        int columnEnd = columnStart;

        for (const auto& currCell : modelIndexs) {
            int row = currCell.row();
            int column = currCell.column();

            rowStart = qMin(rowStart, row);
            rowEnd = qMax(rowEnd, row);
            columnStart = qMin(columnStart, column);
            columnEnd = qMax(columnEnd, column);
        }

        setRowStart(rowStart);
        setRowEnd(rowEnd);
        setColumnStart(columnStart);
        setColumnEnd(columnEnd);
        setModelIndexs(modelIndexs);
    }

    CellSelectedInfo() = default;
    CellSelectedInfo(const CellSelectedInfo& other) = default;
    CellSelectedInfo& operator=(const CellSelectedInfo& other) = default;

    bool operator==(const CellSelectedInfo& other) const {
        return ((getRowStart() == other.getRowStart()) && (getRowEnd() == other.getRowEnd()) &&
                (getColumnStart() == other.getColumnStart()) && (getColumnEnd() == other.getColumnEnd()) &&
                (getModelIndexs() == other.getModelIndexs()));
    }
    bool operator!=(const CellSelectedInfo& other) const {
        return !(*this == other);
    }

    int getRowCount() const {
        return (getRowEnd() - getRowStart() + 1);
    }
    int getColumnCount() const {
        return (getColumnEnd() - getColumnStart() + 1);
    }
    QMap<int, QPair<int, int>> getRowRangeList() const {
        QMap<int, QPair<int, int>> rowRangeList;
        for (const auto& currCell : getModelIndexs()) {
            int row = currCell.row();
            int column = currCell.column();

            auto& range = rowRangeList[column];
            if (rowRangeList.contains(column)) {
                range.first = qMin(range.first, row);
                range.second = qMax(range.second, row);
            } else {
                range = qMakePair(row, row);
            }
        }
        // qDebug() << "\t getRowRangeList :" << rowRangeList;
        return rowRangeList;
    }
    bool getNotSupport() const {
        auto rowRangeList = getRowRangeList();
        if (rowRangeList.size() == 0) {
            return false;
        }
        auto& rangeInfo = rowRangeList[getColumnStart()];
        for (const auto& key : rowRangeList.keys()) {
            // qDebug() << "\t RowInfo[" << key << "] :" << rowRangeList[key] << rangeInfo;
            if (rangeInfo != rowRangeList[key]) {
                return true;
                break;
            }
            rangeInfo = rowRangeList[key];
        }
        return false;
    }
};

class GuiExcel : public AbstractGui {
    Q_OBJECT

    REGISTER_WRITABLE_VALUE(bool, SfcSignal, false)
    REGISTER_WRITABLE_VALUE(bool, OutputState, false)
    REGISTER_WRITABLE_VALUE(bool, CellEditSkip, false)
    REGISTER_WRITABLE_VALUE(int, CurrSheetIndex, 0)
    REGISTER_WRITABLE_CONTAINER(QMap, int, bool, SheetCheckState)

private:
    enum CellMergeType {
        ClearMerge,
        CopyMerge,
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

    void updateDrawDialog(const int& dialogType, const QVariantList& info);
    bool chcekExcelSheet(const int& sheetIndex);
    QVariantList readSheetProperty(const int& sheetIndex, const QVariantList& readIndexInfo, QString& allString);
    QVariantList readSheetDisplay(const int& sheetIndex);
    void syncSheetData(const int& sheetIndex, const bool& readProperty);
    bool isSheetChanged(const int& sheetIndex);
    int isMergeCell(const int& sheetIndex, const int& columnIndex, const int& rowStart);
    bool isDrawCheckBox(const int& sheetIndex, const int& columnIndex);
    bool updateMergeInfo(const bool& erase, const int& sheetIndex, const int& columnIndex, const int& rowStart,
                         const int& rowEnd);
    QMap<int, QList<QPair<int, int>>> findMergeInfo(const QMap<int, QVariantList>& sheetData);
    void constructMergeInfo(const int& sheetIndex, const QMap<int, QVariantList>& sheetData);
    void constructMergeSplitInfo(const QMap<int, QVariantList>& sheetData, const int& rowStart, const int& columnStart);
    void updateDisplaySplitCell(const int& sheetIndex);
    void updateDisplayMergeCell(const int& sheetIndex);
    QMap<int, QSet<QPair<int, int>>> isSheetMergeInfo(const int& sheetIndex, const bool& refreshInfo);
    int updateDisplayInsertDelete(const int& sheetIndex, const bool& insert);
    int updateDisplayMergeSplit(const int& sheetIndex);
    int updateDisplayCopy(const int& sheetIndex, const bool& cutState);
    int updateDisplayPaste(const int& sheetIndex);
    void updateDisplaySheetHeaderAdjust(const int& sheetIndex, const bool& resizeColumn);
    void updateDisplaySheetNew(const int& sheetIndex, const int& rowMax, const int& columnMax);
    void updateDisplaySheetText(const int& sheetIndex);
    void updateCellInfoContent(const int& sheetIndex, const int& row, const int& column);
    void updateDefaultSheetFocus(const int& sheetIndex, const int& row, const int& column);
    void updateInitialExcelSheet();
    void updateDisplayKey(const int& keyValue);
    void updateDisplayArrowKey(const int& keyValue);
    void updateDisplayExcelSheet();
    void updateDisplayCellDataInfo(const int& sheetIndex, const int& row, const int& column);
    void updateDisplayAutoComplete(const int& sheetIndex, const int& row, const int& column);
    void updateDisplayAutoCompleteSignal(const bool& description, const int& columnIndex);
    void updateDisplayAutoCompleteVehicle();
    void updateDisplayAutoCompleteConfigName();
    void updateDisplayValueEnum(const QVariantList& data);
    void updateDisplayTCNameResult(const QStringList& data);
    void updateDisplayAutoInputDescrtion();
    void updateDisplaySelectGenType();
    void updateDisplayGenType(const int& genType);
    void updateDisplayTCCheck(const int& allCheck);
    void printMergeInfo(const QString& title, const bool& mergeSplit);
    void copyClipboardInfo(const bool& cutState);
    int clearClipboardInfo(const bool& escapeKeyClear);
    void pasteClipboardInfo();
    QList<QStringList> isSheetData(const int& sheetIndex, const bool& removeMerge, const bool& readProperty);
    QString isCurrentCellText(const int& sheetIndex, const int& rowIndex, const int& columnIndex);
    void updateDisplayReceiveKeyFocus();
    void updateDisplayShortcut(const int& shortcutType);
    void updateDisplaySheetCheckState(const int& sheetIndex, const int& columnIndex);
    void updateDescriptionInfo(const int& sheetIndex, const int& row);

public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value);

private:
    Ui::GuiExcel* mGui = nullptr;
    QWidget* mMainView = nullptr;
    QSharedPointer<Dialog> mDialog = nullptr;
    QMap<int, QTableWidget*> mExcelSheet = QMap<int, QTableWidget*>();
    QMap<int, ExcelSheet> mMergeInfo = QMap<int, ExcelSheet>();
    QMap<int, QPair<int, int>> mModelIndex = QMap<int, QPair<int, int>>();
    ExcelSheet mCopyMergeInfo = ExcelSheet();
    ExcelSheet mClearMergeInfo = ExcelSheet();
    QList<int> mClearCellInfo = QList<int>();
    QMenu* mMenuRight = nullptr;
    QMap<int, QAction*> mMenuActionItem = QMap<int, QAction*>();
    QTableWidgetItem* mSelectItem = nullptr;
    int mCurrentSheetIndex = 0;
};

#endif  // GUI_EXCEL_H
