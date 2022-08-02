#include <mdz_net_sockets/socket_tcp.h>
#include <mdz_net_sockets/acceptor_multithreaded.h>

#include "server.h"

using namespace Mantids::Network;
using namespace Mantids::Network::Sockets;

int main(int argc, char *argv[])
{
    // TCP SERVER:
    printf("Starting TCP server...\n");
    Socket_TCP * tcpServer = new Socket_TCP;
    if (!tcpServer->listenOn(0,"127.0.0.1",true))
    {
        printf("Error creating a listening server :(...\n");
        return -1;
    }
    printf("TCP server running @%d...\n", tcpServer->getPort());

    // STREAM MANAGER:
    Acceptors::MultiThreaded vstreamer(tcpServer, &serverThread);
    vstreamer.startThreaded();
    printf("Accepting connections for 10 minutes...\n");
    fflush(stdout);
    sleep(60*10);
    vstreamer.stop();
    printf("Server finished...\n");
    return 0;
}
