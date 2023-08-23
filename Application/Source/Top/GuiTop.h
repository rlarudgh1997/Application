#ifndef GUI_TOP_H
#define GUI_TOP_H

#include "AbstractGui.h"

#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStackedWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QListView>


#define USE_LIST_ITEM_EDIT
class ListItem : public QObject {
    Q_OBJECT

public:
    ListItem() {}
    ListItem(const int& index, const int& type, const QString& name, const QString& value, QWidget* parent = nullptr) {
        QFont font;
        font.setPixelSize(15);
        font.setBold(true);
        int posY = 30 + (index * 55);
        int height = 50;

        mName = new QLabel(parent);
        mName->setGeometry(30, posY, 195, height);
        mName->setFont(font);
        mName->setFrameShape(QLabel::StyledPanel);
        mName->setText(name);
        mName->show();

        font.setBold(false);
        mDisplayValue = new QLabel(parent);
        mDisplayValue->setGeometry(230, posY, 895, height);
        mDisplayValue->setFont(font);
        mDisplayValue->setFrameShape(QLabel::StyledPanel);
        mDisplayValue->setWordWrap(true);
        mDisplayValue->setText(value);
        mDisplayValue->show();

#if defined(USE_LIST_ITEM_EDIT)
        mEditButton = new QPushButton(parent);
        mEditButton->setGeometry(1130, posY, height, height);
        mEditButton->setFont(font);
        mEditButton->setText("Edit");
        mEditButton->show();
        connect(mEditButton, &QPushButton::clicked, [=]() {
            editValue(index, type, mEditButton->text());
        });

        mEditValue = new QLineEdit(parent);
        mEditValue->setGeometry(230, posY, 895, height);
        mEditValue->setFont(font);
        mEditValue->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        // QRegExp rx("(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])");
        // QRegExpValidator *validator = new QRegExpValidator(rx,this);
        // mEditValue->setValidator(validator);
        mEditValue->setText(value);
        mEditValue->hide();
        connect(mEditValue, &QLineEdit::returnPressed, [=]() {
            editValue(index, type, "OK");
        });
        connect(mEditValue, &QLineEdit::inputRejected, [=]() {
            qDebug() << "inputRejected";
        });

#endif
    }
    ~ListItem() {
        clear();
    }
    void clear() {
        qDebug() << "~ListItem() ->" << mName->text() << "," << mDisplayValue->text();
        delete mName;
        delete mDisplayValue;
#if defined(USE_LIST_ITEM_EDIT)
        delete mEditButton;
        delete mEditValue;
#endif
    }
#if defined(USE_LIST_ITEM_EDIT)
    void editValue(const int& index, const int& type, const QString& buttonText) {
        if (buttonText.compare("Edit") == false) {
            mDisplayValue->hide();
            mEditValue->show();
            mEditButton->setText("Save");
        } else {
            mDisplayValue->show();
            mEditValue->hide();
            mEditButton->setText("Edit");
            QString value = mEditValue->text();
            if (mDisplayValue->text() != value) {
                mDisplayValue->setText(value);
                emit signalValueChanged(index, type, value);
            }
        }
    }
#endif

signals:
    void signalValueChanged(const int& index, const int& type, const QVariant& value);

private:
    QLabel* mName = nullptr;
    QLabel* mDisplayValue = nullptr;
#if defined(USE_LIST_ITEM_EDIT)
    QPushButton* mEditButton = nullptr;
    QLineEdit* mEditValue = nullptr;
#endif
};


class GuiTop : public AbstractGui {
    Q_OBJECT

private:
    enum class MainType {
        File = 0,
        Edit,
        View,
        Setting,
        Help,
        Max,
    };


public:
    static QSharedPointer<GuiTop>& instance(AbstractHandler* handler = nullptr);


private:
    explicit GuiTop(AbstractHandler* handler = nullptr);

    virtual void drawDisplayDepth0();
    virtual void drawDisplayDepth1();
    virtual void drawDisplayDepth2();
    virtual void updateDisplaySize();
    virtual void updateDisplayVisible();

    void drawMenuFile();
    void drawMenuEdit();
    void drawMenuView();
    void drawMenuSetting();
    void drawMenuHelp();
    void drawMenuEtc();
    void updateDisplayDefaultPath();
    void updateDisplayConfigInfo();
    void updateDisplayNodeAddress();


public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value);


private:
    QMainWindow* mMainWindow = nullptr;
    QStackedWidget* mView = nullptr;
    QWidget* mConfigWidget = nullptr;
    QListView* mNodeAddressList = nullptr;

    QMap<MainType, QMenu*> mMenu = QMap<MainType, QMenu*>();
    QMap<MainType, QToolBar*> mToolBar = QMap<MainType, QToolBar*>();
    QMap<int, ListItem*> mConfigInfo = QMap<int, ListItem*>();
    QPushButton* mConfigHideButton = nullptr;
};

#endif    // GUI_TOP_H
