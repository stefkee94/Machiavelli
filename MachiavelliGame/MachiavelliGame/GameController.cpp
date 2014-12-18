#include "GameController.h"
#include "ServerController.h"

GameController::GameController()
{
	init();
}

void GameController::handle_client_command(std::string new_command)
{
	
}

GameController::~GameController()
{
}

void GameController::init()
{
	MachiavelliReader reader;
	buildingCards = reader.readBuildingCards("Bouwkaarten.csv");
	characterCards = reader.readCharacterCards("karakters.csv");
}

