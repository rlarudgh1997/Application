#include "GuiExcel.h"
#include "AbstractHandler.h"

#include "CommonResource.h"
#include "GuiCenter.h"

#include <QMenu>
#include <QLabel>
#include <QTextEdit>
#include <QHeaderView>
#include <QClipboard>
#include <QMimeData>

// #define USE_EXCEL_ARROW_KEY

const QString VEHICLE_TYPE_ICV = QString("ICV");
const QString VEHICLE_TYPE_EV = QString("EV");
const QString VEHICLE_TYPE_FCEV = QString("FCEV");
const QString VEHICLE_TYPE_PHEV = QString("PHEV");
const QString VEHICLE_TYPE_HEV = QString("HEV");

QSharedPointer<GuiExcel>& GuiExcel::instance(AbstractHandler* handler) {
    static QSharedPointer<GuiExcel> gGui;
    if (gGui.isNull()) {
        gGui = QSharedPointer<GuiExcel>(new GuiExcel(handler));
    }
    return gGui;
}

GuiExcel::GuiExcel(AbstractHandler* handler) : AbstractGui(handler), mGui(new Ui::GuiExcel) {
    mMainView = new QWidget(isHandler()->getScreen());
    mGui->setupUi(mMainView);
    mMenuRight = new QMenu(isHandler()->getScreen());
    updateDisplaySize();
}

void GuiExcel::drawDisplayDepth0() {
    updateDisplayVisible();

    mGui->TabWidgetTC->setVisible(false);
    // mGui->TabWidgetTC->raise();

    connect(mGui->CellInfoContent, &QLineEdit::returnPressed, [=]() {
        if (mExcelSheet[mCurrentSheetIndex]->currentItem()) {
            QString text = mGui->CellInfoContent->text();
            mExcelSheet[mCurrentSheetIndex]->currentItem()->setText(text);
        }

        mGui->CellInfoContent->clear();
        mGui->CellInfoContent->clearFocus();
        if (mExcelSheet[mCurrentSheetIndex]) {
            mExcelSheet[mCurrentSheetIndex]->setFocus();
        }
    });
    connect(mGui->TabWidget, &QTabWidget::tabBarClicked, [=](int index) {
        // connect(mGui->TabWidget, &QTabWidget::currentChanged, [=](int index) {
        int currentIndex = ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription + index;
        if (mCurrentSheetIndex != currentIndex) {
            mCurrentSheetIndex = currentIndex;
            updateCellInfoContent(currentIndex, mModelIndex[currentIndex].first, mModelIndex[currentIndex].second);
        }
    });
}

void GuiExcel::drawDisplayDepth1() {
    qDebug() << "GuiExcel::drawDisplayDepth1()";
}

void GuiExcel::drawDisplayDepth2() {
    qDebug() << "GuiExcel::drawDisplayDepth2()";
}

void GuiExcel::updateDisplaySize() {
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

    rect.setY(20);
    rect.setHeight(size.height() - margin.height() - rect.y());
    mGui->TabWidget->setGeometry(rect);
    mGui->TabWidgetTC->setGeometry(rect);
}

void GuiExcel::updateDisplayVisible() {
    mMainView->setVisible(isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeVisible).toBool());
}

bool GuiExcel::chcekExcleSheet(const int& sheetIndex) {
    if (mExcelSheet.size() == 0) {
        qDebug() << "Fail to - excel sheet was not created";
        return true;
    }

    if (sheetIndex == 0) {
        return false;
    }

    if (mExcelSheet[sheetIndex] == nullptr) {
        qDebug() << "Fail to - excel sheet nullptr :" << sheetIndex;
        return true;
    }

    return false;
}

QVariantList GuiExcel::readExcelSheet(const int& sheetIndex, const QVariantList& readIndexInfo, QString& allString) {
    if (chcekExcleSheet(sheetIndex)) {
        qDebug() << "Fail to excel sheet nullptr : " << sheetIndex << ", size :" << mExcelSheet.size();
        return QVariantList();
    }

    QVariantList sheetData = QVariantList();
    int rowStart = 0;
    int columnStart = 0;
    int rowMax = 0;
    int columnMax = 0;

    if (readIndexInfo.size() == 0) {
        rowMax = mExcelSheet[sheetIndex]->rowCount();
        columnMax = mExcelSheet[sheetIndex]->columnCount();
        // Read Title
        QStringList title = QStringList();
        for (int columnIndex = 0; columnIndex < columnMax; columnIndex++) {
            title.append(mExcelSheet[sheetIndex]->horizontalHeaderItem(columnIndex)->text());
        }
        sheetData.append(title);
    } else {
        if (readIndexInfo.size() != 4) {
            qDebug() << "Fail to read index info size :" << readIndexInfo.size();
            return QVariantList();
        }

        rowStart = readIndexInfo.at(0).toInt();
        columnStart = readIndexInfo.at(1).toInt();
        rowMax = readIndexInfo.at(2).toInt();
        columnMax = readIndexInfo.at(3).toInt();
        // qDebug() << "Read Cell Info :" << rowStart << columnStart << rowMax << columnMax;
    }

    // Read Row Data
    QVariant excelMergeTextStart = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeTextStart);
    QVariant excelMergeTextEnd = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeTextEnd);
    QVariant excelMergeText = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeText);
    QMap<int, QList<QPair<int, int>>> mergeInfo = mMergeInfo[sheetIndex].isMergeInfo();
    for (int rowIndex = rowStart; rowIndex < rowMax; rowIndex++) {
        QStringList rowData = QStringList();
        for (int columnIndex = columnStart; columnIndex < columnMax; columnIndex++) {
            QString readText = ((mExcelSheet[sheetIndex]->item(rowIndex, columnIndex))
                                    ? (mExcelSheet[sheetIndex]->item(rowIndex, columnIndex)->text())
                                    : (QString()));
            QString text = readText;

            if (rowData.size() > 0) {
                allString.append("\t");
            }
            allString.append(text);

            for (const auto& info : mergeInfo[columnIndex]) {
                int rowStart = info.first;
                int rowEnd = (info.first + info.second);
                if ((rowIndex >= rowStart) && (rowIndex < rowEnd)) {
                    if (rowIndex == rowStart) {
                        text = excelMergeTextStart.toString();
                    } else if (rowIndex == (rowEnd - 1)) {
                        text = excelMergeTextEnd.toString();
                    } else {
                        text = excelMergeText.toString();
                    }
                    text = text + readText;
                    // qDebug() << "MergeText[" << sheetIndex << "][" << rowIndex << "," << columnIndex << "] :" << text;
                }
            }
            rowData.append(text);

            // if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription) {
            //     qDebug() << "ReadText[" << sheetIndex << "][" << columnIndex << "," << rowIndex << "] :" << text;
            // }
        }
        sheetData.append(rowData);
        allString.append("\n");
    }

    // qDebug() << "readExcelSheet() ->"<< "Length :" << rowMax << columnMax << sheetData.size();
    // qDebug() << mExcelSheet[sheetIndex] << ":" << sheetData;
    // qDebug() << "==================================================================================================\n";

    return sheetData;
}

