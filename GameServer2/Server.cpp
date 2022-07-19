#include "pch.h"
#include "Server.h"
#include "IOCP.h"
#include "SessionManager.h"
#include "ConfigManager.h"
#include "ListenSocket.h"
#include "TCPListener.h"
#include "TCPSessionPool.h"

// �������� ����ü
struct SOCKETINFO
{
    OVERLAPPED	overlapped;
    SOCKET		socket;
    char		buffer[255];
    int			bufferSize;
    WSABUF		wsabuf;
};

void IOCallback(DWORD transferredBytes, LPOVERLAPPED IOData)
{
    SOCKETINFO* clientInfo = reinterpret_cast<SOCKETINFO*>(IOData);
    clientInfo->buffer[transferredBytes] = '\0';
    std::string recvData(clientInfo->buffer);

    std::cout << recvData << std::endl;


    // �񵿱� ����� ����
    DWORD recvbytes = 0;
    DWORD flags = 0;
    int result = WSARecv(clientInfo->socket, &clientInfo->wsabuf, 1, &recvbytes, &flags, &clientInfo->overlapped, NULL);
    if (result == SOCKET_ERROR)
    {
        if (WSAGetLastError() == ERROR_IO_PENDING)
        {
            return;
        }

        assert(nullptr);
    }
}

IOCompletionCallback ioCallback = IOCallback;
const int SERVER_PORT = 9900;

//void Server::StartServer()
//{
//    ServerHelper::WSAStart();
//    IOCP::CreateInstance(10);
//
//    ListenSocket listenSock(9900, "127.0.0.1");
//    listenSock.StartListen();
//
//    // accpet
//    while (true)
//    {
//        SOCKADDR_IN clientAddr = {};
//        int len = sizeof(clientAddr);
//        SOCKET sessionSocket = accept(listenSock.GetSocket(), (sockaddr*)&clientAddr, &len);
//
//        // IOCP�� ���� ����
//        IOCP::GetInst()->RegisterSocket(sessionSocket, &ioCallback);
//
//        // Ŭ���̾�Ʈ�� ���� �� ���� ���
//        char buff[255];
//        inet_ntop(AF_INET, &clientAddr.sin_addr, buff, sizeof(buff));
//        std::string ip(buff);
//        std::string portNum(std::to_string(ntohs(clientAddr.sin_port)));
//        std::cout << "IP " << ip << " Port: " << portNum << std::endl;
//
//
//        // ���� ���� ����ü �Ҵ�
//        SOCKETINFO* clientInfo = new SOCKETINFO;
//        clientInfo->socket = sessionSocket;
//        clientInfo->bufferSize = 255;
//        clientInfo->wsabuf.buf = clientInfo->buffer;
//        clientInfo->wsabuf.len = clientInfo->bufferSize;
//        ZeroMemory(&clientInfo->overlapped, sizeof(clientInfo->overlapped));
//
//
//        // �񵿱� ����� ����
//        DWORD recvbytes = 0;
//        DWORD flags = 0;
//        int result = WSARecv(clientInfo->socket, &clientInfo->wsabuf, 1, &recvbytes, &flags, &clientInfo->overlapped, NULL);
//        if (result == SOCKET_ERROR)
//        {
//            if (WSAGetLastError() == ERROR_IO_PENDING)
//            {
//                continue;
//            }
//
//            assert(nullptr);
//        }
//    }
//
//
//    IOCP::Destroy();
//    ServerHelper::WSAEnd();
//}


void Server::StartServer()
{
    ServerHelper::WSAStart();

    IOCP::CreateInstance();
    SessionManager::CreateInstance();
    TCPSessionPool::CreateInstance();

    // config ���� �� TCPListener ����
    ConfigManager::CreateInstance();
    ConfigManager::GetInst()->LoadConfig();

    TCPListener::CreateInstance();

    {
        // ���� ���� ����
        TCPListener::GetInst()->StartListen();

        while (true)
        {
            std::string wait;
            std::cin >> wait;

            if (wait == "quit")
            {
                break;
            }
        }
    }
    
    IOCP::Destroy();

    TCPSessionPool::Destroy();
    TCPListener::Destroy();
    SessionManager::Destroy();

    ConfigManager::Destroy();


    ServerHelper::WSAEnd();
}