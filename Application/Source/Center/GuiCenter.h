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
    void updateConfig(const QMap<int, bool>& config) {
        if ((mInit == false) && (config.size() == 0)
            && (mOn = nullptr) && (mOff = nullptr)
            && (mOption1 = nullptr) && (mOption2 = nullptr) && (mOption3 = nullptr)) {
            return;
        }

        bool on = config[static_cast<int>(ReportItemInfo::Config::On)];
        mOn->setChecked(on);
        mOff->setChecked(on == false);
        mOption1->setChecked(config[static_cast<int>(ReportItemInfo::Config::Option1)]);
        mOption1->setEnabled(on);
        mOption2->setChecked(config[static_cast<int>(ReportItemInfo::Config::Option2)]);
        mOption2->setEnabled(on);
        mOption3->setChecked(config[static_cast<int>(ReportItemInfo::Config::Option3)]);
        mOption3->setEnabled(on);
    }
    void updateText(const QMap<int, QString>& text, const bool& on) {
        if ((mInit == false) && (text.size() == 0)
            && (mTitle = nullptr) && (mOn = nullptr) && (mOff = nullptr) && (mOption = nullptr)
            && (mOption1 = nullptr) && (mOption2 = nullptr) && (mOption3 = nullptr)
            && (mApply = nullptr) && (mCancel = nullptr)) {
            return;
        }

        mTitle->setText(text[static_cast<int>(ReportItemInfo::Text::Title)]);
        mOn->setText(text[static_cast<int>(ReportItemInfo::Text::On)]);
        mOff->setText(text[static_cast<int>(ReportItemInfo::Text::Off)]);
        mOption->setText(text[static_cast<int>(ReportItemInfo::Text::Option)]);
        mOption1->setText(text[static_cast<int>(ReportItemInfo::Text::Option1)]);
        mOption1->setEnabled(on);
        mOption2->setText(text[static_cast<int>(ReportItemInfo::Text::Option2)]);
        mOption2->setEnabled(on);
        mOption3->setText(text[static_cast<int>(ReportItemInfo::Text::Option3)]);
        mOption3->setEnabled(on);
        mApply->setText(text[static_cast<int>(ReportItemInfo::Text::Apply)]);
        mCancel->setText(text[static_cast<int>(ReportItemInfo::Text::Cancel)]);
    }
    void clear() {
        delete mFrame;
        delete mTitle;
        delete mOption;
        delete mOn;
        delete mOff;
        delete mOption1;
        delete mOption2;
        delete mOption3;
        delete mGroup;
        delete mApply;
        delete mCancel;
    }

private:
    void initGui(QWidget* parent, const int& index, const bool& select) {
        if (mInit) {
            return;
        }

        mFrame   = ivis::common::createWidget<QFrame>(parent,       QString("color: black; font: bold; font-size:15px"),
                                                                    QRect(30, (20 + index * 310), 750, 300));
        mFrame->setFrameShape(QFrame::Shape::Box);
        if (select) {
            mFrame->setLineWidth(3);
        } else {
            mFrame->setLineWidth(1);
        }
        mFrame->setEnabled(select);

        mTitle   = ivis::common::createWidget<QLabel>(mFrame,       QString("color: black; font: bold; font-size:20px"),
                                                                    QRect(100, 50, 200, 50));
        mOn      = ivis::common::createWidget<QRadioButton>(mFrame, QString("color: black; font: bold; font-size:15px"),
                                                                    QRect(350, 50, 100, 50));
        mOff     = ivis::common::createWidget<QRadioButton>(mFrame, QString("color: black; font: bold; font-size:15px"),
                                                                    QRect(450, 50, 100, 50));

        mGroup   = new QButtonGroup(mFrame);
        mGroup->addButton(mOn, 1);
        mGroup->addButton(mOff, 0);

        mOption  = ivis::common::createWidget<QLabel>(mFrame,       QString("color: black; font: bold; font-size:20px"),
                                                                    QRect(130, 110, 150, 50));
        mOption1 = ivis::common::createWidget<QCheckBox>(mFrame,    QString("color: black; font: bold; font-size:15px"),
                                                                    QRect(300, 110, 100, 50));
        mOption2 = ivis::common::createWidget<QCheckBox>(mFrame,    QString("color: black; font: bold; font-size:15px"),
                                                                    QRect(400, 110, 100, 50));
        mOption3 = ivis::common::createWidget<QCheckBox>(mFrame,    QString("color: black; font: bold; font-size:15px"),
                                                                    QRect(500, 110, 100, 50));

        mApply   = ivis::common::createWidget<QPushButton>(mFrame,  QString("color: black; font: bold; font-size:20px"),
                                                                    QRect(150, 200, 200, 50));
        mCancel  = ivis::common::createWidget<QPushButton>(mFrame,  QString("color: black; font: bold; font-size:20px"),
                                                                    QRect(400, 200, 200, 50));

        connect(mApply, &QPushButton::clicked, [=]() {
            QMap<int, bool> config = QMap<int, bool>();
            config[static_cast<int>(ReportItemInfo::Config::On)] = (mGroup->checkedId() == 1);
            config[static_cast<int>(ReportItemInfo::Config::Option1)] = mOption1->isChecked();
            config[static_cast<int>(ReportItemInfo::Config::Option2)] = mOption2->isChecked();
            config[static_cast<int>(ReportItemInfo::Config::Option3)] = mOption3->isChecked();
            emit signalReportItemValueChanged(config);
        });
        connect(mCancel, &QPushButton::clicked, [=]() {
            emit signalReportItemValueChanged(QMap<int, bool>());
        });
        // connect(mGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), [=](int id) {
        //     qDebug() << "buttonClicked :" << id;
        // });
        // connect(mOption1, QOverload<int>::of(&QCheckBox::stateChanged), [=](int state) {
        //     qDebug() << "1 stateChanged :" << state;
        // });

        mInit = true;
    }

signals:
    void signalReportItemValueChanged(const QMap<int, bool>& config);

private:
    bool mInit = false;
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
    QPushButton* mCancel = nullptr;
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
    // QTabWidget* mMainView = nullptr;
    // QTableWidget* mMainView = nullptr;
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
