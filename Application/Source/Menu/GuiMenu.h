#ifndef GUI_MENU_H
#define GUI_MENU_H

#include "AbstractGui.h"

#include <QWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QLineEdit>
#include <QTextEdit>
#include <QScrollBar>
#include <QTextCharFormat>
#include <QButtonGroup>
#include <QRadioButton>
#include <QShortcut>
#include <QInputDialog>

#include "CommonUtil.h"
#include "Dialog.h"

#include "ui_GuiMenu.h"

class GuiMenu : public AbstractGui {
    Q_OBJECT

    REGISTER_WRITABLE_VALUE(QVariantList, SelectModuleList, QVariantList())
    REGISTER_WRITABLE_VALUE(bool, TestResultComplted, false)
    REGISTER_WRITABLE_VALUE(QVariantList, ProgressInfo, QVariantList({0, 0}))

public:
    static QSharedPointer<GuiMenu>& instance(AbstractHandler* handler = nullptr);

private:
    explicit GuiMenu(AbstractHandler* handler = nullptr);

    virtual void drawDisplayDepth0();
    virtual void drawDisplayDepth1();
    virtual void drawDisplayDepth2();
    virtual void updateDisplaySize();
    virtual void updateDisplayVisible();

    void updateDrawDialog(const int& dialogType, const QVariantList& info);
    void updateDialogAppMode();
    void updateDialogSelectModule();
    void updateDialogSelectOption();
    void updateDialogTestResultInfo();
    void updateDialogEnterScriptText();
    void updateDialogTestReport();
    void updateDialogViewFileList(const int& type);
    void updateDialogViewLogInfo(const bool& show);
    void updateDialogViewFileInfo();

    void updateMenuFile();
    void updateMenuEdit();
    void updateMenuView();
    void updateMenuSetting();
    void updateMenuRun();
    void updateMenuHelp();
    void updateMenuEtc();
    void updateProgressBar(const bool& show, const QVariantList& progressInfo);

    void updateDisplayPath();

public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value);

private:
    Ui::GuiMenu* mGui = nullptr;
    QMainWindow* mMainView = nullptr;
    QSharedPointer<Dialog> mDialog = nullptr;
};

#endif  // GUI_MENU_H
