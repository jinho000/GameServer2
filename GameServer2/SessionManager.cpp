#include "pch.h"
#include "SessionManager.h"
#include "TCPSession.h"

SessionManager::SessionManager()
{
}

SessionManager::~SessionManager()
{
    // ������ Ŭ���̾�Ʈ�� ����Ǿ��ִµ� �����Ѱ��?
    while (m_connectedTCPSession.empty() == false)
    {
        // delete�� ȣ���ϴ� ������� �ϳ����̹Ƿ� �������� �ʴ´�
        TCPSession* pTCPSession = m_connectedTCPSession.front(); m_connectedTCPSession.pop_front();
        delete pTCPSession;
        pTCPSession = nullptr;
    }
}

void SessionManager::AddTCPSession(TCPSession* _pTCPSession)
{
    m_lock.lock();
    m_connectedTCPSession.push_back(_pTCPSession);
    m_lock.unlock();
}

