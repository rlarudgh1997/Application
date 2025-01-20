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

    setProperty(DataTypeScreenRect, rect);
}

Dialog::~Dialog() {
    controlConnet(DisplayTypeMax);
    delete mGui;
    qDebug() << "[Dialog] destructor";
}

void Dialog::drawDialog(const int& dialogType, const QVariantList& info) {
    if (getProperty(DataTypeKeepDialog).toBool()) {
        qDebug() << "[Dialog] Current dialog screen maintenance status";
        return;
    }
    setProperty(DataTypePrevDialogType, getProperty(DataTypeDialogType));
    setProperty(DataTypePrevDialogInfo, getProperty(DataTypeDialogInfo));
    setProperty(DataTypeDialogType, dialogType);
    setProperty(DataTypeDialogInfo, info);

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
        case DialogTypeSelectValueResult:
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
        case DialogTypeNodeView:
        case DialogTypeGenType:
        case DialogTypeAutoCompleteNormal: {
            draw = updateAutoCompleteNormal(info);
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
    if (getProperty(DataTypePrevDialogType).toInt() == displayType) {
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
        case DisplayTypeAutoComplete: {
            connectAutoComplete(true);
            break;
        }
        case DisplayTypeAutoCompleteNormal: {
            connectAutoCompleteNormal(true);
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
            connectAutoCompleteNormal(false);
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
                    setProperty(DataTypeAppMode, rowIndex);
                    break;
                }
            }
            emit signalSelectAppMode(getProperty(DataTypeAppMode).toInt());
            QDialog::accept();
        });
        connect(&mModel, &QStandardItemModel::dataChanged,
                [=](const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles) {
                    int appMode = topLeft.row();
                    int currentAppMode = getProperty(DataTypeAppMode).toInt();
                    if (currentAppMode != appMode) {
                        mModel.item(currentAppMode, 0)->setCheckState(Qt::Unchecked);
                        mModel.item(appMode, 0)->setCheckState(Qt::Checked);
                        setProperty(DataTypeAppMode, appMode);
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
                    setProperty(DataTypeAppMode, appMode);
                    break;
                }
                appMode++;
            }
            emit signalSelectAppMode(getProperty(DataTypeAppMode).toInt());
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
        connect(mGui->SelectListInput, &QLineEdit::textChanged, [=](const QString& text) {
            setProperty(DataTypeSelectInputData, text);
            qDebug() << "SelectListInput :" << text;
        });
        connect(mGui->SelectListAll, &QPushButton::clicked,
                [=]() { updateSelectListCheckState((getProperty(DataTypeSelectAll).toBool() == false), QStringList()); });
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
                    if (getProperty(DataTypeMultiCheck).toBool() == false) {
                        int checkModelIndex = topLeft.row();  // bottomRight.row()
                        int preCheckModelIndex = getProperty(DataTypeCheckModelIndex).toInt();
                        if ((preCheckModelIndex != checkModelIndex) && (preCheckModelIndex >= 0)) {
                            mModel.item(preCheckModelIndex, 0)->setCheckState(Qt::Unchecked);
                        }
                        setProperty(DataTypeCheckModelIndex, checkModelIndex);
                    }
                });
    } else {
        disconnect(mGui->SelectListInput, nullptr, nullptr, nullptr);
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
            setProperty(DataTypeKeepDialog, false);
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
        connect(mGui->AutoCompleteInput, &QLineEdit::textChanged, [=](const QString& text) {
            int dialogType = getProperty(DataTypeDialogType).toInt();
            setProperty(DataTypeAutoCompleteListInput, text);

            if (dialogType == DialogTypeNodeView) {
                updateAutoCompleteSuggestionsList(false, true, false, false);
            } else {
                bool sfc = (mGui->ListCheck1->checkState() == Qt::CheckState::Checked);
                bool vehicle = (mGui->ListCheck2->checkState() == Qt::CheckState::Checked);
                bool tcName = (mGui->ListCheck3->checkState() == Qt::CheckState::Checked);
                updateAutoCompleteSuggestionsList(false, sfc, vehicle, tcName);
            }
        });
        connect(mGui->ListCheck1, &QCheckBox::stateChanged, [=](int check) {
            bool sfc = check;
            bool vehicle = (mGui->ListCheck2->checkState() == Qt::CheckState::Checked);
            bool tcName = (mGui->ListCheck3->checkState() == Qt::CheckState::Checked);
            updateAutoCompleteSuggestionsList(false, sfc, vehicle, tcName);
        });
        connect(mGui->ListCheck2, &QCheckBox::stateChanged, [=](int check) {
            bool sfc = (mGui->ListCheck1->checkState() == Qt::CheckState::Checked);
            bool vehicle = check;
            bool tcName = (mGui->ListCheck3->checkState() == Qt::CheckState::Checked);
            updateAutoCompleteSuggestionsList(false, sfc, vehicle, tcName);
        });
        connect(mGui->ListCheck3, &QCheckBox::stateChanged, [=](int check) {
            bool sfc = (mGui->ListCheck1->checkState() == Qt::CheckState::Checked);
            bool vehicle = (mGui->ListCheck2->checkState() == Qt::CheckState::Checked);
            bool tcName = check;
            updateAutoCompleteSuggestionsList(false, sfc, vehicle, tcName);
        });
        connect(mGui->AutoCompleteDisplay, &QLineEdit::returnPressed, [=]() {
            QString currentText = mGui->AutoCompleteDisplay->text();
            emit signalAutoCompleteSelected(currentText);
            QMetaObject::invokeMethod(this, "accept", Qt::QueuedConnection);  // 다이얼로그 종료시 App crash 발생하여 변경
        });
        connect(mGui->AutoCompleteDisplayOK, &QPushButton::clicked, [=]() {
            emit mGui->AutoCompleteDisplay->returnPressed();
        });
        connect(mGui->SelectKeyword, &QComboBox::currentIndexChanged, [=](const int index) {
            QString selectKeyword = (index == 0) ? ("") : (mGui->SelectKeyword->currentText());
            QString currentText = selectKeyword + mGui->AutoCompleteDisplay->text();
            mGui->AutoCompleteDisplay->setText(currentText);
        });
        connect(mGui->AutoCompleteList, &QListWidget::itemPressed, [=](QListWidgetItem* item) {
            if (item) {
                int index = mGui->SelectKeyword->currentIndex();
                QString selectKeyword = (index == 0) ? ("") : (mGui->SelectKeyword->currentText());
                QString currentText = selectKeyword + item->text();
                mGui->AutoCompleteDisplay->setText(currentText);
            }
        });
        connect(mGui->AutoCompleteList, &QListWidget::itemDoubleClicked, [=](QListWidgetItem* item) {
            if (item) {
                emit mGui->AutoCompleteDisplay->returnPressed();
            }
        });

    } else {
        disconnect(mGui->AutoCompleteInput, nullptr, nullptr, nullptr);
        disconnect(mGui->ListCheck1, nullptr, nullptr, nullptr);
        disconnect(mGui->ListCheck2, nullptr, nullptr, nullptr);
        disconnect(mGui->ListCheck3, nullptr, nullptr, nullptr);
        disconnect(mGui->AutoCompleteDisplay, nullptr, nullptr, nullptr);
        disconnect(mGui->AutoCompleteDisplayOK, nullptr, nullptr, nullptr);
        disconnect(mGui->SelectKeyword, nullptr, nullptr, nullptr);
        disconnect(mGui->AutoCompleteList, nullptr, nullptr, nullptr);
    }
}

