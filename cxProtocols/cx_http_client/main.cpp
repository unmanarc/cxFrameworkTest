#include <cx_net_sockets/socket_tcp.h>
#include <cx_protocols_http/httpv1_client.h>
#include <cx_mem_streams/streamablefd.h>

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
    StreamableFD output;

    //////////////////////////////////////////////////////
    // Configuring HTTP:
    httpLayer.setClientRequest("localhost","/");
    httpLayer.server().content->setStreamableOutput(&output);
    //////////////////////////////////////////////////////

    // Write the HTTP request.
    printf("[+] Receiving...\n");
    fflush(stdout);

    // Process the HTTP response.
    ParseErrorMSG err;
    WRStatus r = httpLayer.parseObject(&err);

    switch (err)
    {
    case PROT_PARSER_SUCCEED:
#ifdef DEBUG_SUCCEED
        printf("[+] - END - connection to %s finished with PROT_PARSER_SUCCEED (rd bytes = %llu)\n", "localhost", (long long unsigned int)r.bytesWritten);// httpLayer.getAnsBytes().bytesWritten);
#endif
        break;
    case PROT_PARSER_ERR_INIT:
        printf("[+] - ERR - connection to %s finished with PROT_PARSER_ERR_INIT\n", "localhost");
        break;
    case PROT_PARSER_ERR_READ:
        printf("[+] - ERR - connection to %s finished with PROT_PARSER_ERR_READ\n", "localhost");
        break;
    case PROT_PARSER_ERR_PARSE:
        printf("[+] - ERR - connection to %s finished with PROT_PARSER_ERR_PARSE\n", "localhost");
        break;
    default:
        break;
    }
    fflush(stdout);
    return 0;
}
