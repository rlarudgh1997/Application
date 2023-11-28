#include <QApplication>
#include <QDebug>

#include "MainWindow.h"
#include "CommonDefine.h"

#if defined(OS_LINUX)
#include <execinfo.h>
#include <csignal>
#include <fstream>
#include <unistd.h>
#endif

void sigHandler(int32_t sig) {
    qDebug() << "Error : signal " << sig;

#if defined(OS_LINUX)
    void* array[20];
    int32_t size = backtrace(array, 20);
    char** backTraceMsg = backtrace_symbols(array, size);

    time_t rawtime;
    time(&rawtime);
    struct tm* timeInfo = nullptr;
    localtime_r(&rawtime, timeInfo);
    char filename[1024];
    memset(filename, 0, sizeof(filename));
    if (timeInfo != nullptr) {
        sprintf(filename, "./TraceMessage-%d-%04d%02d%02d.%02d%02d%02d", getpid(), timeInfo->tm_year + 1900, timeInfo->tm_mon + 1,
                timeInfo->tm_mday, timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
    } else {
        sprintf(filename, "./TraceMessage-%d-%04d%02d%02d.%02d%02d%02d", getpid(), 2023, 1, 1, 1, 1, 1);
    }

    std::ofstream dumpFile;
    dumpFile.open(filename);
    for (size_t i = 0; i < static_cast<size_t>(size); i++) {
        qDebug() << "TraceMessage[" << i << std::hex << "] : [0x" << array << "] - " << backTraceMsg[i];
        dumpFile << i << " : [0x" << std::hex << array << "] " << backTraceMsg[i] << std::endl;
    }
    dumpFile.close();
    free(backTraceMsg);

    exit(1);
#endif
}

enum LogLevel {
    LogLevelInvalid = 0x0000,
    LogLevelDebug = 0x0001,
    LogLevelInfo = 0x0002,
    LogLevelWarnig = 0x0004,
    LogLevelCritical = 0x0008,
    LogLevelFatal = 0x0010,
    LogLevelAll = (LogLevelDebug | LogLevelInfo | LogLevelWarnig | LogLevelCritical | LogLevelFatal),
};
int logLevel = (LogLevelInfo);

// 사용자 정의 로깅 핸들러 함수
void customMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
    switch (type) {
        case QtDebugMsg: {
            if (logLevel & LogLevelDebug) {
                qDebug() << "Debug    :" << msg;
            }
            break;
        }
        case QtInfoMsg: {
            if (logLevel & LogLevelInfo) {
                qInfo() << "Info     :" << msg;
            }
            break;
        }
        case QtWarningMsg: {
            if (logLevel & LogLevelWarnig) {
                qWarning() << "Warning  :" << msg;
            }
            break;
        }
        case QtCriticalMsg: {
            if (logLevel & LogLevelCritical) {
                qCritical() << "Critical :" << msg;
            }
            break;
        }
        // case QtFatalMsg : {
        //     if (logLevel & LogLevelFatal) {
        //         qFatal() << "Fatal    :" << msg;
        //     }
        //     break;
        // }
        default: {
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    signal(SIGABRT, sigHandler);
    signal(SIGSEGV, sigHandler);

    QApplication app(argc, argv);

    // qInstallMessageHandler(customMessageHandler);
    QString messagPattern = QString("%1 %2%3%4%5%6 %7")
                                .arg("[%{time yyyyMMdd h:mm:ss.zzz}]")  // .arg("[%{time yyyyMMdd h:mm:ss.zzz t}")
                                .arg("[%{if-debug}D%{endif}")
                                .arg("%{if-info}I%{endif}")
                                .arg("%{if-warning}W%{endif}")
                                .arg("%{if-critical}C%{endif}")
                                .arg("%{if-fatal}F%{endif}]")
                                .arg("%{message}");  // .arg("[%{category}] %{message}");
    qSetMessagePattern(messagPattern);

    // qputenv("QT_LOGGING_RULES", "qt.scenegraph*=true");
    // qputenv("QT_LOGGING_RULES", "myApp.debug=true;myApp.info=true;myApp.warning=false;myApp.critical=false");

    MainWindow mainWindow;

    return app.exec();
}
