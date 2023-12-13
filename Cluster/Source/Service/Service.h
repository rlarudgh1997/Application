#ifndef SERIVCE_H
#define SERIVCE_H

#include <QObject>
#include <QSharedPointer>
#include <QVariant>

#include <HSubscription.h>
#include <HVehicleSignalModel.h>
#include <vsm.h>
#include <functional>
#include "SFCHelper.h"

namespace ccos::vehicle::vsm {
class HSubscription;
class HVehicleSignal;
class HVehicleSignalModel;
}  // namespace ccos::vehicle::vsm

// using HVehicleSignalList = std::vector<ccos::vehicle::vsm::HVehicleSignal>;
using ccos::vehicle::vsm::HSubscription;
using ccos::vehicle::vsm::HVehicleSignal;
using ccos::vehicle::vsm::HVehicleSignalModel;
using ccos::vehicle::vsm::HSubscriptionType;
using ccos::vehicle::vsm::IHSubscriptionListener;

typedef std::function<void(const std::vector<HVehicleSignal>&)> SignalHandlingFunc;

class VehicleListener : public IHSubscriptionListener {
public:
    explicit VehicleListener(const SignalHandlingFunc& handlingFunc) {
        mHandlingFunc = handlingFunc;
    }
    void onVehicleSignalReceived(const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList) {
        mHandlingFunc(signalList);
    }
    void onVehicleSignalTimeout(const ccos::vehicle::vsm::HVehicleSignal& signal) {
    }

private:
    SignalHandlingFunc mHandlingFunc;
};

class Service : public QObject {
    Q_OBJECT

public:
    enum Constant {
        SpeedAnalogStat = 0,
        SpeedAnalogValue,
        SpeedDigitalStat,
        SpeedDigitalValue,
        SpeedSubDigitalStat,
        SpeedSubDigitalValue,
        SpeedMainDisplayUnitStat,
        SpeedAuxDisplayUnitStat,
        SpeedSubDisplayStat,
        SpeedScaleMaximumStat,
        NaviSpeedLimitStat,
        NaviSpeedLimitOver1ColorValue,
        NaviSpeedLimitOver2ColorValue,
    };
    enum Etc {
        InterDisplaySpeedUnit = 0,
        InterDisplaySpeedValueKPH,
        InterDisplaySpeedValueMPH,
    };


public:
    static QSharedPointer<Service>& instance();

private:
    explicit Service();

    HVehicleSignalModel& getVehicleSignalModel();

    void init();
    void addSubscription(const std::string& nodeAddress, const SignalHandlingFunc& handlingFunc);
    void addSubscriptions(const std::vector<std::string>& nodePaths, const SignalHandlingFunc& handlingFunc);

    void onConstantChanged(const int& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList);
    void subscribeConstant();

    void onTelltaleChanged(const int& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList);
    void subscribeTelltale();

    void onEventChanged(const int& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList);
    void subscribeEvent();

    void onSoundChanged(const int& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList);
    void subscribeSound();

    void onEtcChanged(const int& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList);
    void subscribeEtc();

signals:
    void signalServiceConstantChanged(const int& signalType, const QVariant& signalValue);
    void signalServiceTelltaleChanged(const int& signalType, const QVariant& signalValue);
    void signalServiceEventChanged(const int& signalType, const QVariant& signalValue);
    void signalServiceSoundChanged(const int& signalType, const QVariant& signalValue);
    void signalServiceEtcChanged(const int& signalType, const QVariant& signalValue);


private:
    // HVehicleSignalModel* mVehicleSignalModel = nullptr;
};

#endif  // SERIVCE_H
