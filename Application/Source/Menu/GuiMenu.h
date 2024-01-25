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
#include <QLineEdit>
#include <QTextEdit>
#include <QScrollBar>
#include <QTextCharFormat>
#include <QButtonGroup>
#include <QRadioButton>
#include <QShortcut>

#include "CommonUtil.h"

class SelectModuleDialog;

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

        //        connect(mRadioLayout, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), [=](QAbstractButton*
        //        button) {
        //            updateStatus((button == mOn));
        //        });
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

        mCheckLayout1 = new QHBoxLayout(mLayout->widget());
        mCheckBox1 = ivis::common::createWidget<QCheckBox>(mCheckLayout1->widget(), true);
        mCheckBox1->setText(item);
        mLayout->addWidget(mCheckBox1);

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

        mClose = ivis::common::createWidget<QPushButton>(this, true, QRect(0, 550, 400, 50), mBaseStyle.arg(18));
        mClose->setText("Close");
        mCancel = ivis::common::createWidget<QPushButton>(this, true, QRect(400, 550, 400, 50), mBaseStyle.arg(18));
        mCancel->setText("Cancel");

        connect(mClose, &QPushButton::clicked, [=]() { emit signalTestResultClick(false); });
        connect(mCancel, &QPushButton::clicked, [=]() {
            emit signalTestResultClick(true);
            finished(true);
        });
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

private:
    const QString mBaseStyle = QString("color: blue; font-size: %1px");
    const int mWidth = 800;
    const int mHeight = 600;
    QTextEdit* mInfoLabel = nullptr;
    QTextEdit* mContentLabel = nullptr;
    QPushButton* mClose = nullptr;
    QPushButton* mManual = nullptr;
    QPushButton* mCancel = nullptr;
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
    void updateDisplaySelectModule(const int& runType);
    void updateDisplaySelectOption(const int& runType, const QVariantList& moduleList);
    void updateDisplayTestResultInfo();
    void updateDisplayEnterScriptText();
    void updateDisplayTestReport();

public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value);

private:
    QMainWindow* mMainView = nullptr;
    QMap<MainType, QMenu*> mMenu = QMap<MainType, QMenu*>();
    QMap<MainType, QToolBar*> mToolBar = QMap<MainType, QToolBar*>();
    QMap<MainType, QMap<QString, QAction*>> mAction = QMap<MainType, QMap<QString, QAction*>>();
    QList<QShortcut*> mShortcut = QList<QShortcut*>();
    QPushButton* mDefaultPath = nullptr;
    QProgressBar* mProgressBar = nullptr;
    SelectModuleDialog* mSelectModule = nullptr;
    CheckBoxGroupDialog* mCheckBoxGroup = nullptr;
    LogDisplayDialog* mLogDisplay = nullptr;
    TextEnterDialog* mTextEnter = nullptr;
    SelectReportDialog* mTestReport = nullptr;
};

#endif  // GUI_MENU_H
