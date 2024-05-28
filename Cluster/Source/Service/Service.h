#ifndef SERIVCE_H
#define SERIVCE_H

#include <QObject>
#include <QSharedPointer>
#include <QVariant>

#include <HVehicleSignalModel.h>
#include <vsm.h>
#include "SFCHelper.h"
#include "ServiceEnum.h"

using SignalHandlingFunc = std::function<void(const std::vector<ccos::vehicle::vsm::HVehicleSignal>&)>;
using NodePathToValueFunc = std::function<QVariant(const ccos::vehicle::vsm::HVehicleSignal&)>;

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
    void addSubscriptionGroup(const std::vector<std::string>& nodePaths, const SignalHandlingFunc& handlingFunc);
    QVariant processVehicleSignal(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal,
                                  const std::map<std::string, NodePathToValueFunc>& nodePathToValueFuncs,
                                  QHash<QString, QVariant>& values);

    // Constant
    QVariant isConstantSpeedGauge(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal, QHash<QString, QVariant>& values);
    QVariant isConstantTachometer(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal, QHash<QString, QVariant>& values);
    QVariant isConstantIntroOutro(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal, QHash<QString, QVariant>& values);
    QVariant isConstantOAT(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal, QHash<QString, QVariant>& values);
    QVariant isConstantTransmissionIndicator(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal,
                                             QHash<QString, QVariant>& values);
    QVariant isConstantDriveMode(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal, QHash<QString, QVariant>& values);
    QVariant isConstantADASParkingNew(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal, QHash<QString, QVariant>& values);
    QVariant isConstantHighPerformanceForNBrand(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal,
                                                QHash<QString, QVariant>& values);
    QVariant isConstantADASDrivingNew(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal, QHash<QString, QVariant>& values);
    void onConstantChanged(const Constant& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList);
    void subscribeConstantSpeedGauge();
    void subscribeConstantTachometer();
    void subscribeConstantIntroOutro();
    void subscribeConstantOAT();
    void subscribeConstant();

    // Telltale
    QVariant isTelltaleLampIndicator(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal, QHash<QString, QVariant>& values);
    QVariant isTelltaleOAT(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal, QHash<QString, QVariant>& values);
    QVariant isTelltaleDriveMode(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal, QHash<QString, QVariant>& values);
    QVariant isTelltaleHighPerformanceForNBrand(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal,
                                                QHash<QString, QVariant>& values);
    QVariant isTelltaleADASDrivingNew(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal, QHash<QString, QVariant>& values);
    QVariant isTelltaleHighPerformance(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal, QHash<QString, QVariant>& values);
    void onTelltaleChanged(const Telltale& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList);
    void subscribeTelltaleLampIndicator();
    void subscribeTelltaleOAT();
    void subscribeTelltaleDriveMode();
    void subscribeTelltaleHighPerformanceForNBrand();
    void subscribeTelltaleADASDrivingNew();
    void subscribeTelltaleHighPerformance();
    void subscribeTelltale();

    // Event
    QVariant isEventIntroOutro(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal, QHash<QString, QVariant>& values);
    QVariant isEventOAT(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal, QHash<QString, QVariant>& values);
    QVariant isEventTransmissionIndicator(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal,
                                          QHash<QString, QVariant>& values);
    QVariant isEventDriveMode(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal, QHash<QString, QVariant>& values);
    QVariant isEventADASParkingNew(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal, QHash<QString, QVariant>& values);
    QVariant isEventHighPerformanceForNBrand(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal,
                                             QHash<QString, QVariant>& values);
    QVariant isEventADASDrivingNew(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal, QHash<QString, QVariant>& values);
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
    QVariant isSoundLampIndicator(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal, QHash<QString, QVariant>& values);
    QVariant isSoundIntroOutro(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal, QHash<QString, QVariant>& values);
    QVariant isSoundTransmissionIndicator(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal,
                                          QHash<QString, QVariant>& values);
    QVariant isSoundADASParkingNew(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal, QHash<QString, QVariant>& values);
    QVariant isSoundHighPerformanceForNBrand(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal,
                                             QHash<QString, QVariant>& values);
    QVariant isSoundADASDrivingNew(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal, QHash<QString, QVariant>& values);
    void onSoundChanged(const Sound& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList);
    void subscribeSoundLampIndicator();
    void subscribeSoundIntroOutro();
    void subscribeSoundTransmissionIndicator();
    void subscribeSoundADASParkingNew();
    void subscribeSoundHighPerformanceForNBrand();
    void subscribeSoundADASDrivingNew();
    void subscribeSound();

    // Etc
    QVariant isEtcSpeedGauge(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal, QHash<QString, QVariant>& values);
    QVariant isEtcDriveMode(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal, QHash<QString, QVariant>& values);
    QVariant isEtcADASDrivingNew(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal, QHash<QString, QVariant>& values);
    void onEtcChanged(const Etc& signalType, const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList);
    void subscribeEtcSpeedGauge();
    void subscribeEtcDriveMode();
    void subscribeEtcADASDrivingNew();
    void subscribeEtc();

signals:
    void signalServiceDataChanged(const int& dataType, const int& signalType, const QVariant& signalValue);
    void signalServiceDatasChanged(const int& dataType, const int& signalType, const QHash<QString, QVariant>& signalValues);
};

#endif  // SERIVCE_H
