#ifndef COMMON_FILE_H
#define COMMON_FILE_H

#include <QObject>
#include <QApplication>
#include <QWidget>



#include <QFile>
#include <QFileInfoList>
#include <QDir>
#include <QMap>

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



#define QML_WRITABLE_PROPERTY(type, name, notify) \
    Q_PROPERTY(type name READ get_##name WRITE set_##name NOTIFY name##Changed) \
    public: \
        type get_##name() const { \
            return m_##name ; \
        } \
    public Q_SLOTS: \
        void set_##name(const type& name) { \
            QMutexLocker lock(&m_mutex_##name); \
            if (m_##name != name) { \
                m_##name = name; \
                if (notify) { \
                    emit name##Changed(m_##name); \
                } \
            } \
        } \
    Q_SIGNALS: \
        void name##Changed(type name); \
    private: \
        QMutex m_mutex_##name; \
        type m_##name; \


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
        qDebug() << "filePath :" << filePath;
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
