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


namespace ivis {
namespace common {


enum class PopupType {
    Invalid = 0,
    About,
    AboutQt,
    Exit,
    Open,
    Save,
    DefaultPath,
    OpenFail,
    NoInstallLib,
    NowInstalling,
    InstallComplete,
    CellSelectError,
};

enum class PopupButton {
    Invalid = 0,
    OK,
    Discard,
    Cancel,
    Confirm,
    Install,
};


static PopupType gPopupType = PopupType::Invalid;
static QVariant gPopupData = QVariant();

class Popup : public QObject {
    Q_OBJECT

public:
    static PopupButton drawPopup(const PopupType& popupType, AbstractHandler* handler, QVariant& popupData,
                                    const QVariant& value = QVariant()) {
        PopupButton button = PopupButton::Invalid;
        switch (popupType) {
            case PopupType::About :
            case PopupType::OpenFail :
            case PopupType::NowInstalling :
            case PopupType::InstallComplete : {
                QVariantList infoData = value.toList();
                if (infoData.size() == 2) {
                    button = drawPopupNoraml(handler, infoData.at(0).toString(), infoData.at(1).toString());
                }
                break;
            }
            case PopupType::Exit :
            case PopupType::NoInstallLib : {
                button = drawPopupSelect(popupType, handler, value);
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
        popupData = isPopupData();
        qDebug() << "Popup::drawPopup() -> Button :" << static_cast<int>(button) << ", Data :" << isPopupData();
        return button;
    }


private:
    static PopupType isPopupType() {
        return gPopupType;
    }
    static QVariant isPopupData() {
        qDebug() << "Popup::isPopupData() ->" << gPopupData;
        return gPopupData;
    }
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
    static PopupButton drawPopupSelect(const PopupType& popupType, AbstractHandler* handler, const QVariant& textList) {
        PopupButton buttonType = PopupButton::Invalid;
        QMessageBox selectBox(handler->getScreen());
        QVariantList list = textList.toList();
        QMap<PopupButton, QPushButton*> button = QMap<PopupButton, QPushButton*>();

        if ((popupType == PopupType::Exit) && (list.size() == 5)) {
            selectBox.setText(list[0].toString());
            selectBox.setInformativeText(list[1].toString());
            button[PopupButton::OK] = selectBox.addButton(list[2].toString(), QMessageBox::ActionRole);
            button[PopupButton::Discard] = selectBox.addButton(list[3].toString(), QMessageBox::ActionRole);
            button[PopupButton::Cancel] = selectBox.addButton(list[4].toString(), QMessageBox::ActionRole);
            connect(button[PopupButton::OK], &QPushButton::clicked, [&]() {
                buttonType = PopupButton::OK;
            });
            connect(button[PopupButton::Discard], &QPushButton::clicked, [&]() {
                buttonType = PopupButton::Discard;
            });
            connect(button[PopupButton::Cancel], &QPushButton::clicked, [&]() {
                buttonType = PopupButton::Cancel;
            });
        } else if ((popupType == PopupType::NoInstallLib) && (list.size() == 4)) {
            selectBox.setText(list[0].toString());
            selectBox.setInformativeText(list[1].toString());
            button[PopupButton::Install] = selectBox.addButton(list[2].toString(), QMessageBox::ActionRole);
            button[PopupButton::Confirm] = selectBox.addButton(list[3].toString(), QMessageBox::ActionRole);
            connect(button[PopupButton::Install], &QPushButton::clicked, [&]() {
                buttonType = PopupButton::Install;
            });
            connect(button[PopupButton::Confirm], &QPushButton::clicked, [&]() {
                buttonType = PopupButton::Confirm;
            });
        } else {
        }

        if (button.size() == 0) {
            return PopupButton::Invalid;
        }

        selectBox.exec();
        return buttonType;
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
            QFileDialog dialog(handler->getScreen());
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
            QString showPath = path;
            if (showPath.size() == 0) {
                showPath = QApplication::applicationDirPath();
            }
            QString defaultPath = QFileDialog::getExistingDirectory(handler->getScreen(),
                                            title,
                                            showPath,
                                            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
            if (defaultPath.size() > 0) {
                setPopupData(defaultPath);
                button = PopupButton::OK;
            }
        }
        return button;
    }
};





}  // end of namespace common
}  // end of namespace ivis



#endif  // COMMON_POPUP_H
