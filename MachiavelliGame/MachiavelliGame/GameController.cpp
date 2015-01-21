#include "GameController.h"
#include "ServerController.h"

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
	player_on_turn->get_client()->write("You chose the : " + character_cards.get_card_at(choice)->getName() + "\r\n");
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

		player_on_turn->get_client()->write("You dismissed the : " + character_cards.get_card_at(choice)->getName() + "\r\n");

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
		if (choice > 0 || new_command.compare("0") == 0)
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
			build_building_card();
		break;
	}
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

	// Move on to the next fase
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
		player_on_turn->get_client()->write("Can't play this card because you need " + std::to_string(chosen_building_card->get_points()) + " points to play it \r\n Please choose another card");
	else
	{
		// put card on the field and remove from the hand
		player_on_turn->put_card_on_field(chosen_building_card);
		player_on_turn->remove_card_from_hand(choice);

		player_on_turn->get_client()->write("You have built : " + chosen_building_card->get_name() + "(" + chosen_building_card->color_to_name() + ", " + std::to_string(chosen_building_card->get_points()) + ") \r\n");
		
		remove_choice(choice);
		fase = GamePhase::PlayFase;
		print_turn_info();

	}
}

void GameController::call_next_char()
{
	for (int i = 0; i < players.size(); i++)
	{
		if (players[i]->has_character(char_order[call_count]))
		{
			player_on_turn = players[i];
			print_turn_info();
			return;
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
		player_on_turn->get_client()->write(card->get_name() + "(" + card->color_to_name() + ", " + std::to_string(card->get_points()) + ") \r\n");
	}

	player_on_turn->get_client()->write("\r\n");
	player_on_turn->get_client()->write("Hand: \r\n");

	for (int i = 0; i < player_on_turn->get_hand_cards().size(); i++)
	{
		std::shared_ptr<BuildingCard> card = player_on_turn->get_hand_cards().get_card_at(i);
		player_on_turn->get_client()->write(card->get_name() + "(" + card->color_to_name() + ", " + std::to_string(card->get_points()) + ") \r\n");
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
	fase = GamePhase::ChooseBuildingCard;
	for (int i = 0; i < 2; i++)
		picked_building_cards.push_back(building_cards.get_card_at_top());
	player_on_turn->get_client()->write("You picked these cards, you have to chose one of these and throw the other away \r\n");
	for (int j = 0; j < picked_building_cards.size(); j++)
		player_on_turn->get_client()->write("[" + std::to_string(j) + "] : " + picked_building_cards[j]->get_name() + "(" + picked_building_cards[j]->color_to_name() + ", " + std::to_string(picked_building_cards[j]->get_points()) + ") \r\n");
	player_on_turn->get_client()->write(">");
}

void GameController::build_building_card()
{
	fase = GamePhase::BuildCard;
	player_on_turn->get_client()->write("All the hand cards : \r\n");

	for (int i = 0; i < player_on_turn->get_hand_cards().size(); i++)
		player_on_turn->get_client()->write("[" + std::to_string(i) + "] : " + player_on_turn->get_hand_cards().get_card_at(i)->get_name() + "(" + player_on_turn->get_hand_cards().get_card_at(i)->color_to_name() + ", " + std::to_string(player_on_turn->get_hand_cards().get_card_at(i)->get_points()) + ") \r\n");

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
		player_on_turn->get_client()->write("[" + std::to_string(i) + "]: " + character_cards.get_card_at(i)->getName() + "\r\n");
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
			player_on_turn->get_client()->write("Dismissed : " + character_cards.get_card_at(i)->getName() + "\r\n");
			character_cards.get_card_and_remove_at_index(0);
		}
		else
			player_on_turn->get_client()->write("[" + std::to_string(i) + "]: " + character_cards.get_card_at(i)->getName() + "\r\n");
	}

	if (character_cards.size() == 0)
	{
		fase = GamePhase::PlayFase;
		call_next_char();
	}
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
}

void GameController::remove_choice(int index)
{
	if (index == 0)
		turn_choices.erase(turn_choices.begin());
	else
		turn_choices.erase(turn_choices.begin() + index + 1);
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

	set_turn_choices();
}

