#include "GuiTop.h"
#include "AbstractHandler.h"

#include "CommonEnum.h"


#include <QToolBar>
#include <QTableWidget>

#include <QPushButton>
#include <QLineEdit>


QSharedPointer<GuiTop> GuiTop::instance(AbstractHandler* handler) {
    static QSharedPointer<GuiTop> gGui;
    if (gGui.isNull()) {
        gGui = QSharedPointer<GuiTop>(new GuiTop(handler));
    }
    return gGui;
}

GuiTop::GuiTop(AbstractHandler* handler) : mHandler(handler), mScreen(handler->getScreen()) {
    mMainWindow = new QMainWindow();
    mMainWindow->setGeometry(mScreen->geometry());
    mMainWindow->setParent(mScreen);
    // mTabWidget = new QTabWidget();
    // mMainWindow->setCentralWidget(mTabWidget);
    mMainWindow->show();
}

bool GuiTop::createSignal(const int& type, const QVariant& value) {
    if (mHandler) {
        emit mHandler->signalHandlerEvent(type, value);
        return true;
    }

    qDebug() << "Fail to create signal - handler :" << mHandler;
    return false;
}

void GuiTop::drawDisplayDepth0() {
    // mMainWindow->centralWidget()->hide();
    if ((mMenu.size() > 0) && (mToolBar.size() > 0)) {
        qDebug() << "Skip - drawDisplayDepth0";
        return;
    }

    // =================================================================================================================
    // ETC Info
    // =================================================================================================================
    QLineEdit* defaultPath = new QLineEdit(mScreen);
    defaultPath->setGeometry(QRect(500, 25, 550, 30));
    defaultPath->setAlignment(Qt::AlignCenter);
    defaultPath->setText(QString("Path=%1").arg(mHandler->getProperty(PropertyTypeEnum::PropertyTypeDefaultPath).toString()));
    defaultPath->setDisabled(true);
    defaultPath->show();

    QPushButton* dispalyChange = new QPushButton(mScreen);
    dispalyChange->setGeometry(1100, 25, 150, 30);
    dispalyChange->setStyleSheet("background-color: rgb(255, 255, 255); color: black; font: bold; font-size:20px");
    dispalyChange->setText("Center - Show/Hide");
    dispalyChange->setStyleSheet("color: rgb(50, 50, 100)");
    dispalyChange->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    dispalyChange->show();
    connect(dispalyChange, &QPushButton::clicked, [=]() {
        createSignal(EventTypeEnum::EventTypeDisplayChange, 0);
    });


    // =================================================================================================================
    // FILE
    // =================================================================================================================
    if (true) {
        mMenu[MainType::File] = mMainWindow->menuBar()->addMenu(QString("File"));
        mToolBar[MainType::File] = mMainWindow->addToolBar(QString("File"));

        QAction *actionNew = new QAction(QIcon::fromTheme("actionNew",
                                                            QIcon(":/Image/Main/Copy.png")),
                                                            QString("New"),
                                                            this);
        if (actionNew) {
            actionNew->setShortcuts(QKeySequence::New);
            actionNew->setStatusTip(QString("Create a new file"));
            mMenu[MainType::File]->addAction(actionNew);
            mToolBar[MainType::File]->addAction(actionNew);
            connect(actionNew, &QAction::triggered, [=]() {
                createSignal(EventTypeEnum::EventTypeFileNew, 0);
            });
        }

        QAction *actionOpen = new QAction(QIcon::fromTheme("actionOpen",
                                                            QIcon(":/Image/Main/Open.png")),
                                                            QString("Open"),
                                                            this);
        if (actionOpen) {
            actionOpen->setShortcuts(QKeySequence::Open);
            actionOpen->setStatusTip(QString("Open an existing file"));
            mMenu[MainType::File]->addAction(actionOpen);
            mToolBar[MainType::File]->addAction(actionOpen);
            connect(actionOpen, &QAction::triggered, [=]() {
                createSignal(EventTypeEnum::EventTypeFileOpen, 0);
            });
        }

        QAction *actionSave = new QAction(QIcon::fromTheme("actionSave",
                                                            QIcon(":/Image/Main/Save.png")),
                                                            QString("Save"),
                                                            this);
        if (actionSave) {
            actionSave->setShortcuts(QKeySequence::Save);
            actionSave->setStatusTip(QString("Save the document to disk"));
            mMenu[MainType::File]->addAction(actionSave);
            mToolBar[MainType::File]->addAction(actionSave);
            connect(actionSave, &QAction::triggered, [=]() {
                createSignal(EventTypeEnum::EventTypeFileSave, 0);
            });
        }

        QAction *actionSaveAs = new QAction(QIcon::fromTheme("actionSaveAs"),
                                                            QString("Save As"),
                                                            this);
        if (actionSaveAs) {
            actionSaveAs->setShortcuts(QKeySequence::SaveAs);
            actionSaveAs->setStatusTip(QString("Save the document under a new name"));
            mMenu[MainType::File]->addAction(actionSaveAs);
            // mToolBar[MainType::File]->addAction(actionSaveAs);
            connect(actionSaveAs, &QAction::triggered, [=]() {
                createSignal(EventTypeEnum::EventTypeFileSaveAs, 0);
            });
        }

        mMenu[MainType::File]->addSeparator();
        QAction *actionExit = new QAction(QIcon::fromTheme("actionExit"),
                                                            QString("Exit"),
                                                            this);
        if (actionExit) {
            actionExit->setShortcuts(QKeySequence::Quit);
            actionExit->setStatusTip(QString("Exit the application"));
            mMenu[MainType::File]->addAction(actionExit);
            // mToolBar[MainType::File]->addAction(actionExit);
            connect(actionExit, &QAction::triggered, [=]() {
                createSignal(EventTypeEnum::EventTypeExitProgram, 0);
            });
        }
    }

    // =================================================================================================================
    // EDIT
    // =================================================================================================================
    if (true) {
        mMenu[MainType::Edit] = mMainWindow->menuBar()->addMenu(QString("Edit"));
        mToolBar[MainType::Edit] = mMainWindow->addToolBar(QString("Edit"));

#ifndef QT_NO_CLIPBOARD
        QAction *actionCut = new QAction(QIcon::fromTheme("actionCut",
                                                            QIcon(":/Image/Main/Cut.png")),
                                                            QString("Cut"),
                                                            this);
        if (actionCut) {
            actionCut->setShortcuts(QKeySequence::Cut);
            actionCut->setStatusTip(QString("Cut the current selection's contents to the clipboard"));
            mMenu[MainType::Edit]->addAction(actionCut);
            mToolBar[MainType::Edit]->addAction(actionCut);
            connect(actionCut, &QAction::triggered, [=]() {
                createSignal(EventTypeEnum::EventTypeEditCut, 0);
            });
        }

        QAction *actionCopy = new QAction(QIcon::fromTheme("actionCopy",
                                                            QIcon(":/Image/Main/Copy.png")),
                                                            QString("Copy"),
                                                            this);
        if (actionCopy) {
            actionCopy->setShortcuts(QKeySequence::Copy);
            actionCopy->setStatusTip(QString("Copy the current selection's contents to the clipboard"));
            mMenu[MainType::Edit]->addAction(actionCopy);
            mToolBar[MainType::Edit]->addAction(actionCopy);
            connect(actionCopy, &QAction::triggered, [=]() {
                createSignal(EventTypeEnum::EventTypeEditCopy, 0);
            });
        }

        QAction *actionPaste = new QAction(QIcon::fromTheme("actionPaste",
                                                            QIcon(":/Image/Main/Paste.png")),
                                                            QString("Paste"),
                                                            this);
        if (actionPaste) {
            actionPaste->setShortcuts(QKeySequence::Paste);
            actionPaste->setStatusTip(QString("Paste the clipboard's contents into the current selection"));
            mMenu[MainType::Edit]->addAction(actionPaste);
            mToolBar[MainType::Edit]->addAction(actionPaste);
            connect(actionPaste, &QAction::triggered, [=]() {
                createSignal(EventTypeEnum::EventTypeEditPaste, 0);
            });
        }

        mMainWindow->menuBar()->addSeparator();
#endif
    }

    // =================================================================================================================
    // Setting
    // =================================================================================================================
    if (true) {
        mMenu[MainType::Setting] = mMainWindow->menuBar()->addMenu(QString("Setting"));
        mToolBar[MainType::Setting] = mMainWindow->addToolBar(QString("Setting"));


        QAction *actionDevPath = new QAction(QIcon::fromTheme("actionDevPath"),
                                                            QString("Path"),
                                                            this);
        if (actionDevPath) {
            actionDevPath->setStatusTip(QString("Setting default SFC test output foler"));
            mMenu[MainType::Setting]->addAction(actionDevPath);
            mToolBar[MainType::Setting]->addAction(actionDevPath);
            connect(actionDevPath, &QAction::triggered, [=]() {
                createSignal(EventTypeEnum::EventTypeSettingDevPath, 0);
            });
        }

        QAction *actionTestReport = new QAction(QIcon::fromTheme("actionTestReport"),
                                                            QString("Report"),
                                                            this);
        if (actionTestReport) {
            actionTestReport->setStatusTip(QString("Setting SFC test result info"));
            mToolBar[MainType::Setting]->addAction(actionTestReport);
            connect(actionTestReport, &QAction::triggered, this, [=]() {
                createSignal(EventTypeEnum::EventTypeSettingTestReport, 0);
            });

            QMenu* mSubMenu = mMenu[MainType::Setting]->addMenu(QString("Test Report"));
            if (mSubMenu) {
                QAction *actionTestResult = new QAction(QIcon::fromTheme("actionTestResult"),
                                                                QString("Test Result"),
                                                                this);
                if (actionTestResult) {
                    actionTestResult->setStatusTip(QString("Test Result Tip - ADD"));
                    mSubMenu->addAction(actionTestResult);
                    connect(actionTestResult, &QAction::triggered, [=]() {
                        createSignal(EventTypeEnum::EventTypeSettingTestResult, 0);
                    });
                }

                QAction *actionTestCoverage = new QAction(QIcon::fromTheme("actionTestCoverage"),
                                                                QString("Test Coverage"),
                                                                mSubMenu);
                if (actionTestCoverage) {
                    actionTestCoverage->setStatusTip(QString("Test Coverage Tip - ADD"));
                    mSubMenu->addAction(actionTestCoverage);
                    connect(actionTestCoverage, &QAction::triggered, [=]() {
                        createSignal(EventTypeEnum::EventTypeSettingTestCoverage, 0);
                    });
                }
            }
        }

        // TooBar - Push Button
        // QPushButton* buttonTestResult = new QPushButton(QString("TestResult"));
        // if (buttonTestResult) {
        //     buttonTestResult->setObjectName(QString("TestResult"));
        //     mToolBar[MainType::Setting]->addWidget(buttonTestResult);
        //     connect(buttonTestResult, &QPushButton::clicked, this, [=]() {
        //         qDebug() << "Test Result";
        //     });
        // }
        // QPushButton* buttonTestCoverage = new QPushButton(QString("TestCoverage"));
        // if (buttonTestCoverage) {
        //     buttonTestResult->setObjectName(QString("TestConverage"));
        //     mToolBar[MainType::Setting]->addWidget(buttonTestCoverage);
        //     connect(buttonTestCoverage, &QPushButton::clicked, this, [=]() {
        //         qDebug() << "Test Coverage";
        //     });
        // }

        mMainWindow->menuBar()->addSeparator();
    }


    // =================================================================================================================
    // Help
    // =================================================================================================================
    if (true) {
        mMenu[MainType::Help] = mMainWindow->menuBar()->addMenu(QString("Help"));

        QAction *actionAbout = new QAction(QIcon::fromTheme("actionAbout"),
                                                            QString("About"),
                                                            this);
        if (actionAbout) {
            actionAbout->setStatusTip(QString("Show the application's About box"));
            mMenu[MainType::Help]->addAction(actionAbout);
            connect(actionAbout, &QAction::triggered, [=]() {
                createSignal(EventTypeEnum::EventTypeHelpAbout, 0);
            });
        }

        QAction *actionAboutQt = new QAction(QIcon::fromTheme("actionAboutQt"),
                                                            QString("About Qt"),
                                                            this);
        if (actionAboutQt) {
            actionAboutQt->setStatusTip(QString("Show the Qt library's About box"));
            mMenu[MainType::Help]->addAction(actionAboutQt);
            connect(actionAboutQt, &QAction::triggered, [=]() {
                createSignal(EventTypeEnum::EventTypeHelpAboutQt, 0);
            });
        }
    }
}

void GuiTop::drawDisplayDepth1() {
}

void GuiTop::drawDisplayDepth2() {
}

void GuiTop::slotPropertyChanged(const int& type, const QVariant& value) {
    switch (type) {
        case PropertyTypeEnum::PropertyTypeDepth : {
            if (value == QVariant(ScreenEnum::DisplayDepthDepth0)) {
                drawDisplayDepth0();
            } else if (value == QVariant(ScreenEnum::DisplayDepthDepth1)) {
                drawDisplayDepth1();
            } else if (value == QVariant(ScreenEnum::DisplayDepthDepth2)) {
                drawDisplayDepth2();
            } else {
            }
            break;
        }
        default : {
            break;
        }
    }
}


