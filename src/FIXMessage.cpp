#include "FIXMessage.h"
#include <exception>
#include <string>
using std::string;

FIXMessage::FIXMessage(const string& rawFixString) {
    Parse(rawFixString);
}

void FIXMessage::Parse(const string& rawFixString) {
    std::pair<string, string> pairValue;
    string tag;
    string value;
    bool readingTag = true;
    for (int i = 0; i < rawFixString.size(); i++) {
        char currentChar = rawFixString[i];
        if (currentChar == '\x01') {
            pairValue.first = tag;
            pairValue.second = value;
            m_tagValuePairs[tag] = value;
            tags.insert(tag);
            values.insert(value);
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

string FIXMessage::getValue(const string& tag) const {
    if (m_tagValuePairs.contains(tag)) {
        return m_tagValuePairs.at(tag);
    }
    return "";
}

std::vector<string> FIXMessage::getValues(const string& tag) const {
    std::vector<string> res;
    for (const std::pair<string, string>& currentPair : FixMessage) {
        if (currentPair.first == tag) {
            res.push_back(currentPair.second);
        }
    }

    return res;
}

size_t FIXMessage::getFieldCount() const {
    return FixMessage.size();
}

const std::vector<std::pair<string, string>>& FIXMessage::getAllFields() const {
    return FixMessage;
}

std::pair<string, string> FIXMessage::extractBodyLengthPair() const {
    for (const std::pair<string, string>& currentPair : FixMessage) {
        if (currentPair.first == "9") {
            return currentPair;
        }
    }

    return {"", ""};
}

std::pair<string, string> FIXMessage::extractChecksumPair() const {
    for (const std::pair<string, string>& currentPair : FixMessage) {
        if (currentPair.first == "10") {
            return currentPair;
        }
    }
    // Return empty 'pair' for malformed FIX string
    return {"", ""};
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

int FIXMessage::calculateMessageBodyBytes() const {
    int byteCount = 0;

    for (int i = 2; i < FixMessage.size(); i++) {
        std::pair<string, string> currentPair = FixMessage.at(i);
        if (currentPair.first == "10") {
            break;
        }

        byteCount += currentPair.first.length();
        byteCount += 1;
        byteCount += currentPair.second.length();
        byteCount += 1;
    }
    return byteCount;
}

bool FIXMessage::validate() const {
    std::pair<string, string> checkSumPair = extractChecksumPair();
    std::pair<string, string> bodyLengthPair = extractBodyLengthPair();
    if (checkSumPair.first.empty() || bodyLengthPair.first.empty()) {
        return false;
    }

    try {
        int expectedCheckSum = stoi(checkSumPair.second) % 256;
        int expectedBodyLength = stoi(bodyLengthPair.second);

        int actualCheckSum = calculateTotalBytes() % 256;
        int actualBodyLength = calculateMessageBodyBytes();

        bool checkSumValid = expectedCheckSum == actualCheckSum;
        bool bodyLengthValid = expectedBodyLength == actualBodyLength;

        if (checkSumValid && bodyLengthValid) {
            return true;
        } else {
            return false;
        }

    } catch (const std::exception&) {

        return false;
    }
}

void FIXMessage::addField(std::string& cur_message, const std::string& tag,
                          const std::string& value) {
    cur_message += tag + "=" + value + "\x01";
}
