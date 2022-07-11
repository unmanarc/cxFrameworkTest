#include "server.h"
#include <mdz_net_chains/socket_chain.h>
#include <mdz_net_chains/socket_chain_aes.h>
#include <mdz_net_sockets/streams_bridge.h>
#include <string.h>

extern char serverPassword[256];

using namespace Mantids::Network;
using namespace Mantids::Network::Sockets;

bool serverThread(void *, Sockets::Socket_StreamBase * baseClientSocket, const char * remotePair, bool secure)
{
    printf("[+] %p New connection from %s:%d, starting local telnet chat...\n",baseClientSocket, remotePair, baseClientSocket->getRemotePort());

    /////////////////////////////////////////////////////////////////
    // Create a local managed telnet client.
    Socket_TCP plainTextTcpServer;
    if (!plainTextTcpServer.listenOn(0,"0.0.0.0",true))
        return true;
    /////////////////////////////////////////////////////////////////
    printf("[+] %p Client connected, establishing AES-GCM-256+MT19937IV handshake...\n",baseClientSocket);
    Sockets::Socket_Chain chainSocketAES(baseClientSocket,false);
    Sockets::ChainProtocols::Socket_Chain_AES aesChain;
    aesChain.setPhase1Key(serverPassword);
    int f = -1;
    if (!chainSocketAES.addToChain( &aesChain ))
    {
        printf("[*] Error in AES handshake...\n");
    }
    else
    {
        chainSocketAES.writeStringEx<uint8_t>("My Bogus AES-XOR Server");

        // Establish the local connection from telnet client
        char xtermcmd[512];
        sprintf( xtermcmd,"xterm -e 'telnet 127.0.0.1 %d' &", plainTextTcpServer.getPort() );
        system(xtermcmd);

        Sockets::Socket_StreamBase * localClient = plainTextTcpServer.acceptConnection();

        /////////////////////////////////////////////////////////////////
        // Establish a bridge between 2 connections...
        NetStreams::Bridge bridge;
        bridge.setPeer(0, &chainSocketAES);
        bridge.setPeer(1, localClient);
        bridge.start(false,false);
        f=bridge.wait();
        /////////////////////////////////////////////////////////////////

        delete localClient;
    }

    printf("[+] %p Bridge finished (%d)...\n",baseClientSocket, f);
    return true;
}
