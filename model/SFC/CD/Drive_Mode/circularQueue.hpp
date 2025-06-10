/**
 * @file circularQueue.hpp
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
#ifndef SFC_CIRCURLARQUEUE
#define SFC_CIRCURLARQUEUE

#include <initializer_list>
#include <vector>
#include <tuple>
#include <optional>

template <typename DriveMode>
class Private_DriveModeList {
public:
    Private_DriveModeList() = delete;
    explicit Private_DriveModeList(const std::initializer_list<DriveMode>&& ill) {
        for (const auto& item : ill) {
            mDriveModeList.emplace_back(item, false);
        }
        // NOTE: 저장된 Inter_DriveModeSelectStatus 값이 없다면, iter의 초깃값을 Normal로 설정
        mCurrentModeIter = mDriveModeList.begin() + 1;
        _DInfo() << "[Drive_Mode] init iter: " << static_cast<int>(std::get<kIndexDriveModeValue>(*mCurrentModeIter));
    }
    Private_DriveModeList(const Private_DriveModeList&) = delete;
    Private_DriveModeList(Private_DriveModeList&&) = delete;
    Private_DriveModeList& operator=(const Private_DriveModeList&) = delete;
    Private_DriveModeList& operator=(Private_DriveModeList&&) = delete;
    ~Private_DriveModeList() = default;

    bool enableMode(const DriveMode& mode, bool enable = true) {
        for (auto& item : mDriveModeList) {
            if (std::get<kIndexDriveModeValue>(item) == mode) {
                std::get<kIndexDriveModeEnable>(item) = enable;
                return true;
            }
        }
        return false;
    }

    std::optional<DriveMode> getCurrentMode() {
        auto iter = mCurrentModeIter;

        if (std::get<kIndexDriveModeEnable>(*iter) == true) {
            return std::get<kIndexDriveModeValue>(*iter);
        } else {
            return std::nullopt;
        }
    }

    void setCurrentMode(const DriveMode& driveMode) {
        if (mDriveModeList.empty()) {
            _DError() << "[Drive_Mode] setCurrentMode() - Drive mode list is empty";
            return;
        }

        auto iter = std::find_if(mDriveModeList.begin(), mDriveModeList.end(),
                                 [&driveMode](const auto& item) { return std::get<kIndexDriveModeValue>(item) == driveMode; });
        if (iter == mDriveModeList.end()) {
            _DError() << "[Drive_Mode] setCurrentMode() - Drive mode not found: " << static_cast<int>(driveMode);
            mCurrentModeIter = mDriveModeList.begin() + 1;
            return;
        }

        mCurrentModeIter = iter;

        _DInfo() << "[Drive_Mode] setCurrentMode() - Mode set to: "
                 << static_cast<int>(std::get<kIndexDriveModeValue>(*mCurrentModeIter));
    }

    bool verifyExistCurrentMode(const DriveMode& driveMode) {
        for (auto& item : mDriveModeList) {
            if (std::get<kIndexDriveModeValue>(item) == driveMode) {
                if (std::get<kIndexDriveModeEnable>(item) == false) {
                    return false;
                }
            }
        }
        return true;
    }

    std::optional<DriveMode> getNextEnabledMode() {
        if (mDriveModeList.empty() == true) {
            return std::nullopt;
        }

        auto iter = mCurrentModeIter;
        for (size_t i = 0; i < mDriveModeList.size(); ++i) {
            _DInfo() << "[Drive_Mode] getNextEnabledMode() iter: " << static_cast<int>(std::get<kIndexDriveModeValue>(*iter));
            ++iter;
            if (iter == mDriveModeList.end()) {
                iter = mDriveModeList.begin();
            }

            if (std::get<kIndexDriveModeEnable>(*iter) == true) {
                mCurrentModeIter = iter;
                return std::get<kIndexDriveModeValue>(*iter);
            }
        }

        return std::nullopt;
    }

    std::optional<DriveMode> getPrevEnabledMode() {
        if (mDriveModeList.empty() == true) {
            return std::nullopt;
        }

        auto iter = mCurrentModeIter;
        for (size_t i = 0; i < mDriveModeList.size(); ++i) {
            _DInfo() << "[Drive_Mode] getPrevEnabledMode() iter: " << static_cast<int>(std::get<kIndexDriveModeValue>(*iter));
            if (iter == mDriveModeList.begin()) {
                iter = mDriveModeList.end();
            }
            --iter;

            if (std::get<kIndexDriveModeEnable>(*iter) == true) {
                mCurrentModeIter = iter;
                return std::get<kIndexDriveModeValue>(*iter);
            }
        }

        return std::nullopt;
    }

    std::optional<DriveMode> getFirstEnabledMode() {
        auto iter = mDriveModeList.begin();

        while (iter != mDriveModeList.end()) {
            if (std::get<kIndexDriveModeEnable>(*iter) == true) {
                mCurrentModeIter = iter;
                return std::get<kIndexDriveModeValue>(*iter);
            }
            _DInfo() << "[Drive_Mode] getFirstEnabledMode() iter: " << static_cast<int>(std::get<kIndexDriveModeValue>(*iter));
            ++iter;
        }
        return std::nullopt;
    }

    void setFirstEnabledMode() {
        mCurrentModeIter = mDriveModeList.begin();

        while (mCurrentModeIter != mDriveModeList.end()) {
            if (std::get<kIndexDriveModeEnable>(*mCurrentModeIter) == true) {
                return;
            }
            ++mCurrentModeIter;
        }
    }

    bool checkModeActivationStatus() {
        for (auto& item : mDriveModeList) {
            if (std::get<kIndexDriveModeEnable>(item) == true) {
                return true;
            }
        }
        return false;
    }

    HUInt64 getModeActivationCount() {
        HUInt64 activeDriveModeCount = 0;
        for (auto& item : mDriveModeList) {
            if (std::get<kIndexDriveModeEnable>(item) == true) {
                activeDriveModeCount++;
            }
        }
        return activeDriveModeCount;
    }

private:
    std::vector<std::tuple<DriveMode, bool> > mDriveModeList;
    typename std::vector<std::tuple<DriveMode, bool> >::iterator mCurrentModeIter;

    static constexpr std::size_t kIndexDriveModeValue = 0;
    static constexpr std::size_t kIndexDriveModeEnable = 1;
};
#endif

#ifdef TEST_MODE
#include <iostream>
enum class OffRoadModeSamples {
    AUTO,
    SNOW,
    DEEP_SNOW,
    MUD,
    SAND,
    ROCK
};

enum class BasicModeSamples {
    ECO,
    NORMAL,
    SPORT,
    MY,
    SMART,
    SNOW
};

int TEST_main() {
    std::cout << "Test suit" << std::endl;
    Private_DriveModeList<OffRoadModeSamples> offRoadModeList;
    Private_DriveModeList<BasicModeSamples> basicModeList;
    offRoadModeList.enableMode(OffRoadModeSamples::DEEP_SNOW);
    basicModeList.enableMode(BasicModeSamples::SPORT);
    while (1) {
        char key;
        int offroad_inter_config;
        int basic_inter_config;
        std::optional<OffRoadModeSamples> offRoadModeResult;
        std::optional<BasicModeSamples> basicModeResult;

        std::cin >> key;
        switch (key) {
            case 'a':
                offRoadModeResult = offRoadModeList.getPrevEnabledMode();
                basicModeResult = basicModeList.getPrevEnabledMode();
                break;
            case 's':
                offRoadModeResult = offRoadModeList.getCurrentMode();
                basicModeResult = basicModeList.getCurrentMode();
                break;
            case 'd':
                offRoadModeResult = offRoadModeList.getNextEnabledMode();
                basicModeResult = basicModeList.getNextEnabledMode();
                break;
            case 'f':
                offRoadModeResult = offRoadModeList.getFirstEnabledMode();
                basicModeResult = basicModeList.getFirstEnabledMode();
                break;
            case 'e':
                std::cin >> offroad_inter_config;
                offRoadModeList.enableMode(static_cast<OffRoadModeSamples>(offroad_inter_config));
                std::cout << "OffRoad Mode: " << offroad_inter_config << " enabled." << std::endl;
                std::cin >> basic_inter_config;
                basicModeList.enableMode(static_cast<BasicModeSamples>(basic_inter_config));
                std::cout << "Basic Mode: " << basic_inter_config << " enabled." << std::endl;
                continue;
            case 'r':
                std::cin >> offroad_inter_config;
                offRoadModeList.enableMode(static_cast<OffRoadModeSamples>(offroad_inter_config), false);
                std::cout << "OffRoad Mode: " << offroad_inter_config << " disabled." << std::endl;
                std::cin >> basic_inter_config;
                basicModeList.enableMode(static_cast<BasicModeSamples>(basic_inter_config), false);
                std::cout << "Basic Mode: " << basic_inter_config << " disabled." << std::endl;
                continue;
        }

        if (offRoadModeResult.has_value()) {
            std::cout << "Current OffRoad Mode: " << static_cast<int>(offRoadModeResult.value()) << std::endl;
        } else {
            std::cout << "FAILED!!!" << std::endl;
        }
        if (basicModeResult.has_value()) {
            std::cout << "Current Basic Mode: " << static_cast<int>(basicModeResult.value()) << std::endl;
        } else {
            std::cout << "FAILED!!!" << std::endl;
        }
    }
}
#endif
