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
#include <QDateTime>

#include <type_traits>  // std::is_same 사용을 위해 필요

namespace ivis {
namespace common {

// value
#define REGISTER_WRITABLE_VALUE(TYPE, NAME, VALUE)                                    \
public:                                                                               \
    inline TYPE get##NAME() const {                                                   \
        return m##NAME;                                                               \
    }                                                                                 \
    inline void set##NAME(const TYPE& NAME) {                                         \
        m##NAME = NAME;                                                               \
    }                                                                                 \
private:                                                                              \
    TYPE m##NAME = VALUE;

// continer : QList, QVariantList ....
#define REGISTER_WRITABLE_LIST(LIST, VALUE, NAME)                                     \
public:                                                                               \
    inline void clear##NAME() {                                                       \
        m##NAME.clear();                                                              \
    }                                                                                 \
    inline int isSize##NAME() const {                                                 \
        return m##NAME.size();                                                        \
    }                                                                                 \
    inline VALUE get##NAME(const int& index) const {                                  \
        if (index < m##NAME.size()) {                                                 \
            return m##NAME.at(index);                                                 \
        } else {                                                                      \
            throw std::out_of_range("Index out of range");                            \
            return VALUE();                                                           \
        }                                                                             \
    }                                                                                 \
    inline void set##NAME(const int& index, const VALUE& value) {                     \
        if (index < m##NAME.size()) {                                                 \
            m##NAME[index] = value;                                                   \
        } else {                                                                      \
            throw std::out_of_range("Index out of range");                            \
        }                                                                             \
    }                                                                                 \
    LIST<VALUE> read##NAME() const {                                                  \
        return m##NAME;                                                               \
    }                                                                                 \
    inline void write##NAME(const LIST<VALUE>& value) {                               \
        m##NAME = value;                                                              \
    }                                                                                 \
private:                                                                              \
    LIST<VALUE> m##NAME = LIST<VALUE>();

// conatiner
#define REGISTER_WRITABLE_CONTAINER(CONTAINER, KEY, VALUE, NAME)                      \
public:                                                                               \
    inline void clear##NAME() {                                                       \
        m##NAME.clear();                                                              \
    }                                                                                 \
    inline int isSize##NAME() const {                                                 \
        return m##NAME.size();                                                        \
    }                                                                                 \
    inline VALUE get##NAME(const KEY& key) const {                                    \
        return m##NAME[key];                                                          \
    }                                                                                 \
    inline void set##NAME(const KEY& key, const VALUE& value) {                       \
        m##NAME[key] = value;                                                         \
    }                                                                                 \
    inline CONTAINER<KEY, VALUE> read##NAME() const {                                 \
        return m##NAME;                                                               \
    }                                                                                 \
    inline void write##NAME(const CONTAINER<KEY, VALUE>& info) {                      \
        m##NAME = info;                                                               \
    }                                                                                 \
private:                                                                              \
    CONTAINER<KEY, VALUE> m##NAME = CONTAINER<KEY, VALUE>();

// name : The first letter starts with an uppercase letter
#define REGISTER_WRITABLE_PROPERTY(type, name, value, notify)                         \
    Q_PROPERTY(type name READ get##name WRITE set##name NOTIFY signal##name##Changed) \
public:                                                                               \
    inline type get##name() const {                                                   \
        return m##name;                                                               \
    }                                                                                 \
public Q_SLOTS:                                                                       \
    inline void set##name(const type& name) {                                         \
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
private:                                                                              \
    QMutex mMutex##name;                                                              \
    type m##name = value;

// continer : QList, QVariantList ....
#define REGISTER_WRITABLE_PROPERTY_LIST(CONTAINER, VALUE, NAME, NOTI)                                \
    Q_PROPERTY(CONTAINER<VALUE> NAME READ read##NAME WRITE write##NAME NOTIFY signal##NAME##Changed) \
public:                                                                                              \
    inline void clear##NAME() {                                                                      \
        m##NAME.clear();                                                                             \
    }                                                                                                \
    inline int isSize##NAME() const {                                                                \
        return m##NAME.size();                                                                       \
    }                                                                                                \
    inline VALUE get##NAME(const int& index) const {                                                 \
        if (index < m##NAME.size()) {                                                                \
            return m##NAME.at(index);                                                                \
        } else {                                                                                     \
            throw std::out_of_range("Index out of range");                                           \
            return VALUE();                                                                          \
        }                                                                                            \
    }                                                                                                \
    CONTAINER<VALUE> read##NAME() const {                                                            \
        return m##NAME;                                                                              \
    }                                                                                                \
