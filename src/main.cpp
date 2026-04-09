#include "FIXDictionary.h"
#include "FIXMessage.h"
#include "Heartbeat.h"
#include <iostream>
#include <string>
using std::string;
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

    string heartbeat = "8=FIX.4.2\x01"
                       "9=58\x01"
                       "35=0\x01"
                       "49=BuySide\x01"
                       "56=SellSide\x01"
                       "34=4\x01"
                       "52=20190605-12:19:52.060\x01"
                       "10=165\x01";

    Heartbeat hb(heartbeat);
    FIXMessage fixMessage(testMessage);
    if (fixMessage.validate()) {
        std::cout << "The fix message is valid!\n";
    } else {
        std::cout << "The fix message is not valid!\n";
    }

    for (int i = 0; i < fixMessage.getFieldCount(); i++) {
        string tag = fixMessage.getTagAtIndex(i);
        string tagMeaning = fixDictionary.getFieldName(tag);
        string value = fixMessage.getValueAtIndex(i);

        std::cout << tagMeaning << "=" << value << " "
                  << fixDictionary.getEnumDescription(tag, value) << "\n";
    }

    return 0;
}
