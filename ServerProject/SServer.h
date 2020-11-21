#pragma once
#include "includes.h"
class SServer
{
public:
	explicit SServer()noexcept(true);
	~SServer();
	void startServer();
	void closeServer();
	void handle();
	unsigned short port;
private:
	SOCKET server_socket_;
	WSAData wData;
	IPv4 external_ip_;
};

