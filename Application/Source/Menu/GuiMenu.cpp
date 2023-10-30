#include "GuiMenu.h"
#include "AbstractHandler.h"

#include "CommonResource.h"
#include "CommonUtil.h"

#include <QKeySequence>
#include <QLineEdit>
#include <QCompleter>

#include "GuiCenter.h"


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
    drawMenuEdit();
    drawMenuView();
    drawMenuSetting();
#if !defined(USE_VIEW_REPORT)
    drawMenuReport();
#endif
    drawMenuRun();
    drawMenuHelp();
    drawMenuEtc(false);
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

    mAction[MainType::File][STRING_NEW] = new QAction(QIcon::fromTheme("actionNew",
                                                        QIcon(IAMGE_NEW)),
                                                        STRING_NEW,
                                                        this);

    if (mAction[MainType::File][STRING_NEW]) {
        mAction[MainType::File][STRING_NEW]->setShortcuts(QKeySequence::New);
        mAction[MainType::File][STRING_NEW]->setStatusTip(STRING_NEW_TIP);
        mMenu[MainType::File]->addAction(mAction[MainType::File][STRING_NEW]);
        mToolBar[MainType::File]->addAction(mAction[MainType::File][STRING_NEW]);
        connect(mAction[MainType::File][STRING_NEW], &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeFileNew, QVariant());
        });
    }

    mAction[MainType::File][STRING_OPEN] = new QAction(QIcon::fromTheme("actionOpen",
                                                        QIcon(IAMGE_OPEN)),
                                                        STRING_OPEN,
                                                        this);
    if (mAction[MainType::File][STRING_OPEN]) {
        mAction[MainType::File][STRING_OPEN]->setShortcuts(QKeySequence::Open);
        mAction[MainType::File][STRING_OPEN]->setStatusTip(STRING_OPEN_TIP);
        mMenu[MainType::File]->addAction(mAction[MainType::File][STRING_OPEN]);
        mToolBar[MainType::File]->addAction(mAction[MainType::File][STRING_OPEN]);
        connect(mAction[MainType::File][STRING_OPEN], &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeFileOpen, QVariant());
        });
    }

    mAction[MainType::File][STRING_SAVE] = new QAction(QIcon::fromTheme("actionSave",
                                                        QIcon(IAMGE_SAVE)),
                                                        STRING_SAVE,
                                                        this);
    if (mAction[MainType::File][STRING_SAVE]) {
        mAction[MainType::File][STRING_SAVE]->setShortcuts(QKeySequence::Save);
        mAction[MainType::File][STRING_SAVE]->setStatusTip(STRING_SAVE_TIP);
        mMenu[MainType::File]->addAction(mAction[MainType::File][STRING_SAVE]);
        mToolBar[MainType::File]->addAction(mAction[MainType::File][STRING_SAVE]);
        connect(mAction[MainType::File][STRING_SAVE], &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeFileSave, QVariant());
        });
    }

    mAction[MainType::File][STRING_SAVE_AS] = new QAction(QIcon::fromTheme("actionSaveAs"),
                                                        STRING_SAVE_AS,
                                                        this);
    if (mAction[MainType::File][STRING_SAVE_AS]) {
        // mAction[MainType::File][STRING_SAVE_AS]->setDisabled(true);
        mAction[MainType::File][STRING_SAVE_AS]->setShortcuts(QKeySequence::SaveAs);
        mAction[MainType::File][STRING_SAVE_AS]->setStatusTip(STRING_SAVE_AS_TIP);
        mMenu[MainType::File]->addAction(mAction[MainType::File][STRING_SAVE_AS]);
        // mMenu[MainType::File]->setStyleSheet(QString("QMenu::item{background-color : red; color : blue;);}"));
        // qDebug() << "StyelSheet :" << mMenu[MainType::File]->styleSheet();
        // mToolBar[MainType::File]->addAction(mAction[MainType::File][STRING_SAVE_AS]);
        connect(mAction[MainType::File][STRING_SAVE_AS], &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeFileSaveAs, QVariant());
        });
    }

    mMenu[MainType::File]->addSeparator();
    mAction[MainType::File][STRING_SAVE_EXIT] = new QAction(QIcon::fromTheme("actionExit"),
                                                        STRING_SAVE_EXIT,
                                                        this);
    if (mAction[MainType::File][STRING_SAVE_EXIT]) {
        mAction[MainType::File][STRING_SAVE_EXIT]->setShortcuts(QKeySequence::Quit);
        mAction[MainType::File][STRING_SAVE_EXIT]->setStatusTip(STRING_SAVE_EXIT_TIP);
        mMenu[MainType::File]->addAction(mAction[MainType::File][STRING_SAVE_EXIT]);
        // mToolBar[MainType::File]->addAction(mAction[MainType::File][STRING_SAVE_EXIT]);
        connect(mAction[MainType::File][STRING_SAVE_EXIT], &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeExitProgram, QVariant());
        });
    }
}

