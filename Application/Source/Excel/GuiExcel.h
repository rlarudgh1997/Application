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



#include <QDialog>
#include <QListWidget>
#include <QVBoxLayout>



class AutoCompleteDialog : public QDialog {
    Q_OBJECT

public:
    explicit AutoCompleteDialog(QWidget* parent = nullptr, const QStringList& list = QStringList())
        : QDialog(parent), mAutoCompleteStringList(list) {
        setWindowTitle("AutoComplete");
        setWindowFlag(Qt::WindowContextHelpButtonHint, false);
        setWindowFlag(Qt::WindowCloseButtonHint, false);
        setParent(parent);
        setModal(true);

        mInputeText = new QLineEdit(this);
        mSuggestionsList = new QListWidget(this);

        QVBoxLayout* layout = new QVBoxLayout;
        layout->addWidget(mInputeText);
        layout->addWidget(mSuggestionsList);

        QRect rootWidgetRect =  static_cast<QWidget*>(parent->parent())->geometry();
        QRect setRect = QRect();
        setRect.setX(static_cast<int>(rootWidgetRect.x() + (rootWidgetRect.width() - 800) * 0.5));
        setRect.setY(static_cast<int>(rootWidgetRect.y() + (rootWidgetRect.height() - 500) * 0.5));
        setRect.setWidth(800);
        setRect.setHeight(500);

        setGeometry(setRect);
        setLayout(layout);
        setFocus();
        mInputeText->setFocus();

        connect(mInputeText, &QLineEdit::textChanged, [=](const QString& text){
            mSuggestionsList->clear();
            for (const QString& str : mAutoCompleteStringList) {
                if (str.contains(text, Qt::CaseInsensitive)) {    // (str.startsWith(text, Qt::CaseInsensitive))
                    mSuggestionsList->addItem(str);
                }
            }
            mSuggestionsList->setCurrentRow(0);
        });
        connect(mSuggestionsList, &QListWidget::itemDoubleClicked, [=](QListWidgetItem *item) {
            emit signalAutoCompleteSelectedText(item->text());
            mInputeText->clear();
            mSuggestionsList->clear();
        });
    }
    void setInputText(const QString& text) {
        if (mInputeText) {
            mInputeText->setText(text);
        }
    }
    void setAutoCompleteStringList(const QStringList& autoCompleteStringList) {
        mAutoCompleteStringList = autoCompleteStringList;
    }

signals:
     void signalAutoCompleteSelectedText(const QString& text);


private:
    QLineEdit* mInputeText = nullptr;
    QListWidget* mSuggestionsList;
    QStringList mAutoCompleteStringList;
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

    QVariantList readExcelSheet(const int& sheetIndex);
    void readAllExcelSheet();
    void updateDisplayMergeCell(const int& sheetIndex);
    void updateDisplayCellInfo(const int& sheetIndex, const QVariantList& mergeInfo, const QMap<int, QVariantList>& sheetData);
    void updateDisplayExcelSheet();
    void updateDisplayAutoComplete(const bool& show, QTableWidgetItem* selectItem);


public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value);


private:
    QTabWidget* mMainView = nullptr;
    QMap<int, QTableWidget*> mExcelSheet = QMap<int, QTableWidget*>();
    QMap<int, ExcelSheet> mCellInfo = QMap<int, ExcelSheet>();
    QMap<int, QMap<int, QPair<int, int>>> mMergeInfo = QMap<int, QMap<int, QPair<int, int>>>();
    QMenu* mMenuRight = nullptr;
    QMap<MenuItemRight, QAction*> mMenuActionItem = QMap<MenuItemRight, QAction*>();
    AutoCompleteDialog* mAutoComplete = nullptr;
};

#endif    // GUI_EXCEL_H
