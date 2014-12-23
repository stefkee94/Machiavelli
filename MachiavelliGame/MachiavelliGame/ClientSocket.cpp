#include "ClientSocket.h"

ClientSocket::ClientSocket(const char *host, int port)
{
	// construct network address for server
	struct addrinfo hint;
	std::memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;
	struct addrinfo* infolist{ nullptr };
	int gai_error = ::getaddrinfo(host, std::to_string(port).c_str(), &hint, &infolist);

	if (gai_error) {
		std::ostringstream oss;
		oss << "getaddrinfo error " << gai_error << ": " << gai_strerror(gai_error) << " (" << __FILE__ << ":" << __LINE__ << ")";
		throw std::runtime_error(oss.str());
	}

	using cleanup_func = void(__stdcall*)(PADDRINFOA);
	std::unique_ptr<struct addrinfo, cleanup_func> list(infolist, ::freeaddrinfo);

	// create socket
	throw_if_min1(socket = ::socket(list->ai_family, list->ai_socktype, list->ai_protocol));

	// connect to server
	throw_if_min1(::connect(socket, (struct sockaddr*)list->ai_addr, list->ai_addrlen));
}