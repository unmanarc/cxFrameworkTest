#include "server.h"
#include <cx_net_chains/chainsockets.h>
#include <cx_net_chains_tls/socketchain_aes.h>
#include <cx_net_sockets/socket_stream_bridge.h>
#include <string.h>

extern char serverPassword[256];

bool serverThread(void *, Socket_Base_Stream * baseClientSocket, const char * remotePair)
{
    printf("[+] %p New connection from %s:%d, starting local telnet chat...\n",baseClientSocket, remotePair, baseClientSocket->getRemotePort());

    /////////////////////////////////////////////////////////////////
    // Create a local managed telnet client.
    Socket_TCP tcpServer;
    if (!tcpServer.listenOn(0,"0.0.0.0",true))
        return true;
    /////////////////////////////////////////////////////////////////
    printf("[+] %p Client connected, establishing AES-CBC-256+MT19937IV handshake...\n",baseClientSocket);
    ChainSockets chainSocketAES(baseClientSocket,false);
    SocketChain_AES aesChain;
    aesChain.setPhase1Key(serverPassword);
    int f = -1;
    if (!chainSocketAES.addToChain( &aesChain ))
    {
        printf("[*] Error in AES handshake...\n");
    }
    else
    {
        // Establish the local connection from telnet client
        char xtermcmd[512];
        sprintf( xtermcmd,"xterm -e 'telnet 127.0.0.1 %d' &", tcpServer.getPort() );
        system(xtermcmd);

        Socket_Base_Stream * localClient = tcpServer.acceptConnection();

        /////////////////////////////////////////////////////////////////
        // Establish a bridge between 2 connections...
        Socket_Stream_Bridge bridge;
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
