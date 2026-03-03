#include <iostream>
#include <string>
#include <vector>
using std::string;

class FIXMessage {
  private:
    std::vector<std::pair<string, string>> FixMessage;

  public:
    FIXMessage(std::vector<std::pair<string, string>> message) {
        FixMessage = message;
    }

    string getTagAtIndex(int i) { return FixMessage.at(i).first; }

    string getValueAtIndex(int i) { return FixMessage.at(i).second; }
};

std::vector<std::pair<string, string>> Parse(string rawFixString) {
    std::pair<string, string> pairValue;
    std::vector<std::pair<string, string>> message;
    string tag;
    string value;
    bool readingTag = true;
    for (int i = 0; i < rawFixString.size(); i++) {
        char currentChar = rawFixString[i];
        if (currentChar == '|') {
            pairValue.first = tag;
            pairValue.second = value;
            message.push_back(pairValue);
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
    return message;
}

int main() {
    string testMessage = "8=FIX.4.2|9=49|35=0|49=SENDER|56=TARGET|34=1|52="
                         "20231024-15:30:00.000|10=161|";
    std::vector<std::pair<string, string>> parseResult = Parse(testMessage);

    for (std::pair<string, string> pair : parseResult) {
        std::cout << "Tag = " << pair.first << " Value = " << pair.second
                  << "\n";
    }

    FIXMessage fixMessage(parseResult);
    std::cout << fixMessage.getTagAtIndex(0);

    return 0;
}
