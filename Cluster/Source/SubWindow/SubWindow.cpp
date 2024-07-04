#include "SubWindow.h"
#include "ui_SubWindow.h"
#include "CommonEnum.h"
#include "CommonPopup.h"
#include "CommonResource.h"
#include "ConfigSetting.h"

#if defined(__MODULE_SERVICE__)
#include "Service.h"
#endif

#if defined(USE_FILE_WATCHER_QT)
#include "LogWatcher.h"
#endif

#include <QDebug>
#include <QMapIterator>
#include <QScrollBar>

const QString KEYWORD_DESCRIPTION = QString("[Description]");
const QString KEYWORD_POWERTRAIN = QString("[PowerTrain]");
const QString KEYWORD_PRECONDITION = QString("[Precondition]");
const QString KEYWORD_STEP = QString("[Step]");
const QString KEYWORD_EXPECTED_RESULT = QString("[Expected Result]");
const QString KEYWORD_GROUP = QString("    [Group]");
const QString KEYWORD_PERIOD = QString("    [Period]");
const QString KEYWORD_PERIOD_GROUP = QString("        [Group]");
const QString KEYWORD_LISTEN = QString("[Listen]");
const QString POWERTRAIN_LIST = QString("ICV, EV, FCEV, HEV, PHEV");
const QString LOG_FILE_KEYWORD_01 = QString("D/SimulatorSourcer(Dummy): ");
const QString LOG_FILE_KEYWORD_02 = QString("[After Signal Inject]");  // [After entering the step value, display information.]
const QString LOG_FILE_KEYWORD_03 = QString("Signal Received --------------- ");
const QString LOG_FILE_KEYWORD_04 = QString("  Received : ");

SubWindow::SubWindow(QWidget* parent) : QMainWindow(parent), mGui(new Ui::SubWindow), mTimerTouch(new QTimer) {
    mGui->setupUi(this);
    QRect rect = QRect();
#if defined(__SUB_WINDOW_ONLY__)
    rect = QRect(SCREEN_POSITION_X, SCREEN_POSITION_Y, SCREEN_SIZE_WIDTH, SCREEN_SIZE_HEIGHT);
#else
    int menubarHeight = 6;
    rect = QRect(SUB_SCREEN_POSITION_X, SUB_SCREEN_POSITION_Y + menubarHeight, SUB_SCREEN_SIZE_WIDTH, SUB_SCREEN_SIZE_HEIGHT);
    this->setWindowFlags(Qt::FramelessWindowHint);
#endif
    this->setFixedSize(SUB_SCREEN_SIZE_WIDTH, SUB_SCREEN_SIZE_HEIGHT);
    this->setGeometry(rect);
    this->show();

    init();
}

SubWindow::~SubWindow() {
    excuteScript(false, QString(), QStringList());
    qDebug() << "~SubWindow";
    delete mGui;
}

void SubWindow::init() {
    // excuteScript(false, QString(), QStringList());
#if defined(__MODULE_SERVICE__)
    Service::instance().data();
    Service::instance().data()->init();
#endif

    ConfigSetting::instance();
    setTavPath(ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeTavPath).toString());

    drawDisplay(DisplayTypeMain);
    controlConnect(true);
    updateFileList(ListTypeNormal, nullptr);
}

void SubWindow::controlConnect(const bool& state) {
    if (getInit()) {
        qDebug() << "ControlConnect : Completed";
        return;
    }
    setInit(true);

    connect(mGui->fileList, &QListWidget::itemPressed, [=](QListWidgetItem* item) {
        if (item) {
            if (getListType() == ListTypeNormal) {
                mTimerTouch->start(1000);
            }
        }
    });
    connect(mGui->fileList, &QListWidget::itemClicked, [=](QListWidgetItem* item) {
        if (item) {
            mTimerTouch->stop();
            if (getListType() != ListTypeNormal) {
                updateFileList(ListTypeUpdateCheck, (getListType() == ListTypeCheck) ? (nullptr) : (item));
            }
        }
    });
    // connect(mGui->fileList, &QListWidget::itemChanged, [=](QListWidgetItem* item) {
    //     if (item) {
    //         updateFileList(ListTypeUpdateCheck, nullptr);
    //     }
    // });
    connect(mTimerTouch, &QTimer::timeout, [=]() { updateFileList(ListTypeCheck, nullptr); });
    connect(mGui->checkCancel, &QPushButton::clicked, [=]() { updateFileList(ListTypeNormal, nullptr); });
    connect(mGui->checkDelete, &QPushButton::clicked, [=]() {
        if (deleteFile(DeleteTypeSelectTAV)) {
            updateFileList(ListTypeNormal, nullptr);
        }
    });
    connect(mGui->fileList, &QListWidget::itemDoubleClicked, [=](QListWidgetItem* item) {
        if (getListType() != ListTypeNormal) {
            return;
        }
        if (item) {
            controlScript(StartScriptTypeStop, item->text());
        }
    });
    connect(mGui->detailContent, &QPlainTextEdit::textChanged, [=]() {
        int scriptStart = getScriptStart();
        if ((scriptStart == StartScriptTypeStop) || (scriptStart == StartScriptTypeMenuStop)) {
            bool save = (getTavData() != mGui->detailContent->toPlainText());
            mGui->detailSave->setVisible(save);
            setTavSave(save);
        }
    });
    connect(mGui->detailSave, &QPushButton::clicked, [=]() {
        setTavSave(false);
        mGui->detailSave->setVisible(false);
        updateDetailDataInfo(getSelectFile());
    });
    connect(mGui->detailBack, &QPushButton::clicked, [=]() { controlScript(StartScriptTypeMenuStop, QString()); });
    connect(mGui->detailStart, &QPushButton::clicked, [=]() {
        QString selectFile = (mGui->fileList) ? (mGui->fileList->currentItem()->text()) : (QString());
        int scriptStart = getScriptStart();
        if (scriptStart == StartScriptTypeStop) {
            scriptStart = StartScriptTypeStart;
            mGui->logContent->clear();
            mGui->altonServiceContent->clear();
            mGui->hmiContent->clear();
        } else if (scriptStart == StartScriptTypeStart) {
            scriptStart = StartScriptTypeStop;
        } else if (scriptStart == StartScriptTypeMenuStop) {
            scriptStart = StartScriptTypeMenuStart;
            mGui->logContent->clear();
            mGui->altonServiceContent->clear();
            mGui->hmiContent->clear();
        } else if (scriptStart == StartScriptTypeMenuStart) {
            scriptStart = StartScriptTypeMenuStop;
        } else {
            return;
        }
        controlScript(scriptStart, selectFile);
    });
    connect(mGui->logBack, &QPushButton::clicked, [=]() {
        QString selectFile = getSelectFile();
        selectFile.replace(".tav", ".sh");
        drawDisplay(DisplayTypeViewScript, selectFile);
    });
    connect(mGui->detailViewLog, &QPushButton::clicked, [=]() {
        QString selectFile = getSelectFile();
        selectFile.replace(".tav", ".sh");
        drawDisplay(DisplayTypeViewScriptLog, selectFile);
    });

    connect(mGui->altonServiceClear, &QPushButton::clicked, [=]() { updateAltonClientLog(QString()); });
    connect(mGui->hmiClear, &QPushButton::clicked, [=]() { updateHmiLog(QString()); });

    connect(mGui->actionOpen, &QAction::triggered, [=]() {
        int displayType = getDisplayType();
        if ((displayType == DisplayTypeViewTav) || (displayType == DisplayTypeViewScript)) {
            QVariant popupData = QVariant();
            ivis::common::Popup::drawPopup(ivis::common::PopupType::OpenError, this, popupData,
                                           QVariant(QVariantList({STRING_POPUP_OPEN_ERROR, STRING_POPUP_OPEN_ERROR_TIP})));
            qDebug() << "Can't open file on current screen :" << getDisplayType();
            return;
        }
        QString openPath = openTavFile();
        if (openPath.size() > 0) {
            setTavPath(openPath);
            updateFileList(ListTypeNormal, nullptr);
        }
    });
    connect(mGui->actionTAV_Delete, &QAction::triggered, [=]() { deleteFile(DeleteTypeTAV); });
    connect(mGui->actionScript_Delete, &QAction::triggered, [=]() { deleteFile(DeleteTypeScript); });
    connect(mGui->actionInfo_Delete, &QAction::triggered, [=]() { deleteFile(DeleteTypeInfo); });

    connect(mGui->actionStart, &QAction::triggered, [=]() {
        if (mGui->fileList) {
            controlScript(StartScriptTypeMenuStart, mGui->fileList->currentItem()->text());
        }
    });
    connect(mGui->actionStop, &QAction::triggered, [=]() { controlScript(StartScriptTypeMenuStop, QString()); });

    connect(mGui->actionAbout, &QAction::triggered, [=]() {
        QVariant popupData = QVariant();
        ivis::common::Popup::drawPopup(ivis::common::PopupType::About, this, popupData,
                                       QVariant(QVariantList({STRING_POPUP_ABOUT, STRING_POPUP_ABOUT_TIP})));
    });
    connect(mGui->actionAbout_QT, &QAction::triggered, [=]() {
        QVariant popupData = QVariant();
        ivis::common::Popup::drawPopup(ivis::common::PopupType::AboutQt, this, popupData);
    });
    connect(mGui->actionVSM_Path, &QAction::triggered, [=]() { settingPath(PathTypeVSM); });
    connect(mGui->actionTAV_Path, &QAction::triggered, [=]() { settingPath(PathTypeTAV); });
    connect(mGui->actionAltonClient_Path, &QAction::triggered, [=]() { settingPath(PathTypeAltonClient); });

    // Service
#if defined(__MODULE_SERVICE__)
    connect(Service::instance().data(), &Service::signalServiceDataChanged,
            [=](const int& dataType, const int& signalType, const QVariant& signalValue) {
                // if (getCheckSfcSignal().contains(signalValue.toString())) {
                //     QString signalName = getCheckSfcSignal().split(" ").at(0);
                //     QString singleValueInfo("Singal Received - Single\n");
                //     singleValueInfo.append(QString("  Received : %1 = %2\n").arg(signalName).arg(sfcValue.toString()));
                //     updateHmiLog(singleValueInfo);
                // }
            });
    connect(
        Service::instance().data(), &Service::signalServiceDatasChanged,
        [=](const int& dataType, const int& signalType, const QHash<QString, QVariant>& signalValues) {
            QString multiValueInfo = QString();
            for (auto iter = signalValues.cbegin(); iter != signalValues.cend(); ++iter) {
                QString sfcName = iter.key();
                QVariant sfcValue = iter.value();
                if (getCheckSfcSignal().contains(sfcName)) {
                    multiValueInfo.append(QString("  Received : %1 = %2\n").arg(sfcName).arg(sfcValue.toString()));
                }
            }
            if (multiValueInfo.size() > 0) {
                multiValueInfo.prepend(QString("Singal Received - %1\n").arg((signalValues.size() > 1) ? ("Group") : ("Single")));
                multiValueInfo.append("\n");
                updateHmiLog(multiValueInfo);
            }
        });
#endif
}

