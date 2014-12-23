#pragma once
#include <string>
#include <vector>
#include <memory>
#include "CharacterCard.h"
#include "BuildingCard.h"
#include "CardStack.h"

class MachiavelliReader
{
	public:
		MachiavelliReader();
		virtual ~MachiavelliReader();

		CardStack<std::shared_ptr<CharacterCard>> read_character_cards(std::string path);
		CardStack<std::shared_ptr<BuildingCard>> read_building_cards(std::string path);

	private:

		std::unique_ptr<BuildingCard> create_building_card(std::vector<std::string>& building_card_values);
		std::unique_ptr<CharacterCard> create_character_card(std::vector<std::string>& character_card_values);

		CardColor string_to_card_olor(std::string card_color);
		CharacterType string_to_character_type(std::string character);
};

