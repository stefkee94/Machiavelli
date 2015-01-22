#include "GameController.h"
#include "ServerController.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

GameController::GameController()
{
	init();
}

GameController::~GameController()
{
}

void GameController::handle_client_command(std::shared_ptr<Socket> client, std::string new_command)
{
	if (new_command.compare("help") == 0)
	{
		show_help_text(client);
		return;
	}
	if (player_on_turn->get_client().get() == client.get())
	{
		if (fase == GamePhase::ChooseChar)
			hanlde_choose_char_command(new_command);
		else if (fase == GamePhase::DismissChar)
			handle_dismiss_char_command(new_command);
		else if (fase == GamePhase::PlayFase)
			handle_play_turn_command(new_command);
		else if (fase == GamePhase::ChooseBuildingCard)
			handle_choose_building_card(new_command);
		else if (fase == GamePhase::BuildCard)
			handle_build_card(new_command);
		else if (fase == GamePhase::MagicienProperty)
			handle_magicien_property(new_command);
		else if (fase == GamePhase::MagicienTradeBank)
			handle_magicien_trade_bank_prop(new_command);
		else if (fase == GamePhase::MurderPhase)
			handle_murder_character(new_command);
		else if (fase == GamePhase::ThiefPhase)
			handle_steal_from_character(new_command);
		else if (fase == GamePhase::CondottierePhase)
			handle_condottiere_phase(new_command);
		else if (fase == GamePhase::BuyDestroyedBuildingPhase)
			handle_buy_destroyed_building(new_command);
		else if (fase == GamePhase::LaboratoryPhase)
			handle_labroratory_choice(new_command);
		else if (fase == GamePhase::SchoolOfMagicPhase)
			handle_school_of_magic_choice(new_command);
	}
}

void GameController::consume_command(ClientCommand command, std::shared_ptr<Socket> client)
{
	try
	{
		if (players.size() == 2)
		{
			if (player_on_turn->get_client().get() != client.get())
				client->write("It's not your turn \r\n");
			else
				handle_client_command(client, command.get_command());
		}
		else
			client->write("Please wait for the other player to connect \r\n");
	}
	catch (const std::exception& ex)
	{
		client->write("Sorry, ");
		client->write(ex.what());
		client->write("\n");
	}
	catch (...)
	{
		client->write("Sorry, something went wrong with handling request");
	}

	client->write(">");
}

void GameController::hanlde_choose_char_command(std::string new_command)
{
	bool is_command_digit = false;
	int choice;
	while (!is_command_digit)
	{
		choice = atoi(new_command.c_str());
		if ((choice > 0 || new_command.compare("0") == 0) && (choice < character_cards.size()))
			is_command_digit = true;
		else
		{
			player_on_turn->get_client()->write("Invalid text, please fill in valid text to choose a character \r\n");
			return;
		}
	}
	player_on_turn->get_client()->write("You chose the : " + character_cards.get_card_at(choice)->get_name() + "\r\n");
	player_on_turn->add_character(character_cards.get_card_and_remove_at_index(choice));
	if (first_pick)
	{
		first_pick = false;
		set_turn_to_next_player();
		choose_character();
	}
	else
	{
		fase = GamePhase::DismissChar;
		dismiss_character();
	}
}

void GameController::handle_condottiere_phase(std::string new_command)
{
	if (condottiere_choices.size() >= 7)
	{
		player_on_turn->get_client()->write("You can't destory a building which is a city of 8 buildings or more \r\n");
		return;
	}

	bool is_command_digit = false;
	int choice;

	while (!is_command_digit)
	{
		choice = atoi(new_command.c_str());
		if ((choice > 0 || new_command.compare("0") == 0) && (choice < condottiere_choices.size() - 1))
			is_command_digit = true;
		else
		{
			player_on_turn->get_client()->write("Invalid text, please fill in valid text to destroy the opponents building \r\n");
			return;
		}
	}

	std::shared_ptr<BuildingCard> card_to_destroy = condottiere_choices[choice];
	if (player_on_turn->get_gold() - (card_to_destroy->get_points() - 1) < 0)
		player_on_turn->get_client()->write("Can't destroy the " + card_to_destroy->get_name() + " with " + std::to_string(card_to_destroy->get_points()) + " gold, because you don't have enough gold \r\n");

	if (card_to_destroy->get_points() > 0)
		player_on_turn->remove_gold(card_to_destroy->get_points() - 1);

	for (int i = 0; i < players.size(); i++)
	{
		if (player_on_turn != players[i])
			players[i]->remove_field_card(card_to_destroy->get_name());
	}

	player_on_turn->get_client()->write("You destroyed " + card_to_destroy->get_name() + " with " + std::to_string(card_to_destroy->get_points()) + " gold from your enemy \r\n");

	check_for_graveyard(card_to_destroy->get_name());
}

