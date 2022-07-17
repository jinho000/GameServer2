#include "pch.h"
#include "SessionManager.h"
#include "SessionSocket.h"

SessionManager::SessionManager()
{
}

SessionManager::~SessionManager()
{
    while (m_socketList.empty() == false)
    {
        // delete�� ȣ���ϴ� ������� �ϳ����̹Ƿ� �������� �ʴ´�
        SessionSocket* pSessionSocket = m_socketList.front(); m_socketList.pop_front();
        delete pSessionSocket;
    }
}

void SessionManager::AddSessionSocket(SessionSocket* _pSessionSocket)
{
    m_lock.lock();
    m_socketList.push_back(_pSessionSocket);
    m_lock.unlock();
}
