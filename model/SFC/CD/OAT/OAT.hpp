/**
 * @file OAT.hpp
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
#ifndef SFSS_OAT_H
#define SFSS_OAT_H

#define DLOG_ENABLED gEnableSFCLog

#include "OATBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 8.0.2
// Reference : [CD090] OAT. Contains : Telltale, Constant, Event. Recommend: VALUE_CHANGED
class OAT : public OATBase {
public:
    OAT() = default;
    ~OAT() override = default;
    OAT(const OAT& other) = delete;
    OAT(OAT&& other) noexcept = delete;
    OAT& operator=(const OAT& other) = delete;
    OAT& operator=(OAT&& other) noexcept = delete;

    void onInitialize() override {
        if (mOneShotTimerIceWarnBlinkOn5sStatus.create(kTimerAfterIceWarnBlinkOnInterval5s, this,
                                                       &OAT::onOneShotTimerIceWarnBlinkOn5s, false) == false) {
            DWarning() << "[OAT] mOneShotTimerIceWarnBlinkOn5sStatus Create Failed";
        }

        if (mOneShotTimerIceWarnDisplayTempOff400msStatus.create(kTimerAfterIceWarnDisplayTempOffInterval400ms, this,
                                                                 &OAT::onOneShotTimerIceWarnDisplayTempOff400ms,
                                                                 false) == false) {
            DWarning() << "[OAT] mOneShotTimerIceWarnDisplayTempOff400msStatus Create Failed";
        }

        if (mOneShotTimerIceWarnDisplayWarnOn600msStatus.create(kTimerAfterIceWarnDisplayWarnOnInterval600ms, this,
                                                                &OAT::onOneShotTimerIceWarnDisplayWarnOn600ms, false) == false) {
            DWarning() << "[OAT] mOneShotTimerIceWarnDisplayWarnOn600msStatus Create Failed";
        }
        setSFCOATEventIceWarnID("E00901");
        setSFCOATEventIceWarnLinkedSoundID("SND_PopUpInform2");
        setSFCOATEventIceWarnLinkedSoundType(SFCOATEventIceWarnLinkedSoundType::REPEAT_COUNT);
        setSFCOATEventIceWarnLinkedSoundRepeatCount(1);
        setSFCOATTelltaleIceWarnStatOptional(SFCOATTelltaleIceWarnStatOptional::WARN_ON);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        updatePrivateOutTempDisplayValue();
        // 이 두 함수는 의도적으로 제거합니다.
        // SFC.Extension.Event_Control_Logic.Inter_SystemCheckDisplayStatus 노드는
        // Intro_Outro의 Event를 전달 받아 App에서 표시가 발화되는 기준으로 설정해 줍니다.
        // IGN OFF 및 초기값이 결정될때 Inter_SystemCheckDisplayStatus는 OFF로 결정되는데
        // IGN ON에서는 아래 2 함수가 불릴 경우 OAT에서 팝업을 발생시킵니다. (사양 위배)
        // 하지만 IGN ON에 의해 Intro Outro가 App에 Event를 요청하고, App은
        // Inter_SystemCheckDisplayStatus를 ON으로 설정하는데 이는 OAT 팝업 조건이 아닙니다.
        // loop back이 형성되는데 따라서 본 함수의 call slot에서는 아래 2함수를 호출하지 않고
        // Event의 발화는 본 함수에서 IGN 상태만 변경해 놓고 onSystemCheckDisplayStatusChanged에 의해서 발화 시키는 것으로
        // 처리됩니다.
        // updatePrivateOutTempDisplayAvailable();
        // updateEvent();
        updatePrivateIceWarnStatus();
        DDebug() << "flushIGNon";
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updatePrivateOutTempDisplayValue();
        updatePrivateOutTempDisplayAvailable();
        updatePrivateIceWarnStatus();
        DDebug() << "flushIGNoff";
    }

    void onOutTempDisplayChanged(const ArgumentsOutTempDisplayChanged& args) {
        mOutTempCelsiusDisplayValue = args.mInput_OutTempCelsiusDisplayValue;
        mOutTempFahrenheitDisplayValue = args.mInput_OutTempFahrenheitDisplayValue;
        updatePrivateOutTempDisplayValue();
        updatePrivateIceWarnStatus();
        DDebug() << "flushChangedouttemp";
    }

    void onSystemCheckDisplayStatusChanged(const ArgumentsSystemCheckDisplayStatusChanged& args) {
        mSystemCheckDisplayStatus = args.mInter_SystemCheckDisplayStatus;
        updatePrivateOutTempDisplayAvailable();
        updatePrivateIceWarnStatus();
        DDebug() << "flushSystemCheck";
    }

    void onOutTempUnitChanged(const ArgumentsOutTempUnitChanged& args) {
        mOutTempUnitStatus = args.mInput_OutTempUnitStatus;
        updatePrivateIceWarnStatus();
        DDebug() << "flushUnit";
    }

    void onVariantChanged(const ArgumentsVariantChanged& args) {
        mConfigOAT = args.mInter_ConfigOAT;

        if (mConfigOAT != Inter_ConfigOAT::ON) {
            setAllSignalOff();
            enqueue();
        }
        setBlocked(mConfigOAT != Inter_ConfigOAT::ON);

        updatePrivateOutTempDisplayValue();
        updatePrivateOutTempDisplayAvailable();
        updatePrivateIceWarnStatus();
        DDebug() << "flushVariant";
    }

    void onInterIceWarnSetStatusChanged(ArgumentsInterIceWarnSetStatusChanged& args) {
        updateValueIfValid(mInter_IceWarnSetStatus, args.mInter_IceWarnSetStatus);
        // mInter_IceWarnSetStatus = args.mInter_IceWarnSetStatus;

        DDebug() << "onInterIceWarnSetStatusChanged(), " << static_cast<uint64_t>(mInter_IceWarnSetStatus);
        updatePrivateIceWarnStatus();
    }

    void dump(std::ostringstream& out, const std::string& prefix, const std::string& command) override {
        // no operation
    }

private:
    void setAllSignalOff() {
        setSFCOATConstantOutTempDisplayStat("DISPLAY_OFF");
        setSFCOATConstantOutTempDisplayUnitStat(SFCOATConstantOutTempDisplayUnitStat::DISPLAY_OFF);
        setSFCOATTelltaleIceWarnStat(SFCOATTelltaleIceWarnStat::OFF);
        setSFCOATEventIceWarnStat(SFCOATEventIceWarnStat::OFF);
    }

    void updatePrivateOutTempDisplayValue() {
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mOutTempCelsiusDisplayValue > 0x01 && mOutTempCelsiusDisplayValue < 0xfe) {
                mPrivate_OutTempCelsiusValueStatus = Private_OutTempValueStatus::VALID;
                mPrivate_OutTempCelsiusValue = round(static_cast<HDouble>(mOutTempCelsiusDisplayValue) / 2 - 41);  // Round off
            } else {
                mPrivate_OutTempCelsiusValueStatus = Private_OutTempValueStatus::INVALID;
                mPrivate_OutTempCelsiusValue = 0xff;
            }
            if (mOutTempFahrenheitDisplayValue > 0x01 && mOutTempFahrenheitDisplayValue < 0xfe) {
                mPrivate_OutTempFahrenheitValueStatus = Private_OutTempValueStatus::VALID;
                mPrivate_OutTempFahrenheitValue = mOutTempFahrenheitDisplayValue - 42;
            } else {
                mPrivate_OutTempFahrenheitValueStatus = Private_OutTempValueStatus::INVALID;
                mPrivate_OutTempFahrenheitValue = 0xff;
            }
        } else {
            mPrivate_OutTempCelsiusValueStatus = Private_OutTempValueStatus::INVALID;
            mPrivate_OutTempCelsiusValue = 0xff;
            mPrivate_OutTempFahrenheitValueStatus = Private_OutTempValueStatus::INVALID;
            mPrivate_OutTempFahrenheitValue = 0xff;
        }
    }

    void updatePrivateOutTempDisplayAvailable() {
        if (mIgnElapsed == IgnElapsed::ON_0ms && mSystemCheckDisplayStatus == SystemCheckDisplayStatus::OFF) {
            mPrivate_OutTempDisplayAvailable = Private_OutTempDisplayAvailable::ENABLE;
            DDebug() << "DisplayEnable";
        } else {
            mPrivate_OutTempDisplayAvailable = Private_OutTempDisplayAvailable::DISABLE;
            DDebug() << "DisplayDisable";
        }
    }

    void onOneShotTimerIceWarnBlinkOn5s() {
        mIs5sAfterIceWarnBlinkOn = true;
        updatePrivateIceWarnStatus();
        flush();
    }

    void updatePrivateIceWarnStatus() {
        mPrev_Private_IceWarnStatus = mPrivate_IceWarnStatus;
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mPrivate_IceWarnStatus == Private_IceWarnStatus::OFF &&
                ((mInter_IceWarnSetStatus == Inter_IceWarnSetStatus::ENABLE) &&
                 (mPrivate_OutTempDisplayAvailable == Private_OutTempDisplayAvailable::ENABLE) &&
                 ((mOutTempUnitStatus == OutTempUnitStatus::CELSIUS && mPrivate_OutTempCelsiusValue <= 4) ||
                  (mOutTempUnitStatus == OutTempUnitStatus::FAHRENHEIT && mPrivate_OutTempFahrenheitValue <= 39)))) {
                mPrivate_IceWarnStatus = Private_IceWarnStatus::BLINK_ON;
                if (mOneShotTimerIceWarnBlinkOn5sStatus.start() == false) {
                    DWarning() << "[OAT] mOneShotTimerIceWarnBlinkOn5sStatus Start Failed";
                }
            } else if (mPrivate_IceWarnStatus == Private_IceWarnStatus::BLINK_ON && mIs5sAfterIceWarnBlinkOn == true &&
                       ((mInter_IceWarnSetStatus == Inter_IceWarnSetStatus::ENABLE) &&
                        (mPrivate_OutTempDisplayAvailable == Private_OutTempDisplayAvailable::ENABLE) &&
                        ((mOutTempUnitStatus == OutTempUnitStatus::CELSIUS && mPrivate_OutTempCelsiusValue < 6) ||
                         (mOutTempUnitStatus == OutTempUnitStatus::FAHRENHEIT && mPrivate_OutTempFahrenheitValue < 43)))) {
                mPrivate_IceWarnStatus = Private_IceWarnStatus::WARN_ON;
            } else if (mPrivate_IceWarnStatus != Private_IceWarnStatus::OFF &&
                       ((mOutTempUnitStatus == OutTempUnitStatus::CELSIUS && mPrivate_OutTempCelsiusValue >= 6) ||
                        (mOutTempUnitStatus == OutTempUnitStatus::FAHRENHEIT && mPrivate_OutTempFahrenheitValue >= 43) ||
                        (mInter_IceWarnSetStatus == Inter_IceWarnSetStatus::DISABLE) ||
                        (mPrivate_OutTempDisplayAvailable == Private_OutTempDisplayAvailable::DISABLE))) {
                mPrivate_IceWarnStatus = Private_IceWarnStatus::OFF;
                mOneShotTimerIceWarnBlinkOn5sStatus.stop();
                mIs5sAfterIceWarnBlinkOn = false;
            } else {
                // no operation
            }
        } else {
            mPrivate_IceWarnStatus = Private_IceWarnStatus::OFF;
            mOneShotTimerIceWarnBlinkOn5sStatus.stop();
            mIs5sAfterIceWarnBlinkOn = false;
        }

        updatePrivateIceWarnDisplayStatus();
    }

    void onOneShotTimerIceWarnDisplayTempOff400ms() {
        mIs400msAfterTempOff = true;
        updatePrivateIceWarnDisplayStatus();
        flush();
    }

    void onOneShotTimerIceWarnDisplayWarnOn600ms() {
        mIs600msAfterWarnOn = true;
        updatePrivateIceWarnDisplayStatus();
        flush();
    }

    void updatePrivateIceWarnDisplayStatus() {
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if ((mPrivate_IceWarnDisplayStatus == Private_IceWarnDisplayStatus::WARN_OFF) &&
                (mPrev_Private_IceWarnStatus == Private_IceWarnStatus::OFF) &&
                (mPrivate_IceWarnStatus == Private_IceWarnStatus::BLINK_ON)) {
                mPrivate_IceWarnDisplayStatus = Private_IceWarnDisplayStatus::WARN_ON;
                mIs600msAfterWarnOn = false;
                mIs400msAfterTempOff = false;
                mOneShotTimerIceWarnDisplayTempOff400msStatus.stop();
                if (mOneShotTimerIceWarnDisplayWarnOn600msStatus.start() == false) {
                    DWarning() << "[OAT] mOneShotTimerIceWarnDisplayWarnOn600msStatus Start Failed";
                }
            } else if ((mPrivate_IceWarnDisplayStatus == Private_IceWarnDisplayStatus::TEMP_OFF) &&
                       (mIs400msAfterTempOff == true) &&
                       (mPrivate_IceWarnStatus == Private_IceWarnStatus::BLINK_ON ||
                        mPrivate_IceWarnStatus == Private_IceWarnStatus::WARN_ON)) {
                mPrivate_IceWarnDisplayStatus = Private_IceWarnDisplayStatus::WARN_ON;
                mIs600msAfterWarnOn = false;
                mIs400msAfterTempOff = false;
                mOneShotTimerIceWarnDisplayTempOff400msStatus.stop();
                if (mOneShotTimerIceWarnDisplayWarnOn600msStatus.start() == false) {
                    DWarning() << "[OAT] mOneShotTimerIceWarnDisplayWarnOn600msStatus Start Failed";
                }
            } else if ((mPrivate_IceWarnDisplayStatus == Private_IceWarnDisplayStatus::WARN_ON) &&
                       (mIs600msAfterWarnOn == true) && (mPrivate_IceWarnStatus == Private_IceWarnStatus::BLINK_ON)) {
                mPrivate_IceWarnDisplayStatus = Private_IceWarnDisplayStatus::TEMP_OFF;
                mIs600msAfterWarnOn = false;
                mIs400msAfterTempOff = false;
                mOneShotTimerIceWarnDisplayWarnOn600msStatus.stop();
                if (mOneShotTimerIceWarnDisplayTempOff400msStatus.start() == false) {
                    DWarning() << "[OAT] mOneShotTimerIceWarnDisplayTempOff400msStatus Start Failed";
                }
            } else if ((mPrivate_IceWarnDisplayStatus != Private_IceWarnDisplayStatus::WARN_OFF) &&
                       (mPrev_Private_IceWarnStatus != Private_IceWarnStatus::OFF) &&
                       (mPrivate_IceWarnStatus == Private_IceWarnStatus::OFF)) {
                mPrivate_IceWarnDisplayStatus = Private_IceWarnDisplayStatus::WARN_OFF;
                mIs600msAfterWarnOn = false;
                mIs400msAfterTempOff = false;
                mOneShotTimerIceWarnDisplayWarnOn600msStatus.stop();
                mOneShotTimerIceWarnDisplayTempOff400msStatus.stop();
            } else {
                // no operation
            }
        } else {
            mPrivate_IceWarnDisplayStatus = Private_IceWarnDisplayStatus::WARN_OFF;
            mIs600msAfterWarnOn = false;
            mIs400msAfterTempOff = false;
            mOneShotTimerIceWarnDisplayWarnOn600msStatus.stop();
            mOneShotTimerIceWarnDisplayTempOff400msStatus.stop();
        }

        updateTelltale();
        updateEvent();
        updateConstants();
    }

    void updateTelltale() {
        if (mIgnElapsed == IgnElapsed::ON_0ms && mPrivate_IceWarnDisplayStatus == Private_IceWarnDisplayStatus::WARN_ON) {
            setSFCOATTelltaleIceWarnStat(SFCOATTelltaleIceWarnStat::OPTIONAL);
            // setSFCOATTelltaleWarnStat(SFCOATTelltaleWarnStat::WARN_ON);
        } else {
            setSFCOATTelltaleIceWarnStat(SFCOATTelltaleIceWarnStat::OFF);
            // setSFCOATTelltaleWarnStat(SFCOATTelltaleWarnStat::OFF);
        }
    }

    void updateEvent() {
        if (mIgnElapsed == IgnElapsed::ON_0ms &&
            ((mInter_IceWarnSetStatus == Inter_IceWarnSetStatus::ENABLE &&
              mPrivate_OutTempDisplayAvailable == Private_OutTempDisplayAvailable::ENABLE &&
              mOutTempUnitStatus == OutTempUnitStatus::CELSIUS && (mPrivate_OutTempCelsiusValue <= 4)) ||
             (mInter_IceWarnSetStatus == Inter_IceWarnSetStatus::ENABLE &&
              mPrivate_OutTempDisplayAvailable == Private_OutTempDisplayAvailable::ENABLE &&
              mOutTempUnitStatus == OutTempUnitStatus::FAHRENHEIT && (mPrivate_OutTempFahrenheitValue <= 39)))) {
            setSFCOATEventIceWarnStat(SFCOATEventIceWarnStat::ON);
        } else {
            setSFCOATEventIceWarnStat(SFCOATEventIceWarnStat::OFF);
        }
    }

    void updateConstants() {
        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mOutTempUnitStatus == OutTempUnitStatus::CELSIUS &&
                mPrivate_IceWarnDisplayStatus != Private_IceWarnDisplayStatus::TEMP_OFF &&
                mPrivate_OutTempCelsiusValueStatus != Private_OutTempValueStatus::INVALID) {
                setSFCOATConstantOutTempDisplayStat(std::to_string(mPrivate_OutTempCelsiusValue));
                setSFCOATConstantOutTempDisplayUnitStat(SFCOATConstantOutTempDisplayUnitStat::CELSIUS);
                DDebug() << "ValueCELSIUS: " << mPrivate_OutTempCelsiusValue;
            } else if (mOutTempUnitStatus == OutTempUnitStatus::FAHRENHEIT &&
                       mPrivate_IceWarnDisplayStatus != Private_IceWarnDisplayStatus::TEMP_OFF &&
                       mPrivate_OutTempFahrenheitValueStatus != Private_OutTempValueStatus::INVALID) {
                setSFCOATConstantOutTempDisplayStat(std::to_string(mPrivate_OutTempFahrenheitValue));
                setSFCOATConstantOutTempDisplayUnitStat(SFCOATConstantOutTempDisplayUnitStat::FAHRENHEIT);
                DDebug() << "FAHRENHEIT";
            } else {
                setSFCOATConstantOutTempDisplayStat("DISPLAY_OFF");
                setSFCOATConstantOutTempDisplayUnitStat(SFCOATConstantOutTempDisplayUnitStat::DISPLAY_OFF);
                DDebug() << "DisplayOff";
            }
        } else {
            setSFCOATConstantOutTempDisplayStat("DISPLAY_OFF");
            setSFCOATConstantOutTempDisplayUnitStat(SFCOATConstantOutTempDisplayUnitStat::DISPLAY_OFF);
            DDebug() << "DisplayOff2";
        }
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };
    IgnElapsed mIgnElapsed = IgnElapsed::NONE;

    enum class Private_OutTempValueStatus : HUInt64 {
        INVALID,
        VALID
    };

    enum class Private_OutTempDisplayAvailable : HUInt64 {
        DISABLE,
        ENABLE
    };

    enum class Private_IceWarnStatus : HUInt64 {
        BLINK_ON,
        WARN_ON,
        OFF
    };

    enum class Private_IceWarnDisplayStatus : HUInt64 {
        WARN_ON,
        TEMP_OFF,
        WARN_OFF
    };

    Private_OutTempValueStatus mPrivate_OutTempCelsiusValueStatus = Private_OutTempValueStatus::INVALID;
    Private_OutTempValueStatus mPrivate_OutTempFahrenheitValueStatus = Private_OutTempValueStatus::INVALID;
    Private_OutTempDisplayAvailable mPrivate_OutTempDisplayAvailable = Private_OutTempDisplayAvailable::DISABLE;
    Private_IceWarnStatus mPrivate_IceWarnStatus = Private_IceWarnStatus::OFF;
    Private_IceWarnStatus mPrev_Private_IceWarnStatus = Private_IceWarnStatus::OFF;
    Private_IceWarnDisplayStatus mPrivate_IceWarnDisplayStatus = Private_IceWarnDisplayStatus::WARN_OFF;
    bool mIs5sAfterIceWarnBlinkOn = false;
    bool mIs400msAfterTempOff = false;
    bool mIs600msAfterWarnOn = false;

    using Inter_ConfigOAT = ArgumentsVariantChanged::Inter_ConfigOAT;
    Inter_ConfigOAT mConfigOAT = Inter_ConfigOAT::OFF;
    using SystemCheckDisplayStatus = ArgumentsSystemCheckDisplayStatusChanged::Inter_SystemCheckDisplayStatus;
    SystemCheckDisplayStatus mSystemCheckDisplayStatus = SystemCheckDisplayStatus::ON;
    using OutTempUnitStatus = ArgumentsOutTempUnitChanged::Input_OutTempUnitStatus;
    OutTempUnitStatus mOutTempUnitStatus = OutTempUnitStatus::ERROR;
    using Inter_IceWarnSetStatus = ArgumentsInterIceWarnSetStatusChanged::Inter_IceWarnSetStatus;
    Inter_IceWarnSetStatus mInter_IceWarnSetStatus = Inter_IceWarnSetStatus::NONE;

    HUInt64 mOutTempCelsiusDisplayValue = 0xff;
    HInt64 mPrivate_OutTempCelsiusValue = 0xff;
    HUInt64 mOutTempFahrenheitDisplayValue = 0xff;
    HInt64 mPrivate_OutTempFahrenheitValue = 0xff;

    ssfs::SFCTimer<OAT> mOneShotTimerIceWarnBlinkOn5sStatus;
    static constexpr uint32_t kTimerAfterIceWarnBlinkOnInterval5s = 5000u;

    ssfs::SFCTimer<OAT> mOneShotTimerIceWarnDisplayTempOff400msStatus;
    static constexpr uint32_t kTimerAfterIceWarnDisplayTempOffInterval400ms = 400u;

    ssfs::SFCTimer<OAT> mOneShotTimerIceWarnDisplayWarnOn600msStatus;
    static constexpr uint32_t kTimerAfterIceWarnDisplayWarnOnInterval600ms = 600u;
};

}  // namespace ccos

#endif  // SFSS_OAT_H
