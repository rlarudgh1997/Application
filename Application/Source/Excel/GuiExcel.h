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

using QPairInt = QPair<int, int>;

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
    void updateDialogAutoCompleteVehicle();
    void updateDialogAutoCompleteConfigName();
    void updateDialogValueEnum(const QVariantList& data);
    void updateDialogTCNameResult(const QStringList& data);
    void updateDialogSelectGenType();

    bool isSheetContentChanged(const int& sheetIndex);
    bool isDrawCheckBox(const int& sheetIndex, const int& columnIndex);
    QList<QStringList> isSheetData(const int& sheetIndex, const bool& removeMerge, const bool& readProperty);
    QString isCurrentCellText(const int& sheetIndex, const int& rowIndex, const int& columnIndex);
    QMap<int, QSet<QPair<int, int>>> isPropertyMergeInfo(const int& sheetIndex);
    QMap<int, QList<QPair<int, int>>> isDisplayMergeInfo(const int& sheetIndex, const QVariantList& info);
    bool chcekExcelSheet(const int& sheetIndex);
    QVariantList readSheetDisplay(const int& sheetIndex);
    void syncSheetData(const int& sheetIndex, const bool& readProperty);
    void syncAutoComplete(const int& sheetIndex, const int& columnIndex, const QVariant& data = QVariant());
    int updateShortcutInsertDelete(const int& sheetIndex, const bool& insert);
    int updateShortcutMergeSplit(const int& sheetIndex);
    int updateShortcutCopy(const int& sheetIndex, const bool& cutState);
    int updateShortcutPaste(const int& sheetIndex);
    void updateClearClipboard(const CellSelectedInfo& clearInfo, const bool& hilight);
    void updateRowMax(const int& sheetIndex, const int& rowMax, const int& changeRowMax);
    void updateSheetHeaderAdjust(const int& sheetIndex, const bool& resizeColumn);
    void updateSheetDefaultFocus(const int& sheetIndex, const int& row, const int& column);
    void updateCellContent(const int& sheetIndex, const int& row, const int& column);
    void updateSheetProperty(const int& sheetIndex);
    void updateSheetNew(const int& sheetIndex, const int& rowMax, const int& columnMax);
    void updateInitExcelSheet();
    void updateDescriptionInfo(const int& sheetIndex, const int& row);
    void updateAutoCompleteSignal(const bool& description, const int& columnIndex);

    void updateDisplayKey(const int& keyValue);
    void updateDisplayArrowKey(const int& keyValue);
    void updateDisplayCellDataInfo(const int& sheetIndex, const int& row, const int& column);
    void updateDisplayAutoComplete(const int& sheetIndex, const int& row, const int& column);
    void updateDisplayAutoInputDescrtion();
    void updateDisplayGenType(const int& genType);
    void updateDisplayTCCheck(const int& allCheck);
    void updateDisplayReceiveKeyFocus();
    void updateDisplayShortcut(const int& shortcutType);
    void updateDisplaySheetCheckState(const int& sheetIndex, const int& columnIndex);
    void updateDisplayExcelSheet();

public slots:
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

    // const QString mEmptyString = QString("[Empty]");
};

#endif  // GUI_EXCEL_H
