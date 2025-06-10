/**
 * @file CMS.hpp
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
#ifndef SFSS_CMS_H
#define SFSS_CMS_H

#define DLOG_ENABLED gEnableSFCLog

#include "CMSBase.hpp"

namespace ccos {

// SFC Version : 0.1.1
// Reference : [EC020] CMS. Contains : Event. Recommend: VALUE_CHANGED
class CMS : public CMSBase {
public:
    CMS() = default;
    ~CMS() override = default;
    CMS(const CMS& other) = delete;
    CMS(CMS&& other) noexcept = delete;
    CMS& operator=(const CMS& other) = delete;
    CMS& operator=(CMS&& other) noexcept = delete;

    void onInitialize() override {
        setSFCCMSEventCMSID("E60101");
        setSFCCMSEventCMSLinkedSoundID("SND_PopUpWarn1");
        setSFCCMSEventCMSLinkedSoundType(SFCCMSEventCMSLinkedSoundType::REPEAT_COUNT);
        setSFCCMSEventCMSLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateWarningEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateWarningEvent();
    }

    void onConfigCMSChanged(const ArgumentsConfigCMSChanged& args) {
        mConfigCMS = args.mInter_ConfigCMS;
        updateWarningEvent();
    }

    void onCMSFailWarnStatusChanged(const ArgumentsCMSFailWarnStatusChanged& args) {
        if (ISTIMEOUT(args.mInput_CmsLhFailWarnStatus) == true) {
            mCMSLhFailWarnStatusChanged = CMSLhFailWarnStatusChanged::UNHANDLED_TIMEOUT;
        } else {
            mCMSLhFailWarnStatusChanged = args.mInput_CmsLhFailWarnStatus;
        }
        if (ISTIMEOUT(args.mInput_CmsRhFailWarnStatus) == true) {
            mCMSRhFailWarnStatusChanged = CMSRhFailWarnStatusChanged::UNHANDLED_TIMEOUT;
        } else {
            mCMSRhFailWarnStatusChanged = args.mInput_CmsRhFailWarnStatus;
        }
        updateWarningEvent();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };
    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;

    using ConfigCMS = ArgumentsConfigCMSChanged::Inter_ConfigCMS;
    using CMSLhFailWarnStatusChanged = ArgumentsCMSFailWarnStatusChanged::Input_CmsLhFailWarnStatus;
    using CMSRhFailWarnStatusChanged = ArgumentsCMSFailWarnStatusChanged::Input_CmsRhFailWarnStatus;

    ConfigCMS mConfigCMS = ConfigCMS::OFF;
    CMSLhFailWarnStatusChanged mCMSLhFailWarnStatusChanged = CMSLhFailWarnStatusChanged::OFF;
    CMSRhFailWarnStatusChanged mCMSRhFailWarnStatusChanged = CMSRhFailWarnStatusChanged::OFF;

    void updateWarningEvent() {
        if (mIgnElapsed == IgnElapsed::ON_0ms && mConfigCMS == ConfigCMS::ON &&
            (mCMSLhFailWarnStatusChanged == CMSLhFailWarnStatusChanged::ON ||
             mCMSLhFailWarnStatusChanged == CMSLhFailWarnStatusChanged::UNHANDLED_TIMEOUT ||
             mCMSRhFailWarnStatusChanged == CMSRhFailWarnStatusChanged::ON ||
             mCMSRhFailWarnStatusChanged == CMSRhFailWarnStatusChanged::UNHANDLED_TIMEOUT)) {
            setSFCCMSEventCMSStat(SFCCMSEventCMSStat::ON);
        } else {
            setSFCCMSEventCMSStat(SFCCMSEventCMSStat::OFF);
        }
    }
};

}  // namespace ccos

#endif  // SFSS_CMS_H
