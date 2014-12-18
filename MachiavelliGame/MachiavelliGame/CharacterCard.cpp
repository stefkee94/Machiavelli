#include "CharacterCard.h"


CharacterCard::CharacterCard(int id, std::string name, CharacterType type)
{
	this->id = id;
	this->name = name;
	this->type = type;
}


CharacterCard::~CharacterCard()
{
}
