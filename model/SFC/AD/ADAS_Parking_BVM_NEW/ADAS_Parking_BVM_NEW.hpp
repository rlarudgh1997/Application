/**
 * @file ADAS_Parking_BVM_NEW.hpp
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
#ifndef SFSS_ADAS_Parking_BVM_NEW_H
#define SFSS_ADAS_Parking_BVM_NEW_H

#define DLOG_ENABLED gEnableSFCLog

#include "ADAS_Parking_BVM_NEWBase.hpp"

namespace ccos {

// SFC Version : 5.0.1
// Reference : [AD100] ADAS Parking_BVM_NEW. Contains : Constant, Event. Recommend: VALUE_CHANGED
class ADAS_Parking_BVM_NEW : public ADAS_Parking_BVM_NEWBase {
public:
    ADAS_Parking_BVM_NEW() = default;
    ~ADAS_Parking_BVM_NEW() override = default;
    ADAS_Parking_BVM_NEW(const ADAS_Parking_BVM_NEW& other) = delete;
    ADAS_Parking_BVM_NEW(ADAS_Parking_BVM_NEW&& other) noexcept = delete;
    ADAS_Parking_BVM_NEW& operator=(const ADAS_Parking_BVM_NEW& other) = delete;
    ADAS_Parking_BVM_NEW& operator=(ADAS_Parking_BVM_NEW&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateOutMirrorFold();
        updateEvent();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateOutMirrorFold();
        updateEvent();
    }

    void onOutMirrorFoldStatusChanged(const ArgumentsOutMirrorFoldStatusChanged& args) {
        mOutMirrorFoldStatusFromBCM = args.mInput_OutMirrorFoldStatusFromBCM;
        updateOutMirrorFold();
    }

    void onBvmView2StatusChanged(const ArgumentsBvmView2StatusChanged& args) {
        mBvmView2Status = args.mInput_BvmView2Status;
        updateEvent();
    }

    void onInput_BvmVideoChanged(const ArgumentsInput_BvmVideoChanged& args) {
        mBvmVideo = args.mInput_BvmVideo;
        updateEvent();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    void updateOutMirrorFold() {
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mOutMirrorFoldStatusFromBCM == OutMirrorFoldStatusFromBCM::FOLD ||
                mOutMirrorFoldStatusFromBCM == OutMirrorFoldStatusFromBCM::FOLDING) {
                setSFCADAS_Parking_BVM_NEWConstantBVMLeftOutMirrorFoldStat(
                    SFCADAS_Parking_BVM_NEWConstantBVMLeftOutMirrorFoldStat::ON);
                setSFCADAS_Parking_BVM_NEWConstantBVMRightOutMirrorFoldStat(
                    SFCADAS_Parking_BVM_NEWConstantBVMRightOutMirrorFoldStat::ON);
            } else {
                setSFCADAS_Parking_BVM_NEWConstantBVMLeftOutMirrorFoldStat(
                    SFCADAS_Parking_BVM_NEWConstantBVMLeftOutMirrorFoldStat::OFF);
                setSFCADAS_Parking_BVM_NEWConstantBVMRightOutMirrorFoldStat(
                    SFCADAS_Parking_BVM_NEWConstantBVMRightOutMirrorFoldStat::OFF);
            }
        }
    }

    void updateEvent() {
        std::string eventId;
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mBvmVideo == BvmVideo::VALID) {
                switch (mBvmView2Status) {
                    case BvmView2Status::LEFT_DRIVE_BEFORE_4S:
                        eventId = "E53901";
                        break;
                    case BvmView2Status::LEFT_DRIVE_AFTER_4S:
                        eventId = "E53902";
                        break;
                    case BvmView2Status::LEFT_STOP:
                        eventId = "E53903";
                        break;
                    case BvmView2Status::RIGHT_DRIVE_BEFORE_4S:
                        eventId = "E53904";
                        break;
                    case BvmView2Status::RIGHT_DRIVE_AFTER_4S:
                        eventId = "E53905";
                        break;
                    case BvmView2Status::RIGHT_STOP:
                        eventId = "E53906";
                        break;
                    default:
                        break;
                }
            } else if (mBvmVideo == BvmVideo::INVALID) {
                if (mBvmView2Status == BvmView2Status::LEFT_DRIVE_BEFORE_4S ||
                    mBvmView2Status == BvmView2Status::LEFT_DRIVE_AFTER_4S || mBvmView2Status == BvmView2Status::LEFT_STOP) {
                    eventId = "E53907";
                } else if (mBvmView2Status == BvmView2Status::RIGHT_DRIVE_BEFORE_4S ||
                           mBvmView2Status == BvmView2Status::RIGHT_DRIVE_AFTER_4S ||
                           mBvmView2Status == BvmView2Status::RIGHT_STOP) {
                    eventId = "E53908";
                } else {
                    // no operation
                }
            } else {
                // no operation
            }
        }

        static std::string lastEventIdPrimary;
        static std::string lastEventIdSecondary;
        static std::string lastEventIdThird;
        if (eventId == "E53901" || eventId == "E53904") {
            setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldPrimaryID(eventId);
            setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldPrimaryStat(
                SFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldPrimaryStat::ON);
            flush();
            if (lastEventIdPrimary != eventId && !lastEventIdPrimary.empty()) {
                setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldPrimaryID(lastEventIdPrimary);
                setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldPrimaryStat(
                    SFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldPrimaryStat::OFF);
                flushLastGroup();
            }
            if (!lastEventIdSecondary.empty()) {
                setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldSecondaryID(lastEventIdSecondary);
                setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldSecondaryStat(
                    SFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldSecondaryStat::OFF);
                lastEventIdSecondary = "";
                flushLastGroup();
            }
            if (!lastEventIdThird.empty()) {
                setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldThirdID(lastEventIdThird);
                setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldThirdStat(
                    SFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldThirdStat::OFF);
                lastEventIdThird = "";
                flushLastGroup();
            }
            lastEventIdPrimary = eventId;
        } else if (eventId == "E53902" || eventId == "E53905") {
            setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldSecondaryID(eventId);
            setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldSecondaryStat(
                SFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldSecondaryStat::ON);
            flush();
            if (lastEventIdSecondary != eventId && !lastEventIdSecondary.empty()) {
                setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldSecondaryID(lastEventIdSecondary);
                setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldSecondaryStat(
                    SFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldSecondaryStat::OFF);
                flushLastGroup();
            }
            if (!lastEventIdPrimary.empty()) {
                setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldPrimaryID(lastEventIdPrimary);
                setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldPrimaryStat(
                    SFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldPrimaryStat::OFF);
                lastEventIdPrimary = "";
                flushLastGroup();
            }
            if (!lastEventIdThird.empty()) {
                setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldThirdID(lastEventIdThird);
                setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldThirdStat(
                    SFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldThirdStat::OFF);
                lastEventIdThird = "";
                flushLastGroup();
            }
            lastEventIdSecondary = eventId;
        } else if (eventId == "E53903" || eventId == "E53906" || eventId == "E53907" || eventId == "E53908") {
            setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldThirdID(eventId);
            setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldThirdStat(SFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldThirdStat::ON);
            flush();
            if (lastEventIdThird != eventId && !lastEventIdThird.empty()) {
                setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldThirdID(lastEventIdThird);
                setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldThirdStat(
                    SFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldThirdStat::OFF);
                flushLastGroup();
            }
            if (!lastEventIdPrimary.empty()) {
                setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldPrimaryID(lastEventIdPrimary);
                setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldPrimaryStat(
                    SFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldPrimaryStat::OFF);
                lastEventIdPrimary = "";
                flushLastGroup();
            }
            if (!lastEventIdSecondary.empty()) {
                setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldSecondaryID(lastEventIdSecondary);
                setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldSecondaryStat(
                    SFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldSecondaryStat::OFF);
                lastEventIdSecondary = "";
                flushLastGroup();
            }
            lastEventIdThird = eventId;
        } else {
            if (!lastEventIdPrimary.empty()) {
                setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldPrimaryID(lastEventIdPrimary);
                setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldPrimaryStat(
                    SFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldPrimaryStat::OFF);
                flushLastGroup();
            }
            if (!lastEventIdSecondary.empty()) {
                setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldSecondaryID(lastEventIdSecondary);
                setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldSecondaryStat(
                    SFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldSecondaryStat::OFF);
                flushLastGroup();
            }
            if (!lastEventIdThird.empty()) {
                setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldThirdID(lastEventIdThird);
                setSFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldThirdStat(
                    SFCADAS_Parking_BVM_NEWEventBVMOutMirrorFoldThirdStat::OFF);
                flushLastGroup();
            }
            lastEventIdPrimary = "";
            lastEventIdSecondary = "";
            lastEventIdThird = "";
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };
    IgnElapsed mIgnElapsed = IgnElapsed::NONE;

    using OutMirrorFoldStatusFromBCM = ArgumentsOutMirrorFoldStatusChanged::Input_OutMirrorFoldStatusFromBCM;
    OutMirrorFoldStatusFromBCM mOutMirrorFoldStatusFromBCM = OutMirrorFoldStatusFromBCM::OFF;
    using BvmView2Status = ArgumentsBvmView2StatusChanged::Input_BvmView2Status;
    BvmView2Status mBvmView2Status = BvmView2Status::OFF;
    using BvmVideo = ArgumentsInput_BvmVideoChanged::Input_BvmVideo;
    BvmVideo mBvmVideo = BvmVideo::NONE;
};

}  // namespace ccos

#endif  // SFSS_ADAS_Parking_BVM_NEW_H
