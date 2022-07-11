#include <mdz_net_sockets/socket_tcp.h>
#include <mdz_net_sockets/acceptor_multithreaded.h>

#include "server.h"

using namespace Mantids::Network;
using namespace Mantids::Network::Sockets;

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
    fflush(stdout);

    // STREAM MANAGER:
    Acceptors::MultiThreaded vstreamer;
    vstreamer.setAcceptorSocket(&tcpServer);
    vstreamer.setCallbackOnConnect(&serverThread, nullptr);
    vstreamer.startThreaded();
    while (1) sleep(60*10);
    vstreamer.stop();
    return 0;
}
