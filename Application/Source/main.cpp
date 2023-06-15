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

    QApplication app(argc, argv);

    MainWindow mainWindow;

    return app.exec();
}









