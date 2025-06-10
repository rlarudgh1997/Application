/**
 * @file Detail_Contents_ICC.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2022  Hyundai Motor Company,
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
#ifndef SFSS_Detail_Contents_ICC_H
#define SFSS_Detail_Contents_ICC_H

#define DLOG_ENABLED gEnableSFCLog

#include "Detail_Contents_ICCBase.hpp"

namespace ccos {

// SFC Version : 4.0.1
// Reference : [HD160] Detail_Contents_ICC Contains : Event Recommend: VALUE_CHANGED
class Detail_Contents_ICC : public Detail_Contents_ICCBase {
public:
    Detail_Contents_ICC() = default;
    ~Detail_Contents_ICC() override = default;
    Detail_Contents_ICC(const Detail_Contents_ICC& other) = delete;
    Detail_Contents_ICC(Detail_Contents_ICC&& other) noexcept = delete;
    Detail_Contents_ICC& operator=(const Detail_Contents_ICC& other) = delete;
    Detail_Contents_ICC& operator=(Detail_Contents_ICC&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /* args */) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateEvent();
    }

    void onIccWarningStatus(const ArgumentsIccWarningStatus& args) {
        mIccWarnStatus = args.mInput_IccWarningStatus;
        updateEvent();
    }

    void onConfigHUDChanged(const ArgumentsConfigHUDChanged& args) {
        mConfigHUD = args.mInter_ConfigHUD;
        mConfigNewHUD = args.mInter_ConfigNewHUD;
        updateEvent();
    }

    void dump(std::ostringstream& /* out */, const std::string& /* prefix */, const std::string& /* command */) override {
        // no operation
    }

private:
    void updateEvent() {
        if (!isConfigHudOn()) {
            return;
        }
        std::string eventID;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            switch (mIccWarnStatus) {
                case IccWarnStatus::DDMS_WARN:
                    eventID = "E71170";
                    break;
                case IccWarnStatus::DISTRACTION_WARN:
                    eventID = "E71171";
                    break;
                case IccWarnStatus::DROWSINESS_WARN:
                    eventID = "E71172";
                    break;
                case IccWarnStatus::DDMS_WARN2:
                    eventID = "E71173";
                    break;
                default:
                    break;
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.Detail_Contents_ICC.Event.IccWarn.ID, prevEventID);
        if (!prevEventID.empty() && prevEventID != eventID) {
            setSFCDetail_Contents_ICCEventIccWarnStat(SFCDetail_Contents_ICCEventIccWarnStat::OFF);
            flushLastGroup();
        }

        if (!eventID.empty()) {
            setSFCDetail_Contents_ICCEventIccWarnID(eventID);
            setSFCDetail_Contents_ICCEventIccWarnStat(SFCDetail_Contents_ICCEventIccWarnStat::ON);
        }
    }

    inline HBool isConfigHudOn() const {
        return mConfigHUD == ConfigHUD::ON || mConfigNewHUD == ConfigNewHUD::ON;
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        OFF_0ms,
        ON_0ms,
        MAX
    };

    using IccWarnStatus = ArgumentsIccWarningStatus::Input_IccWarningStatus;
    using ConfigHUD = ArgumentsConfigHUDChanged::Inter_ConfigHUD;
    using ConfigNewHUD = ArgumentsConfigHUDChanged::Inter_ConfigNewHUD;
    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    IccWarnStatus mIccWarnStatus = IccWarnStatus::OFF;
    ConfigHUD mConfigHUD = ConfigHUD::OFF;
    ConfigNewHUD mConfigNewHUD = ConfigNewHUD::OFF;
};

}  // namespace ccos

#endif  // SFSS_Detail_Contents_ICC_H
