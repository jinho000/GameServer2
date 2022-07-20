#include "pch.h"
#include "Server.h"
#include "IOCP.h"
#include "SessionManager.h"
#include "ConfigManager.h"
#include "ListenSocket.h"
#include "TCPListener.h"
#include "TCPSessionPool.h"

void Server::StartServer()
{
    ServerHelper::WSAStart();

    // ������ config ���� �� �ٸ� �Ŵ��� ����
    ConfigManager::CreateInstance();
    ConfigManager::GetInst()->LoadConfig();

    IOCP::CreateInstance();
    TCPListener::CreateInstance();
    SessionManager::CreateInstance();

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

    TCPSessionPool::Destroy();
    SessionManager::Destroy();
    TCPListener::Destroy();

    ConfigManager::Destroy();


    ServerHelper::WSAEnd();
}