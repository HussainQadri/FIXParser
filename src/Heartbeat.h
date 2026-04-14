#pragma once
#include "TypedMessage.h"
class Heartbeat : public TypedMessage {
public:
    Heartbeat(const std::string& rawFixString);
    string msgType() const override;
};
