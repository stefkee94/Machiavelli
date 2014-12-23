#pragma once
#include "Socket.h"

class ServerSocket : public Socket
{
	public:
		ServerSocket(int port_number);

		void start(int port_number);
		std::shared_ptr<Socket> accept();

	private:
		int port_number;
};

