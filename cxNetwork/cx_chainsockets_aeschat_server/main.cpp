#include <cx_net_sockets/socket_tcp.h>
#include <cx_net_threadedacceptor/threadedstreamacceptor.h>

#include "server.h"

char serverPassword[256];

void removeCRLF(char * val)
{
    while (val && *val)
    {
        if (*val==10 || *val==13)
        {
            *val = 0;
            return;
        }
        val++;
    }
}

int main(int argc, char *argv[])
{
    // TCP SERVER:
    printf("[-] Starting AES-CHAT Server...\n");

    printf("Introduce server password: ");
    fgets(serverPassword, 255, stdin);
    removeCRLF(serverPassword);


    Socket_TCP tcpServer;
    if (!tcpServer.listenOn(15289,"127.0.0.1",true))
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
