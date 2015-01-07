#include <iostream>
#include <fstream>
#include "MachiavelliReader.h"

MachiavelliReader::MachiavelliReader()
{
}

MachiavelliReader::~MachiavelliReader()
{
}

CardStack<std::shared_ptr<CharacterCard>> MachiavelliReader::read_character_cards(std::string path)
{
	CardStack<std::shared_ptr<CharacterCard>> character_cards;
	std::vector<std::string> character_card_values;
	std::ifstream file(path);
	std::string line;
	char delimiter = ';';
	while (file.good())
	{
		std::getline(file, line, '\n');
		size_t pos = 0;
		if (!line.empty())
		{
			while ((pos = line.find(delimiter)) != std::string::npos)
			{
				std::string value = line.substr(0, pos);
				character_card_values.push_back(value);
				line.erase(0, pos + 1);
			}
			character_cards.add_card(create_character_card(character_card_values));
			character_card_values.clear();
		}
	}
	return character_cards;
}

CardStack<std::shared_ptr<BuildingCard>> MachiavelliReader::read_building_cards(std::string path)
{
	CardStack<std::shared_ptr<BuildingCard>> building_cards;
	std::vector<std::string> building_card_values;
	std::ifstream file(path);
	std::string line;
	char delimiter = ';';
	while (file.good())
	{
		std::getline(file, line, '\n');
		size_t pos = 0;
		if (!line.empty())
		{
			while ((pos = line.find(delimiter)) != std::string::npos)
			{
				std::string value = line.substr(0, pos);
				building_card_values.push_back(value);
				line.erase(0, pos + 1);
			}

			building_cards.add_card(create_building_card(building_card_values));
			building_card_values.clear();
		}
	}
	return building_cards;
}

std::unique_ptr<BuildingCard> MachiavelliReader::create_building_card(std::vector<std::string>& building_card_values)
{
	if (building_card_values.size() == 3)
	{
		std::string name = building_card_values[0];
		int points = atoi(building_card_values[1].c_str());
		CardColor color = string_to_card_olor(building_card_values[2]);

		return std::make_unique<BuildingCard>(name, points, color);
	}
	else
	{
		//error
	}
}

std::unique_ptr<CharacterCard> MachiavelliReader::create_character_card(std::vector<std::string>& character_card_values)
{
	if (character_card_values.size() == 2){
		int id = atoi(character_card_values[0].c_str());
		std::string name = character_card_values[1];
		CharacterType type = string_to_character_type(name);

		return std::make_unique<CharacterCard>(id, name, type);
	}
	else
	{
		//error
	}
}

CardColor MachiavelliReader::string_to_card_olor(std::string card_color)
{
	if (card_color.compare("geel") == 0)
		return CardColor::Yellow;
	else if (card_color.compare("groen") == 0) 
		return CardColor::Green;
	else if (card_color.compare("blauw") == 0)
		return CardColor::Blue;
	else if (card_color.compare("rood") == 0)
		return CardColor::Red;
	else if (card_color.compare("lila") == 0)
		return CardColor::Lilac;
}

CharacterType MachiavelliReader::string_to_character_type(std::string character)
{
	if (character.compare("Moordenaar") == 0)
		return CharacterType::Murderer;
	else if (character.compare("Dief") == 0)
		return CharacterType::Thief;
	else if (character.compare("Magiër") == 0)
		return CharacterType::Magicien;
	else if (character.compare("Koning") == 0)
		return CharacterType::King;
	else if (character.compare("Prediker") == 0)
		return CharacterType::Preacher;
	else if (character.compare("Koopman") == 0)
		return CharacterType::Merchant;
	else if (character.compare("Bouwmeester") == 0)
		return CharacterType::Architect;
	else if (character.compare("Condottiere") == 0)
		return CharacterType::Condottiere;
}
