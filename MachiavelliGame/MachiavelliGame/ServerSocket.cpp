#include "ServerSocket.h"

ServerSocket::ServerSocket(int port_number)
{
	start(port_number);
}

void ServerSocket::start(int port_number)
{
	throw_if_min1(socket = ::socket(AF_INET, SOCK_STREAM, 0));

	struct sockaddr_in server_socket;
	server_socket.sin_family = AF_INET;
	server_socket.sin_addr.s_addr = INADDR_ANY;
	server_socket.sin_port = htons(port_number);

	throw_if_min1(::bind(socket, (struct sockaddr*)&server_socket, sizeof(struct sockaddr)));
	throw_if_min1(::listen(socket, 100));
}

std::shared_ptr<Socket> ServerSocket::accept()
{
	struct sockaddr address_client;
	address_client.sa_family = AF_INET;
	socklen_t length = sizeof(address_client);

	int fd;
	throw_if_min1(fd = ::accept(socket, &address_client, &length));

	std::shared_ptr<Socket> socket_client = std::make_shared<Socket>(fd, address_client);
	std::cerr << "Connection accepted from " << socket_client->get_dotted_ip() << ", with socket " << fd << "\n";

	return socket_client;
}