void SubWindow::drawDisplay(const int& type, const QString& text) {
    switch (type) {
        case DisplayTypeMain: {
            mGui->tav->setCurrentIndex(TavDisplayTypeList);
            mGui->menubar->setVisible(true);
            mGui->checkCancel->setVisible(false);
            mGui->checkDelete->setVisible(false);
            mGui->detailSave->setVisible(false);
            break;
        }
        case DisplayTypeViewTav: {
            mGui->tav->setCurrentIndex(TavDisplayTypeDetail);
            mGui->detailBack->setVisible(true);
            mGui->detailViewLog->setVisible(false);
            mGui->detailTitle->setText(text);
            mGui->detailTitle->setAlignment(Qt::AlignCenter);
            mGui->detailStart->setText("Start");
            mGui->detailContent->setReadOnly(false);
            break;
        }
        case DisplayTypeViewScript: {
            mGui->tav->setCurrentIndex(TavDisplayTypeDetail);
            mGui->detailBack->setVisible(false);
            mGui->detailViewLog->setVisible(true);
            mGui->detailTitle->setText(text);
            mGui->detailTitle->setAlignment(Qt::AlignCenter);
            mGui->detailStart->setText("Stop");
            mGui->detailContent->setReadOnly(true);
            break;
        }
        case DisplayTypeViewScriptLog: {
            mGui->tav->setCurrentIndex(TavDisplayTypeLog);
            mGui->logBack->setVisible(true);
            mGui->logTitle->setText(mGui->detailTitle->text());
            mGui->logTitle->setAlignment(Qt::AlignCenter);
            mGui->logContent->setReadOnly(true);
            break;
        }
        default: {
            break;
        }
    }

#if defined(USE_IVIS_LOG)
    QString compalnyText("<a href='https://ivis.ai'>(주)아이비스</a>");
    // QString compalnyText("asreawetgsadf\nURL : <a href='https://jira.ccos.dev/browse/EXNCP-40685'>TICKET NO</a>");
    mGui->companyInfo->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mGui->companyInfo->setOpenExternalLinks(true);
    mGui->companyInfo->setText(compalnyText);
    mGui->companyInfo->setVisible(true);
#else
    mGui->companyInfo->setVisible(false);
#endif
    setDisplayType(type);
}

void SubWindow::updateFileList(const int& type, QListWidgetItem* updateItem) {
    if (mGui->fileList == nullptr) {
        return;
    }

    QStringList deleteFileList;
    setListType(type);
    switch (type) {
        case ListTypeNormal: {
            QFileInfoList fileList = QFileInfoList();
            QString path = getTavPath();
            QStringList fileInfo = ivis::common::FileInfo::isFileListInfo(path, QString(".tav"), fileList);
            mGui->fileList->clear();
            for (const auto& file : fileInfo) {
                QListWidgetItem* item = new QListWidgetItem(file);
                QFont font;
                font.setPointSize(20);
                item->setFont(font);
                item->setSizeHint(QSize(638, 50));
                mGui->fileList->addItem(item);
            }
            mGui->fileList->setCurrentRow(0);
            mGui->fileList->show();
            break;
        }
        case ListTypeCheck: {
            for (int rowIndex = 0; rowIndex < mGui->fileList->count(); rowIndex++) {
                QListWidgetItem* item = mGui->fileList->item(rowIndex);
                if (item) {
                    item->setCheckState(Qt::Unchecked);
                }
            }
            break;
        }
        case ListTypeUpdateCheck: {
            if (updateItem) {
                updateItem->setCheckState((updateItem->checkState() == Qt::Checked) ? (Qt::Unchecked) : (Qt::Checked));
            }
            break;
        }
        default: {
            break;
        }
    }

    for (int rowIndex = 0; rowIndex < mGui->fileList->count(); rowIndex++) {
        QListWidgetItem* item = mGui->fileList->item(rowIndex);
        if ((item) && (item->checkState() == Qt::Checked)) {
            deleteFileList.append(item->text());
        }
    }

    if (mGui->checkCancel) {
        mGui->checkCancel->setVisible(type != ListTypeNormal);
    }
    if (mGui->checkDelete) {
        mGui->checkDelete->setVisible(deleteFileList.size() > 0);
    }
    setDeleteFileList(deleteFileList);
}