void GameController::check_for_graveyard(std::string card_name)
{
	for (int i = 0; i < players.size(); i++)
	{
		if (players[i]->has_field_card("Kerkhof") && players[i]->get_char_type() != CharacterType::Condottiere)
		{
			fase = GamePhase::BuyDestroyedBuildingPhase;
			player_on_turn = players[i];
			player_on_turn->get_client()->write("You got the graveyard! \r\n");
			player_on_turn->get_client()->write("Would you like to buy " + card_name + "for 1 gold? \r\n");
			player_on_turn->get_client()->write("[0]: Yes \r\n");
			player_on_turn->get_client()->write("[1]: No \r\n");
		}
		else
		{
			fase = GamePhase::PlayFase;
			print_turn_info();
		}
	}
}

void GameController::handle_school_of_magic_choice(std::string new_command)
{
	int choice; 
	choice = atoi(new_command.c_str());
	std::shared_ptr<BuildingCard> card = player_on_turn->get_field_card("School voor magiërs");
	if (card != nullptr)
	{
		switch (choice)
		{
			case 0:
				card->set_color(CardColor::Yellow);
				player_on_turn->get_client()->write("You changed the school to yellow \r\n");
				break;
			case 1:
				card->set_color(CardColor::Green);
				player_on_turn->get_client()->write("You changed the school to green \r\n");
				break;
			case 2:
				card->set_color(CardColor::Blue);
				player_on_turn->get_client()->write("You changed the school to blue \r\n");
				break;
			case 3:
				card->set_color(CardColor::Red);
				player_on_turn->get_client()->write("You changed the school to red \r\n");
				break;
			case 4:
				card->set_color(CardColor::Lilac);
				player_on_turn->get_client()->write("You changed the school to lilac \r\n");
				break;
		}
	}
	fase = GamePhase::PlayFase;
	print_turn_info();
}

void GameController::handle_workplace()
{
	player_on_turn->remove_gold(3);
	for (int i = 0; i < 2; i++)
	{
		std::shared_ptr<BuildingCard> card = building_cards.get_card_at_top();
		player_on_turn->add_card_to_hand(card);
		player_on_turn->get_client()->write("You picked: " + card->to_string() + "\r\n");
	}

	print_turn_info();

}

void GameController::handle_buy_destroyed_building(std::string new_command)
{
	int choice;
	choice = atoi(new_command.c_str());
	switch (choice)
	{
		case 0:
			player_on_turn->remove_gold(1);
			player_on_turn->add_card_to_hand(card_to_destroy);
			player_on_turn->get_client()->write("You bought " + card_to_destroy->to_string());
			card_to_destroy.reset();
			break;
		case 1:
			card_to_destroy.reset();
			break;
	}

	for (int i = 0; i < players.size(); i++)
	{
		if (players[i]->get_char_type() == CharacterType::Condottiere)
			player_on_turn = players[i];
	}

	fase = GamePhase::PlayFase;
	print_turn_info();
	
}

void GameController::handle_dismiss_char_command(std::string new_command)
{
	bool is_command_digit = false;
	int choice;

	if (character_cards.size() > 0)
	{
		while (!is_command_digit)
		{
			choice = atoi(new_command.c_str());
			if ((choice > 0 || new_command.compare("0") == 0) && (choice < character_cards.size()))
				is_command_digit = true;
			else
			{
				player_on_turn->get_client()->write("Invalid text, please fill in valid text to dismiss a character \r\n");
				return;
			}
		}

		player_on_turn->get_client()->write("You dismissed the : " + character_cards.get_card_at(choice)->get_name() + "\r\n");

		character_cards.get_card_and_remove_at_index(choice);
		set_turn_to_next_player();
	}

	if (character_cards.size() > 0)
	{
		fase = GamePhase::ChooseChar;
		choose_character();
	}
}

