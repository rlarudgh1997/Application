#include "SubWindow.h"
#include "ui_SubWindow.h"
#include "CommonEnum.h"
#include "CommonUtil.h"
#include "Service.h"

#include <QDebug>
#include <QMapIterator>

SubWindow::SubWindow(QWidget* parent) : QMainWindow(parent), mGui(new Ui::SubWindow) {
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
        if (info.size() > 0) {
            QString file = info;
            QString path = QString("%1/../TAV/%2").arg(ivis::common::APP_PWD()).arg(file);
            mOriginalData = ivis::common::FileInfo::readFile(path);
        } else {
            // mOriginalData = ivis::common::FileInfo::readFile(path);
        }

        QString content = QString();
        int foundType = DetailInfoInvalid;

        mDetailInfo.clear();
        for (const auto& data : mOriginalData) {
            if (data.contains("#")) {
                // ex) data = #Info : test 1235                    => 주석 처리 기능 완료
                // ex) data = Info : test 1235  # test info define => 중간 주석 처리 기능 구현 필요
                continue;
            }
            if (data.size() == 0) {
                foundType = DetailInfoInvalid;
            }

            if (data.contains("[Description]")) {
                foundType = DetailInfoDescription;
            } else if (data.contains("[PowerTrain]")) {
                foundType = DetailInfoPowerTrain;
            } else if (data.contains("[Precondition]")) {
                foundType = DetailInfoPrecondition;
            } else if (data.contains("[Step]")) {
                foundType = DetailInfoStep;
            } else if (data.contains("[Group]")) {
                foundType = (foundType == DetailInfoPrecondition) ? (DetailInfoPreconditionGroup) : (DetailInfoStepGroup);
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
                            mDetailInfo[DetailInfoListen].append(QString("%1\n").arg(tempList.at(0)));
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

        for (const auto& detailInfo : mDetailInfo) {
            qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
            int count = 0;
            for (QString info : detailInfo.split("\n")) {
                qDebug() << count++ << ":" << info;
            }
            qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
        }

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
    QString oldDetailContent = mPreviousTavData;
    QString newDetailContent = mGui->detailContent->toPlainText();
    QMap<int, QPair<QString, QString>> mergeDataInfo = isMergeDataInfo(oldDetailContent, newDetailContent);

    for (int index = DetailInfoInvalid; index < DetailInfoMax; index++) {
        if ((mDetailInfo[index].size() == 0) || (index == DetailInfoListen)) {
            continue;
        }

        bool changed = false;
        QStringList detailList = mDetailInfo[index].split("\n");
        for (int lineIndex = 0; lineIndex < detailList.size(); lineIndex++) {
            for (auto iter = mergeDataInfo.begin(); iter != mergeDataInfo.end(); ++iter) {
                if (detailList[lineIndex] == iter.value().first) {
                    changed = true;
                    detailList[lineIndex] = iter.value().second;
                    qDebug() << "Changed Data[" << iter.key() << "] :" << iter.value().first << "->" << iter.value().second;
                    break;
                }
            }
        }
        if (changed) {
            QString data = QString();
            for (const auto& detail : detailList) {
                data.append(detail);
                data.append("\n");
            }
            mDetailInfo[index] = data;
        }
    }
    mPreviousTavData = newDetailContent;
    writeOriginalData(filePath, mergeDataInfo);
}

void SubWindow::writeOriginalData(const QString& filePath, const QMap<int, QPair<QString, QString>>& mergeDataInfo) {
    if ((filePath.size() == 0) || (mergeDataInfo.size() == 0)) {
        return;
    }

    QStringList tempData = mOriginalData;
    for (int index = 0; index < tempData.size(); index++) {
        if ((tempData[index].size() == 0) || (index == DetailInfoListen)) {
            continue;
        }

        for (auto iter = mergeDataInfo.begin(); iter != mergeDataInfo.end(); ++iter) {
            if (tempData[index] == iter.value().first) {
                tempData[index] = iter.value().second;
                qDebug() << "Saved   Data[" << iter.key() << "] :" << iter.value().first << "->" << iter.value().second;
            }
        }
    }

    if (mOriginalData != tempData) {
        QString writeContent = QString();
        QString path = QString("%1/../TAV/%2").arg(ivis::common::APP_PWD()).arg(filePath);
        for (const auto& data : tempData) {
            writeContent.append(data);
            writeContent.append("\n");
        }
        qDebug() << "Save Path :" << path;
        ivis::common::FileInfo::writeFile(path, writeContent, false);
        mOriginalData = tempData;
    }
}

QString SubWindow::createToScript(const QString& file) {
    QString scriptInfo("#!/bin/bash\n\n");
    QString ptValue = QString();
    QString groupValue = QString();
    QString listenValue = QString();
    QStringList abstractionList = QStringList();

    for (int index = DetailInfoInvalid; index < DetailInfoMax; index++) {
        // qDebug() << "[" << index << "] =====================================================================================";
        if (mDetailInfo[index].size() == 0) {
            continue;
        }

        bool newLine = true;
        for (const auto& data : mDetailInfo[index].split("\n")) {
            bool appendSkip = true;
            QString removeSpaceValue = data;

            removeSpaceValue.remove("    ");  // Space : 4 (공백 한칸을 제외한 공백 제거)
            removeSpaceValue.remove("   ");   // Space : 3 (공백 한칸을 제외한 공백 제거)
            removeSpaceValue.remove("  ");    // Space : 2 (공백 한칸을 제외한 공백 제거)

            if (data.size() == 0) {
                // scriptInfo.append("\n");
            } else if ((data.contains("[Description]")) || (data.contains("[Precondition]")) || (data.contains("[Step]")) ||
                       (data.contains("[Expected Result]")) || (data.contains("[Listen]")) || (data.contains("[PowerTrain]"))) {
                scriptInfo.append(QString("#%1\n").arg(removeSpaceValue));
            } else if (data.contains("[Group]")) {
                scriptInfo.append(QString("##%1\n").arg(removeSpaceValue));
            } else {
                appendSkip = false;
            }

            if (appendSkip) {
                continue;
            }

            // Parsing -> Append
            if ((index == DetailInfoDescription) || (index == DetailInfoExpectedResult)) {
                scriptInfo.append(QString("#    %1\n").arg(removeSpaceValue));
            } else if (index == DetailInfoPowerTrain) {
                scriptInfo.append(QString("#    %1\n").arg(removeSpaceValue));
                ptValue.append(removeSpaceValue);
            } else if ((index == DetailInfoPreconditionGroup) || (index == DetailInfoStepGroup)) {
                newLine = false;
                groupValue.append(QString(" %1").arg(removeSpaceValue));
                if (data.contains("   Vehicle.")) {
                    abstractionList.append(removeSpaceValue);
                }
            } else if (index == DetailInfoListen) {
                newLine = false;
                listenValue.append(QString(" %1").arg(removeSpaceValue));
            } else {
                QStringList tempList = removeSpaceValue.split(" ");
                if (data.contains("    SFC.")) {
                    if (tempList.size() == 2) {
                        scriptInfo.append(QString("./alton_client set %1 %2 %3\n")
                                              .arg(tempList.at(0))
                                              .arg(isDataType(tempList.at(1)))
                                              .arg(tempList.at(1)));
                    } else {
                        scriptInfo.append(QString("./alton_client set %1\n").arg(removeSpaceValue));
                    }
                } else if (data.contains("   delay")) {
                    if (tempList.size() == 2) {
                        bool isDouble = false;
                        double value = tempList.at(1).toDouble(&isDouble);
                        scriptInfo.append(QString("sleep %1\n").arg(value * (double)1000));
                    } else {
                        scriptInfo.append(QString("#### Check Error -> %1\n").arg(removeSpaceValue));
                    }
                } else {
                    if (data.contains("   Vehicle.")) {
                        abstractionList.append(removeSpaceValue);
                    }
                    scriptInfo.append(QString("./alton_client inject %1\n").arg(removeSpaceValue));
                }
            }
        }

        if (newLine) {
            scriptInfo.append("\n");
        }

        if (groupValue.size() > 0) {
            scriptInfo.append(QString("./alton_client inject%1\n\n").arg(groupValue));
            groupValue.clear();
        }
        if (listenValue.size() > 0) {
            scriptInfo.append(QString("./alton_client listen%1\n\n").arg(listenValue));
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
        writeContent.replace(ptValue, pt);
        for (const auto& replaceInfo : isReplaceSignal(abstractionList, pasingFileList)) {
            writeContent.replace(replaceInfo.first, replaceInfo.second);
        }

        ivis::common::FileInfo::writeFile(path, writeContent, false);
    }

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
