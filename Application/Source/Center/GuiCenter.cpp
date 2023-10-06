#include "GuiCenter.h"
#include "AbstractHandler.h"

#include "CommonResource.h"
#include "ConfigInfo.h"

#include <QStringListModel>
#include <QCompleter>



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
    // QRect parentRect = mMainView->geometry();

    // Draw - ViewType
    if (mConfigWidget == nullptr) {
        mConfigWidget = ivis::common::createWidget<QWidget>(mMainView);
        updateDisplayViewType<QWidget>(ivis::common::ViewTypeEnum::ViewTypeConfig, mConfigWidget);
    }

    if (mNodeAddressList == nullptr) {
        mNodeAddressList = ivis::common::createWidget<QListView>(mMainView);
        updateDisplayViewType<QListView>(ivis::common::ViewTypeEnum::ViewTypeSignal, mNodeAddressList);

#if !defined(USE_DEMO)
        if (mInputNodeAddress == nullptr) {
            mInputNodeAddress = ivis::common::createWidget<QLineEdit>(mNodeAddressList, true, QRect(400, 20, 840, 50),
                                                QString("background-color: white; color: black; font: bold; font-size:15px"));
            mInputNodeAddress->show();
        }
#endif
    }

    if (mReportWidget == nullptr) {
        mReportWidget = ivis::common::createWidget<QWidget>(mMainView);
        updateDisplayViewType<QWidget>(ivis::common::ViewTypeEnum::ViewTypeReport, mReportWidget);
    }


    // ETC - Button
    if (mConfigHideButton == nullptr) {
        mConfigHideButton = ivis::common::createWidget<QPushButton>(mMainView, true, QRect(1250, 20, 50, 50),
                                            QString("background-color: white; color: black; font: bold; font-size:20px"));
        mConfigHideButton->setText("X");
        connect(mConfigHideButton, &QPushButton::clicked, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeViewInfoClose, QVariant(mMainView->currentIndex()));
        });
    }

    if (mConfigResetButton == nullptr) {
        mConfigResetButton = ivis::common::createWidget<QPushButton>(mMainView, true, QRect(1250, 75, 50, 50),
                                            QString("background-color: white; color: black; font: bold; font-size:12px"));
        mConfigResetButton->setText("Reset");
        connect(mConfigResetButton, &QPushButton::clicked, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeConfigReset, QVariant());
        });
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

template <typename T>
void GuiCenter::updateDisplayViewType(const int& viewType, T* widget) {
    int currViewType = (viewType >= 0) ? (viewType) :
                        (isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeViewType).toInt());
    if ((viewType >= 0) && (widget)) {
        mMainView->insertWidget(currViewType, widget);
    } else {
        mMainView->setCurrentIndex(currViewType);
    }
}

