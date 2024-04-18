#ifndef COMMON_CLASS_H
#define COMMON_CLASS_H

#if 0
#include <QObject>
// #include <QSharedPointer>
// #include <QTimerEvent>
// #include <QMap>
// #include <QVariant>
#include <QDebug>
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
#include <QTabWidget>
#include <QTableWidgetItem>
#include <QMenu>
#include <QDialog>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QProgressBar>
#include <QTextEdit>
#include <QScrollBar>
#include <QTextCharFormat>
#include <QShortcut>
#include <QInputDialog>

#include "CommonUtil.h"

namespace ivis {
namespace common {

class ReportItemInfo {
public:
    enum class Config {
        On = 0,
        Option1,
        Option2,
        Option3,
    };
    enum class Text {
        Title = 0,
        On,
        Off,
        Option,
        Option1,
        Option2,
        Option3,
        Apply,
        Cancel,
    };
};

class ListItem : public QObject {
    Q_OBJECT

public:
    explicit ListItem(const int& index, const int& type, const QString& name, const QString& value, QWidget* parent = nullptr) {
        int posY = 20 + (index * 55);
        int height = 50;

        mNameButton = ivis::common::createWidget<QPushButton>(parent, true, QRect(30, posY, 195, height), mStyleNormal);
        mNameButton->setText(name);
        connect(mNameButton, &QPushButton::clicked, [=]() { editValue(type, mEditState); });

        mValueDispaly = ivis::common::createWidget<QLabel>(parent, false, QRect(230, posY, 1000, height), mStyleNormal);
        mValueDispaly->setFrameShape(QLabel::Shape::Panel);
        mValueDispaly->setIndent(2);
        mValueDispaly->setWordWrap(true);
        mValueDispaly->setText(value);
        mValueDispaly->show();

        mValueEdit = ivis::common::createWidget<QLineEdit>(parent, false, QRect(mValueDispaly->geometry()),
                                                           QString("color: blue; font-size: 15px"));
        mValueEdit->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        mValueEdit->setText(value);
        mValueEdit->hide();
        connect(mValueEdit, &QLineEdit::returnPressed, [=]() { editValue(type, false); });
    }
    ~ListItem() {
        clear();
    }
    void clear() {
        // qDebug() << "~ListItem() ->" << mNameButton->text() << "," << mValueDispaly->text();
        delete mNameButton;
        delete mValueDispaly;
        delete mValueEdit;
    }
    void initStyle() {
        mNameButton->setStyleSheet(mStyleNormal);
        mValueDispaly->setStyleSheet(mStyleNormal);
        mValueDispaly->show();
        mValueEdit->setText(mValueDispaly->text());
        mValueEdit->hide();
        mEditState = true;
    }
    void setData(const QString& name, const QString& value) {
        if (mNameButton) {
            mNameButton->setText(name);
        }
        if (mValueDispaly) {
            mValueDispaly->setText(value);
        }
        if (mValueEdit) {
            mValueEdit->setText(value);
        }
    }

private:
    void editValue(const int& type, const bool& editState) {
        QString style = QString();
        if (editState) {
            mNameButton->setStyleSheet(mStyleEdit);
            mValueDispaly->hide();
            mValueEdit->show();
        } else {
            QString value = mValueEdit->text();
            if (mValueDispaly->text() != value) {
                mValueDispaly->setText(value);
                emit signalValueChanged(type, value);
            }
            mNameButton->setStyleSheet(mStyleNormal);
            mValueDispaly->show();
            mValueEdit->hide();
        }
        mEditState = !editState;
    }

signals:
    void signalValueChanged(const int& type, const QVariant& value);

private:
    QString mStyleNormal = QString("color: black; font-size: 15px");
    QString mStyleEdit = QString("background-color: rgb(150, 150, 150); color: blue; font-size: 15px");
    bool mEditState = true;
    QPushButton* mNameButton = nullptr;
    QLabel* mValueDispaly = nullptr;
    QLineEdit* mValueEdit = nullptr;
};

class ReportItem : public QObject {
    Q_OBJECT

public:
    ReportItem() {
        clear();
    }
    explicit ReportItem(QWidget* parent, const int& index, const bool& select) {
        initGui(parent, index, select);
    }
    ~ReportItem() {
        clear();
    }
    void updateConfig(const QMap<int, QPair<int, bool>>& config) {
        if ((mInit == false) && (config.size() == 0) && (mOn = nullptr) && (mOff = nullptr) && (mOption1 = nullptr) &&
            (mOption2 = nullptr) && (mOption3 = nullptr)) {
            return;
        }
        bool on = config[static_cast<int>(ReportItemInfo::Config::On)].second;
        mOn->setChecked(on);
        mOff->setChecked(on == false);
        mOption1->setChecked(config[static_cast<int>(ReportItemInfo::Config::Option1)].second);
        mOption2->setChecked(config[static_cast<int>(ReportItemInfo::Config::Option2)].second);
        mOption3->setChecked(config[static_cast<int>(ReportItemInfo::Config::Option3)].second);

        QMapIterator<int, QPair<int, bool>> iter(config);
        mConfigType.clear();
        while (iter.hasNext()) {
            iter.next();
            mConfigType.insert(iter.key(), iter.value().first);
        }
    }
    void updateText(const QMap<int, QString>& text) {
        if ((mInit == false) && (text.size() == 0) && (mTitle = nullptr) && (mOn = nullptr) && (mOff = nullptr) &&
            (mOption = nullptr)
            // && (mApply = nullptr)
            && (mOption1 = nullptr) && (mOption2 = nullptr) && (mOption3 = nullptr)) {
            return;
        }
        mTitle->setText(text[static_cast<int>(ReportItemInfo::Text::Title)]);
        mOn->setText(text[static_cast<int>(ReportItemInfo::Text::On)]);
        mOff->setText(text[static_cast<int>(ReportItemInfo::Text::Off)]);
        mOption->setText(text[static_cast<int>(ReportItemInfo::Text::Option)]);
        mOption1->setText(text[static_cast<int>(ReportItemInfo::Text::Option1)]);
        mOption2->setText(text[static_cast<int>(ReportItemInfo::Text::Option2)]);
        mOption3->setText(text[static_cast<int>(ReportItemInfo::Text::Option3)]);
        // mApply->setText(text[static_cast<int>(ReportItemInfo::Text::Apply)]);
    }
    void updateStatus(const bool& on) {
        if ((mInit == false) && (mOption1 = nullptr) && (mOption2 = nullptr) && (mOption3 = nullptr)) {
            return;
        }
        QString color = (on) ? (QString("black")) : (QString("gray"));
        QString colorOption = mBaseStyle.arg(color).arg("20");
        QString colorOption1 = mBaseStyle.arg(color).arg("15");
        QString colorOption2 = mBaseStyle.arg(color).arg("15");
        QString colorOption3 = mBaseStyle.arg(color).arg("15");

        mOption->setStyleSheet(colorOption);
        mOption1->setStyleSheet(colorOption1);
        mOption2->setStyleSheet(colorOption2);
        mOption3->setStyleSheet(colorOption3);

        mOption1->setEnabled(on);
        mOption2->setEnabled(on);
        mOption3->setEnabled(on);
    }
    void clear() {
        mConfigType.clear();
        delete mFrame;
        delete mTitle;
        delete mOption;
        delete mOn;
        delete mOff;
        delete mOption1;
        delete mOption2;
        delete mOption3;
        delete mGroup;
        // delete mApply;
    }

private:
    void initGui(QWidget* parent, const int& index, const bool& select) {
        if (mInit) {
            return;
        }

        mFrame = ivis::common::createWidget<QFrame>(parent, true, QRect(30, (20 + index * 210), 750, 200),
                                                    mBaseStyle.arg("balck").arg("15"));
        mFrame->setFrameShape(QFrame::Shape::Box);
        if (select) {
            mFrame->setLineWidth(3);
        } else {
            mFrame->setLineWidth(1);
        }
        mFrame->setEnabled(select);

        mTitle = ivis::common::createWidget<QLabel>(mFrame, true, QRect(100, 50, 200, 50), mBaseStyle.arg("balck").arg("20"));
        mOn = ivis::common::createWidget<QRadioButton>(mFrame, true, QRect(350, 50, 100, 50), mBaseStyle.arg("balck").arg("15"));
        mOff = ivis::common::createWidget<QRadioButton>(mFrame, true, QRect(450, 50, 100, 50), mBaseStyle.arg("balck").arg("15"));

        mGroup = new QButtonGroup(mFrame);
        mGroup->addButton(mOn, 1);
        mGroup->addButton(mOff, 0);

        mOption = ivis::common::createWidget<QLabel>(mFrame, true, QRect(130, 110, 150, 50), mBaseStyle.arg("balck").arg("20"));
        mOption1 =
            ivis::common::createWidget<QCheckBox>(mFrame, true, QRect(300, 110, 100, 50), mBaseStyle.arg("balck").arg("15"));
        mOption2 =
            ivis::common::createWidget<QCheckBox>(mFrame, true, QRect(400, 110, 100, 50), mBaseStyle.arg("balck").arg("15"));
        mOption3 =
            ivis::common::createWidget<QCheckBox>(mFrame, true, QRect(500, 110, 100, 50), mBaseStyle.arg("balck").arg("15"));

#if 0  // QT6 버전 에서 미지원
        connect(mGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), [=](int id) {
            bool onOff = (id == 1);
            updateStatus(onOff);
            emit signalReportValueChanged(static_cast<int>(ReportItemInfo::Config::On),
                                            mConfigType[static_cast<int>(ReportItemInfo::Config::On)], onOff);
        });
#else
        connect(mGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), [=](QAbstractButton* button) {
            bool onOff = (button == mOn);
            updateStatus(onOff);
            emit signalReportValueChanged(static_cast<int>(ReportItemInfo::Config::On),
                                          mConfigType[static_cast<int>(ReportItemInfo::Config::On)], onOff);
        });
#endif
        connect(mOption1, &QCheckBox::clicked, [=](bool checked) {
            emit signalReportValueChanged(static_cast<int>(ReportItemInfo::Config::Option1),
                                          mConfigType[static_cast<int>(ReportItemInfo::Config::Option1)], checked);
        });
        connect(mOption2, &QCheckBox::clicked, [=](bool checked) {
            emit signalReportValueChanged(static_cast<int>(ReportItemInfo::Config::Option2),
                                          mConfigType[static_cast<int>(ReportItemInfo::Config::Option2)], checked);
        });
        connect(mOption3, &QCheckBox::clicked, [=](bool checked) {
            emit signalReportValueChanged(static_cast<int>(ReportItemInfo::Config::Option3),
                                          mConfigType[static_cast<int>(ReportItemInfo::Config::Option3)], checked);
        });
        mInit = true;
    }

signals:
    void signalReportValueChanged(const int& index, const int& type, const bool& value);

private:
    const QString mBaseStyle = QString("color: %1; font: bold; font-size: %2px");
    bool mInit = false;
    QMap<int, int> mConfigType = QMap<int, int>();
    QFrame* mFrame = nullptr;
    QButtonGroup* mGroup = nullptr;
    QLabel* mTitle = nullptr;
    QLabel* mOption = nullptr;
    QRadioButton* mOn = nullptr;
    QRadioButton* mOff = nullptr;
    QCheckBox* mOption1 = nullptr;
    QCheckBox* mOption2 = nullptr;
    QCheckBox* mOption3 = nullptr;
    QPushButton* mApply = nullptr;
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

class SelectReportDialog : public QDialog {
    Q_OBJECT

public:
    explicit SelectReportDialog(QWidget* parent, const QString& title, const bool& state,
                                const QList<QPair<bool, QString>>& options)
        : QDialog(parent) {
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

        mRadioLayout = new QHBoxLayout(mLayout->widget());
        mOn = ivis::common::createWidget<QRadioButton>(mRadioLayout->widget(), true);
        mOn->setText("On");
        mOn->setChecked(state);
        mRadioLayout->addWidget(mOn);
        mOff = ivis::common::createWidget<QRadioButton>(mRadioLayout->widget(), true);
        mOff->setText("Off");
        mOff->setChecked(state == false);
        mRadioLayout->addWidget(mOff);
        mLayout->addLayout(mRadioLayout);

        if (options.size() > 0) {
            mCheckLayout = new QHBoxLayout(mLayout->widget());
            for (const auto& option : options) {
                int index = mOptionCheck.size();
                mOptionCheck[index] = ivis::common::createWidget<QCheckBox>(mCheckLayout->widget(), true);
                mOptionCheck[index]->setText(option.second);
                mOptionCheck[index]->setChecked(option.first);
                mCheckLayout->addWidget(mOptionCheck[index]);
            }
            mLayout->addLayout(mCheckLayout);
        }
        updateStatus(state);

        mButtonLayout = new QHBoxLayout(mLayout->widget());
        mOK = ivis::common::createWidget<QPushButton>(mButtonLayout->widget(), true);
        mOK->setText("OK");
        mCancel = ivis::common::createWidget<QPushButton>(mButtonLayout->widget(), true);
        mCancel->setText("Cancel");
        mButtonLayout->addWidget(mOK);
        mButtonLayout->addWidget(mCancel);
        mLayout->addLayout(mButtonLayout);

        setLayout(mLayout);

        // connect(mRadioLayout, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), [=](QAbstractButton* button) {
        //     updateStatus((button == mOn));
        // });
        connect(mOn, &QRadioButton::clicked, [=]() {
            mOn->setChecked(true);
            mOff->setChecked(false);
            updateStatus(true);
        });
        connect(mOff, &QRadioButton::clicked, [=]() {
            mOn->setChecked(false);
            mOff->setChecked(true);
            updateStatus(false);
        });
        connect(mOK, &QPushButton::clicked, [=]() {
            bool state = mOn->isChecked();
            QList<QPair<QString, bool>> options = QList<QPair<QString, bool>>();
            for (const auto& option : mOptionCheck) {
                qDebug() << "Option :" << option->text() << (option->checkState() == Qt::CheckState::Checked);
                options.append(QPair<QString, bool>(option->text(), option->checkState() == Qt::CheckState::Checked));
            }
            signalOptionSelected(state, options);
        });
        connect(mCancel, &QPushButton::clicked, [=]() { finished(true); });
    }

private:
    void updateStatus(const bool& on) {
        QString color = (on) ? (QString("black")) : (QString("gray"));
        QString optionStyle = mBaseStyle.arg(color).arg("12");
        for (const auto& option : mOptionCheck) {
            option->setEnabled(on);
            option->setStyleSheet(optionStyle);
        }
        mOn->setStyleSheet(mBaseStyle.arg(QString("black")).arg("12"));
        mOff->setStyleSheet(mBaseStyle.arg(QString("black")).arg("12"));
    }

signals:
    void signalOptionSelected(const bool& state, const QList<QPair<QString, bool>>& options);

private:
    // const QString mBaseStyle = QString("color: %1; font: bold; font-size: %2px");
    const QString mBaseStyle = QString("color: %1; font-size: %2px");
    const int mWidth = 400;
    const int mHeight = 100;
    const int mOnID = 1;
    const int mOffID = 0;
    QVBoxLayout* mLayout = nullptr;
    QHBoxLayout* mRadioLayout = nullptr;
    QRadioButton* mOn = nullptr;
    QRadioButton* mOff = nullptr;
    QHBoxLayout* mCheckLayout = nullptr;
    QMap<int, QCheckBox*> mOptionCheck = QMap<int, QCheckBox*>();
    QHBoxLayout* mButtonLayout = nullptr;
    QPushButton* mOK = nullptr;
    QPushButton* mCancel = nullptr;
};

class CheckBoxGroupDialog : public QDialog {
    Q_OBJECT

public:
    explicit CheckBoxGroupDialog(QWidget* parent, const QString& title, const QString& item, const QStringList& itemList)
        : QDialog(parent) {
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

        // CheckBox 1
        mCheckLayout1 = new QHBoxLayout(mLayout->widget());
        mCheckBox1 = ivis::common::createWidget<QCheckBox>(mCheckLayout1->widget(), true);
        mCheckBox1->setText(item);
        mLayout->addWidget(mCheckBox1);

        // CheckBox 2
        if (itemList.size() > 0) {
            mCheckLayout2 = new QHBoxLayout(mLayout->widget());
            for (const auto& info : itemList) {
                int index = mCheckBox2.size();
                mCheckBox2[index] = ivis::common::createWidget<QCheckBox>(mCheckLayout2->widget(), true);
                mCheckBox2[index]->setText(info);
                mCheckLayout2->addWidget(mCheckBox2[index]);
            }
            mLayout->addLayout(mCheckLayout2);
            mCheckBox1->setChecked(true);
        }

        // // Command
        // mCommand = ivis::common::createWidget<QLabel>(mLayout->widget(), true);
        // // mCommand->setText("./gen_tc.sh -c CV -m [MODULE] -o Negative");
        // mCommand->setText("./run_tc.sh -b /usr/local/bin/altonservice -c CV -d -g -m [MODULE] EV FCEV ICV");
        // mLayout->addWidget(mCommand);

        // OK, Cancel
        mButtonLayout = new QHBoxLayout(mLayout->widget());
        mOK = ivis::common::createWidget<QPushButton>(mButtonLayout->widget(), true);
        mOK->setText("OK");
        mButtonLayout->addWidget(mOK);
        mCancel = ivis::common::createWidget<QPushButton>(mButtonLayout->widget(), true);
        mCancel->setText("Cancel");
        mButtonLayout->addWidget(mCancel);
        mLayout->addLayout(mButtonLayout);

        setLayout(mLayout);

        connect(mOK, &QPushButton::clicked, [=]() {
            bool option1 = (mCheckBox1->checkState() == Qt::CheckState::Checked);
            QList<QPair<QString, bool>> checkStateList = QList<QPair<QString, bool>>();
            for (const auto& check : mCheckBox2) {
                checkStateList.append(QPair<QString, bool>(check->text(), (check->checkState() == Qt::CheckState::Checked)));
            }
            emit signalPtSelected(option1, checkStateList);
        });
        connect(mCancel, &QPushButton::clicked, [=]() { finished(true); });
    }

signals:
    void signalPtSelected(const bool& option1, const QList<QPair<QString, bool>>& checkStateList);

private:
    const int mWidth = 400;
    const int mHeight = 100;
    QVBoxLayout* mLayout = nullptr;
    QHBoxLayout* mCheckLayout1 = nullptr;
    QHBoxLayout* mCheckLayout2 = nullptr;
    QHBoxLayout* mButtonLayout = nullptr;
    QCheckBox* mCheckBox1 = nullptr;
    QMap<int, QCheckBox*> mCheckBox2 = QMap<int, QCheckBox*>();
    QPushButton* mOK = nullptr;
    QPushButton* mCancel = nullptr;
    // QLabel* mCommand = nullptr;
};

class LogDisplayDialog : public QDialog {
    Q_OBJECT

public:
    explicit LogDisplayDialog(QWidget* parent, const QString& title, const QString& info = QString(),
                              const QString& error = QString(), const QString& content = QString())
        : QDialog(parent) {
        setWindowTitle(title);
        setWindowFlag(Qt::WindowContextHelpButtonHint, false);
        setWindowFlag(Qt::WindowCloseButtonHint, false);
        // setWindowFlags(Qt::FramelessWindowHint);
        setParent(parent);
        setModal(true);

        QRect rootWidgetRect = static_cast<QWidget*>(parent->parent())->geometry();
        QRect setRect = QRect();
        setRect.setX(static_cast<int>(rootWidgetRect.x() + (rootWidgetRect.width() - mWidth) * 0.5));
        setRect.setY(static_cast<int>(rootWidgetRect.y() + (rootWidgetRect.height() - mHeight) * 0.5));
        setFixedSize(QSize(mWidth, mHeight));
        setGeometry(setRect);
        setFocus();

        mInfoLabel = ivis::common::createWidget<QTextEdit>(this, true, QRect(25, 5, 750, 80), mBaseStyle.arg(16));
        mInfoLabel->setDisabled(true);

        mContentLabel = ivis::common::createWidget<QTextEdit>(this, true, QRect(25, 90, 750, 455), mBaseStyle.arg(13));
        mContentLabel->setReadOnly(true);
        updateLogDisplay(info, error, content);

        mClose = ivis::common::createWidget<QPushButton>(this, true, QRect(0, 550, 267, 50), mBaseStyle.arg(18));
        mClose->setText("Close");
        mCancel = ivis::common::createWidget<QPushButton>(this, true, QRect(267, 550, 267, 50), mBaseStyle.arg(18));
        mCancel->setText("Cancel");
        mDetail = ivis::common::createWidget<QPushButton>(this, true, QRect(534, 550, 266, 50), mBaseStyle.arg(18));
        mDetail->setText("Detail");

        connect(mClose, &QPushButton::clicked, [=]() { emit signalTestResultClick(false); });
        connect(mCancel, &QPushButton::clicked, [=]() { emit signalTestResultClick(true); });
        connect(mDetail, &QPushButton::clicked, [=]() { emit signalDetailClicked(true); });
    }
    void updateLogDisplay(const QString& info, const QString& error, const QString& content) {
        // qDebug() << "updateLogDisplay :" << info << error << content;
        mInfoLabel->setText(info);
        int startIndex = mInfoLabel->toPlainText().indexOf(error);
        if (startIndex >= 0) {
            QTextCursor cursor = mInfoLabel->textCursor();
            cursor.setPosition(startIndex);

            QTextCharFormat charFormat;
            charFormat.setForeground(QColor("red"));
            // charFormat.setFontWeight(QFont::Bold);
            // charFormat.setFontPointSize(10);

            for (int i = 0; i < error.size() + 1; i++) {
                cursor.setPosition(startIndex + i, QTextCursor::KeepAnchor);
                cursor.setCharFormat(charFormat);
            }
        }
        mContentLabel->setText(content);
        mContentLabel->verticalScrollBar()->setValue(mContentLabel->verticalScrollBar()->maximum());
    }

signals:
    void signalTestResultClick(const bool& cancel);
    void signalDetailClicked(const bool& clicked);

private:
    const QString mBaseStyle = QString("color: blue; font-size: %1px");
    const int mWidth = 800;
    const int mHeight = 600;
    QTextEdit* mInfoLabel = nullptr;
    QTextEdit* mContentLabel = nullptr;
    QPushButton* mClose = nullptr;
    QPushButton* mCancel = nullptr;
    QPushButton* mDetail = nullptr;
};

class DetailLog : public QDialog {
    Q_OBJECT

public:
    explicit DetailLog(QWidget* parent, const QString& title, const bool& buttons = true) : QDialog(parent) {
        setWindowTitle(title);
        setWindowFlag(Qt::WindowContextHelpButtonHint, false);
        setWindowFlag(Qt::WindowCloseButtonHint, false);
        // setWindowFlags(Qt::FramelessWindowHint);
        setParent(parent);
        setModal(true);

        QRect rootWidgetRect = static_cast<QWidget*>(parent->parent())->geometry();
        QRect setRect = QRect();
        setRect.setX(static_cast<int>(rootWidgetRect.x() + (rootWidgetRect.width() - mWidth) * 0.5));
        setRect.setY(static_cast<int>(rootWidgetRect.y() + (rootWidgetRect.height() - mHeight) * 0.5));
        setFixedSize(QSize(mWidth, mHeight));
        setGeometry(setRect);
        setFocus();

        mContentLabel =
            ivis::common::createWidget<QTextEdit>(this, true, QRect(0, 0, mWidth, 550), mBaseStyle.arg("blue").arg(13));
        mContentLabel->setReadOnly(true);
        // updateLogDisplay(info, error, content);

        if (buttons) {
            mClear = ivis::common::createWidget<QPushButton>(this, true, QRect(0, 550, 400, 50), mBaseStyle.arg("blue").arg(18));
            mClear->setText("Clear");
            mStop = ivis::common::createWidget<QPushButton>(this, true, QRect(400, 550, 400, 50), mBaseStyle.arg("blue").arg(18));
            mStop->setText("Stop");

            connect(mClear, &QPushButton::clicked, [=]() { contentClear(); });
            connect(mStop, &QPushButton::clicked, [=]() {
                if (mStopState) {
                    mStop->setText("Stop");
                } else {
                    mStop->setText("Update");
                }
                mStopState = !mStopState;
            });
        }
        QRect closeRect = (buttons) ? (QRect(800, 550, 400, 50)) : (QRect(0, 550, mWidth, 50));
        mClose = ivis::common::createWidget<QPushButton>(this, true, closeRect, mBaseStyle.arg("black").arg(18));
        mClose->setText("Close");
        connect(mClose, &QPushButton::clicked, [=]() {
            updateFindWidget(false);
            emit signalCloseClicked(true);
        });

        mShortcut = new QShortcut(QKeySequence(QString("Ctrl+F")), mContentLabel);
        connect(mShortcut, &QShortcut::activated, [=]() {
            // QString searchText = QInputDialog::getText(nullptr, "Find", "Enter text to find:");
            // updateFindTextCursor(false, searchText);
            updateFindWidget(!mFindInputTextVisible);
        });
        mFindWidget = ivis::common::createWidget<QWidget>(this, mFindInputTextVisible, QRect(0, 0, mWidth, 30));
        mFindInputText =
            ivis::common::createWidget<QTextEdit>(mFindWidget, true, QRect(200, 0, 800, 30), mBaseStyle.arg("black").arg(15));
        mFindPrevious =
            ivis::common::createWidget<QPushButton>(mFindWidget, true, QRect(0, 0, 200, 30), mBaseStyle.arg("black").arg(15));
        mFindPrevious->setText("Previous");
        mFindNext =
            ivis::common::createWidget<QPushButton>(mFindWidget, true, QRect(1000, 0, 200, 30), mBaseStyle.arg("black").arg(15));
        mFindNext->setText("Next");
        connect(mFindInputText, &QTextEdit::textChanged, [=]() {
            QString inputText = mFindInputText->toPlainText();
            if (inputText.contains("\n")) {
                inputText.remove("\n");
                mFindInputText->setText(inputText);
                QTextCursor cursor = mFindInputText->textCursor();
                cursor.movePosition(QTextCursor::End);
                mFindInputText->setTextCursor(cursor);
                mFindInputText->setAlignment(Qt::AlignCenter);
            }
        });
        connect(mFindPrevious, &QPushButton::clicked, [=]() { updateFindTextCursor(true); });
        connect(mFindNext, &QPushButton::clicked, [=]() { updateFindTextCursor(false); });
    }
    void updateLogDisplay(const QStringList& data) {
        // QString text = mContent;
        QString text = QString();
        for (const auto& d : data) {
            text.append(d + "\n");
        }
        if (mStopState == false) {
            // mContentLabel->setText(text);
            mContentLabel->insertPlainText(text);
            mContentLabel->verticalScrollBar()->setValue(mContentLabel->verticalScrollBar()->maximum());
        }
        mContent = text;
    }
    void contentClear() {
        mContent.clear();
        mContentLabel->clear();
    }
    void updateFindWidget(const bool& visible) {
        QRect contentLabelRect = (visible) ? (QRect(0, 30, mWidth, 520)) : (QRect(0, 0, mWidth, 550));
        if (mContentLabel) {
            mContentLabel->setGeometry(contentLabelRect);
            // mContentLabel->verticalScrollBar()->setValue(mContentLabel->verticalScrollBar()->maximum());
        }
        if (mFindWidget) {
            if (mFindInputText) {
                mFindInputText->clear();
                // mFindInputText->setWordWrapMode(QTextOption::WrapMode::NoWrap);
                // mFindInputText->setLineWrapMode(QTextEdit::LineWrapMode::NoWrap);
                mFindInputText->setAlignment(Qt::AlignCenter);
                mFindInputText->setFocus(Qt::MouseFocusReason);
            }
            mFindWidget->setVisible(visible);
        }
        mFindInputTextVisible = visible;
    }
    void updateFindTextCursor(const bool& searchPrevious, const QString& inputSerachText = QString()) {
        if (mContentLabel == nullptr) {
            return;
        }
        QString searchText = (inputSerachText.size() == 0) ? (mFindInputText->toPlainText()) : (inputSerachText);
        QTextCursor cursor = mContentLabel->textCursor();
        QTextDocument::FindFlags flags = (searchPrevious) ? (QTextDocument::FindBackward) : (QTextDocument::FindFlags());
        cursor = mContentLabel->document()->find(searchText, cursor, flags);
        if (!cursor.isNull()) {
            mContentLabel->setTextCursor(cursor);
            mContentLabel->ensureCursorVisible();
        }
    }

signals:
    void signalCloseClicked(const bool& clicked);

private:
    const QString mBaseStyle = QString("color: %1; font-size: %2px");
    const int mWidth = 1200;
    const int mHeight = 600;
    QTextEdit* mContentLabel = nullptr;
    QPushButton* mClear = nullptr;
    QPushButton* mStop = nullptr;
    QPushButton* mClose = nullptr;
    QShortcut* mShortcut = nullptr;
    QWidget* mFindWidget = nullptr;
    QTextEdit* mFindInputText = nullptr;
    QPushButton* mFindPrevious = nullptr;
    QPushButton* mFindNext = nullptr;
    QString mContent = QString();
    bool mStopState = false;
    bool mPressCtrl = false;
    bool mFindInputTextVisible = false;
};

class TextEnterDialog : public QDialog {
    Q_OBJECT

public:
    explicit TextEnterDialog(QWidget* parent, const QString& text = QString()) : QDialog(parent) {
        setWindowTitle("Enter Script Text");
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

        mInputText = ivis::common::createWidget<QLineEdit>(this, true);
        mInputText->setText(text);

        mButtonLayout = new QHBoxLayout(mLayout->widget());
        mOK = ivis::common::createWidget<QPushButton>(mButtonLayout->widget(), true);
        mOK->setText("OK");
        mCancel = ivis::common::createWidget<QPushButton>(mButtonLayout->widget(), true);
        mCancel->setText("Cancel");
        mButtonLayout->addWidget(mOK);
        mButtonLayout->addWidget(mCancel);

        mLayout->addWidget(mInputText);
        mLayout->addLayout(mButtonLayout);
        setLayout(mLayout);

        connect(mOK, &QPushButton::clicked, [=]() { emit signalEnterTextCompletd(mInputText->text()); });
        connect(mCancel, &QPushButton::clicked, [=]() { finished(true); });
    }

signals:
    void signalEnterTextCompletd(const QString& text);

private:
    const int mWidth = 500;
    const int mHeight = 100;
    QVBoxLayout* mLayout = nullptr;
    QLineEdit* mInputText = nullptr;
    QHBoxLayout* mButtonLayout = nullptr;
    QPushButton* mOK = nullptr;
    QPushButton* mCancel = nullptr;
};

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

}  // end of namespace common
}  // end of namespace ivis
#endif

#endif  // COMMON_CLASS_H
