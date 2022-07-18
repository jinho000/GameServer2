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
	// accpetEx ���ε� �����Ѱ��?
	while (m_acceptingTCPSession.empty() == false)
	{
		// delete�� ȣ���ϴ� ������� �ϳ����̹Ƿ� �������� �ʴ´�
		TCPSession* pSessionSocket = m_acceptingTCPSession.front(); m_acceptingTCPSession.pop_front();
		delete pSessionSocket;
		pSessionSocket = nullptr;
	}
}

void TCPListener::ListenCompleteCallback(DWORD _transferredBytes, LPOVERLAPPED _IOData)
{	
	// ����
	// Completion Port�� AcceptEx�� ������ ����ص�ä �����ϸ� 995 ����
	
	// ���޵� Overlapped ����ü�� IOCompletionData�� ĳ���� �� TCPSession ������
	TCPSession& rfTcpSession = reinterpret_cast<IOCompletionData*> (_IOData)->tcpSession;
	rfTcpSession.SetClientAddress();
	rfTcpSession.RegistIOCP();
	rfTcpSession.RequestRecv();

	// �����ʿ��� ������ 
	auto iter = m_acceptingTCPSession.begin();
	while (iter != m_acceptingTCPSession.end())
	{
		if (*iter == &rfTcpSession)
		{
			m_acceptingTCPSession.erase(iter);
			break;
		}

		++iter;
	}

	// ���� �Ŵ����� ���� ����
	SessionManager::GetInst()->AddTCPSession(&rfTcpSession);


	// SessionPool���� TCPSession�� �޾� accept ��û
	TCPSession* pTCPSession = TCPSessionPool::GetInst()->GetTCPSession();
	pTCPSession->RequestAsyncAccept();
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
		m_acceptingTCPSession.push_back(pTCPSession);
	}

}
