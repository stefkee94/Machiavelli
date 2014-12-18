#include "BuildingCard.h"


BuildingCard::BuildingCard(std::string name, int points, CardColor color)
{
	this->name = name;
	this->points = points;
	this->color = color;
}


BuildingCard::~BuildingCard()
{
}
