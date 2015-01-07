#include "GameController.h"
#include "ServerController.h"

GameController::GameController()
{
	init();
}

void GameController::handle_client_command(std::string new_command)
{
	
}

void GameController::connect_player(std::string name, std::string age)
{
	players.push_back(std::make_shared<Player>(name, std::atoi(age.c_str())));
	//if (players.size() >= 2) -- needed to start the game
		start_game();
}

void GameController::start_game()
{
	for (std::shared_ptr<Player> player : players)
	{
		player->add_gold(4);
		for (int i = 0; i < 4; i++)
		{
			player->add_card_to_hand(building_cards.get_card_at_top());
		}
	}
}

GameController::~GameController()
{
}

void GameController::init()
{
	MachiavelliReader reader;
	building_cards = reader.read_building_cards("Bouwkaarten.csv");
	character_cards = reader.read_character_cards("karakters.csv");

	building_cards.shuffle_cards();
	character_cards.shuffle_cards();
}

