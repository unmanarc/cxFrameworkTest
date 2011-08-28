#include <cx_net_sockets/socket_tcp.h>
#include <cx_net_threadedacceptor/threadedstreamacceptor.h>

#include "server.h"

#include <cx_jasyncrpc_tserver/jasyncrpc_server.h>


Json::Value test_sync(void * obj, IAuth_Session * session, const Json::Value & payload,const Json::Value &extraInfo, Json::Value *extraInfoOut)
{
    Json::Value payloadOut;
    //std::cout << "SYNC request: " << JSON_StreamableObject::jsonToString(payload) << ", extraInfo:" << JSON_StreamableObject::jsonToString(extraInfo) << std::endl << std::flush;
    payloadOut["msg"] = time(nullptr);
    return payloadOut;
}

Json::Value test_async(void * obj, IAuth_Session * session, const Json::Value & payload,const Json::Value &extraInfo, Json::Value *extraInfoOut)
{
    Json::Value payloadOut;
    //std::cout << "ASYNC request: " << JSON_StreamableObject::jsonToString(payload) << ", extraInfo:" << JSON_StreamableObject::jsonToString(extraInfo) << std::endl << std::flush;

    payloadOut["msg"] = time(nullptr);
    long long microseconds = 94875;
    for (int i=1;i<10000000;i++) microseconds=microseconds*i; // Math op...
    payloadOut["result"] = Json::Int64(microseconds);

    return payloadOut;
}


int main(int argc, char *argv[])
{
    // TCP SERVER:
    printf("Starting TCP server...\n"); fflush(stdout);
    Socket_TCP tcpServer;
    if (!tcpServer.listenOn(40404,"127.0.0.1",true))
    {
        printf("Error creating a listening server :(...\n");
        return -1;
    }
    printf("TCP server running @%d...\n", tcpServer.getPort()); fflush(stdout);

    initUsers();

    // STREAM MANAGER:
    ThreadedStreamAcceptor vstreamer;
    JAsyncRPC_Server_Methods methods;
    methods.addRPCMethod("synctest", {"attr1","attr2"}, &test_sync, nullptr);
    methods.addRPCMethod("asynctest", {"attr1","attr2"}, &test_async, nullptr);

    vstreamer.setAcceptorSocket(&tcpServer);
    vstreamer.setCallbackOnConnect(&serverThread, &methods);
    vstreamer.startThreaded();
    printf("Accepting connections for 100 minutes...\n"); fflush(stdout);
    sleep(100*60);

   // methods.stop();
//    vstreamer.stop();

    return 0;
}
