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

class SelectedCellInfo {
public:
    explicit SelectedCellInfo(QTableWidget* tableWidget) {
        clear();
        updateSelectedCellInfo(tableWidget);
    }
    SelectedCellInfo() = default;
    SelectedCellInfo(const SelectedCellInfo& other) = default;
    SelectedCellInfo& operator=(const SelectedCellInfo& other) = default;

    QPair<int, int> isRowInfo() const {
        return QPair<int, int>(mRowStart, mRowCount);
    }
    QPair<int, int> isColumnInfo() const {
        return QPair<int, int>(mColumnStart, mColumnCount);
    }
    QPair<int, int> isEndInfo() const {
        return QPair<int, int>((mRowStart + mRowCount), (mColumnStart + mColumnCount));
    }
    QPair<int, int> isMaxInfo() const {
        return QPair<int, int>(mRowMax, mColumnMax);
    }
    QModelIndexList isModelIndexsInfo() const {
        return mModelIndexs;
    }

private:
    void updateSelectedCellInfo(QTableWidget* tableWidget) {
        if (tableWidget == nullptr) {
            return;
        }
        const QModelIndexList modelIndexs = tableWidget->selectionModel()->selectedIndexes();
        const bool selected = (modelIndexs.size() > 0);

        mRowStart = (selected) ? (modelIndexs.at(0).row()) : (tableWidget->currentRow());
        mRowCount = (selected) ? (modelIndexs.last().row() - mRowStart + 1) : (1);
        mRowMax = tableWidget->rowCount();

        mColumnStart = (selected) ? (modelIndexs.at(0).column()) : (tableWidget->currentColumn());
        mColumnCount = (selected) ? (modelIndexs.last().column() - mColumnStart + 1) : (1);
        mColumnMax = tableWidget->columnCount();

        if (mRowCount <= 0) {
            QList<int> rowList = QList<int>();
            for (const auto& index : modelIndexs) {
                rowList.append(index.row());
            }
            if (rowList.size() > 0) {
                mRowStart = (*std::min_element(rowList.constBegin(), rowList.constEnd()));
                mRowCount = (*std::max_element(rowList.constBegin(), rowList.constEnd())) - mRowStart + 1;
            }
        }
        ivis::common::LIMIT(mRowStart, 0, mRowMax);
        ivis::common::LIMIT(mColumnStart, 0, mColumnMax);
    }
    void clear() {
        mRowStart = Default;
        mRowCount = Default;
        mRowMax = Default;
        mColumnStart = Default;
        mColumnCount = Default;
        mColumnMax = Default;
        mModelIndexs.clear();
    }

private:
    const int Default = (-1);
    int mRowStart = Default;
    int mRowCount = Default;
    int mColumnStart = Default;
    int mColumnCount = Default;
    int mRowMax = Default;
    int mColumnMax = Default;
    QModelIndexList mModelIndexs = QModelIndexList();
};

class GuiExcel : public AbstractGui {
    Q_OBJECT

    REGISTER_WRITABLE_VALUE(bool, SfcSignal, false)
    REGISTER_WRITABLE_VALUE(bool, OutputState, false)
    REGISTER_WRITABLE_VALUE(bool, CellEditSkip, false)
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
    QVariantList readExcelSheet(const int& sheetIndex, const QVariantList& readIndexInfo, QString& allString);
    void syncSheetData(const int& sheetIndex);
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
    QMap<int, QSet<QPair<int, int>>> isSheetMergeInfo(const int& sheetIndex);
    void updateSheetMergeInfo(const int& editType, const int& sheetIndex, const int& rowStart, const int& rowCount,
                              const int& columnStart, const int& columnCount);
    void updateDisplayInsertDelete(const int& editType, const int& sheetIndex, const int& columnIndex, const int& rowStart,
                                   const int& rowCount);
    void updateDisplayMergeSplit(const int& editType, const int& sheetIndex, const int& columnStart, const int& columnCount,
                                 const int& rowStart, const int& rowCount);
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
    QList<QStringList> isSheetData(const int& sheetIndex, const bool& removeMerge, const QPair<int, int>& maxInfo);
    QString isCurrentCellText(const int& sheetIndex, const int& rowIndex, const int& columnIndex);
    QString isSeletedSheetData(const int& sheetIndex);
    void updateConstructClipboardInfo(const int& clipboardType);
    void updateDisplayReceiveKeyFocus();
    void updateDisplayEditCellShortcut(const int& editType);
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