void GuiMenu::drawMenuEdit() {
    mMenu[MainType::Edit] = mMainView->menuBar()->addMenu(STRING_EDIT);
    mToolBar[MainType::Edit] = mMainView->addToolBar(STRING_EDIT);

#ifndef QT_NO_CLIPBOARD
    mAction[MainType::Edit][STRING_COPY] = new QAction(QIcon::fromTheme("actionCopy",
                                                        QIcon(IAMGE_COPY)),
                                                        STRING_COPY,
                                                        this);
    if (mAction[MainType::Edit][STRING_COPY]) {
        mAction[MainType::Edit][STRING_COPY]->setShortcuts(QKeySequence::Copy);
        mAction[MainType::Edit][STRING_COPY]->setStatusTip(STRING_COPY_TIP);
        mMenu[MainType::Edit]->addAction(mAction[MainType::Edit][STRING_COPY]);
        mToolBar[MainType::Edit]->addAction(mAction[MainType::Edit][STRING_COPY]);
        connect(mAction[MainType::Edit][STRING_COPY], &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeEditCopy, QVariant());
        });
    }

    mAction[MainType::Edit][STRING_CUT] = new QAction(QIcon::fromTheme("actionCut",
                                                        QIcon(IAMGE_CUT)),
                                                        STRING_CUT,
                                                        this);
    if (mAction[MainType::Edit][STRING_CUT]) {
        mAction[MainType::Edit][STRING_CUT]->setShortcuts(QKeySequence::Cut);
        mAction[MainType::Edit][STRING_CUT]->setStatusTip(STRING_CUT_TIP);
        mMenu[MainType::Edit]->addAction(mAction[MainType::Edit][STRING_CUT]);
        mToolBar[MainType::Edit]->addAction(mAction[MainType::Edit][STRING_CUT]);
        connect(mAction[MainType::Edit][STRING_CUT], &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeEditCut, QVariant());
        });
    }

    mAction[MainType::Edit][STRING_PASTE] = new QAction(QIcon::fromTheme("actionPaste",
                                                        QIcon(IAMGE_PASTE)),
                                                        STRING_PASTE,
                                                        this);
    if (mAction[MainType::Edit][STRING_PASTE]) {
        mAction[MainType::Edit][STRING_PASTE]->setShortcuts(QKeySequence::Paste);
        mAction[MainType::Edit][STRING_PASTE]->setStatusTip(STRING_PASTE_TIP);
        mMenu[MainType::Edit]->addAction(mAction[MainType::Edit][STRING_PASTE]);
        mToolBar[MainType::Edit]->addAction(mAction[MainType::Edit][STRING_PASTE]);
        connect(mAction[MainType::Edit][STRING_PASTE], &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeEditPaste, QVariant());
        });
    }

    mMainView->menuBar()->addSeparator();
#endif
}


