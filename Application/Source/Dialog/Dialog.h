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
    REGISTER_WRITABLE_PROPERTY(int, CheckModelIndex, (-1), false)
    REGISTER_WRITABLE_PROPERTY(bool, FindLog, false, false)
    REGISTER_WRITABLE_PROPERTY(bool, ViewLogStop, false, false)

public:
    enum {
        DialogTypeInvalid = 0,
        DialogTypeAppModeCheck,
        DialogTypeAppModeRadio,
        DialogTypeSelectMoudleInfo,
        DialogTypSelectLogFile,
        DialogTypeSelectNegative,
        DialogTypeSelectVehiclePV,
        DialogTypeSelectVehicleCV,
        DialogTypeEnterText,
        DialogTypeTestReportTC,
        DialogTypeTestReportGCOV,   // 10
        DialogTypeLogDisplay,
        DialogTypeViewLogInfo,
        DialogTypeViewLogFileInfo,
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

    void drawDialog(const int& type, const QVariantList& value);

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

    QRect updateMainRect();
    void updateDisplay(const int& displayType, const QString& title);

    QList<QPair<QFrame*, QRadioButton*>> isRadioWidget() const;
    QList<QCheckBox*> isCheckWidget(const bool& option1) const;

    void updateAppMode(const QString& title, const int& appMode, const QStringList& appModeList);
    void updateAppModeRadio(const QString& title, const int& appMode, const QStringList& appModeList);

    void updateSelectListCheckState(const bool& all);
    void updateSelectList(const QString& title, const QStringList& column, const QStringList& list, const bool& all = false);
    void updateSelectOption(const QString& title, const QString& option1, const QStringList& option2);

    void updateInputText(const QString& title);

    void updateTestReport(const QString& title, const bool& option1, const QStringList& option2Str,
                          const QVariantList& option2Value);

    void updateLogDisplay(const QString& title, const QString& titleInfo, const QString& errorInfo,
                          const QString& moduleStateInfo);

    void refreshViewLog(const int& refreshType);
    void updateViewLog(const QString& title, const QStringList& detailLog);

signals:
    void signalSelectAppMode(const int& appMode);
    void signalSelectListItem(const QList<QPair<int, QString>>& selectItem);
    void signalSelectOption(const bool& option1, const QList<QPair<QString, bool>>& option2);
    void signalEnterTextChanged(const QString& text);
    void signalLogDisplayClicked(const bool& hide, const bool& detail);
    void signalViewLogClicked(const bool& close);

private:
    Ui::Dialog* mGui;
    QStandardItemModel mModel = QStandardItemModel();
};

#endif  // DIALOG_H