void SubWindow::updateDetailFileInfo(const int& viewType, const QString& info) {
    QString content = QString();

    if (viewType == ViewTypeTAV) {
        int foundType = DetailInfoInvalid;
        QStringList tavData = getOriginalData();
        if (info.size() > 0) {
            QString file = info;
            QString path = QString("%1/%2").arg(getTavPath()).arg(file);
            tavData = ivis::common::FileInfo::readFile(path);
        }

        QMap<int, QString> detailInfo = QMap<int, QString>();
        for (const auto& data : tavData) {
            if (data.contains(KEYWORD_DESCRIPTION)) {
                foundType = DetailInfoDescription;
            } else if (data.contains(KEYWORD_POWERTRAIN)) {
                foundType = DetailInfoPowerTrain;
            } else if (data.contains(KEYWORD_PRECONDITION)) {
                foundType = DetailInfoPrecondition;
            } else if (data.contains(KEYWORD_STEP)) {
                foundType = DetailInfoStep;
            } else if (data.contains(KEYWORD_EXPECTED_RESULT)) {
                foundType = DetailInfoExpectedResult;
            } else {
            }
            QString lineStr = QString("%1\n").arg(data);
            content.append(lineStr);
            detailInfo[foundType].append(lineStr);
        }
        detailInfo[DetailInfoListen] = detailInfo[DetailInfoExpectedResult];
        // for (auto iter = detailInfo.begin(); iter != detailInfo.end(); ++iter) {
        //     int index = 0;
        //     for (const auto& data : iter.value().split("\n")) {
        //         qDebug() << iter.key() << ". Data[" << index++ << "] :" << data;
        //     }
        // }

        setAllDetailInfo(detailInfo);
        setTavData(content);
    } else if ((viewType == ViewTypeScript) || (viewType == ViewTypeRedrawTAV)) {
        content = info;
    } else {
        return;
    }

    updateDetailContent(content);
}

void SubWindow::updateDetailContent(const QString& content) {
    if (mGui->detailContent) {
        mGui->detailContent->show();
        mGui->detailContent->setPlainText(content);
    }
}

void SubWindow::updateDetailDataInfo(const QString& filePath) {
    QStringList newDetailContent = (mGui->detailContent) ? (mGui->detailContent->toPlainText().split("\n")) : (QStringList());

    if (getOriginalData() != newDetailContent) {
        setOriginalData(newDetailContent);
        updateDetailFileInfo(ViewTypeTAV, QString());
        writeOriginalData(filePath, newDetailContent);
    }
}

void SubWindow::updateAltonClientLog(const QString& log) {
    if (mGui->logContent) {
        mGui->logContent->insertPlainText(log);
        mGui->logContent->verticalScrollBar()->setValue(mGui->logContent->verticalScrollBar()->maximum());
    }
}

void SubWindow::updateAltonServiceLog(const QString& log) {
    if (mGui->altonServiceContent) {
        mGui->altonServiceContent->insertPlainText(log);
        mGui->altonServiceContent->verticalScrollBar()->setValue(mGui->altonServiceContent->verticalScrollBar()->maximum());
    }
}

void SubWindow::updateHmiLog(const QString& log) {
    if (mGui->hmiContent) {
        if (log.size() == 0) {
            mGui->hmiContent->clear();
        } else {
            mGui->hmiContent->insertPlainText(log);
            mGui->hmiContent->verticalScrollBar()->setValue(mGui->hmiContent->verticalScrollBar()->maximum());
        }
    }
}

void SubWindow::writeOriginalData(const QString& filePath, const QStringList& saveData) {
    if ((filePath.size() == 0) || (saveData.size() == 0)) {
        return;
    }

    QString writeContent = QString();
    QString path = QString("%1/%2").arg(getTavPath()).arg(filePath);
    for (const auto& data : saveData) {
        writeContent.append(data);
        writeContent.append("\n");
    }
    qDebug() << "writeOriginalData :" << saveData.size() << path;
    ivis::common::FileInfo::writeFile(path, writeContent, false);
}

bool SubWindow::isDetailInfo(const int& type, QPair<QString, QStringList>& detailInfo) {
    QMap<int, QString> allDetailInfo = getAllDetailInfo();

    if (allDetailInfo[type].size() == 0) {
        qDebug() << "Fail to detail info size : 0";
        return false;
    }

    detailInfo = QPair<QString, QStringList>();
    for (auto& lineStr : allDetailInfo[type].split("\n")) {
        if ((lineStr.size() == 0) || (lineStr.trimmed().startsWith("#"))) {
            // 공백, 주석 제거
            continue;
        }
        lineStr.replace("\t", "    ");  // Replace : Tab -> Space 4

        if (lineStr.contains("#")) {
            lineStr.remove(lineStr.indexOf("#"), lineStr.size());
        }

        if (detailInfo.first.size() == 0) {
            detailInfo.first = lineStr;
        } else {
            detailInfo.second.append(lineStr);
        }
    }

    if (detailInfo.first.size() == 0) {
        if (type == DetailInfoDescription) {
            detailInfo.first = KEYWORD_DESCRIPTION;
        } else if (type == DetailInfoPowerTrain) {
            detailInfo.first = KEYWORD_POWERTRAIN;
        } else if (type == DetailInfoPrecondition) {
            detailInfo.first = KEYWORD_PRECONDITION;
        } else if (type == DetailInfoStep) {
            detailInfo.first = KEYWORD_STEP;
        } else if (type == DetailInfoExpectedResult) {
            detailInfo.first = KEYWORD_EXPECTED_RESULT;
        } else {
        }
    }

    // for (const auto& detail : detailInfo.second) {
    //     qDebug() << detailInfo.first << ":" << detail;
    // }
    return true;
}

void SubWindow::isSubDetailInfo(const QStringList& inputStr, QMap<int, QStringList>& subDetialInfo) {
    int subDetailType = DetailSubInfoNormal;
    QString normalSpace = QString("    ");               // Space:4(Description, PowerTrain Precondition, Step, Expected Result)
    QString groupSpace = QString("        ");            // Space:8(Group)
    QString periodSpace = groupSpace;                    // Space:8(Period)
    QString periodGroupSpace = QString("            ");  // Space:12(Period -> Group)

    subDetialInfo.clear();
    for (const auto lineStr : inputStr) {
        bool appendState = false;
        if (lineStr.contains(KEYWORD_PERIOD_GROUP)) {
            subDetailType = DetailSubInfoPeriodGroup;
        } else if (lineStr.contains(KEYWORD_PERIOD)) {
            subDetailType = DetailSubInfoPeriod;
        } else if (lineStr.contains(KEYWORD_GROUP)) {
            subDetailType = DetailSubInfoGroup;
        } else {
            appendState = true;
        }

        if (appendState == false) {
            continue;
        }

        if (subDetailType == DetailSubInfoNormal) {
            appendState = (lineStr.contains(normalSpace));
        } else if (subDetailType == DetailSubInfoGroup) {
            appendState = (lineStr.contains(groupSpace));
            // Group 이후에 Precondition 값이 나오는 경우 예외 처리
            if (appendState == false) {
                appendState = (lineStr.contains(normalSpace));
                subDetailType = (appendState) ? (DetailSubInfoNormal) : (DetailSubInfoGroup);
            }
        } else if (subDetailType == DetailSubInfoPeriod) {
            appendState = (lineStr.contains(periodSpace));
        } else if (subDetailType == DetailSubInfoPeriodGroup) {
            appendState = (lineStr.contains(periodGroupSpace));
            // PeriodGroup 이후에 Period 값이 나오는 경우 예외 처리
            if (appendState == false) {
                appendState = (lineStr.contains(periodSpace));
                subDetailType = (appendState) ? (DetailSubInfoPeriod) : (DetailSubInfoPeriodGroup);
            }
        } else {
            appendState = false;
        }

        if (appendState) {
            subDetialInfo[subDetailType].append(lineStr);
        }
    }

#if 0
    qDebug() << "=====================================================================================";
    for (const auto& normal : subDetialInfo[DetailSubInfoNormal]) {
        qDebug() << "\t Normal :" << normal;
    }
    qDebug() << "=====================================================================================";
    for (const auto& group : subDetialInfo[DetailSubInfoGroup]) {
        qDebug() << "\t Group :" << group;
    }
    qDebug() << "=====================================================================================";
    for (const auto& period : subDetialInfo[DetailSubInfoPeriod]) {
        qDebug() << "\t Period :" << period;
    }
    qDebug() << "=====================================================================================";
    for (const auto& periodGroup : subDetialInfo[DetailSubInfoPeriodGroup]) {
        qDebug() << "\t PeriodGroup :" << periodGroup;
    }
    qDebug() << "=====================================================================================";
#endif
}

