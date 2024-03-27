// #pragma once

// #ifndef SFCSIGNALDATAGETTER_H__
// #define SFCSIGNALDATAGETTER_H__

// #include <string>
// #include <unordered_map>
// // #include "ValidatorSignalValue.hpp"
// // #include "HVehicleSignal.h"
// // #include "Node.hpp"

// #include "ClusterTestBackendLog.h"

// using namespace cTest;

// class SFCSignalDataGetter {
// public:
//     static SFCSignalDataGetter& get();
//     bool isNodeExist(const std::string& addressName);
//     // ccos::vehicle::vsm::HVehicleSignalValueType getSignalValueType(const std::string& addressName);

// private:
//     SFCSignalDataGetter();
//     void resolveNodeInfo(std::vector<Node*>& childs);
//     std::unordered_map<std::string, Node*> mNodeFromAddress;

//     // make it as a multithread class;
// };

// #endif  // SFCSIGNALDATAGETTER_H__