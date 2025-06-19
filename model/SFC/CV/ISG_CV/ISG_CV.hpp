/**
 * @file ISG_CV.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2023  Hyundai Motor Company,
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
#ifndef SFSS_ISG_CV_H
#define SFSS_ISG_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "ISG_CVBase.hpp"

namespace ccos {

// SFC Version : 0.0.0
// Spec Version : v0.11
// Reference : [CV970] ISG_CV. Contains : Telltale, Event. Recommend: VALUE_CHANGED
class ISG_CV : public ISG_CVBase {
public:
    ISG_CV() = default;
    ~ISG_CV() override = default;
    ISG_CV(const ISG_CV& other) = delete;
    ISG_CV(ISG_CV&& other) noexcept = delete;
    ISG_CV& operator=(const ISG_CV& other) = delete;
    ISG_CV& operator=(ISG_CV&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;

        updateTelltaleISGCV();
        updateEventISGCV();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;

        updateTelltaleISGCV();
        updateEventISGCV();
    }

    void onIsgcvStatusChanged(const ArgumentsIsgcvStatusChanged& args) {
        mISGCVStatus = args.mInput_ISGCVStatus;

        updateTelltaleISGCV();
        updateEventISGCV();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateTelltaleISGCV() {
        SFCISG_CVTelltaleISGCVStat stat = SFCISG_CVTelltaleISGCVStat::OFF;
        SFCISG_CVTelltaleISGCVBlinkValueA statA = SFCISG_CVTelltaleISGCVBlinkValueA::OFF;
        SFCISG_CVTelltaleISGCVBlinkValueB statB = SFCISG_CVTelltaleISGCVBlinkValueB::OFF;

        if (mIsIgnOn == true) {
            if (mISGCVStatus == ISGCVStatus::AUTO_STOP) {
                stat = SFCISG_CVTelltaleISGCVStat::GREEN;
            } else if (mISGCVStatus == ISGCVStatus::ENFORCED_AUTO_START) {
                stat = SFCISG_CVTelltaleISGCVStat::BLINK2;
                statA = SFCISG_CVTelltaleISGCVBlinkValueA::GREEN;
                statB = SFCISG_CVTelltaleISGCVBlinkValueB::OFF;
            } else if (mISGCVStatus == ISGCVStatus::AUTO_STOP_DEACTIVATED) {
                stat = SFCISG_CVTelltaleISGCVStat::AMBER;
            } else if (mISGCVStatus == ISGCVStatus::ESS_ERROR) {
                stat = SFCISG_CVTelltaleISGCVStat::BLINK2;
                statA = SFCISG_CVTelltaleISGCVBlinkValueA::AMBER;
                statB = SFCISG_CVTelltaleISGCVBlinkValueB::OFF;
            } else {
                // no operation
            }
        }
        setSFCISG_CVTelltaleISGCVStat(stat);
        setSFCISG_CVTelltaleISGCVBlinkValueA(statA);
        setSFCISG_CVTelltaleISGCVBlinkValueB(statB);
    }

    void updateEventISGCV() {
        std::string eventID;
        std::string linkedSoundID;

        if (mIsIgnOn == true) {
            if (mISGCVStatus == ISGCVStatus::ENFORCED_AUTO_START) {
                eventID = "E79701";
                linkedSoundID = "SND_PopUpInform2";
            } else if (mISGCVStatus == ISGCVStatus::ESS_ERROR) {
                eventID = "E79702";
                linkedSoundID = "SND_PopUpWarn1";
            } else {
                // no operation
            }
        }

        std::string prevEventID;
        GETCACHEDVALUE(SFC.ISG_CV.Event.ISGCV.ID, prevEventID);
        if (prevEventID.empty() == false && prevEventID != eventID) {
            setSFCISG_CVEventISGCVStat(SFCISG_CVEventISGCVStat::OFF);
            flushLastGroup();
        }
        if (eventID.empty() == false) {
            setSFCISG_CVEventISGCVID(eventID);
            setSFCISG_CVEventISGCVStat(SFCISG_CVEventISGCVStat::ON);
            setSFCISG_CVEventISGCVLinkedSoundID(linkedSoundID);
            setSFCISG_CVEventISGCVLinkedSoundType(SFCISG_CVEventISGCVLinkedSoundType::REPEAT_COUNT);
            setSFCISG_CVEventISGCVLinkedSoundRepeatCount(1);
        }
    }

    HBool mIsIgnOn = false;

    using ISGCVStatus = ArgumentsIsgcvStatusChanged::Input_ISGCVStatus;

    ISGCVStatus mISGCVStatus = ISGCVStatus::OFF;
};

}  // namespace ccos

#endif  // SFSS_ISG_CV_H
