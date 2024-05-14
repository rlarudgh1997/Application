#include "Dialog.h"
#include "ui_Dialog.h"

#include <QScrollBar>

Dialog::Dialog(const QRect& rect, QWidget* parent) : QDialog(parent), mGui(new Ui::Dialog) {
    mGui->setupUi(this);

    // this->setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    // this->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    // this->setWindowFlag(Qt::WindowCloseButtonHint, false);
    this->setParent(parent);
    // this->setModal(true);
    this->hide();

    setScreenRect(rect);
}

Dialog::~Dialog() {
    controlConnet(DisplayTypeMax);
    delete mGui;
    // qDebug() << "~Dialog";
}

void Dialog::drawDialog(const int& dialogType, const QVariantList& value) {
    qDebug() << "drawDialog :" << dialogType << value.size();
    bool draw = false;
    setDialogType(dialogType);
    switch (dialogType) {
        case DialogTypeAppModeCheck: {
            draw = (value.size() == 3);
            if (draw) {
                QString title = value.at(0).toString();
                int appMode = value.at(1).toInt();
                QStringList appModeList = value.at(2).toStringList();
                updateAppMode(title, appMode, appModeList);
            }
            break;
        }
        case DialogTypeAppModeRadio: {
            draw = (value.size() == 3);
            if (draw) {
                QString title = value.at(0).toString();
                int appMode = value.at(1).toInt();
                QStringList appModeList = value.at(2).toStringList();
                updateAppModeRadio(title, appMode, appModeList);
            }
            break;
        }
        case DialogTypeMoudleInfo:
        case DialogTypeViewLogFile: {
            draw = (value.size() == 4);
            if (draw) {
                QString title = value.at(0).toString();
                QStringList column = value.at(1).toStringList();
                QStringList list = value.at(2).toStringList();
                bool all = value.at(3).toBool();
                updateSelectList(dialogType, title, column, list, all);
            }
            break;
        }
        case DialogTypeSelectNegative:
        case DialogTypeSelectVehiclePV:
        case DialogTypeSelectVehicleCV: {
            draw = (value.size() == 3);
            if (draw) {
                QString title = value.at(0).toString();
                QString option1 = value.at(1).toString();
                QStringList option2 = value.at(2).toStringList();
                updateSelectOption(dialogType, title, option1, option2);
            }
            break;
        }
        case DialogTypeEnterText: {
            draw = (value.size() == 1);
            if (draw) {
                QString title = value.at(0).toString();
                updateInputText(title);
            }
            break;
        }
        case DialogTypeTestReportTC:
        case DialogTypeTestReportGCOV: {
            draw = (value.size() == 4);
            if (draw) {
                QString title = value.at(0).toString();
                bool option1 = value.at(1).toBool();
                QStringList option2Str = value.at(2).toStringList();
                QVariantList option2Value = value.at(3).toList();
                updateTestReport(dialogType, title, option1, option2Str, option2Value);
            }
            break;
        }
        case DialogTypeLogDisplay: {
            draw = (value.size() == 4);
            if (draw) {
                QString title = value.at(0).toString();
                QString titleInfo = value.at(1).toString();
                QString errorInfo = value.at(2).toString();
                QString moduleStateInfo = value.at(3).toString();
                updateLogDisplay(title, titleInfo, errorInfo, moduleStateInfo);
            }
            break;
        }
        case DialogTypeDetialLog: {
            draw = (value.size() == 2);
            if (draw) {
                QString title = value.at(0).toString();
                QStringList detailLog = value.at(3).toStringList();
                updateDetailLog(title, detailLog);
            }
            break;
        }
        default: {
            qDebug() << "\t Fail to dialog type :" << dialogType;
            break;
        }
    }
    qDebug() << "\t Draw :" << ((draw) ? ("Sucess") : ("Fail"));
}

