#pragma once
#include <string>
#include <memory>
#include "Socket.h"

class ClientCommand
{
public:
	ClientCommand();
	ClientCommand(const std::string& command, std::shared_ptr<Socket> client);
	virtual ~ClientCommand();

	std::string get_command() const;
	void set_command(const std::string& command);

	std::shared_ptr<Socket> get_client() const;
	void set_client(std::shared_ptr<Socket> socket);

private:
	std::string command;
	std::shared_ptr<Socket> client;
};

