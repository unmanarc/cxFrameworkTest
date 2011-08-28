#include "server.h"

#include <cx_jasyncrpc_tserver/jasyncrpc_server.h>
#include <cx_auth_sqlite3/iauth_sqlite3.h>

static IAuth_SQLite3 sqliteAuth("vjasyncrpc_server");

void initUsers()
{
    sqliteAuth.accountChangePassword("admin", IAuth::genPassword("123456XYZ",PASS_MODE_SSHA256) );

    sqliteAuth.groupAdd("fullattrs", "all attribs");

    sqliteAuth.attribAdd("attr1", "test attr 1");
    sqliteAuth.attribAdd("attr2", "test attr 2");
    sqliteAuth.attribAdd("attr3", "test attr 3");

    sqliteAuth.groupAccountAdd("fullattrs","admin");

    sqliteAuth.attribGroupAdd("attr1","fullattrs");
    sqliteAuth.attribGroupAdd("attr2","fullattrs");
    sqliteAuth.attribGroupAdd("attr3","fullattrs");
}


bool serverThread(void * vMethods, Socket_Base_Stream * v, const char * remotePair)
{
    std::cout << static_cast<void *>(v) << "> New connection from (" << remotePair << ":" <<  v->getRemotePort() << ")" << std::endl << std::flush;

    JAsyncRPC_Server rpcServer(v);

    // TODO: how to pass connection details?
    rpcServer.setAuthenticator(&sqliteAuth);
    rpcServer.setMethodsManager(static_cast<JAsyncRPC_Server_Methods *>(vMethods));
    rpcServer.getLocalHandshake()->setProductInfo("My RPC Test Server");
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
