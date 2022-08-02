#include <mdz_net_sockets/socket_tls.h>
#include <mdz_net_sockets/acceptor_multithreaded.h>

#include "server.h"
#include "server_certs.h"

using namespace Mantids::Network;
using namespace Mantids::Network::Sockets;

Acceptors::MultiThreaded * createServerPSK()
{
    Socket_TLS * tlsServer = new Socket_TLS;
    printf("Creating PSK TLS server...\n");

    tlsServer->keys.addPSKToServer("myclient", "MyB4d#TLSS3rver!");
    tlsServer->keys.addPSKToServer("myclient2", "t3zt123123123!!");

    if (!tlsServer->listenOn(10001,"127.0.0.1",true))
    {
        printf("Error creating a listening server :(...\n");
        return nullptr;
    }

    printf("PSK TLS server listenning @%d...\n", tlsServer->getPort());

    Acceptors::MultiThreaded * cMTAcceptor = new Acceptors::MultiThreaded(tlsServer, &serverThread,0 , &serverFailed);
    cMTAcceptor->startThreaded();

    return cMTAcceptor;
}

Acceptors::MultiThreaded * createServerPKI()
{
    Socket_TLS * tlsServer = new Socket_TLS;
    printf("Creating PKI TLS server...\n");

    // Will validate the client against the CA:
    if (!tlsServer->keys.loadCAFromPEMMemory( ca_crt, "MYTLSSERVER" ))
    {
        printf("Error setting up the certification authority... :(\n");
        delete tlsServer;
        return nullptr;
    }

    // Server Certificates:
    if (!tlsServer->keys.loadPrivateKeyFromPEMMemoryEP( mytlsserver_key, "abcd1234" ))
    {
        printf("Unable to setup the private key... :(\n");
        delete tlsServer;
        return nullptr;
    }
    if (!tlsServer->keys.loadPublicKeyFromPEMMemory( mytlsserver_crt ))
    {
        printf("Unable to setup the public key... :(\n");
        delete tlsServer;
        return nullptr;
    }

    if (!tlsServer->listenOn(10002,"127.0.0.1",true))
    {
        printf("Error creating a listening server :(...\n");
        delete tlsServer;
        return nullptr;
    }

    printf("PSK TLS server listenning @%d...\n", tlsServer->getPort());

    Acceptors::MultiThreaded * cMTAcceptor = new Acceptors::MultiThreaded(tlsServer, &serverThread,0 , &serverFailed);
    cMTAcceptor->startThreaded();

    return cMTAcceptor;
}


int main(int argc, char *argv[])
{
    // TLS SERVER:
    auto * pskServer = createServerPSK();
    auto * pkiServer = createServerPKI();

    fflush(stdout);
    printf("Accepting connections for 10 minutes...\n");
    sleep(60*10);

    pskServer->stop();
    delete pskServer;
    pkiServer->stop();
    delete pkiServer;

    printf("Server finished...\n");
    return 0;
}
