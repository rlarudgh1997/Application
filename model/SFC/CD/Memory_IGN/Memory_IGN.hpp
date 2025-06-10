/**
 * @file Memory_IGN.hpp
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
#ifndef SFSS_Memory_IGN_H
#define SFSS_Memory_IGN_H

#define DLOG_ENABLED gEnableSFCLog

#include "Memory_IGNBase.hpp"

namespace ccos {

// SFC Version : 5.3.0
// Reference : [CD] virtual SFC to support Memory_IGN variables
class Memory_IGN : public Memory_IGNBase {
public:
    Memory_IGN() = default;
    ~Memory_IGN() override = default;
    Memory_IGN(const Memory_IGN& other) = delete;
    Memory_IGN(Memory_IGN&& other) noexcept = delete;
    Memory_IGN& operator=(const Memory_IGN& other) = delete;
    Memory_IGN& operator=(Memory_IGN&& other) noexcept = delete;

    void onInitialize() override {
        // no operation
    }

    void onSignalTimeOut(const NodeAddress& /*nodeAddress*/) override {
        // no operation
    }

    void onIgnElapseOn0msChanged(const ArgumentsIgnElapseOn0msChanged& /*args*/) {
        // no operation
    }
};

}  // namespace ccos

#endif  // SFSS_Memory_IGN_H
