/**
 * @file CLU_USER_SETTING_INFO.hpp
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
#ifndef SFSS_CLU_USER_SETTING_INFO_H
#define SFSS_CLU_USER_SETTING_INFO_H

#define DLOG_ENABLED gEnableSFCLog

#include "CLU_USER_SETTING_INFOBase.hpp"

namespace ccos {

// SFC Version : 1.0.0
// Reference : [AV161] CLU_USER_SETTING_INFO. Recommend: VALUE_CHANGED. Application Write, SFC Read
class CLU_USER_SETTING_INFO : public CLU_USER_SETTING_INFOBase {
public:
    CLU_USER_SETTING_INFO() = default;
    ~CLU_USER_SETTING_INFO() override = default;
    CLU_USER_SETTING_INFO(const CLU_USER_SETTING_INFO& other) = delete;
    CLU_USER_SETTING_INFO(CLU_USER_SETTING_INFO&& other) noexcept = delete;
    CLU_USER_SETTING_INFO& operator=(const CLU_USER_SETTING_INFO& other) = delete;
    CLU_USER_SETTING_INFO& operator=(CLU_USER_SETTING_INFO&& other) noexcept = delete;

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

    // 4 연산 로직 - SOME/IP Logic: Not implemented in SFC
    // 5 클러스터 수신 정보 - SOME/IP Logic: Not implemented in SFC
    // 6 클러스터 송신 정보 - SOME/IP Logic: Not implemented in SFC
    // 7 표시 로직 - N/A
};

}  // namespace ccos

#endif  // SFSS_CLU_USER_SETTING_INFO_H
