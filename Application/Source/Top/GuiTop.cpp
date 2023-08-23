#include "GuiTop.h"
#include "AbstractHandler.h"

#include "CommonEnum.h"
#include "CommonResource.h"

#include <QKeySequence>
#include <QLineEdit>
#include <QCompleter>
#include <QStringListModel>

// #include <QTextEdit>
// #include <QStandardItemModel>
// #include <QStandardItem>
// #include <QListWidget>
// #include <QVBoxLayout>
// #include <QStyledItemDelegate>



QSharedPointer<GuiTop>& GuiTop::instance(AbstractHandler* handler) {
    static QSharedPointer<GuiTop> gGui;
    if (gGui.isNull()) {
        gGui = QSharedPointer<GuiTop>(new GuiTop(handler));
    }
    return gGui;
}

GuiTop::GuiTop(AbstractHandler* handler) : AbstractGui(handler) {
    mMainWindow = new QMainWindow();    // 부팅시 윈도우창 만들기
    mMainWindow->setParent(isHandler()->getScreen());
    mView = new QStackedWidget(isHandler()->getScreen());
    updateDisplaySize();
}

void GuiTop::drawDisplayDepth0() {
    drawMenuFile();
    drawMenuEdit();
    drawMenuView();
    drawMenuSetting();
    drawMenuHelp();
    drawMenuEtc();
}

void GuiTop::drawDisplayDepth1() {
}

void GuiTop::drawDisplayDepth2() {
}

void GuiTop::updateDisplaySize() {
    QSize size = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize).toSize();
    QSize margin = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySizeMargin).toSize();
    QRect rect = isHandler()->getScreen()->geometry();
    mMainWindow->setGeometry(rect);

    if (margin.width() > 0) {
        rect.setX(margin.width());
        rect.setWidth(size.width() - margin.width());
    }
    if (margin.height() > 0) {
        rect.setY(margin.height());
        rect.setHeight(size.height() - margin.height());
    }
    mView->setGeometry(rect);
}

void GuiTop::updateDisplayVisible() {
    if (isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeVisible).toBool()) {
        mMainWindow->show();
    } else {
        mMainWindow->hide();
    }

    if (isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeSubVisible).toBool()) {
        mView->show();
    } else {
        mView->hide();
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
        // mToolBar[MainType::View]->addAction(actionPython);
        connect(actionPython, &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeViewPythonLib, QVariant());
        });
    }
}

void GuiTop::drawMenuSetting() {
    mMenu[MainType::Setting] = mMainWindow->menuBar()->addMenu(STRING_SETTING);
    mToolBar[MainType::Setting] = mMainWindow->addToolBar(STRING_SETTING);

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

void GuiTop::drawMenuEtc() {
    static QPushButton* lastFile = nullptr;
    if (lastFile == nullptr) {
        lastFile = new QPushButton(mMainWindow);
        lastFile->setGeometry(900, 25, 50, 30);
        lastFile->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(50, 50, 100); font: bold; font-size: 12px");
        lastFile->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        lastFile->setText("Last");
        lastFile->show();
        connect(lastFile, &QPushButton::clicked, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeLastFile, QVariant());
        });
    }

    static QPushButton* excelOpen = nullptr;
    if (excelOpen == nullptr) {
        excelOpen = new QPushButton(mMainWindow);
        excelOpen->setGeometry(960, 25, 50, 30);
        excelOpen->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(50, 50, 100); font: bold; font-size: 12px");
        excelOpen->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        excelOpen->setText("[     ]");
        excelOpen->show();
        connect(excelOpen, &QPushButton::clicked, [=]() {
            // createSignal(ivis::common::EventTypeEnum::EventTypeFileSaveAs, QVariant());
        });
    }
}

