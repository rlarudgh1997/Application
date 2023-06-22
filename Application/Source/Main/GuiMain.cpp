#include "GuiMain.h"
#include "AbstractHandler.h"

#include "CommonEnum.h"
//#include "CommonUtil.h"


#include <QToolBar>
#include <QTableWidget>

#include <QPushButton>
#include <QLineEdit>


QSharedPointer<GuiMain> GuiMain::instance(AbstractHandler* handler) {
    static QSharedPointer<GuiMain> gGui;
    if (gGui.isNull()) {
        gGui = QSharedPointer<GuiMain>(new GuiMain(handler));
    }
    return gGui;
}

GuiMain::GuiMain(AbstractHandler* handler) : mHandler(handler), mScreen(handler->getScreen()) {
    mMainWindow = new QMainWindow();
    mMainWindow->setGeometry(mScreen->geometry());
    mMainWindow->setParent(mScreen);
    mTabWidget = new QTabWidget();
    mMainWindow->setCentralWidget(mTabWidget);
    mMainWindow->show();
}

bool GuiMain::createSignal(const int& type, const QVariant& value) {
    if (mHandler) {
        emit mHandler->signalHandlerEvent(type, value);
        return true;
    }

    qDebug() << "Fail to create signal - handler :" << mHandler;
    return false;
}

void GuiMain::drawDisplayMain() {
    mMainWindow->centralWidget()->hide();

    if ((mMenu.size() > 0) && (mToolBar.size() > 0)) {
        qDebug() << "Skip - drawDisplayMain";
        return;
    }

    // =================================================================================================================
    // Depth Change
    // =================================================================================================================
    QPushButton* depthChange = new QPushButton(mScreen);
    depthChange->setGeometry(1150, 25, 100, 30);
    depthChange->setStyleSheet("background-color: rgb(255, 255, 255); color: black; font: bold; font-size:20px");
    depthChange->setText("Change Depth");
    depthChange->setStyleSheet("color: rgb(50, 50, 100)");
    depthChange->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    depthChange->show();
    connect(depthChange, &QPushButton::clicked, [=]() {
        createSignal(EventTypeEnum::PropertyTypeChangeDepth, 0);
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
                createSignal(EventTypeEnum::PropertyTypeFileNew, 0);
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
                createSignal(EventTypeEnum::PropertyTypeFileOpen, 0);
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
                createSignal(EventTypeEnum::PropertyTypeFileSave, 0);
            });
        }

        QAction *actionSaveAs = new QAction(QIcon::fromTheme("actionSaveAs"),
                                                            QString("Save As"),
                                                            this);
        if (actionSaveAs) {
            actionSaveAs->setShortcuts(QKeySequence::SaveAs);
            actionSaveAs->setStatusTip(QString("Save the document under a new name"));
            mMenu[MainType::File]->addAction(actionSaveAs);
            mToolBar[MainType::File]->addAction(actionSaveAs);
            connect(actionSaveAs, &QAction::triggered, [=]() {
                createSignal(EventTypeEnum::PropertyTypeFileSaveAs, 0);
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
                createSignal(EventTypeEnum::PropertyTypeExitProgram, 0);
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
                createSignal(EventTypeEnum::PropertyTypeEditCut, 0);
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
                createSignal(EventTypeEnum::PropertyTypeEditCopy, 0);
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
                createSignal(EventTypeEnum::PropertyTypeEditPaste, 0);
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
                createSignal(EventTypeEnum::PropertyTypeSettingDevPath, 0);
            });
        }

        QAction *actionTestReport = new QAction(QIcon::fromTheme("actionTestReport"),
                                                            QString("Report"),
                                                            this);
        if (actionTestReport) {
            actionTestReport->setStatusTip(QString("Setting SFC test result info"));
            mToolBar[MainType::Setting]->addAction(actionTestReport);
            connect(actionTestReport, &QAction::triggered, this, [=]() {
                createSignal(EventTypeEnum::PropertyTypeSettingTestReport, 0);
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
                        createSignal(EventTypeEnum::PropertyTypeSettingTestResult, 0);
                    });
                }

                QAction *actionTestCoverage = new QAction(QIcon::fromTheme("actionTestCoverage"),
                                                                QString("Test Coverage"),
                                                                mSubMenu);
                if (actionTestCoverage) {
                    actionTestCoverage->setStatusTip(QString("Test Coverage Tip - ADD"));
                    mSubMenu->addAction(actionTestCoverage);
                    connect(actionTestCoverage, &QAction::triggered, [=]() {
                        createSignal(EventTypeEnum::PropertyTypeSettingTestCoverage, 0);
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
                createSignal(EventTypeEnum::PropertyTypeHelpAbout, 0);
            });
        }

        QAction *actionAboutQt = new QAction(QIcon::fromTheme("actionAboutQt"),
                                                            QString("About Qt"),
                                                            this);
        if (actionAboutQt) {
            actionAboutQt->setStatusTip(QString("Show the Qt library's About box"));
            mMenu[MainType::Help]->addAction(actionAboutQt);
            connect(actionAboutQt, &QAction::triggered, [=]() {
                createSignal(EventTypeEnum::PropertyTypeHelpAboutQt, 0);
            });
        }
    }
}

