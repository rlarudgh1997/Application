#ifndef GUI_CENTER_H
#define GUI_CENTER_H

#include "AbstractGui.h"
#include "CommonUtil.h"

#include <QWidget>
#include <QTableWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QListView>
#include <QStringListModel>
#include <QButtonGroup>
#include <QRadioButton>
#include <QCheckBox>
#include <QCompleter>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QLayout>

#include "ui_GuiCenter.h"

class AutoCompleteDialog;

class ListItem : public QObject {
    Q_OBJECT

public:
    explicit ListItem(const int& index, const int& type, const QString& name, const QString& value, QWidget* parent = nullptr) {
        int posY = 5 + (index * 35);
        int height = 30;

        mTitle = ivis::common::createWidget<QPushButton>(parent, true, QRect(5, posY, 190, height), mStyleNormal);
        mTitle->setText(name);
        connect(mTitle, &QPushButton::clicked, [=]() { editValue(type, mEditState); });

        mDispaly = ivis::common::createWidget<QLabel>(parent, false, QRect(200, posY, 900, height), mStyleNormal);
        mDispaly->setFrameShape(QLabel::Shape::Panel);
        mDispaly->setIndent(2);
        mDispaly->setWordWrap(true);
        mDispaly->setText(value);
        mDispaly->show();

        mEdit = ivis::common::createWidget<QLineEdit>(parent, false, QRect(mDispaly->geometry()),
                                                      QString("color: blue; font-size: 13px"));
        mEdit->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        mEdit->setText(value);
        mEdit->hide();
        connect(mEdit, &QLineEdit::returnPressed, [=]() { editValue(type, false); });
    }
    ~ListItem() {
        clear();
    }
    void clear() {
        delete mTitle;
        delete mDispaly;
        delete mEdit;
    }
    void initStyle() {
        mTitle->setStyleSheet(mStyleNormal);
        mDispaly->setStyleSheet(mStyleNormal);
        mDispaly->show();
        mEdit->setText(mDispaly->text());
        mEdit->hide();
        mEditState = true;
    }
    void setData(const QString& name, const QString& value) {
        if (mTitle) {
            mTitle->setText(name);
        }
        if (mDispaly) {
            mDispaly->setText(value);
        }
        if (mEdit) {
            mEdit->setText(value);
        }
    }

private:
    void editValue(const int& type, const bool& editState) {
        QString style = QString();
        if (editState) {
            mTitle->setStyleSheet(mStyleEdit);
            mDispaly->hide();
            mEdit->show();
        } else {
            QString value = mEdit->text();
            if (mDispaly->text() != value) {
                mDispaly->setText(value);
                emit signalValueChanged(type, value);
            }
            mTitle->setStyleSheet(mStyleNormal);
            mDispaly->show();
            mEdit->hide();
        }
        mEditState = !editState;
    }

signals:
    void signalValueChanged(const int& type, const QVariant& value);

private:
    QString mStyleNormal = QString("color: black; font-size: 13px");
    QString mStyleEdit = QString("background-color: rgb(150, 150, 150); color: blue; font-size: 13px");
    bool mEditState = true;
    QPushButton* mTitle = nullptr;
    QLabel* mDispaly = nullptr;
    QLineEdit* mEdit = nullptr;
};

