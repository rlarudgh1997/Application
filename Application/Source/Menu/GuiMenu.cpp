#include "GuiMenu.h"
#include "AbstractHandler.h"

#include "CommonResource.h"

#include <QKeySequence>
#include <QLineEdit>
#include <QCompleter>
#include <QPushButton>




QSharedPointer<GuiMenu>& GuiMenu::instance(AbstractHandler* handler) {
    static QSharedPointer<GuiMenu> gGui;
    if (gGui.isNull()) {
        gGui = QSharedPointer<GuiMenu>(new GuiMenu(handler));
    }
    return gGui;
}

GuiMenu::GuiMenu(AbstractHandler* handler) : AbstractGui(handler) {
    mMainView = new QMainWindow();
    mMainView->setParent(isHandler()->getScreen());    // 부팅시 윈도우창 만들기
    updateDisplaySize();
}

void GuiMenu::drawDisplayDepth0() {
    drawMenuFile();
#if !defined(USE_DEMO)
    drawMenuEdit();
#endif
    drawMenuView();
    drawMenuSetting();
    drawMenuHelp();
#if !defined(USE_DEMO)
    drawMenuEtc();
#endif
}

void GuiMenu::drawDisplayDepth1() {
}

void GuiMenu::drawDisplayDepth2() {
}

void GuiMenu::updateDisplaySize() {
    QSize size = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize).toSize();
    QSize margin = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySizeMargin).toSize();
    QRect rect = isHandler()->getScreen()->geometry();

    if (margin.width() != 0) {
        rect.setX(margin.width());
        rect.setWidth(size.width() - margin.width());
    }
    if (margin.height() != 0) {
        rect.setY(margin.height());
        rect.setHeight(size.height() - margin.height());
    }
    mMainView->setGeometry(rect);
}

void GuiMenu::updateDisplayVisible() {
    if (isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeVisible).toBool()) {
        mMainView->show();
    } else {
        mMainView->hide();
    }
}

void GuiMenu::drawMenuFile() {
    mMenu[MainType::File] = mMainView->menuBar()->addMenu(STRING_FILE);
    mToolBar[MainType::File] = mMainView->addToolBar(STRING_FILE);

    QAction *actionNew = new QAction(QIcon::fromTheme("actionNew",
                                                        QIcon(IAMGE_COPY)),
                                                        STRING_NEW,
                                                        this);
    if (actionNew) {
        actionNew->setShortcuts(QKeySequence::New);
        actionNew->setStatusTip(STRING_NEW_TIP);
        mMenu[MainType::File]->addAction(actionNew);
        mToolBar[MainType::File]->addAction(actionNew);
        connect(actionNew, &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeFileNew, QVariant());
        });
    }

    QAction *actionOpen = new QAction(QIcon::fromTheme("actionOpen",
                                                        QIcon(IAMGE_OPEN)),
                                                        STRING_OPEN,
                                                        this);
    if (actionOpen) {
        actionOpen->setShortcuts(QKeySequence::Open);
        actionOpen->setStatusTip(STRING_OPEN_TIP);
        mMenu[MainType::File]->addAction(actionOpen);
        mToolBar[MainType::File]->addAction(actionOpen);
        connect(actionOpen, &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeFileOpen, QVariant());
        });
    }

    QAction *actionSave = new QAction(QIcon::fromTheme("actionSave",
                                                        QIcon(IAMGE_SAVE)),
                                                        STRING_SAVE,
                                                        this);
    if (actionSave) {
        actionSave->setShortcuts(QKeySequence::Save);
        actionSave->setStatusTip(STRING_SAVE_TIP);
        mMenu[MainType::File]->addAction(actionSave);
        mToolBar[MainType::File]->addAction(actionSave);
        connect(actionSave, &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeFileSave, QVariant());
        });
    }

    QAction *actionSaveAs = new QAction(QIcon::fromTheme("actionSaveAs"),
                                                        STRING_SAVE_AS,
                                                        this);
    if (actionSaveAs) {
        // actionSaveAs->setDisabled(true);
        actionSaveAs->setShortcuts(QKeySequence::SaveAs);
        actionSaveAs->setStatusTip(STRING_SAVE_AS_TIP);
        mMenu[MainType::File]->addAction(actionSaveAs);
        // mMenu[MainType::File]->setStyleSheet(QString("QMenu::item{background-color : red; color : blue;);}"));
        // qDebug() << "StyelSheet :" << mMenu[MainType::File]->styleSheet();
        // mToolBar[MainType::File]->addAction(actionSaveAs);
        connect(actionSaveAs, &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeFileSaveAs, QVariant());
        });
    }

    mMenu[MainType::File]->addSeparator();
    QAction *actionExit = new QAction(QIcon::fromTheme("actionExit"),
                                                        STRING_SAVE_EXIT,
                                                        this);
    if (actionExit) {
        actionExit->setShortcuts(QKeySequence::Quit);
        actionExit->setStatusTip(STRING_SAVE_EXIT_TIP);
        mMenu[MainType::File]->addAction(actionExit);
        // mToolBar[MainType::File]->addAction(actionExit);
        connect(actionExit, &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeExitProgram, QVariant());
        });
    }
}

