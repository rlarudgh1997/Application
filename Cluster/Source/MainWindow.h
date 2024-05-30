#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QQuickView>
#include <QSharedPointer>

#include "CommonDefine.h"
#if defined(__MODULE_SUB_WINDOW__)
#include "SubWindow.h"
#endif

class MainWindow : public QQuickView {
    Q_OBJECT

public:
    explicit MainWindow(QQuickView* parent = nullptr);
    ~MainWindow();

private:
    void controlConnect();
    void setContextProperty();
    void setQmlRegisterType();

protected:
    void mousePressEvent(QMouseEvent* mouseEvent) override;
    void mouseReleaseEvent(QMouseEvent* mouseEvent) override;
    void keyPressEvent(QKeyEvent* keyEvent) override;
    void keyReleaseEvent(QKeyEvent* keyEvent) override;
    void moveEvent(QMoveEvent* moveEvent) override;
    void resizeEvent(QResizeEvent* resizeEvent) override;
    void closeEvent(QCloseEvent* event) override;

private:
    QRect mScreenInfo = QRect();
#if defined(__MODULE_SUB_WINDOW__)
    SubWindow* mSubWindow = nullptr;
#endif
};

#endif  // MAIN_WINDOW_H
