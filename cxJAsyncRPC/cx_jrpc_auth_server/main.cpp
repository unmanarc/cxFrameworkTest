#include <cx_net_sockets/socket_tcp.h>
#include <cx_auth_sqlite3/iauth_sqlite3.h>
#include <cx_jasyncrpc_tserver/jasyncrpc_server.h>
#include <cx_jasyncrpc_functions_auth/jsyncrpc_functions_auth.h>
#include <cx_net_threadedacceptor/threadedstreamacceptor.h>

static IAuth_SQLite3 sqliteAuth("vjasyncrpc_auth_server");

bool serverThread(void * vMethods, Socket_Base_Stream * v, const char * remotePair)
{
    std::cout << static_cast<void *>(v) << "> New connection from (" << remotePair << ":" <<  v->getRemotePort() << ")" << std::endl << std::flush;

    JAsyncRPC_Server rpcServer(v);

    rpcServer.setAuthenticator(&sqliteAuth);
    rpcServer.setMethodsManager(static_cast<JAsyncRPC_Server_Methods *>(vMethods));
    rpcServer.getLocalHandshake()->setProductInfo("Auth Testing Server");
    rpcServer.getLocalHandshake()->setProductVersion("1.0");

    ParseErrorMSG err;
    rpcServer.parseObject(&err);

    if (!rpcServer.isValidHandShake())
    {
        std::cerr <<  "> Invalid handshake :(" << std::endl << std::flush;
    }

    std::cout << static_cast<void *>(v) << "> Connection Finished." << std::endl << std::flush;
    return  true;
}

void initUsers()
{
    // Your user admin will be always with this password :)
    sqliteAuth.accountChangePassword("admin", IAuth::genPassword("123456XYZ",PASS_MODE_SSHA256) );
}

int main(int argc, char *argv[])
{
    // TCP SERVER:
    printf("Starting server...\n"); fflush(stdout);
    Socket_TCP tcpServer;
    if (!tcpServer.listenOn(48765,"127.0.0.1",true))
    {
        printf("Error creating a listening server :(...\n");
        return -1;
    }
    printf("Authentication server running @tcp/%d...\n", tcpServer.getPort()); fflush(stdout);

    initUsers();

    // STREAM MANAGER:
    ThreadedStreamAcceptor vstreamer;
    JAsyncRPC_Server_Methods methods;

    jASyncRPC_AddAuthMethods(&methods);

    vstreamer.setAcceptorSocket(&tcpServer);
    vstreamer.setCallbackOnConnect(&serverThread, &methods);
    vstreamer.startThreaded();

    for (;;) { sleep(1000); }
    return 0;
}
