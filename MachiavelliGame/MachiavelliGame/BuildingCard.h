#pragma once
#include <string>
#include "CardColor.h"

class BuildingCard
{
	public:
		BuildingCard(std::string name, int points, CardColor color);
		BuildingCard(std::string name, int points, CardColor color, std::string effect);
		std::string get_name();
		std::string color_to_name();
		int get_points();
		virtual ~BuildingCard();
		CardColor get_card_color();
		std::string to_string();
		void set_color(CardColor color);
		std::string get_effect();

	private:
		std::string name;
		int points;
		CardColor color;
		std::string effect;
};

