#include "GuiCenter.h"
#include "AbstractHandler.h"

#include "CommonResource.h"
#include "ConfigInfo.h"

#include <QStringListModel>
#include <QCompleter>

#include "GuiExcel.h"
// #include "CommonFunction.h"
#if 0
#include "ConfigSetting.h"
#include <QtWidgets>
#include <QtWebEngineWidgets/QWebEngineView>
#endif

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

    // Draw - ViewType
    if (mConfigWidget == nullptr) {
        mConfigWidget = ivis::common::createWidget<QWidget>(mMainView);
        updateDisplayViewType<QWidget>(ivis::common::ViewTypeEnum::ViewTypeConfig, mConfigWidget);
    }

    if (mReportWidget == nullptr) {
        mReportWidget = ivis::common::createWidget<QWidget>(mMainView);
        updateDisplayViewType<QWidget>(ivis::common::ViewTypeEnum::ViewTypeReport, mReportWidget);
    }

    if (mNodeAddress == nullptr) {
        mNodeAddress = ivis::common::createWidget<QTableWidget>(mMainView);
        updateDisplayViewType<QTableWidget>(ivis::common::ViewTypeEnum::ViewTypeNodeAddress, mNodeAddress);
    }

    // ETC - Button
    if (mConfigHideButton == nullptr) {
        mConfigHideButton =
            ivis::common::createWidget<QPushButton>(mMainView, false, QRect(1250, 20, 50, 50),
                                                    QString("background-color: white; color: black; font: bold; font-size:20px"));
        mConfigHideButton->setText("X");
        connect(mConfigHideButton, &QPushButton::clicked, [=]() {
            createSignal(ivis::common::EventTypeEnum::EventTypeViewInfoClose, QVariant(mMainView->currentIndex()));
        });
    }

    if (mConfigResetButton == nullptr) {
        mConfigResetButton =
            ivis::common::createWidget<QPushButton>(mMainView, false, QRect(1250, 75, 50, 50),
                                                    QString("background-color: white; color: black; font: bold; font-size:12px"));
        mConfigResetButton->setText("Reset");
        connect(mConfigResetButton, &QPushButton::clicked,
                [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeConfigReset, QVariant()); });
    }

    if (mSelectModuleButton == nullptr) {
        mSelectModuleButton =
            ivis::common::createWidget<QPushButton>(mMainView, false, QRect(1050, 20, 80, 50),
                                                    QString("background-color: white; color: black; font: bold; font-size:12px"));
        mSelectModuleButton->setText("Select\nModule");
        connect(mSelectModuleButton, &QPushButton::clicked,
                [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeShowModule, QVariant()); });
    }

    if (mNodeAddressSearch == nullptr) {
        mNodeAddressSearch =
            ivis::common::createWidget<QPushButton>(mMainView, false, QRect(1150, 20, 80, 50),
                                                    QString("background-color: white; color: black; font: bold; font-size:12px"));
        mNodeAddressSearch->setText("Search");
        connect(mNodeAddressSearch, &QPushButton::clicked, [=]() { updateDisplayAutoComplete(true); });
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
    int currViewType =
        (viewType >= 0) ? (viewType) : (isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeViewType).toInt());
    if ((viewType >= 0) && (widget)) {
        mMainView->insertWidget(currViewType, widget);
    } else {
        mMainView->setCurrentIndex(currViewType);
    }
}

