#ifndef TEST_CASE_H
#define TEST_CASE_H

#include <QObject>

#include "CommonUtil.h"

class TestCase : public QObject {
    REGISTER_WRITABLE_PROPERTY(int, ExcuteType, 0, false)
    REGISTER_WRITABLE_PROPERTY(QStringList, ExcuteData, QStringList(), false)
    REGISTER_WRITABLE_PROPERTY(QStringList, ModuleList, QStringList(), false)
    REGISTER_WRITABLE_PROPERTY(bool, NextStep, false, false)

public:
    enum {
        ExcuteTypeInvalid = 0,
        ExcuteTypeGenConvertData,
        ExcuteTypeGenTC,
        ExcuteTypeRunTC,
    };

public:
    static QSharedPointer<TestCase>& instance();

    void excuteTestCase(const int& type, const QStringList& data = QStringList());
    void inputOption(const QStringList& args);

private:
    explicit TestCase();

    bool parsingExcuteOption(const bool& graphicsMode, const QStringList& data);
    QString selectOptionWithNumbers(const QString& prompt, const QStringList& options);
};

#endif  // TEST_CASE_H
