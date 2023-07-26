#ifndef COMMON_POPUP_H
#define COMMON_POPUP_H

#include <QObject>
#include <QDebug>

#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>

#include "AbstractHandler.h"
#include "CommonEnum.h"
#include "CommonResource.h"


class Popup : public QObject {
    Q_OBJECT

public:
    static void drawPopupAbout(AbstractHandler* handler) {
        if (handler) {
            QMessageBox::about(qobject_cast<QWidget*>(handler->getScreen()),
                            STRING_POPUP_ABOUT,
                            STRING_POPUP_ABOUT_TIP);
        }
    }
    static void drawPopupAboutQt(AbstractHandler* handler) {
        if (handler) {
            // QApplication::aboutQt();
            QMessageBox::aboutQt(qobject_cast<QWidget*>(handler->getScreen()));
        }
    }
    static void drawPopupSave(AbstractHandler* handler, const int& eventType) {
        if (handler) {
            QFileDialog dialog(qobject_cast<QWidget*>(handler->getScreen()));
            dialog.setWindowModality(Qt::WindowModal);
            dialog.setAcceptMode(QFileDialog::AcceptSave);
            if (dialog.exec() == QDialog::Accepted) {
                createSignal(handler, eventType, dialog.selectedFiles().first());
            }
        }
    }
    static void drawPopupOpen(AbstractHandler* handler, const int& eventType, const QString& path) {
        if (handler) {
            QString filePath = QFileDialog::getOpenFileName(qobject_cast<QWidget*>(handler->getScreen()),
                                        STRING_FILE_OPEN,
                                        path,
                                        QString("Excel (*.xls *.xlsx);;All files (*.*)"));
            if (filePath.size() > 0) {
                createSignal(handler, eventType, filePath);
            }
        }
    }
    static void drawPopupDevPath(AbstractHandler* handler, const int& eventType, const QString& path) {
        if (handler) {
            QString defaultPath = QFileDialog::getExistingDirectory(qobject_cast<QWidget*>(handler->getScreen()),
                                            STRING_DEFAULT_PATH,
                                            path,
                                            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
            if (defaultPath.size() == 0) {
                defaultPath = QApplication::applicationDirPath();
            }
            createSignal(handler, eventType, defaultPath);
        }
    }


private:
    static void createSignal(AbstractHandler* handler, const int& type, const QVariant& value) {
        if (handler) {
            emit handler->signalHandlerEvent(type, value);
        }
    }
};








#endif  // COMMON_POPUP_H
