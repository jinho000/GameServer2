#include "pch.h"
#include "TCPSessionPool.h"
#include "TCPSession.h"

TCPSessionPool::TCPSessionPool()
{
    CreateTCPSession(INITIAL_SESSION_COUNT);
}

TCPSessionPool::~TCPSessionPool()
{
    while (m_TCPSessionPool.empty() == false)
    {
        // delete�� ȣ���ϴ� ������� �ϳ����̹Ƿ� �������� �ʴ´�
        TCPSession* pTCPSession = m_TCPSessionPool.front(); m_TCPSessionPool.pop();
        delete pTCPSession;
    }
}

void TCPSessionPool::CreateTCPSession(int _TCPSessionCount)
{
    m_lock.lock();
    for (size_t i = 0; i < _TCPSessionCount; i++)
    {
        TCPSession* pTCPSession = new TCPSession;
        m_TCPSessionPool.push(pTCPSession);
    }
    m_lock.unlock();
}

TCPSession* TCPSessionPool::GetTCPSession()
{
    if (m_TCPSessionPool.empty())
    {
        CreateTCPSession(RECREATE_SESSION_COUNT);
    }

    TCPSession* pTCPSession = m_TCPSessionPool.front();

    m_lock.lock();
    m_TCPSessionPool.pop();
    m_lock.unlock();

    return pTCPSession;
}

void TCPSessionPool::RetrieveTCPSession(TCPSession* _pTCPSession)
{
    // Ŭ���̾�Ʈ�� ������ ������ �� �ٽ� ��Ȱ���ϴ� TCP���Ǹ� �ִ´�
    assert(_pTCPSession->IsRecycleSession() == true);

    // accpet ��û �� ���� Ǯ�� �ִ´�
    m_lock.lock();
    m_TCPSessionPool.push(_pTCPSession);
    m_lock.unlock();
}
