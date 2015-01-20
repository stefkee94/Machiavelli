#include <memory>
#include "ServerController.h"
#include "CardStack.h"
#include "MachiavelliReader.h"
#include "Time.h"
int main(int argc, const char* argv[])
{
	// Create new server controller at startup
	srand(time(NULL));
	std::unique_ptr<ServerController> server_controller(new ServerController());
	server_controller->start_server();
	return 0;
}