void GameController::handle_play_turn_command(std::string new_command)
{
	bool is_command_digit = false;
	int choice;
	while (!is_command_digit)
	{
		choice = atoi(new_command.c_str());
		if ((choice > 0 || new_command.compare("0") == 0) && choice <= turn_choices.size()-1)
			is_command_digit = true;
		else
		{
			player_on_turn->get_client()->write("Invalid text, please fill in valid text to play a turn \r\n");
			return;
		}
	}
	int turn_number = turn_choices[choice].first;
	switch (turn_number)
	{
		case 0:
			take_gold(2);
		break;
		case 1:
			take_building_cards();
		break;
		case 2:
		{
			if (player_on_turn->get_char_type() == CharacterType::Architect)
			{
				count_builded_in_turn_for_architect++;
				if (count_builded_in_turn_for_architect >= 3)
					remove_choice(choice);
			}
			else{
				remove_choice(choice);
			}
			build_building_card();
		}
		break;
		case 3:
			remove_choice(choice);
			handle_char_property();
		break;
		case 4:
			handle_end_turn();
		break;
		case 5:
			handle_laboratory();
			break;
		case 6:
			handle_workplace();
			break;
	}
}

void GameController::handle_end_turn()
{
	if (player_on_turn->get_char_type() == CharacterType::Thief)
	{
		for (int i = 0; i < players.size(); i++)
		{
			if (players[i]->get_is_robbed())
			{
				player_on_turn->add_gold(players[i]->get_gold());
				players[i]->remove_gold(players[i]->get_gold());
			}
		}
	}
	set_turn_choices();
	call_count++;
	call_next_char();
}

void GameController::handle_steal_from_character(std::string new_command)
{
	bool is_command_digit = false;
	int choice;
	while (!is_command_digit)
	{
		choice = atoi(new_command.c_str());
		for (auto it : thief_choices)
		{
			if (it.first == choice)
			{
				is_command_digit = true;
				break;
			}
		}
		if (!is_command_digit)
		{
			player_on_turn->get_client()->write("Invalid text, please fill in valid text to steal from a character \r\n");
			return;
		}
	}

	for (int i = 0; i < players.size(); i++)
	{
		if (players[i]->has_character(thief_choices[choice]) != nullptr)
			players[i]->set_is_robbed(true);
	}

	player_on_turn->get_client()->write("You robbed all the money of the : " + thief_choices[choice] + "\r\n");

	fase = GamePhase::PlayFase;
	print_turn_info();
}

void GameController::handle_choose_building_card(std::string new_command)
{
	bool is_command_digit = false;
	int choice;

	while (!is_command_digit)
	{
		choice = atoi(new_command.c_str());
		if (choice == 1 || new_command.compare("0") == 0)
			is_command_digit = true;
		else
		{
			player_on_turn->get_client()->write("Invalid text, please fill in valid text to choose a building card \r\n");
			return;
		}
	}
	player_on_turn->add_card_to_hand(picked_building_cards[choice]);

	player_on_turn->get_client()->write("You picked up : " + picked_building_cards[choice]->get_name() + "\r\n");// card
	
	picked_building_cards.clear();

	remove_choice(0);
	remove_choice(0);

	fase = GamePhase::PlayFase;
	print_turn_info();
}

