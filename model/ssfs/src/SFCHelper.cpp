#include "SFCHelper.h"

#include <regex>
#include <unordered_set>
#include <unordered_map>
#include <utility>

#include "SFC.h"  // provide SFC instance. SFC should have SFC.Event
#include "EventPriority.hpp"

namespace sfc {

using ccos::vehicle::vsm::HVehicleSignal;

//-----------------------------------------------------------------------------
// SFCNodeAddressProvider : File Scope Utility Class
//-----------------------------------------------------------------------------

class SFCNodeAddressProvider {
public:
    static SFCNodeAddressProvider& get() {
        static SFCNodeAddressProvider me;
        return me;
    }

    void addAddressSet(Node& topNode) {
        resolveNodeInfo(topNode.getChildNodes());
    }

    std::vector<NodeAddress> getMatchedNodeAddressList(const NodeAddress& addressNameInRegExp) {
        std::vector<NodeAddress> matchedNodeAddressList;
        std::regex regularExpression = std::regex(addressNameInRegExp);
        for (const auto& iter : mNodeAddressList) {
            if (std::regex_match(iter, regularExpression)) {
                matchedNodeAddressList.push_back(iter);
            }
        }
        return matchedNodeAddressList;
    }

private:
    SFCNodeAddressProvider() {
        resolveNodeInfo(SFC.getChildNodes());
    }

    void resolveNodeInfo(const std::vector<Node*>& childNodes) {
        for (auto* node : childNodes) {
            // handle node address leaf node and branch both to support branch subscription.
            mNodeAddressList.insert(node->getAddress());
            resolveNodeInfo(node->getChildNodes());
        }
    }

    std::unordered_set<NodeAddress> mNodeAddressList;
};

//-----------------------------------------------------------------------------
// Utility APIs
//-----------------------------------------------------------------------------

bool registerSFCAddressSet(Node& topNode) {
    SFCNodeAddressProvider::get().addAddressSet(topNode);
    return true;
}

std::vector<NodeAddress> searchAddress(const NodeAddress& addressInRegExp) {
    return SFCNodeAddressProvider::get().getMatchedNodeAddressList(addressInRegExp);
}

void tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters = " ") {
    std::string::size_type beginPos = str.find_first_not_of(delimiters, 0);
    std::string::size_type pos = str.find_first_of(delimiters, beginPos);

    while (std::string::npos != pos || std::string::npos != beginPos) {
        tokens.push_back(str.substr(beginPos, pos - beginPos));
        beginPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, beginPos);
    }
}

std::map<std::string /*SFCName*/, std::vector<NodeAddress>> divideBySFCFunction(const std::vector<NodeAddress>& addresses) {
    std::map<std::string /*SFCName*/, std::vector<NodeAddress>> divided;
    for (const auto& nodeAddress : addresses) {
        std::vector<std::string> tokens;
        tokenize(nodeAddress, tokens, ".");
        if (tokens.size() < 4) {
            continue;
        }

        // SFC Address ("SFC"(prefix) + SFCNAME + CATEGORY + FUNCTIONNAME)
        std::string branchAddress = tokens[0] + "." + tokens[1] + "." + tokens[2] + "." + tokens[3];
        auto it = divided.find(branchAddress);
        if (it == divided.end()) {
            it = divided.emplace(branchAddress, std::vector<std::string>()).first;
        }
        it->second.emplace_back(nodeAddress);
    }
    return divided;
}

//-----------------------------------------------------------------------------
// SubscriptionListener
//-----------------------------------------------------------------------------

SubscriptionListener::SubscriptionListener(VehicleSignalMapCallback callback) : mMapCallback(std::move(callback)) {
}

SubscriptionListener::SubscriptionListener(VehicleSignalVectorCallback callback) : mVectorCallback(std::move(callback)) {
}

void SubscriptionListener::onVehicleSignalReceived(const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList) {
    if (mVectorCallback) {
        mVectorCallback(signalList);
    }
    if (mMapCallback) {
        std::map<NodeAddress, HVehicleSignal> vehicleSignalMap;
        std::transform(signalList.begin(), signalList.end(), std::inserter(vehicleSignalMap, vehicleSignalMap.end()),
                       [](const HVehicleSignal& vehicleSignal) -> std::pair<NodeAddress, HVehicleSignal> {
                           return {vehicleSignal.getNodePath(), HVehicleSignal(vehicleSignal)};
                       });
        mMapCallback(vehicleSignalMap);
    }
}

//-----------------------------------------------------------------------------
// SFCNodeAddress : File Scope Utility Class
//-----------------------------------------------------------------------------

