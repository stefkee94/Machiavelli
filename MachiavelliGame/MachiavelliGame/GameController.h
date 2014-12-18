#pragma once
#include <memory>
#include "Socket.h"
#include "ClientCommand.h"
#include "CardStack.h"
#include "MachiavelliReader.h"

class ServerController;
class GameController
{
public:
	GameController();
	virtual ~GameController();
	void handle_client_command(std::string new_command);
	void run();
private:
	CardStack<std::shared_ptr<BuildingCard>> buildingCards;
	CardStack<std::shared_ptr<CharacterCard>> characterCards;
	void init();
};

