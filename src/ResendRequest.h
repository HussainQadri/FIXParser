#pragma once
#include "TypedMessage.h"

class ResendRequest : public TypedMessage {
    ResendRequest(const string& rawFixString);
    string msgType() const;
};
