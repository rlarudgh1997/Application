#include <QApplication>
#include <QDebug>

#include "MainWindow.h"
#include "CommonDefine.h"

#include <execinfo.h>
#include <csignal>
#include <fstream>
#include <unistd.h>


void sigHandler(int32_t sig) {
    char** backTraceMsg;
    void *array[20];
    int32_t size;

    size = backtrace(array, 20);
    qDebug() << "Error : signal " << sig;
    backTraceMsg = backtrace_symbols(array, size);

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
}

int main(int argc, char *argv[]) {
    signal(SIGABRT, sigHandler);
    signal(SIGSEGV, sigHandler);

    // Q_INIT_RESOURCE(application);

    QApplication app(argc, argv);

    MainWindow mainWindow;

    return app.exec();
}
