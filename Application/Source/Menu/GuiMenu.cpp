#include "GuiMenu.h"
#include "AbstractHandler.h"

#include "CommonResource.h"
#include "CommonUtil.h"

#include <QKeySequence>
#include <QLineEdit>
#include <QCompleter>

#include "GuiCenter.h"
#include "ConfigSetting.h"

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
    connect(mGui->actionViewScriptLog, &QAction::triggered, [=]() {
        createSignal(ivis::common::EventTypeEnum::EventTypeViewRunScript, ivis::common::RunTypeEnum::RunTypeViewRunScriptLog);
    });
    connect(mGui->actionGenSSFS, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeGenSSFS, QVariant()); });
    mGui->actionGenSSFS->setVisible(false);
}

void GuiMenu::drawMenuDocker() {
}

void GuiMenu::drawMenuHelp() {
    connect(mGui->actionAbout, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeHelpAbout, QVariant()); });
    connect(mGui->actionAboutQT, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeHelpAboutQt, QVariant()); });
}

void GuiMenu::drawMenuEtc(const bool& update) {
    QVariant path = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeSfcModelPath);
    mGui->actionPath->setText(QString("Path : %1").arg(path.toString()));
    mGui->actionPath->setIconText(QString("Path : %1").arg(path.toString()));

    if (update) {
        return;
    }
    connect(mGui->actionPath, &QAction::triggered,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeSettingSfcModelPath, QVariant()); });
}

void GuiMenu::updateDisplaySelectModule(const int& runType) {
    // qDebug() << "updateDisplaySelectModule :" << runType;
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
                updateDisplaySelectOption(runType, moduleSelect);
            }
        });
        connect(mSelectModule, &QDialog::finished, [=]() {
            disconnect(mSelectModule);
            delete mSelectModule;
            mSelectModule = nullptr;
        });
    }
    mSelectModule->show();
}

