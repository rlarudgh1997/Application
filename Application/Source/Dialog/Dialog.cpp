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

#if defined(USE_DIALOG_PROPERTY)
    setScreenRect(rect);
#else
    setProperty(DataTypeScreenRect, rect);
#endif
}

Dialog::~Dialog() {
    controlConnet(DisplayTypeMax);
    delete mGui;
    qDebug() << "[Dialog] destructor";
}

void Dialog::drawDialog(const int& dialogType, const QVariantList& info) {
#if defined(USE_DIALOG_PROPERTY)
    if (getKeepDialog()) {
        qDebug() << "[Dialog] Current dialog screen maintenance status";
        return;
    }
    setPrevDialogType(getDialogType());
    setPrevDialogInfo(getDialogInfo());
    setDialogType(dialogType);
    setDialogInfo(info);
#else
    if (getProperty(DataTypeKeepDialog).toBool()) {
        qDebug() << "[Dialog] Current dialog screen maintenance status";
        return;
    }
    setProperty(DataTypePrevDialogType, getProperty(DataTypeDialogType));
    setProperty(DataTypePrevDialogInfo, getProperty(DataTypeDialogInfo));
    setProperty(DataTypeDialogType, dialogType);
    setProperty(DataTypeDialogInfo, info);
#endif

    bool draw = false;
    switch (dialogType) {
        case DialogTypeAppModeCheck: {
            draw = updateAppMode(info);
            break;
        }
        case DialogTypeAppModeRadio: {
            draw = updateAppModeRadio(info);
            break;
        }
        case DialogTypeSelectMoudleInfo:
        case DialogTypeSelectLogFile:
        case DialogTypeSelectValueEnumOutput:
        case DialogTypeSelectValueEnumInput:
        case DialogTypeSelectMatchingTableCV:
        case DialogTypeSelectMatchingTablePV: {
            draw = updateSelectList(info);
            break;
        }
        case DialogTypeSelectNegative:
        case DialogTypeSelectVehiclePV:
        case DialogTypeSelectVehicleCV:
        case DialogTypeSelectVehicleType: {
            draw = updateSelectOption(info);
            break;
        }
        case DialogTypeEnterText: {
            draw = updateInputText(info);
            break;
        }
        case DialogTypeTestReportTC:
        case DialogTypeTestReportGCOV: {
            draw = updateTestReport(info);
            break;
        }
        case DialogTypeLogDisplay: {
            draw = updateLogDisplay(info);
            break;
        }
        case DialogTypeViewLogInfo:
        case DialogTypeViewLogFileInfo: {
            draw = updateViewLog(info);
            break;
        }
        case DialogTypeAutoComplete: {
            draw = updateAutoComplete(info);
            break;
        }
        default: {
            qDebug() << "\t Fail to dialog type :" << dialogType;
            break;
        }
    }
    qDebug() << "[Dialog] drawDialog :" << dialogType << info.size() << ((draw) ? ("-> Success") : ("-> Fail"));
}

void Dialog::controlConnet(const int& displayType) {
#if defined(USE_DIALOG_PROPERTY)
    if (getPrevDisplayType() == displayType) {
        qDebug() << "[Dialog] Skip to request for same screen as previous screen :" << displayType;
        return;
    }
#else
    if (getProperty(DataTypePrevDialogType).toInt() == displayType) {
        qDebug() << "[Dialog] Skip to request for same screen as previous screen :" << displayType;
        return;
    }
#endif

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
        case DisplayTypeAutoComplete: {
            connectAutoComplete(true);
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
            connectAutoComplete(false);
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
#if defined(USE_DIALOG_PROPERTY)
                    setAppMode(rowIndex);
#else
                    setProperty(DataTypeAppMode, rowIndex);
#endif
                    break;
                }
            }
#if defined(USE_DIALOG_PROPERTY)
            emit signalSelectAppMode(getAppMode());
#else
            emit signalSelectAppMode(getProperty(DataTypeAppMode).toInt());
