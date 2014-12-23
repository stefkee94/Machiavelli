#pragma once
#include "Socket.h"

class ClientSocket : public Socket
{
	public:
		ClientSocket(const char* host, int port_number);

		void start();

	private:
		const char* host;
		int port_number;
};