QString SubWindow::isDetailSignalInfo(const int& type, const QString& inputStr, QString& convertStr) {
    QStringList inputList = inputStr.split(" ");
    QString abstractionSignal = QString();
    QString signal = QString();
    QString value = QString();

    convertStr.clear();
    if (inputList.size() >= 2) {
        for (int index = 0; index < inputList.size(); index++) {
            if (inputList[index].size() == 0) {
                continue;
            }
            if (signal.size() == 0) {
                signal = inputList[index];
                continue;
            }
            value = inputList[index];
        }
    }

    if (value.size() == 0) {
        convertStr = QString("#### Check Input : %1").arg(inputStr);
        qDebug() << "Fail to check input string (value is invalid) :" << inputStr;
    } else {
        if (type == DetailSubInfoNormal) {
            if (inputStr.contains("SFC.")) {
                convertStr = QString("$ALTON_CLIENT set %1 %2 %3").arg(signal).arg(isDataType(value)).arg(value);
            } else if (inputStr.contains("delay")) {
                convertStr = QString("sleep %1").arg(value.toDouble() * (double)0.001);
            } else {
                if (inputStr.contains("Vehicle.")) {
                    abstractionSignal = inputStr;
                }
                convertStr = QString("$ALTON_CLIENT inject %1").arg(inputStr);
            }
        } else {
            if (inputStr.contains("Vehicle.")) {
                abstractionSignal = inputStr;
            }
            convertStr = QString("%1").arg(inputStr);
        }
    }
    convertStr.remove("    ");
    abstractionSignal.remove("    ");
    // qDebug() << "Signal :" << inputStr << "->" << convertStr << ", Abstraction :" << abstractionSignal;
    return abstractionSignal;
}

QString SubWindow::isToScriptInfo(const int& type, QStringList& infoList, const QString& file) {
    QPair<QString, QStringList> detailInfo = QPair<QString, QStringList>();
    if (isDetailInfo(type, detailInfo) == false) {
        return QString();
    }

    QString scriptInfo = QString();

    infoList.clear();
    if ((type == DetailInfoDescription) || (type == DetailInfoExpectedResult)) {
        QString sfcSignal = QString();
        scriptInfo.append(QString("#%1\n").arg(detailInfo.first));
        for (const auto& detail : detailInfo.second) {
            scriptInfo.append(QString("#%1\n").arg(detail));
            sfcSignal.append(detail + "\n");
        }
        if (type == DetailInfoExpectedResult) {
            setCheckSfcSignal(sfcSignal);
        }
    } else if (type == DetailInfoPowerTrain) {
        QString currPowerTrain = QString("ALL");
        if (detailInfo.second.size() > 0) {
            currPowerTrain = detailInfo.second.at(0);
        }
        if (currPowerTrain.toUpper().contains("ALL")) {
            currPowerTrain = POWERTRAIN_LIST;
        }
        currPowerTrain.remove("    ");
        infoList = currPowerTrain.split(", ");
        scriptInfo.append(QString("#%1\n").arg(detailInfo.first));
        scriptInfo.append(QString("#    %1\n").arg(currPowerTrain));
    } else if ((type == DetailInfoPrecondition) || (type == DetailInfoStep)) {
        QString signalInfo = QString();
        QMap<int, QStringList> subDetialInfo = QMap<int, QStringList>();
        isSubDetailInfo(detailInfo.second, subDetialInfo);

        // Noraml
        scriptInfo.append(QString("#%1\n").arg(detailInfo.first));
        for (const auto& normal : subDetialInfo[DetailSubInfoNormal]) {
            QString abstractionSignal = isDetailSignalInfo(DetailSubInfoNormal, normal, signalInfo);
            if (abstractionSignal.size() > 0) {
                infoList.append(abstractionSignal);
            }
            scriptInfo.append(QString("%1\n").arg(signalInfo));
        }

        // Group
        QString groupSignal = QString();
        for (const auto& group : subDetialInfo[DetailSubInfoGroup]) {
            QString abstractionSignal = isDetailSignalInfo(DetailSubInfoGroup, group, signalInfo);
            if (abstractionSignal.size() > 0) {
                infoList.append(abstractionSignal);
            }
            groupSignal.append(QString(" %1").arg(signalInfo));
        }
        if (groupSignal.size() > 0) {
            scriptInfo.append(QString("$ALTON_CLIENT inject%1\n").arg(groupSignal));
        }

        // Period
        int periodCycle = 0;
        int periodDuration = 0;
        QString periodSignal = QString();
        for (const auto& period : subDetialInfo[DetailSubInfoPeriod]) {
            QString abstractionSignal = isDetailSignalInfo(DetailSubInfoPeriod, period, signalInfo);
            // qDebug() << "Period  :"<< signalInfo;
            if (abstractionSignal.size() > 0) {
                infoList.append(abstractionSignal);
            }

            if (signalInfo.contains("Info :")) {
                QStringList temp = signalInfo.remove("Info :").remove(" ").split("/");
                if (temp.size() == 2) {
                    periodCycle = temp.at(0).toInt();
                    periodDuration = temp.at(1).toInt();
                } else {
                    qDebug() << "Fail to period info parsing :" << temp;
                }
            } else {
                periodSignal.append(QString("    $ALTON_CLIENT inject %1\n").arg(signalInfo));
            }
        }

        if (periodSignal.size() > 0) {
            QString replacePeriodGroup = QString("#%1\n").arg(KEYWORD_PERIOD_GROUP);
            scriptInfo.append("\n");
            scriptInfo.append(QString("for i in `seq 1 %1`\n").arg(periodCycle));
            scriptInfo.append(QString("do\n"));
            scriptInfo.append(QString("%1").arg(periodSignal));
            scriptInfo.append(replacePeriodGroup);
            scriptInfo.append(QString("    sleep %1\n").arg(periodDuration * (double)0.001));
            scriptInfo.append(QString("done\n"));

            // PeriodGroup
            QString periodGroupSignal = QString();
            for (const auto& periodGroup : subDetialInfo[DetailSubInfoPeriodGroup]) {
                QString abstractionSignal = isDetailSignalInfo(DetailSubInfoPeriodGroup, periodGroup, signalInfo);
                // qDebug() << "PeriodGroup  :"<< signalInfo;
                if (abstractionSignal.size() > 0) {
                    infoList.append(abstractionSignal);
                }
                periodGroupSignal.append(QString(" %1").arg(signalInfo));
            }
            if (periodGroupSignal.size() == 0) {
                scriptInfo.replace(replacePeriodGroup, QString());
            } else {
                scriptInfo.replace(replacePeriodGroup, QString("    $ALTON_CLIENT inject%1\n").arg(periodGroupSignal));
            }
        }
    } else if (type == DetailInfoListen) {
        QString listenSignal = QString();
        for (auto& detail : detailInfo.second) {
            detail.remove("    ");
            QStringList temp = detail.split(" ");
            if (temp.size() == 2) {
                listenSignal.append(QString(" %1").arg(temp.at(0)));
            }
        }
        scriptInfo.append(QString("#%1\n").arg(KEYWORD_LISTEN));

        QString tavPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeTavPath).toString();
        QString fileName = QString("%1/%2.AltonListen.info").arg(tavPath).arg(file);
        scriptInfo.append(QString("TAV_FILE=%1\n").arg(fileName));
        scriptInfo.append(QString("rm -f $TAV_FILE\n"));
        // scriptInfo.append(QString("touch $TAV_FILE\n"));
        // scriptInfo.append(QString("chmod -R 777 $TAV_FILE\n"));
        scriptInfo.append(QString("$ALTON_CLIENT listen%1 >> $TAV_FILE &\n").arg(listenSignal));
    } else {
    }

    return scriptInfo;
}

