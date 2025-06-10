/**
 * @file DBC.hpp
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
#ifndef SFSS_DBC_H
#define SFSS_DBC_H

#define DLOG_ENABLED gEnableSFCLog

#include "DBCBase.hpp"

namespace ccos {

// SFC Version : 2.0.0
// Reference : [PT070] DBC. Contains : Telltale, Event. Recommend: VALUE_CHANGED
class DBC : public DBCBase {
public:
    DBC() = default;
    ~DBC() override = default;
    DBC(const DBC& other) = delete;
    DBC(DBC&& other) noexcept = delete;
    DBC& operator=(const DBC& other) = delete;
    DBC& operator=(DBC&& other) noexcept = delete;

    void onInitialize() override {
        setSFCDBCEventAutoCancelStatusID("E20303");
        setSFCDBCEventAutoCancelStatusLinkedSoundID("SND_PopUpWarn1");
        setSFCDBCEventAutoCancelStatusLinkedSoundType(SFCDBCEventAutoCancelStatusLinkedSoundType::REPEAT_COUNT);
        setSFCDBCEventAutoCancelStatusLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateTelltale();
        updateDBCOnOff();
        updateDBCAutoCanceled();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateTelltale();
        updateDBCOnOff();
        updateDBCAutoCanceled();
    }

    void onIgnElapseOn3500msChanged(const ArgumentsIgnElapseOn3500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_3500ms;
        updateTelltale();
        updateDBCOnOff();
        updateDBCAutoCanceled();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateTelltale();
        updateDBCOnOff();
        updateDBCAutoCanceled();
    }

    void onVariantChanged(const ArgumentsVariantChanged& args) {
        mInter_ConfigDBC = args.mInter_ConfigDBC;
        updateTelltale();
        updateDBCOnOff();
        updateDBCAutoCanceled();
    }

    void onFunctionWarnStatusChanged(const ArgumentsFunctionWarnStatusChanged& args) {
        if (ISTIMEOUT(args.mInput_DbcWarnStatus) == true) {
            mDbcWarnStatus = DbcWarnStatus::UNHANDLED_TIMEOUT;
        } else {
            mDbcWarnStatus = args.mInput_DbcWarnStatus;
        }
        mDbcFunctionStatus = args.mInput_DbcFunctionStatus;
        updateTelltale();
        updateDBCOnOff();

        mDbcFunctionStatusCurrent = mDbcFunctionStatus;
    }

    void onAutoCancelStatusChanged(const ArgumentsAutoCancelStatusChanged& args) {
        mDbcAutoCancelDisplayReqStatus = args.mInput_DbcAutoCancelDisplayReqStatus;
        updateDBCAutoCanceled();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        ON_3500ms,
        OFF_0ms,
        MAX
    };

    using Inter_ConfigDBC = ArgumentsVariantChanged::Inter_ConfigDBC;
    using DbcFunctionStatus = ArgumentsFunctionWarnStatusChanged::Input_DbcFunctionStatus;
    using DbcWarnStatus = ArgumentsFunctionWarnStatusChanged::Input_DbcWarnStatus;
    using DbcAutoCancelDisplayReqStatus = ArgumentsAutoCancelStatusChanged::Input_DbcAutoCancelDisplayReqStatus;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    Inter_ConfigDBC mInter_ConfigDBC = Inter_ConfigDBC::OFF;
    DbcFunctionStatus mDbcFunctionStatus = DbcFunctionStatus::OFF;
    DbcFunctionStatus mDbcFunctionStatusCurrent = DbcFunctionStatus::OFF;
    DbcWarnStatus mDbcWarnStatus = DbcWarnStatus::OFF;
    DbcAutoCancelDisplayReqStatus mDbcAutoCancelDisplayReqStatus = DbcAutoCancelDisplayReqStatus::OFF;

    void updateTelltale() {
        if (mInter_ConfigDBC == Inter_ConfigDBC::ON) {
            if (mIgnElapsed == IgnElapsed::ON_500ms) {
                setSFCDBCTelltaleDBCStat(SFCDBCTelltaleDBCStat::YELLOW);
            } else if (mIgnElapsed == IgnElapsed::ON_3500ms) {
                updateAfterPreWarn();
            } else {
                setSFCDBCTelltaleDBCStat(SFCDBCTelltaleDBCStat::OFF);
            }
        } else {
            setSFCDBCTelltaleDBCStat(SFCDBCTelltaleDBCStat::OFF);
        }
    }

    void updateAfterPreWarn() {
        if (mDbcWarnStatus == DbcWarnStatus::ON || mDbcWarnStatus == DbcWarnStatus::UNHANDLED_TIMEOUT) {
            setSFCDBCTelltaleDBCStat(SFCDBCTelltaleDBCStat::YELLOW);
        } else if (mDbcWarnStatus != DbcWarnStatus::ON) {
            if (mDbcFunctionStatus == DbcFunctionStatus::ON) {
                setSFCDBCTelltaleDBCStat(SFCDBCTelltaleDBCStat::GREEN);
            } else if (mDbcFunctionStatus == DbcFunctionStatus::BLINKING) {
                setSFCDBCTelltaleDBCStat(SFCDBCTelltaleDBCStat::BLINK1);
                setSFCDBCTelltaleDBCBlinkValueA(SFCDBCTelltaleDBCBlinkValueA::GREEN);
                setSFCDBCTelltaleDBCBlinkValueB(SFCDBCTelltaleDBCBlinkValueB::OFF);
            } else {
                setSFCDBCTelltaleDBCStat(SFCDBCTelltaleDBCStat::OFF);
            }
        } else {
            setSFCDBCTelltaleDBCStat(SFCDBCTelltaleDBCStat::OFF);
        }
    }

    void updateDBCOnOff() {
        std::string eventID;
        if (mInter_ConfigDBC == Inter_ConfigDBC::ON &&
            (mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms || mIgnElapsed == IgnElapsed::ON_3500ms) &&
            mDbcWarnStatus == DbcWarnStatus::OFF) {
            if (mDbcFunctionStatusCurrent == DbcFunctionStatus::OFF &&
                (mDbcFunctionStatus == DbcFunctionStatus::ON || mDbcFunctionStatus == DbcFunctionStatus::BLINKING)) {
                eventID = "E20301";
            } else if ((mDbcFunctionStatusCurrent == DbcFunctionStatus::ON ||
                        mDbcFunctionStatusCurrent == DbcFunctionStatus::BLINKING) &&
                       mDbcFunctionStatus == DbcFunctionStatus::OFF) {
                eventID = "E20302";
            } else {
                // nothing
            }
        }
        std::string prevEventID;
        GETCACHEDVALUE(SFC.DBC.Event.FunctionWarnStatus.ID, prevEventID);
        if (!prevEventID.empty() && prevEventID != eventID) {
            setSFCDBCEventFunctionWarnStatusStat(SFCDBCEventFunctionWarnStatusStat::OFF);
            flushLastGroup();
        }
        if (!eventID.empty()) {
            setSFCDBCEventFunctionWarnStatusID(eventID);
            setSFCDBCEventFunctionWarnStatusStat(SFCDBCEventFunctionWarnStatusStat::ON);
        }
    }

    void updateDBCAutoCanceled() {
        if (mInter_ConfigDBC == Inter_ConfigDBC::ON &&
            (mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms || mIgnElapsed == IgnElapsed::ON_3500ms) &&
            mDbcAutoCancelDisplayReqStatus == DbcAutoCancelDisplayReqStatus::ON) {
            setSFCDBCEventAutoCancelStatusStat(SFCDBCEventAutoCancelStatusStat::ON);
        } else {
            setSFCDBCEventAutoCancelStatusStat(SFCDBCEventAutoCancelStatusStat::OFF);
        }
    }
};

}  // namespace ccos

#endif  // SFSS_DBC_H
