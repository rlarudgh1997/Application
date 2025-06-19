/**
 * @file AFS.hpp
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
#ifndef SFSS_AFS_H
#define SFSS_AFS_H

#define DLOG_ENABLED gEnableSFCLog

#include "AFSBase.hpp"

namespace ccos {

// SFC Version : 5.0.0
// Spec Version : v0.31
// Reference : [CV640] AFS. Contains : Event. Recommend: VALUE_CHANGED
class AFS : public AFSBase {
public:
    AFS() = default;
    ~AFS() override = default;
    AFS(const AFS& other) = delete;
    AFS(AFS&& other) noexcept = delete;
    AFS& operator=(const AFS& other) = delete;
    AFS& operator=(AFS&& other) noexcept = delete;

    void onInitialize() override {
        setSFCAFSEventIFSIndLampID("E76401");
        setSFCAFSEventIFSIndLampLinkedSoundID("SND_PopUpWarn1");
        setSFCAFSEventIFSIndLampLinkedSoundRepeatCount(1);
        setSFCAFSEventIFSIndLampLinkedSoundType(SFCAFSEventIFSIndLampLinkedSoundType::REPEAT_COUNT);

        setSFCAFSEventIFSSystemID("E76402");
        setSFCAFSEventIFSSystemLinkedSoundID("SND_PopUpWarn1");
        setSFCAFSEventIFSSystemLinkedSoundRepeatCount(1);
        setSFCAFSEventIFSSystemLinkedSoundType(SFCAFSEventIFSSystemLinkedSoundType::REPEAT_COUNT);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        updateEventIFSIndLamp();
        updateEventIFSSystem();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        updateEventIFSIndLamp();
        updateEventIFSSystem();
    }

    void onConfigIFSCVChanged(const ArgumentsConfigIFSCVChanged& args) {
        mConfigIFSCV = args.mInter_ConfigIFSCV;
        updateEventIFSIndLamp();
        updateEventIFSSystem();
    }

    void onIfsIndLampStatChanged(const ArgumentsIfsIndLampStatChanged& args) {
        mIFSIndLampStatus = args.mInput_IFSIndLampStatus;
        updateEventIFSIndLamp();
    }

    void onIFSAfsWarnStatusChanged(const ArgumentsIFSAfsWarnStatusChanged& args) {
        mIFSAfsWarnStatus = args.mInput_IFSAfsWarnStatus;
        updateEventIFSSystem();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateEventIFSIndLamp() {
        SFCAFSEventIFSIndLampStat stat = SFCAFSEventIFSIndLampStat::OFF;
        if (mIsIgnOn == true && mConfigIFSCV == ConfigIFSCV::ON && mIFSIndLampStatus == IFSIndLampStatus::BLOCKAGE) {
            stat = SFCAFSEventIFSIndLampStat::ON;
        }
        setSFCAFSEventIFSIndLampStat(stat);
    }

    void updateEventIFSSystem() {
        SFCAFSEventIFSSystemStat stat = SFCAFSEventIFSSystemStat::OFF;
        if (mIsIgnOn == true && mConfigIFSCV == ConfigIFSCV::ON && mIFSAfsWarnStatus == IFSAfsWarnStatus::ON) {
            stat = SFCAFSEventIFSSystemStat::ON;
        }

        setSFCAFSEventIFSSystemStat(stat);
    }

    using ConfigIFSCV = ArgumentsConfigIFSCVChanged::Inter_ConfigIFSCV;
    using IFSIndLampStatus = ArgumentsIfsIndLampStatChanged::Input_IFSIndLampStatus;
    using IFSAfsWarnStatus = ArgumentsIFSAfsWarnStatusChanged::Input_IFSAfsWarnStatus;

    bool mIsIgnOn = false;
    ConfigIFSCV mConfigIFSCV = ConfigIFSCV::OFF;
    IFSIndLampStatus mIFSIndLampStatus = IFSIndLampStatus::OFF;
    IFSAfsWarnStatus mIFSAfsWarnStatus = IFSAfsWarnStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_AFS_H