QString SubWindow::createToScript(const QString& file, QStringList& scriptFileList) {
    QStringList powerTrain = QStringList();
    QStringList abstractionSignalList = QStringList();
    QStringList signalList = QStringList();
    QString altonClient = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAltonClient).toString();
    QString altonPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAltonPath).toString();
    QString tavPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeTavPath).toString();
    QString fileName = file;
    fileName.remove(".tav");
    fileName.remove(".sh");

    QString scriptInfo("#!/bin/bash");

    // Description
    scriptInfo.append("\n\n");
    scriptInfo.append(isToScriptInfo(DetailInfoDescription, signalList));

    // PowerTrain
    scriptInfo.append("\n\n");
    scriptInfo.append(isToScriptInfo(DetailInfoPowerTrain, powerTrain));

    // AltonClient Path
    scriptInfo.append("\n\n");
    scriptInfo.append("#[AltonClient]\n");
#if 1
    scriptInfo.append("ALTON_CLIENT=\"$1\"\n");
    scriptInfo.append(QString("if [ -z \"$1\" ]; then\n    ALTON_CLIENT=%1/%2\nfi\n").arg(altonPath).arg(altonClient));
#else
    scriptInfo.append(QString("ALTON_CLIENT=\"%1\"\n").arg(altonClient));
    scriptInfo.append(QString("if [ -n \"$1\" ]; then\n    ALTON_CLIENT=\"$1\"\nfi\n"));
#endif
    scriptInfo.append(QString("echo \"ALTON_CLIENT=$ALTON_CLIENT\"\n"));
    scriptInfo.append("sleep 0.3\n");

    // Precondition
    scriptInfo.append("\n\n");
    scriptInfo.append(isToScriptInfo(DetailInfoPrecondition, signalList));
    abstractionSignalList.append(signalList);

    // Listen
    scriptInfo.append("\n\n");
    scriptInfo.append(isToScriptInfo(DetailInfoListen, signalList, fileName));

    // DateTime
    scriptInfo.append("\n\n");
    scriptInfo.append("#[DateTime]\n");
    scriptInfo.append(QString("DATE_TIME_FILE=%1/%2%3\n").arg(tavPath).arg(fileName).arg(".DateTime.info"));
    scriptInfo.append(QString("rm -f $DATE_TIME_FILE\n"));
    scriptInfo.append(QString("CURRENT_DATE_TIME=$(date +\"%Y-%m-%d %H:%M:%S.%3N\")\n"));
    scriptInfo.append(QString("echo \"CURRENT_DATE_TIME=$CURRENT_DATE_TIME\"\n"));
    scriptInfo.append(QString("echo -e \"$CURRENT_DATE_TIME\" >> $DATE_TIME_FILE\n"));

    // Step
    scriptInfo.append("\n\n");
    scriptInfo.append(isToScriptInfo(DetailInfoStep, signalList));
    abstractionSignalList.append(signalList);

    // ExpectedResult
    scriptInfo.append("\n\n");
    scriptInfo.append(isToScriptInfo(DetailInfoExpectedResult, signalList));

    // Save : power train type
    QStringList pasingFileList = isVsmFileInfo(powerTrain, abstractionSignalList);
    QString repacePowerTrain = QString();
    for (const auto& pt : powerTrain) {
        if (repacePowerTrain.size() == 0) {
            repacePowerTrain.append(QString("%1").arg(pt));
        } else {
            repacePowerTrain.append(QString(", %1").arg(pt));
        }
    }
    for (auto& pt : powerTrain) {
        QString ptValid = QString();
        for (const auto& ptInfo : POWERTRAIN_LIST.split(", ")) {
            if (pt == ptInfo) {
                ptValid = pt;
                break;
            }
        }
        if (ptValid.size() == 0) {
            qDebug() << "Fail to validate power train :" << repacePowerTrain << "->" << pt;
            continue;
        }
        QString writeContent = scriptInfo;
        writeContent.replace(repacePowerTrain, pt);

        for (const auto& replaceInfo : isReplaceSignal(abstractionSignalList, pasingFileList)) {
            writeContent.replace(replaceInfo.first, replaceInfo.second);
            scriptInfo.replace(replaceInfo.first, replaceInfo.second);
        }
        // for (const auto& detailInfo : writeContent.split("\n")) {
        //     qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
        //     int count = 0;
        //     for (QString info : detailInfo.split("\n")) {
        //         qDebug() << count++ << ":" << info;
        //     }
        //     qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
        // }

        QString filePath = QString("%1/%2.%3.sh").arg(getTavPath()).arg(fileName).arg(pt);
        ivis::common::FileInfo::writeFile(filePath, writeContent, false);
        qDebug() << "\t WriteFile :" << filePath;
        scriptFileList.append(filePath);
    }
    return scriptInfo;
}

