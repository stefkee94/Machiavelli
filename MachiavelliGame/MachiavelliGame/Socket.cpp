#include "Socket.h"
#include <memory>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

// Need WSA class for windows
class WSA
{
private:
	WSADATA data;

public:
	WSA()
	{
		int result = WSAStartup(MAKEWORD(2, 2), &data);
		if (result != 0)
			std::cerr << "WSAStartup failed with error : " << result << "\n";
	}
	~WSA()
	{
		WSACleanup();
	}
} wsa;

Socket::Socket() : socket(0)
{
}

Socket::Socket(SOCKET socket, const struct sockaddr& address) : socket(socket)
{
	switch (address.sa_family)
	{
		case AF_INET:
			set_inet4(address);
			break;
		case AF_INET6:
			set_inet6(address);
			break;
		default:
			throw std::runtime_error("Invalid type of socket address");
	}
}

void Socket::set_inet4(const struct sockaddr& address)
{
	struct sockaddr_in* address_in = reinterpret_cast<struct sockaddr_in*>(&address_storage);
	const struct sockaddr_in* ipv4 = reinterpret_cast<const struct sockaddr_in*>(&address_storage);
	*address_in = *ipv4;
}

void Socket::set_inet6(const struct sockaddr& address)
{
	struct sockaddr_in6* address_in6 = reinterpret_cast<struct sockaddr_in6*>(&address_storage);
	const struct sockaddr_in6* ipv6 = reinterpret_cast<struct sockaddr_in6*>(&address_storage);
	*address_in6 = *ipv6;
}

std::string Socket::get_ip_inet4(const char* result, char text_buffer[], const struct sockaddr* address_pointer)
{
	const struct sockaddr_in* pointer_in = reinterpret_cast<const struct sockaddr_in*>(address_pointer);
	throw_if_null(result = ::inet_ntop(AF_INET, (void *)&pointer_in->sin_addr, text_buffer, INET6_ADDRSTRLEN));
	return result;
}

std::string Socket::get_ip_inet6(const char* result, char text_buffer[], const struct sockaddr* address_pointer)
{
	const struct sockaddr_in6* pointer6_in = reinterpret_cast<const struct sockaddr_in6*>(address_pointer);
	throw_if_null(result = ::inet_ntop(AF_INET6, (void *)&pointer6_in->sin6_addr, text_buffer, INET6_ADDRSTRLEN));
	return result;
}

SOCKET Socket::get() const
{
	return socket;
}

void Socket::set(SOCKET socket)
{
	this->socket = socket;
}

void Socket::close()
{
	std::cerr << "will close socket " << socket << "\n";
	::closesocket(socket);
	socket = 0;
}

std::string Socket::get_dotted_ip()
{
	const char* result = nullptr;
	char text_buffer[INET6_ADDRSTRLEN];
	const struct sockaddr* address_pointer = reinterpret_cast<const struct sockaddr*>(&address_storage);

	switch (address_pointer->sa_family)
	{
		case AF_INET:
			return get_ip_inet4(result, text_buffer, address_pointer);
			break;
		case AF_INET6:
			return get_ip_inet6(result, text_buffer, address_pointer);
			break;
		default:
			return "n/a";
	}
}

ssize_t Socket::read(char* buf, size_t max_length)
{
	ssize_t length = 0;
	while (ssize_t n = ::recv(socket, buf + length, int(max_length - length), 0))
	{
		throw_if_min1((int)n);
		length += n;

		if (length >= max_length) 
			break;
	}

	return length;
}

std::string Socket::read_line()
{
	std::string line;
	char c;

	while (ssize_t n = ::recv(socket, &c, 1, 0))
	{
		throw_if_min1((int)n);
		if (c == '\n') break;
		if (c != '\r')
			line += c;
	}

	return line;
}

void Socket::write(const std::string& message)
{
	write(message.c_str(), message.length());
}

void Socket::write(const char* buf, size_t length)
{
	throw_if_min1((int)::send(socket, buf, (int)length, 0));
}

Socket::~Socket()
{
	if (socket > 0)
		close();
}
