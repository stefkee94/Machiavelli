#include "Player.h"

Player::Player(std::shared_ptr<Socket> client, std::string name, int age) : client(client), name(name), age(age), gold(0)
{
	this->is_king = false;
}


Player::~Player()
{
}

std::shared_ptr<Socket> Player::get_client()
{
	return client;
}

int Player::get_age()
{
	return age;
}

bool Player::has_character(std::string character_name)
{
	for (int i = 0; i < character_cards.size(); i++)
	{
		if (character_cards.get_card_at(i)->getName().compare(character_name))			
			return true;
	}
	return false;
}

void Player::add_gold(int amount)
{
	gold += amount;
}

int Player::get_gold()
{
	return gold;
}

CardStack<std::shared_ptr<BuildingCard>> Player::get_field_cards()
{
	return field_cards;
}

CardStack<std::shared_ptr<BuildingCard>> Player::get_hand_cards()
{
	return hand_cards;
}

bool Player::get_is_king()
{
	return is_king;
}

void Player::set_is_king(bool is_king)
{
	this->is_king = is_king;
}
void Player::remove_gold(int amount)
{
	if (gold - amount < 0)
		gold = 0;
	else
		gold -= amount;
}

void Player::add_card_to_hand(std::shared_ptr<BuildingCard> hand_card)
{
	hand_cards.add_card(hand_card);
}

void Player::put_card_on_field(std::shared_ptr<BuildingCard> field_card)
{
	field_cards.add_card(field_card);
}

void Player::add_character(std::shared_ptr<CharacterCard> character_card)
{
	character_cards.add_card(character_card);
}