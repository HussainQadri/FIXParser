#include "Logon.h"
#include "TypedMessage.h"

Logon::Logon(const string& rawFixString) : TypedMessage(rawFixString) {
    if (getValue("35") != "A") {
        throw std::invalid_argument("Not a Logon");
    }
}

std::string Logon::msgType() const {
    return "A";
}

std::string Logon::getHeartBtInt() const {
    return getValue("108");
}
