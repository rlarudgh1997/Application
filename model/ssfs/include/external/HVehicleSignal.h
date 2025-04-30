/**
 * @file HVehicleSignal.h
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

#ifndef CCOS_APPLICATION_HVEHICLESIGNAL_H
#define CCOS_APPLICATION_HVEHICLESIGNAL_H

#include <HCommon.h>
#include <HVehicleTypes.h>

namespace ccos {
namespace vehicle {
namespace vsm {

/**
 * @brief HVehicleSignalValueType defines the vehicle signal value types.
 */
enum class HVehicleSignalValueType {
    UINT64,      /**< 64bit Unsigned Integer Type */
    INT64,       /**< 64bit Signed Integer Type */
    DOUBLE,      /**< 64bit Extened Precision Type */
    BOOL,        /**< Boolean Type */
    TRISTATE,    /**< Triple State Type (FALSE/TRUE/INVALID) */
    SWITCHSTATE, /**< Switch State Type (DEFAULT/OFF/ON/INVALID) */
    STRING,      /**< String Type */
    MAX
};

/**
 * @brief Provide vsm based interfaces for handling vehicle signal state.
 * @details This class is the data class for vehicle signal model.
 * @sa ccos::vehicle::vsm::HVehicleSignalModel::setSignal
 */
class HVehicleSignal {
public:
    /**
     * @brief Get vehicle signal value type.
     * @retval HVehicleSignalValueType value type.
     * @sync
     */
    HVehicleSignalValueType getValueType() const;

    /**
     * @brief Get the node path of vehicle signal.
     * @retval std::string vehicle signal node path.
     * @sync
     */
    std::string getNodePath() const;

    /**
     * @brief Get the value of vehicle signal.
     * @details T should be one of "HUInt64", "HInt64", "HDouble", "HBool", "HTriState" and "HSwitchState".
     * @retval T The vehicle signal value.
     * @sync
     */
    template <typename T>
    T getValue() const;

    /**
     * @brief Constructs an object.
     */
    HVehicleSignal();

    /**
     * @brief Constructs an object with nodePath and value.
     * @param[in] nodePath The vehicle signal node path.
     * @param[in] value The vehicle signal value.
     */
    HVehicleSignal(const std::string& nodePath, const HUInt64& value);

    /**
     * @brief Constructs an object with nodePath and value.
     * @param[in] nodePath The vehicle signal node path.
     * @param[in] value The vehicle signal value.
     */
    HVehicleSignal(const std::string& nodePath, const HInt64& value);

    /**
     * @brief Constructs an object with nodePath and value.
     * @param[in] nodePath The vehicle signal node path.
     * @param[in] value The vehicle signal value.
     */
    HVehicleSignal(const std::string& nodePath, const HBool& value);

    /**
     * @brief Constructs an object with nodePath and value.
     * @param[in] nodePath The vehicle signal node path.
     * @param[in] value The vehicle signal value.
     */
    HVehicleSignal(const std::string& nodePath, const HDouble& value);

    /**
     * @brief Constructs an object with nodePath and value.
     * @param[in] nodePath The vehicle signal node path.
     * @param[in] value The vehicle signal value.
     */
    HVehicleSignal(const std::string& nodePath, const HTriState& value);

    /**
     * @brief Constructs an object with nodePath and value.
     * @param[in] nodePath The vehicle signal node path.
     * @param[in] value The vehicle signal value.
     */
    HVehicleSignal(const std::string& nodePath, const HSwitchState& value);

    /**
     * @brief Constructs an object with nodePath and value.
     * @param[in] nodePath The vehicle signal node path.
     * @param[in] value The vehicle signal value.
     */
    HVehicleSignal(const std::string& nodePath, const std::string& value);

    /**
     * @brief Destroys an object.
     */
    ~HVehicleSignal();

    /**
     * @brief Constructs a copy of \a other.
     * @param[in] other A reference to a HVehicleSignal.
     */
    HVehicleSignal(const HVehicleSignal& other);

    /**
     * @brief Overloads assignment operator.
     * @param[in] other A reference to a HVehicleSignal.
     */
    HVehicleSignal& operator=(const HVehicleSignal& other);

    /**
     * @brief Move-constructs a HVehicleSignal instance which pointing same object with \a other.
     * @param[in] other A reference to a HVehicleSignal.
     */
    HVehicleSignal(HVehicleSignal&& other);

    /**
     * @brief Move-assigns \a other to this instance.
     * @param[in] other A reference to a HVehicleSignal.
     */
    HVehicleSignal& operator=(HVehicleSignal&& other);

    /**
     * @brief Overloads comparison operator.
     * @param[in] other A reference to a HVehicleSignal.
     * @retval TRUE If two objects's are same.
     * @retval FALSE If two objects' are different.
     */
    HBool operator==(const HVehicleSignal& other) const;

private:
    class HVehicleSignalImpl;
    std::unique_ptr<HVehicleSignalImpl> m_pImpl;
};

}  // namespace vsm
}  // namespace vehicle
}  // namespace ccos

#endif  // CCOS_VEHICLE_HVEHICLESIGNAL_H