void GuiMenu::drawMenuEdit() {
    mMenu[MainType::Edit] = mMainView->menuBar()->addMenu(STRING_EDIT);
    mToolBar[MainType::Edit] = mMainView->addToolBar(STRING_EDIT);

#ifndef QT_NO_CLIPBOARD
    QAction *actionCut = new QAction(QIcon::fromTheme("actionCut",
                                                        QIcon(IAMGE_CUT)),
                                                        STRING_CUT,
                                                        this);
    if (actionCut) {
        actionCut->setShortcuts(QKeySequence::Cut);
        actionCut->setStatusTip(STRING_CUT_TIP);
        mMenu[MainType::Edit]->addAction(actionCut);
        mToolBar[MainType::Edit]->addAction(actionCut);
        connect(actionCut, &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeEditCut, QVariant());
        });
    }

    QAction *actionCopy = new QAction(QIcon::fromTheme("actionCopy",
                                                        QIcon(IAMGE_COPY)),
                                                        STRING_COPY,
                                                        this);
    if (actionCopy) {
        actionCopy->setShortcuts(QKeySequence::Copy);
        actionCopy->setStatusTip(STRING_COPY_TIP);
        mMenu[MainType::Edit]->addAction(actionCopy);
        mToolBar[MainType::Edit]->addAction(actionCopy);
        connect(actionCopy, &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeEditCopy, QVariant());
        });
    }

    QAction *actionPaste = new QAction(QIcon::fromTheme("actionPaste",
                                                        QIcon(IAMGE_PASTE)),
                                                        STRING_PASTE,
                                                        this);
    if (actionPaste) {
        actionPaste->setShortcuts(QKeySequence::Paste);
        actionPaste->setStatusTip(STRING_PASTE_TIP);
        mMenu[MainType::Edit]->addAction(actionPaste);
        mToolBar[MainType::Edit]->addAction(actionPaste);
        connect(actionPaste, &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeEditPaste, QVariant());
        });
    }

    mMainView->menuBar()->addSeparator();
#endif
}


void GuiMenu::drawMenuView() {
    mMenu[MainType::View] = mMainView->menuBar()->addMenu(STRING_VIEW);
    mToolBar[MainType::View] = mMainView->addToolBar(STRING_VIEW);

    QAction *actionConfig = new QAction(QIcon::fromTheme("actionConfig"),
                                                        STRING_CONFIG,
                                                        this);
    if (actionConfig) {
        actionConfig->setStatusTip(STRING_CONFIG_TIP);
        mMenu[MainType::View]->addAction(actionConfig);
        mToolBar[MainType::View]->addAction(actionConfig);
        connect(actionConfig, &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeViewConfig, QVariant());
        });
    }

    QAction *actionNodeAddress = new QAction(QIcon::fromTheme("actionNodeAddress"),
                                                        STRING_NODE_ADDRESS,
                                                        this);
    if (actionNodeAddress) {
        actionNodeAddress->setStatusTip(STRING_NODE_ADDRESS_TIP);
        mMenu[MainType::View]->addAction(actionNodeAddress);
        mToolBar[MainType::View]->addAction(actionNodeAddress);
        connect(actionNodeAddress, &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeViewNodeAddress, QVariant());
        });
    }

#if 0
    QAction *actionPython = new QAction(QIcon::fromTheme("actionPython"),
                                                        STRING_PYTHON,
                                                        this);
    if (actionPython) {
        actionPython->setStatusTip(STRING_PYTHON_TIP);
        mMenu[MainType::View]->addAction(actionPython);
        // mToolBar[MainType::View]->addAction(actionPython);
        connect(actionPython, &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeViewPythonLib, QVariant());
        });
    }
#endif
}

void GuiMenu::drawMenuSetting() {
    mMenu[MainType::Setting] = mMainView->menuBar()->addMenu(STRING_SETTING);
    mToolBar[MainType::Setting] = mMainView->addToolBar(STRING_SETTING);

    QAction *actionDevPath = new QAction(QIcon::fromTheme("actionDevPath"),
                                                        STRING_DEFAULT_PATH,
                                                        this);
    if (actionDevPath) {
        actionDevPath->setStatusTip(STRING_DEFAULT_PATH_TIP);
        mMenu[MainType::Setting]->addAction(actionDevPath);
        // mToolBar[MainType::Setting]->addAction(actionDevPath);
        connect(actionDevPath, &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeSettingDevPath, QVariant());
        });
    }

#if !defined(USE_DEMO)
    QAction *actionNodePath = new QAction(QIcon::fromTheme("actionNodePath"),
                                                        STRING_NODE_PATH,
                                                        this);
    if (actionNodePath) {
        actionNodePath->setStatusTip(STRING_NODE_PATH_TIP);
        mMenu[MainType::Setting]->addAction(actionNodePath);
        // mToolBar[MainType::Setting]->addAction(actionNodePath);
        connect(actionNodePath, &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeSettingNodePath, QVariant());
        });
    }
