#pragma once
#include <string>

class TypedMessage {
public:
    virtual ~TypedMessage() = default;
    virtual std::string msgType() const = 0;
    virtual bool validate() const = 0;
};