void GuiTop::updateDisplayDefaultPath() {
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

void GuiTop::updateDisplayConfigInfo() {
    qDebug() << "GuiTop::updateDisplayConfigInfo() -> ConfigInfo :" << mConfigInfo.size();
    mView->setCurrentIndex(0);
    mView->show();

    if (mConfigWidget == nullptr) {
        mConfigWidget = new QWidget(mView);
        mConfigWidget->setGeometry(mMainWindow->geometry());
        mConfigWidget->show();
    } else {
        if (mNodeAddressList) {
            mNodeAddressList->hide();
        }
        mConfigWidget->show();
        return;
    }

    QVariantList allConfig = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeConfigInfo).toList();
    if (allConfig.size() == 0) {
        foreach(const auto& info, mConfigInfo) {
            info->clear();
        }
        mConfigInfo.clear();
        return;
    }

    if (mConfigHideButton == nullptr) {
        QRect rect = mConfigWidget->rect();
        mConfigHideButton = new QPushButton(mConfigWidget);
        rect.setX(rect.width() - 50);
        rect.setY(0);
        rect.setWidth(50);
        rect.setHeight(50);
        mConfigHideButton->setGeometry(rect);
        mConfigHideButton->setStyleSheet("background-color: rgb(255, 255, 255); color: black; font: bold; font-size:20px");
        mConfigHideButton->setText("X");
        mConfigHideButton->setStyleSheet("color: rgb(50, 50, 100)");
        mConfigHideButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        mConfigHideButton->show();
        connect(mConfigHideButton, &QPushButton::clicked, [=]() {
            mView->hide();
        });
    }


    int index = 0;
    foreach(const auto& data, allConfig) {
        QVariantList config = data.toList();
        if (config.size() != 3) {
            continue;
        }
        int type = config.at(0).toInt();
        QString name = config.at(1).toString();
        QVariant value = config.at(2);
        QString realValue = QString();
        switch (value.type()) {
            case QVariant::Type::List : {
                foreach(const auto& v, value.toList()) {
                    realValue.append(QString("%1, ").arg(v.toString()));
                }
                realValue.resize(realValue.size() - 2);
                break;
            }
            case QVariant::Type::StringList : {
                foreach(const auto& v, value.toStringList()) {
                    realValue.append(QString("%1, ").arg(v));
                }
                realValue.resize(realValue.size() - 2);
                break;
            }
            case QVariant::Type::Rect : {
                QRect rect = value.toRect();
                realValue.append(QString("%1, %2, %3, %4").arg(rect.x()).arg(rect.y()).arg(rect.width()).arg(rect.height()));
                break;
            }
            default : {
                realValue = value.toString();
                break;
            }
        }
        ListItem* item = new ListItem(index++, type, name, realValue, mConfigWidget);
        mConfigInfo[mConfigInfo.size()] = item;
        connect(item, &ListItem::signalValueChanged, [=](const int& index, const int& type, const QVariant& value) {
            createSignal(ivis::common::EventTypeEnum::EventTypeChangedConfig, QVariant(QVariantList{type, value}));
        });
    }
}

void GuiTop::updateDisplayNodeAddress() {
    qDebug() << "GuiTop::updateDisplayNodeAddress()";
    mView->setCurrentIndex(1);
    mView->show();

    if (mNodeAddressList == nullptr) {
        QRect rect = mMainWindow->geometry();
        rect.setY(60);
        rect.setHeight(rect.height() - rect.y());
        mNodeAddressList = new QListView(mView);
        mNodeAddressList->setGeometry(rect);
        mNodeAddressList->show();
    } else {
        if (mConfigWidget) {
            mConfigWidget->hide();
        }
        mNodeAddressList->show();
        return;
    }

    QStringList nodeAddress = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeSignalListAll).toStringList();
    QStringListModel* model = new QStringListModel();
    model->setStringList(nodeAddress);
    mNodeAddressList->setModel(model);
    // mNodeAddressList->setItemDelegate(new QStyledItemDelegate(mNodeAddressList));

    static QLineEdit* inputNodeAddress = nullptr;
    if (inputNodeAddress == nullptr) {
        inputNodeAddress = new QLineEdit(mNodeAddressList);
        inputNodeAddress->setGeometry(10, 70, 1260, 30);
        inputNodeAddress->setStyleSheet("background-color: rgb(255, 255, 255); color: black; font: bold; font-size:20px");
        inputNodeAddress->setStyleSheet("color: rgb(50, 50, 100)");
        inputNodeAddress->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        inputNodeAddress->show();
    }
    static QCompleter* autoComplete = nullptr;
    if (autoComplete == nullptr) {
        autoComplete = new QCompleter(nodeAddress, inputNodeAddress);
        autoComplete->setCaseSensitivity(Qt::CaseInsensitive);
        autoComplete->setFilterMode(Qt::MatchContains);
        autoComplete->setWrapAround(false);
        // autoComplete->setCompletionMode(QCompleter::CompletionMode::UnfilteredPopupCompletion);
    }
    inputNodeAddress->setCompleter(autoComplete);





