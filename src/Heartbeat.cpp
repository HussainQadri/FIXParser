#include "Heartbeat.h"
#include "FIXMessage.h"
#include <iostream>

Heartbeat::Heartbeat(const std::string& rawFixString)
    : TypedMessage(rawFixString),
      m_mandatoryTags{"8", "9", "35", "49", "56", "34", "52", "10"} {
    if (validate()) {
        std::cout << "Valid heartbeat" << "\n";
    }
}

bool Heartbeat::validate() const {
    // will actually implement, validate will check if header and trailer
    // contains the required fields for header and trailer.
    return true;
}

std::string Heartbeat::msgType() const {
    return "0";
}

bool Heartbeat::containsAllMandatoryTags(
    std::set<std::string> m_mandatoryTags) {

    for (const std::string& tag : m_mandatoryTags) {
        if (!tags.contains(tag)) {
            return false;
        }
    }

    return true;
}
