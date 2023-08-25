#include "GuiCenter.h"
#include "AbstractHandler.h"

#include "CommonEnum.h"
#include "CommonResource.h"

#include <QStringListModel>
#include <QCompleter>
// #include <QTextEdit>
// #include <QStandardItemModel>
// #include <QStandardItem>
// #include <QListWidget>
// #include <QVBoxLayout>
// #include <QStyledItemDelegate>


QSharedPointer<GuiCenter>& GuiCenter::instance(AbstractHandler* handler) {
    static QSharedPointer<GuiCenter> gGui;
    if (gGui.isNull()) {
        gGui = QSharedPointer<GuiCenter>(new GuiCenter(handler));
    }
    return gGui;
}

GuiCenter::GuiCenter(AbstractHandler* handler) : AbstractGui(handler) {
   mMainView = new QStackedWidget(isHandler()->getScreen());
    // mMainView = new QTableWidget(isHandler()->getScreen());
    updateDisplaySize();
}

void GuiCenter::drawDisplayDepth0() {
    updateDisplayVisible();
#if 0
    mMainView->setRowCount(10);
    mMainView->setColumnCount(4);


    QTableWidgetItem *newItem = new QTableWidgetItem(QString("Text1"));
    newItem->setIcon(QIcon(QPixmap(IAMGE_SAVE)));
    newItem->setTextAlignment(Qt::AlignVCenter);
    mMainView->setItem(0, 1, newItem);
#else
    if (mConfigWidget == nullptr) {
        mConfigWidget = new QWidget();
        mConfigWidget->setGeometry(0, 0, mMainView->geometry().width(), mMainView->geometry().height());
        // mConfigWidget->setStyleSheet("background-color: blue");
        mConfigWidget->show();
        mMainView->insertWidget(0, mConfigWidget);
    }

    if (mNodeAddressList == nullptr) {
        mNodeAddressList = new QListView();
        mNodeAddressList->setGeometry(0, 0, mMainView->geometry().width(), mMainView->geometry().height());
        // mNodeAddressList->setStyleSheet("background-color: green");
        mNodeAddressList->show();
        mMainView->insertWidget(1, mNodeAddressList);
    }
#endif

    if (mInputNodeAddress == nullptr) {
        mInputNodeAddress = new QLineEdit(mNodeAddressList);
        mInputNodeAddress->setGeometry(400, 20, 840, 50);
        mInputNodeAddress->setStyleSheet("background-color: white; color: black; font: bold; font-size:15px");
        mInputNodeAddress->show();
    }

    if (mConfigHideButton == nullptr) {
        mConfigHideButton = new QPushButton(mMainView);
        mConfigHideButton->setGeometry(1250, 20, 50, 50);
        mConfigHideButton->setStyleSheet("background-color: rgb(255, 255, 255); color: black; font: bold; font-size:20px");
        mConfigHideButton->setText("X");
        connect(mConfigHideButton, &QPushButton::clicked, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeViewInfoClose, QVariant(mMainView->currentIndex()));
        });
        mConfigHideButton->show();
        // mConfigHideButton->raise();
    }
    // mMainView->stackUnder(mConfigHideButton);
    // mConfigWidget->stackUnder(mConfigHideButton);
    // mNodeAddressList->stackUnder(mConfigHideButton);
}

void GuiCenter::drawDisplayDepth1() {
    qDebug() << "GuiCenter::drawDisplayDepth1()";
}

void GuiCenter::drawDisplayDepth2() {
    qDebug() << "GuiCenter::drawDisplayDepth2()";
}

void GuiCenter::updateDisplaySize() {
    QSize size = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize).toSize();
    QSize margin = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeDisplaySizeMargin).toSize();
    QRect rect = isHandler()->getScreen()->geometry();

    if (margin.width() != 0) {
        rect.setX(margin.width());
        rect.setWidth(size.width() - margin.width());
    }
    if (margin.height() != 0) {
        rect.setY(margin.height());
        rect.setHeight(size.height() - margin.height());
    }
    mMainView->setGeometry(rect);
}

void GuiCenter::updateDisplayVisible() {
    if (isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeVisible).toBool()) {
        mMainView->show();
    } else {
        mMainView->hide();
    }
}

