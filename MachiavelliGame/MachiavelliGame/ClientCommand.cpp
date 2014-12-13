#include "ClientCommand.h"

ClientCommand::ClientCommand()
{

}

ClientCommand::ClientCommand(const std::string& command, std::shared_ptr<Socket> client) : command(command), client(client)
{
}

std::string ClientCommand::get_command() const
{
	return command;
}

void ClientCommand::set_command(const std::string& command)
{
	this->command = command;
}

std::shared_ptr<Socket> ClientCommand::get_client() const
{
	return client;
}

void ClientCommand::set_client(std::shared_ptr<Socket> socket)
{
	client = socket;
}

ClientCommand::~ClientCommand()
{
}
