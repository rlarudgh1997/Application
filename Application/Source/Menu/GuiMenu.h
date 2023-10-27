#ifndef GUI_MENU_H
#define GUI_MENU_H

#include "AbstractGui.h"

#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>

#include "CommonUtil.h"


class SelectModuleDialog;

class SelectPtDialog : public QDialog {
    Q_OBJECT

public:
    explicit SelectPtDialog(QWidget* parent, const QStringList& itemList) : QDialog(parent) {
        setWindowTitle("Select PT");
        setWindowFlag(Qt::WindowContextHelpButtonHint, true);
        setWindowFlag(Qt::WindowCloseButtonHint, true);
        setParent(parent);
        setModal(true);

        QRect rootWidgetRect =  static_cast<QWidget*>(parent->parent())->geometry();
        QRect setRect = QRect();
        setRect.setX(static_cast<int>(rootWidgetRect.x() + (rootWidgetRect.width() - 300) * 0.5));
        setRect.setY(static_cast<int>(rootWidgetRect.y() + (rootWidgetRect.height() - 150) * 0.5));
        setFixedSize(QSize(300, 130));
        setGeometry(setRect);
        setFocus();

        mLayout = new QVBoxLayout(this);
        mCheckLayout = new QHBoxLayout(mLayout->widget());
        mButtonLayout = new QVBoxLayout(mLayout->widget());

        foreach(const auto& item, itemList) {
            int index = mCheckBox.size();
            mCheckBox[index] = ivis::common::createWidget<QCheckBox>(mCheckLayout->widget(), true);
            mCheckBox[index]->setText(item);
            mCheckLayout->addWidget(mCheckBox[index]);
        }

        mOK = ivis::common::createWidget<QPushButton>(mButtonLayout->widget(), true);
        mOK->setText("OK");
        mCancel = ivis::common::createWidget<QPushButton>(mButtonLayout->widget(), true);
        mCancel->setText("Cancel");
        mButtonLayout->addWidget(mOK);
        mButtonLayout->addWidget(mCancel);

        mLayout->addLayout(mCheckLayout);
        mLayout->addLayout(mButtonLayout);
        setLayout(mLayout);

        connect(mOK, &QPushButton::clicked, [=]() {
            QList<QPair<QString, bool>> checkStateList = QList<QPair<QString, bool>>();
            foreach(const auto& check, mCheckBox) {
                checkStateList.append(QPair<QString, bool>(check->text(), check->checkState()));
            }
            emit signalPtSelected(checkStateList);
        });
        connect(mCancel, &QPushButton::clicked, [=]() {
            finished(true);
        });
    }

signals:
    void signalPtSelected(const QList<QPair<QString, bool>>& checkStateList);

private:
    QVBoxLayout* mLayout = nullptr;
    QHBoxLayout* mCheckLayout = nullptr;
    QVBoxLayout* mButtonLayout = nullptr;
    QMap<int, QCheckBox*> mCheckBox = QMap<int, QCheckBox*>();
    QPushButton* mOK = nullptr;
    QPushButton* mCancel = nullptr;
};

class LogDisplayDialog : public QDialog {
    Q_OBJECT

public:
    explicit LogDisplayDialog(QWidget* parent, const QString& title, const QString& info, const QString& content)
        : QDialog(parent) {
        setWindowTitle(title);
        setWindowFlag(Qt::WindowContextHelpButtonHint, false);
        setWindowFlag(Qt::WindowCloseButtonHint, false);
        // setWindowFlags(Qt::FramelessWindowHint);
        setParent(parent);
        setModal(true);

        QRect rootWidgetRect =  static_cast<QWidget*>(parent->parent())->geometry();
        QRect setRect = QRect();
        setRect.setX(static_cast<int>(rootWidgetRect.x() + (rootWidgetRect.width() - 800) * 0.5));
        setRect.setY(static_cast<int>(rootWidgetRect.y() + (rootWidgetRect.height() - 600) * 0.5));
        setFixedSize(QSize(800, 600));
        setGeometry(setRect);
        setFocus();

        mInfoLabel = ivis::common::createWidget<QLabel>(this, true, QRect(25, 0, 750, 80), mBaseStyle.arg(16));
        mContentLabel = ivis::common::createWidget<QLabel>(this, true, QRect(25, 80, 750, 420), mBaseStyle.arg(13));
        mContentLabel->setAlignment(Qt::AlignTop);
        updateLogDisplay(info, content);

        mOK = ivis::common::createWidget<QPushButton>(this, true, QRect(0, 550, 400, 50), mBaseStyle.arg(18));
        mOK->setText("OK");
        mCancel = ivis::common::createWidget<QPushButton>(this, true, QRect(400, 550, 400, 50), mBaseStyle.arg(18));
        mCancel->setText("Cancel");

        connect(mOK, &QPushButton::clicked, [=]() {
            emit signalTestResultCacel();
        });
        connect(mCancel, &QPushButton::clicked, [=]() {
            finished(true);
        });
    }
    void updateLogDisplay(const QString& info, const QString& content) {
        mInfoLabel->setText(info);
        mContentLabel->setText(content);
    }

signals:
    void signalTestResultCacel();

private:
    QString mBaseStyle = QString("color: blue; font-size: %1px");
    QLabel* mInfoLabel = nullptr;
    QLabel* mContentLabel = nullptr;
    QPushButton* mOK = nullptr;
    QPushButton* mCancel = nullptr;
};


class ProgressBar : public QWidget {
    Q_OBJECT

public:
    explicit ProgressBar(QWidget* parent, const QString& title, const int& current, const int) : QWidget(parent) {
    }
};


class GuiMenu : public AbstractGui {
    Q_OBJECT

private:
    enum class MainType {
        File = 0,
        Edit,
        View,
        Setting,
        Report,
        Run,
        Help,
        Max,
    };


public:
    static QSharedPointer<GuiMenu>& instance(AbstractHandler* handler = nullptr);


private:
    explicit GuiMenu(AbstractHandler* handler = nullptr);

    virtual void drawDisplayDepth0();
    virtual void drawDisplayDepth1();
    virtual void drawDisplayDepth2();
    virtual void updateDisplaySize();
    virtual void updateDisplayVisible();

    void drawMenuFile();
    void drawMenuEdit();
    void drawMenuView();
    void drawMenuSetting();
    void drawMenuReport();
    void drawMenuRun();
    void drawMenuHelp();
    void drawMenuEtc(const bool& update = false);
    void updateDisplaySelectModule(const int& type);
    void updateDisplaySelectPT(const int& type, const QVariantList& moduleList);
    void updateDisplayTestResultInfo();


public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value);


private:
    QMainWindow* mMainView = nullptr;
    QMap<MainType, QMenu*> mMenu = QMap<MainType, QMenu*>();
    QMap<MainType, QToolBar*> mToolBar = QMap<MainType, QToolBar*>();
    QMap<MainType, QMap<QString, QAction*>> mAction = QMap<MainType, QMap<QString, QAction*>>();
    SelectModuleDialog* mSelectModule = nullptr;
    SelectPtDialog* mSelectPt = nullptr;
    LogDisplayDialog* mLogDisplay = nullptr;
    QPushButton* mDefaultPath = nullptr;
    QProgressBar* mProgressBar = nullptr;
};

#endif    // GUI_MENU_H
