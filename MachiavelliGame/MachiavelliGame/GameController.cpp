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
		if (fase == GameFase::ChooseChar)
		{
			hanlde_choose_char_command(new_command);
		}
		else if (fase == GameFase::DismissChar)
		{
			handle_dismiss_char_command(new_command);
		}
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
	//int choice = std::atoi(new_command.c_str());
	bool is_command_digit = false;
	int choice;

	while (!is_command_digit)
	{
		choice = atoi(new_command.c_str());
		if (choice > 0 && (choice < character_cards.size()))
			is_command_digit = true;
		else
		{
			player_on_turn->get_client()->write("Invalid text, please fill in valid text to choose a character \r\n");
			return;
		}
	}

	player_on_turn->add_character(character_cards.get_card_and_remove_at_index(choice));

	if (first_pick)
	{
		first_pick = false;
		set_turn_to_next_player();
		choose_character();
	}
	else
	{
		dismiss_character();
		fase = GameFase::DismissChar;
	}
}

void GameController::handle_dismiss_char_command(std::string new_command)
{
	int choice = std::atoi(new_command.c_str());
	character_cards.get_card_and_remove_at_index(choice);
	set_turn_to_next_player();

	if (character_cards.size() == 0)
	{
		fase = GameFase::PlayFase;
		call_next_char();
	}
	else
	{
		fase = GameFase::ChooseChar;
		choose_character();
	}
}

void GameController::call_next_char()
{
	for (int i = 0; i < players.size(); i++){
		if (players[i]->has_character(char_order[call_count])){
			player_on_turn = players[i];
			print_turn_info();
			call_count++;
			return;
		}
	}
}

void GameController::print_turn_info()
{
	player_on_turn->get_client()->write("As of now you're the " + char_order[call_count] + "\r\n");
	player_on_turn->get_client()->write("Gold: " + std::to_string(player_on_turn->get_gold()) + "\r\n\r\n");
	player_on_turn->get_client()->write("Buildings: \r\n");
	for (int i = 0; i < player_on_turn->get_field_cards().size(); i++){
		std::shared_ptr<BuildingCard> card = player_on_turn->get_field_cards().get_card_at(i);
		player_on_turn->get_client()->write(card->get_name() + "(" + card->color_to_name() + ", " + std::to_string(card->get_points()) + ") \r\n");
	}
	player_on_turn->get_client()->write("\r\n");
	player_on_turn->get_client()->write("Hand: \r\n");
	for (int i = 0; i < player_on_turn->get_hand_cards().size(); i++){
		std::shared_ptr<BuildingCard> card = player_on_turn->get_hand_cards().get_card_at(i);
		player_on_turn->get_client()->write(card->get_name() + "(" + card->color_to_name() + ", " + std::to_string(card->get_points()) + ") \r\n");
	}
	player_on_turn->get_client()->write("\r\n");
	player_on_turn->get_client()->write("What to do? \r\n");
}

void GameController::set_turn_to_next_player()
{
	for (int i = 0; i < players.size(); i++)
	{
		if (players[i] != player_on_turn)
		{
			player_on_turn = players[i];
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
	fase = GameFase::ChooseChar;
	first_pick = true;

	choose_character();
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
			player_on_turn->get_client()->write("Dismissed : " + character_cards.get_card_at(i)->getName());
		else
			player_on_turn->get_client()->write("[" + std::to_string(i) + "]: " + character_cards.get_card_at(i)->getName() + "\r\n");
	}
}

void GameController::show_help_text(std::shared_ptr<Socket> client)
{
	client->write("\r\nVoorkant : \r\n");
	client->write("Inkomsten -> Neem 2 goudstukken of neem 2 kaarten en leg er 1 af \r\n");
	client->write("Bouwen -> Leg 1 bouwkaart neer en betaal de waarde");
	client->write("Karaktereigenschap op elk moment te gebruiken \r\n");
	client->write("1. Moordernaar -> Vermoord een ander karakter \r\n");
	client->write("2. Dief -> Steel van een andere speler \r\n");
	client->write("3. Magier -> Ruilt bouwkaarten om \r\n");
	client->write("4. Koning -> Begint volgende beurt, ontvangt van monumenten \r\n");
	client->write("5. Prediker -> Beschermd tegen Condotierre & ontvangt van kerkelijke gebouwen \r\n");
	client->write("6. Koopman -> Ontvangt een extra goudstuk & ontvangt van commerciele gebouwen \r\n");
	client->write("7. Bouwmeester -> trekt twee extra kaarten & mag drie gebouwen bouwen \r\n");
	client->write("8. Condottiere -> Vernietigt een gebouw & ontvangt van alle militaire gebouwen \r\n");
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
}

