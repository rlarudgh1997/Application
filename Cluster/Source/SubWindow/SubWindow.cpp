#include "SubWindow.h"
#include "ui_SubWindow.h"
#include "CommonEnum.h"
#include "CommonPopup.h"
#include "CommonResource.h"
#include "ConfigSetting.h"
#include "Service.h"

#include <QDebug>
#include <QMapIterator>

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
    delete mGui;
}

void SubWindow::init() {
    qDebug() << "Service :" << Service::instance().data();
    ConfigSetting::instance();

    drawDisplay(DisplayTypeListMain);
    drawDisplay(DisplayTypeETC);
    controlConnect(true);
    setCurrentTavPath(ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeTavPath).toString());
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
            setSelectFile(item->text());
            setPreviousTavData("");
            drawDisplay(DisplayTypeListSub, getSelectFile());
            updateDetailFileInfo(ViewTypeTAV, getSelectFile());
            mGui->detailContent->setReadOnly(false);
        }
    });
    connect(mGui->detailBack, &QPushButton::clicked, [=]() {
        setSelectFile("");
        drawDisplay(DisplayTypeListMain);
    });
    connect(mGui->detailStart, &QPushButton::clicked, [=]() {
        if (getTavSave()) {
            setTavSave(false);
            mGui->detailSave->setVisible(false);
            updateDetailDataInfo(QString());
        }

        setScriptStart((getScriptStart()) ? (false) : (true));

        if (getScriptStart()) {
            mScriptFileList.clear();
            mGui->detailBack->hide();
            mGui->detailStart->setText("Stop");
            drawDisplay(DisplayTypeListAltonService);
            drawDisplay(DisplayTypeListHmi);
            updateDetailFileInfo(ViewTypeScript, createToScript(getSelectFile(), mScriptFileList));
        } else {
            mGui->detailBack->show();
            mGui->detailStart->setText("Start");
            updateDetailFileInfo(ViewTypeRedrawTAV, getPreviousTavData());
        }
        mGui->detailContent->setReadOnly(getScriptStart());
        excuteScript(getScriptStart(), getSelectFile(), mScriptFileList);
    });
    connect(mGui->detailContent, &QPlainTextEdit::textChanged, [=]() {
        if (getScriptStart()) {
            return;
        }
        setTavSave(getPreviousTavData() != mGui->detailContent->toPlainText());
        mGui->detailSave->setVisible(getTavSave());
    });
    connect(mGui->detailSave, &QPushButton::clicked, [=]() {
        setTavSave(false);
        mGui->detailSave->setVisible(getTavSave());
        updateDetailDataInfo(getSelectFile());
    });

    connect(mGui->altonServiceClear, &QPushButton::clicked, [=]() { mGui->altonServiceContent->hide(); });
    connect(mGui->hmiClear, &QPushButton::clicked, [=]() { mGui->hmiContent->hide(); });

    connect(mGui->actionOpen, &QAction::triggered, [=]() {
        if (getCurrentDisplay() == DisplayTypeListSub) {
            QVariant popupData = QVariant();
            ivis::common::Popup::drawPopup(ivis::common::PopupType::OpenError, this, popupData,
                                           QVariant(QVariantList({STRING_POPUP_OPEN_ERROR, STRING_POPUP_OPEN_ERROR_TIP})));
            qDebug() << "Can't open file on current screen :" << getCurrentDisplay();
            return;
        }
        QString openPath = openTavFile();
        if (openPath.size() > 0) {
            setCurrentTavPath(openPath);
            updateFileList(ListTypeNormal, nullptr);
        }
    });
    connect(mGui->actionTAV_Delete, &QAction::triggered, [=]() { deleteFile(DeleteTypeTAV); });
    connect(mGui->actionScript_Delete, &QAction::triggered, [=]() { deleteFile(DeleteTypeScript); });
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
}

