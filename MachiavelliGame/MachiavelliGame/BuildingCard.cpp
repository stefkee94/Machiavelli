#include "BuildingCard.h"


BuildingCard::BuildingCard(std::string name, int points, CardColor color) : name(name), points(points), color(color)
{
}

std::string BuildingCard::get_name()
{
	return name;
}

std::string BuildingCard::color_to_name()
{
	switch (color){
	case CardColor::Green:
		return "Green";
	case CardColor::Blue:
		return "Blue";
	case CardColor::Lilac:
		return "Lilac";
	case CardColor::Red:
		return "Red";
	case CardColor::Yellow:
		return "Yellow";
	}
}

CardColor BuildingCard::get_card_color()
{
	return color;
}

int BuildingCard::get_points()
{
	return points;
}

BuildingCard::~BuildingCard()
{
}