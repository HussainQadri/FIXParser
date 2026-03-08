#include "FIXMessage.h"
#include <string>
using std::string;

FIXMessage::FIXMessage(const string &rawFixString) { Parse(rawFixString); }

void FIXMessage::Parse(const string &rawFixString) {
    std::pair<string, string> pairValue;
    string tag;
    string value;
    bool readingTag = true;
    for (int i = 0; i < rawFixString.size(); i++) {
        char currentChar = rawFixString[i];
        if (currentChar == '\x01') {
            pairValue.first = tag;
            pairValue.second = value;
            FixMessage.push_back(pairValue);
            tag = "";
            value = "";
            readingTag = true;
        }

        else if (currentChar == '=') {
            readingTag = false;
            continue;
        }

        else {
            if (readingTag) {
                tag += rawFixString[i];
            } else {
                value += rawFixString[i];
            }
        }
    }
}

string FIXMessage::getTagAtIndex(size_t i) const {
    return FixMessage.at(i).first;
}

string FIXMessage::getValueAtIndex(size_t i) const {
    return FixMessage.at(i).second;
}

size_t FIXMessage::getFieldCount() const { return FixMessage.size(); }

const std::vector<std::pair<string, string>> &FIXMessage::getAllFields() const {
    return FixMessage;
}

std::pair<string, string> FIXMessage::extractChecksumPair() const {

    std::pair<string, string> currentPair = FixMessage.at(0);
    int i = 0;
    while (currentPair.first != "10") {
        i = i + 1;
        currentPair = FixMessage.at(i);
    }
    return currentPair;
}

int FIXMessage::calculateTotalBytes() const {
    std::pair<string, string> currentPair = FixMessage.at(0);
    int i = 0;
    int byteValue = 0;

    while (currentPair.first != "10") {
        for (char c : currentPair.first) {
            byteValue += static_cast<int>(c);
        }
        byteValue += static_cast<int>('=');
        for (char c : currentPair.second) {
            byteValue += static_cast<int>(c);
        }

        byteValue += 1;

        i = i + 1;
        currentPair = FixMessage.at(i);
    }
    return byteValue;
}

bool FIXMessage::validate() const {
    return stoi(extractChecksumPair().second) == (calculateTotalBytes() % 256);
}
