#include "GuiTop.h"
#include "AbstractHandler.h"

#include "CommonEnum.h"
#include "CommonResource.h"

#include <QKeySequence>
#include <QCompleter>

#include <QPushButton>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QLineEdit>


QSharedPointer<GuiTop>& GuiTop::instance(AbstractHandler* handler) {
    static QSharedPointer<GuiTop> gGui;
    if (gGui.isNull()) {
        gGui = QSharedPointer<GuiTop>(new GuiTop(handler));
    }
    return gGui;
}

GuiTop::GuiTop(AbstractHandler* handler) : AbstractGui(handler) {
    mMainWindow->setParent(isHandler()->getScreen());
    mMainWindow->show();
    updateDisplaySize();
}

void GuiTop::drawDisplayDepth0() {
    drawMenuFile();
    drawMenuEdit();
    drawMenuView();
    drawMenuSetting();
    drawMenuHelp();
}

void GuiTop::drawDisplayDepth1() {
}

void GuiTop::drawDisplayDepth2() {
}

void GuiTop::updateDisplaySize() {
    QRect rect = isHandler()->getScreen()->geometry();
    // QRect rect = mMainWindow->geometry();
    QSize size = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize).toSize();
    rect.setWidth(size.width());
    rect.setHeight(size.height());
    mMainWindow->setGeometry(rect);
}

void GuiTop::updateDisplayVisible() {
    if (isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeVisible).toBool()) {
        mMainWindow->show();
    } else {
        mMainWindow->hide();
    }
}

void GuiTop::drawMenuFile() {
    mMenu[MainType::File] = mMainWindow->menuBar()->addMenu(STRING_FILE);
    mToolBar[MainType::File] = mMainWindow->addToolBar(STRING_FILE);

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

void GuiTop::drawMenuEdit() {
    mMenu[MainType::Edit] = mMainWindow->menuBar()->addMenu(STRING_EDIT);
    mToolBar[MainType::Edit] = mMainWindow->addToolBar(STRING_EDIT);

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

    mMainWindow->menuBar()->addSeparator();
#endif
}


void GuiTop::drawMenuView() {
    mMenu[MainType::View] = mMainWindow->menuBar()->addMenu(STRING_VIEW);
    mToolBar[MainType::View] = mMainWindow->addToolBar(STRING_VIEW);

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

    QAction *actionSignal = new QAction(QIcon::fromTheme("actionSignal"),
                                                        STRING_SIGNAL,
                                                        this);
    if (actionSignal) {
        actionSignal->setStatusTip(STRING_SIGNAL_TIP);
        mMenu[MainType::View]->addAction(actionSignal);
        mToolBar[MainType::View]->addAction(actionSignal);
        connect(actionSignal, &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeViewSignal, QVariant());
        });
    }

    QAction *actionPython = new QAction(QIcon::fromTheme("actionPython"),
                                                        STRING_PYTHON,
                                                        this);
    if (actionPython) {
        actionPython->setStatusTip(STRING_PYTHON_TIP);
        mMenu[MainType::View]->addAction(actionPython);
        mToolBar[MainType::View]->addAction(actionPython);
        connect(actionPython, &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeViewPythonLib, QVariant());
        });
    }
}

void GuiTop::drawMenuSetting() {
    mMenu[MainType::Setting] = mMainWindow->menuBar()->addMenu(STRING_SETTING);
    mToolBar[MainType::Setting] = mMainWindow->addToolBar(STRING_SETTING);

    QAction *actionDevPath = new QAction(QIcon::fromTheme("actionDevPath"),
                                                        STRING_PATH,
                                                        this);
    if (actionDevPath) {
        actionDevPath->setStatusTip(STRING_PATH_TIP);
        mMenu[MainType::Setting]->addAction(actionDevPath);
        mToolBar[MainType::Setting]->addAction(actionDevPath);
        connect(actionDevPath, &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeSettingDevPath, QVariant());
        });
    }

    QAction *actionTestReport = new QAction(QIcon::fromTheme("actionTestReport"),
                                                        STRING_REPORT,
                                                        this);
    if (actionTestReport) {
        actionTestReport->setStatusTip(STRING_REPORT_TIP);
        mToolBar[MainType::Setting]->addAction(actionTestReport);
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

    mMainWindow->menuBar()->addSeparator();
}