void GuiExcel::readAllExcelSheet() {
    if (chcekExcleSheet(0)) {
        return;
    }

    int sheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription;
    for (const auto& sheet : mExcelSheet) {
        QString allString = QString();
        QVariantList sheetData = readExcelSheet(sheetIndex, QVariantList(), allString);
        if (sheetData.size() > 0) {
            int eventType = (sheetIndex - ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription) +
                            ivis::common::EventTypeEnum::EventTypeListDescription;
            createSignal(eventType, sheetData);
            sheetIndex++;
        }
    }
    QVariant saveFielPath = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeReadExcelSheetBeforeSave);
    createSignal(ivis::common::EventTypeEnum::EventTypeSaveFromReadExcelSheet, saveFielPath);
}

int GuiExcel::isMergeCell(const int& sheetIndex, const int& columnIndex, const int& rowStart) {
    if (chcekExcleSheet(sheetIndex)) {
        return 0;
    }
    return mMergeInfo[sheetIndex].isMergeCellCount(columnIndex, rowStart);
}

bool GuiExcel::updateMergeInfo(const bool& erase, const int& sheetIndex, const int& columnIndex, const int& rowStart,
                               const int& rowEnd) {
    if (chcekExcleSheet(sheetIndex)) {
        return false;
    }

    // qDebug() << "updateMergeInfo :" << erase << "," << sheetIndex << columnIndex << rowStart << rowEnd;
    if ((erase) || (mMergeInfo[sheetIndex].isContains(columnIndex, rowStart, rowEnd))) {
        mMergeInfo[sheetIndex].erase(columnIndex, rowStart, rowEnd);
        return (erase == false);
    }

    mMergeInfo[sheetIndex].insert(columnIndex, rowStart, rowEnd);
    return false;
}

QMap<int, QList<QPair<int, int>>> GuiExcel::findMergeInfo(const QMap<int, QVariantList>& sheetData) {
    QVariant excelMergeTextStart = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeTextStart);
    QVariant excelMergeTextEnd = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeTextEnd);
    // QVariant excelMergeText = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeText);

    QMap<int, QList<QPair<int, int>>> mergeInfo = QMap<int, QList<QPair<int, int>>>();
    QMapIterator<int, QVariantList> iter(sheetData);
    while (iter.hasNext()) {
        iter.next();
        int rowIndex = 0;
        int mergeColumnIndex = iter.key();
        int mergeRowStart = (-1);
        int mergeRowEnd = (-1);

        for (const auto& v : iter.value()) {
            QString readText = v.toString();
            QStringList startText = readText.split(excelMergeTextStart.toString());
            QStringList endText = readText.split(excelMergeTextEnd.toString());
            // QStringList mergeText = readText.split(excelMergeText.toString());

            if (startText.size() == 2) {
                mergeRowStart = rowIndex;
                mergeRowEnd = (-1);
            } else if (endText.size() == 2) {
                mergeRowEnd = rowIndex - mergeRowStart + 1;
            } else {
            }
            rowIndex++;

            if ((mergeRowStart >= 0) && (mergeRowEnd >= 0)) {
                mergeInfo[mergeColumnIndex].append(QPair<int, int>(mergeRowStart, mergeRowEnd));
                mergeRowStart = (-1);
                mergeRowEnd = (-1);
            }
        }
    }
    return mergeInfo;
}

void GuiExcel::constructMergeInfo(const int& sheetIndex, const QMap<int, QVariantList>& sheetData) {
    QMap<int, QList<QPair<int, int>>> mergeInfo = findMergeInfo(sheetData);
    QMapIterator<int, QList<QPair<int, int>>> iter(mergeInfo);
    while (iter.hasNext()) {
        iter.next();
        int columnIndex = iter.key();
        for (const auto& v : iter.value()) {
            updateMergeInfo(false, sheetIndex, columnIndex, v.first, v.second);
        }
    }
}

void GuiExcel::constructMergeSplitInfo(const QMap<int, QVariantList>& sheetData, const int& rowStart, const int& columnStart) {
    QMapIterator<int, QList<QPair<int, int>>> iter(findMergeInfo(sheetData));
    mClearMergeInfo.clear();
    mCopyMergeInfo.clear();

    while (iter.hasNext()) {
        iter.next();
        int mergeColumnIndex = iter.key();
        for (const auto& v : iter.value()) {
            int mergeRowStart = v.first;
            int mergeRowEnd = v.second;
            int splitColumnIndex = columnStart + mergeColumnIndex;
            int splitRowStart = rowStart + mergeRowStart;
            int splitRowEnd = mergeRowEnd;
            mClearMergeInfo.insert(splitColumnIndex, splitRowStart, splitRowEnd);
            mCopyMergeInfo.insert(mergeColumnIndex, mergeRowStart, mergeRowEnd);
        }
    }
}

void GuiExcel::updateDisplaySplitCell(const int& sheetIndex) {
    if (chcekExcleSheet(sheetIndex)) {
        return;
    }

    mExcelSheet[sheetIndex]->clearSpans();
}

void GuiExcel::updateDisplayMergeCell(const int& sheetIndex) {
    if (chcekExcleSheet(sheetIndex)) {
        return;
    }

    QMap<int, QList<QPair<int, int>>> mergeInfo = mMergeInfo[sheetIndex].isMergeInfo();
    QMapIterator<int, QList<QPair<int, int>>> iter(mergeInfo);
    int columnMax = mExcelSheet[sheetIndex]->columnCount();
    while (iter.hasNext()) {
        iter.next();
        int mergeColumnIndex = iter.key();
        for (const auto& v : iter.value()) {
            if (mExcelSheet[sheetIndex]) {
                int mergeRowStart = v.first;
                int mergeRowEnd = v.second;

                if (mergeColumnIndex >= columnMax) {
                    // qDebug() << "\t SkipMergeCell   :" << sheetIndex << mergeColumnIndex << mergeRowStart << mergeRowEnd
                    //          << ", ColumnMax :" << columnMax;
                    continue;
                }

                for (int clearRowIndex = mergeRowStart + 1; clearRowIndex < (mergeRowStart + mergeRowEnd); clearRowIndex++) {
                    mExcelSheet[sheetIndex]->setItem(clearRowIndex, mergeColumnIndex, new QTableWidgetItem(""));
                }
                // qDebug() << "\t UpdateMergeCell :" << sheetIndex << mergeColumnIndex << mergeRowStart << mergeRowEnd;
                mExcelSheet[sheetIndex]->setSpan(mergeRowStart, mergeColumnIndex, mergeRowEnd, 1);
            }
        }
    }
}

