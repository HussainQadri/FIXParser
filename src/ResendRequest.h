#pragma once
#include "TypedMessage.h"

class ResendRequest : public TypedMessage {
public:
    ResendRequest(const string& rawFixString);
    string msgType() const;
    string beginSeqNo() const;
    string endSeqNo() const;
};
