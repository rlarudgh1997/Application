#ifndef COMMON_FILE_H
#define COMMON_FILE_H

#include <QObject>
#include <QApplication>
#include <QWidget>
#include <QFile>
#include <QFileInfoList>
#include <QDir>
#include <QMap>
#include <QElapsedTimer>
#include <QMutex>
#include <QMutexLocker>
#include <QDebug>


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






class CheckTimer : public QObject {
    Q_OBJECT

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

class FileInfo : public QObject {
    Q_OBJECT

public:
    static QStringList isFileListInfo(const QString& path, QFileInfoList fileList = QFileInfoList()) {
        QString currentPath = path;
        if (currentPath.size() == 0) {
            currentPath.append(QApplication::applicationDirPath());
            currentPath.append("/SFC");
        }
        return readFileInfo(currentPath, fileList);
    }
    static QStringList parsingFile(const QString& filePath) {
        return readFile(filePath);
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
    static QStringList readFile(const QString& filePath) {
        QStringList fileContent = QStringList();
        QFile file(filePath);
        // qDebug() << "filePath :" << filePath;
        if (file.open(QFile::ReadOnly|QFile::Text)) {
            QTextStream readFile(&file);
            while (!readFile.atEnd()) {
                QString data = readFile.readLine();
                fileContent.append(data);
                // qDebug() << "Data :" << data;
            }
            file.close();
        } else {
            if (file.exists()) {
                qDebug() << "Fail to open no exist file";
            } else {
                qDebug() << "Fail to open error";
            }
        }
        return fileContent;
    }
};








#endif  // COMMON_FILE_H
