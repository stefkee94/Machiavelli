#include "ServerController.h"
#include "ServerSocket.h"

static Sync_Queue<ClientCommand> client_queue;

ServerController::ServerController()
{
	game_controller = GameController();
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
	client->write("Welcome to the Machiavelli Game Server");
	client->write(prompt);
	
	game_controller.start_client_game(std::shared_ptr<ServerController>(this), client);
}

void ServerController::consume_command()
{
	while (true)
	{
		ClientCommand command;
		client_queue.get(command);
		std::shared_ptr<Socket> client = command.get_client();
		if (client)
		{
			try
			{
				client->write("Hey you wrote: ");
				client->write(command.get_command());
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

			client->write(prompt);
		}
		else
			std::cerr << "Client has disappeared" << "\n";
	}
}

ServerController::~ServerController()
{
}
