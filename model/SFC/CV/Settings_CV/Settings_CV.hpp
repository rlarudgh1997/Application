/**
 * @file Settings_CV.hpp
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
#ifndef SFSS_Settings_CV_H
#define SFSS_Settings_CV_H

#define DLOG_ENABLED gEnableSFCLog

#include "Settings_CVBase.hpp"

namespace ccos {

// SFC Version : 3.0.0
// Spec Version : v0.28
// Reference : [CV060] Settings_CV. Recommend: VALUE_CHANGED. Application Write, SFC Read
class Settings_CV : public Settings_CVBase {
public:
    Settings_CV() = default;
    ~Settings_CV() override = default;
    Settings_CV(const Settings_CV& other) = delete;
    Settings_CV(Settings_CV&& other) noexcept = delete;
    Settings_CV& operator=(const Settings_CV& other) = delete;
    Settings_CV& operator=(Settings_CV&& other) noexcept = delete;

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

#endif  // SFSS_Settings_CV_H