void GuiCenter::updateDisplayConfigInfo() {
    QVariantList configValue = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeConfigInfo).toList();
    qDebug() << "GuiCenter::updateDisplayConfigInfo() ->" << mConfigValue.size() << "," << configValue.size();

    if (mConfigWidget == nullptr) {
        return;
    } else {
        int viewType = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeViewType).toInt();
        mMainView->setCurrentIndex(viewType);
        if (mNodeAddressList) {
            mNodeAddressList->hide();
        }
        mConfigWidget->raise();
        mConfigHideButton->raise();

        if (mConfigValue == configValue) {
            foreach(const auto& item, mConfigListItem) {
                item->initStyle();
            }
            return;
        }
    }

    bool newItem = (mConfigValue.size() != configValue.size());
    if (newItem) {
        foreach(const auto& item, mConfigListItem) {
            item->clear();
        }
        mConfigListItem.clear();
    }

    int index = 0;
    mConfigValue = configValue;
    foreach(const auto& info, configValue) {
        QVariantList config = info.toList();
        if (config.size() != 3) {
            continue;
        }
        int type = config.at(0).toInt();
        QString name = config.at(1).toString();
        QVariant value = config.at(2);
        QString realValue = QString();
        switch (value.type()) {
            case QVariant::Type::List : {
                foreach(const auto& v, value.toList()) {
                    realValue.append(QString("%1, ").arg(v.toString()));
                }
                realValue.resize(realValue.size() - 2);
                break;
            }
            case QVariant::Type::StringList : {
                foreach(const auto& v, value.toStringList()) {
                    realValue.append(QString("%1, ").arg(v));
                }
                realValue.resize(realValue.size() - 2);
                break;
            }
            case QVariant::Type::Rect : {
                QRect rect = value.toRect();
                realValue.append(QString("%1, %2, %3, %4").arg(rect.x()).arg(rect.y()).arg(rect.width()).arg(rect.height()));
                break;
            }
            default : {
                realValue = value.toString();
                break;
            }
        }

        if (newItem) {
            mConfigListItem[index] = new ListItem(index, type, name, realValue, mConfigWidget);
            connect(mConfigListItem[index], &ListItem::signalValueChanged, [=](const int& type, const QVariant& value) {
                createSignal(ivis::common::EventTypeEnum::EventTypeWriteConfig, QVariant(QVariantList{type, value}));
            });
        } else {
            if (mConfigListItem[index]) {
                mConfigListItem[index]->setData(name, realValue);
            }
        }
        index++;
    }
}

void GuiCenter::updateDisplayNodeAddress() {
    QStringList nodeAddress = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeSignalListAll).toStringList();
    qDebug() << "GuiCenter::updateDisplayNodeAddress() ->" << nodeAddress.size();

    if (mNodeAddressList == nullptr) {
        return;
    } else {
        int viewType = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeViewType).toInt();
        mMainView->setCurrentIndex(viewType);
        if (mConfigWidget) {
            mConfigWidget->hide();
        }
        mNodeAddressList->raise();
        mInputNodeAddress->raise();
        mConfigHideButton->raise();
    }

    static QStringListModel* mNodeAddressModel = new QStringListModel();
    mNodeAddressModel->setStringList(nodeAddress);
    mNodeAddressList->setModel(mNodeAddressModel);
    // mNodeAddressList->setItemDelegate(new QStyledItemDelegate(mNodeAddressList));

    static QCompleter* autoComplete = nullptr;
    if (autoComplete == nullptr) {
        autoComplete = new QCompleter(nodeAddress, mInputNodeAddress);
        autoComplete->setCaseSensitivity(Qt::CaseInsensitive);
        autoComplete->setFilterMode(Qt::MatchContains);
        autoComplete->setWrapAround(false);
        // autoComplete->setCompletionMode(QCompleter::CompletionMode::UnfilteredPopupCompletion);
    }
    if (mInputNodeAddress) {
        mInputNodeAddress->setCompleter(autoComplete);
    }
}


void GuiCenter::slotPropertyChanged(const int& type, const QVariant& value) {
    switch (type) {
        case ivis::common::PropertyTypeEnum::PropertyTypeDepth : {
            drawDisplay(value);
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize : {
            updateDisplaySize();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeVisible : {
            updateDisplayVisible();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeConfigInfo : {
            updateDisplayConfigInfo();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeSignalListAll : {
            updateDisplayNodeAddress();
            break;
        }
        default : {
            break;
        }
    }
}
