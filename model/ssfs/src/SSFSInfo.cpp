/**
 * Copyright 2021 IVIS Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software shall be licensed only pursuant to the terms
 * of an applicable IVIS license agreement.
 */

// This source is part of AltonService.
#include "SSFSInfo.h"

#include <Vehicle.h>

#if defined(TAG_EV) && defined(TAG_FCEV) && defined(TAG_HEV) && defined(TAG_ICV) && defined(TAG_PHEV) && defined(TAG_CV)
#define USING_SFC_HEADER true
#include "SFC.h"
#else
#define USING_SFC_HEADER false
#include "SFCDataBase.h"
#endif

class SSFSInfoImpl : public SSFSInfo {
public:
    SSFSInfoImpl() = default;
    virtual ~SSFSInfoImpl() = default;

    Node* getVSMHead() override {
        return &Vehicle;
    }

    Node* getSFCHead() override {
#if USING_SFC_HEADER
        return &SFC;
#else
        return nullptr;
#endif
    }

    const std::list<NodeAddress>& getSFCAddresses() override {
#if USING_SFC_HEADER
        static std::list<NodeAddress> dummy;
        return dummy;
#else
        return ccos::getSFCAddresses();
#endif
    }
};

extern "C" {

SSFSInfo* getSSFSInfoInstance() {
    static SSFSInfoImpl ssfsInfoImpl;
    return &ssfsInfoImpl;
}

}  // extern "C"