public Q_SLOTS:                                                                                      \
    inline void set##NAME(const int& index, const VALUE& value) {                                    \
        if (index < m##NAME.size()) {                                                                \
            QMutexLocker lock(&mMutex##NAME);                                                        \
            m##NAME[index] = value;                                                                  \
        } else {                                                                                     \
            throw std::out_of_range("Index out of range");                                           \
        }                                                                                            \
    }                                                                                                \
    inline void write##NAME(const CONTAINER<VALUE>& value) {                                         \
        QMutexLocker lock(&mMutex##NAME);                                                            \
        if (m##NAME != value) {                                                                      \
            m##NAME = value;                                                                         \
            if (NOTI) {                                                                              \
                emit signal##NAME##Changed(m##NAME);                                                 \
            }                                                                                        \
        }                                                                                            \
    }                                                                                                \
Q_SIGNALS:                                                                                           \
    void signal##NAME##Changed(const CONTAINER<VALUE>& value);                                       \
private:                                                                                             \
    QMutex mMutex##NAME;                                                                             \
    CONTAINER<VALUE> m##NAME = CONTAINER<VALUE>();

// continer : QMap, QHash ....
#define REGISTER_WRITABLE_PROPERTY_CONTAINER(CONTAINER, KEY, VALUE, NAME, NOTI)                           \
    Q_PROPERTY(CONTAINER<KEY, VALUE> NAME READ read##NAME WRITE write##NAME NOTIFY signal##NAME##Changed) \
public:                                                                                                   \
    inline void clear##NAME() {                                                                           \
        m##NAME.clear();                                                                                  \
    }                                                                                                     \
    inline int isSize##NAME() const {                                                                     \
        return m##NAME.size();                                                                            \
    }                                                                                                     \
    inline VALUE get##NAME(const KEY& key) const {                                                        \
        return m##NAME[key];                                                                              \
    }                                                                                                     \
    inline CONTAINER<KEY, VALUE> read##NAME() const {                                                     \
        return m##NAME;                                                                                   \
    }                                                                                                     \
public Q_SLOTS:                                                                                           \
    inline void set##NAME(const KEY& key, const VALUE& value) {                                           \
        QMutexLocker lock(&mMutex##NAME);                                                                 \
        if (m##NAME[key] != value) {                                                                      \
            m##NAME[key] = value;                                                                         \
            if (NOTI) {                                                                                   \
                emit signal##NAME##ValueChanged(key, value);                                              \
                emit signal##NAME##Changed(m##NAME);                                                      \
            }                                                                                             \
        }                                                                                                 \
    }                                                                                                     \
    inline void write##NAME(const CONTAINER<KEY, VALUE>& value) {                                         \
        QMutexLocker lock(&mMutex##NAME);                                                                 \
        if (m##NAME != value) {                                                                           \
            m##NAME = value;                                                                              \
            if (NOTI) {                                                                                   \
                emit signal##NAME##Changed(m##NAME);                                                      \
            }                                                                                             \
        }                                                                                                 \
    }                                                                                                     \
Q_SIGNALS:                                                                                                \
    void signal##NAME##Changed(const CONTAINER<KEY, VALUE>& value);                                       \
    void signal##NAME##ValueChanged(const KEY& key, const VALUE& value);                                  \
private:                                                                                                  \
    QMutex mMutex##NAME;                                                                                  \
    CONTAINER<KEY, VALUE> m##NAME = CONTAINER<KEY, VALUE>();

// enum
// TO_ENUM(private, PrivateEnum, Enum1, Enum2, Enum3)
// TO_ENUM(public, PublicEnum, Enum1, Enum2, Enum3)
#define TO_ENUM(ACCESS, NAME, ...) \
    ACCESS:                        \
    enum {                         \
        NAME##Invalid = 0,         \
        NAME##__VA_ARGS__,         \
    };

