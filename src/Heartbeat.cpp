#include "Heartbeat.h"
#include "FIXMessage.h"
#include <stdexcept>

Heartbeat::Heartbeat(const std::string& rawFixString)
    : TypedMessage(rawFixString) {

    if (getValue("35") != "0") {
        throw std::invalid_argument("Not a Heartbeat");
    }
}

std::string Heartbeat::msgType() const {
    return "0";
}

std::string Heartbeat::getTestReqID() const {
    return getValue("112");
}
