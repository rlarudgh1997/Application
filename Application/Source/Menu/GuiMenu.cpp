#include "GuiMenu.h"
#include "AbstractHandler.h"

#include "CommonResource.h"
#include "CommonUtil.h"

#include <QKeySequence>
#include <QLineEdit>
#include <QCompleter>

#include "GuiCenter.h"
#include "ConfigSetting.h"
#include "Dialog.h"

QSharedPointer<GuiMenu>& GuiMenu::instance(AbstractHandler* handler) {
    static QSharedPointer<GuiMenu> gGui;
    if (gGui.isNull()) {
        gGui = QSharedPointer<GuiMenu>(new GuiMenu(handler));
    }
    return gGui;
}

GuiMenu::GuiMenu(AbstractHandler* handler) : AbstractGui(handler), mGui(new Ui::GuiMenu) {
    mMainView = new QMainWindow();
    mMainView->setParent(isHandler()->getScreen());  // 부팅시 윈도우창 만들기
    mGui->setupUi(mMainView);
    updateDisplaySize();
}

void GuiMenu::drawDisplayDepth0() {
    drawMenuFile();
    drawMenuEdit();
    drawMenuView();
    drawMenuSetting();
    drawMenuRun();
    drawMenuDocker();
    drawMenuHelp();
    drawMenuEtc();
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
    mMainView->setVisible(isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeVisible).toBool());
}

void GuiMenu::drawMenuFile() {
    connect(mGui->actionNew, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeFileNew, QVariant()); });
    connect(mGui->actionOpen, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeFileOpen, QVariant()); });
    connect(mGui->actionSave, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeFileSave, QVariant()); });
    connect(mGui->actionSaveAs, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeFileSaveAs, QVariant()); });
    connect(mGui->actionExit, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeExitProgram, QVariant()); });
}

void GuiMenu::drawMenuEdit() {
    connect(mGui->actionCopy, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeEditCopy, QVariant()); });
    connect(mGui->actionCut, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeEditCut, QVariant()); });
    connect(mGui->actionPaste, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeEditPaste, QVariant()); });
    connect(mGui->actionInsert, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeEditCellInsert, QVariant()); });
    connect(mGui->actionDelete, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeEditCellDelete, QVariant()); });
    connect(mGui->actionMergeSplit, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeEditCellMergeSplit, QVariant()); });
    connect(mGui->actionUndo, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeEditUndo, QVariant()); });
    connect(mGui->actionRedo, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeEditRedo, QVariant()); });
}

void GuiMenu::drawMenuView() {
    connect(mGui->actionConfig, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeViewConfig, QVariant()); });
    connect(mGui->actionNodeView, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeViewNodeAddress, QVariant()); });
}

void GuiMenu::drawMenuSetting() {
    connect(mGui->actionAppMode, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeSettingAppMode, QVariant()); });
    connect(mGui->actionSFCModelPath, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeSettingSfcModelPath, QVariant()); });
}

void GuiMenu::drawMenuRun() {
    connect(mGui->actionGenTC, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeGenTC, QVariant()); });
    connect(mGui->actionRunTC, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeRunTC, QVariant()); });
    connect(mGui->actionTCReport, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeTestReportResult, QVariant()); });
    connect(mGui->actionGCOVReport, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeTestReportCoverage, QVariant()); });
    connect(mGui->actionEnterScriptText, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeEnterScriptText, QVariant()); });
    connect(mGui->actionGenSSFS, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeGenSSFS, QVariant()); });
    connect(mGui->actionViewScriptLog, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeViewLogFile, QVariant()); });
}

void GuiMenu::drawMenuDocker() {
}

void GuiMenu::drawMenuHelp() {
    connect(mGui->actionAbout, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeHelpAbout, QVariant()); });
    connect(mGui->actionAboutQT, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeHelpAboutQt, QVariant()); });
}

