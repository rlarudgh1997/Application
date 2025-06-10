/**
 * @file AEM.hpp
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
#ifndef SFSS_AEM_H
#define SFSS_AEM_H

#define DLOG_ENABLED gEnableSFCLog

#include "AEMBase.hpp"
#include <Vehicle.h>

namespace ccos {

// SFC Version : 0.1.0
// Reference : [ETC] virtual SFC to support AEM variables
class AEM : public AEMBase {
public:
    AEM() = default;
    ~AEM() override = default;
    AEM(const AEM& other) = delete;
    AEM(AEM&& other) noexcept = delete;
    AEM& operator=(const AEM& other) = delete;
    AEM& operator=(AEM&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        // no operation
    }

    void onTelltaleHmiInitializeChanged(const ArgumentsTelltaleHmiInitializeChanged& args) {
        if (ISINITIALIZEDSIGNAL(args.mInter_TelltaleHmiInitializeComplete) == true) {
            mInter_TelltaleHmiInitializeComplete = args.mInter_TelltaleHmiInitializeComplete;
            updateActuatorValue(Vehicle.System.Engineering.Telltale.Output_TelltaleHmiReadyStatus,
                                static_cast<HUInt64>(mInter_TelltaleHmiInitializeComplete));
            flushActuatorOutput();
        }
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    using Inter_TelltaleHmiInitializeComplete = ArgumentsTelltaleHmiInitializeChanged::Inter_TelltaleHmiInitializeComplete;

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
    Inter_TelltaleHmiInitializeComplete mInter_TelltaleHmiInitializeComplete = Inter_TelltaleHmiInitializeComplete::OFF;
};

}  // namespace ccos

#endif  // SFSS_AEM_H
