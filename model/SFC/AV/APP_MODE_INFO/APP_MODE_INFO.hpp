/**
 * @file APP_MODE_INFO.hpp
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
#ifndef SFSS_APP_MODE_INFO_H
#define SFSS_APP_MODE_INFO_H

#define DLOG_ENABLED gEnableSFCLog

#include "APP_MODE_INFOBase.hpp"

namespace ccos {

// SFC Version : 0.0.0
// Reference : [AV201] APP_MODE_INFO. Recommend: VALUE_CHANGED. Application Write, SFC Read
class APP_MODE_INFO : public APP_MODE_INFOBase {
public:
    APP_MODE_INFO() = default;
    ~APP_MODE_INFO() override = default;
    APP_MODE_INFO(const APP_MODE_INFO& other) = delete;
    APP_MODE_INFO(APP_MODE_INFO&& other) noexcept = delete;
    APP_MODE_INFO& operator=(const APP_MODE_INFO& other) = delete;
    APP_MODE_INFO& operator=(APP_MODE_INFO&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        // no operation
    }

    void dump(std::ostringstream& /*out*/, const std::string& /*prefix*/, const std::string& /*command*/) override {
        // no operation
    }

    // 5 클러스터 수신 정보 - SOME/IP Logic: Not implemented in SFC
};

}  // namespace ccos

#endif  // SFSS_APP_MODE_INFO_H
