#ifndef TEST_CASE_WRITER_H
#define TEST_CASE_WRITER_H

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QStorageInfo>

class TestCaseWriter {
public:
    TestCaseWriter(const QString& basePath = QString()) : mTCFileDirPath(basePath) {
        mSfcDescription = ControlExcel::instance().data()->isDescriptionDataInfo();
        if (mTCFileDirPath.size() == 0) {
            QString path = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeLastSavedFilePath).toString();
            int lastSlashIndex = path.lastIndexOf("/");
            mTCFileDirPath = path.mid(0, lastSlashIndex);
            mAvailableSpace = QStorageInfo(mTCFileDirPath).bytesAvailable() * 9 / 10;
        }
    }

    ~TestCaseWriter() {
        mStream << "  - name: timeout for gcov\n";
        mStream << "    input:\n";
        mStream << "      - Vehicle.CV.EHS.Input_EHSStatus: timeout\n\n";
        mStream << "  - name: dump for gcov\n";
        mStream << "    input:\n";
        mStream << "      - dump: true\n";
    }

    void genTestCaseFile(const QJsonObject& json) {
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
                            QVector<bool> isInitialize;
                            QJsonObject outputSignalList = resultObj["Output_Signal"].toObject();
                            QJsonObject signalObj;
                            for (auto sigIt = outputSignalList.begin(); sigIt != outputSignalList.end(); ++sigIt) {
                                signalObj = sigIt.value().toObject();
                                outputSignal << sigIt.key();
                                outputValue << signalObj["Output_Value"].toString();
                                isInitialize << signalObj["isInitialize"].toBool();
                            }
                            for (const QString& caseKey : resultObj.keys()) {
                                if (caseKey.startsWith("_Case[")) {
                                    QVector<std::tuple<QString, int, int, QJsonArray, QJsonObject, QJsonArray>> signalList;
                                    QJsonObject caseObj = resultObj[caseKey].toObject();
                                    QString caseName = caseObj["Case"].toString();
                                    QStringList inputSignalList =
                                        resultObj[caseKey][QString("InputSignalList")].toObject().keys();
                                    for (const QString& inputSignal : inputSignalList) {
                                        signalList.emplace_back(std::make_tuple(
                                            inputSignal,
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
                                    QJsonArray casesArray = caseObj["cases"].toArray();
                                    QStringList parts;
                                    QStringList inputDataInfo;
                                    int signalOrder = 0;
                                    QString signalName;
                                    QString signalValue;
                                    QString enumString;
                                    QString triggerEnumString;
                                    QString triggerSignal;
                                    QString triggerValue;
                                    for (const QJsonValue& subArrayValue : casesArray) {
                                        parts = subArrayValue.toString().split("=>");
                                        write("- name: " + tcName + ", " + resultName + ", " + caseName + " " +
                                                  QString::number(testCaseCount++),
                                              1);
                                        if (vehicleType.isEmpty() == false) {
                                            write("tag: " + vehicleType, 2, 1);
                                        }
                                        write("precondition:", 2, 1);
                                        inputDataInfo = parts[1].split(":");
                                        signalOrder = 0;
                                        for (QString& number : parts[0].split(",", Qt::SkipEmptyParts)) {
                                            number = number.trimmed();
                                            if (number.contains("[Empty]") == false) {
                                                enumString = std::get<4>(signalList[signalOrder])[number].toString();
                                                if (inputDataInfo[0] == std::get<0>(signalList[signalOrder])) {
                                                    triggerEnumString =
                                                        std::get<4>(signalList[signalOrder])[inputDataInfo[1].trimmed()]
                                                            .toString();
                                                }
                                                if (std::get<0>(signalList[signalOrder]).contains("SFC.Private.IGNElapsed")) {
                                                    signalName =
                                                        ControlExcel::instance().data()->isIGNElapsedName(number.toInt());
                                                    signalValue = "0x" + QString::number(++ignCount, 16).toUpper();
                                                } else {
                                                    signalName = std::get<0>(signalList[signalOrder]);
                                                    signalValue = number;
                                                }
                                                enumString = enumString.isEmpty() ? QString() : QString(" # %1").arg(enumString);
                                                write("- " + signalName + ": " + quoteIfNotNumeric(signalValue) + enumString, 3);
                                            }
                                            signalOrder++;
                                        }

                                        if (isInitialize.contains(true)) {
                                            write("init:", 2, 1);
                                            for (int initCnt = 0; initCnt < isInitialize.size(); initCnt++) {
                                                if (isInitialize[initCnt] == true) {
                                                    if (outputValue[initCnt][0].isDigit() == true) {
                                                        if (outputValue[initCnt].contains("0x")) {
                                                            write("- " + outputSignal[initCnt] + ": 0x0", 3);
                                                        } else {
                                                            write("- " + outputSignal[initCnt] + ": 0", 3);
                                                        }
                                                    } else {
                                                        write("- " + outputSignal[initCnt] + ": " +
                                                                  quoteIfNotNumeric(outputValue[initCnt]),
                                                              3);
                                                    }
                                                }
                                            }
                                        }
                                        if (parts[1].split(":").size() == 2) {
                                            write("input:", 2, 1);
                                            if (inputDataInfo[0].contains("SFC.Private.IGNElapsed")) {
                                                triggerSignal =
                                                    ControlExcel::instance().data()->isIGNElapsedName(inputDataInfo[1].toInt());
                                                triggerValue = "0x" + QString::number(++ignCount, 16).toUpper();
                                            } else {
                                                triggerSignal = inputDataInfo[0];
                                                triggerValue = inputDataInfo[1];
                                            }
                                            triggerEnumString =
                                                triggerEnumString.isEmpty() ? QString() : QString(" # %1").arg(triggerEnumString);
                                            write("- " + triggerSignal + ": " + quoteIfNotNumeric(triggerValue.trimmed()) +
                                                      triggerEnumString,
                                                  3);
                                        }
                                        write("output:", 2);
                                        for (int outputCnt = 0; outputCnt < outputSignal.size(); outputCnt++) {
                                            QString tempOutputSignal = outputSignal[outputCnt];
                                            if (tempOutputSignal.contains("Collect")) {
                                                tempOutputSignal = "collect";
                                            }
                                            (outputCnt == outputSignal.size() - 1)
                                                ? write(
                                                      "- " + tempOutputSignal + ": " + quoteIfNotNumeric(outputValue[outputCnt]),
                                                      3, 2)
                                                : write(
                                                      "- " + tempOutputSignal + ": " + quoteIfNotNumeric(outputValue[outputCnt]),
                                                      3);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

private:
    void write(const QString& text = "", const int& tabStep = 0, const int& newLineStep = 1) {
        if (!mFile.isOpen() || mPrevSplitTcFileCount != mSplitTcFileCount) {
            openNewFile(mSplitTcFileCount);
            mPrevSplitTcFileCount = mSplitTcFileCount;
        }
        mStream << QString("  ").repeated(tabStep) << text << QString("\n").repeated(newLineStep);
        if (mFile.size() > mMaxFileSize) {
            mFileSize += mFile.size();
            if (mFileSize > mAvailableSpace) {
                qDebug() << "TC file size exceeds the available capacity of the current system.";
                QApplication::exit(1);
            } else {
                mSplitTcFileCount++;
            }
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

    void openNewFile(const int& splitTcFileCount) {
        if (mFile.isOpen()) {
            mFile.close();
        }

        QDir dir(mTCFileDirPath);
        if (!dir.exists()) {
            dir.mkpath(".");
        }

        QString filename;
        QRegularExpression regex("\\[[A-Z]+\\d+\\]");
        QRegularExpressionMatch match = regex.match(mSfcDescription[2]);

        if (match.hasMatch()) {
            if (splitTcFileCount == 1) {
                filename = QString("%1.tc").arg(mTCFileDirPath + "/" + mSfcDescription[0] + match.captured(0));
            } else {
                if (splitTcFileCount == 2) {
                    QFile::rename(QString("%1.tc").arg(mTCFileDirPath + "/" + mSfcDescription[0] + match.captured(0)),
                                  QString("%1.tc").arg(mTCFileDirPath + "/" + mSfcDescription[0] + match.captured(0) + "(" +
                                                       QString::number(1).rightJustified(3, '0') + ")"));
                }
                filename = QString("%1.tc").arg(mTCFileDirPath + "/" + mSfcDescription[0] + match.captured(0) + "(" +
                                                QString::number(splitTcFileCount).rightJustified(3, '0') + ")");
            }

            mFile.setFileName(filename);
            if (!mFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                qWarning() << "Failed to open file:" << filename;
                return;
            }
            mStream.setDevice(&mFile);

            writeHeader();
        }
    }

    QString mTCFileDirPath;
    QFile mFile;
    QTextStream mStream;
    QStringList mSfcDescription;
    int mSplitSize = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeTCFileSplitSize).toInt();
    int mSplitTcFileCount = 1;
    int mPrevSplitTcFileCount = 1;
    int mMaxFileSize = mSplitSize * 1024 * 1024;  // 100MB를 바이트로 변환
    qint64 mAvailableSpace;
    qint64 mFileSize;
};

#endif  // TEST_CASE_WRITER_H
