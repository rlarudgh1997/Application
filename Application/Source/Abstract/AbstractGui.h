#ifndef ABSTRACT_GUI_H
#define ABSTRACT_GUI_H

#include <QWidget>


class AbstractHandler;

template <typename T1, typename T2>
inline T1* Q_CAST(T1, T2 base) {
    return qobject_cast<T1*>(base);
}

class AbstractGui : public QWidget {
public:
    explicit AbstractGui(AbstractHandler* handler) : mHandler(handler) {}
    AbstractHandler* isHandler() {
        return mHandler;
    }

private:
    virtual void initItem() = 0;
    virtual QWidget* isItem(const int& type) = 0;
    virtual bool createSignal(const int& type, const QVariant& value) = 0;
    // virtual void drawDisplay(const int& depth) = 0;
    virtual void drawDisplayDepth0() = 0;
    virtual void drawDisplayDepth1() = 0;
    virtual void drawDisplayDepth2() = 0;
    virtual void updateDisplay(const bool& first, const int& type = 0) = 0;

public slots:
    virtual void slotPropertyChanged(const int& type, const QVariant& value) = 0;

private:
    AbstractHandler* mHandler = nullptr;
};

#endif    // ABSTRACT_GUI_H
