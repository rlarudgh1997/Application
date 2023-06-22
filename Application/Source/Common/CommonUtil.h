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
    static QFileInfoList isFileListInfo(const QString& path = QString()) {
        QFileInfoList fileList;
        if (path.size() == 0) {
            readFileInfo(fileList, "/SFC");
        } else {
            readFileInfo(fileList, path);
        }
        return fileList;
    }
    static void isFileListInfo(QFileInfoList& fileList, const QString& path = QString()) {
        if (path.size() == 0) {
            readFileInfo(fileList, "/SFC");
        } else {
            readFileInfo(fileList, path);
        }
    }
    static void parsingFile() {
    }

private:
    static QFileInfoList readFileInfo(QFileInfoList& fileList, QString path) {
        path.push_front(QApplication::applicationDirPath());

        QDir directory(path);
        directory.setFilter(QDir::Files | QDir::NoDotAndDotDot);
        fileList = directory.entryInfoList();

        qDebug() << "\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
        qDebug() << "path :" << path << ", length :" << fileList.size();
        foreach(const QFileInfo& file, fileList) {
            qDebug() << "--------------------------------------------------------------------------------------------";
            qDebug() << "File Name :" << file.fileName() << ", Size :" << file.size() << "byte";
            qDebug() << "File Path :" << file.filePath();
            // qDebug() << "File Created: " << file.created().toString();
        }
        qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
        return fileList;
    }
};








#endif  // COMMON_FILE_H