#endif
            QDialog::accept();
        });
        connect(&mModel, &QStandardItemModel::dataChanged,
                [=](const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles) {
                    int appMode = topLeft.row();
#if defined(USE_DIALOG_PROPERTY)
                    int currentAppMode = getAppMode();
#else
                    int currentAppMode = getProperty(DataTypeAppMode).toInt();
#endif
                    if (currentAppMode != appMode) {
                        mModel.item(currentAppMode, 0)->setCheckState(Qt::Unchecked);
                        mModel.item(appMode, 0)->setCheckState(Qt::Checked);
#if defined(USE_DIALOG_PROPERTY)
                        setAppMode(appMode);
#else
                        setProperty(DataTypeAppMode, appMode);
#endif
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
#if defined(USE_DIALOG_PROPERTY)
                    setAppMode(appMode);
#else
                    setProperty(DataTypeAppMode, appMode);
#endif
                    break;
                }
                appMode++;
            }
#if defined(USE_DIALOG_PROPERTY)
            emit signalSelectAppMode(getAppMode());
#else
            emit signalSelectAppMode(getProperty(DataTypeAppMode).toInt());
#endif
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
#if defined(USE_DIALOG_PROPERTY)
            updateSelectListCheckState((getSelectAll() == false), QStringList());
#else
            updateSelectListCheckState((getProperty(DataTypeSelectAll).toBool() == false), QStringList());
#endif
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
        connect(mGui->SelectListItemList->verticalScrollBar(), &QAbstractSlider::valueChanged, [=](int value) {
            // emit signalScrollBarValueChanged(mGui->SelectListItemList->verticalScrollBar()->value());
            emit signalScrollBarValueChanged(value);
        });
        connect(&mModel, &QStandardItemModel::dataChanged,
                [=](const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles) {
#if defined(USE_DIALOG_PROPERTY)
                    if (getMultiCheck() == false) {
                        int checkModelIndex = topLeft.row();  // bottomRight.row()
                        int preCheckModelIndex = getCheckModelIndex();
                        if ((preCheckModelIndex != checkModelIndex) && (preCheckModelIndex >= 0)) {
                            mModel.item(preCheckModelIndex, 0)->setCheckState(Qt::Unchecked);
                        }
                        setCheckModelIndex(checkModelIndex);
                    }
#else
                    if (getProperty(DataTypeMultiCheck).toBool() == false) {
                        int checkModelIndex = topLeft.row();  // bottomRight.row()
                        int preCheckModelIndex = getProperty(DataTypeCheckModelIndex).toInt();
                        if ((preCheckModelIndex != checkModelIndex) && (preCheckModelIndex >= 0)) {
                            mModel.item(preCheckModelIndex, 0)->setCheckState(Qt::Unchecked);
                        }
                        setProperty(DataTypeCheckModelIndex, checkModelIndex);
                    }
#endif
                });
    } else {
        disconnect(mGui->SelectListAll, nullptr, nullptr, nullptr);
        disconnect(mGui->SelectListOK, nullptr, nullptr, nullptr);
        disconnect(mGui->SelectListItemList->verticalScrollBar(), nullptr, nullptr, nullptr);
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
        connect(mGui->SelectOptionCancel, &QPushButton::clicked, [=]() { QDialog::accept(); });
    } else {
        disconnect(mGui->SelectOptionOK, nullptr, nullptr, nullptr);
        disconnect(mGui->SelectOptionCancel, nullptr, nullptr, nullptr);
    }
}

void Dialog::connectEnterText(const bool& state) {
    if (state) {
        connect(mGui->EnterTextOK, &QPushButton::clicked,
                [=]() { emit signalEnterTextChanged(mGui->EnterTextLineEdit->text()); });
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
        connect(mGui->TestReportCancel, &QPushButton::clicked, [=]() { QDialog::accept(); });
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
        connect(mGui->LogDisplayCancel, &QPushButton::clicked, [=]() { emit signalLogDisplayClicked(false, false); });
        connect(mGui->LogDisplayDetail, &QPushButton::clicked, [=]() { emit signalLogDisplayClicked(true, true); });
    } else {
        disconnect(mGui->LogDisplayClose, nullptr, nullptr, nullptr);
        disconnect(mGui->LogDisplayCancel, nullptr, nullptr, nullptr);
        disconnect(mGui->LogDisplayDetail, nullptr, nullptr, nullptr);
    }
}

