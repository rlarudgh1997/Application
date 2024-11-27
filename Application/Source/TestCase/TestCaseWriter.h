#ifndef TEST_CASE_WRITER_H
#define TEST_CASE_WRITER_H

#include <QFile>
#include <QTextStream>
#include <QDateTime>

class TestCaseWriter {
public:
    TestCaseWriter(const QString& basePath) : mTCFileDirPath(basePath) {
        mSfcDescription = ControlExcel::instance().data()->isDescriptionDataInfo();
    }

    ~TestCaseWriter() {
        mStream << "  - name: timeout for gcov\n";
        mStream << "    input:\n";
        mStream << "      - Vehicle.CV.EHS.Input_EHSStatus: timeout\n\n";
        mStream << "  - name: dump for gcov\n";
        mStream << "    input:\n";
        mStream << "      - dump: true\n";

        // qDebug() << "@@@@@ File size: " << mFile.size();
    }

    void write(const QString& text = "", const int& tabStep = 0, const int& newLineStep = 1) {
        if (!mFile.isOpen()) {
            openNewFile();
        }
        mStream << QString("  ").repeated(tabStep) << text << QString("\n").repeated(newLineStep);
        // mStream.flush();
        if (mFile.size() > mMaxFileSize) {
            qDebug() << "@@@@@ File size: " << mFile.size();
            QApplication::exit(1);
        }
    }

private:
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
        // mStream.flush();
    }

    void openNewFile() {
        if (mFile.isOpen()) {
            mFile.close();
        }

        QDir dir(mTCFileDirPath);
        if (!dir.exists()) {
            dir.mkpath(".");
        }

        QRegularExpression regex("\\[[A-Z]+\\d+\\]");
        QRegularExpressionMatch match = regex.match(mSfcDescription[2]);

        if (match.hasMatch()) {
            QString filename = QString("%1.tc").arg(mTCFileDirPath + "/" + mSfcDescription[0] + match.captured(0));

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
    // QIODevice* device;
    int mSplitSize = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeTCFileSplitSize).toInt();
    int mMaxFileSize = mSplitSize * 1024 * 1024;  // 100MB를 바이트로 변환
};

#endif  // TEST_CASE_WRITER_H