void Dialog::controlConnet(const int& dialogType) {
    if (dialogType == DisplayTypeAppMode) {
        connect(&mModel, &QStandardItemModel::dataChanged,
                [=](const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles) {
                    int appMode = topLeft.row();
                    if (getAppMode() != appMode) {
                        mModel.item(getAppMode(), 0)->setCheckState(Qt::Unchecked);
                        mModel.item(appMode, 0)->setCheckState(Qt::Checked);
                        setAppMode(appMode);
                    }
                });
        connect(mGui->AppModeOK, &QPushButton::clicked, [=]() {
            for (int rowIndex = 0; rowIndex < mModel.rowCount(); rowIndex++) {
                if (mModel.item(rowIndex, 0) == nullptr) {
                    continue;
                }
                if (mModel.item(rowIndex, 0)->checkState() == Qt::Checked) {
                    setAppMode(rowIndex);
                    break;
                }
            }
            emit signalSelectAppMode(getAppMode());
            QDialog::accept();
        });
    } else if (dialogType == DisplayTypeAppModeRadio) {
        for (const auto& widget : isRadioWidget()) {
            connect(widget.second, &QPushButton::clicked, [=]() {
                for (const auto& widgetItem : isRadioWidget()) {
                    widgetItem.second->setChecked(false);
                }
                widget.second->setChecked(true);
            });
        }
        connect(mGui->AppModeRadioOK, &QPushButton::clicked, [=]() {
            int appMode = 0;
            for (const auto& widget : isRadioWidget()) {
                if (widget.second->isChecked()) {
                    setAppMode(appMode);
                    break;
                }
                appMode++;
            }
            emit signalSelectAppMode(getAppMode());
            QDialog::accept();
        });
    } else if (dialogType == DisplayTypeSelectList) {
        connect(mGui->SelectListAll, &QPushButton::clicked, [=]() {
            setSelectAll(getSelectAll() == false);
            updateSelectListCheckState(getSelectAll());
        });
        connect(mGui->SelectListOK, &QPushButton::clicked, [=]() {
            QList<QPair<int, QString>> selectItem = QList<QPair<int, QString>>();
            for (int rowIndex = 0; rowIndex < mModel.rowCount(); rowIndex++) {
                if (mModel.item(rowIndex, 0) == nullptr) {
                    continue;
                }
                if (mModel.item(rowIndex, 0)->checkState() == Qt::Checked) {
                    selectItem.append(QPair<int, QString>(rowIndex, mModel.item(rowIndex, 0)->text()));
                }
            }
            emit signalSelectListItem(selectItem);
            // QDialog::accept();
        });
    } else if (dialogType == DisplayTypeSelectOption) {
        connect(mGui->SelectOptionOK, &QPushButton::clicked, [=]() {
            bool option1 = (mGui->SelectOption1Check->checkState() == Qt::CheckState::Checked);
            QList<QPair<QString, bool>> option2 = QList<QPair<QString, bool>>();
            for (const auto& check : isCheckWidget(true)) {
                if (check->isVisible()) {
                    option2.append(QPair<QString, bool>(check->text(), (check->checkState() == Qt::CheckState::Checked)));
                }
            }
            emit signalSelectOption(option1, option2);
            QDialog::accept();
        });
        connect(mGui->SelectOptionCancel, &QPushButton::clicked, [=]() {
            QDialog::accept();
        });
    } else if (dialogType == DisplayTypeEnterText) {
        connect(mGui->EnterTextOK, &QPushButton::clicked, [=]() {
            QString inputText = mGui->EnterTextLineEdit->text();
            emit signalEnterTextChanged(inputText);
        });
    } else if (dialogType == DisplayTypeTestReport) {
        connect(mGui->TestReportOption1Radio1, &QRadioButton::clicked, [=]() {
            for (const auto& check : isCheckWidget(false)) {
                check->setEnabled(true);
            }
        });
        connect(mGui->TestReportOption1Radio2, &QRadioButton::clicked, [=]() {
            for (const auto& check : isCheckWidget(false)) {
                check->setEnabled(false);
            }
        });
        connect(mGui->TestReportOK, &QPushButton::clicked, [=]() {
            bool option1 = mGui->TestReportOption1Radio1->isChecked();
            QList<QPair<QString, bool>> option2 = QList<QPair<QString, bool>>();
            for (const auto& check : isCheckWidget(false)) {
                if (check->isVisible()) {
                    option2.append(QPair<QString, bool>(check->text(), (check->checkState() == Qt::CheckState::Checked)));
                }
            }
            emit signalSelectOption(option1, option2);
            QDialog::accept();
        });
        connect(mGui->TestReportCancel, &QPushButton::clicked, [=]() {
            QDialog::accept();
        });
    } else if (dialogType == DisplayTypeLogDisplay) {
        connect(mGui->LogDisplayClose, &QPushButton::clicked, [=]() {
            emit signalLogDisplayClicked(true, false);
            QDialog::accept();
        });
        connect(mGui->LogDisplayCancel, &QPushButton::clicked, [=]() {
            emit signalLogDisplayClicked(false, false);
        });
        connect(mGui->LogDisplayDetail, &QPushButton::clicked, [=]() {
            emit signalLogDisplayClicked(true, true);
        });
    } else if (dialogType == DisplayTypeDetialLog) {
    } else {
        disconnect(&mModel);
        disconnect(mGui->AppModeOK, nullptr, nullptr, nullptr);
        disconnect(mGui->AppModeRadioOK, nullptr, nullptr, nullptr);
        for (const auto& widget : isRadioWidget()) {
            disconnect(widget.second, nullptr, nullptr, nullptr);
        }
        disconnect(mGui->SelectListAll, nullptr, nullptr, nullptr);
        disconnect(mGui->SelectListOK, nullptr, nullptr, nullptr);
        disconnect(mGui->SelectOptionOK, nullptr, nullptr, nullptr);
        disconnect(mGui->SelectOptionCancel, nullptr, nullptr, nullptr);
        disconnect(mGui->TestReportOption1Radio1, nullptr, nullptr, nullptr);
        disconnect(mGui->TestReportOption1Radio2, nullptr, nullptr, nullptr);
        disconnect(mGui->TestReportOK, nullptr, nullptr, nullptr);
        disconnect(mGui->TestReportCancel, nullptr, nullptr, nullptr);
        disconnect(mGui->LogDisplayClose, nullptr, nullptr, nullptr);
        disconnect(mGui->LogDisplayCancel, nullptr, nullptr, nullptr);
        disconnect(mGui->LogDisplayDetail, nullptr, nullptr, nullptr);
    }
}