class SFCNodeAddress {
public:
    enum class EventKind : uint32_t {
        ID,
        Stat,
        LinkedSoundID,
        LinkedSoundType,
        LinkedSoundRepeatCount,
        LinkedSoundRepeatMin,
        LinkedSoundRepeatMax,
        LinkedSoundBlinkOffDuration,
        LinkedSoundBlinkOnDuration,
        LinkedSoundDuration,
        LinkedSoundDurationMin,
        LinkedSoundDurationMax,
        MasterWarning,
        LinkedVoiceID,
        Invalid
    };

    static EventKind makeCommonEventAddress(const NodeAddress& address, NodeAddress& commonEventAddress) {
        static std::regex reEvent{R"((SFC|SFCFuel)\.[0-9a-zA-Z_]+\.Event\.[0-9a-zA-Z_]+\.([\.a-zA-Z_]+))"};
        // (alt) R"((SFC[0-9a-zA-Z_]*)\.[0-9a-zA-Z_]+\.Event\.[0-9a-zA-Z_]+\.([\.a-zA-Z_]+))"
        static std::unordered_map<std::string, EventKind> eventKindMap = {
            {"ID", EventKind::ID},
            {"Stat", EventKind::Stat},
            {"MasterWarning", EventKind::MasterWarning},
            {"LinkedVoice.ID", EventKind::LinkedVoiceID},
            {"LinkedSound.ID", EventKind::LinkedSoundID},
            {"LinkedSound.Type", EventKind::LinkedSoundType},
            {"LinkedSound.RepeatCount", EventKind::LinkedSoundRepeatCount},
            {"LinkedSound.RepeatMin", EventKind::LinkedSoundRepeatMin},
            {"LinkedSound.RepeatMax", EventKind::LinkedSoundRepeatMax},
            {"LinkedSound.BlinkOffDuration", EventKind::LinkedSoundBlinkOffDuration},
            {"LinkedSound.BlinkOnDuration", EventKind::LinkedSoundBlinkOnDuration},
            {"LinkedSound.Duration", EventKind::LinkedSoundDuration},
            {"LinkedSound.DurationMin", EventKind::LinkedSoundDurationMin},
            {"LinkedSound.DurationMax", EventKind::LinkedSoundDurationMax}};
        EventKind eventKind = EventKind::Invalid;
        std::smatch match;
        try {
            if (std::regex_match(address, match, reEvent)) {
                if (match.size() == 3) {
                    std::string domain = match[1].str();
                    std::string kind = match[2].str();
                    auto found = eventKindMap.find(kind);
                    if (found != eventKindMap.end()) {
                        eventKind = found->second;
                        commonEventAddress = domain + ".Event." + kind;
                    }
                }
            }
        } catch (const std::regex_error& /*e*/) {
            // log Error
            eventKind = EventKind::Invalid;
        }
        return eventKind;
    }

    static bool isEventId(const NodeAddress& address) {
        static std::regex me{R"((SFC|SFCFuel)\.[0-9a-zA-Z_]+\.Event\.[0-9a-zA-Z_]+\.ID)"};
        // (alt) {R"((SFC[0-9a-zA-Z_]*)\.[0-9a-zA-Z_]+\.Event\.[0-9a-zA-Z_]+\.ID)"
        return regexMatch(address, me);
    }

private:
    inline static bool regexMatch(const NodeAddress& address, const std::regex& re) {
        bool matched = false;
        try {
            matched = std::regex_match(address, re);
        } catch (const std::regex_error& /*e*/) {
            // log Error
            matched = false;
        }
        return matched;
    }
};

//-----------------------------------------------------------------------------
// class GroupSubscriptionListener
//-----------------------------------------------------------------------------

GroupSubscriptionListener::GroupSubscriptionListener(VehicleSignalMapCallback callback, bool filterInitialOffValue)
    : mMapCallback(std::move(callback)), mFilterInitialOffValue(filterInitialOffValue) {
    fillEventPriorityMap(mEventPriorityMap);
}

GroupSubscriptionListener::GroupSubscriptionListener(VehicleSignalVectorCallback callback, bool filterInitialOffValue)
    : mVectorCallback(std::move(callback)), mFilterInitialOffValue(filterInitialOffValue) {
    fillEventPriorityMap(mEventPriorityMap);
}

GroupSubscriptionListener::GroupSubscriptionListener(VehicleSignalMapCallback callback,
                                                     GroupSubscriptionListener::EventPriorityMap&& map,
                                                     bool filterInitialOffValue)
    : mMapCallback(std::move(callback)), mFilterInitialOffValue(filterInitialOffValue), mEventPriorityMap(std::move(map)) {
}

