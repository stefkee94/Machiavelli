#include "CharacterCard.h"


CharacterCard::CharacterCard(int id, std::string name, CharacterType type) : id(id), name(name), type(type)
{
}


CharacterCard::~CharacterCard()
{
}

CharacterType CharacterCard::get_type()
{
	return type;
}

std::string CharacterCard::get_name()
{
	return name;
}