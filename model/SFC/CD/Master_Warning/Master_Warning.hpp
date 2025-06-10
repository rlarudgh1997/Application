/**
 * @file Master_Warning.hpp
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
#ifndef SFSS_Master_Warning_H
#define SFSS_Master_Warning_H

#define DLOG_ENABLED gEnableSFCLog

#include "Master_WarningBase.hpp"

namespace ccos {

// SFC Version : 3.0.1
// Reference : [CD080] Master_Warning. Contains : Telltale. Recommend: VALUE_CHANGED
class Master_Warning : public Master_WarningBase {
public:
    Master_Warning() = default;
    ~Master_Warning() override = default;
    Master_Warning(const Master_Warning& other) = delete;
    Master_Warning(Master_Warning&& other) noexcept = delete;
    Master_Warning& operator=(const Master_Warning& other) = delete;
    Master_Warning& operator=(Master_Warning&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateTelltale();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltale();
    }

    void onMasterWarningTelltaleChanged(const ArgumentsMasterWarningTelltaleChanged& args) {
        updateValueIfValid(mEventCheckDisplayStatus, args.mInter_EventCheckDisplayStatus);
        updateValueIfValid(mSystemCheckDisplayStatus, args.mInter_SystemCheckDisplayStatus);
        updateValueIfValid(mHDPMasterWarningStatus, args.mInter_HDPMasterWarningStatus);
        updateTelltale();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltale() {
        SFCMaster_WarningTelltaleMaster_WarningStat stat = SFCMaster_WarningTelltaleMaster_WarningStat::OFF;

        if ((mIgnElapsed == IgnElapsed::ON_0ms && mSystemCheckDisplayStatus == SystemCheckDisplayStatus::OFF &&
             mEventCheckDisplayStatus == EventCheckDisplayStatus::ON) ||
            (mIgnElapsed == IgnElapsed::OFF_0ms && mHDPMasterWarningStatus == HDPMasterWarningStatus::ON)) {
            stat = SFCMaster_WarningTelltaleMaster_WarningStat::ON;
        }
        setSFCMaster_WarningTelltaleMaster_WarningStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        ON_3500ms,
        OFF_0ms,
        MAX
    };

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;

    using SystemCheckDisplayStatus = ArgumentsMasterWarningTelltaleChanged::Inter_SystemCheckDisplayStatus;
    SystemCheckDisplayStatus mSystemCheckDisplayStatus = SystemCheckDisplayStatus::OFF;

    using EventCheckDisplayStatus = ArgumentsMasterWarningTelltaleChanged::Inter_EventCheckDisplayStatus;
    EventCheckDisplayStatus mEventCheckDisplayStatus = EventCheckDisplayStatus::OFF;

    using HDPMasterWarningStatus = ArgumentsMasterWarningTelltaleChanged::Inter_HDPMasterWarningStatus;
    HDPMasterWarningStatus mHDPMasterWarningStatus = HDPMasterWarningStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_Master_Warning_H