QRect Dialog::updateMainRect() {
    QRect rect = QRect();
    switch (getDialogType()) {
        case DialogTypeAppModeCheck: {
            rect = mGui->AppModeWidget->geometry();
            break;
        }
        case DialogTypeAppModeRadio: {
            rect = mGui->AppModeRadioWidget->geometry();
            break;
        }
        case DialogTypeMoudleInfo: {
            mGui->SelectListWidget->setGeometry(QRect(0, 0, 600, 800));
            rect = mGui->SelectListWidget->geometry();
            break;
        }
        case DialogTypeViewLogFile: {
            mGui->SelectListWidget->setGeometry(QRect(0, 0, 500, 300));
            rect = mGui->SelectListWidget->geometry();
            break;
        }
        case DialogTypeSelectNegative: {
            mGui->SelectOptionWidget->setGeometry(QRect(0, 0, 400, 120));
            rect = mGui->SelectOptionWidget->geometry();
            break;
        }
        case DialogTypeSelectVehiclePV:
        case DialogTypeSelectVehicleCV: {
            mGui->SelectOptionWidget->setGeometry(QRect(0, 0, 400, 150));
            rect = mGui->SelectOptionWidget->geometry();
            break;
        }
        case DialogTypeEnterText: {
            rect = mGui->EnterTextWidget->geometry();
            break;
        }
        case DialogTypeTestReportTC:
        case DialogTypeTestReportGCOV: {
            rect = mGui->TestReportWidget->geometry();
            break;
        }
        case DialogTypeLogDisplay: {
            rect = mGui->LogDisplayWidget->geometry();
            break;
        }
        case DialogTypeDetialLog: {
            rect = mGui->DetialLogWidget->geometry();
            break;
        }
        default: {
            break;
        }
    }
    return rect;
}

void Dialog::updateDisplay(const int& displayType, const QString& title) {
    QRect mainRect = updateMainRect();
    controlConnet(displayType);
    mGui->StackedWidget->setCurrentIndex(displayType);

    if (mainRect.isValid()) {
        QRect screenRect = getScreenRect();
        QRect setRect = QRect();
        setRect.setX(static_cast<int>(screenRect.x() + (screenRect.width() - mainRect.width()) * 0.5));
        setRect.setY(static_cast<int>(screenRect.y() + (screenRect.height() - mainRect.height()) * 0.5));
        setRect.setWidth(mainRect.width());
        setRect.setHeight(mainRect.height());
        // this->setFixedSize(QSize(mainRect.width(), mainRect.height()));
        this->setGeometry(setRect);
        // qDebug() << "updateDisplay :" << screenRect << mainRect << setRect;
    }
    this->setWindowTitle(title);
    this->setModal(true);
    this->setFocus();
    this->show();
    // qDebug() << "updateDisplay :" << displayType << title << geometry();
}