void GuiMenu::drawMenuEtc() {
    // Model Path
    updateDisplayPath();
    connect(mGui->actionPath, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeSettingSfcModelPath, QVariant()); });

    // Test Result Info
    mGui->TestResult->setGeometry(QRect(1000, 23, 150, 30));
    mGui->TestResult->setParent(mMainView);
    updateDisplayProgressBar(false, QVariantList());
    connect(mGui->TestResultView, &QPushButton::clicked, [=]() { updateDisplayTestResultInfo(); });
}

void GuiMenu::updateDrawDialog(const int& dialogType, const QVariantList& info) {
    if (mDialog.isNull()) {
        QRect rect = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeScreenInfo).toRect();
        mDialog = QSharedPointer<Dialog>(new Dialog(rect, isHandler()->getScreen()));

        connect(mDialog.data(), &QDialog::finished, [=]() {
            int dialogType = mDialog.data()->getDialogType();
            int prevDialogType = mDialog.data()->getPrevDialogType();
            QVariantList prevDialogInfo = mDialog.data()->getPrevDialogInfo();
            qDebug() << "\t Info :" << dialogType << prevDialogType << prevDialogInfo.size();

            disconnect(mDialog.data(), nullptr, nullptr, nullptr);
            mDialog.reset();

            switch (dialogType) {
                case Dialog::DialogTypeLogDisplay: {
                    updateDisplayProgressBar((getTestResultComplted() == false), QVariantList());
                    break;
                }
                case Dialog::DialogTypeViewLogInfo: {
                    if (prevDialogType == Dialog::DialogTypeLogDisplay) {
                        updateDisplayTestResultInfo();
                    }
                    break;
                }
                case Dialog::DialogTypeViewLogFileInfo: {
                    if (prevDialogType == Dialog::DialogTypeSelectLogFile) {
                        updateDisplayViewLogFileList();
                    }
                    break;
                }
                default: {
                    break;
                }
            }
        });
        connect(mDialog.data(), &Dialog::signalSelectAppMode,
                [=](const int& appMode) { createSignal(ivis::common::EventTypeEnum::EventTypeSelectAppMode, appMode); });
        connect(mDialog.data(), &Dialog::signalSelectListItem, [=](const QList<QPair<int, QString>>& selectItem) {
            int dialogType = mDialog.data()->getDialogType();
            if (dialogType == Dialog::DialogTypeSelectMoudleInfo) {
                QVariantList selectModule = QVariantList();
                for (const auto& select : selectItem) {
                    selectModule.append(QVariant(select.second));
                }
                if (selectModule.size() == 0) {
                    createSignal(ivis::common::EventTypeEnum::EventTypeSelectModuleError, QVariant());
                } else {
                    setSelectModuleList(selectModule);
                    updateDisplaySelectOption();
                }
            } else if (dialogType == Dialog::DialogTypeSelectLogFile) {
                if (selectItem.size() == 1) {
                    // mDialog.data()->accept();
                    // mDialog.data()->setVisible(false);
                    createSignal(ivis::common::EventTypeEnum::EventTypeViewLogDisplay, QVariant(selectItem.at(0).second));
                }
            } else {
            }
        });
        connect(
            mDialog.data(), &Dialog::signalSelectOption, [=](const bool& option1, const QList<QPair<QString, bool>>& option2) {
                int dialogType = mDialog.data()->getDialogType();
                if ((dialogType == Dialog::DialogTypeTestReportTC) || (dialogType == Dialog::DialogTypeTestReportGCOV)) {
                    QVariantList reportInfo = QVariantList();
                    if (dialogType == Dialog::DialogTypeTestReportTC) {
                        reportInfo.append(ivis::common::TestReportTypeEnum::TestReportTypeTC);
                    } else {
                        reportInfo.append(ivis::common::TestReportTypeEnum::TestReportTypeGCOV);
                    }
                    reportInfo.append(option1);
                    for (const auto& option : option2) {
                        reportInfo.append(option.second);
                    }
                    createSignal(ivis::common::EventTypeEnum::EventTypeRunTestReport, reportInfo);
                } else {
                    int runType = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeSelectModuleOfRun).toInt();
                    QVariantList selectModule = getSelectModuleList();
                    QVariantList checkList = QVariantList();
                    for (const auto& check : option2) {
                        if (check.second) {
                            checkList.append(QVariant(check.first));
                        }
                    }
                    QVariantList info({runType, option1, selectModule, checkList});
                    createSignal(ivis::common::EventTypeEnum::EventTypeSelectModuleOfRun, info);
                }
            });
        connect(mDialog.data(), &Dialog::signalEnterTextChanged, [=](const QString& text) {
            // mDialog.data()->accept();
            // mDialog.data()->setVisible(false);
            createSignal(ivis::common::EventTypeEnum::EventTypeEnterScriptTextCompleted, text);
        });
        connect(mDialog.data(), &Dialog::signalLogDisplayClicked, [=](const bool& hide, const bool& detail) {
            if ((hide == true) && (detail == false)) {  // Close
                updateDisplayProgressBar((getTestResultComplted() == false), QVariantList());
            } else if ((hide == false) && (detail == false)) {  // Cancel
                createSignal(ivis::common::EventTypeEnum::EventTypeGenRunTCCancel, getTestResultComplted());
            } else if ((hide == true) && (detail == true)) {  // Detail
                updateDisplayViewLogInfo(true);
            } else {
            }
        });
    }
    mDialog.data()->drawDialog(dialogType, info);
}

