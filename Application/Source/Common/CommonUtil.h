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
#include <QFileSystemWatcher>
#include <QMenu>
#include <QPair>


namespace ivis {
namespace common {



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

template <typename T1>
inline void widgetVisible(T1* widget, const bool& visible = true) {
    if (widget) {
        if (visible) {
            widget->show();
            widget->raise();
        } else {
            widget->hide();
        }
    }
}

template <typename T1>
inline void widgetStyleSheet(T1* widget, const QString& styleSheet) {
    if (widget) {
        if (styleSheet.size() > 0) {
            widget->setStyleSheet(styleSheet);
        }
    }
}

template <typename T1>
inline void widgetEnable(T1* widget, const bool& enable, const QString& color = QString()) {
    if (widget) {
        QString setStyleSheet = (color.size() == 0) ? ((enable) ? ("color: blue") : ("color: gray")) : (color);
        widgetStyleSheet<T1>(widget, setStyleSheet);
        widget->setEnabled(enable);
    }
}

template <typename T1, typename T2>
inline T1* createWidget(T2* parent, const bool& show = false, const QRect& geometry = QRect(),
                                                                    const QString& styleSheet = QString()) {
    T1* widget = new T1(parent);
    if (geometry.isValid()) {
        widget->setGeometry(geometry);
    }
    widgetStyleSheet<T1>(widget, styleSheet);
    widgetVisible<T1>(widget, show);
    return widget;
}

//template <typename T1>
//inline void deleteWidget(T1* widget, const bool& clearConnect = false) {
//    if (widget) {
//        if (clearConnect) {
//            disconnect(widget);
//        }
//        delete widget;
//        widget = nullptr;
//    }
//}

















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
        return readFileListInfo(currentPath, fileList);
    }
    static QStringList readFile(const QString& filePath) {
        bool openError = false;
        return readFileDataInfo(filePath, openError);
    }
    static QPair<bool, QStringList> readFileData(const QString& filePath) {
        bool openError = false;
        QStringList readData = readFileDataInfo(filePath, openError);
        return QPair<bool, QStringList>(openError, readData);
    }
    static int writeFile(const QString& filePath, const QString& str, const bool& append) {
        return writeFileData(filePath, str, append);
    }

private:
    static QStringList readFileListInfo(const QString& path, QFileInfoList& fileList) {
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
    static QStringList readFileDataInfo(const QString& filePath, bool& openError) {
        QStringList fileContent = QStringList();
        QFile file(filePath);
        openError = false;
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
            openError = true;
        }
        return fileContent;
    }
    static int writeFileData(const QString& filePath, const QString& str, const bool& append) {
        // qDebug() << "filePath :" << filePath;
        QStringList readData = QStringList();
        if (append) {
            bool openError = false;
            readData = readFileDataInfo(filePath, openError);
            // qDebug() << "ReadData :" << readData;
        }
        QFile file(filePath);
        if (file.open(QFile::WriteOnly | QFile::Text) == false) {
            return 0;
        }
        QTextStream out(&file);
        foreach(const auto& data, readData) {
            QString lineData = QString("%1\n").arg(data);
            out << lineData;
        }
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
    bool start(const QString& cmd, QStringList& log) {
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
        qDebug() << "ExcuteProgram::start() ->" << ((result == 0) ? ("Sucess :") : ("Fail :")) << result;
        return (result == 0);
    }

private:
    bool mUseProcess = true;
};

class ExcuteProgramThread: public QObject {
    Q_OBJECT

public:
    explicit ExcuteProgramThread(const bool& useProcess) {
        mUseProcess = useProcess;
    }
    ~ExcuteProgramThread() {
        // join();
        qDebug() << "~ExcuteProgramThread()";
    }
    void setCommandInfo(const QString& cmd) {
        QMutexLocker lock(&mMutex);
        mCommand = cmd;
    }
    void start() {
        this->moveToThread(mThread);
        connect(mThread, &QThread::finished, this, &QObject::deleteLater);
        connect(mThread, &QThread::started, this, &ExcuteProgramThread::runThread);
        mThread->start();
    }
    void terminate() {
        if (mProcess.state() != QProcess::ProcessState::NotRunning) {
            mProcess.terminate();
        }
        join();
    }

private:
    void join() {
        if (mThread->isRunning()) {
            mThread->quit();
            mThread->wait();
        }
    }
    void runThread() {
        int result = 0;
        QStringList log = QStringList();

        emit signalExcuteProgramStarted();

        if (mUseProcess) {
            QStringList splitCmd = mCommand.split(" ");

            if (splitCmd.size() == 0) {
                mProcess.start(mCommand);
            } else {
                QString command = splitCmd.at(0);
                QStringList arguments = QStringList();
                for (int index = 1; index < splitCmd.size(); index++) {
                    arguments.append(splitCmd[index]);
                }
                mProcess.start(command, arguments);
            }

            if (mProcess.waitForStarted()) {    // if (mProcess.waitForFinished()) {
                while (mProcess.waitForReadyRead()) {
                    QString readAllData = mProcess.readAll();
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
            result = system(mCommand.toLatin1());
        }
        qDebug() << "ExcuteProgramThread::runThread() -> Result :" << result << "Command :" << mCommand;
        emit signalExcuteProgramCompleted(result == 0);
    }

signals:
    void signalExcuteProgramStarted();
    void signalExcuteProgramCompleted(const bool& result);

private:
    QThread* mThread = new QThread();
    QMutex mMutex;
    QProcess mProcess;
    bool mUseProcess = true;
    QString mCommand = QString();
};

class CheckLib : public QObject {
    Q_OBJECT

public:
    CheckLib() {}
    ~CheckLib() {
        // join();
    }
    void setLibInfo(const QStringList& libInfo) {
        foreach(const auto info, libInfo) {
            mCheckInfo[mCheckInfo.size()] = QPair<QString, bool>(info, false);
        }
    }
    void check() {
        this->moveToThread(mThread);
        connect(mThread, &QThread::finished, this, &QObject::deleteLater);
        connect(mThread, &QThread::started, this, &CheckLib::runThread);
        mThread->start();
    }

private:
    void join() {
        if (mThread->isRunning()) {
            mThread->quit();
            mThread->wait();
        }
    }
    void runThread() {
        QString filePath = QString("%1/CheckLib.txt").arg(APP_PWD);
        int count = 0;
        foreach(const auto& info, mCheckInfo) {
            ExcuteProgram process(false);
            QString appendStr = QString((count == 0) ? (">") : (">>"));
            QString cmd = QString("pip list | grep ""%1"" %2 %3").arg(info.first).arg(appendStr).arg(filePath);
            QStringList log;
            if (process.start(cmd, log)) {
                count++;
            }
        }
        QStringList readDataList = FileInfo::readFile(filePath);
        QString readData = QString();
        foreach(const auto& data, readDataList) {
            readData.append(data);
        }
        foreach(const auto& info, mCheckInfo) {
            emit signalCheckLibResult(info.first, readData.contains(info.first, Qt::CaseInsensitive));
        }
        emit signalCheckLibResult("Completed", true);
    }

signals:
    void signalCheckLibResult(const QString& lib, const bool& state);

private:
    QThread* mThread = new QThread();
    QMap<int, QPair<QString, bool>> mCheckInfo = QMap<int, QPair<QString, bool>>();
};


class FileSystemWatcherThread : public QObject {
    Q_OBJECT

public:
    FileSystemWatcherThread() {
    }
    explicit FileSystemWatcherThread(const QString& filePath) {
        mWatcherFile = filePath;
    }
    ~FileSystemWatcherThread() {
        // join();
        qDebug() << "~FileSystemWatcherThread()";
    }
    void start() {
        this->moveToThread(mThread);
        connect(mThread, &QThread::finished, this, &QObject::deleteLater);
        connect(mThread, &QThread::started, this, &FileSystemWatcherThread::runThread);
        mThread->start();

        connect(&mWatcher, &QFileSystemWatcher::fileChanged, [=](const QString &path) {
            QPair<bool, QStringList> readData = ivis::common::FileInfo::readFileData(path);
            if (readData.first) {
                emit signalWatcherFileReadError(true);
            } else {
                if (readData.second.size() > 0) {
                    emit signalWatcherFileDataChanged(readData.second);
                }
            }
        });
    }
    void clear() {
    }

private:
    void join() {
        if (mThread->isRunning()) {
            mThread->quit();
            mThread->wait();
        }
    }
    void runThread() {
        int count = 0;
        while (count < 10) {
            if (mWatcher.addPath(mWatcherFile)) {
                qDebug() << "\t [Sucess] Watcher file :" << mWatcherFile;
                count = 0;
                break;
            } else {
                qDebug() << "\t [Fail] Watcher file :" << count << mWatcherFile;
                count++;
                QThread::msleep(1000);
            }
        }

        if (count != 0) {
            emit signalWatcherFileFail(count);
        }
    }

signals:
    void signalWatcherFileFail(const int& count);
    void signalWatcherFileReadError(const bool& error);
    void signalWatcherFileDataChanged(const QStringList& fileData);

private:
    QThread* mThread = new QThread();
    QFileSystemWatcher mWatcher;
    QString mWatcherFile = QString();
    bool mRunThread = true;
};


}  // end of namespace common
}  // end of namespace ivis


#endif  // COMMON_UTIL_H
