#include <mdz_net_sockets/socket_tls.h>

#include "client_certs.h"

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

int connectMe(Socket_TLS * tlsClient, uint16_t port)
{

    if (!tlsClient->connectTo("127.0.0.1", port, 15))
    {
        printf(" [-] Error connecting :(\n%s\n", tlsClient->getLastError().c_str());
        auto errors = tlsClient->getTLSErrorsAndClear();
        printf(" [-] Outgoing connection failed: \n");
        for (auto & i: errors)
        {
            printf(" [-]TLS Error: %s\n",i.c_str());
        }
        fflush(stdout);
        return -1;
    }


    printf(" [-] TLS client connected using %s, commands are: date, pid, quit...\n", tlsClient->getTLSConnectionCipherName().c_str());


    char cmd[128]= "";

    while (1)
    {
        printf(" [-] INPUT CMD> ");
        fgets(cmd,128,stdin);
        removeCRLF(cmd);

        printf(" [-] Sending (%s) to server\n", cmd);
        tlsClient->writeStringEx<uint8_t>(cmd);

        bool readOK;
        std::string ans = tlsClient->readStringEx<uint8_t>(&readOK);
        if (!readOK)
        {
            printf(" [-] TLS client disconnected\n");
            return 0;
        }
        printf(" [-] Server answered: %s\n", ans.c_str());
    }

}

int connect_pki(bool useCerts)
{
    Socket_TLS tlsClient;

    // Will validate the server against the CA:
    if (!tlsClient.keys.loadCAFromPEMMemory( ca_crt, "MYTLSCLIENT" ))
    {
        printf(" [-] Error setting up the certification authority... :(\n");
        return -1;
    }

    if (useCerts)
    {

        if (!tlsClient.keys.loadPrivateKeyFromPEMMemoryEP( mytlsclient_key, "abcd1234" ))
        {
            printf(" [-] Unable to setup the private key... :(\n");
            return -1;
        }
        if (!tlsClient.keys.loadPublicKeyFromPEMMemory( mytlsclient_crt ))
        {
            printf(" [-] Unable to setup the public key... :(\n");
            return -1;
        }
    }

    return connectMe(&tlsClient, 10002);
}


int connect_psk(bool goodPSK)
{
    // TLS client:
    Socket_TLS tlsClient;

    tlsClient.keys.loadPSKAsClient("myclient", goodPSK?"MyB4d#TLSS3rver!":"INVALIDPSK");

    return connectMe(&tlsClient, 10001);
}



int main(int argc, char *argv[])
{
    printf("[+] Connecting using TLS PKI client...\n");
    printf( " [-] PKI returned with value: %d\n", connect_pki(true));

    printf("[+] Connecting using TLS PKI without certificates client (must fail)...\n");
    printf( " [-] PKI returned with value: %d\n", connect_pki(false));

    printf("[+] Connecting using TLS PSK client...\n");
    printf( " [-]PSK returned with value: %d\n", connect_psk(true));

    printf("[+] Connecting using TLS PSK client with invalid PSK...\n");
    printf( " [-]PSK returned with value: %d\n", connect_psk(false));

    return 0;
}