void GameController::handle_char_property()
{
	switch (player_on_turn->get_char_type())
	{
		case CharacterType::Murderer:
			player_on_turn->get_client()->write("Who do you want to murder? \r\n");
			for (auto it : murderer_choices){
				player_on_turn->get_client()->write("[" + std::to_string(it.first) + "]: " + it.second + "\r\n");
			}
			fase = GamePhase::MurderPhase;
		break;
		case CharacterType::Thief:
			player_on_turn->get_client()->write("Steal from? \r\n");
			if (thief_choices.empty())
				init_thief_choices("");

			for (auto it : thief_choices)
				player_on_turn->get_client()->write("[" + std::to_string(it.first) + "]: " + it.second + "\r\n");

			fase = GamePhase::ThiefPhase;
		break;
		case CharacterType::Magicien:
			do_magicien_property();
		break;
		case CharacterType::King:
		{
			int yellow_cards_on_field = 0;
			for (int i = 0; i < player_on_turn->get_field_cards().size(); i++)
			{
				if (!player_on_turn->get_field_cards().is_empty() && player_on_turn->get_field_cards().get_card_at(i)->get_card_color() == CardColor::Yellow)
				{
					player_on_turn->add_gold(1);
					yellow_cards_on_field++;
				}
			}
			player_on_turn->get_client()->write("You got " + std::to_string(yellow_cards_on_field) + " gold from the yellow buildings \r\n");
			print_turn_info();
			fase = GamePhase::PlayFase;
		}
		break;
		case CharacterType::Preacher:
		{
			int blue_cards_on_field = 0;
			for (int i = 0; i < player_on_turn->get_field_cards().size(); i++)
			{
				if (player_on_turn->get_field_cards().get_card_at(i)->get_card_color() == CardColor::Blue)
				{
					player_on_turn->add_gold(1);
					blue_cards_on_field++;
				}
			}
			player_on_turn->get_client()->write("You got " + std::to_string(blue_cards_on_field) + "gold from the blue buildings \r\n");
			print_turn_info();
			fase = GamePhase::PlayFase;
		}
		break;
		case CharacterType::Merchant:
		{
			int green_cards_on_field = 0;
			for (int i = 0; i < player_on_turn->get_field_cards().size(); i++)
			{
				if (player_on_turn->get_field_cards().get_card_at(i)->get_card_color() == CardColor::Green)
				{
					player_on_turn->add_gold(1);
					green_cards_on_field++;
				}
			}
			player_on_turn->get_client()->write("You got " + std::to_string(green_cards_on_field) + "gold from the green buildings \r\n");
			print_turn_info();
			fase = GamePhase::PlayFase;
		}
		break;
		case CharacterType::Architect:
			for (int i = 0; i < 2; i++)
			{
				std::shared_ptr<BuildingCard> new_card = building_cards.get_card_at_top();
				player_on_turn->get_client()->write("You picked up : " + new_card->get_name() + "(" + new_card->color_to_name() + ", " + std::to_string(new_card->get_points()) + ") \r\n");
				player_on_turn->add_card_to_hand(new_card);
			}
			fase = GamePhase::PlayFase;
			print_turn_info();
		break;
		case CharacterType::Condottiere:
		{
			int red_cards_on_field = 0;
			for (int i = 0; i < player_on_turn->get_field_cards().size(); i++)
			{
				if (player_on_turn->get_field_cards().get_card_at(i)->get_card_color() == CardColor::Red)
				{
					player_on_turn->add_gold(1);
					red_cards_on_field++;
				}
			}
			player_on_turn->get_client()->write("You got " + std::to_string(red_cards_on_field) + " gold from the red buildings \r\n");

			condottiere_choices.clear();
			for (int i = 0; i < players.size(); i++)
			{
				std::shared_ptr<Player> player = players[i];
				if (player_on_turn != player && player->has_character("Preacher") == nullptr)
				{
					for (int a = 0; a < player->get_field_cards().size(); a++)
					{
						std::shared_ptr<BuildingCard> card = player->get_field_cards().get_card_at(a);
						if (card->get_name().compare("Kerker") != 0)
							condottiere_choices.insert(std::make_pair(a, player->get_field_cards().get_card_at(a)));
					}
				}
			}
			if (condottiere_choices.size() > 0)
			{
				player_on_turn->get_client()->write("Which building would you like to destroy? \r\n");
				for (auto it : condottiere_choices)
					player_on_turn->get_client()->write("[" + std::to_string(it.first) + "]: " + it.second->to_string() + "\r\n");

				fase = GamePhase::CondottierePhase;
			}
			else
			{
				player_on_turn->get_client()->write("You can't destroy any buildings\r\n");
				fase = GamePhase::PlayFase;
				print_turn_info();
			}
		}
		break;
	}
}

void GameController::handle_murder_character(std::string new_command)
{
	bool is_command_digit = false;
	int choice;
	while (!is_command_digit)
	{
		choice = atoi(new_command.c_str());
		if ((choice > 0 && choice <= murderer_choices.size()) || new_command.compare("0") == 0)
			is_command_digit = true;
		else
		{
			player_on_turn->get_client()->write("Invalid text, please fill in valid text to murder a character \r\n");
			return;
		}
	}
	for (int i = 0; i < players.size(); i++)
		players[i]->remove_character_card(murderer_choices[choice]);

	player_on_turn->get_client()->write("The murderer killed the " + murderer_choices[choice] + "\r\n");
	fase = GamePhase::PlayFase;
	init_thief_choices(murderer_choices[choice]);
	print_turn_info();
}

void GameController::init_thief_choices(std::string name_of_murdered)
{
	for (int i = 0; i < char_order.size(); i++)
	{
		if (char_order[i].compare("Murderer") == 0 || char_order[i].compare("Thief") == 0 ||char_order[i].compare(name_of_murdered) == 0)
			continue;
		thief_choices.insert(std::make_pair(i, char_order[i]));
	}
}

