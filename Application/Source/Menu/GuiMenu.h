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

    void drawMenuFile();
    void drawMenuEdit();
    void drawMenuView();
    void drawMenuSetting();
    void drawMenuRun();
    void drawMenuDocker();
    void drawMenuHelp();
    void drawMenuEtc();
    void updateDrawDialog(const int& dialogType, const QVariantList& info);
    void updateDisplaySelectModule();
    void updateDisplaySelectOption();
    void updateDisplayPath();
    void updateDisplayProgressBar(const bool& show, const QVariantList& progressInfo);
    void updateDisplayTestResultInfo();
    void updateDisplayEnterScriptText();
    void updateDisplayTestReport();
    void updateDisplayViewLogFileList();
    void updateDisplayViewLogInfo(const bool& show);
    void updateDisplayViewLogFileInfo();
    void updateDisplayAppMode();

public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value);

private:
    Ui::GuiMenu* mGui = nullptr;
    QMainWindow* mMainView = nullptr;
    QSharedPointer<Dialog> mDialog = nullptr;
};

#endif  // GUI_MENU_H
