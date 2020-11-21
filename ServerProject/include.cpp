#include "includes.h"

IPv4 GetIp() {

    std::string website_HTML;
    std::locale local;

    char buffer[200] = { 0 };

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData))exit(EXIT_FAILURE);

    SOCKET Socket;
    Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct hostent* host;
    static const std::string url = "api.ipify.org";
    host = gethostbyname(url.c_str());

    SOCKADDR_IN SockAddr;
    SockAddr.sin_port = htons(80);
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

    if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0) exit(EXIT_FAILURE);

    std::string get_http;
    get_http = "GET / HTTP/1.1\r\nHost: " + url + "\r\nConnection: close\r\n\r\n";
    send(Socket, get_http.c_str(), strlen(get_http.c_str()), 0);

    int nDataLength;
    nDataLength = recv(Socket, buffer, 200, 0);
    website_HTML = buffer;
    std::string ip_address = website_HTML.substr(website_HTML.length() - 16, 16);

    closesocket(Socket);
    WSACleanup();
    IPv4 ipv4;
    for (size_t i = 0; i < 3; i++) {

        size_t pos = ip_address.find('.');
        std::stringstream convert{ ip_address.substr(std::ios::beg, pos + 1) };
        ip_address.erase(std::ios::beg, pos + 1);
        convert >> ipv4.S_un.octets[i];
    }

    return ipv4;
}