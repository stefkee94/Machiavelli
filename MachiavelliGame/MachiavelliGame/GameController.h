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

	void start_client_game(std::shared_ptr<ServerController> server_controller, std::shared_ptr<Socket> new_client);
};

