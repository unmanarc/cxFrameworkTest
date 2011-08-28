#include <cx_net_sockets/socket_tcp.h>
#include <cx_net_chains/chainsockets.h>
#include <cx_net_chains_tls/socketchain_aes.h>

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

void * serverReader(void * data)
{
    Socket_Base_Stream * sstream = (Socket_Base_Stream *)data;

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
    fgets(serverPassword, 255, stdin);
    removeCRLF(serverPassword);

    Socket_TCP tcpClient;
    if (!tcpClient.connectTo("127.0.0.1", 15289, 15))
    {
        printf("Error connecting :(...\n");
        return -1;
    }
    printf("[+] TCP Connected :)...\n");
    printf("[+] Handshaking AES-CBC-256+MT19937IV...\n");

    ChainSockets chainSocketAES(&tcpClient,false);
    SocketChain_AES aesChain;
    aesChain.setPhase1Key(serverPassword);
    if (!chainSocketAES.addToChain( &aesChain ))
    {
        printf("[*] Error in AES handshake...\n");
    }
    else
    {
        printf("[+] AES protocol established...\n");
        pthread_t x;
        pthread_create(&x,nullptr,&serverReader,&chainSocketAES);

        char cmd[128];
        while (1)
        {
            printf("CLIENT> ");
            fgets(cmd,128,stdin);
            chainSocketAES.writeBlock("CLIENT> ");
            chainSocketAES.writeBlock(cmd);
        }
    }



    return 0;
}
