#ifndef SUB_WINDOW_ENUM_H
#define SUB_WINDOW_ENUM_H

#include <QObject>

class SubWindowEnum : public QObject {
    Q_OBJECT

public:
    enum DisplayType {
        DisplayTypeInvalid = 0,
        DisplayTypeListMain,
        DisplayTypeListSub,
        DisplayTypeListAltonService,
        DisplayTypeListHmi,
        DisplayTypeETC,
    };
    enum ListType {
        ListTypeNormal = 0,
        ListTypeCheck,
        ListTypeUpdateCheck,
    };
    enum ViewType {
        ViewTypeInvalid = 0,
        ViewTypeTAV,
        ViewTypeScript,
        ViewTypeRedrawTAV,
    };
    enum StartScriptType {
        StartScriptTypeInvalid = 0,
        StartScriptTypeStart,
        StartScriptTypeStop,
        StartScriptTypeMenuStart,
        StartScriptTypeMenuStop,
    };
    enum DetailInfo {
        DetailInfoInvalid = 0,
        DetailInfoDescription,
        DetailInfoPowerTrain,
        DetailInfoPrecondition,
        DetailInfoListen,
        DetailInfoStep,
        DetailInfoExpectedResult,
        DetailInfoMax,
    };
    enum DetailSubInfo {
        DetailSubInfoNormal,
        DetailSubInfoGroup,
        DetailSubInfoPeriod,
        DetailSubInfoPeriodGroup,
    };
    enum PathType {
        PathTypeVSM,
        PathTypeTAV,
        PathTypeAltonClient,
    };
    enum DeleteType {
        DeleteTypeSelectTAV,
        DeleteTypeTAV,
        DeleteTypeScript,
        DeleteTypeInfo,
    };
};

#endif  // SUB_WINDOW_ENUM_H
