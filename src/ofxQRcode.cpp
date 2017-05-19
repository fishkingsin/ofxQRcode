#include "ofxQRcode.h"

void
ofxQRcode::fetch(const std::string& data, size_t size, size_t margin, bool urlEncode)
{
    char * cstr = new char [data.length()+1];
    std::strcpy (cstr, data.c_str());
    string _data = (urlEncode ? url_encode(cstr) :data);
    
    std::stringstream googleChartsQRurl;
    googleChartsQRurl
    << "http://chart.googleapis.com/chart?"   // Google Charts Endpoint
    << "chs=" << size << "x" << size << "&"   // chart size
    << "cht=qr&"                              // chart type: QR Code
    << "chld=L|" << margin << "&"             // Error correction [L,M,Q,H], margin
    << "choe=UTF-8&"                          // Encoding
    << "chl=" << _data;                        // QR code data
    
    
    ofRegisterURLNotification(this);
    ofLoadURLAsync(googleChartsQRurl.str(), "qrcode");
}

void
ofxQRcode::urlResponse(ofHttpResponse& response)
{
    if(response.request.name == "qrcode")
    {
        if (response.status == 200)
            loadImage(response.data);
        
        ofUnregisterURLNotification(this);
    }
}


/* Converts a hex character to its integer value */
char
ofxQRcode::from_hex(char ch) {
    return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

/* Converts an integer value to its hex character*/
char
ofxQRcode::to_hex(char code) {
    static char hex[] = "0123456789abcdef";
    return hex[code & 15];
}

/* Returns a url-encoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *
ofxQRcode::url_encode(char *str) {
    char *pstr = str;
    char *buf = (char*)malloc(strlen(str) * 3 + 1);
    char *pbuf = buf;
    while (*pstr) {
        if (isalnum(*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~')
            *pbuf++ = *pstr;
        else if (*pstr == ' ')
            *pbuf++ = '+';
        else
            *pbuf++ = '%', *pbuf++ = to_hex(*pstr >> 4), *pbuf++ = to_hex(*pstr & 15);
        pstr++;
    }
    *pbuf = '\0';
    return buf;
}

/* Returns a url-decoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *
ofxQRcode::url_decode(char *str) {
    char *pstr = str;
    char *buf = (char*)malloc(strlen(str) + 1);
    char *pbuf = buf;
    while (*pstr) {
        if (*pstr == '%') {
            if (pstr[1] && pstr[2]) {
                *pbuf++ = from_hex(pstr[1]) << 4 | from_hex(pstr[2]);
                pstr += 2;
            }
        } else if (*pstr == '+') {
            *pbuf++ = ' ';
        } else {
            *pbuf++ = *pstr;
        }
        pstr++;
    }
    *pbuf = '\0';
    return buf;
}
