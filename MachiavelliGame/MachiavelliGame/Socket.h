#pragma once

#include <WinSock2.h>
typedef long ssize_t;
#include "ThrowMacro.h"
#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <cstring>

#include <WS2tcpip.h>

class Socket
{
public:
	Socket();
	Socket(SOCKET socket, const struct sockaddr& address);
	virtual ~Socket();

	SOCKET get() const;
	void set(SOCKET sock);
	void close();
	std::string get_dotted_ip();

	ssize_t read(char* buf, size_t max_length);
	std::string read_line();

	void write(const std::string& message);
	void write(const char* buf, size_t length);

protected:
	SOCKET socket;
	struct sockaddr_storage address_storage;

private:
	void set_inet4(const struct sockaddr& address);
	void set_inet6(const struct sockaddr& address);

	std::string get_ip_inet4(const char* result, char text_buffer[], const struct sockaddr* address_pointer);
	std::string get_ip_inet6(const char* result, char text_buffer[], const struct sockaddr* address_pointer);
};



