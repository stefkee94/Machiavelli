#include "Player.h"


Player::Player(int client_id, std::string name, int age) : client_id(client_id), name(name), age(age), gold(0)
{

}


Player::~Player()
{
}

int Player::get_client_id()
{
	return client_id;
}

int Player::get_age()
{
	return age;
}
void Player::add_gold(int amount)
{
	gold += amount;
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