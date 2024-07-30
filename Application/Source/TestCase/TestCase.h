#ifndef TEST_CASE_H
#define TEST_CASE_H

#include <QObject>

#include "CommonUtil.h"

class TestCase : public QObject {
    REGISTER_WRITABLE_PROPERTY(int, ExcuteType, 0, false)

public:
    enum {
        ExcuteTypeInvalid = 0,
        ExcuteTypeGenTC,
        ExcuteTypeRunTC,
    };

public:
    static QSharedPointer<TestCase>& instance();

    void excuteTestCase(const int& type = ExcuteTypeGenTC);

private:
    explicit TestCase();
};

#endif  // TEST_CASE_H
