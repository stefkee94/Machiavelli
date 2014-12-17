#pragma once
#include <thread>
#include <memory>
#include <iostream>

#include "ClientCommand.h"
#include "Socket.h"
#include "GameController.h"

class ServerSocket;
class ServerController
{
public:
	ServerController();
	virtual ~ServerController();

	void consume_command();
	void start_server();

	void handle_client(std::shared_ptr<Socket> socket);
	void queue_put(ClientCommand new_command);

private:
	bool is_server_running;
	bool is_consuming;
	bool is_handling;

	std::shared_ptr<GameController> game_controller;

	// Socket info
	const int port_number = 1080;
	const std::string prompt = "> ";

	
};

