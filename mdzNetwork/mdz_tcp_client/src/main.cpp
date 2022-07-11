#include <mdz_net_sockets/socket_tcp.h>

using namespace Mantids::Network;
using namespace Mantids::Network::Sockets;

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
    int port;
    printf ("Enter a tcp port: ");
    scanf ("%d",&port);

    // TCP SERVER:
    printf("Starting TCP client...\n");
    Socket_TCP tcpClient;
    if (!tcpClient.connectTo("127.0.0.1", port, 15))
    {
        printf("Error connecting :(...\n");
        return -1;
    }
    printf("TCP client connected, commands are: date, pid, quit...\n");
    char cmd[128];
    fgets(cmd,128,stdin);

    while (1)
    {
        printf("INPUT CMD> ");
        fgets(cmd,128,stdin);
        removeCRLF(cmd);

        printf("Sending (%s) to server\n", cmd);
        tcpClient.writeStringEx<uint8_t>(cmd);

        bool readOK;
        std::string ans = tcpClient.readStringEx<uint8_t>(&readOK);
        if (!readOK)
        {
            printf("TCP client disconnected\n");
            return 0;
        }
        printf("Server answered: %s\n", ans.c_str());
    }


    return 0;
}