void SubWindow::excuteScript(const bool& start, const QString& file, const QStringList& scriptFileList) {
    qDebug() << "-------------------------------------------------------------------------------------------------------";
    qDebug() << "excuteScript :" << ((start) ? ("[Start]") : ("[Stop]")) << ", ScriptFile :" << file << scriptFileList.size();

    QString altonService = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAltonService).toString();
    bool result = false;
    QStringList log;
    ivis::common::ExcuteProgram process(false);

    stopWatcherFile(0);
    stopProcess();

    if (start) {
        // 0. create log file name
        QString tavPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeTavPath).toString();
        QString filePath = QString("%1/%2").arg(tavPath).arg(file);
        filePath.replace(".sh", ".info");
        filePath.replace(".tav", ".info");

        // 1. rm -f ../TAV/[FILE].*.info
        QString delFile = QString("rm -f %1").arg(filePath);
        delFile.replace(".info", ".*.info");
        result = process.start(delFile, log);
        qDebug() << "\t\t Delete :" << ((result) ? ("[Success]") : ("[Fail]  ")) << delFile;

        // 2-2. Watcher Log file : ../TAV/[FILE].AltonListen.info
        QString altonLogFile = filePath;
        altonLogFile.replace(".info", ".AltonListen.info");
        startWatcherFile(FileWatcherTypeAltonListen, altonLogFile);

        // 2-1. Watcher Log file : ../TAV/[FILE].AltonService.info
        altonLogFile.replace(".AltonListen.info", ".AltonService.info");
        startWatcherFile(FileWatcherTypeAltonService, altonLogFile);

        // 3. altonservice >> ../TAV/[FILE].AltonService.info
        QString altonPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAltonPath).toString();
        QString altonServiceCmd = QString("%1/%2 > %3 &").arg(altonPath).arg(altonService).arg(altonLogFile);
        result = process.start(altonServiceCmd, log);
        qDebug() << "\t Start - AltonService :" << ((result) ? ("[Success]") : ("[Fail]  ")) << altonServiceCmd;

        // 4. chmod -R 777 ../TAV/[FILE].sh
        QString command = QString();
        for (const auto& script : scriptFileList) {
            if (command.size() == 0) {
                command = script;
            }
            QString permissionCmd = QString("chmod -R 777 %1").arg(script);
            result = process.start(permissionCmd, log);
            qDebug() << "\t\t Permisstion :" << ((result) ? ("[Success]") : ("[Fail]  ")) << permissionCmd;
        }

        // 5. ../TAV/[FILE].sh >> ../TAV/[FILE].Script.info
        QString scriptLogFile = QString(" > %1").arg(filePath);
        // scriptLogFile.clear();
        scriptLogFile.replace(".info", ".Script.info");
        startProcess(command, scriptLogFile);
    } else {
        QString altonClient = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAltonClient).toString();
        result = process.start(QString("killall %1").arg(altonClient), log);
        qDebug() << "\t Kill  - AltonClient  :" << ((result) ? ("[Success]") : ("[Fail]  ")) << altonClient;

        result = process.start(QString("killall %1").arg(altonService), log);
        qDebug() << "\t Kill  - AltonService :" << ((result) ? ("[Success]") : ("[Fail]  ")) << altonService;

        setAltonServiceData(QStringList());
    }
}

void SubWindow::startProcess(const QString& command, const QString& log) {
    stopProcess();

    if (command.size() == 0) {
        qDebug() << "\t Fail to command : nullptr";
    }

    if (mProcess == nullptr) {
        mProcess = QSharedPointer<ivis::common::ExcuteProgramThread>(new ivis::common::ExcuteProgramThread(false),
                                                                     &QObject::deleteLater);
    }
    mProcess.data()->setCommandInfo(command, log);
    mProcess.data()->start();
    connect(
        mProcess.data(), &ivis::common::ExcuteProgramThread::signalExcuteProgramInfo, [=](const bool& start, const bool& result) {
            if (start == false) {
                qDebug() << "*************************************************************************************************";
                qDebug() << "Commnad :" << command;
                qDebug() << "Result  :" << ((result) ? ("Success") : ("fail"));
                qDebug() << "*************************************************************************************************\n";
            }
        });
}

void SubWindow::stopProcess() {
    if (mProcess.isNull() == false) {
        disconnect(mProcess.data());
        mProcess.reset();
    }
}

void SubWindow::startWatcherFile(const int& type, const QString& watcherFile) {
    if (watcherFile.size() == 0) {
        qDebug() << "\t Fail to watcher file : nullptr";
        return;
    }
#if 1
    if (mLogFileWatcher[type] == nullptr) {
        mLogFileWatcher[type] =
            QSharedPointer<ivis::common::FileSystemWatcherThread>(new ivis::common::FileSystemWatcherThread(watcherFile, 30));
    }
    mLogFileWatcher[type].data()->start();
    connect(mLogFileWatcher[type].data(), &ivis::common::FileSystemWatcherThread::signalWatcherFileDataChanged,
            [=](const bool& init, const QStringList& data) {
                // qDebug() << "\t\t File Changed :" << type << data.size();
                if (type == FileWatcherTypeAltonListen) {
                    QString updateData = QString();
                    bool valid = false;
                    for (const auto& d : data) {
                        if (d.contains(LOG_FILE_KEYWORD_03)) {
                            QStringList temp = d.split(LOG_FILE_KEYWORD_03);
                            if (temp.size() == 2) {
                                QString fileName = watcherFile;
                                fileName.replace(".AltonListen.info", ".DateTime.info");
                                QStringList readData = ivis::common::FileInfo::readFile(fileName);
                                QString baseDateTime = (readData.size() > 0) ? (readData.at(0)) : ("");
                                QString inputDateTime = temp.at(1);
                                inputDateTime.remove("(");
                                inputDateTime.remove(")");
                                valid = isDateTimeValid(baseDateTime, inputDateTime);
                            }
                        }
                        if (valid == false) {
                            continue;
                        }
                        if ((d.contains(LOG_FILE_KEYWORD_03)) || (d.contains(LOG_FILE_KEYWORD_04))) {
                            updateData.append(QString("%1\n").arg(d));
                        }
                    }
                    // updateAltonClientLog(updateData);
                    updateAltonServiceLog(updateData);
                    // updateHmiLog(updateData);
                } else {
                    QStringList previousData = getAltonServiceData();
                    setAltonServiceData(data);
                    if ((data.size() - previousData.size()) > 0) {
                        QString updateDataInfo = QString();
                        for (const auto& d : data.mid(previousData.size(), data.size())) {
                            QStringList temp = d.split(LOG_FILE_KEYWORD_01);
                            if (temp.size() >= 2) {
                                updateDataInfo.append(QString("%1\n").arg(temp.at(1)));
                            }
                        }
                        updateAltonClientLog(updateDataInfo);
                    }
                }
            });
    connect(mLogFileWatcher[type].data(), &ivis::common::FileSystemWatcherThread::signalWatcherFileReadError,
            [=](const QString& errorFile) { qDebug() << "\t\t mFileWatcher File Error :" << errorFile; });
    connect(mLogFileWatcher[type].data(), &ivis::common::FileSystemWatcherThread::signalWatcherFileState, [=](const int& state) {
        qDebug() << "\t\t FileWatcher File State :" << state << ((state >= 0) ? ("-> Complete") : ("-> Fail"));
    });
#else
    if (mFileWatcher == nullptr) {
        mFileWatcher =
            QSharedPointer<ivis::common::FileSystemWatcherThread>(new ivis::common::FileSystemWatcherThread(watcherFile, 50));
    }
    mFileWatcher.data()->start();
    connect(mFileWatcher.data(), &ivis::common::FileSystemWatcherThread::signalWatcherFileDataChanged,
            [=](const bool& init, const QStringList& data) {
                QStringList previousData = getAltonServiceData();
                setAltonServiceData(data);
                if ((data.size() - previousData.size()) > 0) {
                    QString updateData = QString();
                    for (const auto& d : data.mid(previousData.size(), data.size())) {
                        QStringList temp = d.split(LOG_FILE_KEYWORD_01);
                        if (temp.size() >= 2) {
                            updateData.append(QString("%1\n").arg(temp.at(1)));
                        }
                    }
                    updateAltonClientLog(updateData);
                    updateAltonServiceLog(updateData);
                    updateHmiLog(updateData);
                }
            });
    connect(mFileWatcher.data(), &ivis::common::FileSystemWatcherThread::signalWatcherFileReadError,
            [=](const QString& errorFile) { qDebug() << "\t\t mFileWatcher File Error :" << errorFile; });
    connect(mFileWatcher.data(), &ivis::common::FileSystemWatcherThread::signalWatcherFileState, [=](const int& state) {
        qDebug() << "\t\t FileWatcher File State :" << state << ((state >= 0) ? ("-> Complete") : ("-> Fail"));
    });
#endif
}

