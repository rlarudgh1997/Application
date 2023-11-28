#include "logutil.h"

Q_LOGGING_CATEGORY(logUtilClass, "IVISuite.AL.LogUtil")

bool LogUtilElapsedTime::s_enabled = true;

LogUtilElapsedTime::LogUtilElapsedTime(const QString& functionName) {
    if (s_enabled) {
        qCDebug(logUtilClass) << functionName << "...start";
        m_functionName = functionName;
        m_elapsedTimer.start();
    }
}

LogUtilElapsedTime::~LogUtilElapsedTime() {
    if (s_enabled) {
        qCDebug(logUtilClass) << m_functionName << "...end" << m_elapsedTimer.elapsed() << "msec";
    }
}

void LogUtilElapsedTime::printElapsedTime(const QString& msg) {
    qCDebug(logUtilClass) << m_functionName << msg << ">>" << m_elapsedTimer.elapsed() << "msec";
}

void LogUtilElapsedTime::setEnabled(bool enabled) {
    s_enabled = enabled;
}

void logHandler(QtMsgType type, const QMessageLogContext& logContext, const QString& msg) {
    switch ((int)type) {
        case QtDebugMsg:
            fprintf(stderr, "[%s]%s %s\n", logContext.category, logContext.function, msg.toLatin1().data());
            fflush(stderr);
            break;
        case QtWarningMsg:
            fprintf(stderr, "[%s]%s %s\n", logContext.category, logContext.function, msg.toLatin1().data());
            fflush(stderr);
            break;
        case QtCriticalMsg:
            fprintf(stderr, "[%s]%s %s\n", logContext.category, logContext.function, msg.toLatin1().data());
            fflush(stderr);
            break;
        case QtFatalMsg:
            fprintf(stderr, "[%s]%s %s\n", logContext.category, logContext.function, msg.toLatin1().data());
            fflush(stderr);
    }
}

void logHandlerForHarman(QtMsgType type, const QMessageLogContext& logContext, const QString& msg) {
#ifdef HARMAN_LOG
    CHBTraceScope const localTraceServerScope(g_traceScopeDef);

    if (true == QString("%1").arg(logContext.category).contains("hmi.app.util.log")) {
        localTraceServerScope.sendFormattedMessage("[%s] %s", logContext.category, msg.toLocal8Bit().data());

        return;
    }

    switch (type) {
        case QtDebugMsg:
            localTraceServerScope.sendFormattedMessage("[%s]%s %s", logContext.category, logContext.function,
                                                       msg.toLocal8Bit().data());
            break;
        case QtWarningMsg:
            localTraceServerScope.sendFormattedWarning("[%s]%s %s", logContext.category, logContext.function,
                                                       msg.toLocal8Bit().data());
            break;
        case QtCriticalMsg:
            localTraceServerScope.sendFormattedError("[%s]%s %s", logContext.category, logContext.function,
                                                     msg.toLocal8Bit().data());
            break;
        default:
            localTraceServerScope.sendFormattedMessage("[%s]%s %s", logContext.category, logContext.function,
                                                       msg.toLocal8Bit().data());
    }
#else
    logHandler(type, logContext, msg);
#endif
}

void logHandlerForDLT(QtMsgType type, const QMessageLogContext& logContext, const QString& msg) {
#ifdef DLT_LOG

#else
    logHandler(type, logContext, msg);
#endif
}
