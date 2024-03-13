#include "SubWindow.h"
#include "ui_SubWindow.h"
#include "CommonEnum.h"
#include "CommonUtil.h"
#include "Service.h"

#include <QDebug>
#include <QMapIterator>

const QString KeyWordDescription = QString("[Description]");
const QString KeyWordPowerTrain = QString("[PowerTrain]");
const QString KeyWordPrecondition = QString("[Precondition]");
const QString KeyWordStep = QString("[Step]");
const QString KeyWordExpectedResult = QString("[Expected Result]");
const QString KeyWordGroup = QString("    [Group]");
const QString KeyWordPeriod = QString("    [Period]");
const QString KeyWordPeriodGroup = QString("        [Group]");
const QString KeyWordListen = QString("[Listen]");
const QString AltonClient = QString("./alton_client");
const QString PowerTrainList = QString("ICV, EV, FCEV, HEV, PHEV");


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

    drawDisplay(DisplayTypeListMain);
    drawDisplay(DisplayTypeETC);
    updateFileList();
    controlConnect(mInit);
}

void SubWindow::controlConnect(const bool& state) {
    if (mInit) {
        qDebug() << "ControlConnect : Completed";
        return;
    }
    mInit = true;

    connect(mGui->fileList, &QListWidget::itemPressed, [=](QListWidgetItem* item) {
        if (item) {
            qDebug() << "itemPressed :" << item;
            // QTimer::singleShot(1500, this, [&]() {
            //     emit signalTimerEvent();
            // });
            mTimerTouch->start(1000);
        }
    });
    connect(mGui->fileList, &QListWidget::itemClicked, [=](QListWidgetItem *item) {
        if (item) {
            qDebug() << "itemClicked :" << item;
            mTimerTouch->stop();
        }
    });
    connect(mTimerTouch, &QTimer::timeout, [=]() {
        qDebug() << "timeout";
    });
    connect(this, &SubWindow::signalTimerEvent, [=]() {
        qDebug() << "signalTimerEvent";
    });

    connect(mGui->fileList, &QListWidget::itemDoubleClicked, [=](QListWidgetItem* item) {
        if (item) {
            mSelectFile = item->text();
            mPreviousTavData.clear();
            drawDisplay(DisplayTypeListSub, mSelectFile);
            updateDetailFileInfo(ViewTypeTAV, mSelectFile);
            mGui->detailContent->setReadOnly(false);
        }
    });
    connect(mGui->detailBack, &QPushButton::clicked, [=]() {
        mSelectFile.clear();
        drawDisplay(DisplayTypeListMain);
    });
    connect(mGui->detailStart, &QPushButton::clicked, [=]() {
        if (mTavSave) {
            mTavSave = false;
            mGui->detailSave->setVisible(false);
            updateDetailDataInfo(QString());
        }

        mStart = !mStart;
        if (mStart) {
            mGui->detailBack->hide();
            mGui->detailStart->setText("Stop");
            drawDisplay(DisplayTypeListAltonService);
            drawDisplay(DisplayTypeListHmi);
            updateDetailFileInfo(ViewTypeScript, createToScript(mSelectFile));
        } else {
            mGui->detailBack->show();
            mGui->detailStart->setText("Start");
            updateDetailFileInfo(ViewTypeRedrawTAV, mPreviousTavData);
        }
        mGui->detailContent->setReadOnly(mStart);
    });
    connect(mGui->detailContent, &QPlainTextEdit::textChanged, [=]() {
        if (mStart) {
            return;
        }
        mTavSave = (mPreviousTavData != mGui->detailContent->toPlainText());
        mGui->detailSave->setVisible(mTavSave);
    });
    connect(mGui->detailSave, &QPushButton::clicked, [=]() {
        mTavSave = false;
        mGui->detailSave->setVisible(mTavSave);
        updateDetailDataInfo(mSelectFile);
    });

    connect(mGui->altonServiceClear, &QPushButton::clicked, [=]() { mGui->altonServiceContent->hide(); });
    connect(mGui->hmiClear, &QPushButton::clicked, [=]() { mGui->hmiContent->hide(); });

    connect(mGui->actionDelete, &QAction::triggered, [=]() {
        QString path = QString("%1/../TAV").arg(ivis::common::APP_PWD());
        bool result = ivis::common::FileInfo::deleteFile(path, QString("*.sh"));
        qDebug() << "Delete Script File :" << ((result) ? ("Sucess") : ("Fail"));
    });
}