void GuiMenu::updateDisplaySelectModule() {
    int runType = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeSelectModuleOfRun).toInt();
    if (runType == ivis::common::RunTypeEnum::RunTypeEnterScriptText) {
        return;
    }

#if defined(USE_DIALOG_OLD)
    if (mSelectModule == nullptr) {
        QVariant moduleList = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeAllModuleList);
        QVariant selectModule = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSelectModule);
        bool allState = (moduleList.toStringList().size() == selectModule.toStringList().size());
        mSelectModule = new SelectModuleDialog(isHandler()->getScreen(), moduleList.toStringList(), allState, true);
        mSelectModule->updateSelectModule(selectModule.toStringList());

        connect(mSelectModule, &SelectModuleDialog::signalModuleSelected, [=](const QList<QPair<int, QString>>& selectModule) {
            QVariantList moduleSelect = QVariantList();
            for (const auto& select : selectModule) {
                moduleSelect.append(QVariant(select.second));
            }
            if (moduleSelect.size() == 0) {
                createSignal(ivis::common::EventTypeEnum::EventTypeSelectModuleError, QVariant());
            } else {
                setSelectModuleList(moduleSelect);
                updateDisplaySelectOption();
            }
        });
        connect(mSelectModule, &QDialog::finished, [=]() {
            disconnect(mSelectModule);
            delete mSelectModule;
            mSelectModule = nullptr;
        });
    }
    mSelectModule->show();
#else
    QVariantList info = QVariantList({
        QString("Select Module"),
        QStringList({"Module"}),
        isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeAllModuleList).toStringList(),
        QStringList(),
        QVariantList(),
        0,
    });
    updateDrawDialog(Dialog::DialogTypeSelectMoudleInfo, info);
#endif
}

void GuiMenu::updateDisplaySelectOption() {
#if defined(USE_DIALOG_OLD)
    if (mCheckBoxGroup == nullptr) {
        int runType = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeSelectModuleOfRun).toInt();
        QString title = QString("Select Negative");
        QString item = QString("Negative");
        QStringList itemList = QStringList();
        if (runType == ivis::common::RunTypeEnum::RunTypeRunTC) {
            title = QString("Select PT");
            item = QString("Docker");
            itemList = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeVehicleType).toStringList();
        }
        mCheckBoxGroup = new CheckBoxGroupDialog(isHandler()->getScreen(), title, item, itemList);

        connect(mCheckBoxGroup, &CheckBoxGroupDialog::signalPtSelected,
                [=](const bool& option1, const QList<QPair<QString, bool>>& checkStateList) {
                    QVariantList checkList = QVariantList();
                    for (const auto& check : checkStateList) {
                        if (check.second) {
                            checkList.append(QVariant(check.first));
                        }
                    }
#if defined(USE_DIALOG_OLD)
                    if (mSelectModule) {
                        mSelectModule->finished(true);
                    }
#else
                    if (mDialog) {
                        mDialog->finished(true);
                    }
#endif
                    mCheckBoxGroup->hide();
                    mCheckBoxGroup->finished(true);
                    createSignal(ivis::common::EventTypeEnum::EventTypeSelectModuleOfRun,
                                 QVariant({runType, option1, getSelectModuleList(), checkList}));
                });
        connect(mCheckBoxGroup, &QDialog::finished, [=]() {
            disconnect(mCheckBoxGroup);
            delete mCheckBoxGroup;
            mCheckBoxGroup = nullptr;
        });
    }
    mCheckBoxGroup->show();