void GameController::handle_laboratory()
{
	player_on_turn->get_client()->write("Which buildingcard you want to turn in for 1 gold? \r\n");
	for (int i = 0; i < player_on_turn->get_hand_cards().size(); i++)
	{
		std::shared_ptr<BuildingCard> card = player_on_turn->get_hand_cards().get_card_at(i);
		lab_choices.insert(std::make_pair(i, card));
		player_on_turn->get_client()->write("[" + std::to_string(i) + "]: " + card->to_string());
	}

	fase = GamePhase::LaboratoryPhase;
}

void GameController::handle_labroratory_choice(std::string new_command)
{
	bool is_command_digit = false;
	int choice;

	while (!is_command_digit)
	{
		choice = atoi(new_command.c_str());
		if ((choice > 0 && choice <= player_on_turn->get_hand_cards().size()) || new_command.compare("0") == 0)
			is_command_digit = true;
		else
		{
			player_on_turn->get_client()->write("Invalid text, please fill in valid text to handle the labratory choice \r\n");
			return;
		}
	}

	player_on_turn->get_client()->write("You destroyed " + lab_choices[choice]->get_name() + " for one gold \r\n");
	player_on_turn->remove_card_from_hand(lab_choices[choice]->get_name());
	player_on_turn->add_gold(1);

	fase = GamePhase::PlayFase;
	print_turn_info();
}

void GameController::handle_build_card(std::string new_command)
{
	bool is_command_digit = false;
	int choice;

	while (!is_command_digit)
	{
		choice = atoi(new_command.c_str());
		if ((choice > 0 && choice <= player_on_turn->get_hand_cards().size()) || new_command.compare("0") == 0)
			is_command_digit = true;
		else
		{
			player_on_turn->get_client()->write("Invalid text, please fill in valid text to play a building card \r\n");
			return;
		}
	}

	std::shared_ptr<BuildingCard> chosen_building_card = player_on_turn->get_hand_cards().get_card_at(choice);
	if (chosen_building_card->get_points() > player_on_turn->get_gold())
		player_on_turn->get_client()->write("Can't play this card because you need " + std::to_string(chosen_building_card->get_points()) + " points to play it \r\nPlease choose another card \r\n");
	else
	{
		player_on_turn->remove_gold(chosen_building_card->get_points());
		player_on_turn->put_card_on_field(chosen_building_card);
		player_on_turn->remove_card_from_hand(choice);
		player_on_turn->get_client()->write("You have built : " + chosen_building_card->get_name() + "(" + chosen_building_card->color_to_name() + ", " + std::to_string(chosen_building_card->get_points()) + ") \r\n");
		fase = GamePhase::PlayFase;
		print_turn_info();
	}
}

void GameController::call_next_char()
{
	if (call_count < char_order.size()){
		for (int i = 0; i < players.size(); i++)
		{
			std::shared_ptr<CharacterCard> card = players[i]->has_character(char_order[call_count]);
			if (card != nullptr)
			{
				player_on_turn = players[i];
				player_on_turn->set_type(card->get_type());
				if (player_on_turn->get_char_type() == CharacterType::Merchant)
					player_on_turn->add_gold(1);

				if (player_on_turn->has_field_card("School voor magiërs"))
				{
					player_on_turn->get_client()->write("You are the master of the school of magics, which color should it become? \r\n");
					player_on_turn->get_client()->write("[0]: Yellow");
					player_on_turn->get_client()->write("[1]: Green");
					player_on_turn->get_client()->write("[2]: Blue");
					player_on_turn->get_client()->write("[3]: Red");
					player_on_turn->get_client()->write("[4]: Lilac");
					fase = GamePhase::SchoolOfMagicPhase;
					return;
				}
				print_turn_info();
				return;
			}
		}
		for (int i = 0; i < players.size(); i++)
			players[i]->get_client()->write("\r\n>The card : " + char_order[call_count] + " is on the discard pile \r\n");

		call_count++;
		call_next_char();
	}
	else
	{
		set_new_king();
		reset_characters();
		for (int i = 0; i < players.size(); i++)
			players[i]->get_client()->write("<----------------------- NEXT ROUND ------------------------> \r\n");

		MachiavelliReader reader;
		character_cards = reader.read_character_cards("karakters.csv");
		call_count = 0;
		count_builded_in_turn_for_architect = 0;
		first_pick = true;
		fase = GamePhase::ChooseChar;
		choose_character();
	}
}

