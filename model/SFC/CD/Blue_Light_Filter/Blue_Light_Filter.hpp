/**
 * @file Blue_Light_Filter.hpp
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
#ifndef SFSS_Blue_Light_Filter_H
#define SFSS_Blue_Light_Filter_H

#define DLOG_ENABLED gEnableSFCLog

#include <Vehicle.h>
#include "Blue_Light_FilterBase.hpp"

namespace ccos {

// SFC Version : 0.0.0
// Reference: [CD210] Contains: Inter Recommend: VALUE_CHANGED
class Blue_Light_Filter : public Blue_Light_FilterBase {
public:
    Blue_Light_Filter() = default;
    ~Blue_Light_Filter() override = default;
    Blue_Light_Filter(const Blue_Light_Filter& other) = delete;
    Blue_Light_Filter(Blue_Light_Filter&& other) noexcept = delete;
    Blue_Light_Filter& operator=(const Blue_Light_Filter& other) = delete;
    Blue_Light_Filter& operator=(Blue_Light_Filter&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onHeadUnitBlueLightChanged(const ArgumentsHeadUnitBlueLightChanged& args) {
        mHeadUnitBlueLightOnOffStatus = args.mInput_HeadUnitBlueLightOnOffStatus;
        mHeadUnitBlueLightLevelStatus = args.mInput_HeadUnitBlueLightLevelStatus;
        updateOutput();
    }

    void onAemInitializeCompleteChanged(const ArgumentsAemInitializeCompleteChanged& args) {
        updateValueIfValid(mIsAEMInit, args.mInter_AEMInitializeComplete);
        if (mIsAEMInit == true) {
            updateOutput();
        }
    }

    void onParamChanged(const ArgumentsParamChanged& args) {
        mBlueLightFilterBlueColorValue = {0,
                                          args.mPar_BlueLightFilterBlueColorValue1,
                                          args.mPar_BlueLightFilterBlueColorValue2,
                                          args.mPar_BlueLightFilterBlueColorValue3,
                                          args.mPar_BlueLightFilterBlueColorValue4,
                                          args.mPar_BlueLightFilterBlueColorValue5,
                                          args.mPar_BlueLightFilterBlueColorValue6,
                                          args.mPar_BlueLightFilterBlueColorValue7,
                                          args.mPar_BlueLightFilterBlueColorValue8,
                                          args.mPar_BlueLightFilterBlueColorValue9};

        mBlueLightFilterGreenColorValue = {0,
                                           args.mPar_BlueLightFilterGreenColorValue1,
                                           args.mPar_BlueLightFilterGreenColorValue2,
                                           args.mPar_BlueLightFilterGreenColorValue3,
                                           args.mPar_BlueLightFilterGreenColorValue4,
                                           args.mPar_BlueLightFilterGreenColorValue5,
                                           args.mPar_BlueLightFilterGreenColorValue6,
                                           args.mPar_BlueLightFilterGreenColorValue7,
                                           args.mPar_BlueLightFilterGreenColorValue8,
                                           args.mPar_BlueLightFilterGreenColorValue9};

        mBlueLightFilterRedColorValue = {0,
                                         args.mPar_BlueLightFilterRedColorValue1,
                                         args.mPar_BlueLightFilterRedColorValue2,
                                         args.mPar_BlueLightFilterRedColorValue3,
                                         args.mPar_BlueLightFilterRedColorValue4,
                                         args.mPar_BlueLightFilterRedColorValue5,
                                         args.mPar_BlueLightFilterRedColorValue6,
                                         args.mPar_BlueLightFilterRedColorValue7,
                                         args.mPar_BlueLightFilterRedColorValue8,
                                         args.mPar_BlueLightFilterRedColorValue9};
        if (mIsAEMInit == true) {
            updateOutput();
        }
    }

    void dump(std::ostringstream& out, const std::string& prefix, const std::string& command) override {
        // no operation
    }

private:
    void updateOutput() {
        HUInt64 blueColorValue = 0;
        HUInt64 redColorValue = 0;
        HUInt64 greenColorValue = 0;
        if (mHeadUnitBlueLightOnOffStatus == HeadUnitBlueLightOnOffStatus::OFF) {
            blueColorValue = 255;
            redColorValue = 255;
            greenColorValue = 255;
        } else if (mHeadUnitBlueLightOnOffStatus == HeadUnitBlueLightOnOffStatus::ON) {
            if (mIsAEMInit == true) {
                uint64_t blueLightLevel = static_cast<uint64_t>(mHeadUnitBlueLightLevelStatus);
                blueColorValue = mBlueLightFilterBlueColorValue[blueLightLevel];
                greenColorValue = mBlueLightFilterGreenColorValue[blueLightLevel];
                redColorValue = mBlueLightFilterRedColorValue[blueLightLevel];
            }
        } else {
            // Do nothing
        }

        mAddressValueList.emplace_back(Vehicle.System.Micom.Output_BLFBlueColorValue, blueColorValue);
        mAddressValueList.emplace_back(Vehicle.System.Micom.Output_BLFGreenColorValue, greenColorValue);
        mAddressValueList.emplace_back(Vehicle.System.Micom.Output_BLFRedColorValue, redColorValue);
        if (mAddressValueList.empty() == false) {
            setValue(mAddressValueList);
            mAddressValueList.clear();
        }
    }

    using HeadUnitBlueLightOnOffStatus = ArgumentsHeadUnitBlueLightChanged::Input_HeadUnitBlueLightOnOffStatus;
    using HeadUnitBlueLightLevelStatus = ArgumentsHeadUnitBlueLightChanged::Input_HeadUnitBlueLightLevelStatus;

    HeadUnitBlueLightOnOffStatus mHeadUnitBlueLightOnOffStatus = HeadUnitBlueLightOnOffStatus::INVALID;
    HeadUnitBlueLightLevelStatus mHeadUnitBlueLightLevelStatus = HeadUnitBlueLightLevelStatus::INVALID;

    HBool mIsAEMInit = false;

    std::array<HUInt64, 10> mBlueLightFilterBlueColorValue = {
        0,
    };
    std::array<HUInt64, 10> mBlueLightFilterGreenColorValue = {
        0,
    };
    std::array<HUInt64, 10> mBlueLightFilterRedColorValue = {
        0,
    };

    AddressValueList mAddressValueList;
};

}  // namespace ccos

#endif  // SFSS_Blue_Light_Filter_H
