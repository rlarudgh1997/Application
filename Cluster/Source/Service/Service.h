#ifndef SERIVCE_H
#define SERIVCE_H

#include <QObject>
#include <QSharedPointer>
#include <QVariant>

#include <HVehicleSignalModel.h>
#include <vsm.h>
#include "SFCHelper.h"
#include "ServiceEnum.h"

// #define USE_SERVCIE_DATA_TEST

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

    static QSharedPointer<ccos::vehicle::vsm::HVehicleSignalModel>& getVehicleSignalModel();
#if 0
    void addSubscription(const std::string& nodeAddress, const SignalHandlingFunc& handlingFunc);
#endif
    void addSubscriptionGroup(const std::vector<std::string>& nodePaths, const SignalHandlingFunc& handlingFunc);

    void subscribeConstantSignals();
    void subscribeTelltaleSignals();
    void subscribeEventSignals();
    void subscribeSoundSignals();
    void subscribeEtcSignals();

    template <typename TYPE>
    void subscribeSignals(const DataType& datatType,
                          const std::vector<std::pair<TYPE, std::vector<std::string>>>& signalsToSubscribe);
    QVariant isSignalValue(const ccos::vehicle::vsm::HVehicleSignal& vehicleSignal, QHash<QString, QVariant>& values);
    template <typename TYPE>
    void onSignalChanged(const DataType& dataType, const TYPE& signalType,
                         const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList);

signals:
    void signalServiceDataChanged(const int& dataType, const int& signalType, const QVariant& signalValue);
    void signalServiceDatasChanged(const int& dataType, const int& signalType, const QHash<QString, QVariant>& signalValues);

#if defined(USE_SERVCIE_DATA_TEST)
private slots:
    void slotServiceDataChanged(const int& dataType, const int& signalType, const QVariant& signalValue);
    void slotServiceDatasChanged(const int& dataType, const int& signalType, const QHash<QString, QVariant>& signalValues);
#endif
};
#endif  // SERIVCE_H
