#ifndef LOGUTIL_H
#define LOGUTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <QLoggingCategory>
#include <QString>
#include <QElapsedTimer>
#include <QTextStream>
#include <QFile>
#include <QDateTime>


class LogUtilElapsedTime
{
public:
    explicit LogUtilElapsedTime(const QString &functionName);
    ~LogUtilElapsedTime();

    void printElapsedTime(const QString &msg);

public:
    static void setEnabled(bool enabled);
    static bool endabled() { return s_enabled; }

private:
    QElapsedTimer m_elapsedTimer;
    QString m_functionName;
    static bool s_enabled;
};

void logHandler(QtMsgType type, const QMessageLogContext & logContext, const QString &msg);
void logHandlerForHarman(QtMsgType type, const QMessageLogContext & logContext, const QString &msg);
void logHandlerForDLT(QtMsgType type, const QMessageLogContext & logContext, const QString &msg);

#endif // LOGUTIL_H
