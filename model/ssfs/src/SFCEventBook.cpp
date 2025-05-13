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
    "E00101", "E00108", "E00501", "E00502", "E00801", "E20008", "E22102", "E22103", "E22104", "E22105", "E22106", "E22110",
    "E22111", "E22112", "E22113", "E22114", "E22115", "E22116", "E22118", "E22119", "E22120", "E22132", "E22809", "E22810",
    "E22811", "E22812", "E22813", "E22815", "E22816", "E22817", "E22818", "E22819", "E22820", "E22821", "E22822", "E22827",
    "E22828", "E22844", "E22847", "E22848", "E22849", "E22877", "E22878", "E24711", "E30606", "E60702", "E60703", "E60711",
    "E60712", "E60715", "E60716", "E60718", "E60720", "E60722", "E60723", "E60724", "E60725", "E60726", "E60727", "E60731",
    "E60732", "E60733", "E60736", "E60737", "E60738", "E60739", "E60740", "E60741", "E60742", "E60743", "E60744", "E60745",
    "E60746", "E60747", "E60748", "E60749", "E60754", "E60755", "E60756", "E61001", "E61004", "E61006", "E61007", "E61501",
    "E61601", "E61602", "E61604", "E61605", "E61606", "E61608", "E61610", "E61701", "E61702", "E22117", "E24601", "E61703"};

const std::set<std::string>& SFCEventBook::getValidEventIDsOnIgnOff() {
    return msValidEventIdsOnIgnOff;
}

bool SFCEventBook::isEventIDValidOnIgnOff(const std::string eventId) {
    auto it = msValidEventIdsOnIgnOff.find(eventId);
    return (it != msValidEventIdsOnIgnOff.end());
}

}  // namespace sfc