// enum type
#define TO_ENUM_TYPE(ACCESS, NAME, ...) \
    ACCESS:                             \
    enum NAME {                         \
        NAME##Invalid = 0,              \
        NAME##__VA_ARGS__,              \
    };

// enum class
#define TO_ENUM_CLASS(NAME, TYPE, ...) \
    class NAME : public QObject {      \
        Q_GADGET                       \
    public:                            \
        enum TYPE { __VA_ARGS__ };     \
        Q_ENUMS(TYPE)                  \
    };

// button -> connect() : signal - slot
#define INIT_BUTTON_WIEDGET(info, widget, index)                       \
    if (index >= 0) {                                                  \
        info.insert(widget->objectName(), index);                      \
        connect(widget, SIGNAL(pressed()), this, SLOT(slotPressed())); \
    }                                                                  \
    connect(widget, SIGNAL(clicked()), this, SLOT(slotClicked()));

inline QString APP_PWD() {
    QString pwd = QApplication::applicationDirPath();  // .toLatin1().data()
    return pwd;
}

inline QString GET_DATE_TIME() {
    return QString("%1_%2").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")).arg(QTime::currentTime().msec());
}

template <typename T>
inline int ARRAY_SIZE(T array) {
    return (sizeof(array) / sizeof(array[0]));
}

template <typename T1, typename T2>
inline void SWAP(T1& a, T2& b) {
    (a) ^= (b) ^= (a) ^= (b);
}

inline void BOOL_REVERSE(bool& value) {
    (value = (value) ? (false) : (true));
}

template <typename T>
inline void LIMIT(T& value, const T& min, const T& max) {
    if constexpr (std::is_same_v<T, QString>) {
        value = (value < min) ? (min) : ((value >= max) ? (max) : (value));
    } else if constexpr (std::is_same_v<T, int>) {
        value = (value < min) ? (min) : ((value >= max) ? (max - 1) : (value));
    } else {
    }
}

template <typename T>
inline void LIMIT_P(T& value, const T& gap, const T& min, const T& max) {
    value += gap;
    LIMIT(value, min, max);
}

template <typename T>
inline void LIMIT_M(T& value, const T& gap, const T& min, const T& max) {
    value -= gap;
    LIMIT(value, min, max);
}

template <typename T>
inline void REVOLVE(T& value, const T& min, const T& max) {
    value = (value < min) ? (max - 1) : ((value >= max) ? (min) : (value));
}

template <typename T>
inline void REVOLVE_P(T& value, const T& gap, const T& min, const T& max) {
    value += gap;
    REVOLVE(value, min, max);
}

template <typename T>
inline void REVOLVE_M(T& value, const T& gap, const T& min, const T& max) {
    value -= gap;
    REVOLVE(value, min, max);
}

