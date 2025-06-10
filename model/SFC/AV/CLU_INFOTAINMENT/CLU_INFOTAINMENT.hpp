/**
 * @file CLU_INFOTAINMENT.hpp
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
#ifndef SFSS_CLU_INFOTAINMENT_H
#define SFSS_CLU_INFOTAINMENT_H

#define DLOG_ENABLED gEnableSFCLog

#include "CLU_INFOTAINMENTBase.hpp"

namespace ccos {

// SFC Version : 16.1.0
// Reference : [AV090] CLU_INFOTAINMENT. Recommend: VALUE_CHANGED. Application Write, SFC Read
class CLU_INFOTAINMENT : public CLU_INFOTAINMENTBase {
public:
    CLU_INFOTAINMENT() = default;
    ~CLU_INFOTAINMENT() override = default;
    CLU_INFOTAINMENT(const CLU_INFOTAINMENT& other) = delete;
    CLU_INFOTAINMENT(CLU_INFOTAINMENT&& other) noexcept = delete;
    CLU_INFOTAINMENT& operator=(const CLU_INFOTAINMENT& other) = delete;
    CLU_INFOTAINMENT& operator=(CLU_INFOTAINMENT&& other) noexcept = delete;

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
    // 6 표시 로직 - N/A
};

}  // namespace ccos

#endif  // SFSS_CLU_INFOTAINMENT_H
