#pragma once
//=============================================================================
// socket.h 
// (c) 8-12-2010, Frans Spijkerman, Avans Hogeschool
// enhanced for cross-platform use by Bob Polis (c) 2013, 2014
//
// Definition of classes Socket, ServerSocket and CientSocket
//
// Socket:
//	Abstraction of tcp-socket using Windows Sockets API (WSA)
//
// ServerSocket:
//  Socket to listen to clients on a tcp-port
//
// ClientSocket:
//	Socket to connect to a server on a tcp-port
//=============================================================================

#if defined(__APPLE__) || defined(__linux__)
#include <sys/socket.h>
typedef int SOCKET;
#else // Windows
#include <winsock2.h>
typedef long ssize_t;
#endif

#include <iostream>
#include <string>

//=============================================================================
class Socket
	//=============================================================================
{
protected:
	SOCKET sock;
	struct sockaddr_storage addr;

public:
	Socket() : sock(0) {}
	Socket(SOCKET sock, const struct sockaddr& address);
	virtual ~Socket();

	SOCKET get() const { return sock; }
	void set(SOCKET sock) { this->sock = sock; }
	void close();
	std::string get_dotted_ip() const;

	ssize_t read(char *buf, size_t maxlen);
	std::string readline();

	void write(const std::string& msg);
	void write(const char *buf, size_t len);
};

//=============================================================================
class ServerSocket : public Socket
	//=============================================================================
{
public:
	ServerSocket(int port);
	Socket* accept();
};

//=============================================================================
class ClientSocket : public Socket
	//=============================================================================
{
public:
	ClientSocket(const char *host, int port);
};

