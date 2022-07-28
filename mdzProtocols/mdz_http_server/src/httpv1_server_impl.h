#ifndef HTTPV1_SERVER_IMPL_H
#define HTTPV1_SERVER_IMPL_H

#include <mdz_proto_http/httpv1_server.h>

class HTTPV1_Server_Impl : public Mantids::Protocols::HTTP::HTTPv1_Server
{
public:
    HTTPV1_Server_Impl(Mantids::Memory::Streams::StreamableObject * sock);
    ~HTTPV1_Server_Impl();

protected:
    bool procHTTPClientURI();
    bool procHTTPClientHeaders();

    Mantids::Protocols::HTTP::Status::eRetCode procHTTPClientContent();

};

#endif // HTTPV1_SERVER_IMPL_H
