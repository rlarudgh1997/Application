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
    OpenFail,
    DataValidation,
    NowInstalling,
    InstallComplete,
    DefaultPathError,
    InputTextError,
    ScriptRunnigCompleted,
    TCReportError,
    GcovReportError,
    RunPathError,
    ModuleSelectError,
    CellSelectionError,
    TestCaseComplete,

    Exit,
    New,
    OpeningFile,
    AnotherFileOpen,
    NoInstallLib,
    FileNotExist,
    RestConfigValue,

    AboutQt,
    Open,
    Save,
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
    static PopupButton drawPopup(const PopupType& popupType, AbstractHandler* handler, QVariant& popupData,
                                 const QVariant& popupInfo = QVariant()) {
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
            case PopupType::OpeningFile:
            case PopupType::DataValidation:
            case PopupType::NowInstalling:
            case PopupType::InstallComplete:
            case PopupType::DefaultPathError:
            case PopupType::InputTextError:
            case PopupType::ScriptRunnigCompleted:
            case PopupType::TCReportError:
            case PopupType::GcovReportError:
            case PopupType::RunPathError:
            case PopupType::ModuleSelectError:
            case PopupType::CellSelectionError: {
                QVariantList infoData = popupInfo.toList();
                if (infoData.size() == 2) {
                    bool warning = ((popupType != PopupType::About) && (popupType != PopupType::ScriptRunnigCompleted));
                    button = drawPopupNoraml(handler, warning, infoData.at(0).toString(), infoData.at(1).toString());
                }
                break;
            }
            case PopupType::Exit:
            case PopupType::New:
            case PopupType::AnotherFileOpen:
            case PopupType::NoInstallLib:
            case PopupType::FileNotExist:
            case PopupType::TestCaseComplete:
            case PopupType::RestConfigValue: {
                button = drawPopupSelect(popupType, handler, popupInfo);
                break;
            }
            case PopupType::AboutQt: {
                button = drawPopupAboutQt(handler);
                break;
            }
            case PopupType::Open: {
                QVariantList infoData = popupInfo.toList();
                if (infoData.size() == 2) {
                    button = drawPopupOpen(handler, infoData.at(0).toString(), infoData.at(1).toString());
                }
                break;
            }
            case PopupType::Save: {
                QVariantList infoData = popupInfo.toList();
                if (infoData.size() == 1) {
                    button = drawPopupSave(handler, infoData.at(0).toString());
                }
                break;
            }
            case PopupType::SettingPath: {
                QVariantList infoData = popupInfo.toList();
                if (infoData.size() == 2) {
                    button = drawPopupSettingPath(handler, infoData.at(0).toString(), infoData.at(1).toString());
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
    static PopupButton drawPopupNoraml(AbstractHandler* handler, const bool& warning, const QString& title, const QString& tip) {
        PopupButton button = PopupButton::Invalid;
        if (handler) {
            if (warning) {
                QMessageBox::warning(handler->getScreen(), title, tip);
            } else {
                QMessageBox::information(handler->getScreen(), title, tip);
                // QMessageBox::about(handler->getScreen(), title, tip);
            }
            button = PopupButton::OK;
        }
        return button;
    }
    static PopupButton drawPopupSelect(const PopupType& popupType, AbstractHandler* handler, const QVariant& textList) {
        PopupButton buttonType = PopupButton::Invalid;
        QMessageBox selectBox(handler->getScreen());
        QVariantList list = textList.toList();
        QMap<PopupButton, QPushButton*> button = QMap<PopupButton, QPushButton*>();

        switch (popupType) {
            case PopupType::Exit: {
                if (list.size() == 5) {
                    selectBox.setWindowTitle(list[0].toString());
                    selectBox.setText(list[1].toString());
                    button[PopupButton::OK] = selectBox.addButton(list[2].toString(), QMessageBox::ActionRole);
                    button[PopupButton::Discard] = selectBox.addButton(list[3].toString(), QMessageBox::ActionRole);
                    button[PopupButton::Cancel] = selectBox.addButton(list[4].toString(), QMessageBox::ActionRole);
                    connect(button[PopupButton::OK], &QPushButton::clicked, [&]() { buttonType = PopupButton::OK; });
                    connect(button[PopupButton::Discard], &QPushButton::clicked, [&]() { buttonType = PopupButton::Discard; });
                    connect(button[PopupButton::Cancel], &QPushButton::clicked, [&]() { buttonType = PopupButton::Cancel; });
                }
                break;
            }
            case PopupType::NoInstallLib: {
                if (list.size() == 4) {
                    selectBox.setWindowTitle(list[0].toString());
                    selectBox.setText(list[1].toString());
                    button[PopupButton::Install] = selectBox.addButton(list[2].toString(), QMessageBox::ActionRole);
                    button[PopupButton::Confirm] = selectBox.addButton(list[3].toString(), QMessageBox::ActionRole);
                    connect(button[PopupButton::Install], &QPushButton::clicked, [&]() { buttonType = PopupButton::Install; });
                    connect(button[PopupButton::Confirm], &QPushButton::clicked, [&]() { buttonType = PopupButton::Confirm; });
                }
                break;
            }
            case PopupType::New:
            case PopupType::FileNotExist:
            case PopupType::RestConfigValue:
            case PopupType::TestCaseComplete:
            case PopupType::AnotherFileOpen: {
                if (list.size() == 4) {
                    selectBox.setWindowTitle(list[0].toString());
                    selectBox.setText(list[1].toString());
                    button[PopupButton::Confirm] = selectBox.addButton(list[2].toString(), QMessageBox::ActionRole);
                    button[PopupButton::Cancel] = selectBox.addButton(list[3].toString(), QMessageBox::ActionRole);
                    connect(button[PopupButton::Confirm], &QPushButton::clicked, [&]() { buttonType = PopupButton::Confirm; });
                    connect(button[PopupButton::Cancel], &QPushButton::clicked, [&]() { buttonType = PopupButton::Cancel; });
                }
                break;
            }
#if 0
            case PopupType::DefaultPathError:
            case PopupType::InputTextError:
            case PopupType::TCReportError:
            case PopupType::GcovReportError: {
                if (list.size() == 3) {
                    selectBox.setWindowTitle(list[0].toString());
                    selectBox.setText(list[1].toString());
                    button[PopupButton::Confirm] = selectBox.addButton(list[2].toString(), QMessageBox::ActionRole);
                    connect(button[PopupButton::Confirm], &QPushButton::clicked, [&]() { buttonType = PopupButton::Confirm; });
                }
                break;
            }
#endif
            default: {
                break;
            }
        }

        connect(&selectBox, &QMessageBox::finished, [&](int result) {
            if (result == QMessageBox::Rejected) {
                buttonType = PopupButton::Cancel;  // X 버튼 또는 Esc 키 눌렀을 때 처리
            }
        });
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
            QString filePath =
                QFileDialog::getOpenFileName(handler->getScreen(), title, path, QString("Excel (*.xls *.xlsx);;All files (*.*)"));
            if (filePath.size() > 0) {
                setPopupData(filePath);
                button = PopupButton::OK;
            }
        }
        return button;
    }
    static PopupButton drawPopupSave(AbstractHandler* handler, const QString& path) {
        PopupButton button = PopupButton::Invalid;
        if (handler) {
            QFileDialog dialog(handler->getScreen());
            dialog.setWindowModality(Qt::WindowModal);
            dialog.setAcceptMode(QFileDialog::AcceptSave);
            dialog.setDirectory(path);
            if (dialog.exec() == QDialog::Accepted) {
                setPopupData(dialog.selectedFiles().first());
                button = PopupButton::OK;
            }
        }
        return button;
    }
    static PopupButton drawPopupSettingPath(AbstractHandler* handler, const QString& title, const QString& path) {
        PopupButton button = PopupButton::Invalid;
        if (handler) {
            QString showPath = path;
            if (showPath.size() == 0) {
                showPath = QApplication::applicationDirPath();
            }
            QString path = QFileDialog::getExistingDirectory(handler->getScreen(), title, showPath,
                                                             QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
            if (path.size() > 0) {
                setPopupData(path);
                button = PopupButton::OK;
            }
        }
        return button;
    }
};

}  // end of namespace common
}  // end of namespace ivis

#endif  // COMMON_POPUP_H
