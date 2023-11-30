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
#include <QListWidget>
#include <QVBoxLayout>

class AutoCompleteDialog : public QDialog {
    Q_OBJECT

public:
    explicit AutoCompleteDialog(QWidget* parent = nullptr, const QString& title = QString(),
                                const QStringList& list = QStringList())
        : QDialog(parent), mAutoCompleteStringList(list) {
        setWindowTitle(title);
        setWindowFlag(Qt::WindowContextHelpButtonHint, false);
        setWindowFlag(Qt::WindowCloseButtonHint, false);
        setParent(parent);
        setModal(true);
        setFocus();

        mInputeText = ivis::common::createWidget<QLineEdit>(this, true);
        mInputeText->setFocus();
        mSuggestionsList = ivis::common::createWidget<QListWidget>(this, true);

        QRect rootWidgetRect = static_cast<QWidget*>(parent->parent())->geometry();
        QRect setRect = QRect();
        setRect.setX(static_cast<int>(rootWidgetRect.x() + (rootWidgetRect.width() - mWidth) * 0.5));
        setRect.setY(static_cast<int>(rootWidgetRect.y() + (rootWidgetRect.height() - mHeight) * 0.5));
        setFixedSize(QSize(mWidth, mHeight));
        setGeometry(setRect);

        mLayout = new QVBoxLayout(this);
        mLayout->addWidget(mInputeText);
        mLayout->addWidget(mSuggestionsList);
        setLayout(mLayout);

        updateSuggestionsList(QString());

        connect(mInputeText, &QLineEdit::textChanged, [=](const QString& text) { updateSuggestionsList(text); });
        connect(mInputeText, &QLineEdit::returnPressed, [=]() { emit signalAutoCompleteSelectedText(inputText()); });
        connect(mSuggestionsList, &QListWidget::itemDoubleClicked, [=](QListWidgetItem* item) {
            mInputeText->clear();
            emit signalAutoCompleteSelectedText(item->text());
        });
    }
    QString inputText() {
        QString text = QString();
        if (mInputeText) {
            text = mInputeText->text();
        }
        return text;
    }
    void setInputText(const QString& text) {
        if (mInputeText) {
            mInputeText->setText(text);
        }
    }
    void setAutoCompleteStringList(const QStringList& autoCompleteStringList) {
        mAutoCompleteStringList = autoCompleteStringList;
    }

private:
    void updateSuggestionsList(const QString& text) {
        bool matching = (text.size() > 0);

        mSuggestionsList->clear();
        for (const QString& str : mAutoCompleteStringList) {
            if (matching) {
                if (str.contains(text, Qt::CaseInsensitive)) {
                    mSuggestionsList->addItem(str);
                }
            } else {
                mSuggestionsList->addItem(str);
            }
        }
        mSuggestionsList->setCurrentRow(0);
    }

signals:
    void signalAutoCompleteSelectedText(const QString& text);

private:
    const int mWidth = 900;
    const int mHeight = 500;
    QVBoxLayout* mLayout = nullptr;
    QLineEdit* mInputeText = nullptr;
    QListWidget* mSuggestionsList = nullptr;
    QStringList mAutoCompleteStringList;
};

class ExcelSheet {
public:
    ExcelSheet() {
        clear();
    }
    QMap<int, QList<QPair<int, int>>> isMergeInfo() {
        return mInfo;
    }
    bool isCellStateMerge(const int& columnIndex, const int& rowStart, const int& rowEnd) {
        if (mInfo.contains(columnIndex)) {
            for (const auto& v : mInfo[columnIndex]) {
                if ((v.first == rowStart) && (v.second == rowEnd)) {
                    // qDebug() << "\t\t Is Cell State Merge :" << columnIndex << v.first << v.second;
                    return true;
                }
            }
        }
        return false;
    }
    void insertMergeInfo(const int& columnIndex, const int& rowStart, const int& rowEnd) {
        // qDebug() << "\t\t\t Insert Merge Cell :" << columnIndex << rowStart << rowEnd;
        mInfo[columnIndex].append(QPair<int, int>(rowStart, rowEnd));
    }
    void updateMergeInfo(const int& columnIndex, const int& rowStart, const int& rowEnd) {
        QMap<int, QList<QPair<int, int>>> mergeInfo = mInfo;
        QMapIterator<int, QList<QPair<int, int>>> iter(mergeInfo);
        clear();
        while (iter.hasNext()) {
            iter.next();
            int currColumnIndex = iter.key();
            for (const auto& v : iter.value()) {
                if ((currColumnIndex == columnIndex) && (rowStart == v.first) && (rowEnd == v.second)) {
                    qDebug() << "\t\t 2. Erase Merge Cell :" << currColumnIndex << v.first << v.second;
                    continue;
                }
                qDebug() << "\t\t 1. Update Merge Cell :" << currColumnIndex << v.first << v.second;
                insertMergeInfo(currColumnIndex, v.first, v.second);
            }
        }
    }
    void clear() {
        mInfo.clear();
    }

private:
    QMap<int, QList<QPair<int, int>>> mInfo = QMap<int, QList<QPair<int, int>>>();
};

class GuiExcel : public AbstractGui {
    Q_OBJECT

private:
    enum class AutoComplete {
        Draw = 0,
        Update,
        Show,
        Hide,
        Cancel,
    };
    enum class CellMergeSplit {
        ClearSpans = 0,
        SetSpan,
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

    QVariantList readExcelSheet(const int& sheetIndex, const QVariantList& readIndexInfo, QString& allString);
    void readAllExcelSheet();
    void updateMergeInfo(const int& sheetIndex, const int& columnIndex, const int& rowStart, const int& rowEnd);
    void constructMergeInfo(const int& sheetIndex, const QMap<int, QVariantList>& sheetData);
    void updateDisplayMergeCell(const int& sheetIndex);
    void updateDisplaySheetHeaderAdjust(const int& sheetIndex);
    void updateDisplaySheetText(const int& sheetIndex);
    void updateDisplayExcelSheet();
    void updateDisplayAutoComplete(const bool& show, const int& sheetIndex, const int& rowIndex, const int& columnIndex);
    void copyClipboardInfo(const bool& cutState);
    void printMergeInfo();
    void pasteClipboardInfo();
    void updateDisplayClipboardInfo(const int& clipboardType);
    void updateDisplayShortcutInfo(const int& shortcutType);

public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value);

private:
    QTabWidget* mMainView = nullptr;
    QMap<int, QTableWidget*> mExcelSheet = QMap<int, QTableWidget*>();
    QMap<int, ExcelSheet> mMergeInfo = QMap<int, ExcelSheet>();
    QMap<CellMergeSplit, ExcelSheet> mCopyMergeSplit = QMap<CellMergeSplit, ExcelSheet>();
    QMenu* mMenuRight = nullptr;
    QMap<int, QAction*> mMenuActionItem = QMap<int, QAction*>();
    QList<int> mShortcutCutCellInfo = QList<int>();
    AutoCompleteDialog* mAutoComplete = nullptr;
    QTableWidgetItem* mSelectItem = nullptr;
    int mCurrentSheetIndex = 0;
};

#endif  // GUI_EXCEL_H
