/**
 * @file ADM_Mode_Information.hpp
 * @author Infotainment Software Development Team
 *
 * Copyright (c) 2025  Hyundai Motor Company,
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
#ifndef SFSS_ADM_Mode_Information_H
#define SFSS_ADM_Mode_Information_H

#define DLOG_ENABLED gEnableSFCLog

#include "ADM_Mode_InformationBase.hpp"

#include <Vehicle.h>
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 1.0.0
// Reference : [WYM010] ADM_Mode_Information. Contains : Constant, Event. Recommend: VALUE_CHANGED
class ADM_Mode_Information : public ADM_Mode_InformationBase {
public:
    ADM_Mode_Information() = default;
    ~ADM_Mode_Information() override = default;
    ADM_Mode_Information(const ADM_Mode_Information& other) = delete;
    ADM_Mode_Information(ADM_Mode_Information&& other) noexcept = delete;
    ADM_Mode_Information& operator=(const ADM_Mode_Information& other) = delete;
    ADM_Mode_Information& operator=(ADM_Mode_Information&& other) noexcept = delete;

    void onInitialize() override {
        setSFCADM_Mode_InformationEventEW0001ID("EW0001");
        setSFCADM_Mode_InformationEventEW0001LinkedSoundID("SND_PopUpWarn1");
        setSFCADM_Mode_InformationEventEW0001LinkedSoundType(SFCADM_Mode_InformationEventEW0001LinkedSoundType::REPEAT_COUNT);
        setSFCADM_Mode_InformationEventEW0001LinkedSoundRepeatCount(1);

        if (mOneShotTimerDebounceTime.create(kTimerInterval5000ms, this, &ADM_Mode_Information::onOneShotTimerDebounceTimeElapsed,
                                             false) == false) {
            DWarning() << "[ADM_Mode_Information] mOneShotTimerDebounceTime Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIsIgnOn = true;
        mIgnElapsed = IgnElapsed::ON_0ms;
        updateInterAdmModeStatus();
        updateAnimationStatusFromSDS();
        updateEvent1();
        mWasIgnOn = true;
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIsIgnOn = true;
        mIgnElapsed = IgnElapsed::ON_500ms;
        updateInterAdmModeStatus();
        updateEvent1();
        mWasIgnOn = true;
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIsIgnOn = false;
        mIgnElapsed = IgnElapsed::OFF_0ms;
        updateInterAdmModeStatus();
        updateAnimationStatusFromSDS();
        updateEvent1();
        if (mAEMInitializeComplete == true) {
            updateValue("SFC.Extension.Memory_IGN.Inter_AdmModeStatus1", static_cast<HUInt64>(mInter_AdmModeStatus1));
            updateValue("SFC.Extension.Memory_IGN.Inter_AdmModeStatus2", static_cast<HUInt64>(mInter_AdmModeStatus2));
        }
        mWasIgnOn = false;
    }

    void onConfigWaymoSDSAppliedChanged(const ArgumentsConfigWaymoSDSAppliedChanged& args) {
        mConfigWaymoSDSApplied = args.mInter_ConfigWaymoSDSApplied;

        updateInterAdmModeStatus();
        updateAnimationStatusFromSDS();
        updateEvent1();
    }

    void onAdmModeStatusChanged(const ArgumentsAdmModeStatusChanged& args) {
        if (ISINITIALIZEDSIGNAL(args.mInput_AdmModeStatus) == true) {
            if (ISTIMEOUT(args.mInput_AdmModeStatus) == true) {
                mAdmModeStatus = AdmModeStatus::UNHANDLED_TIMEOUT;
            } else {
                mAdmModeStatus = args.mInput_AdmModeStatus;
            }
            updateInterAdmModeStatus();
            mPrevAdmModeStatus = mAdmModeStatus;
        }
    }

    void onAdm3VisualSelectStatusChanged(const ArgumentsAdm3VisualSelectStatusChanged& args) {
        mAdm3VisualSelectStatus = args.mInput_Adm3VisualSelectStatus;
        updateConstantAdm3DisplayMode();
    }

    void onAnimationStatusFromSDSChanged(const ArgumentsAnimationStatusFromSDSChanged& args) {
        mAnimationStatusFromSDS = args.mInput_Adm3AnimationStatusFromSDS;
        updateAnimationStatusFromSDS();
    }

    void onAdm0PropulsionSystemStatusChanged(const ArgumentsAdm0PropulsionSystemStatusChanged& args) {
        mAdm0PropulsionSystemStatus = args.mInput_Adm0PropulsionSystemStatus;
        updateEvent1();
    }

    void onAEMInitializeCompleteChanged(const ArgumentsAEMInitializeCompleteChanged& args) {
        if (updateValueIfValid(mAEMInitializeComplete, args.mInter_AEMInitializeComplete) == true) {
            HUInt64 AdmModeStatus1 = 0;
            GETCACHEDVALUE(SFC.Extension.Memory_IGN.Inter_AdmModeStatus1, AdmModeStatus1);

            HUInt64 AdmModeStatus2 = 0;
            GETCACHEDVALUE(SFC.Extension.Memory_IGN.Inter_AdmModeStatus2, AdmModeStatus2);

            if (static_cast<SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1>(AdmModeStatus1) !=
                    SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::NONE &&
                static_cast<SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1>(AdmModeStatus1) !=
                    SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::MAX &&
                static_cast<SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2>(AdmModeStatus2) !=
                    SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2::NONE &&
                static_cast<SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2>(AdmModeStatus2) !=
                    SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2::MAX) {
                mInter_AdmModeStatus1 = static_cast<SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1>(AdmModeStatus1);
                mInter_AdmModeStatus2 = static_cast<SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2>(AdmModeStatus2);
                setSFCADM_Mode_InformationConstantInter_AdmModeStatusStat1(mInter_AdmModeStatus1);
                setSFCADM_Mode_InformationConstantInter_AdmModeStatusStat2(mInter_AdmModeStatus2);

                updateOutputClusterAdmModeStatus();
                updateOutputClusterAdmModeFailStatus();
                updateConstantAdmMode();
                updateConstantAdm3DisplayMode();
            }
        }
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // Timer timeout function
    void onOneShotTimerDebounceTimeElapsed() {
        mIsDebounceTimeElapsed = true;
        updateInterAdmModeStatus();
        flush();
    }

    // 3.1.4 연산 로직
    // 3.1.4.2 ADM Status judged by Cluster
    // EXNCP-46928 에서 언급된 바와 같이 timer 를 동작시켜 Debounce time 을 판단하도록 수정함.
    // Debounce time == 5s
    void updateInterAdmModeStatus() {
        if (mConfigWaymoSDSApplied != ConfigWaymoSDSApplied::ON) {
            mOneShotTimerDebounceTime.stop();
            return;
        }

        if (mPrevAdmModeStatus != mAdmModeStatus) {
            mIsDebounceTimeElapsed = false;
            mOneShotTimerDebounceTime.restart();
        }

        if (mWasIgnOn == true && mIsIgnOn == false &&
            mInter_AdmModeStatus1 != SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::OFF) {
            mInter_AdmModeStatus1 = SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM1;
            mInter_AdmModeStatus2 = SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2::IGN1OFF;
        } else if (mIsIgnOn == true) {
            if (mAdmModeStatus == AdmModeStatus::ADM1 && mIsDebounceTimeElapsed == true) {
                if (mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::OFF ||
                    mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM3) {
                    mInter_AdmModeStatus1 = SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM1;
                    mInter_AdmModeStatus2 = SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2::NORMAL;
                }
            } else if (mAdmModeStatus == AdmModeStatus::ADM1 &&
                       mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM2) {
                mInter_AdmModeStatus1 = SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM1;
                mInter_AdmModeStatus2 = SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2::NORMAL;
            } else if (mAdmModeStatus == AdmModeStatus::ADM2 && mIsDebounceTimeElapsed == false &&
                       mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM1) {
                mInter_AdmModeStatus1 = SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM1;
                mInter_AdmModeStatus2 = SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2::SHORT_SIGNAL_ADM2;
            } else if (mAdmModeStatus == AdmModeStatus::ADM3 && mIsDebounceTimeElapsed == false &&
                       mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM1) {
                mInter_AdmModeStatus1 = SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM1;
                mInter_AdmModeStatus2 = SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2::SHORT_SIGNAL_ADM3;
            } else if (mAdmModeStatus == AdmModeStatus::ADM3 &&
                       mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM2) {
                mInter_AdmModeStatus1 = SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM1;
                mInter_AdmModeStatus2 = SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2::PROHIBIT_TO_ADM3;
            } else if (mAdmModeStatus == AdmModeStatus::UNHANDLED_TIMEOUT) {
                if (mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM1 ||
                    mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM2) {
                    mInter_AdmModeStatus1 = SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM1;
                    mInter_AdmModeStatus2 = SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2::STALE;
                }
            } else if (mAdmModeStatus == AdmModeStatus::ADM2 && mIsDebounceTimeElapsed == true &&
                       mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM1) {
                mInter_AdmModeStatus1 = SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM2;
                mInter_AdmModeStatus2 = SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2::NORMAL;
            } else if (mAdmModeStatus == AdmModeStatus::ADM3 && mIsDebounceTimeElapsed == true &&
                       mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM1) {
                mInter_AdmModeStatus1 = SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM3;
                mInter_AdmModeStatus2 = SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2::NORMAL;
            } else if (mAdmModeStatus == AdmModeStatus::ADM1 && mIsDebounceTimeElapsed == false &&
                       mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM3) {
                mInter_AdmModeStatus1 = SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM3;
                mInter_AdmModeStatus2 = SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2::SHORT_SIGNAL_ADM1;
            } else if (mAdmModeStatus == AdmModeStatus::ADM2 && mIsDebounceTimeElapsed == true &&
                       mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM3) {
                mInter_AdmModeStatus1 = SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM3;
                mInter_AdmModeStatus2 = SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2::PROHIBIT_TO_ADM2;
            } else if (mAdmModeStatus == AdmModeStatus::UNHANDLED_TIMEOUT &&
                       mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM3) {
                mInter_AdmModeStatus1 = SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM3;
                mInter_AdmModeStatus2 = SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2::STALE;
            } else {
                // no operation
            }
        } else {
            // no operation
        }

        setSFCADM_Mode_InformationConstantInter_AdmModeStatusStat1(mInter_AdmModeStatus1);
        setSFCADM_Mode_InformationConstantInter_AdmModeStatusStat2(mInter_AdmModeStatus2);

        updateOutputClusterAdmModeStatus();
        updateOutputClusterAdmModeFailStatus();
        updateConstantAdmMode();
        updateConstantAdm3DisplayMode();
    }

    // 3.1.4.3 출력 신호 처리
    // 3.1.4.3.1 ADM 현재 상태 전달
    void updateOutputClusterAdmModeStatus() {
        if (mConfigWaymoSDSApplied != ConfigWaymoSDSApplied::ON) {
            return;
        }

        AddressValueList list;
        if (mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::OFF) {
            list.emplace_back(Vehicle.WYM.ADM_Mode_Information.Output_ClusterAdmModeStatus,
                              static_cast<HUInt64>(Output_ClusterAdmModeStaus::OFF));
        } else if (mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM1) {
            list.emplace_back(Vehicle.WYM.ADM_Mode_Information.Output_ClusterAdmModeStatus,
                              static_cast<HUInt64>(Output_ClusterAdmModeStaus::ADM1));
        } else if (mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM2) {
            list.emplace_back(Vehicle.WYM.ADM_Mode_Information.Output_ClusterAdmModeStatus,
                              static_cast<HUInt64>(Output_ClusterAdmModeStaus::ADM2));
        } else if (mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM3) {
            list.emplace_back(Vehicle.WYM.ADM_Mode_Information.Output_ClusterAdmModeStatus,
                              static_cast<HUInt64>(Output_ClusterAdmModeStaus::ADM3));
        } else {
            // no operation
        }
        setValue(list);
    }

    // 3.1.4.3.2 ADM 전환 불가 정보 전달
    void updateOutputClusterAdmModeFailStatus() {
        if (mConfigWaymoSDSApplied != ConfigWaymoSDSApplied::ON) {
            return;
        }

        AddressValueList list;
        if (mIsIgnOn == true) {
            if (mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM2 &&
                mInter_AdmModeStatus2 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2::SHORT_SIGNAL_ADM1) {
                list.emplace_back(Vehicle.WYM.ADM_Mode_Information.Output_ClusterAdmModeFailStatus,
                                  static_cast<HUInt64>(Output_ClusterAdmModeFailStatus::UNDEBOUNCE_ADM2_TO_ADM1));
            } else if (mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM3 &&
                       mInter_AdmModeStatus2 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2::SHORT_SIGNAL_ADM1) {
                list.emplace_back(Vehicle.WYM.ADM_Mode_Information.Output_ClusterAdmModeFailStatus,
                                  static_cast<HUInt64>(Output_ClusterAdmModeFailStatus::UNDEBOUNCE_ADM3_TO_ADM1));
            } else if (mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM1 &&
                       mInter_AdmModeStatus2 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2::SHORT_SIGNAL_ADM2) {
                list.emplace_back(Vehicle.WYM.ADM_Mode_Information.Output_ClusterAdmModeFailStatus,
                                  static_cast<HUInt64>(Output_ClusterAdmModeFailStatus::UNDEBOUNCE_ADM1_TO_ADM2));
            } else if (mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM1 &&
                       mInter_AdmModeStatus2 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2::SHORT_SIGNAL_ADM3) {
                list.emplace_back(Vehicle.WYM.ADM_Mode_Information.Output_ClusterAdmModeFailStatus,
                                  static_cast<HUInt64>(Output_ClusterAdmModeFailStatus::UNDEBOUNCE_ADM1_TO_ADM3));
            } else if (mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM1 &&
                       mInter_AdmModeStatus2 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2::STALE) {
                list.emplace_back(Vehicle.WYM.ADM_Mode_Information.Output_ClusterAdmModeFailStatus,
                                  static_cast<HUInt64>(Output_ClusterAdmModeFailStatus::STALE));
            } else if (mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM2 &&
                       mInter_AdmModeStatus2 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2::PROHIBIT_TO_ADM3) {
                list.emplace_back(Vehicle.WYM.ADM_Mode_Information.Output_ClusterAdmModeFailStatus,
                                  static_cast<HUInt64>(Output_ClusterAdmModeFailStatus::REQUEST_ADM2_TO_ADM3));
            } else if (mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM3 &&
                       mInter_AdmModeStatus2 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2::PROHIBIT_TO_ADM2) {
                list.emplace_back(Vehicle.WYM.ADM_Mode_Information.Output_ClusterAdmModeFailStatus,
                                  static_cast<HUInt64>(Output_ClusterAdmModeFailStatus::REQUEST_ADM3_TO_ADM2));
            } else {
                // no operation
            }
        }
        setValue(list);
    }

    // 3.1.5 표시 로직
    // 3.1.5.1 Telltale - N/A
    // 3.1.5.2 표시 구성요소 정보 처리 로직
    // 3.1.5.2.1 Indicator Display(MANUAL, AUTO, STALE)
    void updateConstantAdmMode() {
        if (mConfigWaymoSDSApplied != ConfigWaymoSDSApplied::ON) {
            return;
        }

        SFCADM_Mode_InformationConstantAdmModeStat stat = SFCADM_Mode_InformationConstantAdmModeStat::DISPLAY_OFF;
        if (mIgnElapsed == IgnElapsed::ON_500ms) {
            if (mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM1 &&
                mInter_AdmModeStatus2 != SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2::STALE) {
                stat = SFCADM_Mode_InformationConstantAdmModeStat::MANUAL;
            } else if (mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM2 ||
                       (mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM3 &&
                        mInter_AdmModeStatus2 != SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2::STALE)) {
                stat = SFCADM_Mode_InformationConstantAdmModeStat::AUTO;
            } else if ((mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM1 ||
                        mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM3) &&
                       mInter_AdmModeStatus2 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2::STALE) {
                stat = SFCADM_Mode_InformationConstantAdmModeStat::STALE;
            } else {
                // no operation
            }
        }
        setSFCADM_Mode_InformationConstantAdmModeStat(stat);
    }

    // 3.1.5.2.2 ADM3 only Display Mode
    void updateConstantAdm3DisplayMode() {
        if (mConfigWaymoSDSApplied != ConfigWaymoSDSApplied::ON) {
            return;
        }

        SFCADM_Mode_InformationConstantAdm3DisplayModeStat stat = SFCADM_Mode_InformationConstantAdm3DisplayModeStat::THEME_OFF;
        if ((mIgnElapsed == IgnElapsed::ON_0ms || mIgnElapsed == IgnElapsed::ON_500ms) &&
            mInter_AdmModeStatus1 == SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::ADM3) {
            switch (mAdm3VisualSelectStatus) {
                case Adm3VisualSelectStatus::OFF:
                    stat = SFCADM_Mode_InformationConstantAdm3DisplayModeStat::THEME_DEFAULT;
                    break;
                case Adm3VisualSelectStatus::THEME1:
                    stat = SFCADM_Mode_InformationConstantAdm3DisplayModeStat::THEME1;
                    break;
                case Adm3VisualSelectStatus::THEME2:
                    stat = SFCADM_Mode_InformationConstantAdm3DisplayModeStat::THEME2;
                    break;
                case Adm3VisualSelectStatus::THEME3:
                    stat = SFCADM_Mode_InformationConstantAdm3DisplayModeStat::THEME3;
                    break;
                case Adm3VisualSelectStatus::THEME4:
                    stat = SFCADM_Mode_InformationConstantAdm3DisplayModeStat::THEME4;
                    break;
                case Adm3VisualSelectStatus::THEME5:
                    stat = SFCADM_Mode_InformationConstantAdm3DisplayModeStat::THEME5;
                    break;
                case Adm3VisualSelectStatus::THEME6:
                    stat = SFCADM_Mode_InformationConstantAdm3DisplayModeStat::THEME6;
                    break;
                case Adm3VisualSelectStatus::THEME7:
                    stat = SFCADM_Mode_InformationConstantAdm3DisplayModeStat::THEME7;
                    break;
                default:
                    // no operation
                    break;
            }
        }
        setSFCADM_Mode_InformationConstantAdm3DisplayModeStat(stat);
    }

    void updateAnimationStatusFromSDS() {
        if (mConfigWaymoSDSApplied != ConfigWaymoSDSApplied::ON) {
            return;
        }

        if (mIsIgnOn == true && mAnimationStatusFromSDS == AnimationStatusFromSDS::ON) {
            setSFCADM_Mode_InformationConstantAdm3AnimationStat(SFCADM_Mode_InformationConstantAdm3AnimationStat::ON);
        } else {
            setSFCADM_Mode_InformationConstantAdm3AnimationStat(SFCADM_Mode_InformationConstantAdm3AnimationStat::OFF);
        }
    }

    // 3.1.5.3 이벤트 표시 동작 로직
    void updateEvent1() {
        if (mConfigWaymoSDSApplied != ConfigWaymoSDSApplied::ON) {
            return;
        }

        SFCADM_Mode_InformationEventEW0001Stat stat = SFCADM_Mode_InformationEventEW0001Stat::OFF;
        if (mIsIgnOn == true && mAdm0PropulsionSystemStatus == Adm0PropulsionSystemStatus::ADM0) {
            stat = SFCADM_Mode_InformationEventEW0001Stat::ON;
        }
        setSFCADM_Mode_InformationEventEW0001Stat(stat);
    }

    // 3.1.5.4 사운드 로직 - N/A

    // Class-local Enum
    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        OFF_0ms,
        MAX
    };

    enum class Output_ClusterAdmModeStaus : HUInt64 {
        OFF,
        ADM1,
        ADM2,
        ADM3,
    };

    enum class Output_ClusterAdmModeFailStatus : HUInt64 {
        OFF,
        UNDEBOUNCE_ADM2_TO_ADM1,
        UNDEBOUNCE_ADM3_TO_ADM1,
        UNDEBOUNCE_ADM1_TO_ADM2,
        UNDEBOUNCE_ADM1_TO_ADM3,
        STALE,
        REQUEST_ADM2_TO_ADM3,
        REQUEST_ADM3_TO_ADM2,
    };

    // VSM variables
    using ConfigWaymoSDSApplied = ArgumentsConfigWaymoSDSAppliedChanged::Inter_ConfigWaymoSDSApplied;
    using AdmModeStatus = ArgumentsAdmModeStatusChanged::Input_AdmModeStatus;
    using Adm3VisualSelectStatus = ArgumentsAdm3VisualSelectStatusChanged::Input_Adm3VisualSelectStatus;
    using AnimationStatusFromSDS = ArgumentsAnimationStatusFromSDSChanged::Input_Adm3AnimationStatusFromSDS;
    using Adm0PropulsionSystemStatus = ArgumentsAdm0PropulsionSystemStatusChanged::Input_Adm0PropulsionSystemStatus;

    bool mIsIgnOn = false;
    bool mWasIgnOn = false;
    IgnElapsed mIgnElapsed = IgnElapsed::NONE;
    ConfigWaymoSDSApplied mConfigWaymoSDSApplied = ConfigWaymoSDSApplied::OFF;
    AdmModeStatus mAdmModeStatus = AdmModeStatus::OFF;
    AdmModeStatus mPrevAdmModeStatus = AdmModeStatus::OFF;
    Adm3VisualSelectStatus mAdm3VisualSelectStatus = Adm3VisualSelectStatus::OFF;
    AnimationStatusFromSDS mAnimationStatusFromSDS = AnimationStatusFromSDS::OFF;
    Adm0PropulsionSystemStatus mAdm0PropulsionSystemStatus = Adm0PropulsionSystemStatus::OFF;

    // SFC variables
    HBool mAEMInitializeComplete = false;

    // Private variables
    HBool mIsDebounceTimeElapsed = false;
    SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1 mInter_AdmModeStatus1 =
        SFCADM_Mode_InformationConstantInter_AdmModeStatusStat1::OFF;
    SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2 mInter_AdmModeStatus2 =
        SFCADM_Mode_InformationConstantInter_AdmModeStatusStat2::NORMAL;

    // Constant Variables
    static constexpr uint32_t kTimerInterval5000ms = 5000u;

    // SFC Timer
    ssfs::SFCTimer<ADM_Mode_Information> mOneShotTimerDebounceTime;
};

}  // namespace ccos

#endif  // SFSS_ADM_Mode_Information_H
