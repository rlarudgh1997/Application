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

void SubWindow::mousePressEvent(QMouseEvent* mouseEvent) {
    Q_UNUSED(mouseEvent)
}

void SubWindow::mouseReleaseEvent(QMouseEvent* mouseEvent) {
    Q_UNUSED(mouseEvent)
}

void SubWindow::mouseMoveEvent(QMouseEvent* mouseEvent) {
    Q_UNUSED(mouseEvent)
}

void SubWindow::keyPressEvent(QKeyEvent* keyEvent) {
    Q_UNUSED(keyEvent)
}

void SubWindow::keyReleaseEvent(QKeyEvent* keyEvent) {
    Q_UNUSED(keyEvent)
}

void SubWindow::moveEvent(QMoveEvent* moveEvent) {
    Q_UNUSED(moveEvent)
}

void SubWindow::resizeEvent(QResizeEvent* resizeEvent) {
    Q_UNUSED(resizeEvent)
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
        }
    });
    connect(mGui->detailBack, &QPushButton::clicked, [=]() {
        mSelectFile.clear();
        drawDisplay(DisplayTypeListMain);
    });
    connect(mGui->detailStart, &QPushButton::clicked, [=]() {
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
    });
    connect(mGui->altonServiceClear, &QPushButton::clicked, [=]() { mGui->altonServiceContent->hide(); });
    connect(mGui->hmiClear, &QPushButton::clicked, [=]() { mGui->hmiContent->hide(); });
}

