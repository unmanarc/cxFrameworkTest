#include <mdz_net_sockets/socket_tcp.h>
#include <mdz_net_sockets/acceptor_multithreaded.h>

#include <mdz_proto_http/common_urlvars.h>

#include "httpv1_server_impl.h"

using namespace Mantids::Memory::Streams;
using namespace Mantids::Memory::Containers;
using namespace Mantids::Network;
using namespace Mantids::Protocols::HTTP;
using namespace Mantids::Network::Sockets;

//#define DEBUG_SUCCEED

bool httpServerThread(void *, Sockets::Socket_StreamBase * baseClientSocket, const char * remotePair, bool secure)
{
    HTTPV1_Server_Impl httpV1Server(baseClientSocket);
    // Process the HTTP request.
    Parser::ErrorMSG err;
    httpV1Server.parseObject(&err);
    switch (err)
    {
    case Parser::PARSING_SUCCEED:
#ifdef DEBUG_SUCCEED
        printf("[%p] - END - connection from %s finished with PARSING_SUCCEED (rd bytes = %llu, wr bytes = %llu)\n", baseClientSocket, remotePair, r.bytesWritten, httpV1Server.getAnsBytes().bytesWritten);
#endif
        break;
    case Parser::PARSING_ERR_INIT:
        printf("[%p] - ERR - connection from %s finished with PARSING_ERR_INIT\n", baseClientSocket, remotePair);
        break;
    case Parser::PARSING_ERR_READ:
        printf("[%p] - ERR - connection from %s finished with PARSING_ERR_READ\n", baseClientSocket, remotePair);
        break;
    case Parser::PARSING_ERR_PARSE:
        printf("[%p] - ERR - connection from %s finished with PARSING_ERR_PARSE\n", baseClientSocket, remotePair);
        break;
    }
    fflush(stdout);
    return true;
}

int main(int , char *[])
{
    printf("[-] Testing HTTP components...\n");
    printf("    [+] URL vars parsing...\n");

    // TODO: Test MIME Parsing...
    Mantids::Memory::Containers::B_Chunks bc;
    bc.append("hello=123%3C500%&test=abc&xc=1&&&&cx");
    StreamableObject::Status x;
    Common::URLVars vars;
    bc.streamTo(&vars,x);

    printf("[-] Starting simple HTTP server...\n");

    Socket_TCP tcpServer;
    if (!tcpServer.listenOn(8087,"127.0.0.1",true))
    {
        printf("Error creating a listening server :(...\n");
        return -1;
    }
    printf("TCP server running @%d...\n", tcpServer.getPort());

    fflush(stdout);

    // STREAM MANAGER:
    Acceptors::MultiThreaded vstreamer;
    vstreamer.setAcceptorSocket(&tcpServer);
    vstreamer.setCallbackOnConnect(&httpServerThread, nullptr);
    vstreamer.startThreaded();

    while (1) sleep(60*10);
}
