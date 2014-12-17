#pragma once
#include <vector>
#include <memory>

#include "BuildingCard.h"
#include "CharacterCard.h"

class Player
{
public:
	Player();
	virtual ~Player();

private:
	std::vector<std::unique_ptr<BuildingCard>> hand_cards;
	std::vector<std::unique_ptr<CharacterCard>> field_cards;
};