void GuiMenu::drawMenuView() {
    mMenu[MainType::View] = mMainView->menuBar()->addMenu(STRING_VIEW);
    mToolBar[MainType::View] = mMainView->addToolBar(STRING_VIEW);

    mAction[MainType::View][STRING_CONFIG] = new QAction(QIcon::fromTheme("actionConfig"),
                                                        STRING_CONFIG,
                                                        this);
    if (mAction[MainType::View][STRING_CONFIG]) {
        mAction[MainType::View][STRING_CONFIG]->setStatusTip(STRING_CONFIG_TIP);
        mMenu[MainType::View]->addAction(mAction[MainType::View][STRING_CONFIG]);
        // mToolBar[MainType::View]->addAction(mAction[MainType::View][STRING_CONFIG]);
        connect(mAction[MainType::View][STRING_CONFIG], &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeViewConfig, QVariant());
        });
    }

    mAction[MainType::View][STRING_NODE_ADDRESS] = new QAction(QIcon::fromTheme("actionNodeAddress"),
                                                        STRING_NODE_ADDRESS,
                                                        this);
    if (mAction[MainType::View][STRING_NODE_ADDRESS]) {
        mAction[MainType::View][STRING_NODE_ADDRESS]->setStatusTip(STRING_NODE_ADDRESS_TIP);
        mMenu[MainType::View]->addAction(mAction[MainType::View][STRING_NODE_ADDRESS]);
        mToolBar[MainType::View]->addAction(mAction[MainType::View][STRING_NODE_ADDRESS]);
        connect(mAction[MainType::View][STRING_NODE_ADDRESS], &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeViewNodeAddress, QVariant());
        });
    }

#if defined(USE_VIEW_REPORT)
    mAction[MainType::View][STRING_TEST_REPORT] = new QAction(QIcon::fromTheme("actionTestReport"),
                                                        STRING_TEST_REPORT,
                                                        this);
    if (mAction[MainType::View][STRING_TEST_REPORT]) {
        mAction[MainType::View][STRING_TEST_REPORT]->setStatusTip(STRING_TEST_REPORT_TIP);
        mMenu[MainType::View]->addAction(mAction[MainType::View][STRING_TEST_REPORT]);
        // mToolBar[MainType::View]->addAction(mAction[MainType::View][STRING_TEST_REPORT]);
        connect(mAction[MainType::View][STRING_TEST_REPORT], &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeSettingTestReport, QVariant());
        });

#if 0
        QMenu* mSubMenu = mMenu[MainType::View]->addMenu(STRING_TEST_REPORT);
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
#endif
    }
#endif


#if 0
    QAction *mAction[MainType::View][STRING_PYTHON] = new QAction(QIcon::fromTheme("actionPython"),
                                                        STRING_PYTHON,
                                                        this);
    if (QAction *mAction[MainType::View][STRING_PYTHON]) {
        QAction *mAction[MainType::View][STRING_PYTHON]->setStatusTip(STRING_PYTHON_TIP);
        mMenu[MainType::View]->addAction(QAction *mAction[MainType::View][STRING_PYTHON]);
        // mToolBar[MainType::View]->addAction(QAction *mAction[MainType::View][STRING_PYTHON]);
        connect(QAction *mAction[MainType::View][STRING_PYTHON], &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeViewPythonLib, QVariant());
        });
    }
#endif
}

