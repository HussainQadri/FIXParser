#pragma once
#include <string>
#include <utility>
#include <vector>
using std::string;
class FIXMessage {
  public:
    FIXMessage(const string &rawFixString);

    string getTagAtIndex(size_t i) const;

    string getValueAtIndex(size_t i) const;

    size_t getFieldCount() const;

    const std::vector<std::pair<string, string>> &getAllFields() const;

    bool validate() const;

  private:
    std::vector<std::pair<string, string>> FixMessage;
    void Parse(const string &rawFixString);
    std::pair<string, string> extractChecksumPair() const;
    int calculateTotalBytes() const;
};
