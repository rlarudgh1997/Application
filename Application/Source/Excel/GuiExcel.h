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

// #include "ui_GuiExcel.h"

class SelectModuleDialog;

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
        connect(mInputeText, &QLineEdit::returnPressed, [=]() {
            QString text = inputText();
            emit signalAutoCompleteSelectedText(text);
        });
        connect(mSuggestionsList, &QListWidget::itemDoubleClicked, [=](QListWidgetItem* item) {
            if (item) {
                QString text = item->text();
                emit signalAutoCompleteSelectedText(text);
                mInputeText->clear();
            }
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

class AutoCompleteVehicleDialog : public QDialog {
    Q_OBJECT

public:
    explicit AutoCompleteVehicleDialog(QWidget* parent, const QString& title, QStringList& itemList) : QDialog(parent) {
        setWindowTitle(title);
        setWindowFlag(Qt::WindowContextHelpButtonHint, true);
        setWindowFlag(Qt::WindowCloseButtonHint, true);
        setParent(parent);
        setModal(true);

        QRect rootWidgetRect = static_cast<QWidget*>(parent->parent())->geometry();
        QRect setRect = QRect();
        setRect.setX(static_cast<int>(rootWidgetRect.x() + (rootWidgetRect.width() - mWidth) * 0.5));
        setRect.setY(static_cast<int>(rootWidgetRect.y() + (rootWidgetRect.height() - mHeight) * 0.5));
        setFixedSize(QSize(mWidth, mHeight));
        setGeometry(setRect);
        setFocus();

        mLayout = new QVBoxLayout(this);

        if (itemList.size() > 0) {
            mCheckLayout = new QHBoxLayout(mLayout->widget());
            for (const auto& info : itemList) {
                int index = mCheckBox.size();
                mCheckBox[index] = ivis::common::createWidget<QCheckBox>(mCheckLayout->widget(), true);
                mCheckBox[index]->setText(info);
                mCheckLayout->addWidget(mCheckBox[index]);
            }
            mLayout->addLayout(mCheckLayout);
        }

        mButtonLayout = new QHBoxLayout(mLayout->widget());
        mOK = ivis::common::createWidget<QPushButton>(mButtonLayout->widget(), true);
        mOK->setText("OK");
        mCancel = ivis::common::createWidget<QPushButton>(mButtonLayout->widget(), true);
        mCancel->setText("Cancel");
        mButtonLayout->addWidget(mOK);
        mButtonLayout->addWidget(mCancel);
        mLayout->addLayout(mButtonLayout);

        setLayout(mLayout);

        connect(mOK, &QPushButton::clicked, [=]() {
            QString vehicleTypeText = QString();
            for (const auto& check : mCheckBox) {
                if (check->checkState() == Qt::CheckState::Checked) {
                    if (vehicleTypeText.size() > 0) {
                        vehicleTypeText.append(", ");
                    }
                    vehicleTypeText.append(check->text());
                }
            }
            emit signalSelecteCheck(vehicleTypeText);
        });
        connect(mCancel, &QPushButton::clicked, [=]() { finished(true); });
    }

signals:
    void signalSelecteCheck(const QString& vehicleTypeText);

private:
    const int mWidth = 400;
    const int mHeight = 100;
    QVBoxLayout* mLayout = nullptr;
    QHBoxLayout* mCheckLayout = nullptr;
    QHBoxLayout* mButtonLayout = nullptr;
    QMap<int, QCheckBox*> mCheckBox = QMap<int, QCheckBox*>();
    QPushButton* mOK = nullptr;
    QPushButton* mCancel = nullptr;
};

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

    bool chcekExcleSheet(const int& sheetIndex);
    QVariantList readExcelSheet(const int& sheetIndex, const QVariantList& readIndexInfo, QString& allString);
    void readAllExcelSheet();
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
    void updateDefaultSheetFocus(const int& sheetIndex, const int& row, const int& column);
    void updateInitialExcelSheet();
    void updateDisplayKey(const int& keyValue);
    void updateDisplayExcelSheet();
    void updateDisplayAutoComplete(const bool& show, const int& columnIndex);
    void updateDisplayAutoCompleteVehicle();
    void updateDisplayAutoCompleteInputData(const bool& sfcSignal, const bool& outputState);
    void printMergeInfo(const QString& title, const bool& mergeSplit);
    void copyClipboardInfo(const bool& cutState);
    int clearClipboardInfo(const bool& escapeKeyClear);
    void pasteClipboardInfo();
    void updateDisplayClipboardInfo(const int& clipboardType);
    void updateDisplayReceiveKeyFocus();
    void updateDisplayEditCell(const int& editType);

public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value);

private:
    // Ui::GuiExcel* mGui = nullptr;
    QTabWidget* mMainView = nullptr;

    QMap<int, QTableWidget*> mExcelSheet = QMap<int, QTableWidget*>();
    QMap<int, ExcelSheet> mMergeInfo = QMap<int, ExcelSheet>();
    ExcelSheet mCopyMergeInfo = ExcelSheet();
    ExcelSheet mClearMergeInfo = ExcelSheet();
    QList<int> mClearCellInfo = QList<int>();
    QMenu* mMenuRight = nullptr;
    QMap<int, QAction*> mMenuActionItem = QMap<int, QAction*>();
    AutoCompleteDialog* mAutoComplete = nullptr;
    AutoCompleteVehicleDialog* mAutoCompleteVehicle = nullptr;
    SelectModuleDialog* mAutoCompleteInputData = nullptr;
    QTableWidgetItem* mSelectItem = nullptr;
    int mCurrentSheetIndex = 0;
};

#endif  // GUI_EXCEL_H