void SubWindow::stopWatcherFile(const int& type) {
#if 1
    for (auto iter = mLogFileWatcher.begin(); iter != mLogFileWatcher.end(); ++iter) {
        if (iter.value().isNull() == false) {
            disconnect(iter.value().data());
            iter.value().reset();
        }
    }
#else
    if (mFileWatcher.isNull() == false) {
        disconnect(mFileWatcher.data());
        mFileWatcher.reset();
    }
#endif
}

bool SubWindow::isDateTimeValid(const QString& base, const QString& input) {
    QDateTime baseDateTime = QDateTime::fromString(base, "yyyy-MM-dd HH:mm:ss.zzz");
    QDateTime inputDateTime = QDateTime::fromString(input, "yyyy-MM-dd HH:mm:ss.zzz");

    if ((baseDateTime.isValid() == false) || (inputDateTime.isValid() == false)) {
        qDebug() << "Fali to datetime invalid :" << baseDateTime << inputDateTime;
        return false;
    }

    bool valid = (inputDateTime > baseDateTime);
    qDebug() << "DateTime :" << ((valid) ? ("valid, ") : ("invalid, ")) << baseDateTime << inputDateTime;
    return valid;
}

QStringList SubWindow::isVsmFileInfo(const QStringList& powerTrainList, const QStringList& signalList) {
    QStringList fileList = QStringList();
    QString vsmPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeVsmPath).toString();

    QFileInfoList vsmFileList = QFileInfoList();
    QStringList fileInfo = ivis::common::FileInfo::isFileListInfo(vsmPath, QString(".vsm"), vsmFileList);
    if (fileInfo.size() == 0) {
        qDebug() << "Vsm path error :" << vsmPath;
        QVariant popupData = QVariant();
        ivis::common::Popup::drawPopup(ivis::common::PopupType::PathError, this, popupData,
                                       QVariant(QVariantList({STRING_POPUP_VSM_PATH_ERROR, STRING_POPUP_VSM_PATH_ERROR_TIP})));
        return QStringList();
    }

    for (const auto& signal : signalList) {
        if (signal.contains("Vehicle.") == false) {
            continue;
        }

        QStringList temp = signal.split(".");
        if (temp.size() < 2) {
            continue;
        }
        if (signal.contains("Vehicle.System.")) {
            fileList.append(QString("%1/%2.%3.vsm").arg(vsmPath).arg(temp.at(1)).arg(temp.at(2)));
        } else {
            for (const auto& pt : powerTrainList) {
                if (pt.size() == 0) {
                    continue;
                }
                fileList.append(QString("%1/CLU_VSM_%2.Vehicle.%3.vsm").arg(vsmPath).arg(pt).arg(temp.at(1)));
            }
        }
    }

    fileList.removeDuplicates();
    return fileList;
}

QList<QPair<QString, QString>> SubWindow::isReplaceSignal(const QStringList& abstractionList, const QStringList& vsmFileList) {
    // const QString PREFIX_TYPE = QString("type:");
    const QString PREFIX_SIGNAL_NAME = QString("signalName:");
    // const QString PREFIX_DATA_TYPE = QString("dataType:");
    // const QString PREFIX_DESCRIPTION = QString("description:");
    // const QString PREFIX_ABSTRACTION_NAME = QString("abstractionName:");
    // const QString PREFIX_VALUE_ENUM = QString("valueEnum:");
    // const QString PREFIX_MATCHING_TABLE = QString("matchingTable:");
    // const QString PREFIX_CODE_COMMENTING = QString("# ");
    const QString PREFIX_HYPHEN = QString("-");
    // const QString PREFIX_DOT = QString(".");
    const QString PREFIX_COLON = QString(":");
    // const QString PREFIX_SPACE = QString(" ");

    QList<QPair<QString, QString>> signalList = QList<QPair<QString, QString>>();

    if (vsmFileList.size() == 0) {
        return signalList;
    }

    int index = 0;
    for (const auto& abstraction : abstractionList) {
        // qDebug() << index << ". >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
        QStringList temp = abstraction.split(" ");
        if (temp.size() < 2) {
            continue;
        }
        QString value = temp[1];
        temp = temp[0].split(".");
        if (temp.size() < 3) {
            continue;
        }

        QString sfcSignal = QString();
        if (abstraction.contains("Vehicle.System.")) {
            sfcSignal = temp.at(3);
        } else {
            sfcSignal = QString("%1.%2").arg(temp.at(2)).arg(temp.at(3));
        }

        QString signalName = QString();
        for (const auto& file : vsmFileList) {
            QStringList readData = ivis::common::FileInfo::readFile(file);
            bool foundAbstractionName = false;
            for (const auto& lineStr : readData) {
                if (foundAbstractionName) {
                    if ((lineStr.contains(PREFIX_SIGNAL_NAME))) {
                        signalName = lineStr;
                        signalName.remove(" ");
                        signalName.remove(PREFIX_SIGNAL_NAME);
                        break;
                    } else if ((lineStr.contains(PREFIX_HYPHEN)) && (lineStr.contains(PREFIX_COLON))) {
                        break;
                    } else {
                    }
                    continue;
                }

                if ((lineStr.contains(sfcSignal)) && (lineStr.contains(PREFIX_HYPHEN)) && (lineStr.contains(PREFIX_COLON))) {
                    foundAbstractionName = true;
                }
            }

            if (signalName.size() > 0) {
                QString replaceData = QString("%1 %2").arg(signalName).arg(value);
                signalList.append(QPair<QString, QString>(abstraction, replaceData));
                // qDebug() << "\t File :" << file;
                // qDebug() << "\t      :" << abstraction << "->" << replaceData;
                break;
            }
        }
        index++;
    }
    // qDebug() << "==================================================================================================\n\n";

    return signalList;
}

QString SubWindow::isDataType(const QString& value) {
    bool result;
    value.toULongLong(&result);
    if (result) {
        return "uint64_t";
    }
    value.toLongLong(&result);
    if (result) {
        return "int64_t";
    }
    value.toDouble(&result);
    if (result) {
        return "double";
    }
    if ((value.toLower() == QString("true")) || (value.toLower() == QString("false"))) {
        return "bool";
    }
    return "string";
}

void SubWindow::settingPath(const int& pathType) {
    int configType = ConfigInfo::ConfigTypeInvalid;
    QString popupText = QString();

    switch (pathType) {
        case PathTypeVSM: {
            configType = ConfigInfo::ConfigTypeVsmPath;
            popupText = STRING_VSM_PATH;
            break;
        }
        case PathTypeTAV: {
            configType = ConfigInfo::ConfigTypeTavPath;
            popupText = STRING_TAV_PATH;
            break;
        }
        case PathTypeAltonClient: {
            configType = ConfigInfo::ConfigTypeAltonPath;
            popupText = STRING_ALTON_CLIENT_PATH;
            break;
        }
        default: {
            break;
        }
    }

    if (configType != ConfigInfo::ConfigTypeInvalid) {
        QVariant popupData = QVariant();
        QVariant path = ConfigSetting::instance().data()->readConfig(configType);
        if (path.isNull()) {
            path = ivis::common::APP_PWD();
        }
        QVariantList info = QVariantList({popupText, path});
        if (ivis::common::Popup::drawPopup(ivis::common::PopupType::SettingPath, this, popupData, QVariant(info)) ==
            ivis::common::PopupButton::OK) {
            qDebug() << "Path :" << popupData;
            ConfigSetting::instance().data()->writeConfig(configType, popupData);
        }
    }
}