void GuiMenu::drawMenuSetting() {
    mMenu[MainType::Setting] = mMainView->menuBar()->addMenu(STRING_SETTING);
    // mToolBar[MainType::Setting] = mMainView->addToolBar(STRING_SETTING);

    mAction[MainType::Setting][STRING_DEFAULT_PATH] = new QAction(QIcon::fromTheme("actionDevPath"),
                                                        STRING_DEFAULT_PATH,
                                                        this);
    if (mAction[MainType::Setting][STRING_DEFAULT_PATH]) {
        mAction[MainType::Setting][STRING_DEFAULT_PATH]->setStatusTip(STRING_DEFAULT_PATH_TIP);
        mMenu[MainType::Setting]->addAction(mAction[MainType::Setting][STRING_DEFAULT_PATH]);
        // mToolBar[MainType::Setting]->addAction(mAction[MainType::Setting][STRING_DEFAULT_PATH]);
        connect(mAction[MainType::Setting][STRING_DEFAULT_PATH], &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeSettingDevPath, QVariant());
        });
    }

    mAction[MainType::Setting][STRING_VSM_PATH] = new QAction(QIcon::fromTheme("actionVsmPath"),
                                                        STRING_VSM_PATH,
                                                        this);
    if (mAction[MainType::Setting][STRING_VSM_PATH]) {
        mAction[MainType::Setting][STRING_VSM_PATH]->setStatusTip(STRING_VSM_PATH_TIP);
        mMenu[MainType::Setting]->addAction(mAction[MainType::Setting][STRING_VSM_PATH]);
        // mToolBar[MainType::Setting]->addAction(mAction[MainType::Setting][STRING_VSM_PATH]);
        connect(mAction[MainType::Setting][STRING_VSM_PATH], &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeSettingVsmPath, QVariant());
        });
    }

#if 0
    mAction[MainType::Setting][STRING_NODE_PATH] = new QAction(QIcon::fromTheme("actionNodePath"),
                                                        STRING_NODE_PATH,
                                                        this);
    if (mAction[MainType::Setting][STRING_NODE_PATH]) {
        actionNodmAction[MainType::Setting][STRING_NODE_PATH]ePath->setStatusTip(STRING_NODE_PATH_TIP);
        mMenu[MainType::Setting]->addAction(mAction[MainType::Setting][STRING_NODE_PATH]);
        // mToolBar[MainType::Setting]->addAction(mAction[MainType::Setting][AAAAASTRING_NODE_PATHAAAAAAAA]);
        connect(mAction[MainType::Setting][STRING_NODE_PATH], &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeSettingNodePath, QVariant());
        });
    }
#endif
#if 0
    TooBar - Push Button
    QPushButton* buttonTestResult = new QPushButton(QString("TestResult"));
    if (buttonTestResult) {
        buttonTestResult->setObjectName(QString("TestResult"));
        mToolBar[MainType::Setting]->addWidget(buttonTestResult);
        connect(buttonTestResult, &QPushButton::clicked, this, [=]() {
            qDebug() << "Test Result";
        });
    }
    QPushButton* buttonTestCoverage = new QPushButton(QString("TestCoverage"));
    if (buttonTestCoverage) {
        buttonTestResult->setObjectName(QString("TestConverage"));
        mToolBar[MainType::Setting]->addWidget(buttonTestCoverage);
        connect(buttonTestCoverage, &QPushButton::clicked, this, [=]() {
            qDebug() << "Test Coverage";
        });
    }
#endif

    mMainView->menuBar()->addSeparator();
}

void GuiMenu::drawMenuReport() {
    mMenu[MainType::Report] = mMainView->menuBar()->addMenu(STRING_REPORT);
    // mToolBar[MainType::Report] = mMainView->addToolBar(STRING_REPORT);

    mAction[MainType::Report][STRING_COVERAGE] = new QAction(QIcon::fromTheme("actionReportResult"),
                                                        STRING_COVERAGE,
                                                        this);
    if (mAction[MainType::Report][STRING_COVERAGE]) {
        mAction[MainType::Report][STRING_COVERAGE]->setStatusTip(STRING_RESULT_TIP);
        mMenu[MainType::Report]->addAction(mAction[MainType::Report][STRING_COVERAGE]);
        // mToolBar[MainType::Report]->addAction(mAction[MainType::Report][STRING_COVERAGE]);
        connect(mAction[MainType::Report][STRING_COVERAGE], &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeReportResult, QVariant());
        });
    }

    mAction[MainType::Report][STRING_COVERAGE] = new QAction(QIcon::fromTheme("actionReportCoverage"),
                                                        STRING_COVERAGE,
                                                        this);
    if (mAction[MainType::Report][STRING_COVERAGE]) {
        mAction[MainType::Report][STRING_COVERAGE]->setStatusTip(STRING_COVERAGE_TIP);
        mMenu[MainType::Report]->addAction(mAction[MainType::Report][STRING_COVERAGE]);
        // mToolBar[MainType::Report]->addAction(mAction[MainType::Report][STRING_COVERAGE]);
        connect(mAction[MainType::Report][STRING_COVERAGE], &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeReportCoverage, QVariant());
        });
    }
}

