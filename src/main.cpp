#include "FIXDictionary.h"
#include "FIXMessage.h"
#include <iostream>

int main() {
    FIXDictionary fixDictionary;
    string testMessage = "8=FIX.4.2\x01"
                         "9=118\x01"
                         "35=D\x01"
                         "49=ONIXS\x01"
                         "56=CME\x01"
                         "34=2\x01"
                         "52=20240528-09:20:52.111\x01"
                         "11=983532-3\x01"
                         "21=1\x01"
                         "38=100\x01"
                         "55=NVDA\x01"
                         "40=1\x01"
                         "54=1\x01"
                         "60=20240528-09:20:52.004\x01"
                         "10=017\x01";

    FIXMessage fixMessage(testMessage);
    std::cout << fixDictionary.getFieldName("60") << "\n";
    if (fixMessage.validate()) {
        std::cout << "The fix message is valid!\n";
    } else {
        std::cout << "The fix message is not valid!\n";
    }

    for (int i = 0; i < fixMessage.getFieldCount(); i++) {
        std::cout << "Tag: " << fixMessage.getTagAtIndex(i)
                  << " Value: " << fixMessage.getValueAtIndex(i) << std::endl;
    }

    for (int i = 0; i < fixMessage.getFieldCount(); i++) {
        std::cout << fixDictionary.getFieldName(fixMessage.getTagAtIndex(i))
                  << "=" << fixMessage.getValueAtIndex(i) << "\n";
    }

    return 0;
}
