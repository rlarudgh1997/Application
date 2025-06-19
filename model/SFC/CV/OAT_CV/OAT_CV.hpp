/**
 * @file OAT_CV.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2024  Hyundai Motor Company,
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
#ifndef SFSS_OAT_CV_H
#define SFSS_OAT_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include <cmath>
#include "OAT_CVBase.hpp"

namespace ccos {

// SFC Version : 1.0.0
// Spec Version : v0.35
// Reference : [CV610] OAT_CV. Contains : Constant, Event. Recommend: VALUE_CHANGED
class OAT_CV : public OAT_CVBase {
public:
    OAT_CV() = default;
    ~OAT_CV() override = default;
    OAT_CV(const OAT_CV& other) = delete;
    OAT_CV(OAT_CV&& other) noexcept = delete;
    OAT_CV& operator=(const OAT_CV& other) = delete;
    OAT_CV& operator=(OAT_CV&& other) noexcept = delete;

    void onInitialize() override {
        // 5.3.1 결빙 경고
        setSFCOAT_CVEventIceWarnID("E76101");
        setSFCOAT_CVEventIceWarnLinkedSoundID("SND_PopUpWarn1");
        setSFCOAT_CVEventIceWarnLinkedSoundType(SFCOAT_CVEventIceWarnLinkedSoundType::REPEAT_COUNT);
        setSFCOAT_CVEventIceWarnLinkedSoundRepeatCount(1);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;

        updateConstantOATValue();
        updateConstantOATStat();
        updateConstantOATUnitStat();
        updatePrivateICEWarnStat();
        updateConstantIceWarnStat();
        updateEventIceWarn();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;

        updateConstantOATValue();
        updateConstantOATStat();
        updateConstantOATUnitStat();
        updatePrivateICEWarnStat();
        updateConstantIceWarnStat();
        updateEventIceWarn();
    }

    void onOatCelsiusValueFATCChanged(const ArgumentsOatCelsiusValueFATCChanged& args) {
        mOATCelsiusValue_FATC = args.mInput_OATCelsiusValue_FATC;

        updateConstantOATValue();
        updateConstantOATStat();
        updatePrivateICEWarnStat();
        updateConstantIceWarnStat();
        updateEventIceWarn();
    }

    void onOatFahrenheitValueFATCChanged(const ArgumentsOatFahrenheitValueFATCChanged& args) {
        mOATFahrenheitValue_FATC = args.mInput_OATFahrenheitValue_FATC;

        updateConstantOATValue();
        updateConstantOATStat();
        updatePrivateICEWarnStat();
        updateConstantIceWarnStat();
        updateEventIceWarn();
    }

    void onOatUnitStatusFATCChanged(const ArgumentsOatUnitStatusFATCChanged& args) {
        mOATUnitStatus_FATC = args.mInput_OATUnitStatus_FATC;

        updateConstantOATValue();
        updateConstantOATUnitStat();
        updatePrivateICEWarnStat();
        updateConstantIceWarnStat();
        updateEventIceWarn();
    }

    void onOatCelsiusValueCCMChanged(const ArgumentsOatCelsiusValueCCMChanged& args) {
        mOATCelsiusValue_CCM = args.mInput_OATCelsiusValue_CCM;

        updateConstantOATValue();
        updateConstantOATStat();
        updateConstantOATUnitStat();
        updatePrivateICEWarnStat();
        updateConstantIceWarnStat();
        updateEventIceWarn();
    }

    void onOatCelsiusValueACPChanged(const ArgumentsOatCelsiusValueACPChanged& args) {
        mOATCelsiusValue_ACP = args.mInput_OATCelsiusValue_ACP;

        updateConstantOATValue();
        updateConstantOATStat();
        updateConstantOATUnitStat();
        updatePrivateICEWarnStat();
        updateConstantIceWarnStat();
        updateEventIceWarn();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // Private
    // 5.40.4 연산 로직
    void updatePrivateICEWarnStat() {
        mPrivate_ICEWarnStat = Private_ICEWarnStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mIMG_OATUnit_stat == SFCOAT_CVConstantOATUnitStat::CELSIUS) {
                if (mOATCelsiusValue_FATC <= 90 || mOATCelsiusValue_CCM <= 89 || mOATCelsiusValue_ACP <= 88) {
                    mPrivate_ICEWarnStat = Private_ICEWarnStat::ON;
                } else if (mOATCelsiusValue_FATC == 91 || mOATCelsiusValue_FATC == 92 || mOATCelsiusValue_CCM == 90 ||
                           mOATCelsiusValue_CCM == 91 || mOATCelsiusValue_ACP == 89 || mOATCelsiusValue_ACP == 90) {
                    mPrivate_ICEWarnStat = Private_ICEWarnStat::KEEP_CUR_STAT;
                } else {
                    // no operation
                }
            } else if (mIMG_OATUnit_stat == SFCOAT_CVConstantOATUnitStat::FAHRENHEIT) {
                if (mOATFahrenheitValue_FATC <= 81) {
                    mPrivate_ICEWarnStat = Private_ICEWarnStat::ON;
                } else if (mOATFahrenheitValue_FATC == 82 || mOATFahrenheitValue_FATC == 83 || mOATFahrenheitValue_FATC == 84) {
                    mPrivate_ICEWarnStat = Private_ICEWarnStat::KEEP_CUR_STAT;
                } else {
                    // no operation
                }
            } else {
                // no operation
            }
        }

#ifdef ENABLE_GCOV_ON
        setSFCPrivateOAT_CVICEWarnStat(static_cast<SFCPrivateOAT_CVICEWarnStat>(mPrivate_ICEWarnStat));
#endif
    }

    // Constant
    // 5.2.1 외기온 값 표시
    void updateConstantOATValue() {
        HInt64 value = 0xFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mOATUnitStatus_FATC == OATUnitStatus_FATC::CELSIUS && mOATCelsiusValue_FATC != 0xFE &&
                mOATCelsiusValue_FATC != 0xFF) {
                value = static_cast<HInt64>(lround(static_cast<HDouble>(mOATCelsiusValue_FATC) * 0.5 - 41.0));
            } else if (mOATUnitStatus_FATC == OATUnitStatus_FATC::FAHRENHEIT && mOATFahrenheitValue_FATC != 0xFE &&
                       mOATFahrenheitValue_FATC != 0xFF) {
                value = static_cast<HInt64>(mOATFahrenheitValue_FATC - 42);
            } else if (mOATUnitStatus_FATC == OATUnitStatus_FATC::MESSAGE_TIMEOUT && mOATCelsiusValue_CCM != 0xFE &&
                       mOATCelsiusValue_CCM != 0xFF && (mOATCelsiusValue_ACP == 0xFE || mOATCelsiusValue_ACP == 0xFF)) {
                value = static_cast<HInt64>(lround(static_cast<HDouble>(mOATCelsiusValue_CCM) * 0.5 - 40.5));
            } else if (mOATUnitStatus_FATC == OATUnitStatus_FATC::MESSAGE_TIMEOUT &&
                       (mOATCelsiusValue_CCM == 0xFE || mOATCelsiusValue_CCM == 0xFF) && mOATCelsiusValue_ACP != 0xFE &&
                       mOATCelsiusValue_ACP != 0xFF) {
                value = static_cast<HInt64>(lround(static_cast<HDouble>(mOATCelsiusValue_ACP) * 0.5 - 40.0));
            } else {
                // no operation
            }
        }
        setSFCOAT_CVConstantOATValue(value);
    }

    // 5.2.2 외기온 상태 표시
    void updateConstantOATStat() {
        SFCOAT_CVConstantOATStat stat = SFCOAT_CVConstantOATStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if ((mOATCelsiusValue_FATC != 0xFE && mOATCelsiusValue_FATC != 0xFF) ||
                (mOATFahrenheitValue_FATC != 0xFE && mOATFahrenheitValue_FATC != 0xFF) ||
                (mOATCelsiusValue_CCM != 0xFE && mOATCelsiusValue_CCM != 0xFF) ||
                (mOATCelsiusValue_ACP != 0xFE && mOATCelsiusValue_ACP != 0xFF)) {
                stat = SFCOAT_CVConstantOATStat::ON;
            }
        }
        setSFCOAT_CVConstantOATStat(stat);
    }

    // 5.2.3 외기온 단위 표시
    void updateConstantOATUnitStat() {
        SFCOAT_CVConstantOATUnitStat stat = SFCOAT_CVConstantOATUnitStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mOATUnitStatus_FATC == OATUnitStatus_FATC::CELSIUS ||
                (mOATUnitStatus_FATC == OATUnitStatus_FATC::MESSAGE_TIMEOUT &&
                 ((mOATCelsiusValue_CCM != 0xFE && mOATCelsiusValue_CCM != 0xFF) ||
                  (mOATCelsiusValue_ACP != 0xFE && mOATCelsiusValue_ACP != 0xFF)))) {
                stat = SFCOAT_CVConstantOATUnitStat::CELSIUS;
            } else if (mOATUnitStatus_FATC == OATUnitStatus_FATC::FAHRENHEIT) {
                stat = SFCOAT_CVConstantOATUnitStat::FAHRENHEIT;
            } else {
                // no operation
            }
        }
        mIMG_OATUnit_stat = stat;
        setSFCOAT_CVConstantOATUnitStat(stat);
    }

    // 5.2.4 결빙 경고 아이콘 표시
    void updateConstantIceWarnStat() {
        SFCOAT_CVConstantICEWarnStat stat = SFCOAT_CVConstantICEWarnStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mPrivate_ICEWarnStat == Private_ICEWarnStat::ON) {
                stat = SFCOAT_CVConstantICEWarnStat::ON;
            } else if (mPrivate_ICEWarnStat == Private_ICEWarnStat::KEEP_CUR_STAT) {
                stat = mIMG_ICEWarn_stat;
            } else {
                // no operation
            }
        }

        mIMG_ICEWarn_stat = stat;
        setSFCOAT_CVConstantICEWarnStat(stat);
    }

    // Event
    // 5.3.1 결빙 경고
    void updateEventIceWarn() {
        SFCOAT_CVEventIceWarnStat stat = SFCOAT_CVEventIceWarnStat::OFF;

        if (mIgnElapsed == IgnElapsed::ON_0ms) {
            if (mIMG_ICEWarn_stat == SFCOAT_CVConstantICEWarnStat::ON) {
                stat = SFCOAT_CVEventIceWarnStat::ON;
            }
        }
        setSFCOAT_CVEventIceWarnStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    enum class Private_ICEWarnStat : HUInt64 {
        NONE,
        OFF,
        ON,
        KEEP_CUR_STAT,
        MAX
    };

    using OATUnitStatus_FATC = ArgumentsOatUnitStatusFATCChanged::Input_OATUnitStatus_FATC;

    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    Private_ICEWarnStat mPrivate_ICEWarnStat = Private_ICEWarnStat::OFF;
    HUInt64 mOATCelsiusValue_FATC = 0xFF;
    HUInt64 mOATFahrenheitValue_FATC = 0xFF;
    OATUnitStatus_FATC mOATUnitStatus_FATC = OATUnitStatus_FATC::OFF;
    HUInt64 mOATCelsiusValue_CCM = 0xFF;
    HUInt64 mOATCelsiusValue_ACP = 0xFF;
    SFCOAT_CVConstantOATUnitStat mIMG_OATUnit_stat = SFCOAT_CVConstantOATUnitStat::OFF;
    SFCOAT_CVConstantICEWarnStat mIMG_ICEWarn_stat = SFCOAT_CVConstantICEWarnStat::OFF;
};

}  // namespace ccos

#endif  // SFSS_OAT_CV_H
