/**
 * Copyright 2022 IVIS Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software shall be licensed only pursuant to the terms
 * of an applicable IVIS license agreement.
 */

#ifndef SFC_EVENT_BOOK_H_
#define SFC_EVENT_BOOK_H_

#include <string>
#include <set>

namespace sfc {

class SFCEventBook {
public:
    static const std::set<std::string>& getValidEventIDsOnIgnOff();
    static bool isEventIDValidOnIgnOff(const std::string eventId);

private:
    static std::set<std::string /*EventID*/> msValidEventIdsOnIgnOff;
};

}  // namespace sfc

#endif  // SFC_EVENT_BOOK_H_
