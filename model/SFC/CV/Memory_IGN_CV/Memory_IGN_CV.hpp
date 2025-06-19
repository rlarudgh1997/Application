/**
 * @file Memory_IGN_CV.hpp
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
#ifndef SFSS_Memory_IGN_CV_H
#define SFSS_Memory_IGN_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "Memory_IGN_CVBase.hpp"

namespace ccos {

// SFC Version : 0.0.0
// Spec Version : v0.11
// Reference : [CV] virtual SFC to support Memory_IGN_CV variables
class Memory_IGN_CV : public Memory_IGN_CVBase {
public:
    Memory_IGN_CV() = default;
    ~Memory_IGN_CV() override = default;
    Memory_IGN_CV(const Memory_IGN_CV& other) = delete;
    Memory_IGN_CV(Memory_IGN_CV&& other) noexcept = delete;
    Memory_IGN_CV& operator=(const Memory_IGN_CV& other) = delete;
    Memory_IGN_CV& operator=(Memory_IGN_CV&& other) noexcept = delete;

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
};

}  // namespace ccos

#endif  // SFSS_Memory_IGN_CV_H
