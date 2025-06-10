/**
 * @file IGNElapsed.hpp
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
#ifndef SFSS_IGNElapsed_H
#define SFSS_IGNElapsed_H

#define DLOG_ENABLED gEnableSFCLog

#include "IGNElapsedBase.hpp"
#include <SFCTimer.h>

namespace ccos {

// SFC Version : 0.4.0
// None
class IGNElapsed : public IGNElapsedBase {
public:
    IGNElapsed() = default;
    ~IGNElapsed() override = default;
    IGNElapsed(const IGNElapsed& other) = delete;
    IGNElapsed(IGNElapsed&& other) noexcept = delete;
    IGNElapsed& operator=(const IGNElapsed& other) = delete;
    IGNElapsed& operator=(IGNElapsed&& other) noexcept = delete;

    void onInitialize() override {
        if (mRepeatTimerIgnOffDelayStatus.create(kTimerIgnitionOffInterval100ms, this,
                                                 &IGNElapsed::onRepeatTimerIgnitionOffChanged, true) == false) {
            DWarning() << "[IGNElapsed] mRepeatTimerIgnOffDelayStatus Create Failed";
        }
        if (mRepeatTimerIgnOnDelayStatus.create(kTimerIgnitionOnInterval500ms, this, &IGNElapsed::onRepeatTimerIgnitionOnChanged,
                                                true) == false) {
            DWarning() << "[IGNElapsed] mRepeatTimerIgnOnDelayStatus Create Failed";
        }
    }

    void onSignalTimeOut(const NodeAddress& /* nodeAddress */) override {
        // no operation
    }

    void onRepeatTimerIgnitionOnChanged() {
        mTimerIgnOnElapsed += kTimerIgnitionOnInterval500ms;
        bool runIgnitionOnTimer = true;

        switch (mTimerIgnOnElapsed) {
            case 500:
                DInfo() << "IGN_ON_500ms";
                setSFCPrivateIGNElapsedElapsedOn500ms(mElapsedIndex);
                mElapsedIndex++;
                flush();
                break;
            case 3000:
                DInfo() << "IGN_ON_3000ms";
                setSFCPrivateIGNElapsedElapsedOn3000ms(mElapsedIndex);
                mElapsedIndex++;
                flush();
                break;
            case 3500:
                DInfo() << "IGN_ON_3500ms";
                setSFCPrivateIGNElapsedElapsedOn3500ms(mElapsedIndex);
                mElapsedIndex++;
                flush();
                break;
            case 4000:
                DInfo() << "IGN_ON_4000ms";
                setSFCPrivateIGNElapsedElapsedOn4000ms(mElapsedIndex);
                mElapsedIndex++;
                flush();
                break;
            case 10000:
                DInfo() << "IGN_ON_10s";
                runIgnitionOnTimer = false;
                setSFCPrivateIGNElapsedElapsedOn10s(mElapsedIndex);
                mElapsedIndex++;
                flush();
                break;
            default:
                break;
        }

        if (runIgnitionOnTimer == false) {
            mRepeatTimerIgnOnDelayStatus.stop();
        }
    }

    void onRepeatTimerIgnitionOffChanged() {
        mTimerIgnOffElapsed += kTimerIgnitionOffInterval100ms;
        bool runIgnitionOffTimer = true;

        switch (mTimerIgnOffElapsed) {
            case 100:  // notify IGN_OFF_0ms_Done at this time to keep timer logic simpler
                DInfo() << "IGN_OFF_0ms_Done";
                setSFCIGNElapsedElapsedOff0msDone(mElapsedIndex);  // [HMCP-2562]
                mElapsedIndex++;
                flush();
                break;
            case 500:
                DInfo() << "IGN_OFF_500ms";
                setSFCIGNElapsedElapsedOff0msDone(0);  // validness of ignOffDone expired here
                setSFCPrivateIGNElapsedElapsedOff500ms(mElapsedIndex);
                mElapsedIndex++;
                flush();
                break;
            case 700:
                DInfo() << "IGN_OFF_700ms";
                setSFCPrivateIGNElapsedElapsedOff700ms(mElapsedIndex);
                mElapsedIndex++;
                flush();
                break;
            case 1000:
                DInfo() << "IGN_OFF_1000ms";
                setSFCPrivateIGNElapsedElapsedOff1000ms(mElapsedIndex);
                mElapsedIndex++;
                flush();
                break;
            case 1500:
                DInfo() << "IGN_OFF_1500ms";
                runIgnitionOffTimer = false;
                setSFCPrivateIGNElapsedElapsedOff1500ms(mElapsedIndex);
                mElapsedIndex++;
                flush();
                break;
            default:
                break;
        }
        if (runIgnitionOffTimer == false) {
            mRepeatTimerIgnOffDelayStatus.stop();
        }
    }

    void onIgnitionChanged(const ArgumentsIgnitionChanged& args) {
        bool initState = ISINITIALIZEDSIGNAL(args.mIgnition1);
        if (!initState) {
            return;
        }

        mRepeatTimerIgnOffDelayStatus.stop();
        mRepeatTimerIgnOnDelayStatus.stop();
        mTimerIgnOnElapsed = 0;
        mTimerIgnOffElapsed = 0;

        if (args.mIgnition1 == Ignition::ON) {
            if (mRepeatTimerIgnOnDelayStatus.start() == false) {
                DWarning() << "[IGNElapsed] mRepeatTimerIgnOnDelayStatus Start Failed";
            }
            setSFCIGNElapsedElapsedOff0msDone(0);  // validness of ignOffDone expired here
            setSFCPrivateIGNElapsedElapsedOn0ms(mElapsedIndex);
            mElapsedIndex++;
        } else if (args.mIgnition1 == Ignition::OFF) {
            if (mRepeatTimerIgnOffDelayStatus.start() == false) {
                DWarning() << "[IGNElapsed] mRepeatTimerIgnOffDelayStatus Start Failed";
            }
            setSFCIGNElapsedElapsedOff0msDone(0);  // validness of ignOffDone expired here
            setSFCPrivateIGNElapsedElapsedOff0ms(mElapsedIndex);
            mElapsedIndex++;
        } else {
            DWarning() << "Invalid Ignition";
        }
        flush();
    }

private:
    using Ignition = ArgumentsIgnitionChanged::Ignition1;

    ssfs::SFCTimer<IGNElapsed> mRepeatTimerIgnOffDelayStatus;
    ssfs::SFCTimer<IGNElapsed> mRepeatTimerIgnOnDelayStatus;
    uint32_t mTimerIgnOnElapsed = 0;
    uint32_t mTimerIgnOffElapsed = 0;

    HUInt64 mElapsedIndex = 0;

    static constexpr uint32_t kTimerIgnitionOnInterval500ms = 500u;
    static constexpr uint32_t kTimerIgnitionOffInterval100ms = 100u;
};

}  // namespace ccos

#endif  // SFSS_IGNElapsed_H