void GuiExcel::updateDisplaySheetHeaderAdjust(const int& sheetIndex) {
    if (chcekExcleSheet(sheetIndex)) {
        return;
    }

    if (mExcelSheet[sheetIndex]) {
        mExcelSheet[sheetIndex]->resizeColumnsToContents();
        mExcelSheet[sheetIndex]->resizeRowsToContents();
    }
}

void GuiExcel::updateDisplaySheetText(const int& sheetIndex) {
    if (chcekExcleSheet(sheetIndex)) {
        return;
    }

    QMap<int, QVariantList> newSheetData = QMap<int, QVariantList>();
    QVariant excelMergeTextStart = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeTextStart);
    QVariant excelMergeTextEnd = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeTextEnd);
    QVariant excelMergeText = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeText);
    QVariantList sheetData = isHandler()->getProperty(sheetIndex).toList();
    // qDebug() << "====================================================================================================";
    // qDebug() << "Length :" << sheetData.size() << rowMax << columnMax;
    // qDebug() << sheet << ":" << sheetData;

    for (int rowIndex = 0; rowIndex < sheetData.size(); rowIndex++) {
        QStringList rowDataList = sheetData[rowIndex].toStringList();
        for (int columnIndex = 0; columnIndex < rowDataList.size(); columnIndex++) {
            QString readText = rowDataList[columnIndex];
            QStringList startText = readText.split(excelMergeTextStart.toString());
            QStringList endText = readText.split(excelMergeTextEnd.toString());
            QStringList mergeText = readText.split(excelMergeText.toString());
            QString setText = QString();

            newSheetData[columnIndex].append(readText);
            if (startText.size() == 2) {
                setText = startText.at(1);
            } else if (endText.size() == 2) {
                setText = endText.at(1);
            } else {
                if (mergeText.size() == 2) {
                    setText = mergeText.at(1);
                } else {
                    setText = readText;
                }
            }
            // qDebug() << "Text[" << rowIndex << columnIndex <<"] :" << readText << "->" << setText;
            mExcelSheet[sheetIndex]->setItem(rowIndex, columnIndex, new QTableWidgetItem(setText));
        }
    }
    constructMergeInfo(sheetIndex, newSheetData);
    updateDisplayMergeCell(sheetIndex);  // Draw - Merge Cell
}

void GuiExcel::updateInitialExcelSheet() {
    mCurrentSheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription;
    mMergeInfo.clear();
    mModelIndex.clear();
    mCopyMergeInfo.clear();
    mClearMergeInfo.clear();
    mClearCellInfo.clear();

    for (const auto& action : mMenuActionItem) {
        delete action;
    }
    mMenuActionItem.clear();
    mMenuRight->clear();

    for (const auto& sheet : mExcelSheet) {
        // disconnect(sheet);
        // disconnect(sheet, &QTableWidget::itemSelectionChanged, nullptr, nullptr);
        disconnect(sheet, nullptr, nullptr, nullptr);
        sheet->clear();
        delete sheet;
    }
    mExcelSheet.clear();

    mGui->CellInfoContent->clear();
    for (int index = 0; index < mGui->TabWidget->count(); index++) {
        QWidget* currentWidget = mGui->TabWidget->widget(index);
        if (currentWidget) {
            mGui->TabWidget->removeTab(mGui->TabWidget->indexOf(currentWidget));
            delete currentWidget;
        }
    }
    mGui->TabWidget->clear();

    // qDebug() << "\t ExcelSheet : Init";
}

void GuiExcel::updateCellInfoContent(const int& sheetIndex, const int& row, const int& column) {
    if (chcekExcleSheet(sheetIndex)) {
        return;
    }

    QString text = QString();
    if ((mExcelSheet[sheetIndex]->item(row, column)) && (mExcelSheet[sheetIndex]->rowCount() > 0)) {
        text = mExcelSheet[sheetIndex]->item(row, column)->text();
    }

    if (mCurrentSheetIndex == sheetIndex) {
        // qDebug() << "Info :" <<  mCurrentSheetIndex << sheetIndex << text << row << column;
        mGui->CellInfoContent->setText(text);
    }
}

void GuiExcel::updateDefaultSheetFocus(const int& sheetIndex, const int& row, const int& column) {
    if (chcekExcleSheet(sheetIndex)) {
        return;
    }

    if (mExcelSheet[sheetIndex]->item(row, column) == nullptr) {
        mExcelSheet[sheetIndex]->setItem(row, column, new QTableWidgetItem(""));
    }

    if (mExcelSheet[sheetIndex]->item(row, column)) {
        mExcelSheet[sheetIndex]->item(row, column)->setSelected(true);
        // mExcelSheet[sheetIndex]->setCurrentItem(mExcelSheet[sheetIndex]->item(row, column));
        mExcelSheet[sheetIndex]->setFocus();
    }
}

void GuiExcel::updateDisplayKey(const int& keyValue) {
    int sheetIndex = mCurrentSheetIndex;
    if (chcekExcleSheet(sheetIndex)) {
        return;
    }

    if (keyValue == Qt::Key_Escape) {
        clearClipboardInfo(true);
        return;
    }

    if (keyValue == Qt::Key::Key_Delete) {
        QModelIndexList modelIndexs = mExcelSheet[sheetIndex]->selectionModel()->selectedIndexes();
        if (modelIndexs.size() > 0) {
            for (const auto& model : modelIndexs) {
                mExcelSheet[sheetIndex]->setItem(model.row(), model.column(), new QTableWidgetItem(""));
            }
        }
        return;
    }

    bool inputKeyOK = (keyValue == ivis::common::KeyTypeEnum::KeyInputValueOK);
    int row = mExcelSheet[sheetIndex]->currentRow();
    int column = mExcelSheet[sheetIndex]->currentColumn();
    bool editState = mExcelSheet[sheetIndex]->isPersistentEditorOpen(mExcelSheet[sheetIndex]->item(row, column));
    // qDebug() << "editState :" << editState << ", inputKeyOK :" << inputKeyOK;

    if ((editState) && (inputKeyOK)) {
        mExcelSheet[sheetIndex]->closePersistentEditor(mExcelSheet[sheetIndex]->item(row, column));
    } else if ((editState == false) && (inputKeyOK == false)) {
        mExcelSheet[sheetIndex]->openPersistentEditor(mExcelSheet[sheetIndex]->item(row, column));
    } else {
    }

    if (inputKeyOK) {
#if defined(USE_EXCEL_ARROW_KEY)
        updateDisplayArrowKey(keyValue);
#else
        int rowMax = mExcelSheet[sheetIndex]->rowCount();
        ivis::common::LIMIT_P(row, 1, 0, rowMax);
        mModelIndex[sheetIndex] = QPair<int, int>(row, column);
        mExcelSheet[sheetIndex]->setCurrentCell(row, column);
#endif
    }
}

