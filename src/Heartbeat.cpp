#include "Heartbeat.h"
#include "FIXMessage.h"

Heartbeat::Heartbeat(const std::string& rawFixString)
    : TypedMessage(rawFixString) {
}

std::string Heartbeat::msgType() const {
    return "0";
}

std::string Heartbeat::getTestReqID() const {
    return getValue("112");
}
