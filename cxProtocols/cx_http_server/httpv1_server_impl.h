#ifndef HTTPV1_SERVER_IMPL_H
#define HTTPV1_SERVER_IMPL_H

#include <cx_protocols_http/httpv1_server.h>

class HTTPV1_Server_Impl : public HTTPv1_Server
{
public:
    HTTPV1_Server_Impl(StreamableObject * sock);
    ~HTTPV1_Server_Impl();

protected:
    bool processClientURI();
    bool processClientOptions();

    HttpRetCode processclientRequest();

};

#endif // HTTPV1_SERVER_IMPL_H
