#include "httpv1_server_impl.h"
#include <stdio.h>

using namespace Mantids::Protocols;
using namespace Mantids::Protocols::HTTP;

HTTPV1_Server_Impl::HTTPV1_Server_Impl(Mantids::Memory::Streams::StreamableObject *sock) : HTTPv1_Server(sock)
{
}

HTTPV1_Server_Impl::~HTTPV1_Server_Impl()
{
}

bool HTTPV1_Server_Impl::procHTTPClientURI()
{
    return true;
}

bool HTTPV1_Server_Impl::procHTTPClientHeaders()
{
    return true;
}

Status::eRetCode HTTPV1_Server_Impl::procHTTPClientContent()
{    
    serverResponse.content.writer()->strPrintf("<h1>Welcome to my first webserver</h1><br>\n");

    serverResponse.content.writer()->strPrintf("<b>INIT</b><br>\n");
    serverResponse.content.writer()->strPrintf("Version: %s<br>\n",
                                                        clientRequest.requestLine.getHTTPVersion()->getHTTPVersionString().c_str());
    serverResponse.content.writer()->strPrintf("Resource: %s<br>\n",
                                                        clientRequest.requestLine.getURI().c_str());
    serverResponse.content.writer()->strPrintf("Virtual Host: %s<br>\n",
                                                        clientRequest.virtualHost.c_str()
                                                        );

    serverResponse.content.writer()->strPrintf("<b>END</b><br>\n");

    return HTTP::Status::S_200_OK;
}
