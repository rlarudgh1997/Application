#ifndef COMMON_UTIL_H
#define COMMON_UTIL_H

#include <QObject>
#include <QDebug>

#include <QGuiApplication>
#include <QElapsedTimer>
#include <QMutex>
#include <QMutexLocker>
#include <QThread>

namespace ivis {
namespace common {

// name : The first letter starts with an uppercase letter
#define QML_WRITABLE_PROPERTY(type, name, notify)                                     \
    Q_PROPERTY(type name READ get##name WRITE set##name NOTIFY signal##name##Changed) \
public:                                                                               \
    type get##name() const {                                                          \
        return m##name;                                                               \
    }                                                                                 \
public Q_SLOTS:                                                                       \
    void set##name(const type& name) {                                                \
        QMutexLocker lock(&mMutex##name);                                             \
        if (m##name != name) {                                                        \
            m##name = name;                                                           \
            if (notify) {                                                             \
                emit signal##name##Changed(m##name);                                  \
            }                                                                         \
        }                                                                             \
    }                                                                                 \
Q_SIGNALS:                                                                            \
    void signal##name##Changed(const type& name);                                     \
                                                                                      \
private:                                                                              \
    QMutex mMutex##name;                                                              \
    type m##name;

template <typename T>
inline void LIMIT(T& value, T min, T max) {
    value = (value < min) ? (min) : ((value >= max) ? (max - 1) : (value));
}

template <typename T>
inline void LIMIT_P(T& value, T gap, T min, T max) {
    value += gap;
    LIMIT(value, min, max);
}

template <typename T>
inline void LIMIT_M(T& value, T gap, T min, T max) {
    value -= gap;
    LIMIT(value, min, max);
}

template <typename T>
inline void REVOLVE(T& value, T min, T max) {
    value = (value < min) ? (max - 1) : ((value >= max) ? (min) : (value));
}

template <typename T>
inline void REVOLVE_P(T& value, T gap, T min, T max) {
    value += gap;
    REVOLVE(value, min, max);
}

template <typename T>
inline void REVOLVE_M(T& value, T gap, T min, T max) {
    value -= gap;
    REVOLVE(value, min, max);
}

class CheckTimer {
public:
    CheckTimer() {
        mElapsedTimer.start();
        // qDebug() << "StartTime :" << mElapsedTimer.elapsed() << "ms";
    }
    void reset() {
        mElapsedTimer.restart();
        // qDebug() << "ReStartTime :" << mElapsedTimer.elapsed() << "ms";
    }
    void check(const QString& info = QString()) {
        if (info.size()) {
            qDebug() << "CheckTime[" << info.toLatin1().data() << "] :" << mElapsedTimer.elapsed() << "ms\n";
        } else {
            qDebug() << "CheckTime :" << mElapsedTimer.elapsed() << "ms\n";
        }
    }

private:
    QElapsedTimer mElapsedTimer;
};

}  // end of namespace common
}  // end of namespace ivis

#endif  // COMMON_UTIL_H