void GameController::reset_characters()
{
	for (int i = 0; i < players.size(); i++){
		players[i]->reset_character_cards();
	}
}

void GameController::set_new_king()
{
	for (int i = 0; i < players.size(); i++){
		if (players[i]->has_character("King") != nullptr || players[i]->get_is_king()){
			players[i]->set_is_king(true);
			player_on_turn = players[i];
		}
		else{
			players[i]->set_is_king(false);
		}
	}
}

void GameController::print_turn_info()
{
	player_on_turn->get_client()->write("As of now you're the " + char_order[call_count] + "\r\n");
	player_on_turn->get_client()->write("Gold: " + std::to_string(player_on_turn->get_gold()) + "\r\n\r\n");
	player_on_turn->get_client()->write("Field: \r\n");

	for (int i = 0; i < player_on_turn->get_field_cards().size(); i++)
	{
		std::shared_ptr<BuildingCard> card = player_on_turn->get_field_cards().get_card_at(i);
		player_on_turn->get_client()->write(card->to_string() + "\r\n");
	}

	player_on_turn->get_client()->write("\r\n");
	player_on_turn->get_client()->write("Hand: \r\n");

	for (int i = 0; i < player_on_turn->get_hand_cards().size(); i++)
	{
		std::shared_ptr<BuildingCard> card = player_on_turn->get_hand_cards().get_card_at(i);
		player_on_turn->get_client()->write(card->to_string() + "\r\n");
	}

	player_on_turn->get_client()->write("\r\n");
	player_on_turn->get_client()->write("What to do? \r\n");

	for (int i = 0; i < turn_choices.size(); i++)
		player_on_turn->get_client()->write("[" + std::to_string(i) + "] : " + turn_choices[i].second);

	player_on_turn->get_client()->write(">");
}

void GameController::set_turn_to_next_player()
{
	for (int i = 0; i < players.size(); i++)
	{
		if (players[i] != player_on_turn)
		{
			player_on_turn = players[i];
			set_turn_choices();
			return;
		}
	}
}

void GameController::connect_player(std::shared_ptr<Socket> client, std::string name, std::string age)
{
	players.push_back(std::make_shared<Player>(client, name, std::atoi(age.c_str())));
	if (players.size() >= 2) // needed to start the game
		start_game();
}

void GameController::start_game()
{
	for (std::shared_ptr<Player> player : players)
	{
		// Add 4 gold to each player
		player->add_gold(4);
		// Give each player 4 building cards
		for (int i = 0; i < 4; i++)
			player->add_card_to_hand(building_cards.get_card_at_top());
	}
	// geef beurt aan de speler
	if (players[0]->get_age() >= players[1]->get_age())
		player_on_turn = players[0];
	else
		player_on_turn = players[1];

	player_on_turn->set_is_king(true);
	set_turn_choices();
	fase = GamePhase::ChooseChar;
	first_pick = true;

	choose_character();
}

void GameController::take_gold(int amount)
{
	player_on_turn->add_gold(amount);

	remove_choice(0);
	remove_choice(0);

	player_on_turn->get_client()->write("You picked up " + std::to_string(amount) + " gold \r\n");
	player_on_turn->get_client()->write(">");

	print_turn_info();
}

void GameController::take_building_cards()
{
	int draw_counter = 2;
	if (player_on_turn->has_field_card("Bibliotheek"))
	{
		player_on_turn->get_client()->write("Thanks to the library, you may keep both cards \r\n");
		for (int i = 0; i < draw_counter; i++)
		{
			std::shared_ptr<BuildingCard> card = building_cards.get_card_at_top();
			player_on_turn->add_card_to_hand(card);
			player_on_turn->get_client()->write("You picked: " + card->to_string() + "\r\n");
		}

		print_turn_info();
	}
	else
	{
		if (player_on_turn->has_field_card("Observatorium"))
			draw_counter = 3;

		for (int i = 0; i < draw_counter; i++)
			picked_building_cards.push_back(building_cards.get_card_at_top());

		player_on_turn->get_client()->write("You picked these cards, you have to chose one of these and throw the other away \r\n");

		for (int j = 0; j < picked_building_cards.size(); j++)
			player_on_turn->get_client()->write("[" + std::to_string(j) + "] : " + picked_building_cards[j]->to_string() + "\r\n");
		player_on_turn->get_client()->write(">");

		fase = GamePhase::ChooseBuildingCard;
	}
}

