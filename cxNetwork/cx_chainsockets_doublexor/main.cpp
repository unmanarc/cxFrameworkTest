#include <cx_net_sockets/socket_tcp.h>
#include <cx_net_threadedacceptor/threadedstreamacceptor.h>

#include "server.h"

int main(int argc, char *argv[])
{
    // TCP SERVER:
    printf("Starting XORED TCP server...\n");
    Socket_TCP tcpServer;
    if (!tcpServer.listenOn(15299,"127.0.0.1",true))
    {
        printf("Error creating a listening server :(...\n");
        return -1;
    }
    printf("TCP server running @%d...\n", tcpServer.getPort());

    // STREAM MANAGER:
    ThreadedStreamAcceptor vstreamer;
    vstreamer.setAcceptorSocket(&tcpServer);
    vstreamer.setCallbackOnConnect(&serverThread, nullptr);
    vstreamer.startThreaded();
    while (1) sleep(60*10);
    vstreamer.stop();
    return 0;
}