QList<QPair<QFrame*, QRadioButton*>> Dialog::isRadioWidget() const {
    QList<QPair<QFrame*, QRadioButton*>> widgetList = {
        {mGui->FrameList1, mGui->AppModeRadioButton1}, {mGui->FrameList2, mGui->AppModeRadioButton2},
        {mGui->FrameList3, mGui->AppModeRadioButton3}, {mGui->FrameList4, mGui->AppModeRadioButton4},
        {mGui->FrameList5, mGui->AppModeRadioButton5}, {mGui->FrameList6, mGui->AppModeRadioButton6},
        {mGui->FrameList7, mGui->AppModeRadioButton7}, {mGui->FrameList8, mGui->AppModeRadioButton8},
    };
    return widgetList;
}

QList<QCheckBox*> Dialog::isCheckWidget(const bool& selectOption) const {
    QList<QCheckBox*> widgetList = QList<QCheckBox*>();
    if (selectOption) {
        widgetList = QList<QCheckBox*>{
            {mGui->SelectOption2Check1},
            {mGui->SelectOption2Check2},
            {mGui->SelectOption2Check3},
            {mGui->SelectOption2Check4},
            {mGui->SelectOption2Check5},
        };
    } else {
        widgetList = QList<QCheckBox*>{
            {mGui->TestReportOption2Check1},
            {mGui->TestReportOption2Check2},
            {mGui->TestReportOption2Check3},
            {mGui->TestReportOption2Check4},
            {mGui->TestReportOption2Check5},
        };
    }
    return widgetList;
}

void Dialog::updateAppMode(const QString& title, const int& appMode, const QStringList& appModeList) {
    updateDisplay(DisplayTypeAppMode, title);

    QStringList columnTitle = QStringList({"App Mode"});
    mModel.setHorizontalHeaderLabels(columnTitle);
    mModel.setColumnCount(columnTitle.size());
    mModel.setRowCount(appModeList.size());
    int rowIndex = 0;
    for (const auto& name : appModeList) {
        delete mModel.item(rowIndex, 0);
        mModel.setItem(rowIndex, 0, new QStandardItem(name));
        mModel.item(rowIndex, 0)->setCheckable(true);
        mModel.item(rowIndex, 0)->setCheckState((appMode == rowIndex) ? (Qt::Checked) : (Qt::Unchecked));
        mModel.item(rowIndex, 0)->setFlags(mModel.item(rowIndex, 0)->flags() & ~Qt::ItemFlag::ItemIsEditable);
        rowIndex++;
    }
    setAppMode(appMode);
    mGui->AppModeTableView->setModel(&mModel);
    mGui->AppModeTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    mGui->AppModeTableView->verticalHeader()->setHidden(true);
    for (rowIndex = 0; rowIndex < mModel.rowCount(); ++rowIndex) {
        mGui->AppModeTableView->verticalHeader()->setSectionResizeMode(rowIndex, QHeaderView::Fixed);
        mGui->AppModeTableView->verticalHeader()->resizeSection(rowIndex, 20);
    }
}

void Dialog::updateAppModeRadio(const QString& title, const int& appMode, const QStringList& appModeList) {
    updateDisplay(DisplayTypeAppModeRadio, title);

    mGui->AppModeRadioTitle->setText("Vehicle");
    int index = 0;
    for (const auto& widget : isRadioWidget()) {
        widget.first->setVisible(index < appModeList.size());
        widget.second->setChecked(appMode == index);
        if (widget.first->isVisible()) {
            widget.second->setText(appModeList.at(index));
        }
        index++;
    }
}

void Dialog::updateSelectListCheckState(const bool& all) {
    mGui->SelectListAll->setText((all) ? ("Unselect All") : ("Select All"));
    for (int rowIndex = 0; rowIndex < mModel.rowCount(); rowIndex++) {
        if (mModel.item(rowIndex, 0) == nullptr) {
            continue;
        }
        mModel.item(rowIndex, 0)->setCheckState((all) ? (Qt::Checked) : (Qt::Unchecked));
    }
    setSelectAll(all);
}