void GuiCenter::updateDisplayConfigInfo() {
    QVariantList configValue = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeConfigInfo).toList();
    qDebug() << "GuiCenter::updateDisplayConfigInfo() ->" << mConfigValue.size() << "," << configValue.size();

    if (mConfigWidget == nullptr) {
        return;
    } else {
        updateDisplayViewType<QWidget>();
        ivis::common::widgetVisible(mConfigWidget,      true);
        ivis::common::widgetVisible(mNodeAddressList,   false);
        ivis::common::widgetVisible(mReportWidget,      false);
        ivis::common::widgetVisible(mInputNodeAddress,  false);
        ivis::common::widgetVisible(mConfigHideButton,  true);
        ivis::common::widgetVisible(mConfigResetButton, true);

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
                createSignal(ivis::common::EventTypeEnum::EventTypeUpdateConfig, QVariant(QVariantList{type, value}));
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
    QStringList nodeAddress = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressAll).toStringList();
    qDebug() << "GuiCenter::updateDisplayNodeAddress() ->" << nodeAddress.size();

    if (mNodeAddressList == nullptr) {
        return;
    } else {
        updateDisplayViewType<QWidget>();
        ivis::common::widgetVisible(mConfigWidget,      false);
        ivis::common::widgetVisible(mNodeAddressList,   true);
        ivis::common::widgetVisible(mReportWidget,      false);
        ivis::common::widgetVisible(mInputNodeAddress,  true);
        ivis::common::widgetVisible(mConfigHideButton,  true);
        ivis::common::widgetVisible(mConfigResetButton, false);
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

void GuiCenter::updateDisplayTestReport() {
    if (mReportWidget == nullptr) {
        return;
    } else {
        updateDisplayViewType<QWidget>();
        ivis::common::widgetVisible(mConfigWidget,      false);
        ivis::common::widgetVisible(mNodeAddressList,   false);
        ivis::common::widgetVisible(mReportWidget,      true);
        ivis::common::widgetVisible(mInputNodeAddress,  false);
        ivis::common::widgetVisible(mConfigHideButton,  true);
        ivis::common::widgetVisible(mConfigResetButton, true);
    }

    for (int reportType = ivis::common::ReportTypeEnum::ReportTypeResult;
                                reportType < ivis::common::ReportTypeEnum::ReportTypeAll; reportType++) {
        int propertyType = 0;
        QMap<int, QString> text = QMap<int, QString>();
        if (reportType == ivis::common::ReportTypeEnum::ReportTypeResult) {
            propertyType = ivis::common::PropertyTypeEnum::PropertyTypeTestReportResultInfo;
            text[static_cast<int>(ReportItemInfo::Text::Title)]   = QString("Test Result :");
            text[static_cast<int>(ReportItemInfo::Text::On)]      = QString("On");
            text[static_cast<int>(ReportItemInfo::Text::Off)]     = QString("Off");
            text[static_cast<int>(ReportItemInfo::Text::Option)]  = QString("Options :");
            text[static_cast<int>(ReportItemInfo::Text::Option1)] = QString("Split");
            text[static_cast<int>(ReportItemInfo::Text::Option2)] = QString("Config");
            text[static_cast<int>(ReportItemInfo::Text::Option3)] = QString("Excel");
        } else {
            propertyType = ivis::common::PropertyTypeEnum::PropertyTypeTestReportCoverageInfo;
            text[static_cast<int>(ReportItemInfo::Text::Title)]   = QString("Test Coverage :");
            text[static_cast<int>(ReportItemInfo::Text::On)]      = QString("On");
            text[static_cast<int>(ReportItemInfo::Text::Off)]     = QString("Off");
            text[static_cast<int>(ReportItemInfo::Text::Option)]  = QString("Options :");
            text[static_cast<int>(ReportItemInfo::Text::Option1)] = QString("Line");
            text[static_cast<int>(ReportItemInfo::Text::Option2)] = QString("Function");
            text[static_cast<int>(ReportItemInfo::Text::Option3)] = QString("Branch");
        }
        text[static_cast<int>(ReportItemInfo::Text::Apply)]       = QString("Apply");
        text[static_cast<int>(ReportItemInfo::Text::Cancel)]      = QString("Cancel");

        QVariantList reportData = isHandler()->getProperty(propertyType).toList();
        QMap<int, QPair<int, bool>> config = QMap<int, QPair<int, bool>>();
        int configInfo = static_cast<int>(ReportItemInfo::Config::On);
        bool status = false;
        foreach(const auto& data, reportData) {
            QVariantList info = data.toList();
            if (info.size() != 2) {
                continue;
            }
            // qDebug() << "UpdateConfig :" << configInfo << info.at(0).toInt() << info.at(1).toBool();
            if (configInfo == static_cast<int>(ReportItemInfo::Config::On)) {
                status = info.at(1).toBool();
            }
            config[configInfo++] = QPair<int, bool>(info.at(0).toInt(), info.at(1).toBool());
        }

        if (mTestReport[reportType] == nullptr) {
            mTestReport[reportType] = new ReportItem(mReportWidget, reportType, true);
            connect(mTestReport[reportType], &ReportItem::signalReportValueChanged,
                                                    [=](const int& info, const int& type, const bool& value) {
                // qDebug() << reportType << ". signalReportValueChanged :" << info << type << value;
                createSignal(ivis::common::EventTypeEnum::EventTypeUpdateConfig, QVariant(QVariantList{type, value}));
                if (info == static_cast<int>(ReportItemInfo::Config::On)) {
                    mTestReport[reportType]->updateStatus(value);
                }
            });
        }
        mTestReport[reportType]->updateConfig(config);
        mTestReport[reportType]->updateText(text);
        mTestReport[reportType]->updateStatus(status);
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
        case ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressAll : {
            updateDisplayNodeAddress();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeTestReportType : {
            updateDisplayTestReport();
            break;
        }
        default : {
            break;
        }
    }
}
