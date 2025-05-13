#ifndef GUI_CENTER_H
#define GUI_CENTER_H

#include "AbstractGui.h"
#include "CommonUtil.h"

#include <QWidget>
#include <QTableWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QListView>
#include <QStringListModel>
#include <QButtonGroup>
#include <QRadioButton>
#include <QCheckBox>
#include <QCompleter>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QLayout>

#include "Dialog.h"
#include "ui_GuiCenter.h"

class GuiCenter : public AbstractGui {
    Q_OBJECT

    REGISTER_WRITABLE_VALUE(int, ScrolBarValue, 0)
    REGISTER_WRITABLE_VALUE(QStringList, NodeAddress, QStringList())
    REGISTER_WRITABLE_VALUE(bool, ConfigUpdating, false)

public:
    static QSharedPointer<GuiCenter>& instance(AbstractHandler* handler = nullptr);

private:
    explicit GuiCenter(AbstractHandler* handler = nullptr);

    virtual void drawDisplayDepth0();
    virtual void drawDisplayDepth1();
    virtual void drawDisplayDepth2();
    virtual void updateDisplaySize();
    virtual void updateDisplayVisible();

    void updateDrawDialog(const int& dialogType, const QVariantList& info);
    void updateDialogAutoComplete();
    void updateDialogSelectModule();

    void updateDisplayConfigInfo();
    void updateDisplayNodeAddress(const int& updateType);
    void updateDisplayTerminal();

public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value);

private:
    Ui::GuiCenter* mGui = nullptr;
    QStackedWidget* mMainView = nullptr;
    QSharedPointer<Dialog> mDialog = nullptr;
};

#endif  // GUI_CENTER_H