void GuiExcel::updateDisplayArrowKey(const int& keyValue) {
#if defined(USE_EXCEL_ARROW_KEY)
    int sheetIndex = mCurrentSheetIndex;
    if (chcekExcleSheet(sheetIndex)) {
        return;
    }

    int row = mModelIndex[sheetIndex].first;
    int column = mModelIndex[sheetIndex].second;
    int rowMax = mExcelSheet[sheetIndex]->rowCount();
    int columnMax = mExcelSheet[sheetIndex]->columnCount();
    int keySate = 0;

    QString log = QString("Arrow[%1] : %2 %3").arg(sheetIndex).arg(row).arg(column);
    switch (keyValue) {
        case Qt::Key::Key_Left: {
            ivis::common::LIMIT_M(column, 1, 0, columnMax);
            break;
        }
        case Qt::Key::Key_Right: {
            ivis::common::LIMIT_P(column, 1, 0, columnMax);
            break;
        }
        case Qt::Key::Key_Up: {
            keySate = 1;
            ivis::common::LIMIT_M(row, 1, 0, rowMax);
            break;
        }
        case Qt::Key::Key_Down: {
            keySate = 2;
            ivis::common::LIMIT_P(row, 1, 0, rowMax);
            break;
        }
        case ivis::common::KeyTypeEnum::KeyInputValueOK: {
            ivis::common::LIMIT_P(row, 1, 0, rowMax);
            mExcelSheet[sheetIndex]->setCurrentCell(row, column);
            break;
        }
        default: {
            break;
        }
    }

    QMap<int, QList<QPair<int, int>>> mergeInfo = mMergeInfo[sheetIndex].isMergeInfo();
    QMapIterator<int, QList<QPair<int, int>>> iter(mergeInfo);
    while (iter.hasNext()) {
        iter.next();
        int mergeColumnIndex = iter.key();
        if (column != mergeColumnIndex) {
            continue;
        }

        qDebug() << "**********************************************************\n";
        for (const auto& v : iter.value()) {
            int mergeRowStart = v.first;
            int mergeRowCount = v.second;
            int mergeRowEnd = mergeRowStart + mergeRowCount;
            qDebug() << "Row Range :" << mergeRowStart << "<" << row << "<" << mergeRowEnd;

            if ((row > mergeRowStart) && (row < mergeRowEnd)) {
                // 병합 셀인 경우 병합 셀 다음의 인덱스로
                row = (keySate == 2) ? (mergeRowEnd) : (mergeRowStart);
                qDebug() << "\t\t 1 Row :" << row;
            }
            // if (row > rowMax) {
            //     // 마지막 병합 셀인 경우 최대값이 아니라 병합 셀의 시작 값으로
            //     row = rowMax;
            //     qDebug() << "\t\t 2 Row :" << row;
            // }
        }
    }

    mModelIndex[sheetIndex] = QPair<int, int>(row, column);
    log.append(QString(" -> %1, %2").arg(row).arg(column));
    qDebug() << log;
    updateCellInfoContent(sheetIndex, mModelIndex[sheetIndex].first, mModelIndex[sheetIndex].second);
#else
    mGui->CellInfoContent->clear();
#endif
}

