#pragma once
#include "TypedMessage.h"

class Logout : public TypedMessage {
    Logout(const string& rawFixString);
    string msgType() const;
};
