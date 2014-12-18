#pragma once
#include <string>
#include "CharacterType.h"
class CharacterCard
{
public:
	CharacterCard(int id, std::string name, CharacterType type);
	virtual ~CharacterCard();
private:
	int id;
	std::string name;
	CharacterType type;
};

