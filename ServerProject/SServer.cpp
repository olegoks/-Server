#include "SServer.h"
#include "includes.h"


SServer::SServer()noexcept(true):
	external_ip_(GetIp()){}


SServer::~SServer()
{
}

void SServer::startServer()
{
	//WSAStartup - activation of windows sockets
	//MAKEWORD - WSA version
	if (WSAStartup(MAKEWORD(2, 2), &wData) == 0)
		printf("WSA started success.\n");
	
	SOCKADDR_IN server_socket_address;
	int address_size = sizeof(server_socket_address);
	server_socket_address.sin_addr.S_un.S_addr = INADDR_ANY;// Сокет доступен с любой пользовательской машины
	server_socket_address.sin_port = htons(port);//Конвертируем номер порта в bigendian
	std::cin.clear();
	server_socket_address.sin_family = AF_INET;//TCP/IP Ipv4

	//SOCKADDR ssdf;
	server_socket_ = socket(AF_INET, SOCK_STREAM, NULL);

	//Значение SOCK_STREAM указывается при создании сокета работающего в режиме соединения, 
	//значение SOCK_DGRAM указывается при создании сокета работающего в датаграммном режиме.
	//Последний параметр - определяет используемый протокол. Обычно этот параметр задается равным нулю, при этом используется протокол 
	//принятый по умолчанию для данного типа сокетов (TCP для сокетов типа SOCK_STREAM и UDP для сокетов типа SOCK_DGRAM).
	
	if (server_socket_ == SOCKET_ERROR)
		printf("Socket not created\n");

	//Связывание сокета с портом и адресом
	if (bind(server_socket_, (struct sockaddr*)&server_socket_address, address_size) != SOCKET_ERROR) {
		printf("Socket succed binded\n");
		getsockname(server_socket_, (struct sockaddr*)&server_socket_address, &address_size);
		getpeername(server_socket_, (struct sockaddr*)&server_socket_address, &address_size);
		this->port = server_socket_address.sin_port;
	}
	
	if (listen(server_socket_, SOMAXCONN) != SOCKET_ERROR)
		printf("Start listenin at port %u\n", ntohs(this->port));
	
	handle();
}

void SServer::closeServer() {
	closesocket(server_socket_);
	WSACleanup();
	std::cout << "Server was stoped. You can close app" << std::endl;
}


void SServer::handle() {

	while (true)
	{
		
		/*
		typedef struct sockaddr_in {// Описывает сокет для работы с протоколами IP
			ADDRESS_FAMILY sin_family; AF_INET
			USHORT sin_port; Номер порта который намерен занять процесс
			IN_ADDR sin_addr; IP адрес к которому будет привязан сокет
			CHAR sin_zero[8]; 

		} SOCKADDR_IN, * PSOCKADDR_IN;
		*/
		// В сети используется big-endian прямой порядок
		// в intel little_endian
		SOCKADDR_IN socket_address;
		int address_size = sizeof(socket_address);
		char buffer[512];
		struct sockaddr_in from_struct;
		int froml = sizeof(from_struct);

		SOCKET connection_socket;
		connection_socket = accept(server_socket_, (struct sockaddr*)&socket_address, &address_size);

		if (connection_socket != INVALID_SOCKET) {

			printf("send\n");
			printf("sended Client connected from 0  %u.%u.%u.%u:%u\n",
				(unsigned char)socket_address.sin_addr.S_un.S_un_b.s_b1,
				(unsigned char)socket_address.sin_addr.S_un.S_un_b.s_b2,
				(unsigned char)socket_address.sin_addr.S_un.S_un_b.s_b3,
				(unsigned char)socket_address.sin_addr.S_un.S_un_b.s_b4,
				ntohs(socket_address.sin_port));

			SClient* client = new SClient(connection_socket, socket_address);

		}

		Sleep(200);
	}
}