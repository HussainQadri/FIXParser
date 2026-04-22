#include "Logout.h"
#include "TypedMessage.h"

Logout::Logout(const string& rawFixString) : TypedMessage(rawFixString) {
    if (getValue("35") != "5") {
        throw std::invalid_argument("Not a Logout");
    }
}

std::string Logout::msgType() const {
    return "5";
}
