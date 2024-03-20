#ifndef COMMON_POPUP_H
#define COMMON_POPUP_H

#include <QObject>
#include <QDebug>

#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QPushButton>

// #include "CommonEnum.h"

namespace ivis {
namespace common {

enum class PopupType {
    Invalid = 0,

    About,
    AboutQt,
    OpenFolder,
    OpenError,
    DeleteFile,
    DeleteFileComplete,
    PathError,
    SettingPath,
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
#if 1
    static PopupButton drawPopup(const PopupType& popupType, QWidget* parent, QVariant& popupData,
                                 const QVariant& value = QVariant()) {
        PopupButton button = PopupButton::Invalid;

        qDebug() << "Popup::drawPopup() -> PopupType :" << static_cast<int>(gPopupType) << "->" << static_cast<int>(popupType);
        if (isPopupType() == popupType) {
            qDebug() << "\t This is the same pop-up as the previous one.";
            return button;
        }
        setPopupType(popupType);

        switch (popupType) {
            case PopupType::About: {
                QVariantList infoData = value.toList();
                if (infoData.size() == 2) {
                    button = drawPopupNoraml(parent, false, infoData.at(0).toString(), infoData.at(1).toString());
                }
                break;
            }
            case PopupType::OpenError:
            case PopupType::PathError:
            case PopupType::DeleteFileComplete: {
                QVariantList infoData = value.toList();
                if (infoData.size() == 2) {
                    button = drawPopupNoraml(parent, true, infoData.at(0).toString(), infoData.at(1).toString());
                }
                break;
            }
            case PopupType::AboutQt: {
                button = drawPopupAboutQt(parent);
                break;
            }
            case PopupType::DeleteFile: {
                button = drawPopupSelect(popupType, parent, value);
                break;
            }
            case PopupType::OpenFolder:
            case PopupType::SettingPath: {
                QVariantList infoData = value.toList();
                if (infoData.size() == 2) {
                    button = drawPopupSettingPath(parent, infoData.at(0).toString(), infoData.at(1).toString());
                }
                break;
            }
            default: {
                break;
            }
        }
        setPopupType(PopupType::Invalid);
        popupData = isPopupData();
        qDebug() << "Popup::drawPopup() -> Button :" << static_cast<int>(button) << ", Data :" << popupData;
        return button;
    }

private:
    static PopupType isPopupType() {
        return gPopupType;
    }
    static QVariant isPopupData() {
        return gPopupData;
    }
    static void setPopupType(const PopupType& type) {
        gPopupType = type;
    }
    static void setPopupData(const QVariant& data) {
        gPopupData = data;
    }
    static PopupButton drawPopupAboutQt(QWidget* parent) {
        PopupButton button = PopupButton::Invalid;
        if (parent) {
            QMessageBox::aboutQt(parent);
            button = PopupButton::OK;
        }
        return button;
    }
    static PopupButton drawPopupNoraml(QWidget* parent, const bool& warning, const QString& title, const QString& tip) {
        PopupButton button = PopupButton::Invalid;
        if (parent) {
            if (warning) {
                QMessageBox::warning(parent, title, tip);
            } else {
                QMessageBox::information(parent, title, tip);
            }
            button = PopupButton::OK;
        }
        return button;
    }
    static PopupButton drawPopupSettingPath(QWidget* parent, const QString& title, const QString& path) {
        PopupButton button = PopupButton::Invalid;
        if (parent) {
            QString showPath = path;
            if (showPath.size() == 0) {
                showPath = QApplication::applicationDirPath();
            }
            qDebug() << "Path :" << showPath;
            QString path = QFileDialog::getExistingDirectory(parent, title, showPath,
                                                             QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
            if (path.size() > 0) {
                setPopupData(path);
                button = PopupButton::OK;
            }
        }
        return button;
    }
    static PopupButton drawPopupSelect(const PopupType& popupType, QWidget* parent, const QVariant& textList) {
        PopupButton buttonType = PopupButton::Invalid;
        QMessageBox selectBox(parent);
        QVariantList list = textList.toList();
        QMap<PopupButton, QPushButton*> button = QMap<PopupButton, QPushButton*>();

        if ((popupType == PopupType::DeleteFile) && (list.size() == 4)) {
            selectBox.setWindowTitle(list[0].toString());
            selectBox.setText(list[1].toString());
            button[PopupButton::OK] = selectBox.addButton(list[2].toString(), QMessageBox::ActionRole);
            button[PopupButton::Cancel] = selectBox.addButton(list[3].toString(), QMessageBox::ActionRole);
            connect(button[PopupButton::OK], &QPushButton::clicked, [&]() { buttonType = PopupButton::OK; });
            connect(button[PopupButton::Cancel], &QPushButton::clicked, [&]() { buttonType = PopupButton::Cancel; });
        } else {
        }

        if (button.size() == 0) {
            return PopupButton::Invalid;
        }

        selectBox.exec();
        return buttonType;
    }
#else
    static PopupButton drawPopup(const PopupType& popupType, QWidget* parent, QVariant& popupData,
                                 const QVariant& value = QVariant()) {
        PopupButton button = PopupButton::Invalid;

        qDebug() << "Popup::drawPopup() -> PopupType :" << static_cast<int>(gPopupType) << "->" << static_cast<int>(popupType);
        if (isPopupType() == popupType) {
            qDebug() << "\t This is the same pop-up as the previous one.";
            return button;
        }
        setPopupType(popupType);

        switch (popupType) {
            case PopupType::About:
            case PopupType::OpenFail:
            case PopupType::NowInstalling:
            case PopupType::InstallComplete:
            case PopupType::DefaultPathError:
            case PopupType::InputTextError:
            case PopupType::ScriptRunnigCompleted:
            case PopupType::TCReportError:
            case PopupType::GcovReportError:
            case PopupType::RunPathError:
            case PopupType::ModuleSelectError:
            case PopupType::SelectCellColumnError: {
                QVariantList infoData = value.toList();
                if (infoData.size() == 2) {
                    bool warning = ((popupType != PopupType::About) && (popupType != PopupType::ScriptRunnigCompleted));
                    button = drawPopupNoraml(parent, warning, infoData.at(0).toString(), infoData.at(1).toString());
                }
                break;
            }
            case PopupType::Exit:
            case PopupType::New:
            case PopupType::NoInstallLib:
            case PopupType::FileNotExist: {
                button = drawPopupSelect(popupType, parent, value);
                break;
            }
            case PopupType::AboutQt: {
                button = drawPopupAboutQt(parent);
                break;
            }
            case PopupType::Open: {
                QVariantList infoData = value.toList();
                if (infoData.size() == 2) {
                    button = drawPopupOpen(parent, infoData.at(0).toString(), infoData.at(1).toString());
                }
                break;
            }
            case PopupType::Save: {
                button = drawPopupSave(parent);
                break;
            }
            case PopupType::SettingPath: {
                QVariantList infoData = value.toList();
                if (infoData.size() == 2) {
                    button = drawPopupSettingPath(parent, infoData.at(0).toString(), infoData.at(1).toString());
                }
                break;
            }
            default: {
                break;
            }
        }
        setPopupType(PopupType::Invalid);
        popupData = isPopupData();
        qDebug() << "Popup::drawPopup() -> Button :" << static_cast<int>(button) << ", Data :" << popupData;
        return button;
    }

private:
    static PopupType isPopupType() {
        return gPopupType;
    }
    static QVariant isPopupData() {
        return gPopupData;
    }
    static void setPopupType(const PopupType& type) {
        gPopupType = type;
    }
    static void setPopupData(const QVariant& data) {
        gPopupData = data;
    }
    static PopupButton drawPopupNoraml(QWidget* parent, const bool& warning, const QString& title, const QString& tip) {
        PopupButton button = PopupButton::Invalid;
        if (parent) {
            if (warning) {
                QMessageBox::warning(parent, title, tip);
            } else {
                QMessageBox::information(parent, title, tip);
                // QMessageBox::about(parent, title, tip);
            }
            button = PopupButton::OK;
        }
        return button;
    }
    static PopupButton drawPopupSelect(const PopupType& popupType, QWidget* parent, const QVariant& textList) {
        PopupButton buttonType = PopupButton::Invalid;
        QMessageBox selectBox(parent);
        QVariantList list = textList.toList();
        QMap<PopupButton, QPushButton*> button = QMap<PopupButton, QPushButton*>();

        if ((popupType == PopupType::Exit) && (list.size() == 5)) {
            selectBox.setWindowTitle(list[0].toString());
            selectBox.setText(list[1].toString());
            button[PopupButton::OK] = selectBox.addButton(list[2].toString(), QMessageBox::ActionRole);
            button[PopupButton::Discard] = selectBox.addButton(list[3].toString(), QMessageBox::ActionRole);
            button[PopupButton::Cancel] = selectBox.addButton(list[4].toString(), QMessageBox::ActionRole);
            connect(button[PopupButton::OK], &QPushButton::clicked, [&]() { buttonType = PopupButton::OK; });
            connect(button[PopupButton::Discard], &QPushButton::clicked, [&]() { buttonType = PopupButton::Discard; });
            connect(button[PopupButton::Cancel], &QPushButton::clicked, [&]() { buttonType = PopupButton::Cancel; });
        } else if ((popupType == PopupType::NoInstallLib) && (list.size() == 4)) {
            selectBox.setWindowTitle(list[0].toString());
            selectBox.setText(list[1].toString());
            button[PopupButton::Install] = selectBox.addButton(list[2].toString(), QMessageBox::ActionRole);
            button[PopupButton::Confirm] = selectBox.addButton(list[3].toString(), QMessageBox::ActionRole);
            connect(button[PopupButton::Install], &QPushButton::clicked, [&]() { buttonType = PopupButton::Install; });
            connect(button[PopupButton::Confirm], &QPushButton::clicked, [&]() { buttonType = PopupButton::Confirm; });
        } else if ((((popupType == PopupType::New)) || (popupType == PopupType::FileNotExist)) && (list.size() == 4)) {
            selectBox.setWindowTitle(list[0].toString());
            selectBox.setText(list[1].toString());
            button[PopupButton::Confirm] = selectBox.addButton(list[2].toString(), QMessageBox::ActionRole);
            button[PopupButton::Cancel] = selectBox.addButton(list[3].toString(), QMessageBox::ActionRole);
            connect(button[PopupButton::Confirm], &QPushButton::clicked, [&]() { buttonType = PopupButton::Confirm; });
            connect(button[PopupButton::Cancel], &QPushButton::clicked, [&]() { buttonType = PopupButton::Cancel; });
        } else if (((popupType == PopupType::DefaultPathError) || (popupType == PopupType::InputTextError) ||
                    (popupType == PopupType::TCReportError) || (popupType == PopupType::GcovReportError)) &&
                   (list.size() == 3)) {
            selectBox.setWindowTitle(list[0].toString());
            selectBox.setText(list[1].toString());
            button[PopupButton::Confirm] = selectBox.addButton(list[2].toString(), QMessageBox::ActionRole);
            connect(button[PopupButton::Confirm], &QPushButton::clicked, [&]() { buttonType = PopupButton::Confirm; });
        } else {
        }

        if (button.size() == 0) {
            return PopupButton::Invalid;
        }

        selectBox.exec();
        return buttonType;
    }
    static PopupButton drawPopupAboutQt(QWidget* parent) {
        PopupButton button = PopupButton::Invalid;
        if (parent) {
            // QApplication::aboutQt();
            QMessageBox::aboutQt(parent);
            button = PopupButton::OK;
        }
        return button;
    }
    static PopupButton drawPopupOpen(QWidget* parent, const QString& title, const QString& path) {
        PopupButton button = PopupButton::Invalid;
        if (parent) {
            QString filePath =
                QFileDialog::getOpenFileName(parent, title, path, QString("Excel (*.xls *.xlsx);;All files (*.*)"));
            if (filePath.size() > 0) {
                setPopupData(filePath);
                button = PopupButton::OK;
            }
        }
        return button;
    }
    static PopupButton drawPopupSave(QWidget* parent) {
        PopupButton button = PopupButton::Invalid;
        if (parent) {
            QFileDialog dialog(parent);
            dialog.setWindowModality(Qt::WindowModal);
            dialog.setAcceptMode(QFileDialog::AcceptSave);
            if (dialog.exec() == QDialog::Accepted) {
                setPopupData(dialog.selectedFiles().first());
                button = PopupButton::OK;
            }
        }
        return button;
    }
    static PopupButton drawPopupSettingPath(QWidget* parent, const QString& title, const QString& path) {
        PopupButton button = PopupButton::Invalid;
        if (parent) {
            QString showPath = path;
            if (showPath.size() == 0) {
                showPath = QApplication::applicationDirPath();
            }
            QString path = QFileDialog::getExistingDirectory(parent, title, showPath,
                                                             QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
            if (path.size() > 0) {
                setPopupData(path);
                button = PopupButton::OK;
            }
        }
        return button;
    }
#endif
};

}  // end of namespace common
}  // end of namespace ivis

#endif  // COMMON_POPUP_H
