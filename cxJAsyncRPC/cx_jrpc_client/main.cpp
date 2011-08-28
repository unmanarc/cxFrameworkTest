#include <cx_net_sockets/socket_tcp.h>
#include <cx_jasyncrpc_tclient/jasyncrpc_client.h>
#include <cx_jasyncrpc_common/json_streamableobject.h>

#include <chrono>

#define SYNCREQS 10
#define ASYNCREQS 1000000

static double elapsedTimeASYNCDEC = 0;

void answerHandler(void *, int retCode, const Json::Value & payload, const Json::Value &extraInfo)
{
//    std::cout << "ret code [" << retCode << "], answer: " << JSON_StreamableObject::jsonToString(payload) << ", extraInfo:" << JSON_StreamableObject::jsonToString(extraInfo) << std::endl << std::flush;
}

void rpcClient(Socket_TCP * tcp)
{
    JAsyncRPC_Client rpcClient(tcp);
    //rpcClient.setDestroyOnExit(false);
    rpcClient.getLocalHandshake()->setProductInfo("My RPC Test Server");
    rpcClient.getLocalHandshake()->setProductVersion("1.0");
    std::cout << "Starting rpcCLient " << &rpcClient << std::endl << std::flush;
    rpcClient.start();

    AuthReason authReason = rpcClient.authenticate("admin", "123456XYZ");

    if (authReason == AUTH_REASON_AUTHENTICATED)
    {
        Json::Value payload;

        payload["test"] = "Hello world";

        std::cout << "JRPC authenticated as \"admin\"..." << std::endl << std::flush;

        std::cout << "Trying SYNCED..." << std::endl << std::flush;
        auto t_start = std::chrono::high_resolution_clock::now();
        for (size_t i=0;i<SYNCREQS;i++)
        {
            sRetInfo retInfo;
            rpcClient.execSync("synctest",payload,{},&retInfo);
            //std::cout << "Execution result: " << retInfo.payload.toStyledString() << std::endl << std::flush;
        }
        // the work...
        auto t_end = std::chrono::high_resolution_clock::now();
        double elapsedTimeMs = std::chrono::duration<double, std::milli>(t_end-t_start).count();
        elapsedTimeASYNCDEC+=elapsedTimeMs;
        std::cout << "Execution took " << elapsedTimeMs << "ms for " << SYNCREQS << " req..." << std::endl << std::flush;

        std::cout << "Sending ASYNCED..." << std::endl << std::flush;
        t_start = std::chrono::high_resolution_clock::now();
        for (size_t i=0;i<ASYNCREQS;i++)
        {
            //sRetInfo retInfo;
            rpcClient.execASync("asynctest",payload,{},"",10000,&answerHandler, nullptr);
        }
        t_end = std::chrono::high_resolution_clock::now();
        elapsedTimeMs = std::chrono::duration<double, std::milli>(t_end-t_start).count();
        elapsedTimeASYNCDEC+=elapsedTimeMs;
        std::cout << "Async req's sent... and took " << elapsedTimeMs/(double)ASYNCREQS << "ms per request for " << ASYNCREQS << " req..." << std::endl << std::flush;
    }
    else
    {
        std::cout << "Invalid Auth: " << getAuthReasonText(authReason) << std::endl << std::flush;
    }
}


int main(int , char ** )
{
    std::cout << "Starting TCP client for JRPC..." << std::endl << std::flush;
    Socket_TCP tcpClient;
    if (!tcpClient.connectTo("127.0.0.1", 40404, 15))
    {
        std::cerr << "Error connecting..." << std::endl << std::flush;
        return -1;
    }
    std::cout << "TCP Client Connected..." << std::endl << std::flush;
    auto t_start = std::chrono::high_resolution_clock::now();
    rpcClient(&tcpClient);
    auto t_end = std::chrono::high_resolution_clock::now();
    double elapsedTimeMs = std::chrono::duration<double, std::milli>(t_end-t_start).count();
    std::cout << "Async Execution took " << (elapsedTimeMs-elapsedTimeASYNCDEC)/(double)ASYNCREQS << "ms per request for " << ASYNCREQS << " req..." << std::endl << std::flush;
    return 0;
}