void GuiExcel::updateDisplayExcelSheet() {
    qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    bool excelOpen = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelOpen).toBool();
    qDebug() << "Excel Update :" << ((excelOpen) ? ("Open") : ("New")) << ", Size :" << mExcelSheet.size();

    // Clear - Table, TableWidget, CellInfo, Menu, Action
    updateInitialExcelSheet();
    // qDebug() << "\t Excel : init";

    // Create : Action Item
    mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeCut] = mMenuRight->addAction("Cut");
    mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeCopy] = mMenuRight->addAction("Copy");
    mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypePaste] = mMenuRight->addAction("Paste");
    mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeInsert] = mMenuRight->addAction("Insert");
    mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeDelete] = mMenuRight->addAction("Delete");
    mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeMergeSplit] = mMenuRight->addAction("Merge/Split");
    // qDebug() << "\t Excel : action item";

    // Draw - Sheet
    QStringList sheetName = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetName).toStringList();
    QStringList descTitle = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelDescTitle).toStringList();
    QStringList otherTitle = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelOtherTitle).toStringList();
    QVariantList listRowCount = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelSheetCount).toList();
    int sheetIndex = ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription;
    // qDebug() << "\t Excel : read data";

    for (const auto& sheet : sheetName) {
        QStringList contentTitle =
            (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription) ? (descTitle) : (otherTitle);
        int rowMax = listRowCount.at(sheetIndex - ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription).toInt();
        int columnMax = contentTitle.size();

        // Draw Sheet
        mExcelSheet[sheetIndex] = new QTableWidget(rowMax, columnMax);
        mExcelSheet[sheetIndex]->setHorizontalHeaderLabels(contentTitle);
        // mExcelSheet[sheetIndex]->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
        mExcelSheet[sheetIndex]->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
        mGui->TabWidget->addTab(mExcelSheet[sheetIndex], sheet);
        // qDebug() << "\t\t Sheet[" << sheetIndex << "] : draw sheet";

        // Draw Sheet Data
        if (excelOpen) {
            updateDisplaySheetText(sheetIndex);
        }
        updateDefaultSheetFocus(sheetIndex, 0, 0);
        updateCellInfoContent(sheetIndex, mModelIndex[sheetIndex].first, mModelIndex[sheetIndex].second);
        // qDebug() << "\t\t Sheet[" << sheetIndex << "] : draw sheet data";

        connect(mExcelSheet[sheetIndex], &QTableWidget::cellChanged, [=](int row, int column) {
            // QString text = mExcelSheet[sheetIndex]->item(row, column)->text();
            // qDebug() << sheetIndex << ". cellChanged :" << column << "," << row << ", Text" << text;
            QVariantList editSheetInfo = QVariantList();
            if ((sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoPrivates) ||
                (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoInters)) {
                QString allString = QString();
                QVariantList nodeAddress = QVariantList();
                for (const auto& data : readExcelSheet(sheetIndex, QVariantList(), allString)) {
                    QVariantList rowLineData = data.toList();
                    if (rowLineData.size() > 0) {
                        nodeAddress.append(rowLineData.at(0));
                    }
                }
                if (nodeAddress.size() > 0) {
                    nodeAddress.removeAt(0);  // nodeAddress[0] = "TCName" -> 필요없음
                }
                editSheetInfo = QVariantList({sheetIndex, QVariant(nodeAddress)});
            }
            createSignal(ivis::common::EventTypeEnum::EventTypeEditExcelSheet, QVariant(editSheetInfo));
            updateDisplaySheetHeaderAdjust(sheetIndex);
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::customContextMenuRequested, [=](const QPoint& pos) {
            QAction* selectAction = mMenuRight->exec(mExcelSheet[sheetIndex]->mapToGlobal(QPoint((pos.x() + 20), (pos.y() + 5))));
            if (selectAction == mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeCut]) {
                updateDisplayClipboardInfo(ivis::common::ShortcutTypeEnum::ShortcutTypeCut);
            } else if (selectAction == mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeCopy]) {
                updateDisplayClipboardInfo(ivis::common::ShortcutTypeEnum::ShortcutTypeCopy);
            } else if (selectAction == mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypePaste]) {
                updateDisplayClipboardInfo(ivis::common::ShortcutTypeEnum::ShortcutTypePaste);
            } else if (selectAction == mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeInsert]) {
                updateDisplayEditCell(ivis::common::ShortcutTypeEnum::ShortcutTypeInsert);
            } else if (selectAction == mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeDelete]) {
                updateDisplayEditCell(ivis::common::ShortcutTypeEnum::ShortcutTypeDelete);
            } else if (selectAction == mMenuActionItem[ivis::common::ShortcutTypeEnum::ShortcutTypeMergeSplit]) {
                updateDisplayEditCell(ivis::common::ShortcutTypeEnum::ShortcutTypeMergeSplit);
            } else {
                qDebug() << "Fail to menu right selection action item";
            }
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::cellDoubleClicked, [=](int row, int column) {
            // Config_Signal : 3(Description), 9    Input_Signal : 4    Input_Data : 5    Output_Signal : 6
            bool notSupport = false;
            bool supportAutoCompleteVehicle = false;
            if (sheetIndex == ivis::common::PropertyTypeEnum::PropertyTypeDetailInfoDescription) {
                notSupport = (column != static_cast<int>(ivis::common::ExcelSheetTitle::Description::ConfigSignal));
            } else {
                notSupport = ((column != static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)) &&
                              (column != static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal)) &&
                              (column != static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)) &&
                              (column != static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal)) &&
                              (column != static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue)) &&
                              (column != static_cast<int>(ivis::common::ExcelSheetTitle::Other::ConfigSignal)));
            }

            if (notSupport) {
                qDebug() << "Fail to not support auto complete :" << sheetIndex << column;
                return;
            }

            mSelectItem = mExcelSheet[sheetIndex]->item(row, column);
            if (mSelectItem == nullptr) {
                mExcelSheet[sheetIndex]->setItem(row, column, new QTableWidgetItem(""));
                mSelectItem = mExcelSheet[sheetIndex]->item(row, column);
            }

            if ((column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData)) ||
                (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue))) {
                QString vehicleType = QString();
                int vehicleTypeIndex = static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType);
                if (mExcelSheet[sheetIndex]->item(row, vehicleTypeIndex) != nullptr) {
                    vehicleType = mExcelSheet[sheetIndex]->item(row, vehicleTypeIndex)->text();
                }

                if (vehicleType.isEmpty()) {
                    QVariant vehicleTypeList = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeVehicleType);
                    for (const auto& vehicle : vehicleTypeList.toStringList()) {
                        if (vehicleType.size() > 0) {
                            vehicleType.append(", ");
                        }
                        vehicleType.append(vehicle);
                    }
                }

                QString signalName = QString();
                int signalColumnIndex = ((column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputData))
                                             ? static_cast<int>(ivis::common::ExcelSheetTitle::Other::InputSignal)
                                             : static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputSignal));
                if (mExcelSheet[sheetIndex]->item(row, signalColumnIndex) != nullptr) {
                    signalName = mExcelSheet[sheetIndex]->item(row, signalColumnIndex)->text();
                }

                if (signalName.size() == 0) {
                    return;
                }

                QVariantList inputDataInfo = QVariantList();
                bool outputState = (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::OutputValue));
                if (signalName.indexOf("SFC.") == 0) {
                    inputDataInfo = QVariantList({true, outputState, vehicleType, signalName});
                    createSignal(ivis::common::EventTypeEnum::EventTypeAutoCompleteInputData, inputDataInfo);
                } else if (signalName.indexOf("Vehicle.") == 0) {
                    inputDataInfo = QVariantList({false, outputState, vehicleType, signalName});
                    createSignal(ivis::common::EventTypeEnum::EventTypeAutoCompleteInputData, inputDataInfo);
                } else {
                }
            } else if (column == static_cast<int>(ivis::common::ExcelSheetTitle::Other::VehicleType)) {
                updateDisplayAutoCompleteVehicle();
                mExcelSheet[sheetIndex]->clearFocus();
            } else {
                updateDisplayAutoComplete(true, column);
                mExcelSheet[sheetIndex]->clearFocus();
            }
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::cellClicked, [=](int row, int column) {
            mModelIndex[sheetIndex] = QPair<int, int>(row, column);
            // qDebug() << "Click[" << sheetIndex << "] :" << mModelIndex[sheetIndex].first << mModelIndex[sheetIndex].second;
            updateCellInfoContent(sheetIndex, mModelIndex[sheetIndex].first, mModelIndex[sheetIndex].second);
        });