#else
    int runType = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeSelectModuleOfRun).toInt();
    int dialogType = Dialog::DialogTypeSelectNegative;
    QString title = QString("Select Negative");
    QString option1 = QString("Negative");
    QStringList option2 = QStringList();
    if (runType == ivis::common::RunTypeEnum::RunTypeRunTC) {
        int appMode = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeAppMode).toInt();
        dialogType = (appMode == ivis::common::AppModeEnum::AppModeTypePV) ? (Dialog::DialogTypeSelectVehiclePV)
                                                                           : (Dialog::DialogTypeSelectVehicleCV);
        title = QString("Select PT");
        option1 = QString("Docker");
        option2 = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeVehicleType).toStringList();
    }
    QVariantList info = QVariantList({
        title,
        option1,
        option2,
    });
    updateDrawDialog(dialogType, info);
#endif
}

void GuiMenu::updateDisplayPath() {
    QVariant path = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeSfcModelPath);
    mGui->actionPath->setText(QString("Path : %1").arg(path.toString()));
    mGui->actionPath->setIconText(QString("Path : %1").arg(path.toString()));
}

void GuiMenu::updateDisplayProgressBar(const bool& show, const QVariantList& progressInfo) {
#if defined(USE_DIALOG_OLD)
    // Test Result Info : Visible
    if (mTestResultInfo == nullptr) {
        mTestResultInfo = ivis::common::createWidget<QPushButton>(isHandler()->getScreen(), true, QRect(1100, 25, 100, 30));
        mTestResultInfo->setText("Test Result Info");
        connect(mTestResultInfo, &QPushButton::clicked, [=]() {
            mTestResultInfo->hide();
            if (mLogDisplay) {
                mLogDisplay->show();
            }
            if (mProgressBar) {
                mProgressBar->show();
            }
        });
    }
    mTestResultInfo->hide();

    if (mProgressBar == nullptr) {
        mProgressBar = ivis::common::createWidget<QProgressBar>(isHandler()->getScreen(), true, QRect(1100, 25, 100, 30));
    }
    mProgressBar->setRange(0, total);
    mProgressBar->setValue(current);
#else
    if (progressInfo.size() == 2) {
        int current = progressInfo.at(0).toInt();
        int total = progressInfo.at(1).toInt();
        mGui->TestResultProgressBar->setRange(0, total);
        mGui->TestResultProgressBar->setValue(current);
        mGui->TestResultProgressBar->raise();
    } else {
        mGui->TestResultView->raise();
    }
    mGui->TestResult->setVisible(show);
#endif
}

