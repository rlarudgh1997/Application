/**
 * @file HSubscription.h
 *
 * Copyright (c) 2019-2021  Hyundai Motor Company,
 * All Rights Reserved.
 *
 * Use and copying of this software and preparation of derivative works
 * based upon this software are permitted. Any copy of this software or
 * of any derivative work must include the above copyright notice, this
 * paragraph and the one after it.  Any distribution of this software or
 * derivative works must comply with all applicable laws.
 *
 * This software is made available AS IS, and COPYRIGHT OWNERS DISCLAIMS
 * ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, AND NOTWITHSTANDING ANY OTHER PROVISION CONTAINED HEREIN, ANY
 * LIABILITY FOR DAMAGES RESULTING FROM THE SOFTWARE OR ITS USE IS
 * EXPRESSLY DISCLAIMED, WHETHER ARISING IN CONTRACT, TORT (INCLUDING
 * NEGLIGENCE) OR STRICT LIABILITY, EVEN IF COPYRIGHT OWNERS ARE ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGES.
 */

#ifndef CCOS_APPLICATION_HSUBSCRIPTION_H
#define CCOS_APPLICATION_HSUBSCRIPTION_H

#include <HVehicleSignal.h>
#include <functional>

namespace ccos {
namespace vehicle {
namespace vsm {

/**
 * @brief HSubscriptionType defines the subscription types.
 */
enum class HSubscriptionType {
    VALUE_CHANGED,               /**< Should be captured when value changed. */
    VALUE_CHANGED_SAMPLING_TIME, /**< Should be captured when value changed at every sampling time. */
    PERIODIC_DB_CYCLETIME,       /**< Should be captured periodically according to database cycletime. */
    PERIODIC_USER_CYCLETIME,     /**< Should be captured periodically according to user cycletime. */
    MAX
};

/**
 * @brief Listener for receiving in-vehicle signals events based on VSM format.
 * @details This class is used for receiving in-vehicle signal state events.
 * @sa ccos::vehicle::vsm::HSubscription::HSubscription
 */
class IHSubscriptionListener {
public:
    /**
     * @brief Destroys an object.
     */
    virtual ~IHSubscriptionListener() = default;

    /**
     * @brief Called when a vehicle signal is received.
     * @param[in] signalList The HVehicleSignal list.
     */
    virtual void onVehicleSignalReceived(const std::vector<HVehicleSignal>& signalList) = 0;

    /**
     * @brief Called when a specific signal gets timeout.
     * @details A specific signal which app wants to detect timeout should be subscribed via
     * HVehicleSignalModel::subscribe.
     * @param[in] signal The HVehicleSignal.
     */
    virtual void onVehicleSignalTimeout(const HVehicleSignal& signal);
};

/**
 * @brief Provide vsm based interfaces for subscribing specific in-vehicle signals.
 * @details This class is for monitoring of specific leaf or branch node for vehicle signal model.
 * @sa ccos::vehicle::vsm::HVehicleSignalModel::subscribe
 */
class HSubscription {
public:
    /**
     * @brief Get node paths.
     * @retval std::vector<std::string> paths of vehicle signal nodes.
     */
    std::vector<std::string> getNodePaths() const;

    /**
     * @brief Get subscription type.
     * @retval HSubscriptionType subscription type.
     */
    HSubscriptionType getSubscriptionType() const;

    /**
     * @brief Get cycle time.
     * @details Valid only if HSubscriptionType::VALUE_CHANGED_SAMPLING_TIME or
     * HSubscriptionType::PERIODIC_USER_CYCLETIME is set.
     * @retval HUInt64 cycle time in millisecond.
     */
    HUInt64 getCycleTime() const;

    /**
     * @brief Get get signal listener.
     * @retval std::shared_ptr<IHSubscriptionListener> shared pointer of IHSubscriptionListener.
     */
    std::shared_ptr<IHSubscriptionListener> getSignalListener() const;

    /**
     * @brief Constructs an object with parameters.
     * @param[in] nodePaths The vehicle signal node path.
     * @param[in] type The vehicle signal node type.
     * @param[in] listener The shared pointer of IHSubscriptionListener.
     */
    HSubscription(const std::vector<std::string> nodePaths, const HSubscriptionType& type,
                  const std::shared_ptr<IHSubscriptionListener> listener);

    /**
     * @brief Constructs an object with parameters including cycle time of signal.
     * @param[in] nodePaths The vehicle signal node path.
     * @param[in] type The vehicle signal node type.
     * @param[in] cycleTime The vehicle signal cycle time.
     * @param[in] listener The shared pointer of IHSubscriptionListener.
     */
    HSubscription(const std::vector<std::string> nodePaths, const HSubscriptionType& type, const HUInt64 cycleTime,
                  const std::shared_ptr<IHSubscriptionListener> listener);

    /**
     * @brief Destroys an object.
     */
    ~HSubscription();

private:
    HSubscription(const HSubscription& other) = delete;
    HSubscription& operator=(const HSubscription& other) = delete;
    HSubscription(HSubscription&& other) noexcept = delete;
    HSubscription& operator=(HSubscription&& other) noexcept = delete;

    class HSubscriptionImpl;
    std::unique_ptr<HSubscriptionImpl> m_pImpl;
};

}  // namespace vsm
}  // namespace vehicle
}  // namespace ccos

#endif  // CCOS_APPLICATION_HSUBSCRIPTION_H