#if 0
        connect(mExcelSheet[sheetIndex], &QTableWidget::itemClicked, [=](QTableWidgetItem *item) {
            qDebug() << "1 itemClicked[" << sheetIndex << "] :" << item;
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::itemActivated, [=](QTableWidgetItem *item) {
            qDebug() << "2 itemActivated[" << sheetIndex << "] :" << item;
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::itemEntered, [=](QTableWidgetItem *item) {
            qDebug() << "3 itemEntered[" << sheetIndex << "] :" << item;
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::cellEntered, [=](int row, int column) {
            qDebug() << "4 cellEntered[" << sheetIndex << "] :" << row << column;
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::cellActivated, [=](int row, int column) {
            qDebug() << "5 cellActivated[" << sheetIndex << "] :" << row << column;
        });
        connect(mExcelSheet[sheetIndex], &QTableWidget::itemSelectionChanged, [=]() {
            QString text = QString();
            if (mExcelSheet[sheetIndex]->currentItem()) {
                text = mExcelSheet[sheetIndex]->currentItem()->text();
            }
            mGui->CellInfoContent->setText(text);
        });
        connect(mExcelSheet[sheetIndex]->horizontalHeader(), &QHeaderView::sectionResized,
                                                                [=](int logicalIndex, int oldSize, int newSize) {
            qDebug() << sheetIndex << ". H_sectionResized :" << logicalIndex << oldSize << newSize;
        });
        connect(mExcelSheet[sheetIndex]->verticalHeader(), &QHeaderView::sectionResized,
                                                                [=](int logicalIndex, int oldSize, int newSize) {
            qDebug() << sheetIndex << ". V_sectionResized :" << logicalIndex << oldSize << newSize;
        });
#endif

        // Resize - Cell Width/Height
        updateDisplaySheetHeaderAdjust(sheetIndex);
        sheetIndex++;
    }

    // qDebug() << "\t Excel : complete";
    qDebug() << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
}

void GuiExcel::updateDisplayAutoComplete(const bool& show, const int& columnIndex) {
    qDebug() << "updateDisplayAutoComplete :" << show << columnIndex;

    QStringList list = QStringList();
    list.append(isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressAll).toStringList());
    list.append(isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressPrivate).toStringList());
    list.append(isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeNodeAddressInter).toStringList());

    if (mAutoComplete == nullptr) {
        mAutoComplete = new AutoCompleteDialog(isHandler()->getScreen(), QString("AutoComplete"), list);
        connect(mAutoComplete, &AutoCompleteDialog::signalAutoCompleteSelectedText, [=](const QString& text) {
            if (mSelectItem) {
                mSelectItem->setText(text);
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
        mAutoComplete->setAutoCompleteStringList(list);
        mAutoComplete->show();
        if (mSelectItem) {
            mAutoComplete->setInputText(mSelectItem->text());
        }
    } else {
        mAutoComplete->hide();
    }
}

void GuiExcel::updateDisplayAutoCompleteVehicle() {
    if (mAutoCompleteVehicle == nullptr) {
        QStringList itemList = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeVehicleType).toStringList();
        mAutoCompleteVehicle = new AutoCompleteVehicleDialog(isHandler()->getScreen(), QString("Select Vehicle"), itemList);

        connect(mAutoCompleteVehicle, &AutoCompleteVehicleDialog::signalSelecteCheck, [=](const QString& vehicleTypeText) {
            if (mSelectItem) {
                mSelectItem->setText(vehicleTypeText);
            }
            mAutoCompleteVehicle->hide();
            mAutoCompleteVehicle->finished(true);
        });
        connect(mAutoCompleteVehicle, &QDialog::finished, [=]() {
            disconnect(mAutoCompleteVehicle);
            delete mAutoCompleteVehicle;
            mAutoCompleteVehicle = nullptr;
        });
    }
    mAutoCompleteVehicle->show();
}

void GuiExcel::updateDisplayAutoCompleteInputData(const bool& sfcSignal, const bool& outputState) {
    if (mAutoCompleteInputData == nullptr) {
        QStringList subTitle = QStringList({"Value Enum"});
        QStringList valueEnum =
            isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeInputDataValuEnum).toStringList();
        QSize widgetSize = QSize(500, 300);
        QList<QStringList> matchingTable = QList<QStringList>();

        if ((sfcSignal == false) && (outputState == false)) {
            QVariant data = QVariant();
            QVariant vehicleTypeList = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeVehicleType);

            for (const auto& vehicle : vehicleTypeList.toStringList()) {
                int propertyType = 0;
                if (vehicle.compare(VEHICLE_TYPE_ICV) == false) {
                    propertyType = ivis::common::PropertyTypeEnum::PropertyTypeInputDataMatchingTableICV;
                } else if (vehicle.compare(VEHICLE_TYPE_EV) == false) {
                    propertyType = ivis::common::PropertyTypeEnum::PropertyTypeInputDataMatchingTableEV;
                } else if (vehicle.compare(VEHICLE_TYPE_FCEV) == false) {
                    propertyType = ivis::common::PropertyTypeEnum::PropertyTypeInputDataMatchingTableFCEV;
                } else if (vehicle.compare(VEHICLE_TYPE_PHEV) == false) {
                    propertyType = ivis::common::PropertyTypeEnum::PropertyTypeInputDataMatchingTablePHEV;
                } else if (vehicle.compare(VEHICLE_TYPE_HEV) == false) {
                    propertyType = ivis::common::PropertyTypeEnum::PropertyTypeInputDataMatchingTableHEV;
                } else {
                    continue;
                }

                subTitle.append(vehicle);
                data = isHandler()->getProperty(propertyType);
                matchingTable.append(data.toStringList());
            }
            subTitle.append("System");
            data = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeInputDataMatchingTableSystem);
            matchingTable.append(data.toStringList());

            widgetSize = (vehicleTypeList.toStringList().size() > 3) ? (QSize(864, 300)) : (QSize(664, 300));
        }
        mAutoCompleteInputData = new SelectModuleDialog(isHandler()->getScreen(), valueEnum, outputState);
        mAutoCompleteInputData->updateSelectListInfo(valueEnum, matchingTable);
        mAutoCompleteInputData->updateSelectWidgetInfo(QString("Select Data"), subTitle, widgetSize);
        connect(mAutoCompleteInputData, &SelectModuleDialog::signalModuleSelected,
                [=](const QList<QPair<int, QString>>& selectModule) {
                    if (mSelectItem) {
                        QString selectValueEnum = QString();
                        for (const auto& select : selectModule) {
                            QStringList lineStr = select.second.split(":");
                            if (lineStr.size() != 2) {
                                continue;
                            }

                            QString temp = ((sfcSignal) ? (lineStr.at(0)) : (lineStr.at(1)));
                            if (outputState) {
                                temp = ((sfcSignal) ? (lineStr.at(1)) : (lineStr.at(0)));
                            }
                            temp.remove("\"");

                            if (selectValueEnum.size() > 0) {
                                selectValueEnum.append(", ");
                            }
                            selectValueEnum.append(temp);
                        }
                        mSelectItem->setText(selectValueEnum);
                    }
                    mAutoCompleteInputData->hide();
                    mAutoCompleteInputData->finished(true);
                });
        connect(mAutoCompleteInputData, &QDialog::finished, [=]() {
            disconnect(mAutoCompleteInputData);
            delete mAutoCompleteInputData;
            mAutoCompleteInputData = nullptr;
        });
    }
    mAutoCompleteInputData->show();
}

void GuiExcel::printMergeInfo(const QString& title, const bool& mergeSplit) {
#if 0
    qDebug() << "\t ****************************************************************************";
    qDebug() << "\t [" << title << "]";
    qDebug() << "\t >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
    QMap<int, ExcelSheet> mergeInfo = mMergeInfo;
    QString type = QString("All");
    if (mergeSplit) {
        mergeInfo.clear();
        mergeInfo[mergeInfo.size()] = mClearMergeInfo;
        mergeInfo[mergeInfo.size()] = mCopyMergeInfo;
        type = QString("Copy/Clear");
    }
    for (ExcelSheet merge : mergeInfo) {
        QMapIterator<int, QList<QPair<int, int>>> iter(merge.isMergeInfo());
        while (iter.hasNext()) {
            iter.next();
            for (const auto& info : iter.value()) {
                qDebug() << "\t\t " << type << ":" << iter.key() << info.first << info.second;
            }
        }
        qDebug() << "\t\t -------------------------------------------------";
    }
    qDebug() << "\t <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
#endif
}

