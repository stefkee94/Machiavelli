#pragma once
#include <vector>
#include <memory>
#include "CardStack.h"
#include "BuildingCard.h"
#include "CharacterCard.h"

class Player
{
public:
	Player(std::string name, int age);
	virtual ~Player();
	void addCardToHand(std::shared_ptr<BuildingCard> handCards);
	void putCardOnField(std::shared_ptr<BuildingCard> fieldCards);
	void addCharacter(std::shared_ptr<CharacterCard> characterCards);
	void useCharacterProperty();
private:
	CardStack<std::shared_ptr<BuildingCard>> handCards;
	CardStack<std::shared_ptr<BuildingCard>> fieldCards;
	CardStack<std::shared_ptr<CharacterCard>> characterCards;
	std::string name;
	int age;
	int gold;
	bool isKing;
	bool charPropertyUsed;
};