template <typename T1, typename T2, typename T3>
inline void SET_PROPERTY(const T1& widget, const T2& name, const T3& value) {
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

// template <typename T1>
// inline void deleteWidget(T1* widget, const bool& clearConnect = false) {
//     if (widget) {
//         if (clearConnect) {
//             disconnect(widget);
//         }
//         delete widget;
//         widget = nullptr;
//     }
// }

template <typename T1, typename T2>
inline T1 getRemoved(T1& origin, const T2& toRemove) {
    T1 ret = origin;
    if constexpr (std::is_same_v<T2, QString>) {
        ret.remove(toRemove);
    } else if constexpr (std::is_same_v<T2, QStringList>) {
        for (const auto& removeObj : toRemove) {
            ret.remove(removeObj);
        }
    } else {
    }
    origin = ret;
    return ret;
}

template <typename T1, typename T2>
inline T1 convertDataType(const T2& str) {
    if constexpr (std::is_same_v<T1, T2>) {
        return str;
    } else if constexpr ((std::is_same_v<T1, QString>) && (std::is_same_v<T2, QVariant>)) {
        return str.toString();
    } else if constexpr ((std::is_same_v<T1, QString>) && (std::is_same_v<T2, int>)) {
        return QString::number(str);
    } else if constexpr ((std::is_same_v<T1, QString>) && (std::is_same_v<T2, QStringList>)) {
        return str.join(", ");
    } else if constexpr ((std::is_same_v<T1, QString>) && (std::is_same_v<T2, QVariantList>)) {
        QStringList stringList;
        for (const auto& variant : str) {
            stringList.append(variant.toString());
        }
        return stringList.join(", ");
    } else if constexpr ((std::is_same_v<T1, QVariant>) && (std::is_same_v<T2, QString>)) {
        return QVariant(str);
    } else if constexpr ((std::is_same_v<T1, QVariant>) && (std::is_same_v<T2, QStringList>)) {
        return QVariant(str.join(", "));
    } else if constexpr ((std::is_same_v<T1, QStringList>) && (std::is_same_v<T2, QVariantList>)) {
        QStringList stringList;
        for (const auto& variant : str) {
            stringList.append(variant.toString());
        }
        return stringList;
    } else if constexpr ((std::is_same_v<T1, QVariantList>) && (std::is_same_v<T2, QStringList>)) {
        QVariantList variantList;
        for (const auto& s : str) {
            variantList.append(QVariant(s));
        }
        return variantList;
    } else {
        static_assert(!std::is_same_v<T1, T2>, "Not Support Type : convertDataType");
        return T1();
    }
}

template <typename T1, typename T2>
inline bool isContainsString(const T1& orign, const T2& contains, const bool& checkStartsWith = true) {
    QString originStr = convertDataType<QString, T1>(orign);
    QString containsStr = convertDataType<QString, T2>(contains);
    bool state = ((checkStartsWith) ? (originStr.trimmed().startsWith(containsStr)) : (originStr.contains(containsStr)));
    if ((originStr.isEmpty()) || (containsStr.isEmpty())) {
        state = false;
    }
    // qDebug() << "isContainsString :" << state << originStr << containsStr;
    return state;
}

template <typename T1, typename T2>
inline bool isCompareString(const T1& orign, const T2& contains) {
    QString originStr = convertDataType<QString, T1>(orign);
    QString containsStr = convertDataType<QString, T2>(contains);
    return (originStr.compare(containsStr, Qt::CaseInsensitive) == 0);
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
            qDebug() << "[CheckTime]" << info.toLatin1().data() << ":" << mElapsedTimer.elapsed() << "ms\n";
        } else {
            qDebug() << "[CheckTime] :" << mElapsedTimer.elapsed() << "ms\n";
        }
    }
    qint64 getTime() {
        return mElapsedTimer.elapsed();
    }

private:
    QElapsedTimer mElapsedTimer;
};