void GuiExcel::copyClipboardInfo(const bool& cutState) {
    int sheetIndex = mCurrentSheetIndex;
    if (chcekExcleSheet(sheetIndex)) {
        return;
    }

    QModelIndexList modelIndexs = mExcelSheet[sheetIndex]->selectionModel()->selectedIndexes();
    if (modelIndexs.size() == 0) {
        qDebug() << "Select cell count : 0";
        return;
    }

    QVariant excelMergeTextStart = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeTextStart);
    QVariant excelMergeTextEnd = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeTextEnd);
    QVariant excelMergeText = isHandler()->getProperty(ivis::common::PropertyTypeEnum::PropertyTypeExcelMergeText);

    int rowStart = modelIndexs.at(0).row();
    int rowEnd = modelIndexs.last().row() - rowStart + 1;
    int rowMax = (rowStart + rowEnd);
    int columnStart = modelIndexs.at(0).column();
    int columnEnd = modelIndexs.last().column() - columnStart + 1;
    int columnMax = (columnStart + columnEnd);

    if (rowEnd == 1) {  // 병합된 셀의 경우 다시 선택시 셀 선택한 카운트(rowEnd) 값이 1로 나오는 경우 발생함
        int mergeCount = isMergeCell(sheetIndex, columnStart, rowStart);
        if (mergeCount > 0) {
            rowEnd = mergeCount;
            rowMax = (rowStart + rowEnd);
            // qDebug() << "isMergeCell :" << mergeCount << rowEnd << rowMax;
        }
    }

    // qDebug() << "\t Copy/Cut Sheet[" << sheetIndex << "] - Index :" << rowStart << columnStart << rowEnd << columnEnd << ","
    //          << rowMax << columnMax;

    QMap<int, QVariantList> newSheetData = QMap<int, QVariantList>();
    QString clipboardData = QString();
    QVariantList readIndexInfo = QVariantList({rowStart, columnStart, rowMax, columnMax});
    QVariantList sheetData = readExcelSheet(sheetIndex, readIndexInfo, clipboardData);

    clipboardData.resize(clipboardData.size() - 1);
    QGuiApplication::clipboard()->setText(clipboardData);

    for (const auto& rowListData : sheetData) {
        int columnIndex = 0;
        for (const auto& data : rowListData.toList()) {
            newSheetData[columnIndex++].append(data.toString());
        }
    }

    // Copy, Paste : Cell, Merge Info
    mCopyMergeInfo.clear();
    mClearMergeInfo.clear();
    mClearCellInfo.clear();

    constructMergeSplitInfo(newSheetData, rowStart, columnStart);

    if (cutState) {
        mClearCellInfo.append(sheetIndex);
        mClearCellInfo.append(rowStart);
        mClearCellInfo.append(columnStart);
        mClearCellInfo.append(rowMax);
        mClearCellInfo.append(columnMax);

        for (int clearRowIndex = rowStart; clearRowIndex < rowMax; clearRowIndex++) {
            for (int clearColumnIndex = columnStart; clearColumnIndex < columnMax; clearColumnIndex++) {
                if (mExcelSheet[sheetIndex]->item(clearRowIndex, clearColumnIndex) == nullptr) {
                    mExcelSheet[sheetIndex]->setItem(clearRowIndex, clearColumnIndex, new QTableWidgetItem(""));
                }
                mExcelSheet[sheetIndex]->item(clearRowIndex, clearColumnIndex)->setBackground(QColor(200, 200, 200));
            }
        }
    }
}

int GuiExcel::clearClipboardInfo(const bool& escapeKeyClear) {
    int clearSheetIndex = ((mClearCellInfo.size() == 5) ? (mClearCellInfo.at(0)) : (-1));

    if (clearSheetIndex != (-1)) {
        int clearRowStart = mClearCellInfo.at(1);
        int clearColumnStart = mClearCellInfo.at(2);
        int clearRowEnd = mClearCellInfo.at(3);
        int clearColumnEnd = mClearCellInfo.at(4);

        for (int clearRowIndex = clearRowStart; clearRowIndex < clearRowEnd; clearRowIndex++) {
            for (int clearColumnIndex = clearColumnStart; clearColumnIndex < clearColumnEnd; clearColumnIndex++) {
                mExcelSheet[clearSheetIndex]->setItem(clearRowIndex, clearColumnIndex, new QTableWidgetItem(""));
                mExcelSheet[clearSheetIndex]->item(clearRowIndex, clearColumnIndex)->setBackground(QColor(255, 255, 255));
            }
        }
        mClearCellInfo.clear();
    }

    if (escapeKeyClear) {
        mClearMergeInfo.clear();
        mCopyMergeInfo.clear();
        QGuiApplication::clipboard()->clear();
    }

    return clearSheetIndex;
}

void GuiExcel::pasteClipboardInfo() {
    int sheetIndex = mCurrentSheetIndex;
    if (chcekExcleSheet(sheetIndex)) {
        return;
    }

    if ((QApplication::clipboard()->mimeData() == nullptr) || (QApplication::clipboard()->mimeData()->hasText() == false)) {
        qDebug() << "Clipboard data invalid";
        return;
    }

    QModelIndexList modelIndexs = mExcelSheet[sheetIndex]->selectionModel()->selectedIndexes();
    int selectCellCount = modelIndexs.size();
    if ((selectCellCount != 1)) {
        qDebug() << "Fail to select cell - count :" << selectCellCount;
        return;
    }

    QStringList clipboardData = QApplication::clipboard()->mimeData()->text().split("\n");
    int rowStart = (selectCellCount == 1) ? (modelIndexs.at(0).row()) : (0);
    int columnStart = (selectCellCount == 1) ? (modelIndexs.at(0).column()) : (0);
    int rowMax = mExcelSheet[sheetIndex]->rowCount();
    int columnMax = mExcelSheet[sheetIndex]->columnCount();
    int rowCount = clipboardData.size() + rowStart;  // Row : 0(Header), 1 ~ Count(Item)

    if (rowCount >= rowMax) {
        mExcelSheet[sheetIndex]->setRowCount(rowCount);
    }

    for (int rowIndex = 0; rowIndex < clipboardData.size(); rowIndex++) {
        QStringList rowDataList = clipboardData[rowIndex].split("\t");
        for (int columnIndex = 0; columnIndex < rowDataList.size(); columnIndex++) {
            int itemRowIndex = (rowStart + rowIndex);
            int itemColumnIndex = (columnStart + columnIndex);
            if (itemColumnIndex >= columnMax) {
                continue;
            }
            QString setText = rowDataList[columnIndex];
            mExcelSheet[sheetIndex]->setItem(itemRowIndex, itemColumnIndex, new QTableWidgetItem(setText));
        }
    }

    int clearSheetIndex = clearClipboardInfo(false);
    if (clearSheetIndex >= 0) {
        QMapIterator<int, QList<QPair<int, int>>> iterClear(mClearMergeInfo.isMergeInfo());
        while (iterClear.hasNext()) {
            iterClear.next();
            for (const auto& info : iterClear.value()) {
                updateMergeInfo(true, clearSheetIndex, iterClear.key(), info.first, info.second);
            }
        }
        mClearMergeInfo.clear();

        if (sheetIndex != clearSheetIndex) {
            updateDisplaySplitCell(clearSheetIndex);
            updateDisplayMergeCell(clearSheetIndex);
        }
    }

    QMapIterator<int, QList<QPair<int, int>>> iterCopy(mCopyMergeInfo.isMergeInfo());
    while (iterCopy.hasNext()) {
        iterCopy.next();
        int mergeColumnIndex = columnStart + iterCopy.key();
        for (const auto& info : iterCopy.value()) {
            int mergeRowStart = rowStart + info.first;
            int mergeRowEnd = info.second;
            updateMergeInfo(false, sheetIndex, mergeColumnIndex, mergeRowStart, mergeRowEnd);
        }
    }
    mCopyMergeInfo.clear();
    QGuiApplication::clipboard()->clear();

    updateDisplaySplitCell(sheetIndex);
    updateDisplayMergeCell(sheetIndex);
}

