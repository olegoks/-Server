
#pragma once
#ifndef INCLUDES_H_
#define INCLUDES_H_
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)
#include <iostream>
#include <WinSock2.h>
#include <winsock.h>
#include "SClient.h"
#include <string>
#include <locale>
#include <sstream>

struct IPv4 {
    union {
        struct { UCHAR s_b1, s_b2, s_b3, s_b4; } S_un_b;
        struct { USHORT s_w1, s_w2; } S_un_w;
        ULONG S_addr;
        UCHAR octets[4];
    } S_un;
};

IPv4 GetIp();

#endif //INCLUDES_H_