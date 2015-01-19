#include "ServerController.h"
#include "ServerSocket.h"
#include "Sync_Queue.h"

#include <cctype>
#include <string>

static Sync_Queue<ClientCommand> client_queue;

ServerController::ServerController()
{
	std::shared_ptr<GameController> temp_controller(new GameController());
	game_controller = temp_controller;
}

void ServerController::start_server()
{
	is_server_running = true;

	// Create thread for consuming process
	std::thread consumer_thread(&ServerController::consume_command, this);
	consumer_thread.detach();

	//Create server socket
	ServerSocket server_socket(port_number);

	while (is_server_running)
	{
		try
		{
			std::cerr << "Server listening" << "\n";
			std::shared_ptr<Socket> socket_client = nullptr;

			while ((socket_client = server_socket.accept()) != nullptr)
			{
				std::thread handler_thread(&ServerController::handle_client, this, socket_client);
				handler_thread.detach();
				std::cerr << "Server listening again" << "\n";
			}
		}
		catch (const std::exception& ex)
		{
			std::cerr << ex.what() << ", resuming..." << "\n";
		}
	}
}

void ServerController::queue_put(ClientCommand new_command)
{
	client_queue.put(new_command);
}

void ServerController::handle_client(std::shared_ptr<Socket> socket)
{
	std::shared_ptr<Socket> client = socket;
	startup_player(client);
	
	is_handling = true;

	while (is_handling)
	{
		try
		{
			std::string command = client->read_line();
			ClientCommand client_command = ClientCommand(command, client);
			client_queue.put(client_command);
		}
		catch (const std::exception& ex)
		{
			std::string exception_string = std::string("ERROR: ") + ex.what() + "\n";
			client->write(exception_string);
		}
		catch (...)
		{
			client->write("ERROR: something went wrong");
		}
	}
}

void ServerController::startup_player(std::shared_ptr<Socket> client)
{
	// Set info to client
	client->write("Welcome to the Machiavelli Game Server \r\n");
	client->write("Please fill in your name to start \r\n");
	client->write(prompt);
	std::string name = client->read_line();
	std::string age;
	bool is_age_digit = false;	
	while (!is_age_digit)
	{
		client->write("Please fill in your age \r\n");
		client->write(prompt);
		age = client->read_line();
		int charact = atoi(age.c_str());
		if (charact != 0)
			is_age_digit = true;
	}
	client->write("You are ready to play! \r\n");
	// Get id from the client and save for the player
	game_controller->connect_player(client, name, age);
}

void ServerController::consume_command()
{
	is_consuming = true;

	while (is_consuming)
	{
		ClientCommand command;
		client_queue.get(command);
		std::shared_ptr<Socket> client = command.get_client();
		if (client)
		{
			game_controller->consume_command(command, client);
		}
		else
			std::cerr << "Client has disappeared" << "\n";
	}
}

ServerController::~ServerController()
{
}
