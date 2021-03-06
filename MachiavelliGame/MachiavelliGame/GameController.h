#pragma once
#include <memory>
#include <map>
#include "Socket.h"
#include "ClientCommand.h"
#include "CardStack.h"
#include "MachiavelliReader.h"
#include "Player.h"
#include "GamePhase.h"

class ServerController;
class GameController
{
	public:
		GameController();
		virtual ~GameController();
		void handle_client_command(std::shared_ptr<Socket> client, std::string new_command);
		void consume_command(ClientCommand command, std::shared_ptr<Socket> client);
		void connect_player(std::shared_ptr<Socket> client, std::string name, std::string age);
		bool get_is_ended();
	private:
		CardStack<std::shared_ptr<BuildingCard>> building_cards;
		CardStack<std::shared_ptr<CharacterCard>> character_cards;
		std::vector<std::shared_ptr<Player>> players;
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
		void handle_murder_character(std::string new_command);
		void handle_steal_from_character(std::string new_command);
		void handle_build_card(std::string new_command);
		void handle_end_turn();
		void set_new_king();
		void reset_characters();
		void handle_char_property();
		void handle_magicien_property(std::string new_command);
		void handle_magicien_trade_bank_prop(std::string new_command);
		void handle_condottiere_phase(std::string new_command);
		void handle_buy_destroyed_building(std::string new_command);
		void handle_school_of_magic_choice(std::string new_command);
		void handle_hof_of_miracles_choice(std::string new_command);
		void handle_laboratory();
		void handle_workplace();
		void check_for_hof();
		void decide_winner();
		void check_for_graveyard(std::string card_name);
		void handle_labroratory_choice(std::string new_command);
		void end_game();
		void init();
		void set_turn_to_next_player();
		void take_gold(int amount);
		void take_building_cards();
		void build_building_card();
		std::vector<std::pair<int, std::string>> get_turn_choices();
		void init_thief_choices(std::string name_of_murdered);
		void remove_choice(int index);
		void set_turn_choices();
		void do_magicien_property();
		void magicien_trade_cards_with_player();
		void magicien_trade_cards_with_bank();

		std::vector<std::pair<int, std::string>> turn_choices;
		std::vector<std::string> char_order;
		std::vector<std::string> init_choices;
		std::shared_ptr<Player> player_on_turn;
		std::shared_ptr<BuildingCard> card_to_destroy;
		std::map<int, std::string> murderer_choices;
		std::map<int, std::string> thief_choices;
		std::map<int, std::shared_ptr<BuildingCard>> condottiere_choices;
		std::map < int, std::shared_ptr<BuildingCard>> lab_choices;
		std::map<int, std::shared_ptr<BuildingCard>> hof_choices;
		std::vector<std::shared_ptr<BuildingCard>> picked_building_cards;
		GamePhase phase;
		bool first_pick;

		bool game_is_finished;
		bool is_ended;

		int count_builded_in_turn_for_architect = 0;
		int call_count = 0;
};

