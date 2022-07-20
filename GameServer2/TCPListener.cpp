#include "pch.h"
#include "TCPListener.h"
#include "IOCP.h"
#include "ConfigManager.h"
#include "SessionManager.h"
#include "TCPSessionPool.h"
#include "TCPSession.h"

TCPListener::TCPListener()
	: m_listenSocket(ConfigManager::GetInst()->GetServerPort(), ConfigManager::GetInst()->GetServerIP())
{
	m_listenCompleteCallback = std::bind(&TCPListener::ListenCompleteCallback, this, std::placeholders::_1, std::placeholders::_2);
}

TCPListener::~TCPListener()
{
	// accpetEx ���ε� �����Ѱ��
	// IOCP work�����尡 ����Ǿ� accept���¿��� ������ �������;��Ѵ�
	// accept���̾��� TCP������ ���⼭ ����
	auto iter = m_acceptingTCPSession.begin();
	while (iter != m_acceptingTCPSession.end())
	{
		// delete�� ȣ���ϴ� ������� �ϳ����̹Ƿ� �������� �ʴ´�
		delete iter->second;
		iter->second = nullptr;

		++iter;
	}
}

void TCPListener::ListenCompleteCallback(DWORD _transferredBytes, IOCompletionData* _IOData)
{	
	// ���޵� Overlapped ����ü�� IOCompletionData�� ĳ���� �� TCPSession ������
	TCPSession& rfTcpSession = _IOData->tcpSession;
	rfTcpSession.SetClientAddress();
	rfTcpSession.RegistIOCP();
	rfTcpSession.RequestRecv();

	// �����ϰ��ִ� ���Ǹʿ��� ������
	m_acceptingTCPSession.erase(&rfTcpSession);

	// ���� �Ŵ����� ���� ����
	SessionManager::GetInst()->AddTCPSession(&rfTcpSession);

	// SessionPool���� TCPSession�� �޾� accept ��û
	TCPSession* pTCPSession = TCPSessionPool::GetInst()->GetTCPSession();
	pTCPSession->RequestAsyncAccept();
	m_acceptingTCPSession.insert({ pTCPSession, pTCPSession });
}

void TCPListener::StartListen()
{
	// backlog�� �ִ밪���� ����
	listen(m_listenSocket.GetSocket(), SOMAXCONN);

	// IOCP�� �������� ���
	IOCP::GetInst()->RegisterSocket(m_listenSocket.GetSocket(), &m_listenCompleteCallback);

	// TCPSession�� ������ acceptEx ȣ��
	// accpetEx�� ȣ���� TCPSession�� TCPListener���� ����
	for (size_t i = 0; i < InitialAcceptSocketCount; i++)
	{
		TCPSession* pTCPSession = TCPSessionPool::GetInst()->GetTCPSession();
		pTCPSession->RequestAsyncAccept();
		m_acceptingTCPSession.insert({ pTCPSession, pTCPSession });
	}

}
