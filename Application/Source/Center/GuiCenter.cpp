#include "GuiCenter.h"
#include "AbstractHandler.h"

#include "CommonResource.h"
#include <QStringListModel>
#include <QCompleter>

#include "GuiExcel.h"

QSharedPointer<GuiCenter>& GuiCenter::instance(AbstractHandler* handler) {
    static QSharedPointer<GuiCenter> gGui;
    if (gGui.isNull()) {
        gGui = QSharedPointer<GuiCenter>(new GuiCenter(handler));
    }
    return gGui;
}

GuiCenter::GuiCenter(AbstractHandler* handler) : AbstractGui(handler), mGui(new Ui::GuiCenter) {
    mMainView = new QStackedWidget(isHandler()->getScreen());
    mGui->setupUi(mMainView);
    updateDisplaySize();
}

void GuiCenter::drawDisplayDepth0() {
    updateDisplayVisible();

    // Config View
    connect(mGui->ConfigView, &QTableWidget::itemChanged, [=](QTableWidgetItem* item) {
        if ((getConfigUpdating() == false) && (item != nullptr)) {
            createSignal(ivis::common::EventTypeEnum::EventTypeUpdateConfig, QVariantList{item->row() + 1, item->text()});
        }
    });
    connect(mGui->ConfigViewClose, &QPushButton::clicked, [=]() {
        createSignal(ivis::common::EventTypeEnum::EventTypeViewInfoClose, ivis::common::ViewTypeEnum::CenterViewTypeConfig);
    });
    connect(mGui->ConfigViewReset, &QPushButton::clicked,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeConfigReset, QVariant()); });

    // Node View
    connect(mGui->NodeViewClose, &QPushButton::clicked, [=]() {
        createSignal(ivis::common::EventTypeEnum::EventTypeViewInfoClose, ivis::common::ViewTypeEnum::CenterViewTypeNode);
    });
    connect(mGui->NodeViewSearch, &QPushButton::clicked, [=]() { updateDialogAutoComplete(); });
    connect(mGui->NodeViewSelectModule, &QPushButton::clicked,
            [=]() { createSignal(ivis::common::EventTypeEnum::EventTypeShowModule, QVariant()); });

    // Terminal View
    connect(mGui->TerminalViewClose, &QPushButton::clicked, [=]() {
        createSignal(ivis::common::EventTypeEnum::EventTypeViewInfoClose, ivis::common::ViewTypeEnum::CenterViewTypeTerminal);
    });
    connect(mGui->TerminalViewClear, &QPushButton::clicked, [=]() {
        mGui->TerminalViewInputText->clear();
        mGui->TerminalViewDisplay->clear();
        createSignal(ivis::common::EventTypeEnum::EventTypeTerminalCommand, QString());
    });
    connect(mGui->TerminalViewInputText, &QLineEdit::returnPressed, [=]() {
        QString inputCommand = mGui->TerminalViewInputText->text();
        mGui->TerminalViewInputText->clear();
        createSignal(ivis::common::EventTypeEnum::EventTypeTerminalCommand, inputCommand);
    });
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
    mGui->TerminalViewWidget->setFixedSize(mMainView->size());
}

void GuiCenter::updateDisplayVisible() {
    mMainView->setVisible(isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeVisible).toBool());
}

