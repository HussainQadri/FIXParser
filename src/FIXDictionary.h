#pragma once
#include <string>
using std::string;
class FIXDictionary {
  public:
    FIXDictionary();

    bool isValidTag(const string &tag) const;

    bool isValidMsgType(const string &msgType) const;

    string getFieldName(const string &tag) const;
};
