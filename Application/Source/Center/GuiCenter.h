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
//  #include <QCoreApplication>

class AutoCompleteDialog;

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
        : QDialog(parent), mSelectAllState(allState) {
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

        mLayout = new QVBoxLayout(this);
        mButtonLayout = new QHBoxLayout(mLayout->widget());

        mALL = ivis::common::createWidget<QPushButton>(mButtonLayout->widget(), true, QRect(11, 0, 282, 30));
        mALL->setText((mSelectAllState) ? ("Unselect All") : ("Select All"));
        mOK = ivis::common::createWidget<QPushButton>(mButtonLayout->widget(), true, QRect(293, 0, 281, 30));
        mOK->setText("OK");

        mButtonLayout->addWidget(mALL);
        mButtonLayout->addWidget(mOK);

        mTableView = ivis::common::createWidget<QTableView>(this);
        QStringList subTitle = QStringList({"Module"});  // , "PT"
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
        if (cellWidthFix) {
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

        updateSelectModuleCheck(mSelectAllState);

        connect(mALL, &QPushButton::clicked, [=]() {
            mSelectAllState = !mSelectAllState;
            updateSelectModuleCheck(mSelectAllState);
        });
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
    void updateSelectModuleCheck(const bool& allCheck) {
        mALL->setText((allCheck) ? ("Unselect All") : ("Select All"));
        for (int rowIndex = 0; rowIndex < mModel.rowCount(); rowIndex++) {
            if (mModel.item(rowIndex, 0) == nullptr) {
                continue;
            }
            mModel.item(rowIndex, 0)->setCheckState((allCheck) ? (Qt::Checked) : (Qt::Unchecked));
        }
    }
    void updateSelectModule(const QStringList& selectModuleList) {
        for (int rowIndex = 0; rowIndex < mModel.rowCount(); rowIndex++) {
            // if (mModel.item(rowIndex, 0) == nullptr) {
            //     continue;
            // }
            QString itemName = mModel.item(rowIndex, 0)->text();
            bool select = selectModuleList.contains(itemName);
            mModel.item(rowIndex, 0)->setCheckState((select) ? (Qt::Checked) : (Qt::Unchecked));
        }
    }
    void updateSelectListInfo(const QStringList& selectList, const QList<QStringList>& infoLists) {
        int maxCount = selectList.size();
        int columnCount = 1;
        for (const auto& infoList : infoLists) {
            columnCount++;
            maxCount = qMax(maxCount, infoList.size());
        }
        mModel.setRowCount(maxCount);
        mModel.setColumnCount(5);   // ValueEnum, EV, FCEV, ICV, System

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

signals:
    void signalModuleSelected(const QList<QPair<int, QString>>& selectModule);

private:
    const int mWidth = 600;
    const int mHeight = 800;
    QVBoxLayout* mLayout = nullptr;
    QHBoxLayout* mButtonLayout = nullptr;
    bool mSelectAllState = true;
    QTableView* mTableView = nullptr;
    QPushButton* mALL = nullptr;
    QPushButton* mOK = nullptr;
    QStandardItemModel mModel;
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

    template <typename T>
    void updateDisplayViewType(const int& viewType = (-1), T* widget = nullptr);
    void updateDisplayConfigInfo();
    void updateDisplayTestReport();
    void updateDisplayNodeAddress(const int& updateType);
    void updateDisplayAutoComplete(const bool& show);
    void updateDisplaySelectModule(const bool& show);

public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value);

private:
    QStackedWidget* mMainView = nullptr;
    QWidget* mConfigWidget = nullptr;
    QWidget* mReportWidget = nullptr;
    QTableWidget* mNodeAddress = nullptr;
    QMap<int, ListItem*> mConfigListItem = QMap<int, ListItem*>();
    QVariantList mConfigValue = QVariantList();
    QPushButton* mConfigHideButton = nullptr;
    QPushButton* mConfigResetButton = nullptr;
    QPushButton* mSelectModuleButton = nullptr;
    QPushButton* mNodeAddressSearch = nullptr;
    QMap<int, ReportItem*> mTestReport = QMap<int, ReportItem*>();
    AutoCompleteDialog* mAutoComplete = nullptr;
    SelectModuleDialog* mSelectModule = nullptr;
};

#endif  // GUI_CENTER_H
