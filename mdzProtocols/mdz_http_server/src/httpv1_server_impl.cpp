#include "httpv1_server_impl.h"
#include <stdio.h>

using namespace Mantids::Protocols::HTTP;

HTTPV1_Server_Impl::HTTPV1_Server_Impl(Mantids::Memory::Streams::StreamableObject *sock) : HTTPv1_Server(sock)
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

Response::Status::eCode HTTPV1_Server_Impl::processclientRequest()
{    
    response().content->getStreamableOuput()->strPrintf("<h1>Welcome to my first webserver</h1><br>\n");

    response().content->getStreamableOuput()->strPrintf("<b>INIT</b><br>\n");
    response().content->getStreamableOuput()->strPrintf("Version: %s<br>\n",
                                                        request().request->getHTTPVersion()->getHTTPVersionString().c_str());
    response().content->getStreamableOuput()->strPrintf("Resource: %s<br>\n",
                                                        request().request->getURI().c_str());
    response().content->getStreamableOuput()->strPrintf("Virtual Host: %s<br>\n",
                                                        getRequestVirtualHost().c_str());

    response().content->getStreamableOuput()->strPrintf("<b>END</b><br>\n");

    return Response::Status::eCode::S_200_OK;
}