void GuiMenu::drawMenuRun() {
    mMenu[MainType::Run] = mMainView->menuBar()->addMenu(STRING_RUN);
    // mToolBar[MainType::Run] = mMainView->addToolBar(STRING_RUN);

    mAction[MainType::Run][STRING_GENERATE_TC] = new QAction(QIcon::fromTheme("actionGenTC"),
                                                        STRING_GENERATE_TC,
                                                        this);
    if (mAction[MainType::Run][STRING_GENERATE_TC]) {
        mAction[MainType::Run][STRING_GENERATE_TC]->setStatusTip(STRING_GENERATE_TC_TIP);
        mMenu[MainType::Run]->addAction(mAction[MainType::Run][STRING_GENERATE_TC]);
        // mToolBar[MainType::Run]->addAction(mAction[MainType::Run][STRING_GENERATE_TC]);
        connect(mAction[MainType::Run][STRING_GENERATE_TC], &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeGenerateTC, QVariant());
        });
    }

    mAction[MainType::Run][STRING_RUN_TC] = new QAction(QIcon::fromTheme("actionRun"),
                                                        STRING_RUN_TC,
                                                        this);
    if (mAction[MainType::Run][STRING_RUN_TC]) {
        mAction[MainType::Run][STRING_RUN_TC]->setStatusTip(STRING_RUN_TC_TIP);
        mMenu[MainType::Run]->addAction(mAction[MainType::Run][STRING_RUN_TC]);
        // mToolBar[MainType::Run]->addAction(mAction[MainType::Run][STRING_RUN_TC]);
        connect(mAction[MainType::Run][STRING_RUN_TC], &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeRunTC, QVariant());
        });
    }

    mAction[MainType::Run][STRING_GENERATE_REPORT] = new QAction(QIcon::fromTheme("actionGenReport"),
                                                        STRING_GENERATE_REPORT,
                                                        this);
    if (mAction[MainType::Run][STRING_GENERATE_REPORT]) {
        mAction[MainType::Run][STRING_GENERATE_REPORT]->setStatusTip(STRING_GENERATE_REPORT_TIP);
        mMenu[MainType::Run]->addAction(mAction[MainType::Run][STRING_GENERATE_REPORT]);
        // mToolBar[MainType::Run]->addAction(mAction[MainType::Run][STRING_GENERATE_REPORT]);
        connect(mAction[MainType::Run][STRING_GENERATE_REPORT], &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeGenerateReport, QVariant());
        });
    }
}