QString SubWindow::openTavFile() {
    QVariant popupData = QVariant();
    QVariant tavPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeTavPath);
    QString openPath = QString();

    if (tavPath.isNull()) {
        tavPath = ivis::common::APP_PWD();
    }

    QVariantList info = QVariantList({STRING_OPEN_FOLDER, tavPath});
    if (ivis::common::Popup::drawPopup(ivis::common::PopupType::OpenFolder, this, popupData, QVariant(info)) ==
        ivis::common::PopupButton::OK) {
        openPath = popupData.toString();
        qDebug() << "Open Folder :" << openPath;
    }

    return openPath;
}

bool SubWindow::deleteFile(const int& deleteType) {
    bool result = false;
    QString tavPath = getTavPath();
    QVariant popupData = QVariant();
    QVariantList text = QVariantList();
    QString title = QString();
    QString tip = QString();

    switch (deleteType) {
        case DeleteTypeSelectTAV: {
            title = QString(STRING_FILE_DELETE).arg(STRING_FILE_DELETE_TAV);
            tip = QString(STRING_FILE_DELETE_TIP).arg(STRING_FILE_DELETE_TAV);
            text = QVariantList({title, tip, STRING_POPUP_OK, STRING_POPUP_CANCEL});
            if (ivis::common::Popup::drawPopup(ivis::common::PopupType::DeleteFile, this, popupData, QVariant(text)) ==
                ivis::common::PopupButton::OK) {
                for (const auto& file : getDeleteFileList()) {
                    result = ivis::common::FileInfo::deleteFile(tavPath, file);
                    qDebug() << "DeleteTypeSelectTAV :" << ((result) ? ("Success") : ("Fail")) << file;
                }
            }
            break;
        }
        case DeleteTypeTAV: {
            updateFileList(ListTypeCheck, nullptr);
            break;
        }
        case DeleteTypeScript: {
            title = QString(STRING_FILE_DELETE).arg(STRING_FILE_DELETE_SCRIPT);
            tip = QString(STRING_FILE_DELETE_TIP).arg(STRING_FILE_DELETE_SCRIPT);
            text = QVariantList({title, tip, STRING_POPUP_OK, STRING_POPUP_CANCEL});
            if (ivis::common::Popup::drawPopup(ivis::common::PopupType::DeleteFile, this, popupData, QVariant(text)) ==
                ivis::common::PopupButton::OK) {
                QString extensions = QString("*.sh");
                result = ivis::common::FileInfo::deleteFile(tavPath, extensions);
                qDebug() << "DeleteTypeScript :" << ((result) ? ("Success") : ("Fail")) << extensions;
            }
            break;
        }
        case DeleteTypeInfo: {
            title = QString(STRING_FILE_DELETE).arg(STRING_FILE_DELETE_INFO);
            tip = QString(STRING_FILE_DELETE_TIP).arg(STRING_FILE_DELETE_INFO);
            text = QVariantList({title, tip, STRING_POPUP_OK, STRING_POPUP_CANCEL});
            if (ivis::common::Popup::drawPopup(ivis::common::PopupType::DeleteFile, this, popupData, QVariant(text)) ==
                ivis::common::PopupButton::OK) {
                QString extensions = QString("*.info");
                result = ivis::common::FileInfo::deleteFile(tavPath, extensions);
                qDebug() << "DeleteTypeInfo :" << ((result) ? ("Success") : ("Fail")) << extensions;
            }
            break;
        }
        default: {
            break;
        }
    }

    if (result) {
        QVariant popupData = QVariant();
        ivis::common::Popup::drawPopup(
            ivis::common::PopupType::DeleteFileComplete, this, popupData,
            QVariant(QVariantList({STRING_POPUP_DELETE_FILE_COMPLETE, STRING_POPUP_DELETE_FILE_COMPLETE_TIP})));
    }

    return result;
}

void SubWindow::controlScript(const int& scriptStart, const QString& fileName) {
    QStringList scriptFileList = QStringList();
    QString selectFile = fileName;
    int previousScriptStart = getScriptStart();

    qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    qDebug() << "controlScript :" << previousScriptStart << "->" << scriptStart << ", SelectFile :" << fileName;

    setScriptStart(scriptStart);
    setSelectFile(selectFile);

    switch (scriptStart) {
        case StartScriptTypeStart: {
            selectFile.replace(".tav", ".sh");
            drawDisplay(DisplayTypeViewScript, selectFile);
            if (getTavSave()) {
                setTavSave(false);
                mGui->detailSave->setVisible(false);
                updateDetailDataInfo(QString());
            }
            updateDetailFileInfo(ViewTypeScript, createToScript(selectFile, scriptFileList));
            break;
        }
        case StartScriptTypeMenuStart: {
            selectFile.replace(".tav", ".sh");
            drawDisplay(DisplayTypeViewScript, selectFile);
            updateDetailFileInfo(ViewTypeTAV, selectFile);
            updateDetailFileInfo(ViewTypeScript, createToScript(selectFile, scriptFileList));
            break;
        }
        case StartScriptTypeStop: {
            drawDisplay(DisplayTypeViewTav, selectFile);
            if ((previousScriptStart == StartScriptTypeStop) || (previousScriptStart == StartScriptTypeMenuStop)) {
                updateDetailFileInfo(ViewTypeTAV, selectFile);
            } else {
                updateDetailFileInfo(ViewTypeRedrawTAV, getTavData());
            }
            break;
        }
        case StartScriptTypeMenuStop: {
            drawDisplay(DisplayTypeMain);
            break;
        }
        default: {
            break;
        }
    }

    bool start = ((scriptStart == StartScriptTypeStart) || (scriptStart == StartScriptTypeMenuStart));
    excuteScript(start, selectFile, scriptFileList);

    qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
}

void SubWindow::controlFileWatcher(const bool& start) {
    QString watcherFile = QString("%1/%2").arg(getTavPath()).arg("TAV.info");

#if defined(USE_FILE_WATCHER_QT)
    if (start == false) {
        if (mFileWatcher.isNull() == false) {
            disconnect(mFileWatcher.data());
            mFileWatcher.reset();
        }
        return;
    }

    if (mFileWatcher == nullptr) {
        mFileWatcher =
            QSharedPointer<ivis::common::FileSystemWatcherThread>(new ivis::common::FileSystemWatcherThread(watcherFile, 100));
    }
    mFileWatcher.data()->start();
    connect(mFileWatcher.data(), &ivis::common::FileSystemWatcherThread::signalWatcherFileDataChanged,
            [=](const bool& init, const QStringList& data) {
                QString str;
                for (const auto& d : data) {
                    str.append(QString("%1\n").arg(d));
                }
                onFileDataChanged(str.toStdString());
            });
    connect(mFileWatcher.data(), &ivis::common::FileSystemWatcherThread::signalWatcherFileReadError,
            [=](const QString& errorFile) { qDebug() << "\t mFileWatcher File Error :" << errorFile; });
    connect(mFileWatcher.data(), &ivis::common::FileSystemWatcherThread::signalWatcherFileState, [=](const int& state) {
        qDebug() << "\t mFileWatcher File State :" << state << ((state >= 0) ? ("-> Complete") : ("-> Fail"));
    });
#else
    if (start == false) {
        if (mFileWatcher) {
            mFileWatcher->stopWatch();
        }
        return;
    }

    if (mFileWatcher == nullptr) {
        mFileWatcher = QSharedPointer<LogWatcher>(new LogWatcher);
    }

    mFileWatcher->registerPath(watcherFile.toStdString());
    mFileWatcher->registerSendChangeCbk(std::bind(&SubWindow::onFileDataChanged, this, std::placeholders::_1));
    mFileWatcher->watch();
#endif
}

void SubWindow::onFileDataChanged(const std::string& data) {
    qDebug() << "onFileDataChanged :" << data.c_str();
}
