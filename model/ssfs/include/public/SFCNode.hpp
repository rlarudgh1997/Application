/**
 * Copyright 2020 IVIS Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software shall be licensed only pursuant to the terms
 * of an applicable IVIS license agreement.
 */

#ifndef SFCNODE_H_
#define SFCNODE_H_

#include "Node.hpp"

class SFCNode : public Node {
public:
    SFCNode(const NodeAddress& address, const SignalName& signalName) : Node(address, signalName) {
    }
    ~SFCNode() override = default;

private:
    std::vector<std::unique_ptr<SFCNode>> mNodesInstances;

    friend class SignalCache;
};

#endif  // SFCNODE_H_
