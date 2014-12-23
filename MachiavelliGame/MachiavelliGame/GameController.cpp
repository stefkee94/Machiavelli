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
	building_cards = reader.read_building_cards("Bouwkaarten.csv");
	character_cards = reader.read_character_cards("karakters.csv");
}

