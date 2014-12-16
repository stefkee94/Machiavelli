#include "GameController.h"
#include "ServerController.h"

GameController::GameController()
{
}

void GameController::start_client_game(std::shared_ptr<ServerController> server_controller, std::shared_ptr<Socket> new_client)
{
	while (true)
	{
		try
		{
			std::string command = new_client->read_line();
			std::cerr << "client (" << new_client->get() << ") said: " << command << "\n";

			if (command == "quit")
			{
				new_client->write("LATER!");
				break;
			}

			ClientCommand client_command = ClientCommand(command, new_client);
			server_controller->queue_put(client_command);

		}
		catch (const std::exception& ex)
		{
			std::string exception_string = std::string("ERROR: ") + ex.what() + "\n";
			new_client->write(exception_string);
		}
		catch (...)
		{
			new_client->write("ERROR: something went wrong");
		}
	}
}

GameController::~GameController()
{
}
