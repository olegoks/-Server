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
	server_socket_address.sin_addr.S_un.S_addr = INADDR_ANY;// ����� �������� � ����� ���������������� ������
	server_socket_address.sin_port = htons(port);//������������ ����� ����� � bigendian
	std::cin.clear();
	server_socket_address.sin_family = AF_INET;//TCP/IP Ipv4

	//SOCKADDR ssdf;
	server_socket_ = socket(AF_INET, SOCK_STREAM, NULL);

	//�������� SOCK_STREAM ����������� ��� �������� ������ ����������� � ������ ����������, 
	//�������� SOCK_DGRAM ����������� ��� �������� ������ ����������� � ������������ ������.
	//��������� �������� - ���������� ������������ ��������. ������ ���� �������� �������� ������ ����, ��� ���� ������������ �������� 
	//�������� �� ��������� ��� ������� ���� ������� (TCP ��� ������� ���� SOCK_STREAM � UDP ��� ������� ���� SOCK_DGRAM).
	
	if (server_socket_ == SOCKET_ERROR)
		printf("Socket not created\n");

	//���������� ������ � ������ � �������
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
		typedef struct sockaddr_in {// ��������� ����� ��� ������ � ����������� IP
			ADDRESS_FAMILY sin_family; AF_INET
			USHORT sin_port; ����� ����� ������� ������� ������ �������
			IN_ADDR sin_addr; IP ����� � �������� ����� �������� �����
			CHAR sin_zero[8]; 

		} SOCKADDR_IN, * PSOCKADDR_IN;
		*/
		// � ���� ������������ big-endian ������ �������
		// � intel little_endian
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