void GuiMain::drawDisplayDepth1() {
    mMainWindow->centralWidget()->show();

    QStringList columnTitles = QStringList();
    columnTitles.push_back("TCName");
    columnTitles.push_back("VehicleType");
    columnTitles.push_back("Result");
    columnTitles.push_back("Case");
    columnTitles.push_back("Input_Signal");
    columnTitles.push_back("Input_Data");
    columnTitles.push_back("Output_Signal");
    columnTitles.push_back("isInitialize");
    columnTitles.push_back("Output_Value");
    columnTitles.push_back("Config_Signal");
    columnTitles.push_back("Data");
    columnTitles.push_back("Negative");
    columnTitles.push_back("Test");

    QStringList seetTitles = QStringList();
    seetTitles.push_back("Privates");
    seetTitles.push_back("Telltales");
    seetTitles.push_back("Constants");
    seetTitles.push_back("Events");
    seetTitles.push_back("Sounds");
    seetTitles.push_back("Inters");
    seetTitles.push_back("Outputs");

    int rowCount = 5;
    QMap<int, QTableWidget*> mTableWidgets = QMap<int, QTableWidget*>();
    foreach(auto seetTitle, seetTitles) {
        int index = mTableWidgets.size();
        mTableWidgets[index] = new QTableWidget(rowCount, columnTitles.size()-1, mScreen);
        mTableWidgets[index]->setHorizontalHeaderLabels(columnTitles);
        mTabWidget->addTab(mTableWidgets[index], seetTitle);

        mTableWidgets[index]->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

        connect(mTableWidgets[index], &QTableWidget::customContextMenuRequested, [=](const QPoint &pos) {
            qDebug() << "customContextMenuRequested : " << pos;
        });
        connect(mTableWidgets[index], &QTableWidget::itemPressed, [=](QTableWidgetItem *item) {
            qDebug() << "itemPressed : " << item;
        });
        connect(mTableWidgets[index], &QTableWidget::itemClicked, [=](QTableWidgetItem *item) {
            qDebug() << "itemClicked : " << item;
        });
        connect(mTableWidgets[index], &QTableWidget::currentItemChanged,
                                    [=](QTableWidgetItem *current, QTableWidgetItem *previous) {
            qDebug() << "currentItemChanged : " << previous << " -> " << current;
        });
        connect(mTableWidgets[index], &QTableWidget::itemSelectionChanged, [=]() {
            qDebug() << "itemSelectionChanged";
        });
        connect(mTableWidgets[index], &QTableWidget::cellPressed, [=](auto row, auto column) {  // c++14 > version
            qDebug() << "cellPressed : " << row << ", " << column;
        });
        connect(mTableWidgets[index], &QTableWidget::cellClicked, [=](int row, int column) {
            qDebug() << "cellClicked : " << row << ", " << column;
        });

        // qDebug() << "Table[" << seetTitle << "] : " << mTableWidgets[index]->rowCount()
        //         << ", " << mTableWidgets[index]->columnCount();

        for (int row = 0; row < rowCount; row++) {
            for (int column = 0; column < columnTitles.size(); column++) {
                QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1[%2, %3]").arg(seetTitle).arg(row+1).arg(column+1));
                newItem->setTextAlignment(Qt::AlignCenter);
                mTableWidgets[index]->setItem(row, column, newItem);
            }
        }
    }
}

void GuiMain::drawDisplayDepth2() {
}

void GuiMain::slotPropertyChanged(const int& type, const QVariant& value) {
    switch (type) {
        case PropertyTypeEnum::PropertyTypeDepth : {
            if (value == QVariant(ScreenEnum::DisplayDepthMain)) {
                drawDisplayMain();
            } else if (value == QVariant(ScreenEnum::DisplayDepthDepth1)) {
                drawDisplayDepth1();
            } else if (value == QVariant(ScreenEnum::DisplayDepthDepth1)) {
                drawDisplayDepth1();
            } else {
            }
            break;
        }
        case PropertyTypeEnum::PropertyTypeDefaultPath : {
            QString defaultPathString = mHandler->getProperty(PropertyTypeEnum::PropertyTypeDefaultPath).toString();
            QLineEdit* defaultPath = new QLineEdit(mScreen);
            defaultPath->setGeometry(QRect(500, 25, 550, 30));
            defaultPath->setAlignment(Qt::AlignCenter);
            defaultPath->setText(QString("Path=%1").arg(defaultPathString));
            defaultPath->setDisabled(true);
            defaultPath->show();

//            defaultPath->setProperty("geometry", QRect(500, 25, 550, 30));
//            defaultPath->setProperty("alignment", Qt::AlignCenter);
//            defaultPath->setProperty("enabled", false);
//            defaultPath->setProperty("text", QString("Path=%1").arg(defaultPathString));
//            defaultPath->setProperty("visible", true);
//            SET_PROPERTY(defaultPath, "geometry", QRect(500, 25, 550, 30));
//            SET_PROPERTY(defaultPath, "alignment", Qt::AlignCenter);
//            SET_PROPERTY(defaultPath, "enabled", false);
//            SET_PROPERTY(defaultPath, "text", QString("Path=%1").arg(defaultPathString));
//            SET_PROPERTY(defaultPath, "visible", true);
            break;
        }
        default : {
            break;
        }
    }
}


