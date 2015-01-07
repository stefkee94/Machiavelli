#pragma once
#include <vector>
#include <memory>
#include "CardStack.h"
#include "BuildingCard.h"
#include "CharacterCard.h"

class Player
{
	public:
		Player(std::string name, int age);
		virtual ~Player();

		void add_card_to_hand(std::shared_ptr<BuildingCard> hand_cards);
		void put_card_on_field(std::shared_ptr<BuildingCard> field_cards);
		void add_character(std::shared_ptr<CharacterCard> character_cards);
		void use_character_property();

		void add_gold(int amount);
		void remove_gold(int amount);

	private:
		std::string name;
		int age;
		int gold;

		bool is_king;
		bool char_property_used;
		
		CardStack<std::shared_ptr<BuildingCard>> hand_cards;
		CardStack<std::shared_ptr<BuildingCard>> field_cards;
		CardStack<std::shared_ptr<CharacterCard>> character_cards;
};
