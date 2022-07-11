#include <mdz_net_sockets/socket_tcp.h>
#include <mdz_net_chains/socket_chain.h>
#include <mdz_net_chains/socket_chain_aes.h>

using namespace Mantids::Network;
using namespace Mantids::Network::Sockets;
using namespace Mantids::Network::Sockets;

char serverPassword[256] = {0};

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

void * serverReader(void * data)
{
    Sockets::Socket_StreamBase * sstream = (Sockets::Socket_StreamBase *)data;

    char xdata[1024];
    memset(xdata,0,1024);
    while (sstream->partialRead(xdata,1024)>0)
    {
        printf("\nSERVER> %s", xdata);
        memset(xdata,0,1024);
    }

    printf("\nSERVER FINISHED\n");
    sleep(10);

    _exit(0);

    pthread_exit(nullptr);
    return nullptr;
}

int main(int argc, char *argv[])
{
    printf("[-] Starting AES-CHAT Client...\n");

    printf("Introduce server password: ");
    fflush(stdout);
    fgets(serverPassword, 255, stdin);
    removeCRLF(serverPassword);

    Socket_TCP tcpClient;
    if (!tcpClient.connectTo("127.0.0.1", 15289, 15))
    {
        printf("Error connecting :(...\n");
        return -1;
    }
    printf("[+] TCP Connected :)...\n");
    printf("[+] Handshaking AES-GCM-256+MT19937IV...\n");
    fflush(stdout);

    Socket_Chain chainSocketAES(&tcpClient,false);
    ChainProtocols::Socket_Chain_AES aesChain;
    aesChain.setPhase1Key(serverPassword);
    if (!chainSocketAES.addToChain( &aesChain ))
    {
        printf("[*] Error in AES handshake...\n");
        fflush(stdout);
    }
    else
    {
        std::string server_banner = chainSocketAES.readStringEx<uint8_t>();

        printf("[+] AES protocol established to (%s)...\n", server_banner.c_str());

        fflush(stdout);
        pthread_t x;
        pthread_create(&x,nullptr,&serverReader,&chainSocketAES);

        char cmd[128];
        while (1)
        {
            printf("CLIENT> ");
            fflush(stdout);
            fgets(cmd,128,stdin);
            chainSocketAES.writeFull("CLIENT> ");
            chainSocketAES.writeFull(cmd);
        }
    }



    return 0;
}
