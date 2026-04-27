#pragma once

#include "TypedMessage.h"

class SequenceReset : public TypedMessage {
public:
    SequenceReset(const string& rawFixString);
    string msgType() const;
    string getGapFillFlag() const;
    string getNewSeqNo() const;
};