void Dialog::connectAutoCompleteNormal(const bool& state) {
    if (state) {
        connect(mGui->AutoCompleteNormalInput, &QLineEdit::textChanged, [=](const QString& text) {
            int dialogType = getProperty(DataTypeDialogType).toInt();
            setProperty(DataTypeAutoCompleteListInput, text);
            updateAutoCompleteSuggestionsList(true, false, false, false);
        });
        connect(mGui->AutoCompleteNormalInput, &QLineEdit::returnPressed, [=]() {
            emit mGui->AutoCompleteNormalList->itemDoubleClicked(new QListWidgetItem(mGui->AutoCompleteNormalInput->text()));
        });
        connect(mGui->AutoCompleteNormalList, &QListWidget::itemDoubleClicked, [=](QListWidgetItem* item) {
            if (item) {
                emit signalAutoCompleteSelected(item->text());
                QMetaObject::invokeMethod(this, "accept", Qt::QueuedConnection);  // 다이얼로그 종료시 App crash 발생하여 변경
            }
        });
    } else {
        disconnect(mGui->AutoCompleteNormalInput, nullptr, nullptr, nullptr);
        disconnect(mGui->AutoCompleteNormalList, nullptr, nullptr, nullptr);
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
    int dialogType = getProperty(DataTypeDialogType).toInt();

    switch (dialogType) {
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
        case DialogTypeSelectValueResult:
        case DialogTypeSelectValueEnumOutput: {
            mGui->SelectListWidget->setGeometry(QRect(0, 0, 500, 350));
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
        case DialogTypeNodeView: {
            rect = mGui->AutoCompleteNormalWidget->geometry();
            break;
        }
        case DialogTypeGenType:
        case DialogTypeAutoCompleteNormal: {
            mGui->AutoCompleteNormalWidget->setGeometry(QRect(0, 0, 500, 300));
            rect = mGui->AutoCompleteNormalWidget->geometry();
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
    setProperty(DataTypePrevDisplayType, getProperty(DataTypeDisplayType).toInt());
    setProperty(DataTypeDisplayType, displayType);
    controlConnet(displayType);
    mGui->StackedWidget->setCurrentIndex(displayType);

    if (mainRect.isValid()) {
        QRect screenRect = getProperty(DataTypeScreenRect).toRect();
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
    setProperty(DataTypeSelectAll, allCheck);
}

void Dialog::refreshViewLog(const int& refreshType) {
    switch (refreshType) {
        case RefreshTypeFindHide: {
            if (mGui->ViewLogFind->isVisible()) {
                mGui->ViewLogWidget->addAction(mGui->actionFindText);
                mGui->ViewLogFind->setVisible(false);
                mGui->ViewLogContent->setMaximumSize(QSize(16777215, 16777215));
                bool buttonShow = (getProperty(DataTypeDialogType).toInt() == DialogTypeViewLogInfo);
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
            setProperty(DataTypeViewLogStop, getProperty(DataTypeViewLogStop).toBool());
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

void Dialog::updateAutoCompleteSuggestionsList(const bool& normal, const bool& sfc, const bool& vehicle, const bool& tcName) {
    QStringList autoCompleteList;

    if (normal) {
        mAutoCompleteListWidget = mGui->AutoCompleteNormalList;
        autoCompleteList = getProperty(DataTypeAutoCompleteListNormal).toStringList();
    } else {
        mAutoCompleteListWidget = mGui->AutoCompleteList;
        if (sfc) {
            autoCompleteList.append(getProperty(DataTypeAutoCompleteListSfc).toStringList());
        }
        if (vehicle) {
            autoCompleteList.append(getProperty(DataTypeAutoCompleteListVehicle).toStringList());
        }
        if (tcName) {
            autoCompleteList.append(getProperty(DataTypeAutoCompleteListTCName).toStringList());
        }
    }

    if (mAutoCompleteListWidget) {
        QString inputStr = getProperty(DataTypeAutoCompleteListInput).toString();
        mAutoCompleteListWidget->clear();
        for (const auto& str : autoCompleteList) {
            if ((inputStr.size() > 0)) {
                if (str.contains(inputStr, Qt::CaseInsensitive)) {
                    mAutoCompleteListWidget->addItem(str);
                }
            } else {
                mAutoCompleteListWidget->addItem(str);
            }
        }
        mAutoCompleteListWidget->setCurrentRow(0);
    }
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
    setProperty(DataTypeAppMode, appMode);
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
    int dialogType = getProperty(DataTypeDialogType).toInt();
    bool allCheck = (rowList.size() == selectList.size());
    bool headerFixed = true;
    int rowIndex = 0;
    int columnIndex = 0;
    bool multiCheck = ((dialogType != DialogTypeSelectLogFile) && (dialogType != DialogTypeSelectValueEnumOutput) &&
                       (dialogType != DialogTypeSelectValueResult));

    setProperty(DataTypeMultiCheck, multiCheck);

    setProperty(DataTypeSelectInputData, QVariant());
    mGui->SelectListInput->setVisible(false);

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
    int dialogType = getProperty(DataTypeDialogType).toInt();

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
    updateDisplay(DisplayTypeViewLog, info.at(0).toString());

    QStringList detailLog = info.at(1).toStringList();

    int dialogType = getProperty(DataTypeDialogType).toInt();
    bool viewLogStop = getProperty(DataTypeViewLogStop).toBool();

    setProperty(DataTypeKeepDialog, dialogType == DialogTypeViewLogInfo);

    if (viewLogStop) {
        return true;
    }

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
    if (info.size() != 6) {
        return false;
    }
    updateDisplay(DisplayTypeAutoComplete, info.at(0).toString());

    QString inputStr = info.at(1).toString();
    QStringList sfcList = info.at(2).toStringList();
    QStringList vehicleList = info.at(3).toStringList();
    QStringList tcNameList = info.at(4).toStringList();
    QStringList keywordList = info.at(5).toStringList();

    setProperty(DataTypeAutoCompleteListInput, inputStr);
    setProperty(DataTypeAutoCompleteListSfc, sfcList);
    setProperty(DataTypeAutoCompleteListVehicle, vehicleList);
    setProperty(DataTypeAutoCompleteListTCName, tcNameList);
    setProperty(DataTypeAutoCompleteListKeyword, keywordList);

    mGui->AutoCompleteInput->setText(inputStr);

    int dialogType = getProperty(DataTypeDialogType).toInt();
    if (dialogType == DialogTypeNodeView) {
        mGui->AutoCompleteListCheck->setVisible(false);
        updateAutoCompleteSuggestionsList(false, true, false, false);
    } else {
        mGui->AutoCompleteListCheck->setVisible(true);
        bool keyword = (keywordList.size() > 0);
        mGui->SelectKeyword->clear();
        mGui->SelectKeyword->addItems(keywordList);
        mGui->SelectKeyword->setCurrentIndex(0);

        bool sfc = (sfcList.size() > 0);
        mGui->ListCheck1->setCheckState((sfc) ? (Qt::CheckState::Checked) : (Qt::CheckState::Unchecked));
        mGui->ListCheck1->setEnabled(sfc);
        mGui->ListCheck1->setText("SFC");

        bool vehicle = (vehicleList.size() > 0);
        mGui->ListCheck2->setCheckState((vehicle) ? (Qt::CheckState::Checked) : (Qt::CheckState::Unchecked));
        mGui->ListCheck2->setEnabled(vehicle);
        mGui->ListCheck2->setText("Vehicle");

        bool tcName = (tcNameList.size() > 0);
        mGui->ListCheck3->setCheckState((tcName) ? (Qt::CheckState::Checked) : (Qt::CheckState::Unchecked));
        mGui->ListCheck3->setEnabled(tcName);
        mGui->ListCheck3->setText("TCName");

        mGui->AutoCompleteListCheckTitle->setText("[Signal Type]");
        mGui->AutoCompleteInputTitle->setText("Search :");
        mGui->AutoCompleteDisplayTitle->setText("Select  :");

        updateAutoCompleteSuggestionsList(false, sfc, vehicle, tcName);
    }

    return true;
}

bool Dialog::updateAutoCompleteNormal(const QVariantList& info) {
    if (info.size() != 3) {
        return false;
    }
    updateDisplay(DisplayTypeAutoCompleteNormal, info.at(0).toString());

    QString inputStr = info.at(1).toString();
    QStringList list = info.at(2).toStringList();

    setProperty(DataTypeAutoCompleteListInput, inputStr);
    setProperty(DataTypeAutoCompleteListNormal, list);

    int dialogType = getProperty(DataTypeDialogType).toInt();
    mGui->AutoCompleteNormalInput->setVisible(dialogType != DialogTypeGenType);
    mGui->AutoCompleteNormalInput->setText(inputStr);

    updateAutoCompleteSuggestionsList(true, false, false, false);

    return true;
}