void SubWindow::drawDisplay(const int& type, const QString& text) {
    switch (type) {
        case DisplayTypeListMain: {
            mGui->menubar->show();
            mGui->statusbar->hide();
            mGui->altonClient->setCurrentIndex(1);
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
}
void SubWindow::updateFileList() {
    if (mGui->fileList->count() > 0) {
        return;
    }
    QFileInfoList fileList = QFileInfoList();
    QString path = QString("%1/../TAV").arg(ivis::common::APP_PWD());
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
}

void SubWindow::updateDetailFileInfo(const int& viewType, const QString& info) {
    mGui->detailContent->show();

    if (viewType == ViewTypeTAV) {
        int foundType = DetailInfoInvalid;
        QString content = QString();
        QStringList tavData = mOriginalData;
        if (info.size() > 0) {
            QString file = info;
            QString path = QString("%1/../TAV/%2").arg(ivis::common::APP_PWD()).arg(file);
            tavData = ivis::common::FileInfo::readFile(path);
        }

#if defined(USE_TAV_PARSING_NEW)
        QMap<int, QString> detailInfo = QMap<int, QString>();
        for (const auto& data : tavData) {
            if (data.contains(KeyWordDescription)) {
                foundType = DetailInfoDescription;
            } else if (data.contains(KeyWordPowerTrain)) {
                foundType = DetailInfoPowerTrain;
            } else if (data.contains(KeyWordPrecondition)) {
                foundType = DetailInfoPrecondition;
            } else if (data.contains(KeyWordStep)) {
                foundType = DetailInfoStep;
            } else if (data.contains(KeyWordExpectedResult)) {
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
#else
        mDetailInfo.clear();
        for (const auto& data : tavData) {
            if (data.size() == 0) {
                foundType = DetailInfoInvalid;
            }

            if (data.contains("[Description]")) {
                foundType = DetailInfoDescription;
            } else if (data.contains("[PowerTrain]")) {
                foundType = DetailInfoPowerTrain;
            } else if (data.contains("[Precondition]")) {
                foundType = DetailInfoPrecondition;
            } else if (data.contains("[Period]")) {
                foundType = DetailInfoPeriod;
            } else if (data.contains("[Step]")) {
                foundType = DetailInfoStep;
            } else if (data.contains("[Group]")) {
                if (foundType == DetailInfoPrecondition) {
                    foundType = DetailInfoPreconditionGroup;
                } else if (foundType == DetailInfoPeriod) {
                    foundType = DetailInfoPeriodGroup;
                } else if (foundType == DetailInfoStep) {
                    foundType = DetailInfoStepGroup;
                } else {
                    qDebug() << "Fail to found keyword [Group]";
                }
            } else if (data.contains("[Expected Result]")) {
                foundType = DetailInfoExpectedResult;
            } else {
            }

            if (foundType != DetailInfoInvalid) {
                mDetailInfo[foundType].append(QString("%1\n").arg(data));
                // qDebug() << "[" << foundType << "] " << data;

                if (foundType == DetailInfoExpectedResult) {
                    if (mDetailInfo[DetailInfoListen].size() == 0) {
                        mDetailInfo[DetailInfoListen].append("[Listen]\n");
                        // qDebug() << "\t [" << DetailInfoListen << "] " << "[Listen]";
                    } else {
                        QString temp = data;
                        QStringList tempList = temp.remove("    ").split(" ");
                        if (tempList.size() == 2) {
                            mDetailInfo[DetailInfoListen].append(QString("    %1\n").arg(tempList.at(0)));
                            // qDebug() << "\t [" << DetailInfoListen << "] " << tempList.at(0);
                        }
                    }
                }
            }

#if 1
            content.append(data);
            content.append("\n");
#else
            QRegularExpression regex("https://[^\\s]+");
            QRegularExpressionMatch match = regex.match(data);
            if (match.hasMatch()) {
                QString url = QString("\nURL <a href='%1'>%2</a>").arg(match.captured(0)).arg(data);
                qDebug() << "Found URL:" << url;
                content.append(url);
            } else {
                content.append(data);
                content.append("\n");
            }
#endif
        }

        int powerTrainSize = mDetailInfo[DetailInfoPowerTrain].split("\n").size();
        if (powerTrainSize != 3) {
            QString oldData = mDetailInfo[DetailInfoPowerTrain];
            QString newData = QString();

            if (powerTrainSize < 3) {
                mDetailInfo[DetailInfoPowerTrain].append("    ALL\n");
                newData = mDetailInfo[DetailInfoPowerTrain];
                if (powerTrainSize == 2) {
                    newData.append("\n");
                }
            } else {
                QStringList powerTrainInfo = mDetailInfo[DetailInfoPowerTrain].split("\n");
                int count = 0;
                for (const auto& powerTrain : powerTrainInfo) {
                    if (count < 2) {
                        newData.append((count <= 1) ? (QString("%1\n").arg(powerTrain)) : ("\n"));
                    } else {
                        break;
                    }
                    count++;
                }
                mDetailInfo[DetailInfoPowerTrain] = newData;
            }
            content.replace(oldData, newData);
        }

        // for (const auto& detailInfo : mDetailInfo) {
        //     qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
        //     int count = 0;
        //     for (QString info : detailInfo.split("\n")) {
        //         qDebug() << count++ << ":" << info;
        //     }
        //     qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
        // }
#endif

        mPreviousTavData = content;
        mGui->detailContent->setPlainText(content);
    } else if (viewType == ViewTypeScript) {
        QString scriptInfo = info;
        mGui->detailContent->setPlainText(scriptInfo);
    } else if (viewType == ViewTypeRedrawTAV) {
        QString detailInfo = info;
        mGui->detailContent->setPlainText(detailInfo);
    } else {
    }
}

void SubWindow::updateDetailDataInfo(const QString& filePath) {
    QStringList newDetailContent = mGui->detailContent->toPlainText().split("\n");
    // QString oldDetailContent = mPreviousTavData;

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
    QString path = QString("%1/../TAV/%2").arg(ivis::common::APP_PWD()).arg(filePath);
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
            detailInfo.first = KeyWordDescription;
        } else if (type == DetailInfoPowerTrain) {
            detailInfo.first = KeyWordPowerTrain;
        } else if (type == DetailInfoPrecondition) {
            detailInfo.first = KeyWordPrecondition;
        } else if (type == DetailInfoStep) {
            detailInfo.first = KeyWordStep;
        } else if (type == DetailInfoExpectedResult) {
            detailInfo.first = KeyWordExpectedResult;
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
    QString normalSpace = QString("    ");                 // Space : 4
    QString groupSpace = QString("        ");              // Space : 8
    QString periodSpace = groupSpace;                      // Space : 8
    QString periodGroupSpace = QString("            ");    // Space : 12

    subDetialInfo.clear();
    for (const auto lineStr : inputStr) {
        bool appendState = false;
        if (lineStr.contains(KeyWordPeriodGroup)) {
            subDetailType = DetailSubInfoPeriodGroup;
        } else if (lineStr.contains(KeyWordPeriod)) {
            subDetailType = DetailSubInfoPeriod;
        } else if (lineStr.contains(KeyWordGroup)) {
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
        } else if (subDetailType == DetailSubInfoPeriod) {
            appendState = (lineStr.contains(periodSpace));
        } else if (subDetailType == DetailSubInfoPeriodGroup) {
            appendState = (lineStr.contains(periodGroupSpace));
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
        // qDebug() << "Fail to check input string :" << inputStr;
    } else {
        if (type == DetailSubInfoNormal) {
            if (inputStr.contains("SFC.")) {
                convertStr = QString("%1 set %2 %3 %4").arg(AltonClient)
                                                    .arg(signal)
                                                    .arg(isDataType(value))
                                                    .arg(value);
            } else if (inputStr.contains("delay")) {
                convertStr = QString("sleep %1").arg(value.toDouble() * (double)0.001);
            } else {
                if (inputStr.contains("Vehicle.")) {
                    abstractionSignal = inputStr;
                }
                convertStr = QString("%1 inject %2").arg(AltonClient).arg(inputStr);
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

    // qDebug() << "\t Input :" << inputList.size() << signal << ":" << signal;
    // qDebug() << "\t Signal :" << inputStr << "->" << convertStr << abstractionSignal;

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
            currPowerTrain = PowerTrainList;
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
            scriptInfo.append(QString("%1 inject%2\n").arg(AltonClient).arg(groupSignal));
        }

        // Period
        int periodCycle = 0;
        int periodDuration = 0;
        QString periodSignal = QString();
        for (const auto& period : subDetialInfo[DetailSubInfoPeriod]) {
            QString abstractionSignal = isDetailSignalInfo(DetailSubInfoPeriod, period, signalInfo);
            if (abstractionSignal.size() > 0) {
               infoList.append(abstractionSignal);
            }

            if (signalInfo.contains("Info :")) {
                QStringList temp = signalInfo.remove("Info :").remove(" ").split("/");
                if (temp.size() == 2) {
                    periodCycle = temp.at(0).toInt();
                    periodDuration = temp.at(1).toInt();
                } else {
                    qDebug() << "Fail to period info parsing :"<< temp;
                }
            } else {
                periodSignal.append(QString("    %1 inject%2\n").arg(AltonClient).arg(signalInfo));
            }
        }

        if (periodSignal.size() > 0) {
            scriptInfo.append("\n");
            scriptInfo.append(QString("for i in `seq 1 %1`\n").arg(periodCycle));
            scriptInfo.append(QString("do\n"));
            scriptInfo.append(QString("%1").arg(periodSignal));
            scriptInfo.append(QString("#%1\n").arg(KeyWordPeriodGroup));
            scriptInfo.append(QString("    sleep %1\n").arg(periodDuration * (double)0.001));
            scriptInfo.append(QString("done\n"));

            // PeriodGroup
            QString periodGroupSignal = QString();
            for (const auto& periodGroup : subDetialInfo[DetailSubInfoGroup]) {
                QString abstractionSignal = isDetailSignalInfo(DetailSubInfoPeriodGroup, periodGroup, signalInfo);
                if (abstractionSignal.size() > 0) {
                    infoList.append(abstractionSignal);
                }
                periodGroupSignal.append(QString(" %1").arg(signalInfo));
            }
            QString replaceBase = QString("#%1\n").arg(KeyWordPeriodGroup);
            QString replaceNew = QString();
            if (periodGroupSignal.size() > 0) {
                replaceNew = QString("    %1 inject %2\n").arg(AltonClient).arg(periodGroupSignal);
            }
            scriptInfo.replace(replaceBase, replaceNew);
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
        scriptInfo.append(QString("#%1\n").arg(KeyWordListen));
        scriptInfo.append(QString("%1 listen%2 &\n").arg(AltonClient).arg(listenSignal));
    } else {
    }

    return scriptInfo;
}

QString SubWindow::createToScript(const QString& file) {
    QString scriptInfo("#!/bin/bash\n\n");

#if defined(USE_TAV_PARSING_NEW)
    QPair<QString, QStringList> detailInfo = QPair<QString, QStringList>();
    QStringList powerTrain = QStringList();    // PowerTrainList.split(", ");
    QStringList abstractionSignalList = QStringList();
    QStringList signalList = QStringList();

    // Description
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
        for (const auto& ptInfo : PowerTrainList.split(", ")) {
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

        QString path = QString("%1/../TAV/%2.%3.sh").arg(ivis::common::APP_PWD()).arg(fileName.remove(".tav")).arg(pt);
        ivis::common::FileInfo::writeFile(path, writeContent, false);
    }


#else
    const QString groupKeyword = QString("    ##[Group]\n");
    QString ptValue = QString();
    QString groupValue = QString();
    QString periodValue = QString();
    QString periodGroupValue = QString();
    QString periodGroupInfo = QString();
    QString listenValue = QString();
    QStringList abstractionList = QStringList();

    for (int index = DetailInfoInvalid; index < DetailInfoMax; index++) {
        // qDebug() << "[" << index << "] =====================================================================================";
        if (mDetailInfo[index].size() == 0) {
            continue;
        }

        bool newLine = true;
        int periodCycle = 0;
        int periodDuration = 0;
        for (const auto& data : mDetailInfo[index].split("\n")) {
            bool appendSkip = true;
            QString removeSpaceValue = data;
            removeSpaceValue.replace("\t", "    ");  // Replace : Tab -> Space 4
            removeSpaceValue.remove("    ");    // Space : 4 (공백 한칸을 제외한 공백 제거)
            removeSpaceValue.remove("   ");     // Space : 3 (공백 한칸을 제외한 공백 제거)
            removeSpaceValue.remove("  ");      // Space : 2 (공백 한칸을 제외한 공백 제거)

            if ((data.size() == 0) || (data.trimmed().startsWith('#'))) {
                // 공백, 주석 제거
            } else if ((data.contains("[Description]")) || (data.contains("[Precondition]")) || (data.contains("[Step]")) ||
                       (data.contains("[Expected Result]")) || (data.contains("[Listen]")) || (data.contains("[PowerTrain]"))) {
                scriptInfo.append(QString("#%1\n").arg(removeSpaceValue));
            } else if ((data.contains("[Group]")) || (data.contains("[Period]"))) {
                scriptInfo.append(QString("##%1\n").arg(removeSpaceValue));
            } else {
                appendSkip = false;
            }

            if (appendSkip) {
                continue;
            }

            if (data.contains("#")) {
                removeSpaceValue.remove(removeSpaceValue.indexOf("#"), removeSpaceValue.size());
            }

            // Parsing -> Append
            QStringList tempList = QStringList();
            if ((index == DetailInfoDescription) || (index == DetailInfoExpectedResult)) {
                scriptInfo.append(QString("#    %1\n").arg(removeSpaceValue));
            } else if (index == DetailInfoPowerTrain) {
                scriptInfo.append(QString("#    %1\n").arg(removeSpaceValue));
                ptValue.append(removeSpaceValue);
            } else if (index == DetailInfoListen) {
                newLine = false;
                listenValue.append(QString(" %1").arg(removeSpaceValue));
            } else if ((index == DetailInfoPreconditionGroup) || (index == DetailInfoStepGroup) ||
                       (index == DetailInfoPeriodGroup)) {
                newLine = false;
                if (data.contains("    SFC.")) {
                    tempList = removeSpaceValue.split(" ");
                    if (tempList.size() == 2) {
                        scriptInfo.append(QString("%1 set %2 %3 %4\n")
                                              .arg(altonClient)
                                              .arg(tempList.at(0))
                                              .arg(isDataType(tempList.at(1)))
                                              .arg(tempList.at(1)));
                    } else {
                        scriptInfo.append(QString("%1 set %2\n").arg(altonClient).arg(removeSpaceValue));
                    }
                } else {
                    if (index == DetailInfoPeriodGroup) {
                        periodGroupValue.append(QString(" %1").arg(removeSpaceValue));
                    } else {
                        groupValue.append(QString(" %1").arg(removeSpaceValue));
                    }
                    if (data.contains("   Vehicle.")) {
                        abstractionList.append(removeSpaceValue);
                    }
                }
            } else if (index == DetailInfoPeriod) {
                if (data.contains("    SFC.")) {
                    tempList = removeSpaceValue.split(" ");
                    if (tempList.size() == 2) {
                        scriptInfo.append(QString("%1 set %2 %3 %4\n")
                                              .arg(altonClient)
                                              .arg(tempList.at(0))
                                              .arg(isDataType(tempList.at(1)))
                                              .arg(tempList.at(1)));
                    } else {
                        scriptInfo.append(QString("%1 set %2\n").arg(altonClient).arg(removeSpaceValue));
                    }
                } else if (data.contains("   Info :")) {
                    tempList = removeSpaceValue.remove("Info :").split("/");
                    if (tempList.size() == 2) {
                        periodCycle = tempList.at(0).toInt();
                        periodDuration = tempList.at(1).toInt();
                    } else {
                        qDebug() << "Fail to period info parsing - size :"<< tempList.size();
                    }
                } else {
                    newLine = false;
                    // periodValue.append(QString(" %1").arg(removeSpaceValue));
                    periodValue.append(QString("    %1 inject %2\n").arg(altonClient).arg(removeSpaceValue));
                    if (data.contains("   Vehicle.")) {
                        abstractionList.append(removeSpaceValue);
                    }
                }
            } else {
                tempList = removeSpaceValue.split(" ");
                if (data.contains("    SFC.")) {
                    if (tempList.size() == 2) {
                        scriptInfo.append(QString("%1 set %2 %3 %4\n")
                                              .arg(altonClient)
                                              .arg(tempList.at(0))
                                              .arg(isDataType(tempList.at(1)))
                                              .arg(tempList.at(1)));
                    } else {
                        scriptInfo.append(QString("%1 set %2\n").arg(altonClient).arg(removeSpaceValue));
                    }
                } else if (data.contains("    delay")) {
                    if (tempList.size() == 2) {
                        bool isDouble = false;
                        double value = tempList.at(1).toDouble(&isDouble);
                        scriptInfo.append(QString("sleep %1\n").arg(value * (double)0.001));
                    } else {
                        scriptInfo.append(QString("#### Check Error -> %1\n").arg(removeSpaceValue));
                    }
                } else {
                    if (data.contains("   Vehicle.")) {
                        abstractionList.append(removeSpaceValue);
                    }
                    scriptInfo.append(QString("%1 inject %2\n").arg(altonClient).arg(removeSpaceValue));
                }
            }
        }

        if (newLine) {
            scriptInfo.append("\n");
        }
        if (groupValue.size() > 0) {
            scriptInfo.append(QString("%1 inject%2\n\n").arg(altonClient).arg(groupValue));
            groupValue.clear();
        }
        if (periodValue.size() > 0) {
            if (periodGroupInfo.size() == 0) {
                scriptInfo.append(QString("for i in `seq 1 %1`\n").arg(periodCycle));
                scriptInfo.append(QString("do\n"));
                // scriptInfo.append(QString("    %1 inject%2\n").arg(altonClient).arg(periodValue));
                scriptInfo.append(QString("%1").arg(periodValue));
                scriptInfo.append(QString("%1").arg(groupKeyword));
                scriptInfo.append(QString("    sleep %1\n").arg(periodDuration * (double)0.001));
                scriptInfo.append(QString("done\n\n\n"));
                periodGroupInfo = groupKeyword;
            } else {
                if (periodGroupValue.size() > 0) {
                    scriptInfo.replace(groupKeyword,
                                       QString("%1    %2 inject%3\n").arg(groupKeyword).arg(altonClient).arg(periodGroupValue));
                    periodGroupValue.clear();
                } else {
                    scriptInfo.replace(groupKeyword, "");
                    periodGroupInfo.clear();
                }
                periodValue.clear();
            }
        }
        if (listenValue.size() > 0) {
            scriptInfo.append(QString("%1 listen%2 &\n\n").arg(altonClient).arg(listenValue));
            listenValue.clear();
        }
    }

    // Save : power train type
    const QStringList ptListInfo = QStringList({"ICV", "EV", "FCEV", "HEV", "PHEV"});
    QString ptTemp = (ptValue.size() == 0) ? (QString("ALL")) : (ptValue);
    QStringList ptlist = (ptTemp.toUpper() == QString("ALL")) ? (ptListInfo) : (ptTemp.remove(" ").split(","));
    QStringList pasingFileList = isVsmFileInfo(ptlist, abstractionList);
    QString fileName = file;
    for (const auto& pt : ptlist) {
        QString ptValid = QString();
        for (const auto& ptInfo : ptListInfo) {
            if (pt == ptInfo) {
                ptValid = pt;
                break;
            }
        }
        if (ptValid.size() == 0) {
            qDebug() << "Fail to validate power train :" << pt;
            continue;
        }

        QString path = QString("%1/../TAV/%2.%3.sh").arg(ivis::common::APP_PWD()).arg(fileName.remove(".tav")).arg(pt);
        QString writeContent = scriptInfo;

        // Replace : Abstraction -> CAN
        if (ptValue.size() == 0) {
            writeContent.replace(QString("[PowerTrain]\n"), QString("[PowerTrain]\n#    %1\n").arg(pt));
        } else {
            writeContent.replace(ptValue, pt);
        }

        for (const auto& replaceInfo : isReplaceSignal(abstractionList, pasingFileList)) {
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
        ivis::common::FileInfo::writeFile(path, writeContent, false);
    }

    if (ptValue.size() == 0) {
        scriptInfo.replace(QString("[PowerTrain]\n"), QString("[PowerTrain]\n#    ALL\n"));
    }
#endif

    return scriptInfo;
}

QStringList SubWindow::isVsmFileInfo(const QStringList& powerTrainList, const QStringList& signalList) {
    QStringList fileList = QStringList();
    QString vsmPath = QString("%1").arg("../../../620_SFC/model/VSM");
    QString fileNameBase = QString("CLU_VSM_%1.Vehicle.%2.vsm");

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
                fileList.append(QString("%1/%2").arg(vsmPath).arg(fileNameBase.arg(pt).arg(temp.at(1))));
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

QMap<int, QPair<QString, QString>> SubWindow::isMergeDataInfo(const QString& oldData, const QString& newData) {
    QMap<int, QPair<QString, QString>> mergeData = QMap<int, QPair<QString, QString>>();

    if (oldData == newData) {
        return mergeData;
    }

    QStringList oldDataList = oldData.split("\n");
    QStringList newDataList = newData.split("\n");
    int minLength = qMin(oldDataList.size(), newDataList.size());

    for (int i = 0; i < minLength; ++i) {
        if (oldDataList[i] != newDataList[i]) {
            mergeData[i] = qMakePair(oldDataList[i], newDataList[i]);
        }
    }

    if (newDataList.size() > oldDataList.size()) {
        for (int i = oldDataList.size(); i < newDataList.size(); ++i) {
            mergeData[i] = qMakePair(QString(), newDataList[i]);
        }
    }

    // for (auto iter = mergeData.begin(); iter != mergeData.end(); ++iter) {
    //     qDebug() << "MergeData[" << iter.key() << "] :" << iter.value().first << "->" << iter.value().second;
    // }

    return mergeData;
}
