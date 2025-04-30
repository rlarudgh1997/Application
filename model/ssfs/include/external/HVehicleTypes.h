/**
 * @file HVehicleTypes.h
 *
 * Copyright (c) 2019-2021  Hyundai Motor Company,
 * All Rights Reserved.
 *
 * Use and copying of this software and preparation of derivative works
 * based upon this software are permitted. Any copy of this software or
 * of any derivative work must include the above copyright notice, this
 * paragraph and the one after it.  Any distribution of this software or
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

#ifndef CCOS_VEHICLE_HVEHICLETYPES_H
#define CCOS_VEHICLE_HVEHICLETYPES_H

#include <HCommonTypes.h>

namespace ccos {
namespace vehicle {

/**
 * @brief HTriState defines the vehicle bool.
 */
enum class HTriState {
    OFF,     /**< Off(FALSE) */
    ON,      /**< On(TRUE) */
    INVALID, /**< Invalid(Error Indicator) */
    MAX
};

/**
 * @brief HSwitchState defines the setting state.
 */
enum class HSwitchState {
    DEFAULT, /**< Default or None */
    OFF,     /**< Off */
    ON,      /**< On */
    INVALID, /**< Invalid(Error Indicator) */
    MAX,
};

}  // namespace vehicle
}  // namespace ccos

#endif  // CCOS_VEHICLE_HVEHICLETYPES_H