#endif

    QAction *actionTestReport = new QAction(QIcon::fromTheme("actionTestReport"),
                                                        STRING_REPORT,
                                                        this);
    if (actionTestReport) {
        actionTestReport->setStatusTip(STRING_REPORT_TIP);
        // mToolBar[MainType::Setting]->addAction(actionTestReport);
        connect(actionTestReport, &QAction::triggered, this, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeSettingTestReport, QVariant());
        });

        QMenu* mSubMenu = mMenu[MainType::Setting]->addMenu(STRING_TEST_REPORT);
        if (mSubMenu) {
            QAction *actionTestResult = new QAction(QIcon::fromTheme("actionTestResult"),
                                                            STRING_TEST_RESULT,
                                                            this);
            if (actionTestResult) {
                actionTestResult->setStatusTip(STRING_TEST_RESULT_TIP);
                mSubMenu->addAction(actionTestResult);
                connect(actionTestResult, &QAction::triggered, [=]() {
                    createSignal(ivis::common::EventTypeEnum::EventTypeSettingTestResult, QVariant());
                });
            }

            QAction *actionTestCoverage = new QAction(QIcon::fromTheme("actionTestCoverage"),
                                                            STRING_TEST_RESULT_COVERAGE,
                                                            mSubMenu);
            if (actionTestCoverage) {
                actionTestCoverage->setStatusTip(STRING_TEST_RESULT_COVERAGE_TIP);
                mSubMenu->addAction(actionTestCoverage);
                connect(actionTestCoverage, &QAction::triggered, [=]() {
                    createSignal(ivis::common::EventTypeEnum::EventTypeSettingTestCoverage, QVariant());
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

    mMainView->menuBar()->addSeparator();
}

void GuiMenu::drawMenuHelp() {
    mMenu[MainType::Help] = mMainView->menuBar()->addMenu(STRING_HELP);

    QAction *actionAbout = new QAction(QIcon::fromTheme("actionAbout"),
                                                        STRING_ABOUT,
                                                        this);
    if (actionAbout) {
        actionAbout->setStatusTip(STRING_ABOUT_TIP);
        mMenu[MainType::Help]->addAction(actionAbout);
        connect(actionAbout, &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeHelpAbout, QVariant());
        });
    }

    QAction *actionAboutQt = new QAction(QIcon::fromTheme("actionAboutQt"),
                                                        STRING_ABOUT_QT,
                                                        this);
    if (actionAboutQt) {
        actionAboutQt->setStatusTip(STRING_ABOUT_QT_TIP);
        mMenu[MainType::Help]->addAction(actionAboutQt);
        connect(actionAboutQt, &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeHelpAboutQt, QVariant());
        });
    }
}

void GuiMenu::drawMenuEtc() {
    updateDisplayDefaultPath();

    static QPushButton* lastFile = nullptr;
    if (lastFile == nullptr) {
        lastFile = new QPushButton(mMainView);
        lastFile->setGeometry(900, 25, 50, 30);
        lastFile->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(50, 50, 100); font: bold; font-size: 12px");
        lastFile->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        lastFile->setText("Last");
        lastFile->show();
        connect(lastFile, &QPushButton::clicked, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeLastFile, QVariant());
        });
    }

#if 1
    static QPushButton* excelOpen = nullptr;
    if (excelOpen == nullptr) {
        excelOpen = new QPushButton(mMainView);
        excelOpen->setGeometry(960, 25, 50, 30);
        excelOpen->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(50, 50, 100); font: bold; font-size: 12px");
        excelOpen->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        excelOpen->setText("Read");
        excelOpen->show();
        connect(excelOpen, &QPushButton::clicked, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeReadExcelInfo, QVariant());
        });
    }
#endif
}

void GuiMenu::updateDisplayDefaultPath() {
    QString path = QString("Path : %1")
                        .arg(isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeDefaultPath).toString());
    static QPushButton* defaultPath = nullptr;
    if (defaultPath == nullptr) {
        defaultPath = new QPushButton(isHandler()->getScreen());
        defaultPath->setGeometry(380, 25, 500, 30);
        defaultPath->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(50, 50, 100); font: bold; font-size: 12px");
        defaultPath->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        defaultPath->show();
        connect(defaultPath, &QPushButton::clicked, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeSettingDevPath, QVariant());
        });
    }
    defaultPath->setText(path);
}

void GuiMenu::slotPropertyChanged(const int& type, const QVariant& value) {
    switch (type) {
        case ivis::common::PropertyTypeEnum::PropertyTypeDepth : {
            drawDisplay(value);
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize : {
            updateDisplaySize();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeVisible : {
            updateDisplayVisible();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeDefaultPath : {
            updateDisplayDefaultPath();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeSignalListSFC :
        case ivis::common::PropertyTypeEnum::PropertyTypeSignalListVSM : {
            break;
        }
        default : {
            break;
        }
    }
}
