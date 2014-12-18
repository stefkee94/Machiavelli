#pragma once
#include <string>
#include "CardColor.h"
class BuildingCard
{
public:
	BuildingCard(std::string name, int points, CardColor color);
	virtual ~BuildingCard();
private:
	std::string name;
	int points;
	CardColor color;
};