void GuiMenu::updateDisplayTestResultInfo() {
    QVariantList testResultInfo = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeTestResultInfo).toList();
    // bool runState = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeRunScriptState).toBool();

    if (testResultInfo.size() != 3) {
        qDebug() << "Fail to test result info size :" << testResultInfo.size();
        return;
    }

    QVariantList countInfo = testResultInfo.at(0).toList();
    if (countInfo.size() != 3) {
        qDebug() << "Fail to count info size :" << countInfo.size() << countInfo;
        return;
    }
    // Test Result Info : Parsing Value Info
    int current = countInfo.at(0).toInt();
    int total = countInfo.at(1).toInt();
    setTestResultComplted(countInfo.at(2).toBool());

    QString titleInfo = QString();
    for (const auto& info : testResultInfo.at(1).toList()) {
        titleInfo.append(info.toString() + "\n");
    }
    QString errorInfo = QString();
    if (testResultInfo.at(1).toList().size() == 2) {
        errorInfo = testResultInfo.at(1).toList().at(1).toString();
    }
    QString moduleStateInfo = QString();
    for (const auto& info : testResultInfo.at(2).toList()) {
        moduleStateInfo.append(info.toString() + "\n");
    }
    // qDebug() << "\t " << testResultInfo;
    // qDebug() << "\t [0] :" << testResultInfo.at(0).toList().size() << testResultInfo.at(0);
    // qDebug() << "\t [1] :" << testResultInfo.at(1).toList().size() << testResultInfo.at(1);
    // qDebug() << "\t [2] :" << testResultInfo.at(2).toList().size() << testResultInfo.at(2);

#if defined(USE_DIALOG_OLD)
    // Test Result Info
    if (mLogDisplay == nullptr) {
        mLogDisplay = new LogDisplayDialog(isHandler()->getScreen(), QString("Test Result Info"));
        connect(mLogDisplay, &LogDisplayDialog::signalTestResultClick, [=](const bool& cancel) {
            if (cancel) {
                createSignal(ivis::common::EventTypeEnum::EventTypeGenRunTCCancel, getTestResultComplted());
            } else {
                bool runState = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeRunScriptState).toBool();
                mLogDisplay->hide();
                if ((mTestResultInfo) && (runState)) {
                    mTestResultInfo->show();
                }
                if (runState == false) {
                    mLogDisplay->finished(true);
                }
            }
        });
        connect(mLogDisplay, &LogDisplayDialog::signalDetailClicked,
                [=](const bool& clicked) { updateDisplayViewLogInfo(true); });
        connect(mLogDisplay, &QDialog::finished, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeGenRunTCCompleted, true);

            bool runState = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeRunScriptState).toBool();
            if (runState) {
                if (mTestResultInfo) {
                    mTestResultInfo->show();
                }
                return;
            }
            if (mDetailLog) {
                delete mDetailLog;
                mDetailLog = nullptr;
            }
            if (mProgressBar) {
                delete mProgressBar;
                mProgressBar = nullptr;
            }
            if (mTestResultInfo) {
                delete mTestResultInfo;
                mTestResultInfo = nullptr;
            }
            disconnect(mLogDisplay);
            delete mLogDisplay;
            mLogDisplay = nullptr;
        });
    }
    mLogDisplay->updateLogDisplay(titleInfo, errorInfo, moduleStateInfo);
    mLogDisplay->show();
#else
    QVariantList info = QVariantList({QString("Test Result Info"), titleInfo, errorInfo, moduleStateInfo});
    updateDrawDialog(Dialog::DialogTypeLogDisplay, info);
#endif

    // Test Result Info : ProgressBar
    setProgressInfo(QVariantList({current, total}));
    updateDisplayProgressBar(true, getProgressInfo());
}

void GuiMenu::updateDisplayEnterScriptText() {
#if defined(USE_DIALOG_OLD)
    if (mTextEnter == nullptr) {
        mTextEnter = new TextEnterDialog(isHandler()->getScreen());

        connect(mTextEnter, &TextEnterDialog::signalEnterTextCompletd, [=](const QString& text) {
            createSignal(ivis::common::EventTypeEnum::EventTypeEnterScriptTextCompleted, text);
            mTextEnter->hide();
            mTextEnter->finished(true);
        });
        connect(mTextEnter, &QDialog::finished, [=]() {
            disconnect(mTextEnter);
            delete mTextEnter;
            mTextEnter = nullptr;
        });
    }
    mTextEnter->show();
#else
    QVariantList info = QVariantList({
        QString("Enter Script Text"),
    });
    updateDrawDialog(Dialog::DialogTypeEnterText, info);
#endif
}