void GuiMenu::updateDisplaySelectOption(const int& runType, const QVariantList& moduleList) {
    if (mCheckBoxGroup == nullptr) {
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
                    if (mSelectModule) {
                        mSelectModule->finished(true);
                    }
                    mCheckBoxGroup->hide();
                    mCheckBoxGroup->finished(true);

                    createSignal(ivis::common::EventTypeEnum::EventTypeSelectModuleOfRun,
                                 QVariant({runType, option1, moduleList, checkList}));
                });
        connect(mCheckBoxGroup, &QDialog::finished, [=]() {
            disconnect(mCheckBoxGroup);
            delete mCheckBoxGroup;
            mCheckBoxGroup = nullptr;
        });
    }
    mCheckBoxGroup->show();
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
    static bool complete = false;
    int current = countInfo.at(0).toInt();
    int total = countInfo.at(1).toInt();
    complete = countInfo.at(2).toBool();

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

    // Test Result Info
    if (mLogDisplay == nullptr) {
        mLogDisplay = new LogDisplayDialog(isHandler()->getScreen(), QString("Test Result Info"));
        connect(mLogDisplay, &LogDisplayDialog::signalTestResultClick, [=](const bool& cancel) {
            if (cancel) {
                createSignal(ivis::common::EventTypeEnum::EventTypeGenRunTCCancel, complete);
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
        connect(mLogDisplay, &LogDisplayDialog::signalDetailClicked, [=](const bool& clicked) { updateDisplayDetailLog(true); });
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

    // Test Result Info : ProgressBar
    if (mProgressBar == nullptr) {
        mProgressBar = ivis::common::createWidget<QProgressBar>(isHandler()->getScreen(), true, QRect(1100, 25, 100, 30));
    }
    mProgressBar->setRange(0, total);
    mProgressBar->setValue(current);

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
}

void GuiMenu::updateDisplayDetailLog(const bool& visible) {
    if (mDetailLog == nullptr) {
        mDetailLog = new DetailLog(isHandler()->getScreen(), QString("Detail Log"));
        connect(mDetailLog, &DetailLog::signalCloseClicked, [=](const bool& clicked) { mDetailLog->hide(); });
        mDetailLog->hide();
    }

    QStringList detailLog =
        isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeRunScriptLogCurrent).toStringList();

    if (detailLog.size() == 0) {
        mDetailLog->contentClear();
    } else {
        mDetailLog->updateLogDisplay(detailLog);
    }

    if (visible) {
        mDetailLog->show();
    }
}

void GuiMenu::updateDisplayEnterScriptText() {
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
}

void GuiMenu::updateDisplayTestReport() {
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
        if (testReportType == static_cast<int>(ivis::common::TestReportTypeEnum::TestReportTypeResult)) {
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
}

void GuiMenu::updateDisplayViewRunScriptList() {
    if (mViewRunScript == nullptr) {
        QVariant fileList = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeViewRunScriptList);
        mViewRunScript = new SelectModuleDialog(isHandler()->getScreen(), fileList.toStringList(), true);
        mViewRunScript->updateSelectListInfo(fileList.toStringList(), QList<QStringList>());
        mViewRunScript->updateSelectWidgetInfo(QString("Select Log File"), QStringList({"File List"}), QSize(500, 300));
        connect(mViewRunScript, &SelectModuleDialog::signalModuleSelected, [=](const QList<QPair<int, QString>>& selectModule) {
            if (selectModule.size() == 1) {
                createSignal(ivis::common::EventTypeEnum::EventTypeViewRunScriptDetail, QVariant(selectModule.at(0).first));
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
}

void GuiMenu::updateDisplayViewRunScriptDetail() {
    QVariant detailInfo = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeViewRunScriptDetail);
    qDebug() << "detailInfo :" << detailInfo.toStringList().size();

    if (mDetailLog == nullptr) {
        mDetailLog = new DetailLog(isHandler()->getScreen(), QString("Detail Log"), false);
        connect(mDetailLog, &DetailLog::signalCloseClicked, [=](const bool& clicked) { mDetailLog->hide(); });
    }
    mDetailLog->show();

    QVariant detailLog = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeViewRunScriptDetail);
    mDetailLog->contentClear();
    mDetailLog->updateLogDisplay(detailLog.toStringList());
}

void GuiMenu::updateDisplayAppMode() {
    int appMode = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeAppMode).toInt();
    QStringList appModeList = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeAppModeList).toStringList();
    QRect screenInfo = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeScreenInfo).toRect();

    if (mDialog == nullptr) {
        mDialog = new Dialog(screenInfo, this);
        connect(mDialog, &Dialog::signalSelectAppMode,
                [=](const int& appMode) { createSignal(ivis::common::EventTypeEnum::EventTypeSelectAppMode, appMode); });
        connect(mDialog, &QDialog::finished, [=]() {
            disconnect(mDialog);
            delete mDialog;
            mDialog = nullptr;
        });
    }
    mDialog->updateAppModeRadio(appMode, appModeList);
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
#if defined(USE_DEFAULT_VSM_PATH)
        case ivis::common::PropertyTypeEnum::PropertyTypeDefaultPath: {
            drawMenuEtc(true);
            break;
        }
#else
        case ivis::common::PropertyTypeEnum::PropertyTypeSfcModelPath: {
            drawMenuEtc(true);
            break;
        }
#endif
        case ivis::common::PropertyTypeEnum::PropertyTypeSelectModuleOfRun: {
            int runType = value.toInt();
            if (runType != ivis::common::RunTypeEnum::RunTypeEnterScriptText) {
                updateDisplaySelectModule(runType);
            }
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
        case ivis::common::PropertyTypeEnum::PropertyTypeRunScriptLogCurrent: {
            updateDisplayDetailLog(false);
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeViewRunScriptList: {
            updateDisplayViewRunScriptList();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeViewRunScriptDetail: {
            updateDisplayViewRunScriptDetail();
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
