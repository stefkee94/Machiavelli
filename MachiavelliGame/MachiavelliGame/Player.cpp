#include "Player.h"

Player::Player(std::shared_ptr<Socket> client, std::string name, int age) : client(client), name(name), age(age), gold(0)
{
	this->is_king = false;
	this->is_robbed = false;
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

std::shared_ptr<CharacterCard> Player::has_character(std::string character_name)
{
	for (int i = 0; i < character_cards.size(); i++)
	{
		std::shared_ptr<CharacterCard> card = character_cards.get_card_at(i);
		if (card->get_name().compare(character_name) == 0)			
			return card;
	}
	return nullptr;
}

bool Player::has_field_card(std::string card_name)
{
	for (int i = 0; i < field_cards.size(); i++){
		if (field_cards.get_card_at(i)->get_name().compare(card_name) == 0){
			return true;
		}
	}
	return false;
}
void Player::set_type(CharacterType type){
	character_type = type;
}

CharacterType Player::get_char_type()
{
	return character_type;
}

void Player::reset_character_cards()
{
	character_cards.clear();
	for (int i = 0; i < character_cards.size(); i++){
		std::shared_ptr<CharacterCard> card = character_cards.get_card_and_remove_at_index(i);
		card.reset();
	}
}
void Player::remove_character_card(std::string character_name)
{
	for (int i = 0; i < character_cards.size(); i++){
		if (character_cards.get_card_at(i)->get_name().compare(character_name) == 0){
			std::shared_ptr<CharacterCard> card = character_cards.get_card_and_remove_at_index(i);
			card.reset();
		}
	}
}

void Player::remove_field_card(std::string name)
{
	for (int i = 0; i < field_cards.size(); i++){
		if (field_cards.get_card_at(i)->get_name().compare(name) == 0){
			std::shared_ptr<BuildingCard> card = field_cards.get_card_and_remove_at_index(i);
			card.reset();
		}
	}
}

void Player::remove_card_from_hand(std::string name)
{
	for (int i = 0; i < hand_cards.size(); i++){
		if (hand_cards.get_card_at(i)->get_name().compare(name) == 0){
			std::shared_ptr<BuildingCard> card = hand_cards.get_card_and_remove_at_index(i);
			card.reset();
		}
	}
}

bool Player::get_is_robbed()
{
	return is_robbed;
}

void Player::set_is_robbed(bool b)
{
	is_robbed = b;
}

void Player::add_gold(int amount)
{
	gold += amount;
}

int Player::get_gold()
{
	return gold;
}

void Player::add_points(int points)
{
	this->points += points;
}

int Player::get_points()
{
	return points;
}

CardStack<std::shared_ptr<BuildingCard>> Player::get_field_cards()
{
	return field_cards;
}

CardStack<std::shared_ptr<BuildingCard>> Player::get_hand_cards()
{
	return hand_cards;
}

std::shared_ptr<BuildingCard> Player::get_field_card(std::string card_name)
{
	for (int i = 0; i < field_cards.size(); i++)
	{
		std::shared_ptr<BuildingCard> card = field_cards.get_card_at(i);
		if (card->get_name().compare(card_name) == 0)
			return card;
	}
	return nullptr;
}

std::string Player::get_name()
{
	return name;
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

void Player::remove_card_from_hand(int index)
{
	hand_cards.get_card_and_remove_at_index(index);
}