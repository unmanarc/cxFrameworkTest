#include "httpv1_server_impl.h"
#include <stdio.h>

HTTPV1_Server_Impl::HTTPV1_Server_Impl(StreamableObject *sock) : HTTPv1_Server(sock)
{
}

HTTPV1_Server_Impl::~HTTPV1_Server_Impl()
{
}

bool HTTPV1_Server_Impl::processClientURI()
{
    return true;
}

bool HTTPV1_Server_Impl::processClientOptions()
{
    return true;
}

HttpRetCode HTTPV1_Server_Impl::processclientRequest()
{
    output()->printf("<h1>Welcome to my first webserver</h1><br>\n");

    output()->printf("<b>INIT</b><br>\n");
    output()->printf("Version: %s<br>\n", client().request->getHTTPVersion()->getHTTPVersionString().c_str());
    output()->printf("Resource: %s<br>\n", getURI().c_str());
    output()->printf("Virtual Host: %s<br>\n", getVirtualHost().c_str());

    output()->printf("<b>END</b><br>\n");

    return HTTP_RET_200_OK;
}
