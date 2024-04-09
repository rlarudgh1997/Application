#include "Dialog.h"
#include "ui_Dialog.h"

Dialog::Dialog(const QRect& rect, QWidget *parent) : QDialog(parent), mGui(new Ui::Dialog) {
    this->setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    this->setModal(true);

    mGui->setupUi(this);
    setScreenRect(rect);
    controlConnet(true);
}

Dialog::~Dialog() {
    controlConnet(false);
    delete mGui;
}

void Dialog::controlConnet(const bool& state) {
    if (state) {
    } else {
        disconnect(&mModel);
        disconnect(mGui->AppModeOK);
        disconnect(mGui->AppModeRadioOK);
        for (const auto& widget : isRadioWidget()) {
            disconnect(widget.second);
        }
    }
}

void Dialog::drawDialog(const int& dialogType, const QString& title) {
    QRect subRect = QRect();
    switch (dialogType) {
        case DialogTypeAppMode: {
            subRect = mGui->AppModeWidget->geometry();
            break;
        }
        case DialogTypeRadioButton: {
            subRect = mGui->AppModeRadioWidget->geometry();
            break;
        }
        case DialogTypeMoudleInfo: {
            break;
        }
        default: {
            break;
        }
    }
    mGui->Widget->setCurrentIndex(dialogType);
    setWindowTitle(title);
    if (subRect.isValid()) {
        QRect setRect = QRect();
        QRect screenRect = getScreenRect();
        setRect.setX(static_cast<int>(screenRect.x() + (screenRect.width() - subRect.width()) * 0.5));
        setRect.setY(static_cast<int>(screenRect.y() + (screenRect.height() - subRect.height()) * 0.5));
        this->setFixedSize(QSize(subRect.width(), subRect.height()));
        this->setGeometry(setRect);
    }
    this->setFocus();
    this->show();
}

QList<QPair<QFrame*, QRadioButton*>> Dialog::isRadioWidget() const {
    QList<QPair<QFrame*, QRadioButton*>> widgetList = {
        {mGui->FrameList1, mGui->AppModeRadioButton1},
        {mGui->FrameList2, mGui->AppModeRadioButton2},
        {mGui->FrameList3, mGui->AppModeRadioButton3},
        {mGui->FrameList4, mGui->AppModeRadioButton4},
        {mGui->FrameList5, mGui->AppModeRadioButton5},
        {mGui->FrameList6, mGui->AppModeRadioButton6},
        {mGui->FrameList7, mGui->AppModeRadioButton7},
        {mGui->FrameList8, mGui->AppModeRadioButton8}
    };
    return widgetList;
}

void Dialog::updateAppMode(const int& appMode, const QStringList& appModeList) {
    if (mGui->AppModeTableView == nullptr) {
        return;
    }
    drawDialog(DialogTypeAppMode, QString("Select App Mode"));
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
}

void Dialog::updateAppModeRadio(const int& appMode, const QStringList& appModeList) {
    if (mGui->AppModeRadioWidget == nullptr) {
        return;
    }
    drawDialog(DialogTypeRadioButton, QString("Select Vehicle"));
    mGui->AppModeRadioTitle->setText("Vehicle");
    int index = 0;
    for (const auto& widget : isRadioWidget()) {
        widget.first->setVisible(index < appModeList.size());
        widget.second->setChecked(appMode == index);
        if (widget.first->isVisible()) {
            widget.second->setText(appModeList.at(index));
        }
        index++;

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
}
