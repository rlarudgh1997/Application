#ifndef TEST_CASE_H
#define TEST_CASE_H

#include <QObject>

#include "CommonUtil.h"

class TestCase : public QObject {
    REGISTER_WRITABLE_PROPERTY(int, ExcuteType, 0, false)
    REGISTER_WRITABLE_PROPERTY(QStringList, ExcuteData, QStringList(), false)

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

private:
    explicit TestCase();

    void parsingExcuteOption(const QStringList& data);
};

#endif  // TEST_CASE_H