void GuiExcel::updateDisplayClipboardInfo(const int& clipboardType) {
    if ((clipboardType == ivis::common::ShortcutTypeEnum::ShortcutTypeCopy) ||
        (clipboardType == ivis::common::ShortcutTypeEnum::ShortcutTypeCut)) {
        copyClipboardInfo(clipboardType == ivis::common::ShortcutTypeEnum::ShortcutTypeCut);
    } else if (clipboardType == ivis::common::ShortcutTypeEnum::ShortcutTypePaste) {
        pasteClipboardInfo();
    } else if (clipboardType == ivis::common::ShortcutTypeEnum::ShortcutTypeUndo) {
        qDebug() << "\t\t Shortcut not implemented : Undo";
    } else if (clipboardType == ivis::common::ShortcutTypeEnum::ShortcutTypeRedo) {
        qDebug() << "\t\t Shortcut not implemented : Redo";
    } else {
    }
}

void GuiExcel::updateDisplayReceiveKeyFocus() {
    int sheetIndex = mCurrentSheetIndex;
    if (chcekExcleSheet(sheetIndex)) {
        return;
    }

    mExcelSheet[sheetIndex]->setFocus();  // Sheet 활성화 상태에서 화면 전화시 focus 설정 되도록
}

void GuiExcel::updateDisplayEditCell(const int& editType) {
    int sheetIndex = mCurrentSheetIndex;
    if (chcekExcleSheet(sheetIndex)) {
        return;
    }

    QModelIndexList modelIndexs = mExcelSheet[sheetIndex]->selectionModel()->selectedIndexes();
    int selectCellCount = modelIndexs.size();
    int rowStart = (selectCellCount == 0) ? (mExcelSheet[sheetIndex]->currentRow()) : (modelIndexs.at(0).row());
    int rowEnd = (selectCellCount == 0) ? (1) : (modelIndexs.last().row() - rowStart + 1);
    int columnStart = (selectCellCount == 0) ? (mExcelSheet[sheetIndex]->currentColumn()) : (modelIndexs.at(0).column());
    int columnEnd = (selectCellCount == 0) ? (1) : (modelIndexs.last().column() - columnStart + 1);
    bool lastRowIndex = ((rowStart + rowEnd) == mExcelSheet[sheetIndex]->rowCount());

    if (rowEnd <= 0) {
        QList<int> rowList = QList<int>();
        for (const auto& index : modelIndexs) {
            rowList.append(index.row());
        }
        if (rowList.size() > 0) {
            rowStart = (*std::min_element(rowList.constBegin(), rowList.constEnd()));
            rowEnd = (*std::max_element(rowList.constBegin(), rowList.constEnd())) - rowStart + 1;
        }
    }
    qDebug() << "EditCell[" << sheetIndex << "] - Index :" << selectCellCount << rowStart << rowEnd << columnStart << columnEnd;

    ivis::common::LIMIT(rowStart, 0, mExcelSheet[sheetIndex]->rowCount());
    ivis::common::LIMIT(columnStart, 0, mExcelSheet[sheetIndex]->columnCount());

    if ((editType == ivis::common::ShortcutTypeEnum::ShortcutTypeInsert) ||
        (editType == ivis::common::ShortcutTypeEnum::ShortcutTypeDelete)) {
        for (int rowIndex = 0; rowIndex < rowEnd; rowIndex++) {
            if (editType == ivis::common::ShortcutTypeEnum::ShortcutTypeInsert) {
                mExcelSheet[sheetIndex]->insertRow((lastRowIndex) ? (mExcelSheet[sheetIndex]->rowCount()) : (rowStart));
            } else {
                mExcelSheet[sheetIndex]->removeRow(rowStart);
                ivis::common::LIMIT(rowStart, 0, mExcelSheet[sheetIndex]->rowCount());
                mExcelSheet[sheetIndex]->setCurrentCell(rowStart, columnStart);
            }
        }
    } else if (editType == ivis::common::ShortcutTypeEnum::ShortcutTypeMergeSplit) {
        bool columnSelectWarning = (columnEnd > 1);
        if (columnSelectWarning) {
            createSignal(ivis::common::EventTypeEnum::EventTypeCellMergeSplitWarning, QVariant());
        } else {
            if (updateMergeInfo(false, sheetIndex, columnStart, rowStart, rowEnd)) {
                updateDisplaySplitCell(sheetIndex);
            }
            updateDisplayMergeCell(sheetIndex);
        }
    } else {
        qDebug() << "Fail to edit type :" << editType;
        return;
    }

    createSignal(ivis::common::EventTypeEnum::EventTypeEditExcelSheet, QVariant());
    updateDisplaySheetHeaderAdjust(sheetIndex);
}

void GuiExcel::slotPropertyChanged(const int& type, const QVariant& value) {
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
        case ivis::common::PropertyTypeEnum::PropertyTypeExcelOpen: {
            updateDisplayExcelSheet();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeReadExcelSheetBeforeSave: {
            readAllExcelSheet();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeClipboardType: {
            int clipboardType = value.toInt();
            updateDisplayClipboardInfo(clipboardType);
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeShortcutType: {
            int shortcutType = value.toInt();
            updateDisplayEditCell(shortcutType);
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeKey: {
            int keyValue = value.toInt();
            updateDisplayKey(keyValue);
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeArrowKey: {
            int keyValue = value.toInt();
            updateDisplayArrowKey(keyValue);
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeReceiveKeyFocus: {
            updateDisplayReceiveKeyFocus();
            break;
        }
        case ivis::common::PropertyTypeEnum::PropertyTypeInputDataVisible: {
            QVariantList info = value.toList();
            if (info.size() == 2) {
                bool sfcSignal = info.at(0).toBool();
                bool outputState = info.at(1).toBool();
                updateDisplayAutoCompleteInputData(sfcSignal, outputState);
            }
            break;
        }
        default: {
            break;
        }
    }
}
