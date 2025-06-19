/**
 * @file LWS.hpp
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
#ifndef SFSS_LWS_H
#define SFSS_LWS_H

#define DLOG_ENABLED gEnableSFCLog

#include "LWSBase.hpp"

namespace ccos {

// SFC Version : 13.0.0
// Spec Version : v0.24
// Reference : [CV140] LWS. Contains : Telltale, Constant. Recommend: VALUE_CHANGED
class LWS : public LWSBase {
public:
    LWS() = default;
    ~LWS() override = default;
    LWS(const LWS& other) = delete;
    LWS(LWS&& other) noexcept = delete;
    LWS& operator=(const LWS& other) = delete;
    LWS& operator=(LWS&& other) noexcept = delete;

    void onInitialize() override {
        setSFCLWSTelltaleLWSBlinkValueA(SFCLWSTelltaleLWSBlinkValueA::ON);
        setSFCLWSTelltaleLWSBlinkValueB(SFCLWSTelltaleLWSBlinkValueB::OFF);
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mIsIgnOn = true;
        updateLWSTelltale();
        updateConstantPageStatus();
        updateConstantLWSAxleValueAndStatus();
        updateConstantAxleStatus();
        updateLWSWarningTextStatus();
    }

    void onIgnElapseOn500msChanged(const ArgumentsIgnElapseOn500msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::ON_500ms;
        mIsIgnOn = true;
        updateLWSTelltale();
        updateConstantPageStatus();
        updateConstantLWSAxleValueAndStatus();
        updateConstantAxleStatus();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& /*args*/) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIsIgnOn = false;
        updateLWSTelltale();
        updateConstantPageStatus();
        updateConstantLWSAxleValueAndStatus();
        updateConstantAxleStatus();
        updateLWSWarningTextStatus();
    }

    void onLwsAxleValueChanged(const ArgumentsLwsAxleValueChanged& args) {
        mLWSAxle1LeftValue = args.mInput_LWS1AxleLeftValue;
        mLWSAxle1RightValue = args.mInput_LWS1AxleRightValue;
        mLWSAxle2LeftValue = args.mInput_LWS2AxleLeftValue;
        mLWSAxle2RightValue = args.mInput_LWS2AxleRightValue;
        mLWSAxle3LeftValue = args.mInput_LWS3AxleLeftValue;
        mLWSAxle3RightValue = args.mInput_LWS3AxleRightValue;
        mLWSAxle4LeftValue = args.mInput_LWS4AxleLeftValue;
        mLWSAxle4RightValue = args.mInput_LWS4AxleRightValue;
        mLWSAxle5LeftValue = args.mInput_LWS5AxleLeftValue;
        mLWSAxle5RightValue = args.mInput_LWS5AxleRightValue;
        updateConstantLWSAxleValueAndStatus();
    }

    void onLwsWarningSignalStatusChanged(const ArgumentsLwsWarningSignalStatusChanged& args) {
        mWarningSignalStatus = args.mInput_LWSWarningSignalStatus;
        updateLWSTelltale();
        updateLWSWarningTextStatus();
    }

    void onConfigAxleInfoChanged(const ArgumentsConfigAxleInfoChanged& args) {
        mConfigAxleInfo = args.mInter_ConfigAxleInfo;
        updateConstantAxleStatus();
    }

    void onConfigAreaChanged(const ArgumentsConfigAreaChanged& args) {
        mConfigArea = args.mInter_ConfigArea;
        updateConstantAxleStatus();
    }

    void onPageStatusChanged(const ArgumentsPageStatusChanged& args) {
        mPageStatus = args.mInput_LWSPageStatus;
        updateConstantPageStatus();
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

private:
    // 5.1.1 LWS Telltale
    void updateLWSTelltale() {
        SFCLWSTelltaleLWSStat telltaleStat = SFCLWSTelltaleLWSStat::OFF;
        if (mIgnElapsed == IgnElapsed::ON_500ms) {
            switch (mWarningSignalStatus) {
                case WarningSignalStatus::ON:
                    telltaleStat = SFCLWSTelltaleLWSStat::ON;
                    break;
                case WarningSignalStatus::BLINK1:
                    telltaleStat = SFCLWSTelltaleLWSStat::BLINK1;
                    break;
                case WarningSignalStatus::BLINK2:
                    telltaleStat = SFCLWSTelltaleLWSStat::BLINK2;
                    break;
                default:
                    break;
            }
        }
        setSFCLWSTelltaleLWSStat(telltaleStat);
    }

    // 5.2.1 LWS 표시 페이지 활성화
    void updateConstantPageStatus() {
        SFCLWSConstantPageStat stat = SFCLWSConstantPageStat::OFF;
        if (mIsIgnOn == true && mPageStatus == PageStatus::ON) {
            stat = SFCLWSConstantPageStat::ON;
        }
        setSFCLWSConstantPageStat(stat);
    }

    // 5.2.2/5.2.3 LWS %상태 표시, LWS 고장 상태 표시
    void updateConstantLWSAxleValueAndStatus() {
        updateLWSAxle1LeftValueAndStatus();
        updateLWSAxle1RightValueAndStatus();
        updateLWSAxle2LeftValueAndStatus();
        updateLWSAxle2RightValueAndStatus();
        updateLWSAxle3LeftValueAndStatus();
        updateLWSAxle3RightValueAndStatus();
        updateLWSAxle4LeftValueAndStatus();
        updateLWSAxle4RightValueAndStatus();
        updateLWSAxle5LeftValueAndStatus();
        updateLWSAxle5RightValueAndStatus();
    }

    void calLWSAxleValue(const HBool& isIgnOn, const HUInt64& inputAxleValue, HDouble& axleValueToSet) {
        if (isIgnOn == true) {
            if (inputAxleValue <= 250) {
                // Round down below 2 decimal place, 소수 둘째자리 이하 버림.
                axleValueToSet = floor(static_cast<HDouble>(inputAxleValue) * kLWSValueFactor * 10.0) / 10.0;
            }
        }
    }

    template <typename T1>
    void calLWSAxleStatus(const HBool& isIgnOn, const HUInt64& inputAxleValue, T1& failStatus) {
        if (isIgnOn == true) {
            if (inputAxleValue <= 251 || inputAxleValue == 255) {
                failStatus = T1::NORMAL;
            } else if (inputAxleValue == 252 || inputAxleValue == 253 || inputAxleValue == 254) {
                failStatus = T1::WARN;
            } else {
                // no operation
            }
        }
    }

    void updateLWSAxle1LeftValueAndStatus() {
        HDouble axleVal = static_cast<HDouble>(0xFF);
        SFCLWSConstantAxle1LeftStat stat = SFCLWSConstantAxle1LeftStat::OFF;
        calLWSAxleValue(mIsIgnOn, mLWSAxle1LeftValue, axleVal);
        calLWSAxleStatus(mIsIgnOn, mLWSAxle1LeftValue, stat);
        setSFCLWSConstantAxle1LeftStat(stat);
        setSFCLWSConstantAxle1LeftValue(axleVal);
    }

    void updateLWSAxle1RightValueAndStatus() {
        HDouble axleVal = static_cast<HDouble>(0xFF);
        SFCLWSConstantAxle1RightStat stat = SFCLWSConstantAxle1RightStat::OFF;
        calLWSAxleValue(mIsIgnOn, mLWSAxle1RightValue, axleVal);
        calLWSAxleStatus(mIsIgnOn, mLWSAxle1RightValue, stat);
        setSFCLWSConstantAxle1RightStat(stat);
        setSFCLWSConstantAxle1RightValue(axleVal);
    }

    void updateLWSAxle2LeftValueAndStatus() {
        HDouble axleVal = static_cast<HDouble>(0xFF);
        SFCLWSConstantAxle2LeftStat stat = SFCLWSConstantAxle2LeftStat::OFF;
        calLWSAxleValue(mIsIgnOn, mLWSAxle2LeftValue, axleVal);
        calLWSAxleStatus(mIsIgnOn, mLWSAxle2LeftValue, stat);
        setSFCLWSConstantAxle2LeftStat(stat);
        setSFCLWSConstantAxle2LeftValue(axleVal);
    }

    void updateLWSAxle2RightValueAndStatus() {
        HDouble axleVal = static_cast<HDouble>(0xFF);
        SFCLWSConstantAxle2RightStat stat = SFCLWSConstantAxle2RightStat::OFF;
        calLWSAxleValue(mIsIgnOn, mLWSAxle2RightValue, axleVal);
        calLWSAxleStatus(mIsIgnOn, mLWSAxle2RightValue, stat);
        setSFCLWSConstantAxle2RightStat(stat);
        setSFCLWSConstantAxle2RightValue(axleVal);
    }

    void updateLWSAxle3LeftValueAndStatus() {
        HDouble axleVal = static_cast<HDouble>(0xFF);
        SFCLWSConstantAxle3LeftStat stat = SFCLWSConstantAxle3LeftStat::OFF;
        calLWSAxleValue(mIsIgnOn, mLWSAxle3LeftValue, axleVal);
        calLWSAxleStatus(mIsIgnOn, mLWSAxle3LeftValue, stat);
        setSFCLWSConstantAxle3LeftStat(stat);
        setSFCLWSConstantAxle3LeftValue(axleVal);
    }

    void updateLWSAxle3RightValueAndStatus() {
        HDouble axleVal = static_cast<HDouble>(0xFF);
        SFCLWSConstantAxle3RightStat stat = SFCLWSConstantAxle3RightStat::OFF;
        calLWSAxleValue(mIsIgnOn, mLWSAxle3RightValue, axleVal);
        calLWSAxleStatus(mIsIgnOn, mLWSAxle3RightValue, stat);
        setSFCLWSConstantAxle3RightStat(stat);
        setSFCLWSConstantAxle3RightValue(axleVal);
    }

    void updateLWSAxle4LeftValueAndStatus() {
        HDouble axleVal = static_cast<HDouble>(0xFF);
        SFCLWSConstantAxle4LeftStat stat = SFCLWSConstantAxle4LeftStat::OFF;
        calLWSAxleValue(mIsIgnOn, mLWSAxle4LeftValue, axleVal);
        calLWSAxleStatus(mIsIgnOn, mLWSAxle4LeftValue, stat);
        setSFCLWSConstantAxle4LeftStat(stat);
        setSFCLWSConstantAxle4LeftValue(axleVal);
    }

    void updateLWSAxle4RightValueAndStatus() {
        HDouble axleVal = static_cast<HDouble>(0xFF);
        SFCLWSConstantAxle4RightStat stat = SFCLWSConstantAxle4RightStat::OFF;
        calLWSAxleValue(mIsIgnOn, mLWSAxle4RightValue, axleVal);
        calLWSAxleStatus(mIsIgnOn, mLWSAxle4RightValue, stat);
        setSFCLWSConstantAxle4RightStat(stat);
        setSFCLWSConstantAxle4RightValue(axleVal);
    }

    void updateLWSAxle5LeftValueAndStatus() {
        HDouble axleVal = static_cast<HDouble>(0xFF);
        SFCLWSConstantAxle5LeftStat stat = SFCLWSConstantAxle5LeftStat::OFF;
        calLWSAxleValue(mIsIgnOn, mLWSAxle5LeftValue, axleVal);
        calLWSAxleStatus(mIsIgnOn, mLWSAxle5LeftValue, stat);
        setSFCLWSConstantAxle5LeftStat(stat);
        setSFCLWSConstantAxle5LeftValue(axleVal);
    }

    void updateLWSAxle5RightValueAndStatus() {
        HDouble axleVal = static_cast<HDouble>(0xFF);
        SFCLWSConstantAxle5RightStat stat = SFCLWSConstantAxle5RightStat::OFF;
        calLWSAxleValue(mIsIgnOn, mLWSAxle5RightValue, axleVal);
        calLWSAxleStatus(mIsIgnOn, mLWSAxle5RightValue, stat);
        setSFCLWSConstantAxle5RightStat(stat);
        setSFCLWSConstantAxle5RightValue(axleVal);
    }

    // 5.2.4 축 구분 사양
    void updateConstantAxleStatus() {
        if (mIsIgnOn == true) {
            if (mConfigAxleInfo == ConfigAxleInfo::T6X2_TAG && mConfigArea != ConfigArea::EEC) {
                setSFCLWSConstantAxleInfoStat(SFCLWSConstantAxleInfoStat::T6X2_TAG_DOUBLE);
            } else {
                const auto iter = mAxleInfoMap.find(mConfigAxleInfo);
                if (iter != mAxleInfoMap.end()) {
                    setSFCLWSConstantAxleInfoStat(iter->second);
                }
            }
        }
    }

    // 5.2.6 LWS 경고 문구 표시
    void updateLWSWarningTextStatus() {
        SFCLWSConstantLWSWaringTextStat stat = SFCLWSConstantLWSWaringTextStat::OFF;
        if (mIsIgnOn == true) {
            if (mWarningSignalStatus == WarningSignalStatus::ON || mWarningSignalStatus == WarningSignalStatus::BLINK1) {
                stat = SFCLWSConstantLWSWaringTextStat::WARN_TEXT1;
            } else if (mWarningSignalStatus == WarningSignalStatus::BLINK2) {
                stat = SFCLWSConstantLWSWaringTextStat::WARN_TEXT2;
            } else {
                // OFF
            }
        }
        setSFCLWSConstantLWSWaringTextStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        ON_500ms,
        OFF_0ms,
        MAX
    };

    using WarningSignalStatus = ArgumentsLwsWarningSignalStatusChanged::Input_LWSWarningSignalStatus;
    using ConfigAxleInfo = ArgumentsConfigAxleInfoChanged::Inter_ConfigAxleInfo;
    using PageStatus = ArgumentsPageStatusChanged::Input_LWSPageStatus;
    using ConfigArea = ArgumentsConfigAreaChanged::Inter_ConfigArea;

    WarningSignalStatus mWarningSignalStatus = WarningSignalStatus::OFF;
    ConfigAxleInfo mConfigAxleInfo = ConfigAxleInfo::T4X2;
    ConfigArea mConfigArea = ConfigArea::DOM;
    PageStatus mPageStatus = PageStatus::OFF;
    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    HBool mIsIgnOn = false;
    HUInt64 mLWSAxle1LeftValue = 0;
    HUInt64 mLWSAxle1RightValue = 0;
    HUInt64 mLWSAxle2LeftValue = 0;
    HUInt64 mLWSAxle2RightValue = 0;
    HUInt64 mLWSAxle3LeftValue = 0;
    HUInt64 mLWSAxle3RightValue = 0;
    HUInt64 mLWSAxle4LeftValue = 0;
    HUInt64 mLWSAxle4RightValue = 0;
    HUInt64 mLWSAxle5LeftValue = 0;
    HUInt64 mLWSAxle5RightValue = 0;

    std::map<ConfigAxleInfo, SFCLWSConstantAxleInfoStat> mAxleInfoMap = {
        {ConfigAxleInfo::T4X2, SFCLWSConstantAxleInfoStat::T4X2},
        {ConfigAxleInfo::T6X2, SFCLWSConstantAxleInfoStat::T6X2},
        {ConfigAxleInfo::T6X2_TAG, SFCLWSConstantAxleInfoStat::T6X2_TAG},
        {ConfigAxleInfo::T6X4, SFCLWSConstantAxleInfoStat::T6X4},
        {ConfigAxleInfo::T8X4, SFCLWSConstantAxleInfoStat::T8X4},
        {ConfigAxleInfo::T8X4_PUSHER, SFCLWSConstantAxleInfoStat::T8X4_PUSHER},
        {ConfigAxleInfo::T10X4_PUSHER, SFCLWSConstantAxleInfoStat::T10X4_PUSHER},
        {ConfigAxleInfo::T10X4_TAG, SFCLWSConstantAxleInfoStat::T10X4_TAG}};

    static constexpr HDouble kLWSValueFactor = 0.4;
};

}  // namespace ccos

#endif  // SFSS_LWS_H
