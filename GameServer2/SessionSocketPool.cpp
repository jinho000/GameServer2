#include "pch.h"
#include "SessionSocketPool.h"
#include "SessionSocket.h"

SessionSocketPool* SessionSocketPool::pInstance = nullptr;

SessionSocketPool::SessionSocketPool(int _initialSocketCount)
{
    CreateSessionSocket(_initialSocketCount);
}

SessionSocketPool::~SessionSocketPool()
{
    while (m_sessionSocketPool.empty() == false)
    {
        // delete�� ȣ���ϴ� ������� �ϳ����̹Ƿ� �������� �ʴ´�
        SessionSocket* pSessionSocket = m_sessionSocketPool.front(); m_sessionSocketPool.pop();
        delete pSessionSocket;
    }
}

void SessionSocketPool::CreateInstance(int _initialSocketCount)
{
    if (pInstance != nullptr)
        return;

    pInstance = new SessionSocketPool(_initialSocketCount);
}

void SessionSocketPool::Destroy()
{
    if (pInstance != nullptr)
    {
        delete pInstance;
        pInstance = nullptr;
    }
}

SessionSocketPool* SessionSocketPool::GetInst()
{
    return pInstance;
}

void SessionSocketPool::CreateSessionSocket(int _count)
{
    m_lock.lock();
    for (size_t i = 0; i < _count; i++)
    {
        m_sessionSocketPool.push(new SessionSocket);
    }
    m_lock.unlock();
}

SessionSocket* SessionSocketPool::GetSessionSocket()
{
    if (m_sessionSocketPool.empty())
    {
        CreateSessionSocket(RECREATE_SOCKET_COUNT);
    }
    SessionSocket* pSessionSocket = m_sessionSocketPool.front(); 

    m_lock.lock();
    m_sessionSocketPool.pop();
    m_lock.unlock();

    return pSessionSocket;
}

void SessionSocketPool::RetrieveSessionSocket(SessionSocket* _sessionSocket)
{
    // �̹� �ѹ� ���� ���ϸ� ȸ��
    assert(_sessionSocket->IsUsed() == true);

    _sessionSocket->SetUsed();

    m_lock.lock();
    m_sessionSocketPool.push(_sessionSocket);
    m_lock.unlock();
}
