#include "server.h"
#include <cx_net_chains/chainsockets.h>
#include <cx_net_chains/socketchain_xor.h>
#include <string.h>

bool serverThread(void *, Socket_Base_Stream * baseClientSocket, const char * remotePair)
{
    printf("%p New connection from %s:%d, creating chain...\n",baseClientSocket, remotePair, baseClientSocket->getRemotePort());

    // Create the chain here.
    // Two chained xor will return the original value (useful for testing)...
    ChainSockets chainSockets(baseClientSocket,false);
    SocketChain_XOR chainXor1, chainXor2;
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

        chainSockets.writeBlock("ECHO: ");
        chainSockets.writeBlock(data);

        fflush(stdout);
        memset(data,0,256);
    }

    printf("%p CHAINSOCKET CLOSED... (%d)\n",baseClientSocket ,r);
    fflush(stdout);

    return true;
}