void SubWindow::drawDisplay(const int& type, const QString& text) {
    switch (type) {
        case DisplayTypeListMain: {
            mGui->menubar->show();
            mGui->statusbar->hide();
            mGui->altonClient->setCurrentIndex(1);
            mGui->checkCancel->hide();
            mGui->checkDelete->hide();
            mGui->detailSave->hide();
            mGui->altonServiceContent->hide();
            mGui->hmiContent->hide();
            break;
        }
        case DisplayTypeListSub: {
            mGui->altonClient->setCurrentIndex(2);
            mGui->detailTitle->setText(text);
            mGui->detailTitle->setAlignment(Qt::AlignCenter);
            break;
        }
        case DisplayTypeListAltonService: {
            mGui->altonServiceContent->show();
            break;
        }
        case DisplayTypeListHmi: {
            mGui->hmiContent->show();
            break;
        }
        case DisplayTypeETC: {
            QString compalnyText("<a href='https://ivis.ai'>(주)아이비스</a>");
            // QString compalnyText("asreawetgsadf\nURL : <a href='https://jira.ccos.dev/browse/EXNCP-40685'>TICKET NO</a>");
            mGui->companyInfo->setTextInteractionFlags(Qt::TextBrowserInteraction);
            mGui->companyInfo->setOpenExternalLinks(true);
            mGui->companyInfo->setText(compalnyText);
            mGui->companyInfo->show();
            break;
        }
        default: {
            break;
        }
    }
    setCurrentDisplay(type);
}

void SubWindow::updateFileList(const int& type, QListWidgetItem* updateItem) {
    if (mGui->fileList == nullptr) {
        return;
    }

    setListType(type);
    mDeleteFileList.clear();
    switch (type) {
        case ListTypeNormal: {
            QFileInfoList fileList = QFileInfoList();
            QString path = getCurrentTavPath();
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
            mDeleteFileList.append(item->text());
        }
    }

    if (mGui->checkCancel) {
        mGui->checkCancel->setVisible(type != ListTypeNormal);
    }
    if (mGui->checkDelete) {
        mGui->checkDelete->setVisible(mDeleteFileList.size() > 0);
    }
}

