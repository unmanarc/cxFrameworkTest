#include "server.h"
#include <mdz_net_chains/socket_chain.h>
#include <mdz_net_chains/socket_chain_xor.h>
#include <string.h>

using namespace Mantids::Network;
using namespace Mantids::Network::Sockets;

bool serverThread(void *, Socket_StreamBase * baseClientSocket, const char * remotePair, bool secure)
{
    printf("%p New connection from %s:%d, creating chain...\n",baseClientSocket, remotePair, baseClientSocket->getRemotePort());

    // Create the chain here.
    // Two chained xor will return the original value (useful for testing)...
    Socket_Chain chainSockets(baseClientSocket,false);
    ChainProtocols::Socket_Chain_XOR chainXor1, chainXor2;
    chainSockets.addToChain( &chainXor1 );
    chainSockets.addToChain( &chainXor2 );

    // Process data...
    char data[256];
    memset(data,0,256);
    int r;
    while ( (r=chainSockets.partialRead(data,255))>0 )
    {
        printf("%p HEX (%d bytes): ", baseClientSocket,r);
        for (int i=0;i<r;i++)
        {
            printf("%02X ", data[i]);
        }
        printf("\n%p BIN (%d bytes): %s",baseClientSocket,r,data);

        chainSockets.writeFull("ECHO: ");
        chainSockets.writeFull(data);

        fflush(stdout);
        memset(data,0,256);
    }

    printf("%p CHAINSOCKET CLOSED... (%d)\n",baseClientSocket ,r);
    fflush(stdout);

    return true;
}