#if 0
    static QListWidget* mView = nullptr;
    if (mView == nullptr) {
        QSize size = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize).toSize();
        QRect rect = mMainWindow->geometry();
        rect.setY(120);
        rect.setWidth(size.width());
        rect.setHeight(size.height() - 120);
        mView = new QListWidget(mMainWindow);
        mView->setGeometry(rect);
        mView->show();
    }
    for (int i = 0; i < 100; ++i) {
        QListWidgetItem *item = new QListWidgetItem(mView);
        QWidget *widget = new QWidget;
        QHBoxLayout *layout = new QHBoxLayout(widget);

        QLabel *label = new QLabel("Item " + QString::number(i));
        // label->setGeometry(0, 0, 100, 50);
        QPushButton *left = new QPushButton("left");
        QPushButton *right = new QPushButton("right");
        layout->addWidget(left);
        layout->addWidget(label);
        layout->addWidget(right);
        layout->addStretch();     // Add stretchable space to align the button to the right

        item->setSizeHint(widget->sizeHint());
        mView->setItemWidget(item, widget);
    }
#endif



#if 0
    static QListWidget* mView = nullptr;
    if (mView == nullptr) {
        QSize size = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize).toSize();
        QRect rect = mMainWindow->geometry();
        rect.setY(120);
        rect.setWidth(size.width());
        rect.setHeight(size.height());
        mView = new QListWidget(mMainWindow);
        mView->setGeometry(rect);
        mView->show();
    }

//    new QListWidgetItem(tr("Oak"), mView);
//    new QListWidgetItem(tr("Fir"), mView);
//    new QListWidgetItem(tr("Pine"), mView);

    QListWidgetItem *newItem1 = new QListWidgetItem();
    QListWidgetItem *newItem2 = new QListWidgetItem();
    QListWidgetItem *newItem3 = new QListWidgetItem();
    QListWidgetItem *newItem4 = new QListWidgetItem();
    newItem1->setText("000000000000000");
    mView->insertItem(0, newItem1);
    newItem2->setText("111111111111111111");
    mView->insertItem(1, newItem2);
    newItem3->setText("222222222222222");
    mView->insertItem(2, newItem3);
    mView->insertItem(3, new QListWidgetItem("33333333333"));
    mView->insertItem(5, new QListWidgetItem("444444444444"));
#endif




#if 0
    static QListView* mView = nullptr;
    if (mView == nullptr) {
        QSize size = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize).toSize();
        QRect rect = mMainWindow->geometry();
        rect.setY(120);
        rect.setWidth(size.width());
        rect.setHeight(size.height());
        mView = new QListView(mMainWindow);
        mView->setGeometry(rect);
        mView->show();
    }

    QStandardItemModel* model = new QStandardItemModel(5, 2);
    QStringList data = QStringList({"Item 1", "Item 2", "Item 3", "Item 4", "Item 5", "Item 6"});

    foreach(auto& d, data) {
        model->appendRow(new QStandardItem(d));
    }
    mView->setModel(model);

    for (int i = 0; i < model->rowCount(); i++) {
        QWidget* widget = new QWidget();
        // QPushButton* left = new QPushButton(widget);
        // left->setGeometry(0, 0, 80, 80);
        // left->setText("Left");
        QPushButton* right = new QPushButton(widget);
        right->setGeometry(200, 0, 80, 80);
        right->setText("Right");
        mView->setIndexWidget(model->index(i, 0), new QPushButton("Right"));
        mView->setIndexWidget(model->index(i, 0), new QPushButton("Right"));
    }
#endif
}

void GuiTop::slotPropertyChanged(const int& type, const QVariant& value) {
    switch (type) {
        case ivis::common::PropertyTypeEnum::PropertyTypeDepth : {
            drawDisplay(value);
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize : {
            updateDisplaySize();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeVisible :
        case ivis::common::PropertyTypeEnum::PropertyTypeSubVisible : {
            updateDisplayVisible();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeDefaultPath : {
            updateDisplayDefaultPath();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeConfigInfo : {
            updateDisplayConfigInfo();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeSignalListAll : {
            updateDisplayNodeAddress();
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
