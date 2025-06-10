#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QFrame>
#include <QRadioButton>
#include <QListWidget>

#include "CommonUtil.h"

#define USE_UPDATE_THREAD
#if defined(USE_UPDATE_THREAD)
#include <QWidget>
#include <QThread>
// #include <QWaitCondition>
#include <QPlainTextEdit>
#include <QTextCursor>
#include <QScrollBar>

class DialogUpdateThread : public QObject {
    Q_OBJECT

public:
    DialogUpdateThread(QPlainTextEdit* widget) : mWidget(widget) {
    }

public slots:
    void slotUpdateThread(const QStringList& detailLog) {
        // ivis::common::CheckTimer checkTimer;
        // ivis::common::CheckTimer checkTimer2;
        if ((mWidget) && (mWidget->isVisible())) {
            int count = 0;
            for (int index = 0; index < detailLog.size(); index += mDataSize) {
                QString data = detailLog.mid(index, mDataSize).join("\n") + "\n";
                QMetaObject::invokeMethod(
                    mWidget,
                    [this, data]() {
                        QTextCursor cursor = mWidget->textCursor();
                        cursor.movePosition(QTextCursor::End);
                        cursor.insertText(data);
                        mWidget->verticalScrollBar()->setValue(mWidget->verticalScrollBar()->maximum());
                    },
                    Qt::QueuedConnection);
                // checkTimer2.check(QString("update[%1] : %2 -> %3").arg(count++).arg(index).arg(index + mDataSize));
                QThread::msleep(50);
            }
            mWidget->verticalScrollBar()->setValue(mWidget->verticalScrollBar()->maximum());
        }
        // checkTimer.check(QString("slotUpdateThread : %1").arg(detailLog.size()));
    }

private:
    const int mDataSize = 40000;
    QPlainTextEdit* mWidget = nullptr;
};
#endif

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog {
    Q_OBJECT

    REGISTER_WRITABLE_CONTAINER(QHash, int, QVariant, Property)
    REGISTER_WRITABLE_VALUE(QStringList, LogData, QStringList())

public:
    enum {
        DataTypeInvalid = 0,
        DataTypeDialogType,
        DataTypePrevDialogType,
        DataTypeDialogInfo,
        DataTypePrevDialogInfo,
        DataTypeKeepDialog,
        DataTypeDisplayType,
        DataTypePrevDisplayType,
        DataTypeAppMode,
        DataTypeSelectRadioList,
        DataTypeScreenRect,
        DataTypeSelectAll,
        DataTypeMultiCheck,
        DataTypeSelectInputData,
        DataTypeSelectListCheckIndex,
        DataTypeViewLogStop,
        DataTypeAutoCompleteListInput,
        DataTypeAutoCompleteListKeyword,
        DataTypeAutoCompleteListUseOnlyKeyword,
        DataTypeAutoCompleteListNormal,
        DataTypeAutoCompleteListSfc,
        DataTypeAutoCompleteListVehicle,
        DataTypeAutoCompleteListTCName,
        DataTypeAutoCompleteListDependentName,
    };
    enum {
        DialogTypeInvalid = 0,
        DialogTypeAppModeCheck,
        DialogTypeAppModeRadio,
        DialogTypeCycleMode,
        DialogTypeSelectMoudleInfo,
        DialogTypeSelectTCFile,
        DialogTypeSelectLogFile,
        DialogTypeSelectValueEnumInput,
        DialogTypeSelectValueEnumOutput,
        DialogTypeSelectMatchingTableCV,
        DialogTypeSelectMatchingTablePV,
        DialogTypeSelectValueResult,
        DialogTypeSelectNegative,
        DialogTypeSelectVehiclePV,
        DialogTypeSelectVehicleCV,
        DialogTypeSelectVehicleType,
        DialogTypeEnterText,
        DialogTypeTestReportTC,
        DialogTypeTestReportGCOV,
        DialogTypeLogDisplay,
        DialogTypeViewLogInfo,
        DialogTypeViewLogFileInfo,
        DialogTypeNodeView,
        DialogTypeAutoComplete,
        DialogTypeGenType,
        DialogTypeAutoCompleteNormal,
        DialogTypeSelectValue,
    };

private:
    enum {
        DisplayTypeInvalid = 0,
        DisplayTypeAppMode,
        DisplayTypeSelectRadio,
        DisplayTypeSelectList,
        DisplayTypeSelectOption,
        DisplayTypeEnterText,
        DisplayTypeTestReport,
        DisplayTypeLogDisplay,
        DisplayTypeViewLog,
        DisplayTypeAutoCompleteNormal,
        DisplayTypeAutoComplete,
        DisplayTypeSelectValue,
        DisplayTypeMax,
    };
    enum {
        RefreshTypeInvalid = 0,
        RefreshTypeFindHide,
        RefreshTypeFindShow,
        RefreshTypeStop,
        RefreshTypeClear,
        RefreshTypeSearch,
        RefreshTypeSearchPrevious,
        RefreshTypeSearchNext,
    };

public:
    explicit Dialog(const QRect& rect, QWidget* parent = nullptr);
    ~Dialog();

    void drawDialog(const int& type, const QVariantList& info);

private:
    void controlConnet(const int& displayType);
    void connectAppMode(const bool& state);
    void connectSelectRadio(const bool& state);
    void connectSelectRadio2(const bool& state);
    void connectSelectList(const bool& state);
    void connectSelectOption(const bool& state);
    void connectEnterText(const bool& state);
    void connectTestReport(const bool& state);
    void connectLogDisplay(const bool& state);
    void connectViewLog(const bool& state);
    void connectAutoComplete(const bool& state);
    void connectAutoCompleteNormal(const bool& state);
    void connectSelectValue(const bool& state);

    QRect updateMainRect();
    void updateDisplay(const int& displayType, const QString& title);

    QList<QPair<QFrame*, QRadioButton*>> isRadioWidget() const;
    QList<QCheckBox*> isCheckWidget(const bool& option1) const;
    void updateSelectListCheckState(const bool& allCheck, const QStringList& selectList);
    void refreshViewLog(const int& refreshType);
    void updateAutoCompleteSuggestionsList(const bool& normal, const bool& sfc, const bool& vehicle, const bool& tcName,
                                           const bool& dependentName);

    bool updateAppMode(const QVariantList& info);
    bool updateSelectRadio(const QVariantList& info);
    bool updateSelectRadio2(const QVariantList& info);
    bool updateSelectList(const QVariantList& info);
    bool updateSelectOption(const QVariantList& info);
    bool updateInputText(const QVariantList& info);
    bool updateTestReport(const QVariantList& info);
    bool updateLogDisplay(const QVariantList& info);
    bool updateViewLog(const QVariantList& info);
    bool updateAutoComplete(const QVariantList& info);
    bool updateAutoCompleteNormal(const QVariantList& info);
    bool updateSelectValue(const QVariantList& info);

#if defined(USE_DIALOG_KEY_EVENT)
protected:
    void keyPressEvent(QKeyEvent* keyEvent) override;
    void keyReleaseEvent(QKeyEvent* keyEvent) override;
#endif

signals:
    void signalSelectRadio(const int& selectIndex);
    void signalSelectRadioValue(const QString& text);
    void signalSelectListItem(const QList<QPair<int, QString>>& selectItem);
    void signalSelectOption(const bool& option1, const QList<QPair<QString, bool>>& option2);
    void signalScrollBarValueChanged(const int& value);
    void signalEnterTextChanged(const QString& text);
    void signalLogDisplayClicked(const bool& hide, const bool& detail);
    void signalViewLogClicked(const bool& close);
    void signalAutoCompleteSelected(const QString& text);
    void signalSelectValue(const int& value);
    void signalUpdateThread(const QStringList& detailLog);

private:
    Ui::Dialog* mGui;
    QStandardItemModel mModel = QStandardItemModel();
    QListWidget* mAutoCompleteListWidget = nullptr;

#if defined(USE_UPDATE_THREAD)
    QSharedPointer<QThread> mThread;
    QSharedPointer<DialogUpdateThread> mUpdateThread;
#endif
};

#endif  // DIALOG_H
