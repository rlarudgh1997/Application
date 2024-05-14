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
    REGISTER_WRITABLE_PROPERTY(int, DisplayType, 0, false)
    REGISTER_WRITABLE_PROPERTY(int, AppMode, 0, false)
    REGISTER_WRITABLE_PROPERTY(QRect, ScreenRect, QRect(), false)
    REGISTER_WRITABLE_PROPERTY(bool, SelectAll, false, false)

public:
    enum {
        DialogTypeInvalid = 0,
        DialogTypeAppModeCheck,
        DialogTypeAppModeRadio,
        DialogTypeMoudleInfo,
        DialogTypeViewLogFile,
        DialogTypeSelectNegative,
        DialogTypeSelectVehiclePV,
        DialogTypeSelectVehicleCV,
        DialogTypeEnterText,
        DialogTypeTestReportTC,
        DialogTypeTestReportGCOV,   // 10
        DialogTypeLogDisplay,
        DialogTypeDetialLog,
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
        DisplayTypeDetialLog,
        DisplayTypeMax,
    };

public:
    explicit Dialog(const QRect& rect, QWidget* parent = nullptr);
    ~Dialog();

    void drawDialog(const int& type, const QVariantList& value);

    // protected:
    //     void showEvent(QShowEvent* event) override {
    //     }

private:
    void controlConnet(const int& dialogType);
    QRect updateMainRect();
    void updateDisplay(const int& displayType, const QString& title);

    QList<QPair<QFrame*, QRadioButton*>> isRadioWidget() const;
    QList<QCheckBox*> isCheckWidget(const bool& option1) const;

    void updateAppMode(const QString& title, const int& appMode, const QStringList& appModeList);
    void updateAppModeRadio(const QString& title, const int& appMode, const QStringList& appModeList);

    void updateSelectListCheckState(const bool& all);
    void updateSelectList(const int& dialogType, const QString& title, const QStringList& column, const QStringList& list,
                          const bool& all = false);

    void updateSelectOption(const int& dialogType, const QString& title, const QString& option1, const QStringList& option2);
    void updateInputText(const QString& title);
    void updateTestReport(const int& dialogType, const QString& title, const bool& option1, const QStringList& option2Str,
                          const QVariantList& option2Value);
    void updateLogDisplay(const QString& title, const QString& titleInfo, const QString& errorInfo,
                          const QString& moduleStateInfo);
    void updateDetailLog(const QString& title, const QStringList& detailLog);

signals:
    void signalSelectAppMode(const int& appMode);
    void signalSelectListItem(const QList<QPair<int, QString>>& selectItem);
    void signalSelectOption(const bool& option1, const QList<QPair<QString, bool>>& option2);
    void signalEnterTextChanged(const QString& text);
    void signalLogDisplayClicked(const bool& hide, const bool& detail);

private:
    Ui::Dialog* mGui;
    QStandardItemModel mModel = QStandardItemModel();
};

#endif  // DIALOG_H
