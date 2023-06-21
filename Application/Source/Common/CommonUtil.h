#ifndef COMMON_FILE_H
#define COMMON_FILE_H

#include <QObject>
#include <QApplication>

#include <QFile>
#include <QFileInfoList>
#include <QDir>
#include <QMap>

#include <QDebug>


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