void GuiTop::drawMenuHelp() {
    mMenu[MainType::Help] = mMainWindow->menuBar()->addMenu(STRING_HELP);

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


void GuiTop::updateDisplayAllConfig() {
    static QTextEdit* configContent = nullptr;
    if (configContent == nullptr) {
        QRect rect = isHandler()->getScreen()->rect();
        rect.setX(0);
        rect.setY(120);

        QFont font;
        font.setPixelSize(15);

        configContent = new QTextEdit(isHandler()->getScreen());
        configContent->setGeometry(rect);
        configContent->setFont(font);
        configContent->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        // configContent->setDisabled(true);
    }

    QString content = QString(" [Config Infomation]\n\n");
    QVariantList allConfig = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeAllConfigInfo).toList();
    foreach(const auto& config, allConfig) {
        content.append(config.toString());
    }
    configContent->setPlainText(content);
    configContent->show();


    static QPushButton* lastFileExit = nullptr;
    if (lastFileExit == nullptr) {
        QRect rect = configContent->rect();
        lastFileExit = new QPushButton(configContent);
        rect.setX(rect.width() - 30);
        rect.setY(0);
        rect.setWidth(30);
        rect.setHeight(20);
        lastFileExit->setGeometry(rect);
        lastFileExit->setStyleSheet("background-color: rgb(255, 255, 255); color: black; font: bold; font-size:20px");
        lastFileExit->setText("X");
        lastFileExit->setStyleSheet("color: rgb(50, 50, 100)");
        lastFileExit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        lastFileExit->show();
        connect(lastFileExit, &QPushButton::clicked, [=]() {
            configContent->hide();
        });
    }
}

void GuiTop::updateDisplayDefaultPath() {
    QString pathString = QString("Path : %1")
                        .arg(isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeDefaultPath).toString());
    static QLineEdit* defaultPath = nullptr;

    if (defaultPath == nullptr) {
        defaultPath = new QLineEdit(isHandler()->getScreen());
        defaultPath->setGeometry(QRect(350, 25, 450, 30));
        defaultPath->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        defaultPath->setDisabled(true);
        defaultPath->show();
    }
    defaultPath->setText(pathString);
}

