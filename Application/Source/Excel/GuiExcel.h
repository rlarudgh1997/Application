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
    void updateDisplayExcelSheet();
    QVariantList readExcelSheet(const int& sheetIndex);
    void readAllExcelSheet();
#else
    void updateDisplaySheetInfo(const int& type);
    void readExcelInfo();
#endif


public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value);


private:
    QTabWidget* mMainView = nullptr;
#if defined(USE_EXCEL_FUNCTION_NEW)
    QMap<QVariant, QTableWidget*> mExcelSheet = QMap<QVariant, QTableWidget*>();
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