void GuiCenter::updateDrawDialog(const int& dialogType, const QVariantList& info) {
    if (mDialog.isNull()) {
        QRect rect = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeScreenInfo).toRect();
        mDialog = QSharedPointer<Dialog>(new Dialog(rect, isHandler()->getScreen()));

        connect(mDialog.data(), &QDialog::finished, [=]() {
            disconnect(mDialog.data(), nullptr, nullptr, nullptr);
            mDialog.reset();
        });
        connect(mDialog.data(), &Dialog::signalScrollBarValueChanged, [=](const int& value) {
            // qDebug() << "signalScrollBarValueChanged :" << value;
            setScrolBarValue(value);
        });
        connect(mDialog.data(), &Dialog::signalSelectListItem, [=](const QList<QPair<int, QString>>& selectItem) {
            QVariantList selectModule = QVariantList();
            for (const auto& select : selectItem) {
                selectModule.append(QVariant(select.second));
            }
            if (selectModule.size() == 0) {
                createSignal(ivis::common::EventTypeEnum::EventTypeSelectModuleError, QVariant());
            } else {
                // mDialog.data()->setVisible(false);
                createSignal(ivis::common::EventTypeEnum::EventTypeSelectModule, selectModule);
                mDialog.data()->accept();
            }
        });
        connect(mDialog.data(), &Dialog::signalAutoCompleteSelected, [=](const QString& text) {
            int rowIndex = 0;
            mGui->NodeView->clearSelection();
            for (const auto& nodeName : getNodeAddress()) {
                if (nodeName == text) {
                    mGui->NodeView->scrollToItem(mGui->NodeView->item(rowIndex, 0), QAbstractItemView::PositionAtTop);
                    mGui->NodeView->item(rowIndex, 0)->setSelected(true);
                    break;
                }
                rowIndex++;
            }
            // qDebug() << "signalAutoCompleteSelected :" << rowIndex << text;
        });
    }

    if (dialogType != Dialog::DialogTypeInvalid) {
        mDialog.data()->drawDialog(dialogType, info);
    }
}

void GuiCenter::updateDialogAutoComplete() {
    QStringList nodeAddressName = QStringList();
    QStringList nodeAddress = QStringList();

    for (int rowIndex = 0; rowIndex < mGui->NodeView->rowCount(); rowIndex++) {
        nodeAddress.append(mGui->NodeView->item(rowIndex, 0)->text());
    }

    if (nodeAddress.size() == 0) {
        nodeAddress = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressAll).toStringList();
    }

    for (int rowIndex = 0; rowIndex < nodeAddress.size(); rowIndex++) {
        QStringList text = nodeAddress[rowIndex].split("\t");
        nodeAddressName.append(text[0]);
    }

    setNodeAddress(nodeAddressName);
    QVariantList info = QVariantList({
        QString("Search Node"),
        QString(),
        nodeAddressName,
    });
    updateDrawDialog(Dialog::DialogTypeNodeView, info);
}

void GuiCenter::updateDialogSelectModule() {
    QVariantList info = QVariantList({
        QString("Select Module"),
        QStringList({"Module"}),
        isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeAllModuleList).toStringList(),
        isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeUpdateSelectModule).toStringList(),
        QVariantList(),
        getScrolBarValue(),
    });
    updateDrawDialog(Dialog::DialogTypeSelectMoudleInfo, info);
}

void GuiCenter::updateDisplayConfigInfo() {
    QVariantList prevConfig = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeConfigInfoPrevious).toList();
    QVariantList currConfig = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeConfigInfo).toList();
    // qDebug() << "updateDisplayConfigInfo :" << prevConfig.size() << currConfig.size();

    mMainView->setCurrentIndex(ivis::common::ViewTypeEnum::CenterViewTypeConfig);
    setConfigUpdating(true);

    QStringList title = QStringList({"Config Name", "Config Value"});
    mGui->ConfigView->setRowCount(currConfig.size());
    mGui->ConfigView->setColumnCount(title.size());
    mGui->ConfigView->setGeometry(mGui->ConfigView->geometry());
    mGui->ConfigView->setHorizontalHeaderLabels(title);

    QList<QPair<QString, QString>> configInfo;
    for (const auto& info : currConfig) {
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
        configInfo.append(qMakePair(name, realValue));
    }

    int rowIndex = 0;
    for (const auto& config : configInfo) {
        for (int columnIndex = 0; columnIndex < title.size(); ++columnIndex) {
            QTableWidgetItem* item = new QTableWidgetItem((columnIndex == 0) ? (config.first) : (config.second));
            mGui->ConfigView->setItem(rowIndex, columnIndex, item);

            QHeaderView::ResizeMode resizeMode = (columnIndex == 0) ? QHeaderView::Fixed : QHeaderView::ResizeToContents;
            mGui->ConfigView->horizontalHeader()->setSectionResizeMode(columnIndex, resizeMode);

            Qt::ItemFlags flags = mGui->ConfigView->item(rowIndex, columnIndex)->flags();
            flags = (columnIndex == 0) ? (flags & ~Qt::ItemIsEditable) : (flags | Qt::ItemIsEditable);
            mGui->ConfigView->item(rowIndex, columnIndex)->setFlags(flags);
        }
        mGui->ConfigView->verticalHeader()->resizeSection(rowIndex, 30);
        rowIndex++;
    }
    // mGui->ConfigView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mGui->ConfigView->resizeColumnsToContents();
    // mGui->ConfigView->resizeRowsToContents();

    setConfigUpdating(false);
}

