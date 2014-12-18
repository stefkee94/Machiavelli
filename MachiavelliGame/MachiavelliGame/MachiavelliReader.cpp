#include <iostream>
#include <fstream>
#include "MachiavelliReader.h"

MachiavelliReader::MachiavelliReader()
{
}

MachiavelliReader::~MachiavelliReader()
{
}

CardStack<std::shared_ptr<CharacterCard>> MachiavelliReader::readCharacterCards(std::string path)
{
	CardStack<std::shared_ptr<CharacterCard>> characterCards;
	std::vector<std::string> characterCardValues;
	std::ifstream file(path);
	std::string line;
	char delimiter = ';';
	if (!line.empty()){
		while (file.good()){
			std::getline(file, line, '\n');
			size_t pos = 0;
			while ((pos = line.find(delimiter)) != std::string::npos){
				std::string value = line.substr(0, pos);
				characterCardValues.push_back(value);
				line.erase(0, pos + 1);
			}
			characterCards.addCard(createCharacterCard(characterCardValues));
			characterCardValues.clear();
		}
	}
	return characterCards;
}

CardStack<std::shared_ptr<BuildingCard>> MachiavelliReader::readBuildingCards(std::string path)
{
	CardStack<std::shared_ptr<BuildingCard>> buildingCards;
	std::vector<std::string> buildingCardValues;
	std::ifstream file(path);
	std::string line;
	char delimiter = ';';
	while (file.good()){
		std::getline(file, line, '\n');
		size_t pos = 0;
		if (!line.empty()){
			while ((pos = line.find(delimiter)) != std::string::npos){
				std::string value = line.substr(0, pos);
				buildingCardValues.push_back(value);
				line.erase(0, pos + 1);
			}
			buildingCards.addCard(createBuildingCard(buildingCardValues));
			buildingCardValues.clear();
		}
	}
	return buildingCards;
}

std::unique_ptr<BuildingCard> MachiavelliReader::createBuildingCard(std::vector<std::string>& buildingCardValues)
{
	if (buildingCardValues.size() == 3){
		std::string name = buildingCardValues[0];
		int points = atoi(buildingCardValues[1].c_str());
		CardColor color = stringToCardColor(buildingCardValues[2]);
		return std::make_unique<BuildingCard>(name, points, color);
	}
	else{
		//error
	}
}

std::unique_ptr<CharacterCard> MachiavelliReader::createCharacterCard(std::vector<std::string>& characterCardValues)
{
	if (characterCardValues.size() == 2){
		int id = atoi(characterCardValues[0].c_str());
		std::string name = characterCardValues[1];
		CharacterType type = stringToCharacterType(name);
		return std::make_unique<CharacterCard>(id, name, type);
	}
	else{
		//error
	}
}

CardColor MachiavelliReader::stringToCardColor(std::string cardColor)
{
	if (cardColor.compare("geel") == 0){
		return CardColor::Yellow;
	}
	else if (cardColor.compare("groen") == 0){
		return CardColor::Green;
	}
	else if (cardColor.compare("blauw") == 0){
		return CardColor::Blue;
	}
	else if (cardColor.compare("rood") == 0){
		return CardColor::Red;
	}
	else if (cardColor.compare("lila") == 0){
		return CardColor::Lilac;
	}
}

CharacterType MachiavelliReader::stringToCharacterType(std::string character)
{
	if (character.compare("Moordenaar") == 0){
		return CharacterType::Murderer;
	}
	else if (character.compare("Dief") == 0){
		return CharacterType::Thief;
	}
	else if (character.compare("Magiër") == 0){
		return CharacterType::Magicien;
	}
	else if (character.compare("Koning") == 0){
		return CharacterType::King;
	}
	else if (character.compare("Prediker") == 0){
		return CharacterType::Preacher;
	}
	else if (character.compare("Koopman") == 0){
		return CharacterType::Merchant;
	}
	else if (character.compare("Bouwmeester") == 0){
		return CharacterType::Architect;
	}
	else if (character.compare("Condottiere") == 0){
		return CharacterType::Condottiere;
	}
}
