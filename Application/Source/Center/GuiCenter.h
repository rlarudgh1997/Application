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

#include "Dialog.h"
#include "ui_GuiCenter.h"

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

class GuiCenter : public AbstractGui {
    Q_OBJECT

    REGISTER_WRITABLE_PROPERTY(int, ScrolBarValue, 0, false)
    REGISTER_WRITABLE_PROPERTY(QStringList, NodeAddress, QStringList(), false)

public:
    static QSharedPointer<GuiCenter>& instance(AbstractHandler* handler = nullptr);

private:
    explicit GuiCenter(AbstractHandler* handler = nullptr);

    virtual void drawDisplayDepth0();
    virtual void drawDisplayDepth1();
    virtual void drawDisplayDepth2();
    virtual void updateDisplaySize();
    virtual void updateDisplayVisible();

    void updateDrawDialog(const int& dialogType, const QVariantList& info);
    void updateDisplayConfigInfo();
    void updateDisplayNodeAddress(const int& updateType);
    void updateDisplayAutoComplete(const bool& show);
    void updateDisplaySelectModule(const bool& show);

public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value);

private:
    Ui::GuiCenter* mGui = nullptr;
    QStackedWidget* mMainView = nullptr;
    QSharedPointer<Dialog> mDialog = nullptr;
    QMap<int, ListItem*> mConfigListItem = QMap<int, ListItem*>();
};

#endif  // GUI_CENTER_H
