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
#include <QUndoStack>

#include "Dialog.h"
#include "ui_GuiExcel.h"

using QPairInt = QPair<int, int>;

class SheetData : public QObject, public QUndoCommand {
    Q_OBJECT

public:
    SheetData(const int& index, const int& row, const int& column, const QList<QStringList>& previousData,
              const QList<QStringList>& currentData)
        : mSheetIndex(index), mRowIndex(row), mColumnIndex(column), mPreviousData(previousData), mCurrentData(currentData) {}
    void undo() override {
        emit signalSheetDataUndo(mSheetIndex, mRowIndex, mColumnIndex, mPreviousData);
    }
    void redo() override {
        if (mFirstRedoSkip) {    // 데이터 추가시 redo 함수 호출 예외처리
            // for (const auto& rowData : mCurrentData) {
            //     qDebug() << "\t Data :" << rowData;
            // }
            mFirstRedoSkip = false;
            return;
        }
        emit signalSheetDataUndo(mSheetIndex, mRowIndex, mColumnIndex, mCurrentData);
    }

signals:
    void signalSheetDataUndo(const int& sheetIndex, const int& row, const int& column, const QList<QStringList>& data);

private:
    bool mFirstRedoSkip = true;
    int mSheetIndex;
    int mRowIndex;
    int mColumnIndex;
    QList<QStringList> mPreviousData;
    QList<QStringList> mCurrentData;
};

class CellSelectedInfo {
    REGISTER_WRITABLE_VALUE(int, RowStart, 0)
    REGISTER_WRITABLE_VALUE(int, RowEnd, 0)
    REGISTER_WRITABLE_VALUE(int, RowMax, 0)
    REGISTER_WRITABLE_VALUE(int, ColumnStart, 0)
    REGISTER_WRITABLE_VALUE(int, ColumnEnd, 0)
    REGISTER_WRITABLE_VALUE(int, ColumnMax, 0)
    REGISTER_WRITABLE_VALUE(QModelIndexList, ModelIndexs, QModelIndexList())

