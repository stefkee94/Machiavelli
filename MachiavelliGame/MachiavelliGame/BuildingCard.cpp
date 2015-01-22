#include "BuildingCard.h"


BuildingCard::BuildingCard(std::string name, int points, CardColor color) : name(name), points(points), color(color)
{
}

BuildingCard::BuildingCard(std::string name, int points, CardColor color, std::string effect) : name(name), points(points), color(color), effect(effect)
{

}

std::string BuildingCard::get_name()
{
	return name;
}

std::string BuildingCard::get_effect()
{
	return effect;
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
void BuildingCard::set_color(CardColor color)
{
	this->color = color;
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

std::string BuildingCard::to_string()
{
	if (get_effect().compare("") == 0)
		return name + "(" + color_to_name() + ", " + std::to_string(points) + ")";
	else
		return name + "(" + color_to_name() + ", " + std::to_string(points) + ", " + get_effect() + ")";
}