void GuiCenter::updateDisplayConfigInfo() {
    QVariantList configValue = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeConfigInfo).toList();
    // qDebug() << "GuiCenter::updateDisplayConfigInfo() ->" << mConfigValue.size() << "," << configValue.size();

    if (mConfigWidget == nullptr) {
        return;
    } else {
        updateDisplayViewType<QWidget>();
        ivis::common::widgetVisible(mConfigWidget, true);
        ivis::common::widgetVisible(mReportWidget, false);
        ivis::common::widgetVisible(mNodeAddress, false);
        ivis::common::widgetVisible(mConfigHideButton, true);
        ivis::common::widgetVisible(mConfigResetButton, true);
        ivis::common::widgetVisible(mSelectModuleButton, false);
        ivis::common::widgetVisible(mNodeAddressSearch, false);

        if (mConfigValue == configValue) {
            for (const auto& item : mConfigListItem) {
                item->initStyle();
            }
            return;
        }
    }

    bool newItem = (mConfigValue.size() != configValue.size());
    if (newItem) {
        for (const auto& item : mConfigListItem) {
            item->clear();
        }
        mConfigListItem.clear();
    }

    int index = 0;
    mConfigValue = configValue;
    for (const auto& info : configValue) {
        QVariantList config = info.toList();
        if (config.size() != 3) {
            continue;
        }
        int type = config.at(0).toInt();
        QString name = config.at(1).toString();
        QVariant value = config.at(2);
        QString realValue = QString();
        switch (value.type()) {
            case QVariant::Type::List: {
                for (const auto& v : value.toList()) {
                    realValue.append(QString("%1, ").arg(v.toString()));
                }
                realValue.resize(realValue.size() - 2);
                break;
            }
            case QVariant::Type::StringList: {
                for (const auto& v : value.toStringList()) {
                    realValue.append(QString("%1, ").arg(v));
                }
                realValue.resize(realValue.size() - 2);
                break;
            }
            case QVariant::Type::Rect: {
                QRect rect = value.toRect();
                realValue.append(QString("%1, %2, %3, %4").arg(rect.x()).arg(rect.y()).arg(rect.width()).arg(rect.height()));
                break;
            }
            default: {
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

#if 0
    QString path = ConfigSetting::instance().data()->readConfig(ConfigInfo::ConfigTypeDefaultPath).toString();
    QString htmlFilePath = path + "/HTML/gtest_report.html";
    qDebug() << "htmlFilePath :" << htmlFilePath;

    static QWebEngineView* webView = new QWebEngineView(mConfigWidget);
    webView->setGeometry(QRect(0, 100, 800, 800));
    webView->setUrl(QUrl::fromLocalFile(htmlFilePath));
    webView->show();
#endif
}

void GuiCenter::updateDisplayTestReport() {
    // qDebug() << "GuiCenter::updateDisplayTestReport()";

    if (mReportWidget == nullptr) {
        return;
    } else {
        updateDisplayViewType<QWidget>();
        ivis::common::widgetVisible(mConfigWidget, false);
        ivis::common::widgetVisible(mReportWidget, true);
        ivis::common::widgetVisible(mNodeAddress, false);
        ivis::common::widgetVisible(mConfigHideButton, true);
        ivis::common::widgetVisible(mConfigResetButton, true);
        ivis::common::widgetVisible(mSelectModuleButton, false);
        ivis::common::widgetVisible(mNodeAddressSearch, false);
    }

    for (int reportType = ivis::common::ReportTypeEnum::ReportTypeResult;
         reportType < ivis::common::ReportTypeEnum::ReportTypeAll; reportType++) {
        int propertyType = 0;
        QMap<int, QString> text = QMap<int, QString>();
        if (reportType == ivis::common::ReportTypeEnum::ReportTypeResult) {
            propertyType = ivis::common::PropertyTypeEnum::PropertyTypeTestReportResultInfo;
            text[static_cast<int>(ReportItemInfo::Text::Title)] = QString("Test Result :");
            text[static_cast<int>(ReportItemInfo::Text::On)] = QString("On");
            text[static_cast<int>(ReportItemInfo::Text::Off)] = QString("Off");
            text[static_cast<int>(ReportItemInfo::Text::Option)] = QString("Options :");
            text[static_cast<int>(ReportItemInfo::Text::Option1)] = QString("Split");
            text[static_cast<int>(ReportItemInfo::Text::Option2)] = QString("Config");
            text[static_cast<int>(ReportItemInfo::Text::Option3)] = QString("Excel");
        } else {
            propertyType = ivis::common::PropertyTypeEnum::PropertyTypeTestReportCoverageInfo;
            text[static_cast<int>(ReportItemInfo::Text::Title)] = QString("Test Coverage :");
            text[static_cast<int>(ReportItemInfo::Text::On)] = QString("On");
            text[static_cast<int>(ReportItemInfo::Text::Off)] = QString("Off");
            text[static_cast<int>(ReportItemInfo::Text::Option)] = QString("Options :");
            text[static_cast<int>(ReportItemInfo::Text::Option1)] = QString("Line");
            text[static_cast<int>(ReportItemInfo::Text::Option2)] = QString("Function");
            text[static_cast<int>(ReportItemInfo::Text::Option3)] = QString("Branch");
        }
        text[static_cast<int>(ReportItemInfo::Text::Apply)] = QString("Apply");
        text[static_cast<int>(ReportItemInfo::Text::Cancel)] = QString("Cancel");

        QVariantList reportData = isHandler()->getProperty(propertyType).toList();
        QMap<int, QPair<int, bool>> config = QMap<int, QPair<int, bool>>();
        int configInfo = static_cast<int>(ReportItemInfo::Config::On);
        bool status = false;
        for (const auto& data : reportData) {
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

void GuiCenter::updateDisplayNodeAddress(const int& updateType) {
    QStringList nodeAddress = isHandler()->getProperty(updateType).toStringList();
    // qDebug() << "GuiCenter::updateDisplayNodeAddress() ->" << updateType << nodeAddress.size();

    if (mNodeAddress == nullptr) {
        return;
    } else {
        updateDisplayViewType<QTableWidget>();
        ivis::common::widgetVisible(mConfigWidget, false);
        ivis::common::widgetVisible(mReportWidget, false);
        ivis::common::widgetVisible(mNodeAddress, true);
        ivis::common::widgetVisible(mConfigHideButton, true);
        ivis::common::widgetVisible(mConfigResetButton, false);
        ivis::common::widgetVisible(mSelectModuleButton, true);
        ivis::common::widgetVisible(mNodeAddressSearch, true);
    }

    QStringList title = QStringList({"Node Name", "Vehicle Type"});
    mNodeAddress->setRowCount(nodeAddress.size());
    mNodeAddress->setColumnCount(title.size());
    mNodeAddress->setGeometry(mNodeAddress->geometry());
    mNodeAddress->setHorizontalHeaderLabels(title);

    for (int rowIndex = 0; rowIndex < nodeAddress.size(); rowIndex++) {
        QStringList text = nodeAddress[rowIndex].split("\t");
        if (text.size() == 2) {
            for (int columnIndex = 0; columnIndex < text.size(); columnIndex++) {
                mNodeAddress->setItem(rowIndex, columnIndex, new QTableWidgetItem(text[columnIndex]));
            }
        } else {
            mNodeAddress->setItem(rowIndex, 0, new QTableWidgetItem(text[0]));
        }
    }
    mNodeAddress->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mNodeAddress->resizeColumnsToContents();
    mNodeAddress->resizeRowsToContents();
    mNodeAddress->show();
}

void GuiCenter::updateDisplayAutoComplete(const bool& show) {
    QStringList nodeAddressName = QStringList();
    QStringList nodeAddress = QStringList();

    for (int rowIndex = 0; rowIndex < mNodeAddress->rowCount(); rowIndex++) {
        nodeAddress.append(mNodeAddress->item(rowIndex, 0)->text());
    }

    if (nodeAddress.size() == 0) {
        nodeAddress = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressAll).toStringList();
    }

    for (int rowIndex = 0; rowIndex < nodeAddress.size(); rowIndex++) {
        QStringList text = nodeAddress[rowIndex].split("\t");
        nodeAddressName.append(text[0]);
    }

    if (mAutoComplete == nullptr) {
        mAutoComplete = new AutoCompleteDialog(isHandler()->getScreen(), QString("Search"), nodeAddressName);
        connect(mAutoComplete, &AutoCompleteDialog::signalAutoCompleteSelectedText, [=](const QString& selectNodeName) {
            int rowIndex = 0;
            for (const auto& nodeName : nodeAddressName) {
                if (selectNodeName == nodeName) {
                    break;
                }
                rowIndex++;
            }
            mNodeAddress->clearSelection();
            QTableWidgetItem* scrollItem = mNodeAddress->item(rowIndex, 0);
            if (scrollItem) {
                mNodeAddress->scrollToItem(scrollItem, QAbstractItemView::PositionAtTop);
                scrollItem->setSelected(true);
            }

            mAutoComplete->hide();
            // mAutoComplete->finished(true);
        });
        connect(mAutoComplete, &QDialog::finished, [=]() {
            disconnect(mAutoComplete);
            delete mAutoComplete;
            mAutoComplete = nullptr;
        });
    }

    if (show) {
        mAutoComplete->show();
    } else {
        mAutoComplete->hide();
    }
}

void GuiCenter::updateDisplaySelectModule(const bool& show) {
    if (show) {
        if (mSelectModule == nullptr) {
            QVariant moduleList = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeAllModuleList);
            QVariant selectModule = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSelectModule);
            bool allState = (moduleList.toStringList().size() == selectModule.toStringList().size());
            mSelectModule = new SelectModuleDialog(isHandler()->getScreen(), moduleList.toStringList(), allState, true);
            mSelectModule->updateSelectModule(selectModule.toStringList());

            qDebug() << "ModuleList :" << moduleList.toStringList().size() << selectModule.toStringList().size();
            connect(mSelectModule, &SelectModuleDialog::signalModuleSelected,
                    [=](const QList<QPair<int, QString>>& selectModule) {
                        mSelectModule->hide();
                        QVariantList moduleSelect = QVariantList();
                        for (const auto& select : selectModule) {
                            moduleSelect.append(QVariant(select.second));
                        }
                        createSignal(ivis::common::EventTypeEnum::EventTypeSelectModule, QVariant(moduleSelect));
                        // mSelectModule->finished(true);
                    });
            connect(mSelectModule, &QDialog::finished, [=]() {
                disconnect(mSelectModule);
                delete mSelectModule;
                mSelectModule = nullptr;
            });
        }
        mSelectModule->show();
    }
}

void GuiCenter::slotPropertyChanged(const int& type, const QVariant& value) {
    switch (type) {
        case ivis::common::PropertyTypeEnum::PropertyTypeDepth: {
            drawDisplay(value);
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeDisplaySize: {
            updateDisplaySize();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeVisible: {
            updateDisplayVisible();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeConfigInfo: {
            updateDisplayConfigInfo();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeTestReportType: {
            updateDisplayTestReport();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressAll: {
            updateDisplayNodeAddress(type);
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeShowSelectModule: {
            updateDisplaySelectModule(value.toBool());
            break;
        }
        default: {
            break;
        }
    }
}
