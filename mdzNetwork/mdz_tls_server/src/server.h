#ifndef SERVER_H
#define SERVER_H

#include <mdz_net_sockets/socket_tls.h>
bool serverThread(void *, Mantids::Network::Sockets::Socket_StreamBase * v, const char *remotePair, bool secure);
bool serverFailed(void *, Mantids::Network::Sockets::Socket_StreamBase * v, const char *remotePair, bool secure);

#endif // SERVER_H