void GuiTop::updateDisplayTempWidget(const int& type) {
    QWidget* screen = isHandler()->getScreen();
    static QPushButton* lastFile = nullptr;
    static QPushButton* parsing = nullptr;
    static QPushButton* excelOpen = nullptr;
    static QLineEdit* lineEdit = nullptr;

    if (lastFile == nullptr) {
        lastFile = new QPushButton(screen);
        lastFile->setGeometry(810, 25, 50, 30);
        lastFile->setStyleSheet("background-color: rgb(255, 255, 255); color: black; font: bold; font-size:20px");
        lastFile->setText("Last");
        lastFile->setStyleSheet("color: rgb(50, 50, 100)");
        lastFile->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        lastFile->show();
        connect(lastFile, &QPushButton::clicked, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeLastFile, QVariant());
        });
    }
    if (parsing == nullptr) {
        parsing = new QPushButton(screen);
        parsing->setGeometry(870, 25, 50, 30);
        parsing->setStyleSheet("background-color: rgb(255, 255, 255); color: black; font: bold; font-size:20px");
        parsing->setText("[     ]");
        parsing->setStyleSheet("color: rgb(50, 50, 100)");
        parsing->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        parsing->show();
        connect(parsing, &QPushButton::clicked, [=]() {
            // createSignal(ivis::common::EventTypeEnum::EventTypeFileOpen, QVariant());
        });
    }
    if (excelOpen == nullptr) {
        excelOpen = new QPushButton(screen);
        excelOpen->setGeometry(930, 25, 50, 30);
        excelOpen->setStyleSheet("background-color: rgb(255, 255, 255); color: black; font: bold; font-size:20px");
        excelOpen->setText("[     ]");
        excelOpen->setStyleSheet("color: rgb(50, 50, 100)");
        excelOpen->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        excelOpen->show();
        connect(excelOpen, &QPushButton::clicked, [=]() {
            // createSignal(ivis::common::EventTypeEnum::EventTypeFileSaveAs, QVariant());
        });
    }
    if (lineEdit == nullptr) {
        lineEdit = new QLineEdit(screen);
        // lineEdit->setGeometry(930, 25, 250, 30);
        lineEdit->setGeometry(10, 80, 1280-20, 30);
        lineEdit->setStyleSheet("background-color: rgb(255, 255, 255); color: black; font: bold; font-size:20px");
        lineEdit->setStyleSheet("color: rgb(50, 50, 100)");
        lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        lineEdit->show();
        // connect(lineEdit, &QLineEdit::cursorPositionChanged, [=](int oldPos, int newPos) {
        //     qDebug() << "cursorPositionChanged :" << oldPos << "->" << newPos;
        // });
        // connect(lineEdit, &QLineEdit::selectionChanged, [=]() {
        //     qDebug() << "selectionChanged";
        // });
    }


    if (type == ivis::common::PropertyTypeEnum::PropertyTypeSignalListSFC) {
        // add
    } else if (type == ivis::common::PropertyTypeEnum::PropertyTypeSignalListVSM) {
        // add
    } else if (type == ivis::common::PropertyTypeEnum::PropertyTypeSignalListAll) {
#if 1
        static QCompleter *completer = nullptr;
        if (completer == nullptr) {
            QStringList fileNames =
                            isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeSignalListAll).toStringList();
            completer = new QCompleter(fileNames, screen);
            completer->setCaseSensitivity(Qt::CaseInsensitive);
            completer->setFilterMode(Qt::MatchContains);
            completer->setWrapAround(false);
            // completer->setCompletionMode(QCompleter::CompletionMode::UnfilteredPopupCompletion);
            lineEdit->setCompleter(completer);
#else
#if 1
            static QCompleter *completerFile = new QCompleter(screen);
            static QFileSystemModel *model = new QFileSystemModel(completerFile);
        //        model->setRootPath(QDir::currentPath());
                model->setRootPath(isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeDefaultPath).toString());
            model->setFilter(QDir::Dirs|QDir::NoDotAndDotDot);
            model->sort(0, Qt::SortOrder::AscendingOrder);
            completerFile->setModel(model);
            completerFile->setFilterMode(Qt::MatchContains);
            lineEdit->setCompleter(completerFile);
#endif
#if 0
            static QCompleter *completerFile = new QCompleter(screen);
            completerFile->setModel(new QFileSystemModel(completerFile));
            lineEdit->setCompleter(completerFile);
#endif
#if 0
            QTreeView *tree = new QTreeView(mScscreenreen);
            tree->setModel(model);
            tree->setGeometry(0, 50, 1200, 400);
            tree->show();
#endif
#endif
        }
    }
}


void GuiTop::updateDisplay(const bool& first, const int& type) {
    if (type == ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize) {
        updateDisplaySize();
    } else if (type == ivis::common::PropertyTypeEnum::PropertyTypeVisible) {
        updateDisplayVisible();
    } else if (type == ivis::common::PropertyTypeEnum::PropertyTypeAllConfigInfo) {
        updateDisplayAllConfig();
    } else if (type == ivis::common::PropertyTypeEnum::PropertyTypeDefaultPath) {
        updateDisplayDefaultPath();
    } else {
        updateDisplayTempWidget(type);
    }
}

void GuiTop::slotPropertyChanged(const int& type, const QVariant& value) {
    switch (type) {
        case ivis::common::PropertyTypeEnum::PropertyTypeDepth : {
            drawDisplay(value);
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize :
        case ivis::common::PropertyTypeEnum::PropertyTypeVisible :
        case ivis::common::PropertyTypeEnum::PropertyTypeAllConfigInfo :
        case ivis::common::PropertyTypeEnum::PropertyTypeDefaultPath :
        case ivis::common::PropertyTypeEnum::PropertyTypeSignalListAll :
        case ivis::common::PropertyTypeEnum::PropertyTypeSignalListSFC :
        case ivis::common::PropertyTypeEnum::PropertyTypeSignalListVSM : {
            updateDisplay(false, type);
            break;
        }
        default : {
            break;
        }
    }
}
