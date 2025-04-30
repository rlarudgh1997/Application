/**
 * Copyright 2020 IVIS Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software shall be licensed only pursuant to the terms
 * of an applicable IVIS license agreement.
 */

#ifndef SFCHELPER_H_
#define SFCHELPER_H_

#include <unordered_map>
#include <HSubscription.h>
#include "Node.hpp"

namespace sfc {

bool registerSFCAddressSet(Node& topNode);
std::vector<NodeAddress> searchAddress(const NodeAddress& addressInRegExp);
std::map<std::string /*SFCName*/, std::vector<NodeAddress>> divideBySFCFunction(const std::vector<NodeAddress>& addresses);

using VehicleSignalMapCallback = std::function<void(const std::map<NodeAddress, ccos::vehicle::vsm::HVehicleSignal>&)>;
using VehicleSignalVectorCallback = std::function<void(const std::vector<ccos::vehicle::vsm::HVehicleSignal>&)>;
static constexpr char kUnknownPriority[] = "Z-";

/* Subscription 을 간편하게 할 수 있도록 해주는 helper class
 * HVehicleSignal 을 vector 또는 map 으로 선택적으로 수신할 수 있다.
 * 생성자로 std::function 을 입력하게 되어 있기 때문에 class member function 을 bind 하거나 lambda function 을 사용할 수 있다.
 * example - bind)
 *    HVehicleSignalMode hVehicleSignalModel;
 *    hVehicleSignalModel.subscribe(
 *        std::make_shared<HSubscription>(addressList, HSubscriptionType::VALUE_CHANGED,
 *                                        std::make_shared<SubscriptionListener>(std::bind(...)));
 * example - lambda)
 *    hVehicleSignalModel.subscribe(
 *        std::make_shared<HSubscription>(addressList, HSubscriptionType::VALUE_CHANGED,
 *                                        std::make_shared<SubscriptionListener>(
 *                                            [](const std::map<NodeAddress, HVehicleSignal>& vehicleSignalMap){}));
 */
class SubscriptionListener : public ccos::vehicle::vsm::IHSubscriptionListener {
public:
    explicit SubscriptionListener(VehicleSignalMapCallback callback);
    explicit SubscriptionListener(VehicleSignalVectorCallback callback);

private:
    void onVehicleSignalReceived(const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList) override;

    VehicleSignalMapCallback mMapCallback = nullptr;
    VehicleSignalVectorCallback mVectorCallback = nullptr;
};

/* Event 를 Group 화 하여 Subscription 을 간편하게 할 수 있도록 해주는 helper class
 * 기본적인 사용법은 SubscriptionListener 와 동일하나 여러 event 를 group 화 하여 subscribe 하고
 * group 별로 수신된 event 를 공통 node address 로 매핑하여 callback 이 호출 된다.
 * example)
 *    HVehicleSignalMode hVehicleSignalModel;
 *    std::vector<NodeAddress> addressList = searchAddress("SFC.*.Event.*");
 *    hVehicleSignalModel.subscribe(
 *        std::make_shared<HSubscription>(addressList, HSubscriptionType::VALUE_CHANGED,
 *                                        std::make_shared<SubscriptionListener>(std::bind(...)));
 *
 * 위와 같이 사용 했을 때 아래와 같이 여러 group 의 event 들은 각각 common 한 node address 로 수신이 된다.
 * example1)
 * SFC.Event.ID                      <- SFC.ACTIVE_AIR_FLAP.Event.ACTIVE_AIR_FLAP.ID
 * SFC.Event.Stat                    <- SFC.ACTIVE_AIR_FLAP.Event.ACTIVE_AIR_FLAP.Stat
 * SFC.Event.LinkedSound.ID          <- SFC.ACTIVE_AIR_FLAP.Event.ACTIVE_AIR_FLAP.LinkedSound.ID
 * SFC.EVent.LinkedSound.Type        <- SFC.ACTIVE_AIR_FLAP.Event.ACTIVE_AIR_FLAP.LinkedSound.Type
 * SFC.Event.LinkedSound.RepeatCount <- SFC.ACTIVE_AIR_FLAP.Event.ACTIVE_AIR_FLAP.LinkedSound.RepeatCount
 * SFC.Event.LinkedSound.BlinkOnDuration <- SFC.ACTIVE_AIR_FLAP.Event.ACTIVE_AIR_FLAP.LinkedSound.BlinkOnDuration
 * SFC.Event.LinkedSound.BlinkOffDuration <- SFC.ACTIVE_AIR_FLAP.Event.ACTIVE_AIR_FLAP.LinkedSound.BlinkOffDuration
 *
 * example2)
 * SFC.Event.ID                      <- SFC.ABS_ESC.Event.Escoff.ID
 * SFC.Event.Stat                    <- SFC.ABS_ESC.Event.Escoff.Stat
 * SFC.Event.LinkedSound.ID          <- SFC.ABS_ESC.Event.Escoff.LinkedSound.ID
 * SFC.Event.LinkedSound.Type        <- SFC.ABS_ESC.Event.Escoff.LinkedSound.Type
 * SFC.Event.LinkedSound.RepeatCount <- SFC.ABS_ESC.Event.Escoff.LinkedSound.RepeatCount
 * SFC.Event.LinkedSound.BlinkOnDuration <- SFC.ABS_ESC.Event.Escoff.LinkedSound.BlinkOnDuration
 * SFC.Event.LinkedSound.BlinkOffDuration <- SFC.ABS_ESC.Event.Escoff.LinkedSound.BlinkOffDuration
 */
class GroupSubscriptionListener : public ccos::vehicle::vsm::IHSubscriptionListener {
public:
    using EventPriorityMap = std::unordered_map<std::string /*EventID*/, std::string /*Priority ID*/>;

    explicit GroupSubscriptionListener(VehicleSignalMapCallback callback, bool filterInitialOffValue = false);
    explicit GroupSubscriptionListener(VehicleSignalVectorCallback callback, bool filterInitialOffValue = false);
    explicit GroupSubscriptionListener(VehicleSignalMapCallback callback, EventPriorityMap&& map,
                                       bool filterInitialOffValue = false);
    explicit GroupSubscriptionListener(VehicleSignalVectorCallback callback, EventPriorityMap&& map,
                                       bool filterInitialOffValue = false);

protected:
    virtual std::string getPriorityId(const std::string& eventId);

private:
    using SignalList = std::vector<ccos::vehicle::vsm::HVehicleSignal>;
    void onVehicleSignalReceived(const SignalList& signalList) override;
    void dispatchEventCallbackVector(const SignalList& signalList);
    void dispatchEventCallbackMap(const SignalList& signalList);
    std::map<std::string /*event id*/, SignalList> separateEachEvent(const SignalList& signalList);

    VehicleSignalMapCallback mMapCallback = nullptr;
    VehicleSignalVectorCallback mVectorCallback = nullptr;
    bool mFilterInitialOffValue;
    bool mInitialReceive = true;
    EventPriorityMap mEventPriorityMap;
};

}  // namespace sfc

#endif  // SFCHELPER_H_