void SubWindow::drawDisplay(const int& type, const QString& text) {
    switch (type) {
        case DisplayTypeListMain: {
            mGui->menubar->show();
            mGui->statusbar->hide();
            mGui->altonClient->setCurrentIndex(1);
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
        QString file = info;
        QString path = QString("%1/../TAV/%2").arg(ivis::common::APP_PWD()).arg(file);
        QStringList fileData = ivis::common::FileInfo::readFile(path);
        QString content = QString();
        int foundType = DetailInfoInvalid;

        mDetailInfo.clear();
        for (const auto& data : fileData) {
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
            removeSpaceValue.remove("    ");    // Space : 4

            if (data.size() == 0) {
                // scriptInfo.append("\n");
            } else if ((data.contains("[Description]")) || (data.contains("[Precondition]")) || (data.contains("[Step]"))
                || (data.contains("[Expected Result]")) || (data.contains("[Listen]")) || (data.contains("[PowerTrain]"))) {
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
            } else if (index == DetailInfoListen) {
                newLine = false;
                listenValue.append(QString(" %1").arg(removeSpaceValue));
            } else {
                QStringList tempList = removeSpaceValue.split(" ");
                if (data.contains("    SFC.")) {
                    if (tempList.size() == 2) {
                        scriptInfo.append(QString("./alton_client set %1 %2 %3\n").arg(tempList.at(0))
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
    QString ptTemp = ptValue;
    QString fileName = file;
    QStringList ptlist = ((ptTemp.toUpper() == QString("ALL")) || (ptTemp.size() == 0)) ?
                          (QStringList({"ICV", "EV", "FCEV", "HEV", "PHEV"})) : (ptTemp.remove(" ").split(","));
    QStringList pasingFileList = isVsmFileInfo(ptlist, abstractionList);

    for (const auto& pt : ptlist) {
        QString path = QString("%1/../TAV/%2.%3.sh").arg(ivis::common::APP_PWD()).arg(fileName.remove(".tav")).arg(pt);
        QString writeContent = scriptInfo;

        // Replace : Abstraction -> CAN
        for (const auto& replaceInfo : isCanSignal(abstractionList, pasingFileList)) {
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

QList<QPair<QString, QString>> SubWindow::isCanSignal(const QStringList& abstractionList, const QStringList& vsmFileList) {
    const QString PREFIX_TYPE = QString("type:");
    const QString PREFIX_SIGNAL_NAME = QString("signalName:");
    const QString PREFIX_DATA_TYPE = QString("dataType:");
    const QString PREFIX_DESCRIPTION = QString("description:");
    const QString PREFIX_ABSTRACTION_NAME = QString("abstractionName:");
    const QString PREFIX_VALUE_ENUM = QString("valueEnum:");
    const QString PREFIX_MATCHING_TABLE = QString("matchingTable:");
    const QString PREFIX_CODE_COMMENTING = QString("# ");
    const QString PREFIX_HYPHEN = QString("-");
    const QString PREFIX_DOT = QString(".");
    const QString PREFIX_COLON = QString(":");
    const QString PREFIX_SPACE = QString(" ");

    QList<QPair<QString, QString>> canSignalList = QList<QPair<QString, QString>>();

    qDebug() << "======================================================================================================";
    QStringList signalList = QStringList();
    for (const auto& abstraction : abstractionList) {
        QStringList temp = abstraction.split(" ");
        if (temp.size() < 2) {
            continue;
        }

        QString value = temp[1];
        temp = temp[0].split(".");
        if (temp.size() < 3) {
            continue;
        }
        QString signal = QString();
        if (abstraction.contains("Vehicle.System.")) {
            signal = temp.at(3);
        } else {
            signal = QString("%1.%2").arg(temp.at(2)).arg(temp.at(3));
        }
        signalList.append(signal);

        QString replaceData = QString("%1 %2").arg(signal).arg(value);
        qDebug() << "\t Abstraction :" << abstraction;
        qDebug() << "\t            ->" << replaceData;
        // canSignalList.append(QPair<QString, QString>(abstraction, replaceData));

        bool foundCanSignal = false;
        qDebug() << "======================================================================================================";
        for (const auto& file : vsmFileList) {
            qDebug() << "\t File        :" << file;
            QStringList readData = ivis::common::FileInfo::readFile(file);
            QStringList vsmInfo = QStringList();
            bool foundSignal = false;
            for (QString lineStr : readData) {
                if (foundSignal) {
                    if ((lineStr.contains(PREFIX_SIGNAL_NAME))) {
                        lineStr.remove(" ");
                        lineStr.remove(PREFIX_SIGNAL_NAME);
                        qDebug() << "]t Found CAN :" << lineStr;
                        foundCanSignal = true;
                        break;
                    // } else if ((lineStr.contains(PREFIX_HYPHEN)) && (lineStr.contains(PREFIX_COLON))) {
                    //     break;
                    } else {
                    }
                    continue;
                }

                if ((lineStr.contains(signal)) && (lineStr.contains(PREFIX_HYPHEN)) && (lineStr.contains(PREFIX_COLON))) {
                    foundSignal = true;
                }
            }

            if (foundCanSignal) {
                break;
            }
        }

    }

    return canSignalList;
}



QMap<int, QStringList> SubWindow::isCanSignal(const bool& sfcSignal, const QString& signalName,
                                                              const QMap<int, QStringList>& vsmFileList) {
    const QString PREFIX_TYPE = QString("type:");
    const QString PREFIX_SIGNAL_NAME = QString("signalName:");
    const QString PREFIX_DATA_TYPE = QString("dataType:");
    const QString PREFIX_DESCRIPTION = QString("description:");
    const QString PREFIX_ABSTRACTION_NAME = QString("abstractionName:");
    const QString PREFIX_VALUE_ENUM = QString("valueEnum:");
    const QString PREFIX_MATCHING_TABLE = QString("matchingTable:");
    const QString PREFIX_CODE_COMMENTING = QString("# ");
    const QString PREFIX_HYPHEN = QString("-");
    const QString PREFIX_DOT = QString(".");
    const QString PREFIX_COLON = QString(":");
    const QString PREFIX_SPACE = QString(" ");
    // const QString PREFIX_MCAN = QString("_MCAN");
    // const QString PREFIX_CCAN = QString("_CCAN");

    QMap<int, QStringList> inputDataInfo = QMap<int, QStringList>();
#if 1

#else
    QString signal = QString();
    bool vehiclSystem = signalName.contains("Vehicle.System.");
    int startAppendIndex = ((vehiclSystem) ? (3) : (2));
    int index = 0;
    for (const auto& splitText : signalName.split(PREFIX_DOT)) {
        if (index >= startAppendIndex) {
            signal.append((signal.size() > 0) ? (".") : (""));
            signal.append(splitText);
        }
        index++;
    }
    if (signal.size() == 0) {
        qDebug() << "Fail to signal name size : 0";
        return inputDataInfo;
    }

    qDebug() << "Input  Signal      :" << signalName;
    qDebug() << "Parser Signal      :" << signal;
    QMapIterator<int, QStringList> iter(vsmFileList);
    while (iter.hasNext()) {
        iter.next();
        int inputDataType = iter.key();
        QStringList fileName = iter.value();
        // qDebug() << inputDataType << ". Size :" << fileName.size() << ", File :" << fileName;
        QStringList valueEunm = QStringList();
        QStringList matchingTable = QStringList();
        for (const auto& file : fileName) {
            QStringList readData = ivis::common::FileInfo::readFile(file);
            QStringList vsmInfo = QStringList();
            bool foundSignal = false;
            for (QString lineStr : readData) {
                if (foundSignal) {
                    if ((lineStr.contains(PREFIX_HYPHEN)) && (lineStr.contains(PREFIX_COLON))) {
                        qDebug() << "\t Next  Signal[" << inputDataType << "] :" << lineStr;
                        break;
                    } else if ((lineStr.size() == 0) || (lineStr.contains(PREFIX_TYPE)) || (lineStr.contains(PREFIX_DATA_TYPE)) ||
                               (lineStr.contains(PREFIX_SIGNAL_NAME)) || (lineStr.contains(PREFIX_ABSTRACTION_NAME)) ||
                               (lineStr.contains(PREFIX_DESCRIPTION)) || (lineStr.contains(PREFIX_CODE_COMMENTING))) {
                        // Skip : type, signalName, dataType, description, abstractionName, #(주석)
                        continue;
                    } else {
                        // Append : ValueEnum, MatchingTable
                        lineStr.remove(PREFIX_SPACE);
                        vsmInfo.append(lineStr);
                    }
                } else {
                    if ((lineStr.contains(signal)) && (lineStr.contains(PREFIX_HYPHEN)) && (lineStr.contains(PREFIX_COLON))) {
                        // Input Signal : (Vehicle.Speed_Gauge.Output_DisplaySpeedUnit)
                        // Read  Signal : (- Speed_Gauge.Output_DisplaySpeedUnit:)
                        lineStr.remove(PREFIX_HYPHEN);
                        lineStr.remove(PREFIX_COLON);
                        lineStr.remove(PREFIX_SPACE);
                        foundSignal = ((sfcSignal) ? (true) : (lineStr.compare(signal) == false));
                        qDebug() << ((foundSignal) ? ("\t Found") : ("\t Skip ")) << "Signal[" << inputDataType
                                 << "] :" << lineStr;
                    }
                }
            }

            bool foundValueEnum = false;
            bool foundMatchingTable = false;
            for (const auto& info : vsmInfo) {
                if (foundMatchingTable) {
                    foundValueEnum = false;
                    matchingTable.append(info);
                    // qDebug() << "\t MatchingTable :" << info;
                } else {
                    foundMatchingTable = (info.contains(PREFIX_MATCHING_TABLE));
                }

                if ((vehiclSystem) || (inputDataType == ivis::common::InputDataTypeEnum::InputDataTypeValueEnum)) {
                    if (foundValueEnum) {
                        if (info.contains(PREFIX_MATCHING_TABLE)) {
                            continue;
                        }
                        valueEunm.append(info);
                        // qDebug() << "\t ValueEnum     :" << info;
                    } else {
                        foundValueEnum = info.contains(PREFIX_VALUE_ENUM);
                    }
                }
            }
        }

        if (inputDataType == ivis::common::InputDataTypeEnum::InputDataTypeValueEnum) {
            // qDebug() << "\t Value    Size :" << valueEunm.size();
            inputDataInfo[inputDataType] = valueEunm;
        } else {
            // qDebug() << "\t Matching Size :" << matchingTable.size();
            inputDataInfo[inputDataType] = matchingTable;
            if ((valueEunm.size() > 0) && (vehiclSystem)) {
                // qDebug() << "\t Value    Size :" << valueEunm.size();
                inputDataInfo[ivis::common::InputDataTypeEnum::InputDataTypeValueEnum] = valueEunm;
            }
        }
    }
#endif

    return inputDataInfo;
}
