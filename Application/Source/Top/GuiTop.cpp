#include "GuiTop.h"
#include "AbstractHandler.h"

#include "CommonEnum.h"
#include "CommonResource.h"


#include <QToolBar>
#include <QTableWidget>
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QProcess>


#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QCompleter>
#include <QFileSystemModel>
#include <QTreeView>
#include <QListView>




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

    updateDisplay(true);

    mActionInfo[ActionType::FileNew] = ActionInfo(EventTypeEnum::EventTypeFileNew,
                                                    "actionNew", QIcon(IAMGE_COPY),
                                                    STRING_NEW, STRING_NEW_TIP, QKeySequence::New, this);

    // =================================================================================================================
    // FILE
    // =================================================================================================================
    if (true) {
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
                createSignal(EventTypeEnum::EventTypeFileNew, QVariant());
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
                QString filePath = QFileDialog::getOpenFileName(qobject_cast<QWidget*>(mHandler),
                                            STRING_FILE_OPEN,
                                            mHandler->getProperty(PropertyTypeEnum::PropertyTypeDefaultPath).toString()+"/TC",
                                            "Files (*.*)");
                if (filePath.size() > 0) {
                    createSignal(EventTypeEnum::EventTypeFileOpen, filePath);
                }
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
                createSignal(EventTypeEnum::EventTypeFileSave, QVariant());
            });
        }

        QAction *actionSaveAs = new QAction(QIcon::fromTheme("actionSaveAs"),
                                                            STRING_SAVE_AS,
                                                            this);
        if (actionSaveAs) {
            actionSaveAs->setShortcuts(QKeySequence::SaveAs);
            actionSaveAs->setStatusTip(STRING_SAVE_AS_TIP);
            mMenu[MainType::File]->addAction(actionSaveAs);
            // mToolBar[MainType::File]->addAction(actionSaveAs);
            connect(actionSaveAs, &QAction::triggered, [=]() {
                createSignal(EventTypeEnum::EventTypeFileSaveAs, QVariant());
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
                createSignal(EventTypeEnum::EventTypeExitProgram, QVariant());
            });
        }
    }

    // =================================================================================================================
    // EDIT
    // =================================================================================================================
    if (true) {
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
                createSignal(EventTypeEnum::EventTypeEditCut, QVariant());
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
                createSignal(EventTypeEnum::EventTypeEditCopy, QVariant());
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
                createSignal(EventTypeEnum::EventTypeEditPaste, QVariant());
            });
        }

        mMainWindow->menuBar()->addSeparator();