class FileInfo {
public:
    static QStringList isFileListInfo(const QString& path, const QString& fileExtesion, QFileInfoList& fileList) {
        QString currentPath = path;
        if (currentPath.size() == 0) {
            currentPath.append(QApplication::applicationDirPath());
            currentPath.append("/SFC");
        }
        return readFileListInfo(currentPath, fileExtesion, fileList);
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
    static QByteArray readFileByteArray(const QString& filePath) {
        return readFileByteArrayInfo(filePath);
    }
    static int writeFile(const QString& filePath, const QString& str, const bool& append) {
        return writeFileData(filePath, str, append);
    }
    static bool deleteFile(const QString& path, const QString& deleteInfo) {
        return deleteFileData(path, deleteInfo);
    }

private:
    static QStringList readFileListInfo(const QString& path, const QString& fileExtesion, QFileInfoList& fileList) {
        QStringList fileNames = QStringList();
        QDir directory(path);
        directory.setFilter(QDir::Files | QDir::NoDotAndDotDot);
        fileList = directory.entryInfoList();
        // qDebug() << "\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
        // qDebug() << "path :" << path << ", Count :" << fileList.size();
        for (const QFileInfo& file : fileList) {
            QStringList tempList = file.fileName().split(".");
            QString currentExtesion = QString(".%1").arg((tempList.size() == 0) ? ("") : (tempList.at(tempList.size() - 1)));
            bool validExtension = (currentExtesion.compare(fileExtesion) == 0);
            if (validExtension == false) {
                continue;
            }
            fileNames.append(file.fileName());
            // qDebug() << "--------------------------------------------------------------------------------------------";
            // qDebug() << "File Name :" << file.fileName() << ", Size :" << file.size() << "byte";
            // qDebug() << "File Path :" << file.filePath();
            // // qDebug() << "File Created: " << file.created().toString();
        }
        // qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
        return fileNames;
    }
    static QStringList readFileDataInfo(const QString& filePath, bool& openError) {
        QStringList fileContent = QStringList();
        QFile file(filePath);
        openError = false;
        // qDebug() << "readFileDataInfo :" << filePath;
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
    static QByteArray readFileByteArrayInfo(const QString& filePath) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text) == false) {
            qDebug() << "Fail to open file: " << filePath;
            return QByteArray();
        }
        QByteArray readData = file.readAll();
        file.close();
        return readData;
    }
    static int writeFileData(const QString& filePath, const QString& str, const bool& append) {
        // qDebug() << "writeFileData :" << filePath;
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
        for (const auto& data : readData) {
            QString lineData = QString("%1\n").arg(data);
            out << lineData;
        }
        out << str;
        file.close();
        return str.size();
    }
    static bool deleteFileData(const QString& path, const QString& deleteInfo) {
        QFile file;
        if (deleteInfo.contains("*.")) {
            QString fileExtenstion = deleteInfo;
            fileExtenstion.remove("*");
            QFileInfoList fileList = QFileInfoList();
            for (const auto& fileName : isFileListInfo(path, fileExtenstion, fileList)) {
                file.setFileName(QString("%1/%2").arg(path).arg(fileName));
                bool result = file.remove();
                // qDebug() << ((result) ? ("Delete Success :") : ("1 Delete Fail :")) << file.fileName();
            }
            return true;
        } else {
            file.setFileName(QString("%1/%2").arg(path).arg(deleteInfo));
            bool result = file.remove();
            // qDebug() << ((result) ? ("Delete Success :") : ("2 Delete Fail :")) << file.fileName();
            return result;
        }
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

            if (process.waitForStarted()) {  // if (process.waitForFinished()) {
                while (process.waitForReadyRead()) {
                    QString readAllData = process.readAll();
                    QString logData = QString();
                    for (const QString& data : readAllData) {
                        if (data.compare("\n") == 0) {
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
        // qDebug() << "ExcuteProgram::start() ->" << ((result == 0) ? ("Success :") : ("Fail :")) << result;
        return (result == 0);
    }

private:
    bool mUseProcess = true;
};

class ExcuteProgramThread : public QObject {
    Q_OBJECT

public:
    explicit ExcuteProgramThread(const bool& useProcess) {
        mUseProcess = useProcess;
    }
    ~ExcuteProgramThread() {
        // join();
        qDebug() << "~ExcuteProgramThread()";
    }
    void setCommandInfo(const QString& cmd, const QString& arg = QString()) {
        QMutexLocker lock(&mMutex);
        mCommand = cmd;
        if (arg.size() > 0) {
            mCommand.append(arg);
        }
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

        emit signalExcuteProgramInfo(true, true);

        if (mUseProcess) {
            QStringList splitCmd = mCommand.split(" ");

            // mProcess.setProcessChannelMode(QProcess::MergedChannels);
            // connect(&mProcess, &QProcess::readyReadStandardOutput, [=]() {
            //     QByteArray newData = mProcess.readAllStandardOutput();
            //     QString log = QString::fromUtf8(newData);
            //     qDebug() << "\t LogData :" << log;
            // });

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
            // disconnect(&mProcess, &QProcess::finished, nullptr, nullptr);
            // connect(&mProcess, &QProcess::finished, [=](int exitCode, QProcess::ExitStatus exitStatus) {
            //     qDebug() << "\t ExcuteProgramThread::finished()  -> Exit :" << exitCode << exitStatus;
            // });

            if (mProcess.waitForStarted()) {  // waitForStarted, waitForFinished
                while (mProcess.waitForReadyRead()) {
                    QString readAllData = mProcess.readAll();
                    QString logData = QString();
                    for (const QString& data : readAllData) {
                        if (data.compare("\n") == 0) {
                            // qDebug() << "\t Log :" << logData;
                            log.append(logData);
                            logData.clear();
                        } else {
                            logData.append(data);
                        }
                    }
                }
                mProcess.waitForFinished();
            } else {
                result = (-1);
            }
        } else {
            result = system(mCommand.toLatin1());
        }
        qDebug() << "\t ExcuteProgramThread::runThread() -> Result :" << result << ", Command :" << mCommand;
        emit signalExcuteProgramInfo(false, (result == 0));
    }

signals:
    void signalExcuteProgramInfo(const bool& start, const bool& result);

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
    CheckLib() {
    }
    ~CheckLib() {
        // join();
    }
    void setLibInfo(const QStringList& libInfo) {
        for (const auto& info : libInfo) {
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
        QString filePath = QString("%1/CheckLib.txt").arg(APP_PWD());
        int count = 0;
        for (const auto& info : mCheckInfo) {
            ExcuteProgram process(false);
            QString appendStr = QString((count == 0) ? (">") : (">>"));
            QString cmd = QString(
                              "pip list | grep "
                              "%1"
                              " %2 %3")
                              .arg(info.first)
                              .arg(appendStr)
                              .arg(filePath);
            QStringList log;
            if (process.start(cmd, log)) {
                count++;
            }
        }
        QStringList readDataList = FileInfo::readFile(filePath);
        QString readData = QString();
        for (const auto& data : readDataList) {
            readData.append(data);
        }
        for (const auto& info : mCheckInfo) {
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
    explicit FileSystemWatcherThread(const QString& filePath, const int& checkCount = 0) {
        mWatcherFile = filePath;
        mCheckCount = checkCount;
    }
    ~FileSystemWatcherThread() {
        QMutexLocker lock(&mMutex);
        mWatcher.removePath(mWatcherFile);
        mWatcherFile.clear();
        qDebug() << "~FileSystemWatcherThread()";
    }
    void start() {
        this->moveToThread(mThread);
        connect(mThread, &QThread::finished, this, &QObject::deleteLater);
        connect(mThread, &QThread::started, this, &FileSystemWatcherThread::runThread);
        mThread->start();

        connect(&mWatcher, &QFileSystemWatcher::fileChanged, [=](const QString& path) {
            bool readState = (mCheckCount == 0) ? (true) : ((mReadCount++ % mCheckCount) == 0);
            readFile(false, path, readState);
        });
    }
    void readFinalData() {
        if (mCheckCount > 0) {
            readFile(false, mWatcherFile, true);
            emit signalWatcherFileState(1);
        }
    }

private:
    void readFile(const bool& init, const QString& path, const bool& readState) {
        if (readState == false) {
            return;
        }
        QPair<bool, QStringList> readData = ivis::common::FileInfo::readFileData(path);
        if (readData.first) {
            emit signalWatcherFileReadError(mWatcherFile);
        } else {
            if (readData.second.size() > 0) {
                emit signalWatcherFileDataChanged(init, readData.second);
            }
        }
    }
    void runThread() {
        while (mCount < 10) {
            if (mWatcherFile.size() == 0) {
                qDebug() << "\t [Stop]   Watcher file :" << mCount << mWatcherFile;
                break;
            }

            if (mWatcher.addPath(mWatcherFile)) {
                qDebug() << "\t [Success] Watcher file :" << mCount << mWatcherFile;
                mCount = 0;
                readFile(true, mWatcherFile, true);
                break;
            } else {
                qDebug() << "\t [Fail]   Watcher file :" << mCount << mWatcherFile;
                mCount++;
                QThread::msleep(1000);
            }
        }
        if (mCount >= 10) {
            QMutexLocker lock(&mMutex);
            emit signalWatcherFileState(-1);
        }
    }
    void join() {
        // if ((mThread->thread() != nullptr) && (mThread->thread() != QThread::currentThread())) {
        if (mThread->isRunning()) {
            qDebug() << "FileSystemWatcherThread - terminate";
            mThread->quit();
            mThread->wait();
        }
    }

signals:
    void signalWatcherFileState(const int& state);
    void signalWatcherFileReadError(const QString& errorFile);
    void signalWatcherFileDataChanged(const bool& init, const QStringList& data);

private:
    QThread* mThread = new QThread();
    QMutex mMutex;
    QFileSystemWatcher mWatcher;
    QString mWatcherFile = QString();
    int mCount = 0;
    int mReadCount = 0;
    int mCheckCount = 0;
};

}  // end of namespace common
}  // end of namespace ivis

#endif  // COMMON_UTIL_H
