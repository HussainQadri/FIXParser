#include "ResendRequest.h"
#include "TypedMessage.h"

ResendRequest::ResendRequest(const string& rawFixString)
    : TypedMessage(rawFixString) {
    if (getValue("35") != "2") {
        throw std::invalid_argument("Not a ResendRequest");
    }
}

std::string ResendRequest::msgType() const {
    return "2";
}