void SubWindow::updateDetailFileInfo(const int& viewType, const QString& info) {
    QString title = mGui->detailTitle->text();

    mGui->detailContent->show();
    if (viewType == ViewTypeTAV) {
        int foundType = DetailInfoInvalid;
        QString content = QString();
        QStringList tavData = mOriginalData;
        if (info.size() > 0) {
            QString file = info;
            QString path = QString("%1/%2").arg(getCurrentTavPath()).arg(file);
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
        mDetailInfo = detailInfo;

        // for (auto iter = mDetailInfo.begin(); iter != mDetailInfo.end(); ++iter) {
        //     int index = 0;
        //     for (const auto& data : iter.value().split("\n")) {
        //         qDebug() << iter.key() << ". Data[" << index++ << "] :" << data;
        //     }
        // }

        setPreviousTavData(content);
        mGui->detailContent->setPlainText(content);
        mGui->detailTitle->setText(title.replace(".sh", ".tav"));
    } else if (viewType == ViewTypeScript) {
        QString scriptInfo = info;
        mGui->detailContent->setPlainText(scriptInfo);
        mGui->detailTitle->setText(title.replace(".tav", ".sh"));
    } else if (viewType == ViewTypeRedrawTAV) {
        QString detailInfo = info;
        mGui->detailContent->setPlainText(detailInfo);
        mGui->detailTitle->setText(title.replace(".sh", ".tav"));
    } else {
    }
}

void SubWindow::updateDetailDataInfo(const QString& filePath) {
    QStringList newDetailContent = mGui->detailContent->toPlainText().split("\n");

    if (mOriginalData != newDetailContent) {
        qDebug() << "updateDetailDataInfo :" << mOriginalData.size() << "->" << newDetailContent.size();
        mOriginalData = newDetailContent;
        updateDetailFileInfo(ViewTypeTAV, QString());
        writeOriginalData(filePath, newDetailContent);
    }
}

void SubWindow::writeOriginalData(const QString& filePath, const QStringList& saveData) {
    if ((filePath.size() == 0) || (saveData.size() == 0)) {
        return;
    }

    QString writeContent = QString();
    QString path = QString("%1/%2").arg(getCurrentTavPath()).arg(filePath);
    for (const auto& data : saveData) {
        writeContent.append(data);
        writeContent.append("\n");
    }
    qDebug() << "writeOriginalData :" << saveData.size() << path;
    ivis::common::FileInfo::writeFile(path, writeContent, false);
}

bool SubWindow::isDetailInfo(const int& type, QPair<QString, QStringList>& detailInfo) {
    detailInfo = QPair<QString, QStringList>();

    if (mDetailInfo[type].size() == 0) {
        qDebug() << "Fail to detail info size : 0";
        return false;
    }

    for (auto& lineStr : mDetailInfo[type].split("\n")) {
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

QString SubWindow::isToScriptInfo(const int& type, QStringList& infoList) {
    QPair<QString, QStringList> detailInfo = QPair<QString, QStringList>();
    if (isDetailInfo(type, detailInfo) == false) {
        return QString();
    }

    QString scriptInfo = QString();

    infoList.clear();
    if ((type == DetailInfoDescription) || (type == DetailInfoExpectedResult)) {
        scriptInfo.append(QString("#%1\n").arg(detailInfo.first));
        for (const auto& detail : detailInfo.second) {
            scriptInfo.append(QString("#%1\n").arg(detail));
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
        scriptInfo.append(QString("$ALTON_CLIENT listen%1 &\n").arg(listenSignal));
    } else {
    }

    return scriptInfo;
}

QString SubWindow::createToScript(const QString& file, QStringList& scriptFileList) {
    QStringList powerTrain = QStringList();
    QStringList abstractionSignalList = QStringList();
    QStringList signalList = QStringList();
    QString altonClient = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAltonClient).toString();
    QString altonClientPath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeAltonClientPath).toString();
    QString scriptInfo("#!/bin/bash");

    // AltonClient Path
    scriptInfo.append("\n\n");
    scriptInfo.append("#[AltonClient]\n");
#if 1
    scriptInfo.append("ALTON_CLIENT=\"$1\"\n");
    scriptInfo.append(QString("if [ -z \"$1\" ]; then\n    ALTON_CLIENT=\"%1/%2\"\nfi\n").arg(altonClientPath).arg(altonClient));
#else
    scriptInfo.append(QString("ALTON_CLIENT=\"%1\"\n").arg(altonClient));
    scriptInfo.append(QString("if [ -n \"$1\" ]; then\n    ALTON_CLIENT=\"$1\"\nfi\n"));
#endif
    scriptInfo.append(QString("echo \"ALTON_CLIENT=$ALTON_CLIENT\"\n"));

    // Description
    scriptInfo.append("\n\n");
    scriptInfo.append(isToScriptInfo(DetailInfoDescription, signalList));

    // PowerTrain
    scriptInfo.append("\n\n");
    scriptInfo.append(isToScriptInfo(DetailInfoPowerTrain, powerTrain));

    // Precondition
    scriptInfo.append("\n\n");
    scriptInfo.append(isToScriptInfo(DetailInfoPrecondition, signalList));
    abstractionSignalList.append(signalList);

    // Listen
    scriptInfo.append("\n\n");
    scriptInfo.append(isToScriptInfo(DetailInfoListen, signalList));

    // Step
    scriptInfo.append("\n\n");
    scriptInfo.append(isToScriptInfo(DetailInfoStep, signalList));
    abstractionSignalList.append(signalList);

    // ExpectedResult
    scriptInfo.append("\n\n");
    scriptInfo.append(isToScriptInfo(DetailInfoExpectedResult, signalList));

    // AbstractionSignalList
    // for (const auto& signal : abstractionSignalList) {
    //     qDebug() << "AbstractionSignal :" << signal;
    // }

    // Save : power train type
    QStringList pasingFileList = isVsmFileInfo(powerTrain, abstractionSignalList);
    QString fileName = file;
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

        // QString path = QString("%1/../TAV/%2.%3.sh").arg(ivis::common::APP_PWD()).arg(fileName.remove(".tav")).arg(pt);
        QString path = QString("%1/%2.%3.sh").arg(getCurrentTavPath()).arg(fileName.remove(".tav")).arg(pt);
        ivis::common::FileInfo::writeFile(path, writeContent, false);
        scriptFileList.append(path);
    }
    return scriptInfo;
}

void SubWindow::excuteScript(const bool& start, const QString& file, const QStringList& scriptFileList) {
    qDebug() << "excuteScript :" << ((start) ? ("[Start]") : ("[Stop]")) << ", ScriptFile :" << file << scriptFileList.size();

    int index = 0;
    if (start) {
        QString logFile = file;
        logFile.remove(".tav");

        qDebug() << "\t 1. start : altonservice";
        qDebug() << "\t 2. start : watcher -" << QString("%1.info").arg(logFile);
        qDebug() << "\t 3. start : chmod -R 777 *.sh";
        for (const auto& script : scriptFileList) {
            // startProcess(script, QString("./%1.info").arg(logFile));
            qDebug() << "\t" << QString("4-%1. start : %2 >> %3.info").arg(index++).arg(script).arg(logFile);
        }
    } else {
        qDebug() << "\t 1. stop : altonservice";
        qDebug() << "\t 2. stop : watcher";
        for (const auto& script : scriptFileList) {
            qDebug() << "\t" << QString("3-%1. stop : %2").arg(index++).arg(script);
        }
        qDebug() << "\t 4. stop : delete watcher info file";
    }
}

void SubWindow::startProcess(const QString& command, const QString& arg) {
    stopProcess();
    QSharedPointer<ivis::common::ExcuteProgramThread> mProcess = nullptr;

    mProcess =
        QSharedPointer<ivis::common::ExcuteProgramThread>(new ivis::common::ExcuteProgramThread(false), &QObject::deleteLater);
    mProcess.data()->setCommandInfo(command, QString(" >> %1").arg(arg));
    mProcess.data()->start();
    connect(
        mProcess.data(), &ivis::common::ExcuteProgramThread::signalExcuteProgramInfo, [=](const bool& start, const bool& result) {
            if (start == false) {
                qDebug() << "*************************************************************************************************";
                qDebug() << "Commnad :" << command;
                qDebug() << "Result  :" << ((result) ? ("sucess") : ("fail"));
                qDebug() << "*************************************************************************************************\n";
            }
        });
}

void SubWindow::stopProcess() {
}

void SubWindow::startWatcherFile(const int& type, const QString& watcherFile) {
}

void SubWindow::stopWatcherFile(const int& type) {
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
            configType = ConfigInfo::ConfigTypeAltonClientPath;
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
    QVariant path = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeTavPath);
    QString openPath = QString();

    if (path.isNull()) {
        path = ivis::common::APP_PWD();
    }

    QVariantList info = QVariantList({STRING_OPEN_FOLDER, path});
    if (ivis::common::Popup::drawPopup(ivis::common::PopupType::OpenFolder, this, popupData, QVariant(info)) ==
        ivis::common::PopupButton::OK) {
        openPath = popupData.toString();
        qDebug() << "Open Folder :" << openPath;
    }

    return openPath;
}