#endif
    }

    // =================================================================================================================
    // Setting
    // =================================================================================================================
    if (true) {
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
                QString defaultPath = QFileDialog::getExistingDirectory(qobject_cast<QWidget*>(mHandler),
                                                STRING_DEFAULT_PATH,
                                                mHandler->getProperty(PropertyTypeEnum::PropertyTypeDefaultPath).toString(),
                                                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
                if (defaultPath.size() == 0) {
                    defaultPath = QApplication::applicationDirPath();
                }
                createSignal(EventTypeEnum::EventTypeSettingDevPath, defaultPath);
            });
        }

        QAction *actionTestReport = new QAction(QIcon::fromTheme("actionTestReport"),
                                                            STRING_REPORT,
                                                            this);
        if (actionTestReport) {
            actionTestReport->setStatusTip(STRING_REPORT_TIP);
            mToolBar[MainType::Setting]->addAction(actionTestReport);
            connect(actionTestReport, &QAction::triggered, this, [=]() {
                createSignal(EventTypeEnum::EventTypeSettingTestReport, QVariant());
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
                        createSignal(EventTypeEnum::EventTypeSettingTestResult, QVariant());
                    });
                }

                QAction *actionTestCoverage = new QAction(QIcon::fromTheme("actionTestCoverage"),
                                                                STRING_TEST_RESULT_COVERAGE,
                                                                mSubMenu);
                if (actionTestCoverage) {
                    actionTestCoverage->setStatusTip(STRING_TEST_RESULT_COVERAGE_TIP);
                    mSubMenu->addAction(actionTestCoverage);
                    connect(actionTestCoverage, &QAction::triggered, [=]() {
                        createSignal(EventTypeEnum::EventTypeSettingTestCoverage, QVariant());
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
        mMenu[MainType::Help] = mMainWindow->menuBar()->addMenu(STRING_HELP);

        QAction *actionAbout = new QAction(QIcon::fromTheme("actionAbout"),
                                                            STRING_ABOUT,
                                                            this);
        if (actionAbout) {
            actionAbout->setStatusTip(STRING_ABOUT_TIP);
            mMenu[MainType::Help]->addAction(actionAbout);
            connect(actionAbout, &QAction::triggered, [=]() {
                QMessageBox::about(qobject_cast<QWidget*>(mHandler),
                                STRING_POPUP_ABOUT,
                                STRING_POPUP_ABOUT_TIP);
            });
        }

        QAction *actionAboutQt = new QAction(QIcon::fromTheme("actionAboutQt"),
                                                            STRING_ABOUT_QT,
                                                            this);
        if (actionAboutQt) {
            actionAboutQt->setStatusTip(STRING_ABOUT_QT_TIP);
            mMenu[MainType::Help]->addAction(actionAboutQt);
            connect(actionAboutQt, &QAction::triggered, [=]() {
                QApplication::aboutQt();
            });
        }
    }
}

void GuiTop::drawDisplayDepth1() {
}

void GuiTop::drawDisplayDepth2() {
}

void GuiTop::updateDisplaySize() {
    QRect rect = mMainWindow->geometry();
    QSize size = mHandler->getProperty(PropertyTypeEnum::PropertyTypeDisplaySize).toSize();
    rect.setWidth(size.width());
    rect.setHeight(size.height());
    mMainWindow->setGeometry(rect);
}

void GuiTop::updateDisplayVisible() {
}


void GuiTop::updateDisplay(const bool& first, const int& type) {
    static QLineEdit* defaultPath = new QLineEdit(mScreen);
    static QPushButton* dispalyChange = new QPushButton(mScreen);
    static QPushButton* parsing = new QPushButton(mScreen);
    static QLineEdit *lineEdit = new QLineEdit(mScreen);

    if (first) {
        defaultPath->setGeometry(QRect(350, 25, 450, 30));
        // defaultPath->setAlignment(Qt::AlignCenter);
        defaultPath->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        defaultPath->setDisabled(true);
        defaultPath->show();

        dispalyChange->setGeometry(810, 25, 50, 30);
        dispalyChange->setStyleSheet("background-color: rgb(255, 255, 255); color: black; font: bold; font-size:20px");
        dispalyChange->setText("Visible");
        dispalyChange->setStyleSheet("color: rgb(50, 50, 100)");
        dispalyChange->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        dispalyChange->show();
        connect(dispalyChange, &QPushButton::clicked, [=]() {
            createSignal(EventTypeEnum::EventTypeCenterVisible, QVariant());
        });

        parsing->setGeometry(870, 25, 50, 30);
        parsing->setStyleSheet("background-color: rgb(255, 255, 255); color: black; font: bold; font-size:20px");
        parsing->setText("Parsing");
        parsing->setStyleSheet("color: rgb(50, 50, 100)");
        parsing->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        parsing->show();
        connect(parsing, &QPushButton::clicked, [=]() {
            createSignal(EventTypeEnum::EventTypeParsingExcel, QVariant());
        });

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

    if (type == PropertyTypeEnum::PropertyTypeDisplaySize) {
        updateDisplaySize();
    } else if (type == PropertyTypeEnum::PropertyTypeSignalListSFC) {
        // add
    } else if (type == PropertyTypeEnum::PropertyTypeSignalListVSM) {
        // add
    } else if (type == PropertyTypeEnum::PropertyTypeSignalListAll) {
#if 1
        QStringList fileNames = mHandler->getProperty(PropertyTypeEnum::PropertyTypeSignalListAll).toStringList();
        static QCompleter *completer = new QCompleter(fileNames, mScreen);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        completer->setFilterMode(Qt::MatchContains);
        completer->setWrapAround(false);
        // completer->setCompletionMode(QCompleter::CompletionMode::UnfilteredPopupCompletion);
        lineEdit->setCompleter(completer);
#else
#if 1
        static QCompleter *completerFile = new QCompleter(mScreen);
        static QFileSystemModel *model = new QFileSystemModel(completerFile);
    //        model->setRootPath(QDir::currentPath());
            model->setRootPath(mHandler->getProperty(PropertyTypeEnum::PropertyTypeDefaultPath).toString());
        model->setFilter(QDir::Dirs|QDir::NoDotAndDotDot);
        model->sort(0, Qt::SortOrder::AscendingOrder);
        completerFile->setModel(model);
        completerFile->setFilterMode(Qt::MatchContains);
        lineEdit->setCompleter(completerFile);
#endif


#if 0
        static QCompleter *completerFile = new QCompleter(mScreen);
        completerFile->setModel(new QFileSystemModel(completerFile));
        lineEdit->setCompleter(completerFile);
#endif



#if 0
        QTreeView *tree = new QTreeView(mScreen);
        tree->setModel(model);
        tree->setGeometry(0, 50, 1200, 400);
        tree->show();
#endif
#endif
    }


    if (type == PropertyTypeEnum::PropertyTypeDefaultPath) {
        defaultPath->setText(QString("Path=%1").arg(mHandler->getProperty(PropertyTypeEnum::PropertyTypeDefaultPath).toString()));
    }
}

void GuiTop::slotPropertyChanged(const int& type, const QVariant& value) {
    switch (type) {
        case PropertyTypeEnum::PropertyTypeDisplaySize :
        case PropertyTypeEnum::PropertyTypeDefaultPath :
        case PropertyTypeEnum::PropertyTypeSignalListAll :
        case PropertyTypeEnum::PropertyTypeSignalListSFC :
        case PropertyTypeEnum::PropertyTypeSignalListVSM : {
            updateDisplay(false, type);
            break;
        }
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
