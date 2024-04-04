#include "Dialog.h"
#include "ui_Dialog.h"

Dialog::Dialog(const QRect& rect, QWidget *parent) : QDialog(parent), mGui(new Ui::Dialog) {
    mGui->setupUi(this);
    setScreenRect(rect);
    controlConnet(true);

    this->setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    this->setModal(true);
    this->setFocus();
    this->show();
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
    }
}

void Dialog::drawDialog(const int& dialogType) {
    QString title = QString();
    QRect subRect = QRect();
    switch (dialogType) {
        case DialogTypeAppMode: {
            title = QString("Select App Mode");
            subRect = mGui->AppModeWidget->geometry();
            break;
        }
        case DialogTypeMoudleInfo: {
            title = QString("Select Module");
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
}

void Dialog::updateAppMode(const int& appMode, const QStringList& appModeList) {
    if (mGui->AppModeTableView == nullptr) {
        return;
    }
    drawDialog(DialogTypeAppMode);

    QStringList subTitle = QStringList({"App Mode"});
    mModel.setHorizontalHeaderLabels(subTitle);
    mModel.setColumnCount(subTitle.size());
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