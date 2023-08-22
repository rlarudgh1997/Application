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
// #include <QTextEdit>
#include <QLineEdit>



// #include <QHBoxLayout>
// #include <QPushButton>




// #include <QObject>

class ListItem : public QObject {
    Q_OBJECT

public:
    ListItem() {}
    ListItem(const int& index, const int& type, const QString& name, const QString& value, QWidget* parent = nullptr) {
        QFont font;
        font.setPixelSize(15);

        mName = new QLabel(parent);
        mName->setGeometry(20, (index * 50), 300, 50);
        mName->setFont(font);
        mName->setFrameShape(QLabel::StyledPanel);
        mName->setText(name);
        mName->show();

        mValue = new QLineEdit(parent);
        mValue->setGeometry(370, (index * 50), 900, 50);
        mValue->setFont(font);
        mValue->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        // QRegExp rx("(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])");
        // QRegExpValidator *validator = new QRegExpValidator(rx,this);
        // mValue->setValidator(validator);
        mValue->setText(value);
        mValue->show();

        // connect(mValue, &QLineEdit::textChanged, [=]() {
        connect(mValue, &QLineEdit::returnPressed, [=]() {
            emit signalValueChanged(index, type, QVariant(mValue->text()));
        });
    }
    ~ListItem() {
        clear();
    }
    void clear() {
        qDebug() << "~ListItem() ->" << mName->text() << "," << mValue;
        delete mName;
        delete mValue;
    }

signals:
    void signalValueChanged(const int& index, const int& type, const QVariant& value);

private:
    int mIndex = (-1);
    QLabel* mName = nullptr;
    QLineEdit* mValue = nullptr;
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
    virtual void updateDisplay(const bool& first, const int& type = 0);
    virtual void updateDisplaySize();
    virtual void updateDisplayVisible();

    void drawMenuFile();
    void drawMenuEdit();
    void drawMenuView();
    void drawMenuSetting();
    void drawMenuHelp();
    void drawMenuEtc();
    void updateDisplayConfigInfo();
    void updateDisplayNodeAddress();
    void updateDisplayDefaultPath();
    void updateDisplayTempWidget(const int& type);


public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value);


private:
    QMainWindow* mMainWindow = new QMainWindow();
    QStackedWidget* mView = new QStackedWidget();
    QMap<MainType, QMenu*> mMenu = QMap<MainType, QMenu*>();
    QMap<MainType, QToolBar*> mToolBar = QMap<MainType, QToolBar*>();
//    QMap<int, QPair<QLabel*, QPlainTextEdit*>> mConfigInfo = QMap<int, QPair<QLabel*, QPlainTextEdit*>>();
    QMap<int, ListItem*> mConfigInfo = QMap<int, ListItem*>();
};

#endif    // GUI_TOP_H
