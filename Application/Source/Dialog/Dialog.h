#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QFrame>
#include <QRadioButton>

#include "CommonUtil.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog {
    Q_OBJECT

    REGISTER_WRITABLE_PROPERTY(int, DialogType, 0, false)
    REGISTER_WRITABLE_PROPERTY(int, PrevDialogType, 0, false)
    REGISTER_WRITABLE_PROPERTY(QVariantList, DialogInfo, QVariantList(), false)
    REGISTER_WRITABLE_PROPERTY(QVariantList, PrevDialogInfo, QVariantList(), false)
    REGISTER_WRITABLE_PROPERTY(bool, KeepDialog, false, false)
    REGISTER_WRITABLE_PROPERTY(int, DisplayType, 0, false)
    REGISTER_WRITABLE_PROPERTY(int, PrevDisplayType, 0, false)
    REGISTER_WRITABLE_PROPERTY(int, AppMode, 0, false)
    REGISTER_WRITABLE_PROPERTY(QRect, ScreenRect, QRect(), false)
    REGISTER_WRITABLE_PROPERTY(bool, SelectAll, false, false)
    REGISTER_WRITABLE_PROPERTY(bool, MultiCheck, true, false)
    REGISTER_WRITABLE_PROPERTY(int, CheckModelIndex, (-1), false)
    REGISTER_WRITABLE_PROPERTY(bool, FindLog, false, false)
    REGISTER_WRITABLE_PROPERTY(bool, ViewLogStop, false, false)
    REGISTER_WRITABLE_PROPERTY(QStringList, AutoCompleteList, QStringList(), false)

public:
    enum {
        DialogTypeInvalid = 0,
        DialogTypeAppModeCheck,
        DialogTypeAppModeRadio,
        DialogTypeSelectMoudleInfo,
        DialogTypeSelectLogFile,
        DialogTypeSelectValueEnumInput,
        DialogTypeSelectValueEnumOutput,
        DialogTypeSelectMatchingTableCV,
        DialogTypeSelectMatchingTablePV,
        DialogTypeSelectNegative,
        DialogTypeSelectVehiclePV,
        DialogTypeSelectVehicleCV,  // 10
        DialogTypeSelectVehicleType,
        DialogTypeEnterText,
        DialogTypeTestReportTC,
        DialogTypeTestReportGCOV,
        DialogTypeLogDisplay,
        DialogTypeViewLogInfo,
        DialogTypeViewLogFileInfo,
        DialogTypeAutoComplete,
    };

private:
    enum {
        DisplayTypeInvalid = 0,
        DisplayTypeAppMode,
        DisplayTypeAppModeRadio,
        DisplayTypeSelectList,
        DisplayTypeSelectOption,
        DisplayTypeEnterText,
        DisplayTypeTestReport,
        DisplayTypeLogDisplay,
        DisplayTypeViewLog,
        DisplayTypeAutoComplete,
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

    // protected:
    //     void showEvent(QShowEvent* event) override {
    //     }

private:
    void controlConnet(const int& displayType);
    void connectAppMode(const bool& state);
    void connectAppModeRadio(const bool& state);
    void connectSelectList(const bool& state);
    void connectSelectOption(const bool& state);
    void connectEnterText(const bool& state);
    void connectTestReport(const bool& state);
    void connectLogDisplay(const bool& state);
    void connectViewLog(const bool& state);
    void connectAutoComplete(const bool& state);

    QRect updateMainRect();
    void updateDisplay(const int& displayType, const QString& title);

    QList<QPair<QFrame*, QRadioButton*>> isRadioWidget() const;
    QList<QCheckBox*> isCheckWidget(const bool& option1) const;
    void updateSelectListCheckState(const bool& allCheck, const QStringList& selectList);
    void refreshViewLog(const int& refreshType);
    void updateAutoCompleteSuggestionsList(const QString& text);

    bool updateAppMode(const QVariantList& info);
    bool updateAppModeRadio(const QVariantList& info);
    bool updateSelectList(const QVariantList& info);
    bool updateSelectOption(const QVariantList& info);
    bool updateInputText(const QVariantList& info);
    bool updateTestReport(const QVariantList& info);
    bool updateLogDisplay(const QVariantList& info);
    bool updateViewLog(const QVariantList& info);
    bool updateAutoComplete(const QVariantList& info);

signals:
    void signalSelectAppMode(const int& appMode);
    void signalSelectListItem(const QList<QPair<int, QString>>& selectItem);
    void signalSelectOption(const bool& option1, const QList<QPair<QString, bool>>& option2);
    void signalScrollBarValueChanged(const int& value);
    void signalEnterTextChanged(const QString& text);
    void signalLogDisplayClicked(const bool& hide, const bool& detail);
    void signalViewLogClicked(const bool& close);
    void signalAutoCompleteSelected(const QString& text);

private:
    Ui::Dialog* mGui;
    QStandardItemModel mModel = QStandardItemModel();
};

#endif  // DIALOG_H
