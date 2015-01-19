#include "CharacterCard.h"


CharacterCard::CharacterCard(int id, std::string name, CharacterType type) : id(id), name(name), type(type)
{
}


CharacterCard::~CharacterCard()
{
}

std::string CharacterCard::getName()
{
	return name;
}