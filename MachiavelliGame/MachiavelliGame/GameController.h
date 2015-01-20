#pragma once
#include <memory>
#include "Socket.h"
#include "ClientCommand.h"
#include "CardStack.h"
#include "MachiavelliReader.h"
#include "Player.h"
#include "GameFase.h"

class ServerController;
class GameController
{
	public:
		GameController();
		virtual ~GameController();

		void handle_client_command(std::shared_ptr<Socket> client, std::string new_command);
		void consume_command(ClientCommand command, std::shared_ptr<Socket> client);

		void connect_player(std::shared_ptr<Socket> client, std::string name, std::string age);
		void start_game();
		void choose_character();
		void dismiss_character();
		
		void call_next_char();
		void print_turn_info();
		void show_help_text(std::shared_ptr<Socket> client);

		void hanlde_choose_char_command(std::string new_command);
		void handle_dismiss_char_command(std::string new_command);
		void handle_play_turn_command(std::string new_command);
		void handle_choose_building_card(std::string new_command);

	private:
		CardStack<std::shared_ptr<BuildingCard>> building_cards;
		CardStack<std::shared_ptr<CharacterCard>> character_cards;
		std::vector<std::shared_ptr<Player>> players;

		void init();
		void set_turn_to_next_player();
		void take_gold(int amount);
		void take_building_cards();

		std::vector<std::string> get_turn_choices();
		std::vector<std::string> turn_choices;

		void set_turn_choices();
		void remove_choices();

		std::vector<std::string> char_order;
		std::vector<std::string> init_choices;
		std::shared_ptr<Player> player_on_turn;

		std::vector<std::shared_ptr<BuildingCard>> picked_building_cards;

		GameFase fase;
		bool first_pick;
		int call_count = 0;
};