void Dialog::updateSelectList(const int& dialogType, const QString& title, const QStringList& column, const QStringList& list,
                              const bool& all) {
    updateDisplay(DisplayTypeSelectList, title);

    bool headerFixed = true;
    int rowIndex = 0;

    if (dialogType == DialogTypeMoudleInfo) {
        updateSelectListCheckState(all);
    } else {
        updateSelectListCheckState(false);
        mGui->SelectListAll->setVisible(false);
    }

    mModel.setHorizontalHeaderLabels(column);
    mModel.setColumnCount(column.size());
    mModel.setRowCount(list.size());
    for (const auto& module : list) {
        delete mModel.item(rowIndex, 0);
        mModel.setItem(rowIndex, 0, new QStandardItem(module));
        mModel.item(rowIndex, 0)->setCheckable(true);
        mModel.item(rowIndex, 0)->setFlags(mModel.item(rowIndex, 0)->flags() & ~Qt::ItemFlag::ItemIsEditable);
        rowIndex++;
    }
    if (headerFixed) {
        mGui->SelectListItemTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    } else {
        mGui->SelectListItemTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
    }
    mGui->SelectListItemTableView->setModel(&mModel);
    mGui->SelectListItemTableView->verticalHeader()->setHidden(true);
    for (rowIndex = 0; rowIndex < mModel.rowCount(); ++rowIndex) {
        mGui->SelectListItemTableView->verticalHeader()->setSectionResizeMode(rowIndex, QHeaderView::Fixed);
        mGui->SelectListItemTableView->verticalHeader()->resizeSection(rowIndex, 20);
    }
}

void Dialog::updateSelectOption(const int& dialogType, const QString& title, const QString& option1, const QStringList& option2) {
    updateDisplay(DisplayTypeSelectOption, title);

    mGui->SelectOption1->setVisible(true);
    mGui->SelectOption1Check->setText(option1);
    mGui->SelectOption1Check->setChecked((dialogType != DialogTypeSelectNegative) );

    int index = 0;
    mGui->SelectOption2->setVisible(option2.size() > 0);
    for (const auto& check : isCheckWidget(true)) {
        check->setVisible(index < option2.size());
        if (check->isVisible()) {
            check->setText(option2.at(index));
            index++;
        }
    }
}

void Dialog::updateInputText(const QString& title) {
    updateDisplay(DisplayTypeEnterText, title);
}

void Dialog::updateTestReport(const int& dialogType, const QString& title, const bool& option1, const QStringList& option2Str,
                              const QVariantList& option2Value) {
    updateDisplay(DisplayTypeTestReport, title);

    mGui->TestReportOption1->setVisible(true);
    mGui->TestReportOption1Radio1->setText("On");
    mGui->TestReportOption1Radio1->setChecked(option1);
    mGui->TestReportOption1Radio2->setText("Off");
    mGui->TestReportOption1Radio2->setChecked(option1 == false);

    int index = 0;
    mGui->TestReportOption2->setVisible(option2Str.size() > 0);
    for (const auto& check : isCheckWidget(false)) {
        check->setVisible(index < option2Str.size());
        if (check->isVisible()) {
            check->setText(option2Str.at(index));
            check->setChecked(option2Value.at(index).toBool());
            check->setEnabled(option1);
            index++;
        }
    }
}

void Dialog::updateLogDisplay(const QString& title, const QString& titleInfo, const QString& errorInfo,
                              const QString& moduleStateInfo) {
    updateDisplay(DisplayTypeLogDisplay, title);

    mGui->LogDisplayTitle->setText(titleInfo);
    int startIndex = mGui->LogDisplayTitle->toPlainText().indexOf(errorInfo);
    if (startIndex >= 0) {
        QTextCursor cursor = mGui->LogDisplayTitle->textCursor();
        cursor.setPosition(startIndex);

        QTextCharFormat charFormat;
        charFormat.setForeground(QColor("red"));
        // charFormat.setFontWeight(QFont::Bold);
        // charFormat.setFontPointSize(10);

        for (int index = 0; index < errorInfo.size() + 1; index++) {
            cursor.setPosition((startIndex + index), QTextCursor::KeepAnchor);
            cursor.setCharFormat(charFormat);
        }
    }
    mGui->LogDisplayContent->setText(moduleStateInfo);
    mGui->LogDisplayContent->verticalScrollBar()->setValue(mGui->LogDisplayContent->verticalScrollBar()->maximum());
}

void Dialog::updateDetailLog(const QString& title, const QStringList& detailLog) {
    updateDisplay(DisplayTypeDetialLog, title);

}
