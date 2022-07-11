#include <mdz_net_sockets/socket_tcp.h>
#include <mdz_proto_http/httpv1_client.h>
#include <mdz_mem_vars/streamablefile.h>

using namespace Mantids::Memory::Streams;
using namespace Mantids::Network;
using namespace Mantids::Protocols::HTTP;
using namespace Mantids::Network::Sockets;

#define DEBUG_SUCCEED

int main(int argc, char *argv[])
{
    printf("[-] Starting simple HTTP client...\n");
    Socket_TCP tcpClient;
    if (!tcpClient.connectTo("127.0.0.1", 80, 15))
    {
        printf("Error connecting :(...\n");
        return -1;
    }
    printf("[+] TCP Connected to port 80 :)...\n");
    printf("[+] Setting up HTTP parameters...\n");
    fflush(stdout);

    HTTPv1_Client httpLayer(&tcpClient);
    StreamableFile output;

    //////////////////////////////////////////////////////
    // Configuring HTTP:
    httpLayer.setClientRequest("localhost","/");
    httpLayer.response().content->setStreamableOutput(&output);
    //////////////////////////////////////////////////////

    // Write the HTTP request.
    printf("[+] Receiving...\n");
    fflush(stdout);

    // Process the HTTP response.
    Parser::ErrorMSG err;
    StreamableObject::Status r = httpLayer.parseObject(&err);

    switch (err)
    {
    case Parser::PARSING_SUCCEED:
#ifdef DEBUG_SUCCEED
        printf("[+] - END - connection to %s finished with PARSING_SUCCEED (rd bytes = %llu)\n", "localhost", (long long unsigned int)r.bytesWritten);// httpLayer.getAnsBytes().bytesWritten);
#endif
        break;
    case Parser::PARSING_ERR_INIT:
        printf("[+] - ERR - connection to %s finished with PARSING_ERR_INIT\n", "localhost");
        break;
    case Parser::PARSING_ERR_READ:
        printf("[+] - ERR - connection to %s finished with PARSING_ERR_READ\n", "localhost");
        break;
    case Parser::PARSING_ERR_PARSE:
        printf("[+] - ERR - connection to %s finished with PARSING_ERR_PARSE\n", "localhost");
        break;
    default:
        break;
    }
    fflush(stdout);
    return 0;
}
