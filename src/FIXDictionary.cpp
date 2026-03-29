#include "FIXDictionary.h"
#include <iostream>
#include <pugixml.hpp>
#include <string>
using std::string;

FIXDictionary::FIXDictionary() {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(FIX_DICTIONARY_PATH);
    std::cout << "Load result: " << result.description() << "\n";
}
