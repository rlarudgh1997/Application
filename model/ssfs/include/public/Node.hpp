/**
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

#ifndef NODE_H
#define NODE_H

#include <string>
#include <utility>
#include <set>

#ifndef SSFS_INTERNAL
#include <HSubscription.h>
typedef std::string NodeAddress;
typedef std::string SignalName;
#ifdef SSFS_VALIDATOR
#include "AltonTypeDefine.h"
#else
namespace keyword {
constexpr const char* kVSMNodeTypeTaggedBranch = "tagged_branch";
}
#endif
#else
#include "AltonTypeDefine.h"
#include "SignalValue.h"
#endif

class Node {
public:
    Node(const Node&) = default;
    Node(Node&&) noexcept = default;
    const Node& operator=(const Node&) = delete;
    const Node& operator=(Node&&) noexcept = delete;
    virtual ~Node() = default;

    virtual operator std::string() const {
        return getAddress();
    }

    virtual bool equals(const Node& node) const {
        return mAddress == node.getAddress();
    }

    virtual bool equals(Node* node) const {
        return equals(*node);
    }

    virtual inline bool operator==(const Node& rhs) const {
        return equals(rhs);
    }

    virtual inline bool operator==(const std::string& rhs) const {
        return getAddress() == rhs;
    }

    friend std::ostream& operator<<(std::ostream& os, const Node& rhs);

    virtual NodeAddress getAddress() const {
        return mAddress;
    }

    virtual SignalName getSignalName() const {
        return mSignalName;
    }

    std::vector<Node*>& getChildNodes() {
        return mChildNodes;
    }
    std::vector<Node*>& getChildNodes(const std::set<std::string>& tag) {
        const size_t isElement = 1;

        if (getNodeType() == keyword::kVSMNodeTypeTaggedBranch) {
            for (const auto& node : mMultiTypes) {
                if (tag.count(node.first) == isElement) {
                    // 성능으로 인해 첫번째 만나는 Tag 에 대해서만 처리함.
                    return node.second->getChildNodes();
                }
            }
        }
        return mChildNodes;
    }

#if defined(SSFS_INTERNAL) || defined(SSFS_VALIDATOR)
    virtual bool translate(const SignalValue& signalValue, uint64_t& value) {
        value = signalValue.getRawValue();
        return true;
    }
    virtual bool translate(const SignalValue& signalValue, int64_t& value) {
        value = signalValue.getValueInt64();
        return true;
    }

    virtual bool reverseTranslate(const uint64_t& value, uint64_t& rawValue) {
        rawValue = value;
        return true;
    }

    virtual RawValueType getRawValueType() const {
        return RawValueType::NA;
    }

    virtual uint32_t getRawValueSize() const {
        return 64;
    }
#endif  // SSFS_INTERNAL

    virtual std::string getNodeType() const {
        return "";
    }
    std::map<std::string, Node*>& getMultiTypes() {
        return mMultiTypes;
    }

    virtual ccos::vehicle::vsm::HVehicleSignalValueType getVehicleSignalValueType() {
        return ccos::vehicle::vsm::HVehicleSignalValueType::MAX;
    }

protected:
    Node(NodeAddress address, SignalName signalName) : mAddress(std::move(address)), mSignalName(std::move(signalName)) {
    }

    std::vector<Node*> mChildNodes;
    std::map<std::string, Node*> mMultiTypes;

private:
    const NodeAddress mAddress;
    const SignalName mSignalName;

    friend class SignalCache;
};

inline std::ostream& operator<<(std::ostream& os, const Node& rhs) {
    os << "Node(" << std::hex << &rhs;
    os << ",address=" << rhs.getAddress();
    os << ",signal=" << rhs.getSignalName();
    os << ")";
    os << std::dec;
    return os;
}

inline bool operator==(const std::string& lhs, const Node& rhs) {
    return lhs == rhs.getAddress();
}

#endif  // NODE_H
