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

    REGISTER_WRITABLE_PROPERTY(int, AppMode, 0, false)
    REGISTER_WRITABLE_PROPERTY(QRect, ScreenRect, QRect(), false)

public:
    enum {
        DialogTypeAppMode = 0,
        DialogTypeRadioButton,
        DialogTypeMoudleInfo,
    };

public:
    explicit Dialog(const QRect& rect, QWidget* parent = nullptr);
    ~Dialog();
    void updateAppMode(const int& appMode, const QStringList& appModeList);
    void updateAppModeRadio(const int& appMode, const QStringList& appModeList);

    // protected:
    //     void showEvent(QShowEvent* event) override {
    //     }

private:
    void controlConnet(const bool& state);
    void drawDialog(const int& dialogType, const QString& title);
    QList<QPair<QFrame*, QRadioButton*>> isRadioWidget() const;

signals:
    void signalSelectAppMode(const int& appMode);

private:
    Ui::Dialog* mGui;
    QStandardItemModel mModel = QStandardItemModel();
};

#endif  // DIALOG_H
