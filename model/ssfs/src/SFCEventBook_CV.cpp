/**
 * Copyright 2022 IVIS Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software shall be licensed only pursuant to the terms
 * of an applicable IVIS license agreement.
 */
#include "SFCEventBook.h"

namespace sfc {

// (B+ on, >>NOT<< IGN1 off) -> Event ID list
// IGN1 OFF Event doesn't need to be added list below,
// Because It only could be sent out when IGN1 OFF event occured.
std::set<std::string /*EventID*/> SFCEventBook::msValidEventIdsOnIgnOff = {
    "E00501", "E00502", "E00801", "E74202", "E74212", "E74314", "E74315", "E74601", "E74508", "E74509", "E76601",
    "E76602", "E76603", "E76604", "E76605", "E76606", "E76607", "E76608", "E76609", "E76610", "E76611", "E76612",
    "E76613", "E76614", "E76615", "E76616", "E76619", "E76620", "E76902", "E76906", "E76911", "E76915", "E77201",
    "E77202", "E77203", "E77204", "E77205", "E77206", "E77207", "E78301", "E78302", "E78303", "E78304", "E78502"};

const std::set<std::string>& SFCEventBook::getValidEventIDsOnIgnOff() {
    return msValidEventIdsOnIgnOff;
}

bool SFCEventBook::isEventIDValidOnIgnOff(const std::string eventId) {
    auto it = msValidEventIdsOnIgnOff.find(eventId);
    return (it != msValidEventIdsOnIgnOff.end());
}

}  // namespace sfc