class SelectModuleDialog : public QDialog {
    Q_OBJECT

public:
    explicit SelectModuleDialog(QWidget* parent, const QStringList& moduleList, const bool& allState, const bool& cellWidthFix)
        : QDialog(parent), mSelectAllState(allState), mCellWidthFix(cellWidthFix), mSingleCheck(false) {
        init(parent);
        initWidget(moduleList);
        controlConnect();
        updateSelectModuleCheck(allState);
    }
    explicit SelectModuleDialog(QWidget* parent, const QStringList& moduleList, const bool& singleCheck)
        : QDialog(parent), mSelectAllState(false), mCellWidthFix(true), mSingleCheck(singleCheck) {
        init(parent);
        initWidget(moduleList);
        controlConnect();
        // updateSelectModuleCheck(false);
    }
    ~SelectModuleDialog() {
        delete mCurrentItem;
        mCurrentItem = nullptr;
    }
    void init(QWidget* parent) {
        setWindowTitle("Select Module");
        setWindowFlag(Qt::WindowContextHelpButtonHint, false);
        setWindowFlag(Qt::WindowCloseButtonHint, false);
        setParent(parent);
        setModal(true);

        QRect rootWidgetRect = static_cast<QWidget*>(parent->parent())->geometry();
        QRect setRect = QRect();
        setRect.setX(static_cast<int>(rootWidgetRect.x() + (rootWidgetRect.width() - mWidth) * 0.5));
        setRect.setY(static_cast<int>(rootWidgetRect.y() + (rootWidgetRect.height() - mHeight) * 0.5));
        setFixedSize(QSize(mWidth, mHeight));
        setGeometry(setRect);
        setFocus();
    }
    void initWidget(const QStringList& moduleList) {
        mLayout = new QVBoxLayout(this);
        mButtonLayout = new QHBoxLayout(mLayout->widget());
        if (mSingleCheck) {
            mOK = ivis::common::createWidget<QPushButton>(mButtonLayout->widget(), true, QRect(11, 0, 563, 30));
            mOK->setText("OK");
            mButtonLayout->addWidget(mOK);
        } else {
            mALL = ivis::common::createWidget<QPushButton>(mButtonLayout->widget(), true, QRect(11, 0, 282, 30));
            mALL->setText((mSelectAllState) ? ("Unselect All") : ("Select All"));
            mButtonLayout->addWidget(mALL);
            mOK = ivis::common::createWidget<QPushButton>(mButtonLayout->widget(), true, QRect(293, 0, 281, 30));
            mOK->setText("OK");
            mButtonLayout->addWidget(mOK);
        }

        mTableView = ivis::common::createWidget<QTableView>(this);
        QStringList subTitle = QStringList({"Module"});
        mModel.setHorizontalHeaderLabels(subTitle);
        mModel.setColumnCount(subTitle.size());
        mModel.setRowCount(moduleList.size());
        int rowIndex = 0;
        for (const auto& name : moduleList) {
            delete mModel.item(rowIndex, 0);
            mModel.setItem(rowIndex, 0, new QStandardItem(name));
            mModel.item(rowIndex, 0)->setCheckable(true);
            // mModel.item(rowIndex, 0)->setCheckState(Qt::Checked);
            mModel.item(rowIndex, 0)->setFlags(mModel.item(rowIndex, 0)->flags() & ~Qt::ItemFlag::ItemIsEditable);
            rowIndex++;
        }
        mTableView->setModel(&mModel);

        if (mCellWidthFix) {
            mTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
        } else {
            mTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
            // mTableView->resizeColumnsToContents();
        }
        mTableView->verticalHeader()->setHidden(true);
        for (rowIndex = 0; rowIndex < mModel.rowCount(); ++rowIndex) {
            mTableView->verticalHeader()->setSectionResizeMode(rowIndex, QHeaderView::Fixed);
            mTableView->verticalHeader()->resizeSection(rowIndex, 20);
        }
        mTableView->show();

        mLayout->addLayout(mButtonLayout);
        mLayout->addWidget(mTableView);
        setLayout(mLayout);
    }
    void controlConnect() {
        if ((mSingleCheck) && (mModel.rowCount() > 0)) {
            connect(&mModel, &QStandardItemModel::dataChanged,
                    [=](const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles) {
                        if (mDialogShow) {
                            int currentRowIndex = topLeft.row();  // bottomRight.row()
                            if ((mPreviousRowIndex != currentRowIndex) && (mPreviousRowIndex >= 0)) {
                                mModel.item(mPreviousRowIndex, 0)->setCheckState(Qt::Unchecked);
                            }
                            mPreviousRowIndex = currentRowIndex;
                        }
                    });
        }
        if (mALL) {
            connect(mALL, &QPushButton::clicked, [=]() {
                mSelectAllState = !mSelectAllState;
                updateSelectModuleCheck(mSelectAllState);
            });
        }
        if (mOK) {
            connect(mOK, &QPushButton::clicked, [=]() {
                QList<QPair<int, QString>> selectModule = QList<QPair<int, QString>>();
                for (int rowIndex = 0; rowIndex < mModel.rowCount(); rowIndex++) {
                    if (mModel.item(rowIndex, 0) == nullptr) {
                        continue;
                    }
                    if (mModel.item(rowIndex, 0)->checkState() == Qt::Checked) {
                        selectModule.append(QPair<int, QString>(rowIndex, mModel.item(rowIndex, 0)->text()));
                    }
                }
                emit signalModuleSelected(selectModule);
            });
        }
    }
    void updateSelectModuleCheck(const bool& allCheck) {
        if (mALL) {
            mALL->setText((allCheck) ? ("Unselect All") : ("Select All"));
        }

        for (int rowIndex = 0; rowIndex < mModel.rowCount(); rowIndex++) {
            if (mModel.item(rowIndex, 0) == nullptr) {
                continue;
            }
            mModel.item(rowIndex, 0)->setCheckState((allCheck) ? (Qt::Checked) : (Qt::Unchecked));
        }
    }
    void updateSelectModule(const QStringList& selectModuleList) {
        for (int rowIndex = 0; rowIndex < mModel.rowCount(); rowIndex++) {
            QString itemName = mModel.item(rowIndex, 0)->text();
            bool select = selectModuleList.contains(itemName);
            mModel.item(rowIndex, 0)->setCheckState((select) ? (Qt::Checked) : (Qt::Unchecked));
        }
    }
    void updateSelectListInfo(const QStringList& selectList, const QList<QStringList>& infoLists) {
        bool vehicleDataType = (infoLists.size() > 0);
        int maxCount = selectList.size();
        int columnCount = 1;
        for (const auto& infoList : infoLists) {
            columnCount++;
            maxCount = qMax(maxCount, infoList.size());
        }
        mModel.setRowCount(maxCount);
        mModel.setColumnCount((vehicleDataType) ? (5) : (1));  // SFC : ValueEnum, Vehicle : ValueEnum, EV, FCEV, ICV, System

        for (int rowIndex = 0; rowIndex < maxCount; rowIndex++) {
            if (rowIndex < selectList.size()) {
                mModel.setItem(rowIndex, 0, new QStandardItem(selectList[rowIndex]));
                mModel.item(rowIndex, 0)->setCheckable(true);
                mModel.item(rowIndex, 0)->setFlags(mModel.item(rowIndex, 0)->flags() & ~Qt::ItemFlag::ItemIsEditable);
            }

            for (int columnIndex = 0; columnIndex < infoLists.size(); columnIndex++) {
                const QStringList& infoList = infoLists[columnIndex];
                if (rowIndex < infoList.size()) {
                    int infoColumnIndex = columnIndex + 1;
                    mModel.setItem(rowIndex, infoColumnIndex, new QStandardItem(infoList[rowIndex]));
                    mModel.item(rowIndex, infoColumnIndex)
                        ->setFlags(mModel.item(rowIndex, infoColumnIndex)->flags() & ~Qt::ItemFlag::ItemIsEditable);
                }
            }

            // 높이 고정
            mTableView->verticalHeader()->setSectionResizeMode(rowIndex, QHeaderView::Fixed);
            mTableView->verticalHeader()->resizeSection(rowIndex, 20);
        }

        if (vehicleDataType) {
            // 너비 고정
            for (int columnIndex = 0; columnIndex < columnCount; columnIndex++) {
                mTableView->horizontalHeader()->setSectionResizeMode(columnIndex, QHeaderView::Fixed);
                if (columnIndex == 0) {
                    mTableView->horizontalHeader()->resizeSection(columnIndex, 240);
                } else {
                    mTableView->horizontalHeader()->resizeSection(columnIndex, 100);
                }
            }
        }
    }
    void updateSelectWidgetInfo(const QString& title, const QStringList& subTitle, const QSize& size = QSize()) {
        this->setWindowTitle(title);

        if (subTitle.isEmpty() == false) {
            mModel.setColumnCount(subTitle.size());
            mModel.setHorizontalHeaderLabels(subTitle);
        }

        if (size.isEmpty() == false) {
            QRect rootWidgetRect = this->geometry();
            QRect setRect = QRect();
            setRect.setX(static_cast<int>(rootWidgetRect.x() + (rootWidgetRect.width() - size.width()) * 0.5));
            setRect.setY(static_cast<int>(rootWidgetRect.y() + (rootWidgetRect.height() - size.height()) * 0.5));
            setFixedSize(QSize(size.width(), size.height()));
            setGeometry(setRect);
        }
    }

protected:
    void showEvent(QShowEvent* event) override {
        mDialogShow = true;
    }

signals:
    void signalModuleSelected(const QList<QPair<int, QString>>& selectModule);

private:
    const int mWidth = 600;
    const int mHeight = 800;
    QVBoxLayout* mLayout = nullptr;
    QHBoxLayout* mButtonLayout = nullptr;
    QTableView* mTableView = nullptr;
    QPushButton* mALL = nullptr;
    QPushButton* mOK = nullptr;
    QStandardItemModel mModel;
    QStandardItem* mCurrentItem = nullptr;
    bool mSelectAllState = true;
    bool mCellWidthFix = false;
    bool mSingleCheck = false;
    bool mDialogShow = false;
    int mPreviousRowIndex = (-1);
};

class GuiCenter : public AbstractGui {
    Q_OBJECT

public:
    static QSharedPointer<GuiCenter>& instance(AbstractHandler* handler = nullptr);

private:
    explicit GuiCenter(AbstractHandler* handler = nullptr);

    virtual void drawDisplayDepth0();
    virtual void drawDisplayDepth1();
    virtual void drawDisplayDepth2();
    virtual void updateDisplaySize();
    virtual void updateDisplayVisible();

    void updateDisplayConfigInfo();
    void updateDisplayNodeAddress(const int& updateType);
    void updateDisplayAutoComplete(const bool& show);
    void updateDisplaySelectModule(const bool& show);

public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value);

private:
    Ui::GuiCenter* mGui = nullptr;
    QStackedWidget* mMainView = nullptr;

    QMap<int, ListItem*> mConfigListItem = QMap<int, ListItem*>();
    AutoCompleteDialog* mAutoComplete = nullptr;
    SelectModuleDialog* mSelectModule = nullptr;
};

#endif  // GUI_CENTER_H
