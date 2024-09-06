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

class GuiExcel : public AbstractGui {
    Q_OBJECT

    REGISTER_WRITABLE_PROPERTY(bool, SfcSignal, false, false)
    REGISTER_WRITABLE_PROPERTY(bool, OutputState, false, false)

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
    void readAllExcelSheet(const int& sheetIndex, const bool& dataStorage);
    int isMergeCell(const int& sheetIndex, const int& columnIndex, const int& rowStart);
    bool updateMergeInfo(const bool& erase, const int& sheetIndex, const int& columnIndex, const int& rowStart,
                         const int& rowEnd);
    QMap<int, QList<QPair<int, int>>> findMergeInfo(const QMap<int, QVariantList>& sheetData);
    void constructMergeInfo(const int& sheetIndex, const QMap<int, QVariantList>& sheetData);
    void constructMergeSplitInfo(const QMap<int, QVariantList>& sheetData, const int& rowStart, const int& columnStart);
    void updateDisplaySplitCell(const int& sheetIndex);
    void updateDisplayMergeCell(const int& sheetIndex);
    void updateDisplaySheetHeaderAdjust(const int& sheetIndex);
    void updateDisplaySheetText(const int& sheetIndex);
    void updateCellInfoContent(const int& sheetIndex, const int& row, const int& column);
    void updateDefaultSheetFocus(const int& sheetIndex, const int& row, const int& column);
    void updateInitialExcelSheet();
    void updateDisplayKey(const int& keyValue);
    void updateDisplayArrowKey(const int& keyValue);
    void updateDisplayExcelSheet();
    void updateDisplayCellDataInfo(const int& sheetIndex, const int& row, const int& column);
    void updateDisplayAutoComplete(const int& sheetIndex, const int& row, const int& column);
    void updateDisplayAutoCompleteSignal(const bool& show, const int& columnIndex);
    void updateDisplayAutoCompleteVehicle();
    void updateDisplayValueEnum(const QVariantList& data);
    void updateDisplayAutoInputDescrtion();
    void printMergeInfo(const QString& title, const bool& mergeSplit);
    void copyClipboardInfo(const bool& cutState);
    int clearClipboardInfo(const bool& escapeKeyClear);
    void pasteClipboardInfo();
    void updateDisplayClipboardInfo(const int& clipboardType);
    void updateDisplayReceiveKeyFocus();
    void updateDisplayEditCell(const int& editType);
    void updateDescriptionInfo(const int& sheetIndex, const int& row);
    void updateTCNameInfo(const int& sheetIndex);

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