void GuiMenu::drawMenuHelp() {
    mMenu[MainType::Help] = mMainView->menuBar()->addMenu(STRING_HELP);

    mAction[MainType::Help][STRING_ABOUT] = new QAction(QIcon::fromTheme("actionAbout"),
                                                        STRING_ABOUT,
                                                        this);
    if (mAction[MainType::Help][STRING_ABOUT]) {
        mAction[MainType::Help][STRING_ABOUT]->setStatusTip(STRING_ABOUT_TIP);
        mMenu[MainType::Help]->addAction(mAction[MainType::Help][STRING_ABOUT]);
        connect(mAction[MainType::Help][STRING_ABOUT], &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeHelpAbout, QVariant());
        });
    }

    mAction[MainType::Help][STRING_ABOUT_QT] = new QAction(QIcon::fromTheme("actionAboutQt"),
                                                        STRING_ABOUT_QT,
                                                        this);
    if (mAction[MainType::Help][STRING_ABOUT_QT]) {
        mAction[MainType::Help][STRING_ABOUT_QT]->setStatusTip(STRING_ABOUT_QT_TIP);
        mMenu[MainType::Help]->addAction(mAction[MainType::Help][STRING_ABOUT_QT]);
        connect(mAction[MainType::Help][STRING_ABOUT_QT], &QAction::triggered, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeHelpAboutQt, QVariant());
        });
    }
}

void GuiMenu::drawMenuEtc(const bool& update) {
    QString styleInfo = QString("background-color: rgb(255, 255, 255); color: rgb(50, 50, 100); font: bold; font-size: 12px");

    if (mDefaultPath == nullptr) {
        mDefaultPath = ivis::common::createWidget<QPushButton>(mMainView, true, QRect(350, 25, 600, 30), styleInfo);
        connect(mDefaultPath, &QPushButton::clicked, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeSettingDevPath, QVariant());
        });
    }
    QVariant path = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeDefaultPath);
    mDefaultPath->setText(QString("Path : %1").arg(path.toString()));


#if !defined(USE_DEMO)
    if (update == false) {
        static QPushButton* lastFile = nullptr;
        if (lastFile == nullptr) {
            lastFile = ivis::common::createWidget<QPushButton>(mMainView, true, QRect(1000, 25, 50, 30), styleInfo);
            lastFile->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            lastFile->setText("Last");
            connect(lastFile, &QPushButton::clicked, [=]() {
                createSignal(ivis::common::EventTypeEnum::EventTypeLastFile, QVariant());
            });
        }

        static QPushButton* excelOpen = nullptr;
        if (excelOpen == nullptr) {
            excelOpen = ivis::common::createWidget<QPushButton>(mMainView, true, QRect(1060, 25, 50, 30), styleInfo);
            excelOpen->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            excelOpen->setText("Test");
            connect(excelOpen, &QPushButton::clicked, [=]() {
                createSignal(ivis::common::EventTypeEnum::EventTypeTest, QVariant());
            });
        }
    }
#endif
}

void GuiMenu::updateDisplaySelectModule(const int& type) {
    qDebug() << "updateDisplaySelectModule :" << type;

    if (mSelectModule == nullptr) {
        QVariant moduleList = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeAllModuleList);
        QVariant selectModule = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSelectModule);
        bool allState = (moduleList.toStringList().size() == selectModule.toStringList().size());
        mSelectModule = new SelectModuleDialog(isHandler()->getScreen(), moduleList.toStringList(), allState);
        mSelectModule->updateSelectModule(selectModule.toStringList());

        connect(mSelectModule, &SelectModuleDialog::signalModuleSelected,
                                                        [=](const QList<QPair<int, QString>>& selectModule) {
            QVariantList moduleSelect = QVariantList();
            foreach(const auto& select, selectModule) {
                moduleSelect.append(QVariant(select.second));
            }
            updateDisplaySelectPT(type, moduleSelect);
        });
        connect(mSelectModule, &QDialog::finished, [=]() {
            disconnect(mSelectModule);
            delete mSelectModule;
            mSelectModule = nullptr;
        });
    }
    mSelectModule->show();
}

