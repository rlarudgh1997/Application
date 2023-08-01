#ifndef COMMON_UTIL_H
#define COMMON_UTIL_H

#include <QObject>
#include <QDebug>

#include <QApplication>
#include <QWidget>
#include <QFile>
#include <QFileInfoList>
#include <QDir>
#include <QMap>
#include <QElapsedTimer>
#include <QMutex>
#include <QMutexLocker>
#include <QMessageBox>
#include <QProcess>
#include <QThread>
// #include <thread>




#define APP_PWD QApplication::applicationDirPath().toLatin1().data()
#define CURRENT_DATE_TIME QString("%1_%2").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")) \
                                            .arg(QTime::currentTime().msec())
#define GET_CURRENT_DATE_TIME CURRENT_DATE_TIME.toLatin1().data()


#define ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))

#define TO_STRING(A) QString("%1").arg(A)
#define STR_COMPARE(str1, str2) (QString::compare(str1, str2) == false) ? (true) : (false)
#define OBJECT_NAME(widget) (widget->objectName().toLatin1().data())
#define SWAP(a, b) {(a)^=(b)^=(a)^=(b);}    /*Value Switching*/
#define BOOL_REVERSE(var) (var = (var) ? (false) : (true))


// name : The first letter starts with an uppercase letter
#define QML_WRITABLE_PROPERTY(type, name, notify) \
    Q_PROPERTY(type name READ get##name WRITE set##name NOTIFY signal##name##Changed) \
    public: \
        type get##name() const { \
            return m##name ; \
        } \
    public Q_SLOTS: \
        void set##name(const type& name) { \
            QMutexLocker lock(&mMutex##name); \
            if (m##name != name) { \
                m##name = name; \
                if (notify) { \
                    emit signal##name##Changed(m##name); \
                } \
            } \
        } \
    Q_SIGNALS: \
        void signal##name##Changed(const type& name); \
    private: \
        QMutex mMutex##name; \
        type m##name; \


#define QML_ENUM_CLASS(name, ...) \
    class name : public QObject { \
        Q_GADGET \
    public: \
        enum Type { __VA_ARGS__ }; \
        Q_ENUMS(Type) \
    };


#define INIT_BUTTON_WIEDGET(info, widget, index) \
    if (index >= 0) {\
        info.insert(widget->objectName(), index);\
        connect(widget, SIGNAL(pressed()), this, SLOT(slotPressed()));\
    } \
    connect(widget, SIGNAL(clicked()), this, SLOT(slotClicked()));\


template <typename T>
inline void LIMIT(T &value, T min, T max) {
    (value) = ((value) < (min)) ? (min) : (((value) > (max)) ? (max) : (value));
}

template <typename T>
inline void LIMIT_P(T& value, T gap, T min, T max) {
    value += gap;
    (value) = ((value) < (min)) ? (min) : (((value) > (max)) ? (max) : (value));
}

template <typename T>
inline void LIMIT_M(T& value, T gap, T min, T max) {
    value -= gap;
    (value) = ((value) < (min)) ? (min) : (((value) > (max)) ? (max) : (value));
}


template <typename T>
inline void REVOLVE(T &value, T min, T max) {
    (value) = ((value) < (min)) ? (max) : ((value > (max)) ? (min) : (value));
}

template <typename T>
inline void REVOLVE_P(T& value, T gap, T min, T max) {
    value += gap;
    (value) = ((value) < (min)) ? (max) : ((value > (max)) ? (min) : (value));
}

template <typename T>
inline void REVOLVE_M(T& value, T gap, T min, T max) {
    value -= gap;
    (value) = ((value) < (min)) ? (max) : ((value > (max)) ? (min) : (value));
}

template <typename T1, typename T2, typename T3>
inline void SET_PROPERTY(T1 widget, T2 name, T3 value) {
    if (widget) {
        widget->setProperty(name, value);
    } else {
        qDebug() << "Fail to calling function - widget->setProperty(" << name << ", " << value << ")";
    }
}




