#pragma once
#include "TypedMessage.h"
class Logon : public TypedMessage {
public:
    Logon(const std::string& rawFixString);
    string getHeartBtInt() const;
    string msgType() const override;
};
