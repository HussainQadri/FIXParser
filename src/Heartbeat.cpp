#include "Heartbeat.h"
#include "FIXMessage.h"
#include <iostream>

Heartbeat::Heartbeat(const std::string& rawFixString)
    : TypedMessage(rawFixString) {
}

std::string Heartbeat::msgType() const {
    return "0";
}
