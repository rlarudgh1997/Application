#ifndef COMMON_POPUP_H
#define COMMON_POPUP_H

#include <QObject>
#include <QDebug>

#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>

#include "AbstractHandler.h"
#include "CommonEnum.h"


enum class PopupType {
    About = 0,
    AboutQt,
    Open,
    Save,
    DefaultPath,
    OpenFail,
};

class Popup : public QObject {
    Q_OBJECT

public:
    static void drawPopup(const PopupType& popupType, AbstractHandler* handler,
                            const int& eventType = 0, const QVariant& value = QVariant()) {
        switch (popupType) {
            case PopupType::About :
            case PopupType::OpenFail : {
                QVariantList infoData = value.toList();
                if (infoData.size() == 2) {
                    drawPopupNoraml(handler, infoData.at(0).toString(), infoData.at(1).toString());
                }
                break;
            }
            case PopupType::AboutQt : {
                drawPopupAboutQt(handler);
                break;
            }
            case PopupType::Open : {
                QVariantList infoData = value.toList();
                if (infoData.size() == 2) {
                    drawPopupOpen(handler, eventType, infoData.at(0).toString(), infoData.at(1).toString());
                }
                break;
            }
            case PopupType::Save : {
                drawPopupSave(handler, eventType);
                break;
            }
            case PopupType::DefaultPath : {
                QVariantList infoData = value.toList();
                if (infoData.size() == 2) {
                    drawPopupDevPath(handler, eventType, infoData.at(0).toString(), infoData.at(1).toString());
                }
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
    static void drawPopupNoraml(AbstractHandler* handler, const QString& title, const QString& tip) {
        if (handler) {
            QMessageBox::about(qobject_cast<QWidget*>(handler->getScreen()), title, tip);
        }
    }
    static void drawPopupAboutQt(AbstractHandler* handler) {
        if (handler) {
            // QApplication::aboutQt();
            QMessageBox::aboutQt(qobject_cast<QWidget*>(handler->getScreen()));
        }
    }
    static void drawPopupOpen(AbstractHandler* handler, const int& eventType, const QString& title, const QString& path) {
        if (handler) {
            QString filePath = QFileDialog::getOpenFileName(qobject_cast<QWidget*>(handler->getScreen()),
                                        title,
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
    static void drawPopupDevPath(AbstractHandler* handler, const int& eventType, const QString& title, const QString& path) {
        if (handler) {
            QString defaultPath = QFileDialog::getExistingDirectory(qobject_cast<QWidget*>(handler->getScreen()),
                                            title,
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
