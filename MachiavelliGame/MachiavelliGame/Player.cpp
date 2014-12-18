#include "Player.h"


Player::Player(std::string name, int age)
{
	this->name = name;
	this->age = age;
}


Player::~Player()
{
}

void Player::addCardToHand(std::shared_ptr<BuildingCard> handCard)
{
	handCards.addCard(handCard);
}

void Player::putCardOnField(std::shared_ptr<BuildingCard> fieldCard)
{
	fieldCards.addCard(fieldCard);
}

void Player::addCharacter(std::shared_ptr<CharacterCard> characterCard)
{
	characterCards.addCard(characterCard);
}