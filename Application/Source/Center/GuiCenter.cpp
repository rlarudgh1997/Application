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
        mGui->TerminalViewInputText->setFocus();
        createSignal(ivis::common::EventTypeEnum::EventTypeTerminalCommand, QString());
    });
    connect(mGui->TerminalViewInputText, &QLineEdit::returnPressed, [=]() {
        QString command = mGui->TerminalViewInputText->text();
        if (command.size() == 0) {
            return;
        }

        if (command.compare("clear") == 0) {
            emit mGui->TerminalViewClear->clicked();
        } else {
            mGui->TerminalViewInputText->clear();
            createSignal(ivis::common::EventTypeEnum::EventTypeTerminalCommand, command);
        }

        // Save : Command History
        QStringList commandHistory = getCommandHistory();
        commandHistory.prepend(command);
        setCommandHistory(commandHistory);
    });
    connect(mGui->TerminalViewPathTitle, &QPushButton::clicked, [=]() {
        createSignal(ivis::common::EventTypeEnum::EventTypeTerminalSetPath, QString());
        mGui->TerminalViewInputText->setFocus();
    });
}

void GuiCenter::drawDisplayDepth1() {
    qDebug() << "GuiCenter::drawDisplayDepth1()";
}

void GuiCenter::drawDisplayDepth2() {
    qDebug() << "GuiCenter::drawDisplayDepth2()";
}

bool GuiCenter::eventFilter(QObject* object, QEvent* event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        int key = keyEvent->key();

        switch (key) {
            case ivis::common::KeyTypeEnum::KeyInputValueUp:
            case ivis::common::KeyTypeEnum::KeyInputValueDown: {
                QStringList commandHistory = getCommandHistory();
                int historyCount = commandHistory.size();
                int index = getHistoryIndex();

                if (key == ivis::common::KeyTypeEnum::KeyInputValueUp) {
                    ivis::common::REVOLVE_P(index, 1, 0, historyCount);
                } else {
                    ivis::common::REVOLVE_M(index, 1, 0, historyCount);
                }
                setHistoryIndex(index);
                mGui->TerminalViewInputText->setText(commandHistory.at(index));
                // qDebug() << "History :" << index << "/" << historyCount << "->" << commandHistory.at(index);
                break;
            }
            // case ivis::common::KeyTypeEnum::KeyInputValueTab: {
            //     // mGui->TerminalViewInputText->setFocus();
            //     break;
            // }
            default: {
                break;
            }
        }
    }
    return QObject::eventFilter(object, event);
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
    // mGui->ConfigView->setFixedSize(mMainView->size());
    // mGui->NodeView->setFixedSize(mMainView->size());
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
#if 0
            QHeaderView::ResizeMode resizeMode = (columnIndex == 0) ? QHeaderView::Fixed : QHeaderView::ResizeToContents;
            mGui->ConfigView->horizontalHeader()->setSectionResizeMode(columnIndex, resizeMode);
#else
            mGui->ConfigView->horizontalHeader()->setSectionResizeMode(columnIndex, QHeaderView::Fixed);
#endif
            Qt::ItemFlags flags = mGui->ConfigView->item(rowIndex, columnIndex)->flags();
            flags = (columnIndex == 0) ? (flags & ~Qt::ItemIsEditable) : (flags | Qt::ItemIsEditable);
            mGui->ConfigView->item(rowIndex, columnIndex)->setFlags(flags);
        }
        mGui->ConfigView->verticalHeader()->resizeSection(rowIndex, 30);
        rowIndex++;
    }

    mGui->ConfigView->resizeColumnsToContents();
    if (title.size() == 2) {
        int mainWidth = mMainView->size().width();
        int columnWidth = (mGui->ConfigView->columnWidth(0) + 100);  // 0 : Config Name
        mGui->ConfigView->horizontalHeader()->resizeSection(1, (mainWidth - columnWidth));   // 1 : Config Value
    }

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

#if 0
    mGui->NodeView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    mGui->NodeView->horizontalHeader()->resizeSection(0, 800);
#else
    if (title.size() == 2) {
        int mainWidth = mMainView->size().width();
        int columnWidth = (mGui->NodeView->columnWidth(1) + 300);
        mGui->NodeView->horizontalHeader()->resizeSection(0, (mainWidth - columnWidth));
    }
#endif
}

void GuiCenter::updateDisplayTerminal(const bool& first, const bool& updatPath) {
    if (first) {
        mMainView->setCurrentIndex(ivis::common::ViewTypeEnum::CenterViewTypeTerminal);
        mGui->TerminalViewWidget->setFixedSize(mMainView->size());
        mGui->TerminalViewPathInfo->clear();
        mGui->TerminalViewDisplay->clear();
        mGui->TerminalViewInputText->clear();
        mGui->TerminalViewInputText->setFocus();

        mGui->TerminalViewPathInfo->setFocusPolicy(Qt::NoFocus);
        mGui->TerminalViewDisplay->setFocusPolicy(Qt::NoFocus);
        mGui->TerminalViewTitle->setFocusPolicy(Qt::NoFocus);

        mGui->TerminalViewPathTitle->setFocusPolicy(Qt::NoFocus);
        mGui->TerminalViewClose->setFocusPolicy(Qt::NoFocus);
        mGui->TerminalViewClear->setFocusPolicy(Qt::NoFocus);

        mGui->TerminalViewInputText->installEventFilter(this);
        setHistoryIndex(-1);
    }

    if (updatPath) {
        QString pathInfo = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeTerminalPathInfo).toString();
        mGui->TerminalViewPathInfo->setText(QString(" %1").arg(pathInfo));
        // qDebug() << "\t Path :" << pathInfo;
    } else {
        const int bufferSize = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeTerminalBufferSize).toInt();
        QString info = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeTerminalInfo).toString();
        mGui->TerminalViewDisplay->moveCursor(QTextCursor::End);
        mGui->TerminalViewDisplay->insertPlainText(info);
        mGui->TerminalViewDisplay->ensureCursorVisible();

        QStringList currentText = mGui->TerminalViewDisplay->toPlainText().split('\n');
        if (currentText.size() > bufferSize) {
            int displaySize = currentText.size() - bufferSize;
            currentText = currentText.mid(displaySize);
            mGui->TerminalViewDisplay->setPlainText(currentText.join('\n'));
            mGui->TerminalViewDisplay->moveCursor(QTextCursor::End);
        }
    }
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
            updateDisplayTerminal(true, false);
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
        case ivis::common::PropertyTypeEnum::PropertyTypeTerminalInfo: {
            updateDisplayTerminal(false, false);
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeTerminalPathInfo: {
            updateDisplayTerminal(false, true);
            break;
        }
        default: {
            break;
        }
    }
}
