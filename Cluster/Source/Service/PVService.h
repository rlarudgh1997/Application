#ifndef PVSERVICE_H
#define PVSERVICE_H

#include <QObject>

#ifndef X86_64
#include <HSubscription.h>
#include <HVehicleSignalModel.h>
#include <listener/ClusterSignalListener.h>
#endif

#include "AppService.h"
#include "common/CommonEnum.h"

#ifndef X86_64
#include <vsm.h>
using ccos::vehicle::vsm::HSubscriptionType;
#endif

namespace ccos::vehicle::vsm {
class HSubscription;
class HVehicleSignal;
class HVehicleSignalModel;
}  // namespace ccos::vehicle::vsm

using HVehicleSignalList = std::vector<ccos::vehicle::vsm::HVehicleSignal>;
using ccos::vehicle::vsm::HSubscription;
using ccos::vehicle::vsm::HVehicleSignal;
using ccos::vehicle::vsm::HVehicleSignalModel;
typedef std::function<void(const std::vector<HVehicleSignal>&)> SignalHandlingFunc;

class PVService : public QObject {
    Q_OBJECT
public:
    explicit PVService(QObject* parent = Q_NULLPTR);
    virtual ~PVService();

    void addSubscription(const std::string& nodeAddress, const SignalHandlingFunc& handlingFunc);
    void addSubscriptions(const std::vector<std::string>& nodePaths, const SignalHandlingFunc& handlingFunc);

    void setAppService(AppService* appservice);

private:
    void subscribeToCluster();
    void subscribeTelltale();
    void subscribeEvent();

    // Telltale
    void onTelltaleEvPwrDnStatChanged(const HVehicleSignalList& signalList);
    void onTelltalexEVsrvLmpEvStatChanged(const HVehicleSignalList& signalList);
    void onTelltalexEVsrvLmpHevStatChanged(const HVehicleSignalList& signalList);
    void onTelltalexEVsrvLmpFcevStatChanged(const HVehicleSignalList& signalList);

private:
    AppService* mAppService = nullptr;
};

#endif  // PVSERVICE_H
