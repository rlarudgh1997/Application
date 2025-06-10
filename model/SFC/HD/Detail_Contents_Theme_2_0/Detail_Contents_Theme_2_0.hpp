/**
 * @file Detail_Contents_Theme_2_0.hpp
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
#ifndef SFSS_Detail_Contents_Theme_2_0_H
#define SFSS_Detail_Contents_Theme_2_0_H

#define DLOG_ENABLED gEnableSFCLog

#include "Detail_Contents_Theme_2_0Base.hpp"

namespace ccos {

// SFC Version : 0.0.0
// Reference : [HD211] Detail_Contents_Theme_2_0. Recommend: VALUE_CHANGED. Application Write, SFC Read
class Detail_Contents_Theme_2_0 : public Detail_Contents_Theme_2_0Base {
public:
    Detail_Contents_Theme_2_0() = default;
    ~Detail_Contents_Theme_2_0() override = default;
    Detail_Contents_Theme_2_0(const Detail_Contents_Theme_2_0& other) = delete;
    Detail_Contents_Theme_2_0(Detail_Contents_Theme_2_0&& other) noexcept = delete;
    Detail_Contents_Theme_2_0& operator=(const Detail_Contents_Theme_2_0& other) = delete;
    Detail_Contents_Theme_2_0& operator=(Detail_Contents_Theme_2_0&& other) noexcept = delete;

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

#endif  // SFSS_Detail_Contents_Theme_2_0_H
