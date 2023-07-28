#ifndef COMMON_POPUP_H
#define COMMON_POPUP_H

#include <QObject>
#include <QDebug>

#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QPushButton>

#include "AbstractHandler.h"
#include "CommonEnum.h"


enum class PopupType {
    Invalid = 0,
    About,
    AboutQt,
    Exit,
    Open,
    Save,
    DefaultPath,
    OpenFail,
};

enum class PopupButton {
    Invalid = 0,
    OK,
    Discard,
    Cancel,
};

static PopupType gPopupType = PopupType::Invalid;

class Popup : public QObject {
    Q_OBJECT

public:
    static PopupType isPopupType() {
        return gPopupType;
    }
    static PopupButton drawPopup(const PopupType& popupType, AbstractHandler* handler,
                            const int& eventType = 0, const QVariant& value = QVariant()) {
        PopupButton button = PopupButton::Invalid;
        switch (popupType) {
            case PopupType::About :
            case PopupType::OpenFail : {
                QVariantList infoData = value.toList();
                if (infoData.size() == 2) {
                    button = drawPopupNoraml(handler, infoData.at(0).toString(), infoData.at(1).toString());
                }
                break;
            }
            case PopupType::Exit : {
                button = drawPopupSelect(handler, value);
                break;
            }
            case PopupType::AboutQt : {
                button = drawPopupAboutQt(handler);
                break;
            }
            case PopupType::Open : {
                QVariantList infoData = value.toList();
                if (infoData.size() == 2) {
                    button = drawPopupOpen(handler, eventType, infoData.at(0).toString(), infoData.at(1).toString());
                }
                break;
            }
            case PopupType::Save : {
                button = drawPopupSave(handler, eventType);
                break;
            }
            case PopupType::DefaultPath : {
                QVariantList infoData = value.toList();
                if (infoData.size() == 2) {
                    button = drawPopupDevPath(handler, eventType, infoData.at(0).toString(), infoData.at(1).toString());
                }
                break;
            }
            default : {
                break;
            }
        }
        setPopuptype(popupType);
        qDebug() << "Popup::drawPopup() -> Button :" << static_cast<int>(button);
        return button;
    }


private:
    static void setPopuptype(const PopupType& type) {
        gPopupType = type;
    }
    static void createSignal(AbstractHandler* handler, const int& eventType, const QVariant& value) {
        if (handler) {
            emit handler->signalHandlerEvent(eventType, value);
        }
    }
    static PopupButton drawPopupNoraml(AbstractHandler* handler, const QString& title, const QString& tip) {
        PopupButton button = PopupButton::Invalid;
        if (handler) {
            QMessageBox::about(handler->getScreen(), title, tip);
            button = PopupButton::OK;
        }
        return button;
    }
    static PopupButton drawPopupSelect(AbstractHandler* handler, const QVariant& value = QVariant()) {
        PopupButton button = PopupButton::Invalid;
        QMessageBox selectBox(handler->getScreen());
        QPushButton* save = selectBox.addButton(QString("저장"), QMessageBox::ActionRole);
        QPushButton* discard = selectBox.addButton(QString("저장 안함"), QMessageBox::ActionRole);
        QPushButton* cancel = selectBox.addButton(QString("취소"), QMessageBox::ActionRole);
        selectBox.setText("파일 저장");
        selectBox.setInformativeText("변경 내용을 저장하시겠습니까?");

#if 1
        selectBox.exec();
        if (selectBox.clickedButton() == save) {
            button = PopupButton::OK;
        } else if (selectBox.clickedButton() == discard) {
            button = PopupButton::Discard;
        } else if (selectBox.clickedButton() == cancel) {
            button = PopupButton::Cancel;
        } else {
        }
#else
        selectBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        selectBox.setDefaultButton(QMessageBox::Save);
        switch (selectBox.exec()) {
            case QMessageBox::Save : {
                qDebug() << "File Save";
                break;
            }
            case QMessageBox::Discard : {
                qDebug() << "Program Exit";
                break;
            }
            case QMessageBox::Cancel : {
                qDebug() << "Cancel";
                break;
            }
            default : {
                break;
            }
        }
#endif
        return button;
    }
    static PopupButton drawPopupAboutQt(AbstractHandler* handler) {
        PopupButton button = PopupButton::Invalid;
        if (handler) {
            // QApplication::aboutQt();
            QMessageBox::aboutQt(handler->getScreen());
            button = PopupButton::OK;
        }
        return button;
    }
    static PopupButton drawPopupOpen(AbstractHandler* handler, const int& eventType, const QString& title, const QString& path) {
        PopupButton button = PopupButton::Invalid;
        if (handler) {
            QString filePath = QFileDialog::getOpenFileName(handler->getScreen(),
                                        title,
                                        path,
                                        QString("Excel (*.xls *.xlsx);;All files (*.*)"));
            if (filePath.size() > 0) {
                qDebug() << "Open :" << filePath << "\n\n";
                createSignal(handler, eventType, filePath);
                button = PopupButton::OK;
            }
        }
        return button;
    }
    static PopupButton drawPopupSave(AbstractHandler* handler, const int& eventType) {
        PopupButton button = PopupButton::Invalid;
        if (handler) {
            QFileDialog dialog(qobject_cast<QWidget*>(handler->getScreen()));
            dialog.setWindowModality(Qt::WindowModal);
            dialog.setAcceptMode(QFileDialog::AcceptSave);
            if (dialog.exec() == QDialog::Accepted) {
                createSignal(handler, eventType, dialog.selectedFiles().first());
                button = PopupButton::OK;
            }
        }
        return button;
    }
    static PopupButton drawPopupDevPath(AbstractHandler* handler, const int& eventType,
                                            const QString& title, const QString& path) {
        PopupButton button = PopupButton::Invalid;
        if (handler) {
            QString defaultPath = QFileDialog::getExistingDirectory(handler->getScreen(),
                                            title,
                                            path,
                                            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
            if (defaultPath.size() == 0) {
                defaultPath = QApplication::applicationDirPath();
            }
            createSignal(handler, eventType, defaultPath);
            button = PopupButton::OK;
        }
        return button;
    }
};








#endif  // COMMON_POPUP_H
