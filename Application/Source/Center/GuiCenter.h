#ifndef GUI_CENTER_H
#define GUI_CENTER_H

#include "AbstractGui.h"

#include <QWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QListView>
#include <QStringListModel>



class ListItem : public QObject {
    Q_OBJECT

public:
    ListItem() {}
    ListItem(const int& index, const int& type, const QString& name, const QString& value, QWidget* parent = nullptr) {
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
        qDebug() << "~ListItem() ->" << mNameButton->text() << "," << mValueDispaly->text();
        delete mNameButton;
        delete mValueDispaly;
        delete mValueEdit;
    }
    void initStyle() {
        mNameButton->setStyleSheet(mStyleNormal);
        mValueDispaly->setStyleSheet(mStyleNormal);
        mValueEdit->hide();
        mEditState = true;
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


public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value);


private:
    QStackedWidget* mMainView = nullptr;
    QWidget* mConfigWidget = nullptr;
    QWidget* mNodeAddressWidget = nullptr;
    QListView* mNodeAddressList = nullptr;
    QStringListModel* mNodeAddressModel = new QStringListModel();
    QPushButton* mConfigHideButton = nullptr;
    QMap<int, ListItem*> mConfigListItem = QMap<int, ListItem*>();
};

#endif    // GUI_CENTER_H
