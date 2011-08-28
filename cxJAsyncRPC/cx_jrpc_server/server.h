#ifndef SERVER_H
#define SERVER_H

#include <cx_net_sockets/socket_tcp.h>

void initUsers();
bool serverThread(void *, Socket_Base_Stream * v, const char *remotePair);

#endif // SERVER_H
