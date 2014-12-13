#include "ServerController.h"

#include <memory>

int main(int argc, const char* argv[])
{
	// Create new server controller at startup
	std::unique_ptr<ServerController> server_controller(new ServerController());
	server_controller->start_server();
	return 0;
}