void Dialog::connectViewLog(const bool& state) {
    if (state) {
        connect(mGui->actionFindText, &QAction::triggered, [=]() { refreshViewLog(RefreshTypeFindShow); });
        connect(mGui->ViewLogFindInput, &QTextEdit::textChanged, [=]() { refreshViewLog(RefreshTypeSearch); });
        connect(mGui->ViewLogFindPrevious, &QPushButton::clicked, [=]() { refreshViewLog(RefreshTypeSearchPrevious); });
        connect(mGui->ViewLogFindNext, &QPushButton::clicked, [=]() { refreshViewLog(RefreshTypeSearchNext); });
        connect(mGui->ViewLogClear, &QPushButton::clicked, [=]() { refreshViewLog(RefreshTypeClear); });
        connect(mGui->ViewLogStop, &QPushButton::clicked, [=]() { refreshViewLog(RefreshTypeStop); });
        connect(mGui->ViewLogClose, &QPushButton::clicked, [=]() {
#if defined(USE_DIALOG_PROPERTY)
            setKeepDialog(false);
#else
            setProperty(DataTypeKeepDialog, false);
#endif
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

void Dialog::connectAutoComplete(const bool& state) {
    if (state) {
        connect(mGui->AutoCompleteInput, &QLineEdit::textChanged,
                [=](const QString& text) { updateAutoCompleteSuggestionsList(text); });
        connect(mGui->AutoCompleteInput, &QLineEdit::returnPressed, [=]() {
            emit signalAutoCompleteSelected(mGui->AutoCompleteInput->text());
            QDialog::accept();
        });
        connect(mGui->AutoCompleteList, &QListWidget::itemDoubleClicked, [=](QListWidgetItem* item) {
            if (item) {
                emit signalAutoCompleteSelected(item->text());
            }
            // QDialog::accept();    // App crash
            QMetaObject::invokeMethod(this, "accept", Qt::QueuedConnection);  // 다이얼로그 종료를 안전하게 큐에 넣기
        });
    } else {
        disconnect(mGui->AutoCompleteInput, nullptr, nullptr, nullptr);
        disconnect(mGui->AutoCompleteList, nullptr, nullptr, nullptr);
    }
}

QVariant Dialog::getData(const int& type) {
    return mData[type];
}

void Dialog::setData(const int& type, const QVariant& value) {
    if (mData[type] != value) {
        mData[type] = value;
    }
}

QRect Dialog::updateMainRect() {
    QRect rect = QRect();
#if defined(USE_DIALOG_PROPERTY)
    switch (getDialogType()) {
#else
    switch (getProperty(DataTypeDialogType).toInt()) {
#endif
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
        case DialogTypeSelectLogFile:
        case DialogTypeSelectValueEnumInput:
        case DialogTypeSelectValueEnumOutput: {
            mGui->SelectListWidget->setGeometry(QRect(0, 0, 500, 300));
            rect = mGui->SelectListWidget->geometry();
            break;
        }
        case DialogTypeSelectMatchingTableCV: {
            mGui->SelectListWidget->setGeometry(QRect(0, 0, 664, 300));
            rect = mGui->SelectListWidget->geometry();
            break;
        }
        case DialogTypeSelectMatchingTablePV: {
            mGui->SelectListWidget->setGeometry(QRect(0, 0, 864, 300));
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
        case DialogTypeSelectVehicleType: {
            mGui->SelectOptionWidget->setGeometry(QRect(0, 0, 400, 110));
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
        case DialogTypeAutoComplete: {
            rect = mGui->AutoCompleteWidget->geometry();
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
#if defined(USE_DIALOG_PROPERTY)
    setPrevDisplayType(getDisplayType());
    setDisplayType(displayType);
#else
    setProperty(DataTypePrevDisplayType, getProperty(DataTypeDisplayType).toInt());
    setProperty(DataTypeDisplayType, displayType);
#endif
    controlConnet(displayType);
    mGui->StackedWidget->setCurrentIndex(displayType);

    if (mainRect.isValid()) {
#if defined(USE_DIALOG_PROPERTY)
        QRect screenRect = getScreenRect();
#else
        QRect screenRect = getProperty(DataTypeScreenRect).toRect();
#endif
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
            {mGui->SelectOption2Check1}, {mGui->SelectOption2Check2}, {mGui->SelectOption2Check3},
            {mGui->SelectOption2Check4}, {mGui->SelectOption2Check5},
        };
    } else {
        widgetList = QList<QCheckBox*>{
            {mGui->TestReportOption2Check1}, {mGui->TestReportOption2Check2}, {mGui->TestReportOption2Check3},
            {mGui->TestReportOption2Check4}, {mGui->TestReportOption2Check5},
        };
    }
    return widgetList;
}

void Dialog::updateSelectListCheckState(const bool& allCheck, const QStringList& selectList) {
    mGui->SelectListAll->setText((allCheck) ? ("Unselect All") : ("Select All"));
    for (int rowIndex = 0; rowIndex < mModel.rowCount(); rowIndex++) {
        if (mModel.item(rowIndex, 0) == nullptr) {
            continue;
        }
        if (selectList.size() == 0) {
            mModel.item(rowIndex, 0)->setCheckState((allCheck) ? (Qt::Checked) : (Qt::Unchecked));
        } else {
            QString selectName = mModel.item(rowIndex, 0)->text();
            bool select = selectList.contains(selectName);
            mModel.item(rowIndex, 0)->setCheckState((select) ? (Qt::Checked) : (Qt::Unchecked));
        }
    }
#if defined(USE_DIALOG_PROPERTY)
    setSelectAll(allCheck);
#else
    setProperty(DataTypeSelectAll, allCheck);
#endif
}

void Dialog::refreshViewLog(const int& refreshType) {
    switch (refreshType) {
        case RefreshTypeFindHide: {
            if (mGui->ViewLogFind->isVisible()) {
                mGui->ViewLogWidget->addAction(mGui->actionFindText);
                mGui->ViewLogFind->setVisible(false);
                mGui->ViewLogContent->setMaximumSize(QSize(16777215, 16777215));
#if defined(USE_DIALOG_PROPERTY)
                bool buttonShow = (getDialogType() == DialogTypeViewLogInfo);
#else
                bool buttonShow = (getProperty(DataTypeDialogType).toInt() == DialogTypeViewLogInfo);
#endif
                mGui->ViewLogClear->setVisible(buttonShow);
                mGui->ViewLogStop->setVisible(buttonShow);
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
#if defined(USE_DIALOG_PROPERTY)
            setViewLogStop(getViewLogStop() == false);
#else
            setProperty(DataTypeViewLogStop, getProperty(DataTypeViewLogStop).toBool());
#endif
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

void Dialog::updateAutoCompleteSuggestionsList(const QString& inputStr) {
    mGui->AutoCompleteList->clear();
#if defined(USE_DIALOG_PROPERTY)
    for (const auto& str : getAutoCompleteList()) {
#else
    for (const auto& str : getProperty(DataTypeAutoCompleteList).toStringList()) {
#endif
        if ((inputStr.size() > 0)) {
            if (str.contains(inputStr, Qt::CaseInsensitive)) {
                mGui->AutoCompleteList->addItem(str);
            }
        } else {
            mGui->AutoCompleteList->addItem(str);
        }
    }
    mGui->AutoCompleteList->setCurrentRow(0);
}

bool Dialog::updateAppMode(const QVariantList& info) {
    if (info.size() != 3) {
        return false;
    }

    updateDisplay(DisplayTypeAppMode, info.at(0).toString());
    int appMode = info.at(1).toInt();
    QStringList appModeList = info.at(2).toStringList();

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
#if defined(USE_DIALOG_PROPERTY)
    setAppMode(appMode);
#else
    setProperty(DataTypeAppMode, appMode);
#endif
    mGui->AppModeTableView->setModel(&mModel);
    mGui->AppModeTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    mGui->AppModeTableView->verticalHeader()->setHidden(true);
    for (rowIndex = 0; rowIndex < mModel.rowCount(); ++rowIndex) {
        mGui->AppModeTableView->verticalHeader()->setSectionResizeMode(rowIndex, QHeaderView::Fixed);
        mGui->AppModeTableView->verticalHeader()->resizeSection(rowIndex, 20);
    }

    return true;
}

bool Dialog::updateAppModeRadio(const QVariantList& info) {
    if (info.size() != 3) {
        return false;
    }

    updateDisplay(DisplayTypeAppModeRadio, info.at(0).toString());
    int appMode = info.at(1).toInt();
    QStringList appModeList = info.at(2).toStringList();

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

    return true;
}

bool Dialog::updateSelectList(const QVariantList& info) {
    if (info.size() != 6) {
        return false;
    }

    updateDisplay(DisplayTypeSelectList, info.at(0).toString());
    QStringList columnList = info.at(1).toStringList();
    QStringList rowList = info.at(2).toStringList();
    QStringList selectList = info.at(3).toStringList();
    QVariantList subList = info.at(4).toList();
    int scrolBarValue = info.at(5).toInt();
#if defined(USE_DIALOG_PROPERTY)
    int dialogType = getDialogType();
#else
    int dialogType = getProperty(DataTypeDialogType).toInt();
#endif
    bool allCheck = (rowList.size() == selectList.size());
    bool headerFixed = true;
    int rowIndex = 0;
    int columnIndex = 0;
    bool multiCheck = ((dialogType != DialogTypeSelectLogFile) && (dialogType != DialogTypeSelectValueEnumOutput));

#if defined(USE_DIALOG_PROPERTY)
    setMultiCheck(multiCheck);
#else
    setProperty(DataTypeMultiCheck, multiCheck);
#endif

    mGui->SelectListAll->setVisible(multiCheck);
    mModel.setHorizontalHeaderLabels(columnList);
    mModel.setColumnCount(columnList.size());
    mModel.setRowCount(rowList.size());
    for (const auto& rowValue : rowList) {
        delete mModel.item(rowIndex, columnIndex);
        mModel.setItem(rowIndex, columnIndex, new QStandardItem(rowValue));
        mModel.item(rowIndex, columnIndex)->setCheckable(true);
        mModel.item(rowIndex, columnIndex)->setFlags(mModel.item(rowIndex, columnIndex)->flags() & ~Qt::ItemFlag::ItemIsEditable);
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
    mGui->SelectListItemList->verticalScrollBar()->setValue(scrolBarValue);

    // Draw Select List Check
    updateSelectListCheckState(allCheck, selectList);

    // Draw Matching Table
    if (subList.size() > 0) {
        columnIndex = 0;
        for (const auto& matchingTable : subList) {
            mGui->SelectListItemList->horizontalHeader()->setSectionResizeMode(columnIndex, QHeaderView::Fixed);
            if (columnIndex == 0) {
                mGui->SelectListItemList->horizontalHeader()->resizeSection(columnIndex, 240);
            } else {
                rowIndex = 0;
                mGui->SelectListItemList->horizontalHeader()->resizeSection(columnIndex, 100);
                for (const auto& matchingTableValue : matchingTable.toStringList()) {
                    delete mModel.item(rowIndex, columnIndex);
                    mModel.setItem(rowIndex, columnIndex, new QStandardItem(matchingTableValue));
                    // mModel.item(rowIndex, columnIndex)->setCheckable(false);
                    mModel.item(rowIndex, columnIndex)
                        ->setFlags(mModel.item(rowIndex, columnIndex)->flags() & ~Qt::ItemFlag::ItemIsEditable);
                    rowIndex++;
                }
            }
            columnIndex++;
        }
    }
    return true;
}

bool Dialog::updateSelectOption(const QVariantList& info) {
    if (info.size() != 3) {
        return false;
    }

    updateDisplay(DisplayTypeSelectOption, info.at(0).toString());
    QString option1 = info.at(1).toString();
    QStringList option2 = info.at(2).toStringList();
#if defined(USE_DIALOG_PROPERTY)
    int dialogType = getDialogType();
#else
    int dialogType = getProperty(DataTypeDialogType).toInt();
#endif

    mGui->SelectOption1->setVisible(dialogType != DialogTypeSelectVehicleType);
    mGui->SelectOption1Check->setText(option1);
    mGui->SelectOption1Check->setChecked((dialogType != DialogTypeSelectNegative));

    int index = 0;
    mGui->SelectOption2->setVisible(option2.size() > 0);
    for (const auto& check : isCheckWidget(true)) {
        check->setVisible(index < option2.size());
        if (check->isVisible()) {
            check->setText(option2.at(index));
            index++;
        }
    }

    return true;
}

bool Dialog::updateInputText(const QVariantList& info) {
    if (info.size() != 1) {
        return false;
    }

    updateDisplay(DisplayTypeEnterText, info.at(0).toString());

    return true;
}

bool Dialog::updateTestReport(const QVariantList& info) {
    if (info.size() != 4) {
        return false;
    }

    updateDisplay(DisplayTypeTestReport, info.at(0).toString());
    bool option1 = info.at(1).toBool();
    QStringList option2Str = info.at(2).toStringList();
    QVariantList option2Value = info.at(3).toList();

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

    return true;
}

bool Dialog::updateLogDisplay(const QVariantList& info) {
    if (info.size() != 4) {
        return false;
    }

    updateDisplay(DisplayTypeLogDisplay, info.at(0).toString());
    QString titleInfo = info.at(1).toString();
    QString errorInfo = info.at(2).toString();
    QString moduleStateInfo = info.at(3).toString();

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

    return true;
}

bool Dialog::updateViewLog(const QVariantList& info) {
    if (info.size() != 2) {
        return false;
    }

#if defined(USE_DIALOG_PROPERTY)
    int dialogType = getDialogType();
    bool viewLogStop = getViewLogStop();

    setKeepDialog(dialogType == DialogTypeViewLogInfo);
#else
    int dialogType = getProperty(DataTypeDialogType).toInt();
    bool viewLogStop = getProperty(DataTypeViewLogStop).toBool();

    setProperty(DataTypeKeepDialog, dialogType == DialogTypeViewLogInfo);
#endif

    if (viewLogStop) {
        return true;
    }

    updateDisplay(DisplayTypeViewLog, info.at(0).toString());
    QStringList detailLog = info.at(1).toStringList();

    QString content = QString();
    for (const auto& log : detailLog) {
        content.append(QString("%1\n").arg(log));
    }

    refreshViewLog(RefreshTypeFindHide);
    mGui->ViewLogContent->insertPlainText(content);
    mGui->ViewLogContent->verticalScrollBar()->setValue(mGui->ViewLogContent->verticalScrollBar()->maximum());

    return true;
}

bool Dialog::updateAutoComplete(const QVariantList& info) {
    if (info.size() != 3) {
        return false;
    }

    updateDisplay(DisplayTypeAutoComplete, info.at(0).toString());
    QString inputStr = info.at(1).toString();
    QStringList signalList = info.at(2).toStringList();

#if defined(USE_DIALOG_PROPERTY)
    setAutoCompleteList(signalList);
#else
    setProperty(DataTypeAutoCompleteList, signalList);
#endif
    mGui->AutoCompleteInput->setText(inputStr);
    updateAutoCompleteSuggestionsList(inputStr);

    return true;
}