void GuiMenu::updateDisplayTestReport() {
#if defined(USE_DIALOG_OLD)
    if (mTestReport == nullptr) {
        QVariantList reportInfo = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeTestReport).toList();
        if (reportInfo.size() != 5) {
            qDebug() << "Fail to report info size :" << reportInfo.size();
        }

        int testReportType = reportInfo.at(0).toInt();
        bool state = reportInfo.at(1).toBool();
        bool option1 = reportInfo.at(2).toBool();
        bool option2 = reportInfo.at(3).toBool();
        bool option3 = reportInfo.at(4).toBool();

        QString title = QString();
        QList<QPair<bool, QString>> options = QList<QPair<bool, QString>>();
        if (testReportType == static_cast<int>(ivis::common::TestReportTypeEnum::TestReportTypeTC)) {
            title = QString("TC Report");
            options.append(QPair<bool, QString>(option1, QString("Split")));
            options.append(QPair<bool, QString>(option2, QString("Config")));
            options.append(QPair<bool, QString>(option3, QString("Excel")));
        } else {
            title = QString("GCOV Report");
            options.append(QPair<bool, QString>(option2, QString("Function")));
            options.append(QPair<bool, QString>(option3, QString("Branch")));
            options.append(QPair<bool, QString>(true, QString("Line")));
        }
        mTestReport = new SelectReportDialog(isHandler()->getScreen(), title, state, options);
        connect(mTestReport, &SelectReportDialog::signalOptionSelected,
                [=](const bool& state, const QList<QPair<QString, bool>>& options) {
                    QVariantList reportInfo = QVariantList();
                    reportInfo.append(testReportType);
                    reportInfo.append(state);
                    for (const auto& option : options) {
                        reportInfo.append(option.second);
                    }
                    createSignal(ivis::common::EventTypeEnum::EventTypeRunTestReport, reportInfo);
                    mTestReport->hide();
                    mTestReport->finished(true);
                });
        connect(mTestReport, &QDialog::finished, [=]() {
            disconnect(mTestReport);
            delete mTestReport;
            mTestReport = nullptr;
        });
    }
    mTestReport->show();
#else
    QVariantList reportInfo = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeTestReport).toList();
    if (reportInfo.size() != 5) {
        qDebug() << "Fail to report info size :" << reportInfo.size();
        return;
    }

    int testReportType = reportInfo.at(0).toInt();
    int dialogType = 0;
    QString title = QString();
    bool option1 = reportInfo.at(1).toBool();
    QStringList option2Str = QStringList();
    QVariantList option2Value = QVariantList({
        reportInfo.at(2).toBool(),
        reportInfo.at(3).toBool(),
        reportInfo.at(4).toBool(),
    });
    if (testReportType == ivis::common::TestReportTypeEnum::TestReportTypeTC) {
        dialogType = Dialog::DialogTypeTestReportTC;
        title = QString("TC Report");
        option2Str.append(QString("Split"));
        option2Str.append(QString("Config"));
        option2Str.append(QString("Excel"));
    } else {
        dialogType = Dialog::DialogTypeTestReportGCOV;
        title = QString("GCOV Report");
        option2Str.append(QString("Function"));
        option2Str.append(QString("Branch"));
        option2Str.append(QString("Line"));
    }

    QVariantList info = QVariantList({
        title,
        option1,
        option2Str,
        option2Value,
    });
    updateDrawDialog(dialogType, info);
#endif
}

void GuiMenu::updateDisplayViewLogFileList() {
#if defined(USE_DIALOG_OLD)
    if (mViewRunScript == nullptr) {
        QVariant fileList = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeViewLogFileList);
        mViewRunScript = new SelectModuleDialog(isHandler()->getScreen(), fileList.toStringList(), true);
        mViewRunScript->updateSelectListInfo(fileList.toStringList(), QList<QStringList>());
        mViewRunScript->updateSelectWidgetInfo(QString("Select Log File"), QStringList({"File List"}), QSize(500, 300));
        connect(mViewRunScript, &SelectModuleDialog::signalModuleSelected, [=](const QList<QPair<int, QString>>& selectModule) {
            if (selectModule.size() == 1) {
                createSignal(ivis::common::EventTypeEnum::EventTypeViewLogDisplay, QVariant(selectModule.at(0).first));
                mViewRunScript->finished(true);
            }
        });
        connect(mViewRunScript, &QDialog::finished, [=]() {
            disconnect(mViewRunScript);
            delete mViewRunScript;
            mViewRunScript = nullptr;
        });
    }
    mViewRunScript->show();
