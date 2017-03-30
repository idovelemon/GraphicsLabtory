//-----------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 09 / 04
// Brief: Define some module used to comminucate
//-----------------------------------------------------
#include "comm.h"

#include <assert.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

namespace comm {

//-----------------------------------------------------
// Implementation Define
//-----------------------------------------------------
class CommImp;
static CommImp* s_CommImp = NULL;

//-----------------------------------------------------
// Class/Typedef
//-----------------------------------------------------

//-----------------------------------------------------
// Class CommImp
//-----------------------------------------------------
class CommImp {
public:
    CommImp();
    virtual ~CommImp();

public:
    void Initialize(int16_t accept_port, int16_t post_port, bool listen);
    void Terminate();
    
    void Send(Packet* packet);
    Packet* Recieve();

private:
    void PostConnection(int16_t post_port);
    void AcceptConnection(int16_t accept_port);

private:
    SOCKET m_ListenSocket;
    SOCKET m_SendSocket;
    SOCKET m_RecvSocket;
};

//-----------------------------------------------------
// Implementation
//-----------------------------------------------------

//-----------------------------------------------------
// Implement CommImp
//-----------------------------------------------------
CommImp::CommImp()
: m_ListenSocket(INVALID_SOCKET)
, m_SendSocket(INVALID_SOCKET)
, m_RecvSocket(INVALID_SOCKET) {
}

CommImp::~CommImp() {
    Terminate();
}

void CommImp::Initialize(int16_t accept_port, int16_t post_port, bool listen) {
    WORD version = MAKEWORD(2,2);
    WSADATA data;
    if (!WSAStartup(version, &data)) {
        if (listen == true) {
            AcceptConnection(accept_port);
            PostConnection(post_port);
        } else {
            PostConnection(post_port);
            AcceptConnection(accept_port);
        }
    } else {
        assert(false);  // Initialize socket lib failed
    }
}

void CommImp::Terminate() {
    if (m_SendSocket != INVALID_SOCKET) {
        closesocket(m_SendSocket);
        m_SendSocket = INVALID_SOCKET;
    }

    if (m_RecvSocket != INVALID_SOCKET) {
        closesocket(m_RecvSocket);
        m_RecvSocket = INVALID_SOCKET;
    }

    if (m_ListenSocket != INVALID_SOCKET) {
        closesocket(m_ListenSocket);
        m_ListenSocket = INVALID_SOCKET;
    }
}

void CommImp::Send(Packet* packet) {
    if (packet != NULL) {
        int32_t header_len = sizeof(Packet) - sizeof(void*);
        int32_t total_len = header_len + packet->size_in_bytes;
        int8_t* buffer = new int8_t[total_len];
        memset(buffer, 0, total_len);
        memcpy(buffer, packet, header_len);
        memcpy(buffer + header_len, packet->data, packet->size_in_bytes);
        send(m_SendSocket, reinterpret_cast<char*>(buffer), total_len, 0);
        delete[] buffer;
        buffer = NULL;
    }
}

Packet* CommImp::Recieve() {
    Packet* packet = NULL;

    int8_t buffer[1024 * 256];
    int32_t len = recv(m_RecvSocket, reinterpret_cast<char*>(buffer), sizeof(buffer), 0);
    if (len > 0) {
        packet = new Packet;
        packet->type = reinterpret_cast<int32_t*>(buffer)[0];
        packet->size_in_bytes = reinterpret_cast<int32_t*>(buffer)[1];
        packet->data = new int8_t[packet->size_in_bytes];
        memcpy(packet->data, buffer + sizeof(Packet) - sizeof(void*), packet->size_in_bytes);
    }

    return packet;
}

void CommImp::PostConnection(int16_t post_port) {
    m_SendSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_SendSocket != INVALID_SOCKET) {
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(post_port);
        addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
        while (true) {
            if (!connect(m_SendSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr))) {
                OutputDebugStringA("Connect Editor");
                break;
            }
        }
    }
}

void CommImp::AcceptConnection(int16_t port) {
    m_ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_ListenSocket != INVALID_SOCKET) {
        sockaddr_in addr;
        int32_t addr_len = sizeof(sockaddr);
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
        if (!bind(m_ListenSocket, reinterpret_cast<sockaddr*>(&addr), addr_len)) {
            listen(m_ListenSocket, 5);
            m_RecvSocket = accept(m_ListenSocket, reinterpret_cast<sockaddr*>(&addr), &addr_len);
            assert(m_RecvSocket != INVALID_SOCKET);
            OutputDebugStringA("Editor has connected");
        } else {
            assert(false);
        }
    }
}

//-----------------------------------------------------
// Implement Comm
//-----------------------------------------------------
void Comm::Initialize(int16_t accept_port, int16_t post_port, bool listen) {
    if (s_CommImp == NULL) {
        s_CommImp = new CommImp;
        if (s_CommImp != NULL) {
            s_CommImp->Initialize(accept_port, post_port, listen);
        } else {
            assert(false);
        }
    }
}

void Comm::Terminate() {
    if (s_CommImp != NULL) {
        s_CommImp->Terminate();
        delete s_CommImp;
        s_CommImp = NULL;
    }
}

void Comm::Send(Packet* packet) {
    if (s_CommImp != NULL) {
        s_CommImp->Send(packet);
    } else {
        assert(false);
    }
}

Packet* Comm::Recieve() {
    Packet* packet = NULL;

    if (s_CommImp != NULL) {
        packet = s_CommImp->Recieve();
    } else {
        assert(false);
    }

    return packet;
}

};  // namespace comm