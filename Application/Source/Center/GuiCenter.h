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
    ListItem() {}
    explicit ListItem(const int& index, const int& type, const QString& name, const QString& value, QWidget* parent = nullptr) {
        int posY = 20 + (index * 55);
        int height = 50;

        mNameButton = new QPushButton(parent);
        mNameButton->setGeometry(30, posY, 195, height);
        mNameButton->setStyleSheet(mStyleNormal);
        mNameButton->setText(name);
        mNameButton->show();
        connect(mNameButton, &QPushButton::clicked, [=]() {
            editValue(type, mEditState);
        });

        mValueDispaly = new QLabel(parent);
        mValueDispaly->setGeometry(230, posY, 1000, height);
        mValueDispaly->setStyleSheet(mStyleNormal);
        mValueDispaly->setFrameShape(QLabel::StyledPanel);
        mValueDispaly->setIndent(2);
        mValueDispaly->setWordWrap(true);
        mValueDispaly->setText(value);
        mValueDispaly->show();

        mValueEdit = new QLineEdit(parent);
        mValueEdit->setGeometry(mValueDispaly->geometry());
        mValueEdit->setStyleSheet(QString("color: blue; font-size: 15px"));
        mValueEdit->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        // QRegExp rx("(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])");
        // QRegExpValidator *validator = new QRegExpValidator(rx,this);
        // mValueEdit->setValidator(validator);
        mValueEdit->setText(value);
        mValueEdit->hide();
        connect(mValueEdit, &QLineEdit::returnPressed, [=]() {
            editValue(type, false);
        });
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
            mValueEdit->show();
        } else {
            QString value = mValueEdit->text();
            if (mValueDispaly->text() != value) {
                mValueDispaly->setText(value);
                emit signalValueChanged(type, value);
            }
            mNameButton->setStyleSheet(mStyleNormal);
            mValueEdit->hide();
        }
        mEditState =!editState;
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
        if ((mInit == false) && (config.size() == 0) && (mOn = nullptr) && (mOff = nullptr)
            && (mOption1 = nullptr) && (mOption2 = nullptr) && (mOption3 = nullptr)) {
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
        if ((mInit == false) && (text.size() == 0)
            && (mTitle = nullptr) && (mOn = nullptr) && (mOff = nullptr) && (mOption = nullptr)
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

        mFrame   = ivis::common::createWidget<QFrame>(parent,       mBaseStyle.arg("balck").arg("15"),
                                                                    QRect(30, (20 + index * 210), 750, 200));
                                                                    // QRect(30, (20 + index * 310), 750, 300));
        mFrame->setFrameShape(QFrame::Shape::Box);
        if (select) {
            mFrame->setLineWidth(3);
        } else {
            mFrame->setLineWidth(1);
        }
        mFrame->setEnabled(select);

        mTitle   = ivis::common::createWidget<QLabel>(mFrame,       mBaseStyle.arg("balck").arg("20"), QRect(100, 50, 200, 50));
        mOn      = ivis::common::createWidget<QRadioButton>(mFrame, mBaseStyle.arg("balck").arg("15"), QRect(350, 50, 100, 50));
        mOff     = ivis::common::createWidget<QRadioButton>(mFrame, mBaseStyle.arg("balck").arg("15"), QRect(450, 50, 100, 50));

        mGroup   = new QButtonGroup(mFrame);
        mGroup->addButton(mOn, 1);
        mGroup->addButton(mOff, 0);

        mOption  = ivis::common::createWidget<QLabel>(mFrame,       mBaseStyle.arg("balck").arg("20"), QRect(130, 110, 150, 50));
        mOption1 = ivis::common::createWidget<QCheckBox>(mFrame,    mBaseStyle.arg("balck").arg("15"), QRect(300, 110, 100, 50));
        mOption2 = ivis::common::createWidget<QCheckBox>(mFrame,    mBaseStyle.arg("balck").arg("15"), QRect(400, 110, 100, 50));
        mOption3 = ivis::common::createWidget<QCheckBox>(mFrame,    mBaseStyle.arg("balck").arg("15"), QRect(500, 110, 100, 50));
        // mApply = ivis::common::createWidget<QPushButton>(mFrame,  mBaseStyle.arg("balck").arg("15"), QRect(250, 200, 250, 50));

        connect(mGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), [=](int id) {
            updateStatus(id == 1);
            emit signalReportValueChanged(static_cast<int>(ReportItemInfo::Config::On),
                                            mConfigType[static_cast<int>(ReportItemInfo::Config::On)], (id == 1));
        });
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
        // connect(mApply, &QPushButton::clicked, [=]() {
        //     emit signalReportValueChanged(static_cast<int>(ReportItemInfo::Config::On),
        //                                     mConfigType[static_cast<int>(ReportItemInfo::Config::On)],
        //                                     (mGroup->checkedId() == 1));
        //     emit signalReportValueChanged(static_cast<int>(ReportItemInfo::Config::Option1),
        //                                     mConfigType[static_cast<int>(ReportItemInfo::Config::Option1)],
        //                                     mOption1->isChecked());
        //     emit signalReportValueChanged(static_cast<int>(ReportItemInfo::Config::Option2),
        //                                     mConfigType[static_cast<int>(ReportItemInfo::Config::Option2)],
        //                                     mOption2->isChecked());
        //     emit signalReportValueChanged(static_cast<int>(ReportItemInfo::Config::Option3),
        //                                     mConfigType[static_cast<int>(ReportItemInfo::Config::Option3)],
        //                                     mOption3->isChecked());
        // });

        mInit = true;
    }

signals:
    void signalReportValueChanged(const int& index, const int& type, const bool& value);

private:
    bool mInit = false;
    QMap<int, int> mConfigType = QMap<int, int>();
    QString mBaseStyle = QString("color: %1; font: bold; font-size: %2px");
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
    void updateDisplayNodeAddress();
    void updateDisplayTestReport();


public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value);


private:
    QStackedWidget* mMainView = nullptr;
    QWidget* mConfigWidget = nullptr;
    QListView* mNodeAddressList = nullptr;
    QWidget* mReportWidget = nullptr;
    QMap<int, ListItem*> mConfigListItem = QMap<int, ListItem*>();
    QVariantList mConfigValue = QVariantList();
    QLineEdit* mInputNodeAddress = nullptr;
    QPushButton* mConfigHideButton = nullptr;
    QPushButton* mConfigResetButton = nullptr;
    QMap<int, ReportItem*> mTestReport = QMap<int, ReportItem*>();
};

#endif    // GUI_CENTER_H
