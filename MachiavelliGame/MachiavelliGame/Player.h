#pragma once
#include <vector>
#include <memory>
#include "Socket.h"
#include "CardStack.h"
#include "BuildingCard.h"
#include "CharacterCard.h"

class Player
{
	public:
		Player(std::shared_ptr<Socket> client, std::string name, int age);
		virtual ~Player();

		void add_card_to_hand(std::shared_ptr<BuildingCard> hand_cards);
		void remove_card_from_hand(int index);

		void put_card_on_field(std::shared_ptr<BuildingCard> field_cards);
		void add_character(std::shared_ptr<CharacterCard> character_cards);
		
		CardStack<std::shared_ptr<BuildingCard>> get_field_cards();
		CardStack<std::shared_ptr<BuildingCard>> get_hand_cards();

		void use_character_property();

		void add_gold(int amount);
		void remove_gold(int amount);
		void set_type(CharacterType type);
		void set_is_king(bool is_king);
		bool get_is_king();
		void reset_character_cards();
		bool get_is_robbed();
		void set_is_robbed(bool b);

		std::shared_ptr<CharacterCard> has_character(std::string character_name);
		void remove_character_card(std::string character_name);
		void remove_field_card(std::string name);
		bool has_field_card();

		CharacterType get_char_type();
		int get_gold();
		int get_age();
		std::shared_ptr<Socket> get_client();

		std::vector<std::string> get_turn_choices();
		void set_turn_choices();
		void remove_choice(int index);

	private:
		std::shared_ptr<Socket> client;
		std::string name;
		int age;
		int gold;

		bool is_king;
		bool char_property_used;
		bool is_robbed;

		CharacterType character_type;

		CardStack<std::shared_ptr<BuildingCard>> hand_cards;
		CardStack<std::shared_ptr<BuildingCard>> field_cards;
		CardStack<std::shared_ptr<CharacterCard>> character_cards;
};
