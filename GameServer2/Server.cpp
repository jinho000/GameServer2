#include "pch.h"
#include "Server.h"
#include "IOCP.h"
#include "SessionManager.h"
#include "ConfigManager.h"
#include "ListenSocket.h"
#include "TCPListener.h"
#include "TCPSessionPool.h"
#include "PacketHandler.h"  

void Server::StartServer()
{
    ServerHelper::WSAStart();

    // ������ config ���� �� �ٸ� �Ŵ��� ����
    ConfigManager::CreateInstance();
    ConfigManager::GetInst()->LoadConfig();

    IOCP::CreateInstance();
    TCPListener::CreateInstance();
    SessionManager::CreateInstance();
    PacketHandler::CreateInstance();

    {
        // ���� ���� ����
        TCPListener::GetInst()->StartListen();
        TCPSessionPool::CreateInstance();

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

    PacketHandler::Destroy();
    TCPSessionPool::Destroy();
    SessionManager::Destroy();
    TCPListener::Destroy();

    ConfigManager::Destroy();


    ServerHelper::WSAEnd();
}