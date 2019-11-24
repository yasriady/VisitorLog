#ifndef MYHTTPREQUESTINPUT_H
#define MYHTTPREQUESTINPUT_H

#include "httprequestworker.h"


class MyHttpRequestInput : public HttpRequestInput
{
public:
    MyHttpRequestInput();
    MyHttpRequestInput(QString v_url_str, QString v_http_method);
};

#endif // MYHTTPREQUESTINPUT_H
