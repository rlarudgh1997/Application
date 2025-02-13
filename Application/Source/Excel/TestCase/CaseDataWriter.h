#ifndef CASE_DATA_WRITER_H
#define CASE_DATA_WRITER_H

#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QStorageInfo>

#include "ExcelUtil.h"
#include "JSEngineManager.h"

class CaseDataWriter {
public:
    CaseDataWriter(const QString& basePath = QString()) : mTCFileDirPath(basePath) {
        mSfcDescription = ExcelUtil::instance().data()->isDescriptionDataInfo();
        if (mTCFileDirPath.size() == 0) {
            mOpenFilePath = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeTCFilePath).toString();
            int lastSlashIndex = mOpenFilePath.lastIndexOf("/");
            mTCFileDirPath = mOpenFilePath.mid(0, lastSlashIndex);
            mAvailableSpace = QStorageInfo(mTCFileDirPath).bytesAvailable() * 9 / 10;
        }
        qDebug() << "CaseDataWriter::CaseDataWriter() complete.";
    }

    ~CaseDataWriter() {
        if (mStream.device()) {
            mStream << "  - name: timeout for gcov\n";
            mStream << "    input:\n";
            mStream << "      - Vehicle.CV.EHS.Input_EHSStatus: timeout\n\n";
            mStream << "  - name: dump for gcov\n";
            mStream << "    input:\n";
            mStream << "      - dump: true\n";
        }
    }

    bool genTestCaseFile(const QJsonObject& json, const int& totalTestCaseCount) {
        ivis::common::CheckTimer checkTimer;
        int ignCount = 0;
        int testCaseCount = 1;
        auto quoteIfNotNumeric = [](const QString& value) {
            return (!value.isEmpty() && !value[0].isDigit() && !value.contains("timeout") && !value.contains("crc"))
                       ? "\"" + value + "\""
                       : value;
        };
        for (const QString& sheet : json.keys()) {
            QJsonObject sectionObj = json[sheet].toObject();
            for (auto tcIt = sectionObj.begin(); tcIt != sectionObj.end(); ++tcIt) {
                QJsonObject tcObj = tcIt.value().toObject();
                QString tcName = tcObj["TCName"].toString();
                QString vehicleType = tcObj["VehicleType"].toString();
                if (!tcName.isEmpty()) {
                    QString othersResultName;
                    QStringList othersInputSignalList;
                    QVector<std::tuple<QString, int, int, QJsonArray, QJsonObject, QJsonArray>> othersSignalListInfo;
                    QStringList othersOutputSignal;
                    QStringList othersOutputValue;
                    QVector<bool> othersIsInitialize;
                    for (const QString& key : tcObj.keys()) {
                        if (key.startsWith("_Result[")) {
                            QJsonObject resultObj = tcObj[key].toObject();
                            QString resultName = resultObj["Result"].toString();
                            QStringList outputSignal;
                            QStringList outputValue;
                            QStringList outputEnum;
                            QVector<bool> isInitialize;
                            QJsonObject outputSignalList = resultObj["Output_Signal"].toObject();
                            QJsonObject signalObj;
                            for (auto sigIt = outputSignalList.begin(); sigIt != outputSignalList.end(); ++sigIt) {
                                signalObj = sigIt.value().toObject();
                                outputSignal << sigIt.key();
                                outputValue << signalObj["Output_Value"].toString();
                                outputEnum.clear();
                                isInitialize << signalObj["isInitialize"].toBool();
                                for (int outputCnt = 0; outputCnt < outputSignal.size(); outputCnt++) {
                                    QString tempOutputEnum;
                                    tempOutputEnum = SignalDataManager::instance().data()->isSignalValueEnum(
                                        outputSignal[outputCnt], outputValue[outputCnt]);
                                    if (tempOutputEnum.isEmpty() == false) {
                                        outputEnum << " # " + tempOutputEnum;
                                    } else {
                                        outputEnum << tempOutputEnum;
                                    }
                                }
                            }

                            for (const QString& caseKey : resultObj.keys()) {
                                if (caseKey.startsWith("_Case[")) {
                                    QVector<std::tuple<QString, int, int, QJsonArray, QJsonObject, QJsonArray>> signalList;
                                    QJsonObject caseObj = resultObj[caseKey].toObject();
                                    QString caseName = caseObj["Case"].toString();
                                    QString genType = caseObj["GenType"].toString();
                                    QStringList inputSignalList =
                                        resultObj[caseKey][QString("InputSignalList")].toObject().keys();
                                    QVector<int> preconditionOrder;
                                    for (const QJsonValue& value : resultObj[caseKey][QString("PreconditionOrder")].toArray()) {
                                        if (value.isDouble()) {
                                            preconditionOrder.append(value.toInt());
                                        }
                                    }
                                    for (const QString& inputSignal : inputSignalList) {
                                        signalList.emplace_back(std::make_tuple(
                                            resultObj[caseKey][QString("InputSignalList")][inputSignal][QString("SignalName")]
                                                .toString(),
                                            resultObj[caseKey][QString("InputSignalList")][inputSignal][QString("DataType")]
                                                .toInt(),
                                            resultObj[caseKey][QString("InputSignalList")][inputSignal][QString("KeywordType")]
                                                .toInt(),
                                            resultObj[caseKey][QString("InputSignalList")][inputSignal][QString("Precondition")]
                                                .toArray(),
                                            resultObj[caseKey][QString("InputSignalList")][inputSignal][QString("ValueEnum")]
                                                .toObject(),
                                            resultObj[caseKey][QString("InputSignalList")][inputSignal][QString("InputData")]
                                                .toArray()));
                                    }
                                    QJsonObject casesMap = caseObj["cases"].toObject();
                                    QJsonObject initMap = caseObj["InitCase"].toObject();
                                    QStringList parts;
                                    QStringList inputDataInfo;
                                    QStringList signalValueList;
                                    int signalOrder = 0;
                                    QString signalName;
                                    QString signalValue;
                                    QString enumString;
                                    QString triggerEnumString;
                                    QString triggerSignal;
                                    QString triggerValue;
                                    QString testCase;
                                    for (auto it = casesMap.begin(); it != casesMap.end(); ++it) {
                                        testCase.clear();
                                        parts = it.key().split("=>");
                                        signalValueList = parts[0].split(",", Qt::SkipEmptyParts);
                                        inputDataInfo = parts[1].split(":");
                                        signalOrder = 0;
                                        testCase += "  - name: " + tcName + ", " + resultName + ", " + caseName + " " +
                                                    QString::number(testCaseCount++) + "\n";
                                        if (vehicleType.isEmpty() == false) {
                                            testCase += "    tag: " + vehicleType + "\n";
                                        }
                                        if (signalValueList[0].contains("config") == true) {
                                            signalValueList[0].remove("[");
                                            signalValueList[0].remove("]");
                                            testCase += "    " + signalValueList[0] + "\n";
                                            signalValueList.removeAt(0);
                                        }
                                        if (genType.contains("Negative", Qt::CaseInsensitive) == true) {
                                            testCase += "    state: negative\n";
                                        } else if (genType.contains("Positive", Qt::CaseInsensitive) == true) {
                                            testCase += "    state: positive\n";
                                        }
                                        testCase += "    precondition:\n";
                                        QMap<QString, QString> sigValueMap;
                                        QVector<QString> preconditionString;
                                        for (QString& number : signalValueList) {
                                            number = number.trimmed();
                                            if (number.isEmpty() == false) {
                                                enumString = std::get<4>(signalList[signalOrder])[number].toString();
                                                if (inputDataInfo[0] == std::get<0>(signalList[signalOrder])) {
                                                    triggerEnumString =
                                                        std::get<4>(signalList[signalOrder])[inputDataInfo[1].trimmed()]
                                                            .toString();
                                                    sigValueMap.insert(inputDataInfo[0], inputDataInfo[1].trimmed());
                                                } else {
                                                    sigValueMap.insert(std::get<0>(signalList[signalOrder]), number);
                                                }
                                                if (std::get<0>(signalList[signalOrder]).contains("SFC.Private.IGNElapsed")) {
                                                    signalName = ExcelUtil::instance().data()->isIGNElapsedName(number.toInt());
                                                    signalValue = "0x" + QString::number(++ignCount, 16).toUpper();
                                                } else {
                                                    signalName = std::get<0>(signalList[signalOrder]);
                                                    signalValue = number;
                                                }

                                                enumString = enumString.isEmpty() ? QString() : QString(" # %1").arg(enumString);
                                                preconditionString << "      - " + signalName + ": " +
                                                                          quoteIfNotNumeric(signalValue) + enumString + "\n";
                                                signalOrder++;
                                            }
                                        }

                                        if (preconditionString.size() != preconditionOrder.size()) {
                                            qWarning() << "The number of signals in the precondition and the number of "
                                                          "precondition orders are different.\nA review of the generated "
                                                          "TcGenHistory.json file is required.";
                                        } else {
                                            for (int order = 0; order < preconditionOrder.size(); order++) {
                                                if (preconditionString[preconditionOrder[order]].contains("[Empty]") == false) {
                                                    testCase += preconditionString[preconditionOrder[order]];
                                                }
                                            }
                                        }

                                        if (isInitialize.contains(true)) {
                                            testCase += "    init:\n";
                                            for (int initCnt = 0; initCnt < isInitialize.size(); initCnt++) {
                                                if (isInitialize[initCnt] == true) {
                                                    if (outputValue[initCnt][0].isDigit() == true) {
                                                        if (outputValue[initCnt].contains("0x")) {
                                                            testCase += "      - " + outputSignal[initCnt] + ": 0x0 # NONE\n";
                                                        } else {
                                                            testCase += "      - " + outputSignal[initCnt] + ": 0\n";
                                                        }
                                                    } else {
                                                        if (outputSignal[initCnt].contains("E")) {
                                                            testCase += "      - " + outputSignal[initCnt] + ": " +
                                                                        quoteIfNotNumeric(outputValue[initCnt]) + "\n";
                                                        } else if (outputSignal[initCnt].contains("SND")) {
                                                            testCase += "      - " + outputSignal[initCnt] + ": " +
                                                                        quoteIfNotNumeric(outputValue[initCnt]) + "\n";
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        if (parts[1].split(":").size() == 2) {
                                            testCase += "    input:\n";
                                            if (inputDataInfo[0].contains("SFC.Private.IGNElapsed")) {
                                                triggerSignal =
                                                    ExcelUtil::instance().data()->isIGNElapsedName(inputDataInfo[1].toInt());
                                                triggerValue = "0x" + QString::number(++ignCount, 16).toUpper();
                                            } else {
                                                triggerSignal = inputDataInfo[0];
                                                triggerValue = inputDataInfo[1];
                                            }
                                            triggerEnumString =
                                                triggerEnumString.isEmpty() ? QString() : QString(" # %1").arg(triggerEnumString);
                                            testCase += "      - " + triggerSignal + ": " +
                                                        quoteIfNotNumeric(triggerValue.trimmed()) + triggerEnumString + "\n";
                                        }
                                        testCase += "    output:\n";
                                        for (int outputCnt = 0; outputCnt < outputSignal.size(); outputCnt++) {
                                            QString tempOutputSignal = outputSignal[outputCnt];
                                            if (tempOutputSignal.contains("Collect")) {
                                                tempOutputSignal = "collect";
                                            }
                                            // Cal logic 구현 (단, Sheet 는 먼저 풀려있어야 함)
                                            QString tempOutputValue = outputValue[outputCnt];
                                            if (tempOutputValue.contains("[Cal]")) {
                                                if (tempOutputValue.contains("math.")) {
                                                    tempOutputValue.replace("math.", "Math.");
                                                }
                                                tempOutputValue.remove("[Cal]");
                                                for (const auto& sigNameKey : sigValueMap.keys()) {
                                                    if (tempOutputValue.contains(sigNameKey)) {
                                                        tempOutputValue.replace(sigNameKey, sigValueMap[sigNameKey]);
                                                    }
                                                }
                                                tempOutputValue =
                                                    JSEngineManager::instance().getEngine().evaluate(tempOutputValue).toString();
                                            }
                                            (outputCnt == outputSignal.size() - 1)
                                                ? testCase += "      - " + tempOutputSignal + ": " +
                                                              quoteIfNotNumeric(tempOutputValue) + outputEnum[outputCnt] + "\n\n"
                                                : testCase += "      - " + tempOutputSignal + ": " +
                                                              quoteIfNotNumeric(tempOutputValue) + outputEnum[outputCnt] + "\n";
                                        }
                                        write(testCase);
                                    }

                                    if (!initMap.isEmpty()) {
                                        QStringList initParts;
                                        QString initEnumString;
                                        QString initSignalName;
                                        QString initSignalValue;
                                        QString initCase;
                                        int initSignalOrder = 0;
                                        initCase.clear();
                                        initCase += "  - name: " + tcName + ", " + resultName + ", " + caseName +
                                                    " Input Signal Initialization." + "\n";
                                        if (vehicleType.isEmpty() == false) {
                                            initCase += "    tag: " + vehicleType + "\n";
                                        }
                                        initCase += "    input:\n";
                                        for (auto it = initMap.begin(); it != initMap.end(); ++it) {
                                            initParts = it.key().split("=>");
                                            for (QString& number : initParts[0].split(",", Qt::SkipEmptyParts)) {
                                                number = number.trimmed();
                                                if (number.isEmpty() == false) {
                                                    if (number.contains("[Empty]") == false) {
                                                        initEnumString =
                                                            std::get<4>(signalList[initSignalOrder])[number].toString();
                                                        if (std::get<0>(signalList[initSignalOrder])
                                                                .contains("SFC.Private.IGNElapsed")) {
                                                            initSignalName =
                                                                ExcelUtil::instance().data()->isIGNElapsedName(number.toInt());
                                                            initSignalValue = "0x" + QString::number(++ignCount, 16).toUpper();
                                                        } else {
                                                            initSignalName = std::get<0>(signalList[initSignalOrder]);
                                                            initSignalValue = number;
                                                        }
                                                        initEnumString = initEnumString.isEmpty()
                                                                             ? QString()
                                                                             : QString(" # %1").arg(initEnumString);
                                                        initCase += "      - " + initSignalName + ": " +
                                                                    quoteIfNotNumeric(initSignalValue) + initEnumString + "\n";
                                                    }
                                                    initSignalOrder++;
                                                }
                                            }
                                        }
                                        initCase += "\n";
                                        write(initCase);
                                    }
                                    printProgressBar(static_cast<double>(testCaseCount) /
                                                     static_cast<double>(totalTestCaseCount));
                                }
                            }
                        }
                    }
                }
            }
        }
        std::cout << std::endl;
        std::cout << "\033[92mTest case file generation completed!!!\033[0m" << std::endl;
        checkTimer.check("genTestCaseFile()");

        return (testCaseCount > 1);
    }

private:
    void printProgressBar(double progress) {
        int barWidth = 50;

        std::cout << "\033[92mGenerating test cases files... [";
        int pos = barWidth * progress;
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos)
                std::cout << "=";
            else if (i == pos)
                std::cout << ">";
            else
                std::cout << " ";
        }
        std::cout << "] " << std::fixed << std::setprecision(1) << std::min(100.0, progress * 100.0) << " %\r\033[0m";
        std::cout.flush();
    }

    void write(const QString& testCase = "") {
        if (!mFile.isOpen() || mPrevSplitTcFileCount != mSplitTcFileCount) {
            openNewFile(mSplitTcFileCount);
            mPrevSplitTcFileCount = mSplitTcFileCount;
        }
        if (mFile.size() > mMaxFileSize) {
            mFileSize += mFile.size();
            if (mFileSize > mAvailableSpace) {
                qDebug() << "TC file size exceeds the available capacity of the current system.";
                exit(-1);
            } else {
                mSplitTcFileCount++;
            }
        }
        if (mStream.device() && testCase.isEmpty() == false) {
            mStream << testCase;
        }
    }

    QString getUserName() {
        QProcess process;
        process.start("whoami");
        process.waitForFinished();
        QString userName = process.readAll().trimmed();
        return userName;
    }

    void writeHeader() {
        QDateTime currentTime = QDateTime::currentDateTime();

        if (mStream.device()) {
            // NOTE: 기존 파일이 있으면 생성 년도를 유지할 수 있는 방법. ivis::common::APP_PWD()
            mStream << QString("# Copyright (c) %1 Hyundai Motor Company,\n").arg(currentTime.toString("yyyy"));
            mStream << "# All Rights Reserved.\n";
            mStream << "#\n";
            mStream << "# Use and copying of this software and preparation of derivative works\n";
            mStream << "# based upon this software are permitted. Any copy of this software or\n";
            mStream << "# of any derivative work must include the above copyright notice, this\n";
            mStream << "# paragraph and the one after it. Any distribution of this software or\n";
            mStream << "# derivative works must comply with all applicable laws.\n";
            mStream << "#\n";
            mStream << "# This software is made available AS IS, and COPYRIGHT OWNERS DISCLAIMS\n";
            mStream << "# ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE\n";
            mStream << "# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR\n";
            mStream << "# PURPOSE, AND NOTWITHSTANDING ANY OTHER PROVISION CONTAINED HEREIN, ANY\n";
            mStream << "# LIABILITY FOR DAMAGES RESULTING FROM THE SOFTWARE OR ITS USE IS\n";
            mStream << "# EXPRESSLY DISCLAIMED, WHETHER ARISING IN CONTRACT, TORT (INCLUDING\n";
            mStream << "# NEGLIGENCE) OR STRICT LIABILITY, EVEN IF COPYRIGHT OWNERS ARE ADVISED\n";
            mStream << "# OF THE POSSIBILITY OF SUCH DAMAGES.\n";
            mStream << "#\n";
            mStream << "# [TC file last modification information]\n";
            mStream << QString("#   Date   : %1\n").arg(currentTime.toString("yyyy-MM-dd hh:mm:ss"));
            mStream << QString("#   User   : %1\n").arg(getUserName());
            mStream << QString("#   System : %1\n").arg(QSysInfo::prettyProductName());
            mStream << "#\n\n";
            mStream << QString("- test: %1\n").arg(mSfcDescription[0]);
            mStream << QString("  version: %1\n").arg(mSfcDescription[1]);
            mStream << QString("  description: %1\n").arg("\"" + mSfcDescription[2] + "\"");
            mStream << "  case:\n";
            mStream << "  - name: init AEMInitializeComplete\n";
            mStream << "    input:\n";
            mStream << "      - SFC.Extension.AEM.Inter_AEMInitializeComplete: true\n\n";
        }
    }

    void openNewFile(const int& splitTcFileCount) {
        if (mFile.isOpen()) {
            mFile.close();
        }

        QDir dir(mTCFileDirPath);
        if (!dir.exists()) {
            dir.mkpath(".");
        }

        QString tcFileName;
        if (mSfcDescription[2].isEmpty() == false) {
            QRegularExpression regex("\\[[A-Z]+\\d+\\]");
            QRegularExpressionMatch match = regex.match(mSfcDescription[2]);
            if (match.hasMatch()) {
                if (splitTcFileCount == 1) {
                    QDir dir(mTCFileDirPath);
                    QStringList files = dir.entryList(QDir::Files);
                    for (const QString& fileName : files) {
                        QString filePath = dir.filePath(fileName);
                        QFile file(filePath);

                        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                            qWarning() << "Failed to open file:" << filePath;
                            continue;
                        }

                        QTextStream in(&file);
                        QString fileContent = in.readAll();
                        file.close();

                        if (fileName.contains(mSfcDescription[0] + match.captured(0), Qt::CaseSensitive) &&
                            fileName.endsWith(".tc", Qt::CaseInsensitive)) {
                            if (QFile::remove(filePath)) {
                                qDebug() << "Deleted file:" << filePath;
                            } else {
                                qWarning() << "Failed to delete file:" << filePath;
                            }
                        }
                    }
                    tcFileName = QString("%1.tc").arg(mTCFileDirPath + "/" + mSfcDescription[0] + match.captured(0));
                } else {
                    if (splitTcFileCount == 2) {
                        QFile::remove(mTCFileDirPath + "/" + mSfcDescription[0] + match.captured(0) + "(001).tc");
                        QFile::rename(QString("%1.tc").arg(mTCFileDirPath + "/" + mSfcDescription[0] + match.captured(0)),
                                      QString("%1.tc").arg(mTCFileDirPath + "/" + mSfcDescription[0] + match.captured(0) + "(" +
                                                           QString::number(1).rightJustified(3, '0') + ")"));
                    }
                    tcFileName = QString("%1.tc").arg(mTCFileDirPath + "/" + mSfcDescription[0] + match.captured(0) + "(" +
                                                      QString::number(splitTcFileCount).rightJustified(3, '0') + ")");
                }
            } else {
                qWarning() << "SFC Description Error.";
                return;
            }
        } else {
            qWarning() << "Cannot create test, version, and description in tc file due to unreadable Description in Excel file.";
            tcFileName = mOpenFilePath.section('.', 0, -2) + ".tc";
        }

        if (tcFileName.isEmpty() == true) {
            tcFileName = "NO_SFC_DESCRIPTION_AND_FILE_NAME.tc";
        }

        mFile.setFileName(tcFileName);

        if (!mFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            mStream.setDevice(nullptr);  // 명시적으로 디바이스를 제거
            return;
        }
        mStream.setDevice(&mFile);

        writeHeader();
    }

    QString mTCFileDirPath;
    QFile mFile;
    QTextStream mStream;
    QStringList mSfcDescription;
    int mSplitSize = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeTCFileSplitSize).toInt();
    int mSplitTcFileCount = 1;
    int mPrevSplitTcFileCount = 1;
    int mMaxFileSize = mSplitSize * 1024 * 1024;  // 100MB를 바이트로 변환
    qint64 mAvailableSpace = 0;
    qint64 mFileSize = 0;
    QString mOpenFilePath;
};

#endif  // CASE_DATA_WRITER_H
