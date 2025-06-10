/**
 * @file Prewarning_Notification.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2021  Hyundai Motor Company,
 * All Rights Reserved.
 *
 * Use and copying of this software and preparation of derivative works
 * based upon this software are permitted. Any copy of this software or
 * of any derivative work must include the above copyright notice, this
 * paragraph and the one after it. Any distribution of this software or
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
#ifndef SFSS_Prewarning_Notification_H
#define SFSS_Prewarning_Notification_H

#define DLOG_ENABLED gEnableSFCLog

#include <Vehicle.h>
#include "Prewarning_NotificationBase.hpp"

namespace ccos {

// SFC Version : 0.4.0
// Reference : [ETC] virtual SFC to support Prewarning_Notification
class Prewarning_Notification : public Prewarning_NotificationBase {
public:
    Prewarning_Notification() = default;
    ~Prewarning_Notification() override = default;
    Prewarning_Notification(const Prewarning_Notification& other) = delete;
    Prewarning_Notification(Prewarning_Notification&& other) noexcept = delete;
    Prewarning_Notification& operator=(const Prewarning_Notification& other) = delete;
    Prewarning_Notification& operator=(Prewarning_Notification&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& args) {
        if (ISINITIALIZEDSIGNAL(args.mElapsedOn0ms) == true) {
            setSFCPrewarning_NotificationTelltalePrewarningStatus(SFCPrewarning_NotificationTelltalePrewarningStatus::PREPARE);
            flush();
        }
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& args) {
        if (ISINITIALIZEDSIGNAL(args.mElapsedOn500ms) == true) {
            updateActuatorValue(Vehicle.System.Engineering.Telltale.Output_PreWarningStatus,
                                static_cast<ccos::HUInt64>(PrewarningComplete::PreWarning));
            flushActuatorOutput();
            setSFCPrewarning_NotificationTelltalePrewarningStatus(SFCPrewarning_NotificationTelltalePrewarningStatus::PREWARNING);
            flush();
        }
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& args) {
        if (ISINITIALIZEDSIGNAL(args.mElapsedOff0ms) == true) {
            updateActuatorValue(Vehicle.System.Engineering.Telltale.Output_PreWarningStatus,
                                static_cast<ccos::HUInt64>(PrewarningComplete::OFF));
            flushActuatorOutput();
            setSFCPrewarning_NotificationTelltalePrewarningStatus(SFCPrewarning_NotificationTelltalePrewarningStatus::OFF);
            flush();
        }
    }

private:
    enum class PrewarningComplete : HUInt64 {
        OFF,
        PreWarning
    };

    template <typename T>
    void updateActuatorValue(const std::string& nodeAddress, const T& value) {
        mActuatorOutputAddrList.emplace_back(nodeAddress, value);
    }

    void flushActuatorOutput() {
        if (!mActuatorOutputAddrList.empty()) {
            setValue(mActuatorOutputAddrList);
            mActuatorOutputAddrList.clear();
        }
    }

    AddressValueList mActuatorOutputAddrList;
};

}  // namespace ccos

#endif  // SFSS_Prewarning_Notification_H