GroupSubscriptionListener::GroupSubscriptionListener(VehicleSignalVectorCallback callback,
                                                     GroupSubscriptionListener::EventPriorityMap&& map,
                                                     bool filterInitialOffValue)
    : mVectorCallback(std::move(callback)), mFilterInitialOffValue(filterInitialOffValue), mEventPriorityMap(std::move(map)) {
}

void GroupSubscriptionListener::onVehicleSignalReceived(const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList) {
    auto&& separatedSignalMap = separateEachEvent(signalList);

    // Sort???
    std::multimap<std::string, std::string> priorityToEventIdMap;
    for (const auto& signalListPrivate : separatedSignalMap) {
        auto eventId = signalListPrivate.first;
        auto priorityId = getPriorityId(eventId);
        priorityToEventIdMap.emplace(priorityId, eventId);
    }

    for (const auto& element : priorityToEventIdMap) {
        const auto& signalListElement = separatedSignalMap.at(element.second);
        if (mVectorCallback) {
            dispatchEventCallbackVector(signalListElement);
        }
        if (mMapCallback) {
            dispatchEventCallbackMap(signalListElement);
        }
    }
    mInitialReceive = false;
}

std::map<std::string /*event id*/, GroupSubscriptionListener::SignalList> GroupSubscriptionListener::separateEachEvent(
    const SignalList& signalList) {
    std::map<std::string /*event id*/, SignalList> eventIdSignalMap;
    std::list<std::pair<std::string /*prefix*/, std::string /*event id*/>> prefixEventIdList;

    // Sampling Signals with Event.ID
    for (const auto& eventSignal : signalList) {
        NodeAddress nodeAddress = eventSignal.getNodePath();
        if (!SFCNodeAddress::isEventId(nodeAddress)) {
            continue;
        }
        auto eventId = eventSignal.getValue<std::string>();
        auto pos = nodeAddress.find(".ID");
        auto prefix = nodeAddress.substr(0, pos + 1);  // prefix with dot [EXNCP-34127], [PD-396]

        eventIdSignalMap.insert({eventId, SignalList()});
        prefixEventIdList.emplace_back(prefix, eventId);
    }

    // Making SignalList by each Prefix
    SignalList signalListPrivate = signalList;
    for (const auto& element : prefixEventIdList) {
        const auto& prefix = element.first;
        const auto& eventId = element.second;
        auto& eventSignalList = eventIdSignalMap[eventId];

        auto prefixSearchIterator = signalListPrivate.begin();
        while (prefixSearchIterator != signalListPrivate.end()) {
            if (prefixSearchIterator->getNodePath().find(prefix) == 0) {
                eventSignalList.emplace_back(*prefixSearchIterator);
                prefixSearchIterator = signalListPrivate.erase(prefixSearchIterator);
                continue;
            }
            prefixSearchIterator++;
        }
    }
    if (!signalListPrivate.empty()) {
        eventIdSignalMap.emplace("", signalListPrivate);
    }
    return eventIdSignalMap;
}

void GroupSubscriptionListener::dispatchEventCallbackVector(const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList) {
    // it do not handle non-event type signals
    using ccos::HUInt64;
    using EventStat = decltype(SFC.Event.Stat)::Type;
    std::vector<HVehicleSignal> vehicleSignalVector;
    for (const auto& it : signalList) {
        NodeAddress nodeAddress = it.getNodePath();
        std::string commonEventAddress;
        SFCNodeAddress::EventKind kind = SFCNodeAddress::makeCommonEventAddress(nodeAddress, commonEventAddress);

        switch (kind) {
            case SFCNodeAddress::EventKind::ID:
                vehicleSignalVector.emplace_back(commonEventAddress, it.getValue<std::string>());
                break;
            case SFCNodeAddress::EventKind::Stat:
                if (mFilterInitialOffValue && mInitialReceive && it.getValue<HUInt64>() != static_cast<HUInt64>(EventStat::ON)) {
                    return;  // do not report initial OFF (non ON) event
                }
                vehicleSignalVector.emplace_back(commonEventAddress, it.getValue<HUInt64>());
                break;
            case SFCNodeAddress::EventKind::MasterWarning:
                vehicleSignalVector.emplace_back(commonEventAddress, it.getValue<ccos::HBool>());
                break;
            case SFCNodeAddress::EventKind::LinkedVoiceID:
            case SFCNodeAddress::EventKind::LinkedSoundID:
                vehicleSignalVector.emplace_back(commonEventAddress, it.getValue<std::string>());
                break;
            case SFCNodeAddress::EventKind::LinkedSoundType:
            case SFCNodeAddress::EventKind::LinkedSoundRepeatCount:
            case SFCNodeAddress::EventKind::LinkedSoundRepeatMin:
            case SFCNodeAddress::EventKind::LinkedSoundRepeatMax:
            case SFCNodeAddress::EventKind::LinkedSoundBlinkOffDuration:
            case SFCNodeAddress::EventKind::LinkedSoundBlinkOnDuration:
            case SFCNodeAddress::EventKind::LinkedSoundDuration:
            case SFCNodeAddress::EventKind::LinkedSoundDurationMin:
            case SFCNodeAddress::EventKind::LinkedSoundDurationMax:
                vehicleSignalVector.emplace_back(commonEventAddress, it.getValue<HUInt64>());
                break;
            case SFCNodeAddress::EventKind::Invalid:
                std::cout << "unhandled (identified as not-a-event address) : " << nodeAddress << std::endl;
                break;
            default:
                std::cout << "unhandled (error, something missed): " << nodeAddress << std::endl;
                break;
        }
    }

    if (!vehicleSignalVector.empty()) {
        mVectorCallback(vehicleSignalVector);
    }
    mInitialReceive = false;  // todo : fix it not to hide map case
}

