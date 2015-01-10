#pragma once
#include <memory>
#include "Socket.h"
#include "ClientCommand.h"
#include "CardStack.h"
#include "MachiavelliReader.h"
#include "Player.h"

class ServerController;
class GameController
{
	public:
		GameController();
		virtual ~GameController();

		void handle_client_command(std::shared_ptr<Socket> client, std::string new_command);
		void connect_player(int client_id, std::string name, std::string age);
		void start_game();

	private:
		CardStack<std::shared_ptr<BuildingCard>> building_cards;
		CardStack<std::shared_ptr<CharacterCard>> character_cards;
		std::vector<std::shared_ptr<Player>> players;

		void init();

		int player_turn;
};