bool SubWindow::deleteFile(const int& deleteType) {
    bool result = false;
    QVariant popupData = QVariant();
    QVariantList text = QVariantList();
    QString path = getCurrentTavPath();

    switch (deleteType) {
        case DeleteTypeSelectTAV: {
            text = QVariantList({STRING_FILE_DELETE_TAV, STRING_FILE_DELETE_TAV_TIP, STRING_POPUP_OK, STRING_POPUP_CANCEL});
            if (ivis::common::Popup::drawPopup(ivis::common::PopupType::DeleteFile, this, popupData, QVariant(text)) ==
                ivis::common::PopupButton::OK) {
                for (const auto& file : mDeleteFileList) {
                    // path = QString("%1/../TAV").arg(ivis::common::APP_PWD());
                    result = ivis::common::FileInfo::deleteFile(path, file);
                    qDebug() << "DeleteTypeSelectTAV :" << ((result) ? ("Sucess") : ("Fail")) << file;
                }
            }
            break;
        }
        case DeleteTypeTAV: {
            updateFileList(ListTypeCheck, nullptr);
            break;
        }
        case DeleteTypeScript: {
            text = QVariantList({STRING_FILE_DELETE_SCRIPT, STRING_FILE_DELETE_SCRIPT_TIP, STRING_POPUP_OK, STRING_POPUP_CANCEL});
            if (ivis::common::Popup::drawPopup(ivis::common::PopupType::DeleteFile, this, popupData, QVariant(text)) ==
                ivis::common::PopupButton::OK) {
                QString extensions = QString("*.sh");
                // path = QString("%1/../TAV").arg(ivis::common::APP_PWD());
                result = ivis::common::FileInfo::deleteFile(path, extensions);
                qDebug() << "DeleteTypeScript :" << ((result) ? ("Sucess") : ("Fail")) << extensions;
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
