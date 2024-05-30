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

    void subscribeConstantSignals();
    void subscribeTelltaleSignals();
    void subscribeEventSignals();
    void subscribeSoundSignals();
    void subscribeEtcSignals();

    template <typename TYPE>
    void subscribeSignals(const DataType& datatType,
                          const std::vector<std::pair<TYPE, std::vector<std::string>>>& signalsToSubscribe);
    template <typename TYPE>
    QVariant isSignalValue(const DataType& dataType, const TYPE& signalType,
                           const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal
                           QHash<int, QPair<QString, QVariant>>& values);
    template <typename TYPE>
    void onSignalChanged(const DataType& dataType, const TYPE& signalType,
                         const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList);
#if 0
    QVariant isConstantSignal(const Constant& signalType, const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal,
                              QHash<QString, QVariant>& values);
    QVariant isTelltaleSignal(const Telltale& signalType, const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal,
                              QHash<QString, QVariant>& values);
    QVariant isEventSignal(const Event& signalType, const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal,
                           QHash<QString, QVariant>& values);
    QVariant isSoundSignal(const Sound& signalType, const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal,
                           QHash<QString, QVariant>& values);
    QVariant isEtcSignal(const Etc& signalType, const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal,
                         QHash<QString, QVariant>& values);
#endif

signals:
    void signalServiceDataChanged(const int& dataType, const int& signalType, const QVariant& signalValue);
    void signalServiceDatasChanged(const int& dataType, const int& signalType,
                                   const QHash<int, QPair<QString, QVariant>>& signalValues);
};
#endif  // SERIVCE_H
