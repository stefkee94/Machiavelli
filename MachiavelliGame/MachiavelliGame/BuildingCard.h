#pragma once
#include <string>
#include "CardColor.h"

class BuildingCard
{
	public:
		BuildingCard(std::string name, int points, CardColor color);
		std::string get_name();
		std::string color_to_name();
		int get_points();
		virtual ~BuildingCard();

	private:
		std::string name;
		int points;
		CardColor color;
};