    REGISTER_WRITABLE_VALUE(int, SheetIndex, 0)
    REGISTER_WRITABLE_VALUE(bool, ClearData, false)
    REGISTER_WRITABLE_CONTAINER(QMap, int, QList<QPairInt>, ClearMergeInfo)
    REGISTER_WRITABLE_CONTAINER(QMap, int, QList<QPairInt>, SpanMergeInfo)

public:
    explicit CellSelectedInfo(const QTableWidget* excelSheet) {
        if (excelSheet == nullptr) {
            qDebug() << "ExcelSheet is nullptr";
            return;
        }

        const QModelIndexList& modelIndexs = excelSheet->selectionModel()->selectedIndexes();
        if (modelIndexs.isEmpty()) {
            qDebug() << "Selected cell is empty";
            return;
        }

        int rowStart = modelIndexs.first().row();
        int rowEnd = rowStart;
        int rowMax = excelSheet->rowCount();

        int columnStart = modelIndexs.first().column();
        int columnEnd = columnStart;
        int columnMax = excelSheet->columnCount();

        for (const auto& currCell : modelIndexs) {
            int row = currCell.row();
            int column = currCell.column();

            rowStart = qMin(rowStart, row);
            rowEnd = qMax(rowEnd, row);
            columnStart = qMin(columnStart, column);
            columnEnd = qMax(columnEnd, column);
        }

        // qDebug() << "CellSelectedInfo :" << modelIndexs.size();
        // qDebug() << "\t Row    :" << rowStart << rowEnd << rowMax;
        // qDebug() << "\t Column :" << columnStart << columnEnd << columnMax;
        // qDebug() << "\t Max    :" << rowMax << columnMax;

        setRowStart(rowStart);
        setRowEnd(rowEnd);
        setRowMax(rowMax);

        setColumnStart(columnStart);
        setColumnEnd(columnEnd);
        setColumnMax(columnMax);

        setModelIndexs(modelIndexs);

        setSheetIndex(0);
        setClearData(false);
        clearClearMergeInfo();
        clearSpanMergeInfo();
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
    bool isEmpty() const {
        if (getModelIndexs().isEmpty()) {
            qDebug() << "Selected cell is empty";
            return true;
        }
        return false;
    }
    bool isNotSupport() const {
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
    void clear() {
        setRowStart(0);
        setRowEnd(0);
        setRowMax(0);

        setColumnStart(0);
        setColumnEnd(0);
        setColumnMax(0);

        setModelIndexs(QModelIndexList());
    }
};

class GuiExcel : public AbstractGui {
    Q_OBJECT

    REGISTER_WRITABLE_VALUE(bool, SfcSignal, false)
    REGISTER_WRITABLE_VALUE(bool, OutputState, false)
    REGISTER_WRITABLE_VALUE(bool, CellEditSkip, false)
    REGISTER_WRITABLE_VALUE(int, CurrSheetIndex, 0)
    REGISTER_WRITABLE_VALUE(CellSelectedInfo, CopyInfo, CellSelectedInfo())
    REGISTER_WRITABLE_CONTAINER(QMap, int, bool, SheetCheckState)

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
    void updateDialogAutoCompleteVehicle();
    void updateDialogAutoCompleteConfigName();
    void updateDialogValueEnum(const QVariantList& data);
    void updateDialogTCNameResult(const QStringList& data);
    void updateDialogSelectGenType();

    bool isSheetContentChanged(const int& sheetIndex);
    bool isDrawCheckBox(const int& sheetIndex, const int& columnIndex);
    QList<QStringList> isSheetData(const int& sheetIndex, const bool& readProperty, const bool& removeMerge);
    QString isCurrentCellText(const int& sheetIndex, const int& rowIndex, const int& columnIndex);
    QMap<int, QSet<QPair<int, int>>> isPropertyMergeInfo(const int& sheetIndex);
    QMap<int, QList<QPair<int, int>>> isDisplayMergeInfo(const int& sheetIndex, const QVariantList& info = QVariantList());
    bool checkExcelSheet(const int& sheetIndex);
    QVariantList readSheetDisplay(const int& sheetIndex);
    void syncSheetData(const int& sheetIndex, const QVariantList& undoRedoData = QVariantList());
    void syncAutoComplete(const int& sheetIndex, const int& columnIndex, const QVariant& data = QVariant());
    void screenUpdateBlock(const int& sheetIndex, const bool& block);
    void updateEditSheetData(const int& sheetIndex, const QList<QStringList>& previousData, const QVariantList& data);
    int updateShortcutInsertDelete(const int& sheetIndex, const bool& insertState);
    int updateShortcutMergeSplit(const int& sheetIndex);
    int updateShortcutCopy(const int& sheetIndex, const bool& cutState);
    int updateShortcutPaste(const int& sheetIndex);
    void updateShortcutUndoRedo(const int& sheetIndex, const bool& undoState);
    void updateClearClipboard(const CellSelectedInfo& clearInfo, const bool& cellHighlight);
    void updateRowMax(const int& sheetIndex, const int& rowMax, const int& changeRowMax);
    void updateSheetHeaderAdjust(const int& sheetIndex, const bool& resizeColumn, const int& columnIndex = (-1));
    void updateSheetDefaultFocus(const int& sheetIndex, const int& row, const int& column);
    void updateCellContent(const int& sheetIndex, const int& row, const int& column);
    void updateCheckState(const int& sheetIndex, const int& columnIndex);
    void updateSheetProperty(const int& sheetIndex, const int& viewSheetIndex);
    void updateInitExcelSheet();
    void updateDescriptionInfo(const int& sheetIndex, const int& row);
    void updateAutoCompleteSignal(const bool& description, const int& columnIndex);

    void updateDisplayKey(const int& keyValue);
    void updateDisplayCellDataInfo(const int& sheetIndex, const int& row, const int& column);
    void updateDisplayAutoComplete(const int& sheetIndex, const int& row, const int& column);
    void updateDisplayAutoInputDescrtion();
    void updateDisplayGenType(const int& genType);
    void updateDisplayTCCheck(const int& allCheck);
    void updateDisplayShortcut(const int& shortcutType);
    void updateDisplayExcelSheet();

public slots:
    void slotSheetDataUndo(const int& sheetIndex, const int& row, const int& column, const QList<QStringList>& data);
    virtual void slotPropertyChanged(const int& type, const QVariant& value);

private:
    Ui::GuiExcel* mGui = nullptr;
    QWidget* mMainView = nullptr;
    QSharedPointer<Dialog> mDialog = nullptr;
    QMap<int, QTableWidget*> mExcelSheet = QMap<int, QTableWidget*>();
    QMenu* mMenuRight = nullptr;
    QMap<int, QAction*> mMenuActionItem = QMap<int, QAction*>();
    QMap<int, QPair<int, int>> mModelIndex = QMap<int, QPair<int, int>>();
    QTableWidgetItem* mSelectItem = nullptr;
    QUndoStack* mUndoStack = nullptr;
};

#endif  // GUI_EXCEL_H