void GameController::build_building_card()
{
	fase = GamePhase::BuildCard;
	player_on_turn->get_client()->write("All the hand cards : \r\n");

	for (int i = 0; i < player_on_turn->get_hand_cards().size(); i++)
		player_on_turn->get_client()->write("[" + std::to_string(i) + "] : " + player_on_turn->get_hand_cards().get_card_at(i)->to_string() + "\r\n");

	player_on_turn->get_client()->write("Choose one of these cards to build \r\n");
}

void GameController::choose_character()
{
	if (player_on_turn->get_is_king() && first_pick)
	{
		character_cards.get_card_at_top();
	}
	player_on_turn->get_client()->write("Choose a character \r\n");
	for (int i = 0; i < character_cards.size(); i++)
	{
		player_on_turn->get_client()->write("[" + std::to_string(i) + "]: " + character_cards.get_card_at(i)->get_name() + "\r\n");
	}
	player_on_turn->get_client()->write(">");
}

void GameController::dismiss_character()
{
	player_on_turn->get_client()->write("Dismiss a character \r\n");

	for (int i = 0; i < character_cards.size(); i++)
	{
		if (character_cards.size() == 1)
		{
			player_on_turn->get_client()->write("Dismissed : " + character_cards.get_card_at(i)->get_name() + "\r\n");
			character_cards.get_card_and_remove_at_index(0);
		}
		else
			player_on_turn->get_client()->write("[" + std::to_string(i) + "]: " + character_cards.get_card_at(i)->get_name() + "\r\n");
	}

	if (character_cards.size() == 0)
	{
		fase = GamePhase::PlayFase;
		call_next_char();
	}
}

void GameController::do_magicien_property()
{
	player_on_turn->get_client()->write("What do you want to do? \r\n");
	player_on_turn->get_client()->write("[0] : Trade hand cards with another player \r\n");
	player_on_turn->get_client()->write("[1] : Chose cards to exchange with the bank \r\n");
	player_on_turn->get_client()->write(">");

	fase = GamePhase::MagicienProperty;
}

void GameController::handle_magicien_property(std::string new_command)
{
	bool is_command_digit = false;
	int choice;

	while (!is_command_digit)
	{
		choice = atoi(new_command.c_str());
		if ((choice > 0 && choice < 3) || new_command.compare("0") == 0)
			is_command_digit = true;
		else
		{
			player_on_turn->get_client()->write("Invalid text, please fill in valid text to play the magicien property \r\n");
			return;
		}
	}

	switch (choice)
	{
		case 0:
			magicien_trade_cards_with_player();
		break;
		case 1:
			magicien_trade_cards_with_bank();
		break;
	}
}

void GameController::magicien_trade_cards_with_player()
{
	CardStack<std::shared_ptr<BuildingCard>> current_player_cards = player_on_turn->get_hand_cards();
	CardStack<std::shared_ptr<BuildingCard>> other_player_cards;
	CardStack<std::shared_ptr<BuildingCard>> tmp_player_cards;

	for (int i = 0; i < players.size(); i++)
	{
		if (players[i] != player_on_turn)
		{
			other_player_cards = players[i]->get_hand_cards();
			break;
		}
	}
	// Change cards to other players
	tmp_player_cards = current_player_cards;
	current_player_cards.clear();
	current_player_cards = other_player_cards;
	other_player_cards.clear();
	other_player_cards = tmp_player_cards;

	player_on_turn->get_client()->write("You have swapped your cards with the other player \r\n");
	
	fase = GamePhase::PlayFase;
	print_turn_info();
}

void GameController::magicien_trade_cards_with_bank()
{
	for (int i = 0; i < player_on_turn->get_hand_cards().size(); i++)
	{
		std::shared_ptr<BuildingCard> card = player_on_turn->get_hand_cards().get_card_at(i);
		player_on_turn->get_client()->write(card->to_string() + "\r\n");
	}

	player_on_turn->get_client()->write("which cards do you want to replace? Write the index including a comma \r\n >");

	fase = GamePhase::MagicienTradeBank;
}

bool greaterThan(std::string i, std::string j) { return i > j; }

