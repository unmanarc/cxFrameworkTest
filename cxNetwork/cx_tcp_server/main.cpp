#include <cx_net_sockets/socket_tcp.h>
#include <cx_net_threadedacceptor/threadedstreamacceptor.h>

#include "server.h"

int main(int argc, char *argv[])
{
    // TCP SERVER:
    printf("Starting TCP server...\n");
    Socket_TCP tcpServer;
    if (!tcpServer.listenOn(0,"127.0.0.1",true))
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
    printf("Accepting connections for 10 minutes...\n");
    sleep(60*10);
    vstreamer.stop();
    printf("Server finished...\n");
    return 0;
}
