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
	CardStack<std::shared_ptr<CharacterCard>> readCharacterCards(std::string path);
	CardStack<std::shared_ptr<BuildingCard>> readBuildingCards(std::string path);
private:
	std::unique_ptr<BuildingCard> createBuildingCard(std::vector<std::string>& buildingCardValues);
	std::unique_ptr<CharacterCard> createCharacterCard(std::vector<std::string>& characterCardValues);
	CardColor stringToCardColor(std::string cardColor);
	CharacterType stringToCharacterType(std::string character);
};