namespace ivis {
namespace common {



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

class FileInfo {
public:
    static QStringList isFileListInfo(const QString& path, QFileInfoList fileList = QFileInfoList()) {
        QString currentPath = path;
        if (currentPath.size() == 0) {
            currentPath.append(QApplication::applicationDirPath());
            currentPath.append("/SFC");
        }
        return readFileInfo(currentPath, fileList);
    }
    static QStringList readFile(const QString& filePath) {
        return readFileData(filePath);
    }
    static int writeFile(const QString& filePath, const QString& str) {
        return writeFileData(filePath, str);
    }

private:
    static QStringList readFileInfo(const QString& path, QFileInfoList& fileList) {
        QStringList fileNames = QStringList();
        QDir directory(path);
        directory.setFilter(QDir::Files | QDir::NoDotAndDotDot);
        fileList = directory.entryInfoList();
        qDebug() << "\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
        qDebug() << "path :" << path << ", Count :" << fileList.size();
        foreach(const QFileInfo& file, fileList) {
            fileNames.append(file.fileName());
            qDebug() << "--------------------------------------------------------------------------------------------";
            qDebug() << "File Name :" << file.fileName() << ", Size :" << file.size() << "byte";
            qDebug() << "File Path :" << file.filePath();
            // qDebug() << "File Created: " << file.created().toString();
        }
        qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
        return fileNames;
    }
    static QStringList readFileData(const QString& filePath) {
        QStringList fileContent = QStringList();
        QFile file(filePath);
        // qDebug() << "filePath :" << filePath;
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream readData(&file);
            while (!readData.atEnd()) {
                QString data = readData.readLine();
                fileContent.append(data);
            }
            file.close();
        } else {
            if (file.exists()) {
                qDebug() << "Fail to open no exist file :" << filePath;
            } else {
                qDebug() << "Fail to open error :" << filePath;
            }
        }
        return fileContent;
    }
    static int writeFileData(const QString& filePath, const QString& str) {
        // qDebug() << "filePath :" << filePath;
        QFile file(filePath);
        if (file.open(QFile::WriteOnly | QFile::Text) == false) {
            return 0;
        }
        QTextStream out(&file);
        out << str;
        file.close();
        return str.size();
    }
};

class ExcuteProgram {
public:
    ExcuteProgram() {
        mUseProcess = false;
    }
    explicit ExcuteProgram(const bool& useProcess) {
        mUseProcess = useProcess;
    }
    int start(const QString& cmd, QStringList& log) {
        int result = 0;
        log.clear();
        if (mUseProcess) {
            QProcess process;
            QStringList splitCmd = cmd.split(" ");

            if (splitCmd.size() == 0) {
                 process.start(cmd);
            } else {
                QString command = splitCmd.at(0);
                QStringList arguments = QStringList();
                for (int index = 1; index < splitCmd.size(); index++) {
                    arguments.append(splitCmd[index]);
                }
                process.start(command, arguments);
            }

            if (process.waitForStarted()) {    // if (process.waitForFinished()) {
                while (process.waitForReadyRead()) {
                    QString readAllData = process.readAll();
                    QString logData = QString();
                    foreach(const QString& data, readAllData) {
                        if (data.compare("\n") == false) {
                            // qDebug() << "Log :" << logData;
                            log.append(logData);
                            logData.clear();
                        } else {
                            logData.append(data);
                        }
                    }
                }
            } else {
                result = (-1);
            }
        } else {
            result = system(cmd.toLatin1());
        }
        qDebug() << "ExcuteProgram::start() ->" << result;
        return result;
    }

private:
    bool mUseProcess = true;
};

class CheckLib : public QObject {
    Q_OBJECT

public:
    CheckLib() {}
    void setLibInfo(const QStringList& libInfo) {
        foreach(const auto& info, libInfo) {
            mCheckLib[info] = false;
        }
    }
    void check() {
        this->moveToThread(mThread);
        connect(mThread, &QThread::finished, this, &QObject::deleteLater);
        connect(mThread, &QThread::started, this, &CheckLib::runThread);
        mThread->start();
        // this->moveToThread(&mThread);
        // connect(&mThread, &QThread::finished, this, &QObject::deleteLater);
        // connect(&mThread, &QThread::started, this, &CheckLib::runThread);
        // mThread.start();
    }

private:
    void join() {
    }
    void runThread() {
        QMapIterator<QString, bool> iter(mCheckLib);
        while (iter.hasNext()) {
            iter.next();
            ExcuteProgram process(false);
            QString cmd = QString("pip list | grep ""%1"" >> %2/CheckLib.txt").arg(iter.key()).arg(APP_PWD);
            QStringList log;
            process.start(cmd, log);
        }
        // QThread::currentThread()->quit();
        // QThread::currentThread()->wait();
    }

private:
    QThread* mThread = new QThread();
    // QThread mThread;
    QMap<QString, bool> mCheckLib = QMap<QString, bool>();
};




}  // end of namespace common
}  // end of namespace ivis


#endif  // COMMON_UTIL_H
