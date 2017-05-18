#pragma once

#include "ofURLFileLoader.h"
#include "ofImage.h"
#include <string>
#include <sstream>

class ofxQRcode
: public ofImage
{
public:
  void fetch(const std::string& data, size_t size=320, size_t margin=1 , bool urlEncode=true);
  void urlResponse(ofHttpResponse& response);
    
    /* Converts a hex character to its integer value */
    char from_hex(char ch);
    char to_hex(char code);
    char* url_encode(char *str);
    char* url_decode(char *str);
};
