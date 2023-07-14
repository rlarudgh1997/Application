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
    void *array[20];
    int32_t size = backtrace(array, 20);
    char** backTraceMsg = backtrace_symbols(array, size);

    time_t rawtime;
    time(&rawtime);
    struct tm* timeInfo = nullptr;
    localtime_r(&rawtime, timeInfo);
    char filename[1024];
    memset(filename, 0, sizeof(filename));
    if (timeInfo != nullptr) {
        sprintf(filename, "./TraceMessage-%d-%04d%02d%02d.%02d%02d%02d", getpid(),
                            timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday,
                            timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
    } else {
        sprintf(filename, "./TraceMessage-%d-%04d%02d%02d.%02d%02d%02d", getpid(),
                            2023, 1, 1,
                            1, 1, 1);
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

int main(int argc, char *argv[]) {
    signal(SIGABRT, sigHandler);
    signal(SIGSEGV, sigHandler);

    QString messagPattern = QString("%1 %2%3%4%5%6 %7")
                                    .arg("[%{time yyyyMMdd h:mm:ss.zzz}]")    // .arg("[%{time yyyyMMdd h:mm:ss.zzz t}")
                                    .arg("[%{if-debug}D%{endif}")
                                    .arg("%{if-info}I%{endif}")
                                    .arg("%{if-warning}W%{endif}")
                                    .arg("%{if-critical}C%{endif}")
                                    .arg("%{if-fatal}F%{endif}]")
                                    .arg("%{message}");   // .arg("[%{category}] %{message}");
    qSetMessagePattern(messagPattern);

    QApplication app(argc, argv);

    qputenv("QT_LOGGING_RULES", "qt.scenegraph*=true");

    MainWindow mainWindow;

    return app.exec();
}
