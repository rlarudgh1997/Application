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
    updateDisplaySize();
}

void GuiCenter::drawDisplayDepth0() {
    updateDisplayVisible();

    if (mConfigWidget == nullptr) {
        mConfigWidget = new QWidget();
        mConfigWidget->setGeometry(0, 0, mMainView->geometry().width(), mMainView->geometry().height());
        mConfigWidget->show();
        mMainView->insertWidget(0, mConfigWidget);
    }
    if (mNodeAddressWidget == nullptr) {
        mNodeAddressWidget = new QWidget();
        mNodeAddressWidget->setGeometry(0, 0, mMainView->geometry().width(), mMainView->geometry().height());
        mNodeAddressWidget->show();
        mMainView->insertWidget(1, mNodeAddressWidget);

        if (mNodeAddressList == nullptr) {
            mNodeAddressList = new QListView(mNodeAddressWidget);
            mNodeAddressList->setGeometry(0, 0, mMainView->geometry().width(), mMainView->geometry().height() - 100);
            mNodeAddressList->show();
            mNodeAddressWidget->stackUnder(mNodeAddressList);
        }
    }


    if (mConfigHideButton == nullptr) {
        mConfigHideButton = new QPushButton(mMainView);
        mConfigHideButton->setGeometry(1300 - 50, 0, 50, 50);
        mConfigHideButton->setStyleSheet("background-color: rgb(255, 255, 255); color: black; font: bold; font-size:20px");
        mConfigHideButton->setText("X");
        // mConfigHideButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        connect(mConfigHideButton, &QPushButton::clicked, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeViewInfoClose, QVariant(mMainView->currentIndex()));
        });
        mConfigHideButton->show();
        mConfigHideButton->raise();
        mMainView->stackUnder(mConfigHideButton);
    }
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
    QVariantList allConfig = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeConfigInfo).toList();
    qDebug() << "GuiCenter::updateDisplayConfigInfo() ->" << allConfig.size();
    if ((allConfig.size() == 0) || (mConfigWidget == nullptr)) {
        foreach(const auto& item, mConfigListItem) {
            item->clear();
        }
        return;
    }

    mMainView->setCurrentIndex(0);
    if (mNodeAddressList) {
        mNodeAddressList->hide();
    }
    mConfigWidget->show();

    if (mConfigListItem.size() > 0) {
        foreach(const auto& item, mConfigListItem) {
            item->initStyle();
        }
        qDebug() << "ConfigInfo List Item Count :" << mConfigListItem.size();
        return;
    }

    int index = 0;
    foreach(const auto& data, allConfig) {
        QVariantList config = data.toList();
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
        ListItem* item = new ListItem(index++, type, name, realValue, mConfigWidget);
        mConfigListItem[mConfigListItem.size()] = item;
        connect(item, &ListItem::signalValueChanged, [=](const int& type, const QVariant& value) {
            createSignal(ivis::common::EventTypeEnum::EventTypeChangedConfig, QVariant(QVariantList{type, value}));
        });
    }
}

void GuiCenter::updateDisplayNodeAddress() {
    QStringList nodeAddress = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeSignalListAll).toStringList();
    qDebug() << "GuiCenter::updateDisplayNodeAddress() ->" << nodeAddress.size();
    if ((nodeAddress.size() == 0) || (mNodeAddressList == nullptr)) {
        return;
    }

    mMainView->setCurrentIndex(1);
    if (mConfigWidget) {
        mConfigWidget->hide();
    }
    mNodeAddressList->show();

    mNodeAddressModel->setStringList(nodeAddress);
    mNodeAddressList->setModel(mNodeAddressModel);
    // mNodeAddressList->setItemDelegate(new QStyledItemDelegate(mNodeAddressList));

    static QLineEdit* inputNodeAddress = nullptr;
    if (inputNodeAddress == nullptr) {
        inputNodeAddress = new QLineEdit(mNodeAddressList);
        inputNodeAddress->setGeometry(10, 70, 1260, 30);
        inputNodeAddress->setStyleSheet("background-color: rgb(255, 255, 255); color: black; font: bold; font-size:20px");
        inputNodeAddress->setStyleSheet("color: rgb(50, 50, 100)");
        inputNodeAddress->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        inputNodeAddress->show();
    }
    static QCompleter* autoComplete = nullptr;
    if (autoComplete == nullptr) {
        autoComplete = new QCompleter(nodeAddress, inputNodeAddress);
        autoComplete->setCaseSensitivity(Qt::CaseInsensitive);
        autoComplete->setFilterMode(Qt::MatchContains);
        autoComplete->setWrapAround(false);
        // autoComplete->setCompletionMode(QCompleter::CompletionMode::UnfilteredPopupCompletion);
    }
    inputNodeAddress->setCompleter(autoComplete);
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