void GroupSubscriptionListener::dispatchEventCallbackMap(const std::vector<ccos::vehicle::vsm::HVehicleSignal>& signalList) {
    // it do not handle non-event type signals
    using ccos::HUInt64;
    using EventStat = decltype(SFC.Event.Stat)::Type;
    std::map<NodeAddress, HVehicleSignal> vehicleSignalMap;
    for (const auto& it : signalList) {
        NodeAddress nodeAddress = it.getNodePath();
        std::string commonEventAddress;
        SFCNodeAddress::EventKind kind = SFCNodeAddress::makeCommonEventAddress(nodeAddress, commonEventAddress);

        switch (kind) {
            case SFCNodeAddress::EventKind::ID:
                vehicleSignalMap.emplace(commonEventAddress, HVehicleSignal(nodeAddress, it.getValue<std::string>()));
                break;
            case SFCNodeAddress::EventKind::Stat:
                if (mFilterInitialOffValue && mInitialReceive && it.getValue<HUInt64>() != static_cast<HUInt64>(EventStat::ON)) {
                    return;  // do not report initial OFF (non ON) event
                }
                vehicleSignalMap.emplace(commonEventAddress, HVehicleSignal(nodeAddress, it.getValue<HUInt64>()));
                break;
            case SFCNodeAddress::EventKind::MasterWarning:
                vehicleSignalMap.emplace(commonEventAddress, HVehicleSignal(nodeAddress, it.getValue<ccos::HBool>()));
                break;
            case SFCNodeAddress::EventKind::LinkedVoiceID:
            case SFCNodeAddress::EventKind::LinkedSoundID:
                vehicleSignalMap.emplace(commonEventAddress, HVehicleSignal(nodeAddress, it.getValue<std::string>()));
                break;
            case SFCNodeAddress::EventKind::LinkedSoundType:
            case SFCNodeAddress::EventKind::LinkedSoundRepeatCount:
            case SFCNodeAddress::EventKind::LinkedSoundRepeatMin:
            case SFCNodeAddress::EventKind::LinkedSoundRepeatMax:
            case SFCNodeAddress::EventKind::LinkedSoundBlinkOffDuration:
            case SFCNodeAddress::EventKind::LinkedSoundBlinkOnDuration:
            case SFCNodeAddress::EventKind::LinkedSoundDuration:
            case SFCNodeAddress::EventKind::LinkedSoundDurationMin:
            case SFCNodeAddress::EventKind::LinkedSoundDurationMax:
                vehicleSignalMap.emplace(commonEventAddress, HVehicleSignal(nodeAddress, it.getValue<HUInt64>()));
                break;
            case SFCNodeAddress::EventKind::Invalid:
                std::cout << "unhandled (identified as not-a-event address) : " << nodeAddress << std::endl;
                break;
            default:
                std::cout << "unhandled (error, something missed): " << nodeAddress << std::endl;
                break;
        }
    }

    if (!vehicleSignalMap.empty()) {
        mMapCallback(vehicleSignalMap);
    }
    mInitialReceive = false;  // todo : fix it not to hide vector case
}

std::string GroupSubscriptionListener::getPriorityId(const std::string& eventId) {
    auto result = mEventPriorityMap.find(eventId);
    if (result == mEventPriorityMap.end()) {
        return kUnknownPriority;
    }

    return result->second;
}

}  // namespace sfc