void GameController::handle_magicien_trade_bank_prop(std::string new_command)
{
	std::vector<std::string> card_indices;
	std::stringstream ss(new_command);
	std::string buffer;

	while (std::getline(ss, buffer, ',')) {
		card_indices.push_back(buffer);
	}

	std::sort(card_indices.begin(), card_indices.end(), greaterThan);

	for (int x = 0; x < card_indices.size(); x++)
	{
		int choice = atoi(card_indices[x].c_str());
		if ((choice == 0 && card_indices[x].compare("0") != 0) || choice > player_on_turn->get_hand_cards().size() - 1)
		{
			player_on_turn->get_client()->write("Invalid text, please fill in valid text for the numbers to exchange with the bank \r\n");
			return;
		}
	}

	int count_of_new_cards = card_indices.size();
	for (int i = 0; i < card_indices.size(); i++)
	{
		player_on_turn->get_client()->write("Removed : " + player_on_turn->get_hand_cards().get_card_at(atoi(card_indices.at(i).c_str()))->get_name() + "\r\n");
		player_on_turn->remove_card_from_hand(atoi(card_indices.at(i).c_str()));
	}

	for (int j = 0; j < count_of_new_cards; j++)
	{
		std::shared_ptr<BuildingCard> new_card = building_cards.get_card_at_top();
		player_on_turn->get_client()->write("You picked up : " + new_card->to_string() + "\r\n");

		player_on_turn->add_card_to_hand(new_card);
	}

	fase = GamePhase::PlayFase;

	print_turn_info();
}

std::vector<std::pair<int, std::string>> GameController::get_turn_choices()
{
	return turn_choices;
}

void GameController::set_turn_choices()
{
	turn_choices.clear();
	for (int x = 0; x < init_choices.size(); x++)
		turn_choices.push_back(std::pair<int, std::string>(x,init_choices[x]));
	if (player_on_turn->has_field_card("Laboratorium")){
		turn_choices.push_back(std::pair<int, std::string>(turn_choices.size(), "Drop buildingcard and get one gold \r\n"));
	}
	else if (player_on_turn->has_field_card("Werkplaats")){
		turn_choices.push_back(std::pair<int, std::string>(turn_choices.size(), "Pay 3 gold and draw two building cards \r\n"));
	}
}

void GameController::remove_choice(int index)
{
	turn_choices.erase(turn_choices.begin() + index );
}

void GameController::show_help_text(std::shared_ptr<Socket> client)
{
	client->write("\r\nFront : \r\n");
	client->write("Inkomsten -> Take 2 gold or take 2 building cards and choose one \r\n");
	client->write("Bouwen -> Build 1 building card and pay the value \r\n");
	client->write("Character property can be used at every moment when it's your turn \r\n");
	client->write("1. Murderer -> Kill another character \r\n");
	client->write("2. Thief -> Steal from another character \r\n");
	client->write("3. Magicien -> Change building cards \r\n");
	client->write("4. King -> Begin next turn, get gold from monuments \r\n");
	client->write("5. Preacher -> Protected against the Condottiere & get gold from church buildings \r\n");
	client->write("6. Merchant -> Gets 1 extra gold and get gold from commercial buildings \r\n");
	client->write("7. Architect -> Get two extra cards and is able to build 3 buildings in one turn \r\n");
	client->write("8. Condottiere -> Destroys a building and get gold from military buildings\r\n");
}

void GameController::init()
{
	MachiavelliReader reader;
	building_cards = reader.read_building_cards("Bouwkaarten.csv");
	character_cards = reader.read_character_cards("karakters.csv");

	building_cards.shuffle_cards();
	character_cards.shuffle_cards();

	char_order.push_back("Murderer");
	char_order.push_back("Thief");
	char_order.push_back("Magicien");
	char_order.push_back("King");
	char_order.push_back("Preacher");
	char_order.push_back("Merchant");
	char_order.push_back("Architect");
	char_order.push_back("Condottiere");

	init_choices.push_back("Take 2 gold\r\n");
	init_choices.push_back("Take 2 building cards and choose one \r\n");
	init_choices.push_back("Build 1 building card and pay the value \r\n");
	init_choices.push_back("Play character property \r\n");
	init_choices.push_back("End turn \r\n");

	murderer_choices = std::map<int, std::string>{
		{ 0, "Thief" },
		{ 1, "Magicien"},
		{ 2, "King"},
		{ 3, "Preacher"},
		{ 4, "Merchant"},
		{ 5, "Architect"},
		{ 6, "Condottiere"}
	};
	count_builded_in_turn_for_architect = 0;
	first_pick = true;
}

