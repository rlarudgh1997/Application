#ifndef SERIVCE_H
#define SERIVCE_H

#include <QObject>
#include <QSharedPointer>
#include <QVariant>

#include <HVehicleSignalModel.h>
#include <vsm.h>
#include "SFCHelper.h"
#include "ServiceEnum.h"

typedef std::function<void(const std::vector<ccos::vehicle::vsm::HVehicleSignal>&)> SignalHandlingFunc;

class VehicleListener : public ccos::vehicle::vsm::IHSubscriptionListener {
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
    static QSharedPointer<Service>& instance();
    void init();

private:
    explicit Service();

    ccos::vehicle::vsm::HVehicleSignalModel& getVehicleSignalModel();

    void addSubscription(const std::string& nodeAddress, const SignalHandlingFunc& handlingFunc);
    void addSubscriptions(const std::vector<std::string>& nodePaths, const SignalHandlingFunc& handlingFunc);

    // Constant
    QVariant isConstantSpeedGauge(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    QVariant isConstantTachometer(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    QVariant isConstantIntroOutro(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    QVariant isConstantOAT(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    QVariant isConstantTransmissionIndicator(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    QVariant isConstantDriveMode(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    QVariant isConstantADASParkingNew(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    QVariant isConstantHighPerformanceForNBrand(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    QVariant isConstantADASDrivingNew(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    void onConstantChanged(const Constant& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList);
    void subscribeConstantSpeedGauge();
    void subscribeConstantTachometer();
    void subscribeConstantIntroOutron();
    void subscribeConstant();

    // Telltale
    QVariant isTelltaleLampIndicator(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    QVariant isTelltaleOAT(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    QVariant isTelltaleDriveMode(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    QVariant isTelltaleHighPerformanceForNBrand(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    QVariant isTelltaleADASDrivingNew(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    void onTelltaleChanged(const Telltale& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList);
    void subscribeTelltaleLampIndicator();
    void subscribeTelltaleOAT();
    void subscribeTelltaleDriveMode();
    void subscribeTelltaleHighPerformanceForNBrand();
    void subscribeTelltaleADASDrivingNew();
    void subscribeTelltale();

    // Event
    QVariant isEventIntroOutro(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    QVariant isEventOAT(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    QVariant isEventTransmissionIndicator(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    QVariant isEventDriveMode(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    QVariant isEventADASParkingNew(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    QVariant isEventHighPerformanceForNBrand(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    QVariant isEventADASDrivingNew(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    void onEventChanged(const Event& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList);
    void subscribeEventIntroOutro();
    void subscribeEventOAT();
    void subscribeEventTransmissionIndicator();
    void subscribeEventDriveMode();
    void subscribeEventADASParking_New();
    void subscribeEventHighPerformanceForNBrand();
    void subscribeEventADASDrivingNew();
    void subscribeEvent();

    // Sount
    QVariant isSoundLampIndicator(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    QVariant isSoundIntroOutro(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    QVariant isSoundTransmissionIndicator(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    QVariant isSoundADASParkingNew(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    QVariant isSoundHighPerformanceForNBrand(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    QVariant isSoundADASDrivingNew(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    void onSoundChanged(const Sound& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList);
    void subscribeSoundLampIndicator();
    void subscribeSoundIntroOutro();
    void subscribeSoundTransmissionIndicator();
    void subscribeSoundADASParkingNew();
    void subscribeSoundHighPerformanceForNBrand();
    void subscribeSoundADASDrivingNew();
    void subscribeSound();

    // Etc
    QVariant isEtcSpeedGauge(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    QVariant isEtcDriveMode(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    QVariant isEtcADASDrivingNew(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal);
    void onEtcChanged(const Etc& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList);
    void subscribeEtcSpeedGauge();
    void subscribeEtcDriveMode();
    void subscribeEtcADASDrivingNew();
    void subscribeEtc();

signals:
    void signalServiceConstantChanged(const int& signalType, const QVariant& signalValue);
    void signalServiceTelltaleChanged(const int& signalType, const QVariant& signalValue);
    void signalServiceEventChanged(const int& signalType, const QVariant& signalValue);
    void signalServiceSoundChanged(const int& signalType, const QVariant& signalValue);
    void signalServiceEtcChanged(const int& signalType, const QVariant& signalValue);
    void signalServiceDataChanged(const int& dataType, const int& signalType, const QVariant& signalValue);
};

#endif  // SERIVCE_H
