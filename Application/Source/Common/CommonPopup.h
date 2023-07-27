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


enum class PopupType {
    About = 0,
    AboutQt,
    Open,
    Save,
    DefaultPath,
};

class Popup : public QObject {
    Q_OBJECT

public:
    static void drawPopup(const PopupType& popupType,
                            AbstractHandler* handler,
                            const int& eventType = 0,
                            const QVariant& value = QVariant()) {
        switch (popupType) {
            case PopupType::About : {
                drawPopupAbout(handler);
                break;
            }
            case PopupType::AboutQt : {
                drawPopupAboutQt(handler);
                break;
            }
            case PopupType::Open : {
                drawPopupOpen(handler, eventType, value.toString());
                break;
            }
            case PopupType::Save : {
                drawPopupSave(handler, eventType);
                break;
            }
            case PopupType::DefaultPath : {
                drawPopupDevPath(handler, eventType, value.toString());
                break;
            }
            default : {
                break;
            }
        }
    }


private:
    static void createSignal(AbstractHandler* handler, const int& eventType, const QVariant& value) {
        if (handler) {
            emit handler->signalHandlerEvent(eventType, value);
        }
    }
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
};








#endif  // COMMON_POPUP_H
