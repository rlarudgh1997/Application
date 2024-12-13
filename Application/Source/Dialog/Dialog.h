#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QFrame>
#include <QRadioButton>
#include <QListWidget>

#include "CommonUtil.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog {
    Q_OBJECT

    REGISTER_WRITABLE_PROPERTY_CONTAINER(QHash, int, QVariant, Property, false)

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
        DataTypeScreenRect,
        DataTypeSelectAll,
        DataTypeMultiCheck,
        DataTypeCheckModelIndex,
        DataTypeViewLogStop,
        DataTypeAutoCompleteListInput,
        DataTypeAutoCompleteListKeyword,
        DataTypeAutoCompleteListNormal,
        DataTypeAutoCompleteListSfc,
        DataTypeAutoCompleteListVehicle,
        DataTypeAutoCompleteListTCName,
    };
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
        DialogTypeAutoCompleteNormal,
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
        DisplayTypeAutoCompleteNormal,
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
    void connectAutoCompleteNormal(const bool& state);

    QVariant getData(const int& type);
    void setData(const int& type, const QVariant& value);

    QRect updateMainRect();
    void updateDisplay(const int& displayType, const QString& title);

    QList<QPair<QFrame*, QRadioButton*>> isRadioWidget() const;
    QList<QCheckBox*> isCheckWidget(const bool& option1) const;
    void updateSelectListCheckState(const bool& allCheck, const QStringList& selectList);
    void refreshViewLog(const int& refreshType);
    void updateAutoCompleteSuggestionsList(const bool& normal, const bool& sfc, const bool& vehicle, const bool& tcName);

    bool updateAppMode(const QVariantList& info);
    bool updateAppModeRadio(const QVariantList& info);
    bool updateSelectList(const QVariantList& info);
    bool updateSelectOption(const QVariantList& info);
    bool updateInputText(const QVariantList& info);
    bool updateTestReport(const QVariantList& info);
    bool updateLogDisplay(const QVariantList& info);
    bool updateViewLog(const QVariantList& info);
    bool updateAutoComplete(const QVariantList& info);
    bool updateAutoCompleteNormal(const QVariantList& info);

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
    QMap<int, QVariant> mData = QMap<int, QVariant>();
    QStandardItemModel mModel = QStandardItemModel();
    QListWidget* mAutoCompleteListWidget = nullptr;
};

#endif  // DIALOG_H
