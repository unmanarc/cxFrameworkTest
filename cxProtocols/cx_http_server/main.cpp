#include <cx_net_sockets/socket_tcp.h>
#include <cx_net_threadedacceptor/threadedstreamacceptor.h>

#include <cx_protocols_urlvars/url_vars.h>

#include "httpv1_server_impl.h"

//#define DEBUG_SUCCEED

bool httpServerThread(void *, Socket_Base_Stream * baseClientSocket, const char * remotePair)
{
    HTTPV1_Server_Impl httpV1Server(baseClientSocket);
    // Process the HTTP request.
    ParseErrorMSG err;
    httpV1Server.parseObject(&err);
    switch (err)
    {
    case PROT_PARSER_SUCCEED:
#ifdef DEBUG_SUCCEED
        printf("[%p] - END - connection from %s finished with PROT_PARSER_SUCCEED (rd bytes = %llu, wr bytes = %llu)\n", baseClientSocket, remotePair, r.bytesWritten, httpV1Server.getAnsBytes().bytesWritten);
#endif
        break;
    case PROT_PARSER_ERR_INIT:
        printf("[%p] - ERR - connection from %s finished with PROT_PARSER_ERR_INIT\n", baseClientSocket, remotePair);
        break;
    case PROT_PARSER_ERR_READ:
        printf("[%p] - ERR - connection from %s finished with PROT_PARSER_ERR_READ\n", baseClientSocket, remotePair);
        break;
    case PROT_PARSER_ERR_PARSE:
        printf("[%p] - ERR - connection from %s finished with PROT_PARSER_ERR_PARSE\n", baseClientSocket, remotePair);
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
    BinaryContainer_Chunks bc;
    bc.append("hello=123%3C500%&test=abc&xc=1&&&&cx");
    WRStatus x;
    URL_Vars vars;
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
    ThreadedStreamAcceptor vstreamer;
    vstreamer.setAcceptorSocket(&tcpServer);
    vstreamer.setCallbackOnConnect(&httpServerThread, nullptr);
    vstreamer.startThreaded();

    while (1) sleep(60*10);
}
