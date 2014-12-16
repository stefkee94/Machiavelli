#pragma once
#include <memory>
#include "Socket.h"
#include "ClientCommand.h"

class ServerController;
class GameController
{
public:
	GameController();
	virtual ~GameController();

	void handle_client_command(std::string new_command);
};

