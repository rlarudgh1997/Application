#ifndef SERIVCE_H
#define SERIVCE_H

#include <QObject>
#include <QSharedPointer>


#include <HSubscription.h>
#include <HVehicleSignalModel.h>
#include <listener/ClusterSignalListener.h>
#include <vsm.h>

#include "SFCHelper.h"


namespace ccos::vehicle::vsm {
class HSubscription;
class HVehicleSignal;
class HVehicleSignalModel;
}  // namespace ccos::vehicle::vsm

using HVehicleSignalList = std::vector<ccos::vehicle::vsm::HVehicleSignal>;
using ccos::vehicle::vsm::HSubscription;
using ccos::vehicle::vsm::HVehicleSignal;
using ccos::vehicle::vsm::HVehicleSignalModel;
using ccos::vehicle::vsm::HSubscriptionType;

typedef std::function<void(const std::vector<HVehicleSignal>&)> SignalHandlingFunc;


class Service : public QObject {
    Q_OBJECT

public:
    static QSharedPointer<Service>& instance();

private:
    explicit Service();

    void init();
    HVehicleSignalModel* getVehicleSignalModel() const;
    void addSubscription(const std::string& nodeAddress, const SignalHandlingFunc& handlingFunc);
    void addSubscriptions(const std::vector<std::string>& nodePaths, const SignalHandlingFunc& handlingFunc);
    void onGaugeChanged(const int& signalType, const HVehicleSignalList& signalList);
    void subscribeGauge();
    void subscribeTelltale();
    void subscribeEvent();
    void subscribeContents();
    void subscribeMainView();


private:
    HVehicleSignalModel* mVehicleSignalModel = nullptr;
};

#endif  // SERIVCE_H
