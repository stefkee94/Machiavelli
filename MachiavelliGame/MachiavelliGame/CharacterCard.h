#pragma once
#include <string>
#include "CharacterType.h"

class CharacterCard
{
	public:
		CharacterCard(int id, std::string name, CharacterType type);
		virtual ~CharacterCard();
		std::string get_name();
		CharacterType get_type();
	private:
		int id;
		std::string name;
		CharacterType type;
};

