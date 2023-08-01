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
static QVariant gPopupData = QVariant();

class Popup : public QObject {
    Q_OBJECT

public:
    static PopupType isPopupType() {
        return gPopupType;
    }
    static QVariant isPopupData() {
        return gPopupData;
    }
    static PopupButton drawPopup(const PopupType& popupType, AbstractHandler* handler, const QVariant& value = QVariant()) {
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
                    button = drawPopupOpen(handler, infoData.at(0).toString(), infoData.at(1).toString());
                }
                break;
            }
            case PopupType::Save : {
                button = drawPopupSave(handler);
                break;
            }
            case PopupType::DefaultPath : {
                QVariantList infoData = value.toList();
                if (infoData.size() == 2) {
                    button = drawPopupDevPath(handler, infoData.at(0).toString(), infoData.at(1).toString());
                }
                break;
            }
            default : {
                break;
            }
        }
        setPopupType(popupType);
        qDebug() << "Popup::drawPopup() -> Button :" << static_cast<int>(button) << ", Data :" << isPopupData();
        return button;
    }


private:
    static void setPopupType(const PopupType& type) {
        gPopupType = type;
    }
    static void setPopupData(const QVariant& data) {
        gPopupData = data;
    }
    static PopupButton drawPopupNoraml(AbstractHandler* handler, const QString& title, const QString& tip) {
        PopupButton button = PopupButton::Invalid;
        if (handler) {
            QMessageBox::about(handler->getScreen(), title, tip);
            button = PopupButton::OK;
        }
        return button;
    }
    static PopupButton drawPopupSelect(AbstractHandler* handler, const QVariant& textList) {
        PopupButton buttypType = PopupButton::Invalid;
        QMessageBox selectBox(handler->getScreen());
        QVariantList list = textList.toList();
        QMap<PopupButton, QPushButton*> button = QMap<PopupButton, QPushButton*>();
        if (list.size() == 5) {
            selectBox.setText(list[0].toString());
            selectBox.setInformativeText(list[1].toString());
            button[PopupButton::OK] = selectBox.addButton(list[2].toString(), QMessageBox::ActionRole);
            button[PopupButton::Discard] = selectBox.addButton(list[3].toString(), QMessageBox::ActionRole);
            button[PopupButton::Cancel] = selectBox.addButton(list[4].toString(), QMessageBox::ActionRole);
        } else {
            return PopupButton::Invalid;
        }
#if 1
        selectBox.exec();
        if (selectBox.clickedButton() == button[PopupButton::OK]) {
            buttypType = PopupButton::OK;
        } else if (selectBox.clickedButton() == button[PopupButton::Discard]) {
            buttypType = PopupButton::Discard;
        } else if (selectBox.clickedButton() == button[PopupButton::Cancel]) {
            buttypType = PopupButton::Cancel;
        } else {
            return PopupButton::Invalid;
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
        return buttypType;
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
    static PopupButton drawPopupOpen(AbstractHandler* handler, const QString& title, const QString& path) {
        PopupButton button = PopupButton::Invalid;
        if (handler) {
            QString filePath = QFileDialog::getOpenFileName(handler->getScreen(),
                                        title,
                                        path,
                                        QString("Excel (*.xls *.xlsx);;All files (*.*)"));
            if (filePath.size() > 0) {
                setPopupData(filePath);
                button = PopupButton::OK;
            }
        }
        return button;
    }
    static PopupButton drawPopupSave(AbstractHandler* handler) {
        PopupButton button = PopupButton::Invalid;
        if (handler) {
            QFileDialog dialog(qobject_cast<QWidget*>(handler->getScreen()));
            dialog.setWindowModality(Qt::WindowModal);
            dialog.setAcceptMode(QFileDialog::AcceptSave);
            if (dialog.exec() == QDialog::Accepted) {
                setPopupData(dialog.selectedFiles().first());
                button = PopupButton::OK;
            }
        }
        return button;
    }
    static PopupButton drawPopupDevPath(AbstractHandler* handler, const QString& title, const QString& path) {
        PopupButton button = PopupButton::Invalid;
        if (handler) {
            QString defaultPath = QFileDialog::getExistingDirectory(handler->getScreen(),
                                            title,
                                            path,
                                            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
            if (defaultPath.size() == 0) {
                defaultPath = QApplication::applicationDirPath();
            }
            setPopupData(defaultPath);
            button = PopupButton::OK;
        }
        return button;
    }
};








#endif  // COMMON_POPUP_H