void GuiCenter::updateDisplayNodeAddress() {
    QStringList nodeAddress = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressAll).toStringList();

    mMainView->setCurrentIndex(ivis::common::ViewTypeEnum::CenterViewTypeNode);

    QStringList title = QStringList({"Node Name", "Vehicle Type"});
    mGui->NodeView->setRowCount(nodeAddress.size());
    mGui->NodeView->setColumnCount(title.size());
    mGui->NodeView->setGeometry(mGui->NodeView->geometry());
    mGui->NodeView->setHorizontalHeaderLabels(title);

    for (int rowIndex = 0; rowIndex < nodeAddress.size(); rowIndex++) {
        QStringList text = nodeAddress[rowIndex].split("\t");
        if (text.size() == 2) {
            for (int columnIndex = 0; columnIndex < text.size(); columnIndex++) {
                mGui->NodeView->setItem(rowIndex, columnIndex, new QTableWidgetItem(text[columnIndex]));
            }
        } else {
            mGui->NodeView->setItem(rowIndex, 0, new QTableWidgetItem(text[0]));
        }
    }
    mGui->NodeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mGui->NodeView->resizeColumnsToContents();
    mGui->NodeView->resizeRowsToContents();
    mGui->NodeView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    mGui->NodeView->horizontalHeader()->resizeSection(0, 800);
}

void GuiCenter::updateDisplayTerminal(const bool& first) {
    if (first) {
        mMainView->setCurrentIndex(ivis::common::ViewTypeEnum::CenterViewTypeTerminal);
        mGui->TerminalViewWidget->setFixedSize(mMainView->size());
        mGui->TerminalViewInputText->clear();
        mGui->TerminalViewInputText->setFocus();
    }

    QString pathInfo = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeTerminalPathInfo).toString();
    QString terminalInfo = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeTerminalInfo).toString();
    mGui->TerminalViewPath->setText(QString(" Path : %1").arg(pathInfo));
    mGui->TerminalViewDisplay->append(terminalInfo);
}

void GuiCenter::updateDisplayViweType() {
    int viewType = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeViewType).toInt();
    switch (viewType) {
        case ivis::common::ViewTypeEnum::CenterViewTypeConfig: {
            updateDisplayConfigInfo();
            break;
        }
        case ivis::common::ViewTypeEnum::CenterViewTypeNode: {
            updateDisplayNodeAddress();
            break;
        }
        case ivis::common::ViewTypeEnum::CenterViewTypeTerminal: {
            updateDisplayTerminal(true);
            break;
        }
        default: {
            break;
        }
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
        case ivis::common::PropertyTypeEnum::PropertyTypeShowSelectModule: {
            updateDialogSelectModule();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeViewType: {
            updateDisplayViweType();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeTerminalInfo:
        case ivis::common::PropertyTypeEnum::PropertyTypeTerminalPathInfo: {
            updateDisplayTerminal(false);
            break;
        }
        default: {
            break;
        }
    }
}
