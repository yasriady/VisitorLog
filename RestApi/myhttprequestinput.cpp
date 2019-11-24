#include "myhttprequestinput.h"

MyHttpRequestInput::MyHttpRequestInput()
{

}

MyHttpRequestInput::MyHttpRequestInput(QString v_url_str, QString v_http_method)
    : HttpRequestInput(v_url_str, v_http_method)
{
    add_var("X-API-KEY", "12345");
}
