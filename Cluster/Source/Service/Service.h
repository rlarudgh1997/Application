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
        // SpeedGauge
        ConstantSpeedGaugeStart = 0,
        SpeedAnalogStat,
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
        ConstantSpeedGaugeEnd,

        // Tachometer
        ConstantTachometerStart,
        RpmValue,
        RedZoneExceptNbrandStat,
        RedZoneNbrandStat,
        MaxRpmStat,
        RpmDampStat,
        ConstantTachometerEnd,

        // IntroOutro
        ConstantIntroOutroStart,
        ConstantIntroOutroEnd,
    };
    enum Telltale {
        // LampIndicator
        LampIndicatorStart = 0,
        LampIndicatorEnd,
    };
    enum Telltale {
        // SpeedGauge
        EtcSpeedGaugeStart = 0,
        InterDisplaySpeedUnit,
        InterDisplaySpeedValueKPH,
        InterDisplaySpeedValueMPH,
        EtcSpeedGaugeEnd,
    };
    enum Etc {
        // SpeedGauge
        EtcSpeedGaugeStart = 0,
        InterDisplaySpeedUnit,
        InterDisplaySpeedValueKPH,
        InterDisplaySpeedValueMPH,
        EtcSpeedGaugeEnd,
    };


public:
    static QSharedPointer<Service>& instance();

private:
    explicit Service();

    HVehicleSignalModel& getVehicleSignalModel();

    void init();
    void addSubscription(const std::string& nodeAddress, const SignalHandlingFunc& handlingFunc);
    void addSubscriptions(const std::vector<std::string>& nodePaths, const SignalHandlingFunc& handlingFunc);

    // Constant
    QVariant isConstantSpeedGauge(const int& signalType, const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    QVariant isConstantTachometer(const int& signalType, const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    QVariant isConstantIntroOutro(const int& signalType, const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    void onConstantChanged(const int& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList);
    void subscribeConstantSpeedGauge();
    void subscribeConstantTachometer();
    void subscribeConstantIntroOutron();
    void subscribeConstant();

    // Telltale
    QVariant isTelltaleLampIndicator(const int& signalType, const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    void onTelltaleChanged(const int& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList);
    void subscribeTelltaleLampIndicator();
    void subscribeTelltale();

    // Event
    void onEventChanged(const int& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList);
    void subscribeEvent();

    // Sount
    void onSoundChanged(const int& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList);
    void subscribeSoundLampIndicator();
    void subscribeSound();

    // Etc
    QVariant isEtcSpeedGauge(const int& signalType, const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    void onEtcChanged(const int& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList);
    void subscribeEtcSpeedGauge();
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
