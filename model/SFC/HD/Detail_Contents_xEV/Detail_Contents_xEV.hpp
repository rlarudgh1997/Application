/**
 * @file Detail_Contents_xEV.hpp
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
#ifndef SFSS_Detail_Contents_xEV_H
#define SFSS_Detail_Contents_xEV_H

#define DLOG_ENABLED gEnableSFCLog

#include <map>
#include <cassert>
#include "Detail_Contents_xEVBase.hpp"

namespace ccos {

// SFC Version : 7.0.1
// Reference : [HD200] Detail_Contents_xEV Contains : Event Recommend: VALUE_CHANGED
class Detail_Contents_xEV : public Detail_Contents_xEVBase {
public:
    Detail_Contents_xEV() = default;
    ~Detail_Contents_xEV() override = default;
    Detail_Contents_xEV(const Detail_Contents_xEV& other) = delete;
    Detail_Contents_xEV(Detail_Contents_xEV&& other) noexcept = delete;
    Detail_Contents_xEV& operator=(const Detail_Contents_xEV& other) = delete;
    Detail_Contents_xEV& operator=(Detail_Contents_xEV&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& args) {
        mIgnElapsed = IgnElapsed::ON_0ms;
        mIsIgnOn = true;

        updateAll();
    }

    void onIgnElapseOff0msChanged(const ArgumentsIgnElapseOff0msChanged& args) {
        mIgnElapsed = IgnElapsed::OFF_0ms;
        mIsIgnOn = false;
    }

    void onConfigChanged(const ArgumentsConfigChanged& args) {
        mConfigHUD = args.mInter_ConfigHUD;
        mConfigNewHUD = args.mInter_ConfigNewHUD;
        mConfigVehicleType = args.mInter_ConfigVehicleType;

        updateAll();
    }

    void onConfigBoostModeChanged(const ArgumentsConfigBoostModeChanged& args) {
        mConfigBoostMode = args.mInter_ConfigBoostMode;

        updateEvEcoPwrChgGaugeValue();
    }

    void onEvBoostGaugeValueChanged(const ArgumentsEvBoostGaugeValueChanged& args) {
        mEvBoostGaugeValue = args.mEvBoostGaugeValue;

        updateEvBoostGaugeValue();
    }

    void onEvEcoPwrChgGaugeChanged(const ArgumentsEvEcoPwrChgGaugeChanged& args) {
        mEvEcoPwrChgGaugeValue = args.mEvEcoPwrChgGaugeValue;
        mEvEcoPwrChgGaugeStatus = args.mEvEcoPwrChgGaugeStat;
        mEvEcoPwrChgGaugeBlinkValueA = args.mEvEcoPwrChgGaugeBlinkValueA;
        mEvEcoPwrChgGaugeBlinkValueB = args.mEvEcoPwrChgGaugeBlinkValueB;

        updateEvEcoPwrChgGaugeValue();
    }

    void onRegenFrontRecognizedChanged(const ArgumentsRegenFrontRecognizedChanged& args) {
        mRegenFrontRecognizedStatus = args.mRegenFrontRecognizedStat;

        updateRegenFrontRecognizedStatus();
    }

    void onRegenStatusChanged(const ArgumentsRegenStatusChanged& args) {
        mRegenStatus = args.mRegenStatusStat;

        updateRegenStatus();
    }

    void onRegenStepChanged(const ArgumentsRegenStepChanged& args) {
        mRegenStepStatus = args.mRegenStepStat;
        mRegenStepValue = args.mRegenStepValue;

        updateRegenStepValue();
    }

    void onRegenSystemStepChanged(const ArgumentsRegenSystemStepChanged& args) {
        mRegenSystemStepStatus = args.mRegenSystemStepStat;

        updateRegenSystemStepValue();
    }

    void dump(std::ostringstream& out, const std::string& prefix, const std::string& command) override {
        // no operation
    }

private:
    inline HBool isXevOnIgnOn() const {
        return mIsIgnOn && isXevOn();
    }

    inline HBool isXevOn() const {
        return (mConfigHUD == ConfigHUD::ON || mConfigNewHUD == ConfigNewHUD::ON) && mConfigVehicleType == ConfigVehicleType::EV;
    }

    template <typename KEY, typename VALUE>
    const VALUE getMappingData(const std::map<KEY, VALUE>& mapTable, const KEY& key) {
        VALUE ret = static_cast<VALUE>(0);
        const auto& it = mapTable.find(key);
        if (it != mapTable.end()) {
            ret = it->second;
        } else {
            assert(true);
        }

        return ret;
    }

    void updateAll() {
        updateEvEcoPwrChgGaugeValue();
        updateEvBoostGaugeValue();
        updateRegenStatus();
        updateRegenFrontRecognizedStatus();
        updateRegenStepValue();
        updateRegenSystemStepValue();
    }

    void updateEvEcoPwrChgGaugeValue() {
        if (isXevOnIgnOn() == false) {
            return;
        }

        HInt64 value = 0.0;
        SFCDetail_Contents_xEVConstantEvEcoPwrChgGaugeStat stat = SFCDetail_Contents_xEVConstantEvEcoPwrChgGaugeStat::NONE;
        SFCDetail_Contents_xEVConstantEvEcoPwrChgGaugeBlinkValueA blinkValueA =
            SFCDetail_Contents_xEVConstantEvEcoPwrChgGaugeBlinkValueA::NONE;
        SFCDetail_Contents_xEVConstantEvEcoPwrChgGaugeBlinkValueB blinkValueB =
            SFCDetail_Contents_xEVConstantEvEcoPwrChgGaugeBlinkValueB::NONE;

        if (mConfigBoostMode == ConfigBoostMode::OFF || mConfigBoostMode == ConfigBoostMode::ON) {
            value = mEvEcoPwrChgGaugeValue;
            stat = getMappingData<EvEcoPwrChgGaugeStatus, SFCDetail_Contents_xEVConstantEvEcoPwrChgGaugeStat>(
                mEvEcoPwrChgGaugeStatusMap, mEvEcoPwrChgGaugeStatus);
            blinkValueA = getMappingData<EvEcoPwrChgGaugeBlinkValueA, SFCDetail_Contents_xEVConstantEvEcoPwrChgGaugeBlinkValueA>(
                mEvEcoPwrChgGaugeBlinkValueAMap, mEvEcoPwrChgGaugeBlinkValueA);
            blinkValueB = getMappingData<EvEcoPwrChgGaugeBlinkValueB, SFCDetail_Contents_xEVConstantEvEcoPwrChgGaugeBlinkValueB>(
                mEvEcoPwrChgGaugeBlinkValueBMap, mEvEcoPwrChgGaugeBlinkValueB);
        }

        setSFCDetail_Contents_xEVConstantEvEcoPwrChgGaugeValue(value);
        setSFCDetail_Contents_xEVConstantEvEcoPwrChgGaugeStat(stat);
        setSFCDetail_Contents_xEVConstantEvEcoPwrChgGaugeBlinkValueA(blinkValueA);
        setSFCDetail_Contents_xEVConstantEvEcoPwrChgGaugeBlinkValueB(blinkValueB);
    }

    void updateEvBoostGaugeValue() {
        if (isXevOnIgnOn() == false) {
            return;
        }

        SFCDetail_Contents_xEVConstantEvBoostGaugeValue value = SFCDetail_Contents_xEVConstantEvBoostGaugeValue::DISPLAY_OFF;
        value = getMappingData<EvBoostGaugeValue, SFCDetail_Contents_xEVConstantEvBoostGaugeValue>(mEvBoostGaugeValueMap,
                                                                                                   mEvBoostGaugeValue);
        setSFCDetail_Contents_xEVConstantEvBoostGaugeValue(value);
    }

    void updateRegenStatus() {
        if (isXevOnIgnOn() == false) {
            return;
        }

        SFCDetail_Contents_xEVConstantRegenStatusStat stat = SFCDetail_Contents_xEVConstantRegenStatusStat::DISPLAY_OFF;
        stat = getMappingData<RegenStatus, SFCDetail_Contents_xEVConstantRegenStatusStat>(mRegenStatusMap, mRegenStatus);
        setSFCDetail_Contents_xEVConstantRegenStatusStat(stat);
    }

    void updateRegenFrontRecognizedStatus() {
        if (isXevOnIgnOn() == false) {
            return;
        }

        SFCDetail_Contents_xEVConstantRegenFrontRecognizedStat stat =
            SFCDetail_Contents_xEVConstantRegenFrontRecognizedStat::DISPLAY_OFF;
        stat = getMappingData<RegenFrontRecognizedStatus, SFCDetail_Contents_xEVConstantRegenFrontRecognizedStat>(
            mRegenFrontRecognizedStatusMap, mRegenFrontRecognizedStatus);
        setSFCDetail_Contents_xEVConstantRegenFrontRecognizedStat(stat);
    }

    void updateRegenStepValue() {
        if (isXevOnIgnOn() == false) {
            return;
        }

        SFCDetail_Contents_xEVConstantRegenStepStat stat = SFCDetail_Contents_xEVConstantRegenStepStat::DISPLAY_OFF;
        HDouble value = 0.0;

        stat =
            getMappingData<RegenStepStatus, SFCDetail_Contents_xEVConstantRegenStepStat>(mRegenStepStatusMap, mRegenStepStatus);
        value = mRegenStepValue;
        setSFCDetail_Contents_xEVConstantRegenStepStat(stat);
        setSFCDetail_Contents_xEVConstantRegenStepValue(value);
    }

    void updateRegenSystemStepValue() {
        if (isXevOn() == false) {
            return;
        }

        SFCDetail_Contents_xEVConstantRegenSystemStepStat stat = SFCDetail_Contents_xEVConstantRegenSystemStepStat::NONE;
        stat = getMappingData<RegenSystemStepStatus, SFCDetail_Contents_xEVConstantRegenSystemStepStat>(mRegenSystemStepStatusMap,
                                                                                                        mRegenSystemStepStatus);
        setSFCDetail_Contents_xEVConstantRegenSystemStepStat(stat);
    }

    enum class IgnElapsed : HUInt64 {
        NONE,
        ON_0ms,
        OFF_0ms,
        MAX
    };

    using ConfigHUD = ArgumentsConfigChanged::Inter_ConfigHUD;
    using ConfigNewHUD = ArgumentsConfigChanged::Inter_ConfigNewHUD;
    using ConfigVehicleType = ArgumentsConfigChanged::Inter_ConfigVehicleType;
    using ConfigBoostMode = ArgumentsConfigBoostModeChanged::Inter_ConfigBoostMode;
    using EvBoostGaugeValue = ArgumentsEvBoostGaugeValueChanged::EvBoostGaugeValue;
    using EvEcoPwrChgGaugeStatus = ArgumentsEvEcoPwrChgGaugeChanged::EvEcoPwrChgGaugeStat;
    using EvEcoPwrChgGaugeBlinkValueA = ArgumentsEvEcoPwrChgGaugeChanged::EvEcoPwrChgGaugeBlinkValueA;
    using EvEcoPwrChgGaugeBlinkValueB = ArgumentsEvEcoPwrChgGaugeChanged::EvEcoPwrChgGaugeBlinkValueB;
    using RegenFrontRecognizedStatus = ArgumentsRegenFrontRecognizedChanged::RegenFrontRecognizedStat;
    using RegenStatus = ArgumentsRegenStatusChanged::RegenStatusStat;
    using RegenStepStatus = ArgumentsRegenStepChanged::RegenStepStat;
    using RegenSystemStepStatus = ArgumentsRegenSystemStepChanged::RegenSystemStepStat;

    IgnElapsed mIgnElapsed = IgnElapsed::OFF_0ms;
    HBool mIsIgnOn = false;
    HInt64 mEvEcoPwrChgGaugeValue = 0;
    HDouble mRegenStepValue = 0.0;
    ConfigHUD mConfigHUD = ConfigHUD::OFF;
    ConfigNewHUD mConfigNewHUD = ConfigNewHUD::OFF;
    ConfigVehicleType mConfigVehicleType = ConfigVehicleType::ICV;
    ConfigBoostMode mConfigBoostMode = ConfigBoostMode::OFF;
    EvBoostGaugeValue mEvBoostGaugeValue = EvBoostGaugeValue::NONE;
    EvEcoPwrChgGaugeStatus mEvEcoPwrChgGaugeStatus = EvEcoPwrChgGaugeStatus::NONE;
    EvEcoPwrChgGaugeBlinkValueA mEvEcoPwrChgGaugeBlinkValueA = EvEcoPwrChgGaugeBlinkValueA::NONE;
    EvEcoPwrChgGaugeBlinkValueB mEvEcoPwrChgGaugeBlinkValueB = EvEcoPwrChgGaugeBlinkValueB::NONE;
    RegenFrontRecognizedStatus mRegenFrontRecognizedStatus = RegenFrontRecognizedStatus::NONE;
    RegenStatus mRegenStatus = RegenStatus::NONE;
    RegenStepStatus mRegenStepStatus = RegenStepStatus::NONE;
    RegenSystemStepStatus mRegenSystemStepStatus = RegenSystemStepStatus::NONE;

    std::map<EvEcoPwrChgGaugeStatus, SFCDetail_Contents_xEVConstantEvEcoPwrChgGaugeStat> mEvEcoPwrChgGaugeStatusMap = {
        {EvEcoPwrChgGaugeStatus::NONE, SFCDetail_Contents_xEVConstantEvEcoPwrChgGaugeStat::NONE},
        {EvEcoPwrChgGaugeStatus::VALUE_BOOST_MODE_ON, SFCDetail_Contents_xEVConstantEvEcoPwrChgGaugeStat::VALUE_BOOST_MODE_ON},
        {EvEcoPwrChgGaugeStatus::VALUE_BOOST_MODE_OFF, SFCDetail_Contents_xEVConstantEvEcoPwrChgGaugeStat::VALUE_BOOST_MODE_OFF},
        {EvEcoPwrChgGaugeStatus::BLINK1, SFCDetail_Contents_xEVConstantEvEcoPwrChgGaugeStat::BLINK1}};

    std::map<EvEcoPwrChgGaugeBlinkValueA, SFCDetail_Contents_xEVConstantEvEcoPwrChgGaugeBlinkValueA>
        mEvEcoPwrChgGaugeBlinkValueAMap = {
            {EvEcoPwrChgGaugeBlinkValueA::NONE, SFCDetail_Contents_xEVConstantEvEcoPwrChgGaugeBlinkValueA::NONE},
            {EvEcoPwrChgGaugeBlinkValueA::OFF, SFCDetail_Contents_xEVConstantEvEcoPwrChgGaugeBlinkValueA::OFF},
            {EvEcoPwrChgGaugeBlinkValueA::ON, SFCDetail_Contents_xEVConstantEvEcoPwrChgGaugeBlinkValueA::ON}};

    std::map<EvEcoPwrChgGaugeBlinkValueB, SFCDetail_Contents_xEVConstantEvEcoPwrChgGaugeBlinkValueB>
        mEvEcoPwrChgGaugeBlinkValueBMap = {
            {EvEcoPwrChgGaugeBlinkValueB::NONE, SFCDetail_Contents_xEVConstantEvEcoPwrChgGaugeBlinkValueB::NONE},
            {EvEcoPwrChgGaugeBlinkValueB::OFF, SFCDetail_Contents_xEVConstantEvEcoPwrChgGaugeBlinkValueB::OFF},
            {EvEcoPwrChgGaugeBlinkValueB::ON, SFCDetail_Contents_xEVConstantEvEcoPwrChgGaugeBlinkValueB::ON}};

    std::map<EvBoostGaugeValue, SFCDetail_Contents_xEVConstantEvBoostGaugeValue> mEvBoostGaugeValueMap = {
        {EvBoostGaugeValue::NONE, SFCDetail_Contents_xEVConstantEvBoostGaugeValue::NONE},
        {EvBoostGaugeValue::DISPLAY_OFF, SFCDetail_Contents_xEVConstantEvBoostGaugeValue::DISPLAY_OFF},
        {EvBoostGaugeValue::READY, SFCDetail_Contents_xEVConstantEvBoostGaugeValue::READY},
        {EvBoostGaugeValue::ON, SFCDetail_Contents_xEVConstantEvBoostGaugeValue::ON}};

    std::map<RegenStatus, SFCDetail_Contents_xEVConstantRegenStatusStat> mRegenStatusMap = {
        {RegenStatus::DISPLAY_OFF, SFCDetail_Contents_xEVConstantRegenStatusStat::DISPLAY_OFF},
        {RegenStatus::STOP, SFCDetail_Contents_xEVConstantRegenStatusStat::STOP},
        {RegenStatus::MANUAL_LV0, SFCDetail_Contents_xEVConstantRegenStatusStat::MANUAL_LV0},
        {RegenStatus::MANUAL_LV1, SFCDetail_Contents_xEVConstantRegenStatusStat::MANUAL_LV1},
        {RegenStatus::MANUAL_LV2, SFCDetail_Contents_xEVConstantRegenStatusStat::MANUAL_LV2},
        {RegenStatus::MANUAL_LV3, SFCDetail_Contents_xEVConstantRegenStatusStat::MANUAL_LV3},
        {RegenStatus::AUTO, SFCDetail_Contents_xEVConstantRegenStatusStat::AUTO},
        {RegenStatus::MAX, SFCDetail_Contents_xEVConstantRegenStatusStat::MAX}};

    std::map<RegenFrontRecognizedStatus, SFCDetail_Contents_xEVConstantRegenFrontRecognizedStat> mRegenFrontRecognizedStatusMap =
        {{RegenFrontRecognizedStatus::NONE, SFCDetail_Contents_xEVConstantRegenFrontRecognizedStat::NONE},
         {RegenFrontRecognizedStatus::DISPLAY_OFF, SFCDetail_Contents_xEVConstantRegenFrontRecognizedStat::DISPLAY_OFF},
         {RegenFrontRecognizedStatus::RECOGNIZED, SFCDetail_Contents_xEVConstantRegenFrontRecognizedStat::RECOGNIZED},
         {RegenFrontRecognizedStatus::UNRECOGNIZED, SFCDetail_Contents_xEVConstantRegenFrontRecognizedStat::UNRECOGNIZED},
         {RegenFrontRecognizedStatus::INACTIVE, SFCDetail_Contents_xEVConstantRegenFrontRecognizedStat::INACTIVE}};

    std::map<RegenStepStatus, SFCDetail_Contents_xEVConstantRegenStepStat> mRegenStepStatusMap = {
        {RegenStepStatus::NONE, SFCDetail_Contents_xEVConstantRegenStepStat::NONE},
        {RegenStepStatus::DISPLAY_OFF, SFCDetail_Contents_xEVConstantRegenStepStat::DISPLAY_OFF},
        {RegenStepStatus::MANUAL_LV0, SFCDetail_Contents_xEVConstantRegenStepStat::MANUAL_LV0},
        {RegenStepStatus::MANUAL_LV1, SFCDetail_Contents_xEVConstantRegenStepStat::MANUAL_LV1},
        {RegenStepStatus::MANUAL_LV2, SFCDetail_Contents_xEVConstantRegenStepStat::MANUAL_LV2},
        {RegenStepStatus::MANUAL_LV3, SFCDetail_Contents_xEVConstantRegenStepStat::MANUAL_LV3},
        {RegenStepStatus::MANUAL_MAX, SFCDetail_Contents_xEVConstantRegenStepStat::MANUAL_MAX},
        {RegenStepStatus::VALUE, SFCDetail_Contents_xEVConstantRegenStepStat::VALUE}};

    std::map<RegenSystemStepStatus, SFCDetail_Contents_xEVConstantRegenSystemStepStat> mRegenSystemStepStatusMap = {
        {RegenSystemStepStatus::NONE, SFCDetail_Contents_xEVConstantRegenSystemStepStat::NONE},
        {RegenSystemStepStatus::APPLIED_STEP3, SFCDetail_Contents_xEVConstantRegenSystemStepStat::APPLIED_STEP3},
        {RegenSystemStepStatus::APPLIED_STEP4, SFCDetail_Contents_xEVConstantRegenSystemStepStat::APPLIED_STEP4}};
};

}  // namespace ccos

#endif  // SFSS_Detail_Contents_xEV_H
