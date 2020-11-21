#include "SServer.h"

int main()
{
	SServer* server = new SServer();
	server->port = 0;
	server->startServer();
	//SClient* client = new SClient();
	
	return 0;
}