void GuiMenu::updateDisplaySelectPT(const int& type, const QVariantList& moduleList) {
    if (mSelectPt == nullptr) {
        int eventType = ivis::common::EventTypeEnum::EventTypeSelectModuleOfGenTC;
        QStringList itemList = QStringList({"Negative"});
        if (type == ivis::common::PropertyTypeEnum::PropertyTypeSelectModuleOfRunTC) {
            eventType = ivis::common::EventTypeEnum::EventTypeSelectModuleOfRunTC;
            itemList = QStringList({"ICV", "FCEV", "EV"});
        }
        mSelectPt = new SelectPtDialog(isHandler()->getScreen(), itemList);

        connect(mSelectPt, &SelectPtDialog::signalPtSelected, [=](const QList<QPair<QString, bool>>& checkStateList) {
            QVariantList checkList = QVariantList();
            foreach(const auto& check, checkStateList) {
                if (check.second) {
                    checkList.append(QVariant(check.first));
                }
            }
            createSignal(eventType, QVariant({moduleList, checkList}));
            if (mSelectModule) {
                mSelectModule->finished(true);
            }
            mSelectPt->hide();
            mSelectPt->finished(true);
        });
        connect(mSelectPt, &QDialog::finished, [=]() {
            disconnect(mSelectPt);
            delete mSelectPt;
            mSelectPt = nullptr;
        });
    }
    mSelectPt->show();
}

void GuiMenu::updateDisplayTestResultInfo() {
    QVariantList testResultInfo = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeTestResultInfo).toList();

    if (testResultInfo.size() != 3) {
        qDebug() << "Fail to test result info size :" << testResultInfo.size();
        return;
    }

    QVariantList countInfo = testResultInfo.at(0).toList();
    if (countInfo.size() != 3) {
        qDebug() << "Fail to count info size :" << countInfo.size() << countInfo;
        return;
    }
    int current = countInfo.at(0).toInt();
    int total = countInfo.at(1).toInt();
    bool complete = countInfo.at(2).toBool();

    if (mProgressBar == nullptr) {
        mProgressBar = ivis::common::createWidget<QProgressBar>(isHandler()->getScreen(), false, QRect(1100, 25, 100, 30));
        ivis::common::widgetVisible<QProgressBar>(mProgressBar, true);
    }
    mProgressBar->setRange(0, total);
    mProgressBar->setValue(current);

    QString errorInfo = testResultInfo.at(1).toString();
    QString moduleStateInfo = QString();
    foreach(const auto& info, testResultInfo.at(2).toList()) {
        moduleStateInfo.append(info.toString() + "\n");
    }

    // qDebug() << "\t " << testResultInfo;
    // qDebug() << "\t [0] :" << testResultInfo.at(0).toList().size() << testResultInfo.at(0);
    // qDebug() << "\t [1] :" << testResultInfo.at(1).toString().size() << testResultInfo.at(1);
    // qDebug() << "\t [2] :" << testResultInfo.at(2).toList().size() << testResultInfo.at(2);

    if (mLogDisplay == nullptr) {
        mLogDisplay = new LogDisplayDialog(isHandler()->getScreen(), QString("Test Result Info"), errorInfo, moduleStateInfo);

        connect(mLogDisplay, &LogDisplayDialog::signalTestResultClick, [=](const bool& cancel) {
            if (cancel) {
                createSignal(ivis::common::EventTypeEnum::EventTypeTestResultClick, true);
            }
            mLogDisplay->hide();
            mLogDisplay->finished(true);
        });
        connect(mLogDisplay, &QDialog::finished, [=]() {
            if (mProgressBar) {
                delete mProgressBar;
                mProgressBar = nullptr;
            }
            disconnect(mLogDisplay);
            delete mLogDisplay;
            mLogDisplay = nullptr;
        });
        mLogDisplay->show();
    }
    mLogDisplay->updateLogDisplay(errorInfo, moduleStateInfo);
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
            drawMenuEtc(true);
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeSelectModuleOfGenTC :
        case ivis::common::PropertyTypeEnum::PropertyTypeSelectModuleOfRunTC : {
            bool update = value.toBool();
            if (update) {
                updateDisplaySelectModule(type);
            }
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeTestResultInfo : {
            updateDisplayTestResultInfo();
            break;
        }
        default : {
            break;
        }
    }
}