#else
    QVariantList info = QVariantList({
        QString("Select Log File"),
        QStringList({"Log File"}),
        isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeViewLogFileList).toStringList(),
        QStringList(),
        QVariantList(),
        0,
    });
    updateDrawDialog(Dialog::DialogTypeSelectLogFile, info);
#endif
}

void GuiMenu::updateDisplayViewLogInfo(const bool& show) {
    QVariant logInfo = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeViewLogInfo);

    qDebug() << "updateDisplayViewLogInfo :" << show << logInfo.toStringList().size();
#if defined(USE_DIALOG_OLD)
    if (mDetailLog == nullptr) {
        mDetailLog = new DetailLog(isHandler()->getScreen(), QString("Detail Log"));
        connect(mDetailLog, &DetailLog::signalCloseClicked, [=](const bool& clicked) { mDetailLog->hide(); });
        mDetailLog->hide();
    }

    if (logInfo.toStringList().size() == 0) {
        mDetailLog->contentClear();
    } else {
        mDetailLog->updateLogDisplay(logInfo.toStringList());
    }

    if (show) {
        mDetailLog->show();
    }
#else
    if (show) {
        QVariantList info = QVariantList({
            QString("View Log : Real Time"),
            logInfo.toStringList(),
        });
        updateDrawDialog(Dialog::DialogTypeViewLogInfo, info);
    }
#endif
}

void GuiMenu::updateDisplayViewLogFileInfo() {
    QVariantList logInfo = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeViewLogFileInfo).toList();
    if (logInfo.size() != 2) {
        qDebug() << "Fail to view log file info size :" << logInfo.size();
        return;
    }
    QString title = QString("View Log : %1").arg(logInfo.at(0).toString());
    QStringList detailLog = logInfo.at(1).toStringList();
#if defined(USE_DIALOG_OLD)
    if (mDetailLog == nullptr) {
        mDetailLog = new DetailLog(isHandler()->getScreen(), QString("Detail Log"), false);
        connect(mDetailLog, &DetailLog::signalCloseClicked, [=](const bool& clicked) { mDetailLog->hide(); });
    }
    mDetailLog->contentClear();
    mDetailLog->updateLogDisplay(detailLog);
    mDetailLog->show();
#else
    QVariantList info = QVariantList({
        title,
        detailLog,
    });
    updateDrawDialog(Dialog::DialogTypeViewLogFileInfo, info);
#endif
}

void GuiMenu::updateDisplayAppMode() {
    QVariantList info = QVariantList({
        QString("Select App Mode"),
        isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeAppMode).toInt(),
        isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeAppModeList).toStringList(),
    });
    updateDrawDialog(Dialog::DialogTypeAppModeRadio, info);
}

void GuiMenu::slotPropertyChanged(const int& type, const QVariant& value) {
    switch (type) {
        case ivis::common::PropertyTypeEnum::PropertyTypeDepth: {
            drawDisplay(value);
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize: {
            updateDisplaySize();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeVisible: {
            updateDisplayVisible();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeSfcModelPath: {
            updateDisplayPath();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeSelectModuleOfRun: {
            updateDisplaySelectModule();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeTestResultInfo: {
            updateDisplayTestResultInfo();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeEnterScriptText: {
            if (value.toBool()) {
                updateDisplayEnterScriptText();
            }
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeTestReport: {
            updateDisplayTestReport();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeViewLogFileList: {
            updateDisplayViewLogFileList();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeViewLogFileInfo: {
            updateDisplayViewLogFileInfo();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeViewLogInfo: {
            updateDisplayViewLogInfo(false);
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeAppMode: {
            updateDisplayAppMode();
            break;
        }
        default: {
            break;
        }
    }
}
