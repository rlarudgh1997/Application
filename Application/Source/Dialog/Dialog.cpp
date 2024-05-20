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
    qDebug() << "Destructor : Dialog";
}

void Dialog::drawDialog(const int& dialogType, const QVariantList& value) {
    if (getKeepDialog()) {
        qDebug() << "[Dialog] Current dialog screen maintenance status";
        return;
    }

    bool draw = false;

    // Previous Info
    setPrevDialogInfo(getDialogInfo());
    setPrevDialogType(getDialogType());

    // Current Info
    setDialogInfo(value);
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
        case DialogTypeSelectMoudleInfo:
        case DialogTypSelectLogFile: {
            draw = (value.size() == 4);
            if (draw) {
                QString title = value.at(0).toString();
                QStringList column = value.at(1).toStringList();
                QStringList list = value.at(2).toStringList();
                bool all = value.at(3).toBool();
                updateSelectList(title, column, list, all);
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
                updateSelectOption(title, option1, option2);
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
                updateTestReport(title, option1, option2Str, option2Value);
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
        case DialogTypeViewLogInfo:
        case DialogTypeViewLogFileInfo: {
            draw = (value.size() == 2);
            if (draw) {
                setKeepDialog(dialogType == DialogTypeViewLogInfo);
                QString title = value.at(0).toString();
                QStringList detailLog = value.at(1).toStringList();
                updateViewLog(title, detailLog);
            }
            break;
        }
        default: {
            qDebug() << "\t Fail to dialog type :" << dialogType;
            break;
        }
    }
    qDebug() << "drawDialog :" << dialogType << value.size() << ((draw) ? ("-> Sucess") : ("-> Fail"));
}

void Dialog::controlConnet(const int& displayType) {
    if (getPrevDisplayType() == displayType) {
        qDebug() << "[Dialog] Skip to request for same screen as previous screen :" << displayType;
        return;
    }

    switch (displayType) {
        case DisplayTypeAppMode: {
            connectAppMode(true);
            break;
        }
        case DisplayTypeAppModeRadio: {
            connectAppModeRadio(true);
            break;
        }
        case DisplayTypeSelectList: {
            connectSelectList(true);
            break;
        }
        case DisplayTypeSelectOption: {
            connectSelectOption(true);
            break;
        }
        case DisplayTypeEnterText: {
            connectEnterText(true);
            break;
        }
        case DisplayTypeTestReport: {
            connectTestReport(true);
            break;
        }
        case DisplayTypeLogDisplay: {
            connectLogDisplay(true);
            break;
        }
        case DisplayTypeViewLog: {
            connectViewLog(true);
            break;
        }
        default: {
            connectAppMode(false);
            connectAppModeRadio(false);
            connectSelectList(false);
            connectSelectOption(false);
            connectEnterText(false);
            connectTestReport(false);
            connectLogDisplay(false);
            connectViewLog(false);
            qDebug() << "[Dialog] disconnect : complted";
            break;
        }
    }
}

void Dialog::connectAppMode(const bool& state) {
    if (state) {
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
        connect(&mModel, &QStandardItemModel::dataChanged,
                [=](const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles) {
                    int appMode = topLeft.row();
                    if (getAppMode() != appMode) {
                        mModel.item(getAppMode(), 0)->setCheckState(Qt::Unchecked);
                        mModel.item(appMode, 0)->setCheckState(Qt::Checked);
                        setAppMode(appMode);
                    }
                });
    } else {
        disconnect(&mModel, nullptr, nullptr, nullptr);
        disconnect(mGui->AppModeOK, nullptr, nullptr, nullptr);
    }
}

void Dialog::connectAppModeRadio(const bool& state) {
    if (state) {
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
    } else {
        for (const auto& widget : isRadioWidget()) {
            disconnect(widget.second, nullptr, nullptr, nullptr);
        }
        disconnect(mGui->AppModeRadioOK, nullptr, nullptr, nullptr);
    }
}

void Dialog::connectSelectList(const bool& state) {
    if (state) {
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
        connect(&mModel, &QStandardItemModel::dataChanged,
                [=](const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles) {
                    if (getDialogType() == DialogTypSelectLogFile) {
                        int checkModelIndex = topLeft.row();  // bottomRight.row()
                        int preCheckModelIndex = getCheckModelIndex();
                        if ((preCheckModelIndex != checkModelIndex) && (preCheckModelIndex >= 0)) {
                            mModel.item(preCheckModelIndex, 0)->setCheckState(Qt::Unchecked);
                        }
                        setCheckModelIndex(checkModelIndex);
                    }
                });
    } else {
        disconnect(mGui->SelectListAll, nullptr, nullptr, nullptr);
        disconnect(mGui->SelectListOK, nullptr, nullptr, nullptr);
        disconnect(&mModel, nullptr, nullptr, nullptr);
    }
}

void Dialog::connectSelectOption(const bool& state) {
    if (state) {
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
    } else {
        disconnect(mGui->SelectOptionOK, nullptr, nullptr, nullptr);
        disconnect(mGui->SelectOptionCancel, nullptr, nullptr, nullptr);
    }
}

void Dialog::connectEnterText(const bool& state) {
    if (state) {
        connect(mGui->EnterTextOK, &QPushButton::clicked, [=]() {
            QString inputText = mGui->EnterTextLineEdit->text();
            emit signalEnterTextChanged(inputText);
        });
    } else {
        disconnect(mGui->EnterTextOK, nullptr, nullptr, nullptr);
    }
}

void Dialog::connectTestReport(const bool& state) {
    if (state) {
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
    } else {
        disconnect(mGui->TestReportOption1Radio1, nullptr, nullptr, nullptr);
        disconnect(mGui->TestReportOption1Radio2, nullptr, nullptr, nullptr);
        disconnect(mGui->TestReportOK, nullptr, nullptr, nullptr);
        disconnect(mGui->TestReportCancel, nullptr, nullptr, nullptr);
    }
}

void Dialog::connectLogDisplay(const bool& state) {
    if (state) {
        connect(mGui->LogDisplayClose, &QPushButton::clicked, [=]() {
            emit signalLogDisplayClicked(true, false);
            QDialog::accept();
        });
        connect(mGui->LogDisplayCancel, &QPushButton::clicked, [=]() {
            emit signalLogDisplayClicked(false, false);
        });
        connect(mGui->LogDisplayDetail, &QPushButton::clicked, [=]() {
            qDebug() << "signalLogDisplayClicked : detail";
            emit signalLogDisplayClicked(true, true);
        });
    } else {
        disconnect(mGui->LogDisplayClose, nullptr, nullptr, nullptr);
        disconnect(mGui->LogDisplayCancel, nullptr, nullptr, nullptr);
        disconnect(mGui->LogDisplayDetail, nullptr, nullptr, nullptr);
    }
}

void Dialog::connectViewLog(const bool& state) {
    if (state) {
        connect(mGui->actionFindText, &QAction::triggered, [=]() {
            refreshViewLog(RefreshTypeFindShow);
        });
        connect(mGui->ViewLogFindInput, &QTextEdit::textChanged, [=]() {
            refreshViewLog(RefreshTypeSearch);
        });
        connect(mGui->ViewLogFindPrevious, &QPushButton::clicked, [=]() {
            refreshViewLog(RefreshTypeSearchPrevious);
        });
        connect(mGui->ViewLogFindNext, &QPushButton::clicked, [=]() {
            refreshViewLog(RefreshTypeSearchNext);
        });
        connect(mGui->ViewLogClear, &QPushButton::clicked, [=]() {
            refreshViewLog(RefreshTypeClear);
        });
        connect(mGui->ViewLogStop, &QPushButton::clicked, [=]() {
            refreshViewLog(RefreshTypeStop);
        });
        connect(mGui->ViewLogClose, &QPushButton::clicked, [=]() {
            setKeepDialog(false);
            QDialog::accept();
        });
    } else {
        disconnect(mGui->actionFindText, nullptr, nullptr, nullptr);
        disconnect(mGui->ViewLogFindInput, nullptr, nullptr, nullptr);
        disconnect(mGui->ViewLogFindPrevious, nullptr, nullptr, nullptr);
        disconnect(mGui->ViewLogFindNext, nullptr, nullptr, nullptr);
        disconnect(mGui->ViewLogClear, nullptr, nullptr, nullptr);
        disconnect(mGui->ViewLogStop, nullptr, nullptr, nullptr);
        disconnect(mGui->ViewLogClose, nullptr, nullptr, nullptr);
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
        case DialogTypeSelectMoudleInfo: {
            mGui->SelectListWidget->setGeometry(QRect(0, 0, 600, 800));
            rect = mGui->SelectListWidget->geometry();
            break;
        }
        case DialogTypSelectLogFile: {
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
        case DialogTypeViewLogInfo:
        case DialogTypeViewLogFileInfo: {
            rect = mGui->ViewLogWidget->geometry();
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
    setPrevDisplayType(getDisplayType());
    setDisplayType(displayType);
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

void Dialog::updateSelectList(const QString& title, const QStringList& column, const QStringList& list, const bool& all) {
    updateDisplay(DisplayTypeSelectList, title);

    bool headerFixed = true;
    int rowIndex = 0;

    if (getDialogType() == DialogTypeSelectMoudleInfo) {
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
        mGui->SelectListItemList->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    } else {
        mGui->SelectListItemList->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
    }
    mGui->SelectListItemList->setModel(&mModel);
    mGui->SelectListItemList->verticalHeader()->setHidden(true);
    for (rowIndex = 0; rowIndex < mModel.rowCount(); ++rowIndex) {
        mGui->SelectListItemList->verticalHeader()->setSectionResizeMode(rowIndex, QHeaderView::Fixed);
        mGui->SelectListItemList->verticalHeader()->resizeSection(rowIndex, 20);
    }
}

void Dialog::updateSelectOption(const QString& title, const QString& option1, const QStringList& option2) {
    updateDisplay(DisplayTypeSelectOption, title);

    mGui->SelectOption1->setVisible(true);
    mGui->SelectOption1Check->setText(option1);
    mGui->SelectOption1Check->setChecked((getDialogType() != DialogTypeSelectNegative) );

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

void Dialog::updateTestReport(const QString& title, const bool& option1, const QStringList& option2Str,
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

void Dialog::refreshViewLog(const int& refreshType) {
    switch (refreshType) {
        case RefreshTypeFindHide: {
            if (mGui->ViewLogFind->isVisible()) {
                mGui->ViewLogWidget->addAction(mGui->actionFindText);
                mGui->ViewLogFind->setVisible(false);
                mGui->ViewLogContent->setMaximumSize(QSize(16777215, 16777215));
                mGui->ViewLogClear->setVisible(getDialogType() == DialogTypeViewLogInfo);
                mGui->ViewLogStop->setVisible(getDialogType() == DialogTypeViewLogInfo);
            }
            break;
        }
        case RefreshTypeFindShow: {
            if (mGui->ViewLogFind->isVisible() == false) {
                mGui->ViewLogFind->setVisible(true);
                mGui->ViewLogContent->setMaximumSize(QSize(16777215, 520));
                mGui->ViewLogContent->verticalScrollBar()->setValue(mGui->ViewLogContent->verticalScrollBar()->maximum());
                mGui->ViewLogFindInput->setAlignment(Qt::AlignCenter);
                mGui->ViewLogFindInput->setFocus();
            }
            break;
        }
        case RefreshTypeStop: {
            setViewLogStop(getViewLogStop() == false);
            break;
        }
        case RefreshTypeClear: {
            mGui->ViewLogContent->clear();
            break;
        }
        case RefreshTypeSearch:
        case RefreshTypeSearchPrevious:
        case RefreshTypeSearchNext: {
            QString searchText = mGui->ViewLogFindInput->toPlainText();
            QTextCursor cursor = mGui->ViewLogContent->textCursor();
            if (refreshType == RefreshTypeSearch) {
                if (searchText.contains("\n")) {
                    searchText.remove("\n");
                    mGui->ViewLogFindInput->setText(searchText);
                    QTextCursor inputCursor = mGui->ViewLogFindInput->textCursor();
                    inputCursor.movePosition(QTextCursor::End);
                    mGui->ViewLogFindInput->setTextCursor(inputCursor);
                    mGui->ViewLogFindInput->setAlignment(Qt::AlignCenter);
                    cursor = mGui->ViewLogContent->document()->find(searchText, cursor, QTextDocument::FindBackward);
                }
            } else if (refreshType == RefreshTypeSearchNext) {
                cursor = mGui->ViewLogContent->document()->find(searchText, cursor, QTextDocument::FindFlags());
            } else {
                cursor = mGui->ViewLogContent->document()->find(searchText, cursor, QTextDocument::FindBackward);
            }
            if (cursor.isNull() == false) {
                mGui->ViewLogContent->setTextCursor(cursor);
                mGui->ViewLogContent->ensureCursorVisible();
            }
            break;
        }
        default: {
            break;
        }
    }
}

void Dialog::updateViewLog(const QString& title, const QStringList& detailLog) {
    if (getViewLogStop()) {
        return;
    }

    updateDisplay(DisplayTypeViewLog, title);

    QString content = QString();
    for (const auto& log : detailLog) {
        content.append(QString("%1\n").arg(log));
    }

    refreshViewLog(RefreshTypeFindHide);
    mGui->ViewLogContent->insertPlainText(content);
    mGui->ViewLogContent->verticalScrollBar()->setValue(mGui->ViewLogContent->verticalScrollBar()->maximum());
}
