#include "pch.h"
#include "TCPSession.h"
#include "TCPListener.h"
#include "IOCP.h"

TCPSession::TCPSession()
	: m_sessionSocket()
	, m_IOCompletionData(*this)
	, m_IOCompletionCallback(std::bind(&TCPSession::IOCompletionCallback, this, std::placeholders::_1, std::placeholders::_2))
{
}

TCPSession::~TCPSession()
{
}

void TCPSession::IOCompletionCallback(DWORD _transferredBytes, LPOVERLAPPED _IOData)
{
	// Recv ó�� �� �ٽ� Recv ��û



	RequestRecv();
}

bool TCPSession::IsRecycleSession()
{
	// ���Ǽ����� IOCP�� ��� �ƴ��� ���η� ����� �������� �Ǵ�
    return m_sessionSocket.IsRegistIOCP();
}

void TCPSession::RequestAsyncAccept()
{
    DWORD dwByte = 0;
    BOOL result = AcceptEx(TCPListener::GetInst()->GetListenSocket()
        , m_sessionSocket.GetSocket()
        , m_IOCompletionData.buffer
        , 0
        , sizeof(sockaddr_in) + 16
        , sizeof(sockaddr_in) + 16
        , &dwByte
        , &m_IOCompletionData.overlapped);

	if (FALSE == result)
	{
		if (ERROR_IO_PENDING != WSAGetLastError())
		{
			ServerHelper::PrintLastError("AcceptEx Error");
			return;
		}
	}

}

void TCPSession::SetClientAddress()
{
	SOCKADDR_IN* pLocalAddr = nullptr;
	SOCKADDR_IN* pClientAddress = nullptr;

	int localLen = 0;
	int RemoteLen = 0;
	GetAcceptExSockaddrs(m_IOCompletionData.buffer,
		0,
		sizeof(sockaddr_in) + 16,
		sizeof(sockaddr_in) + 16,
		(LPSOCKADDR*)&pLocalAddr,
		&localLen,
		(LPSOCKADDR*)&pClientAddress,
		&RemoteLen);

	m_sessionSocket.SetClientAddress(*pClientAddress);
}

void TCPSession::RegistIOCP()
{
	if (m_sessionSocket.IsRegistIOCP() == false)
	{
		// �ѹ��� ��ϵ��� ���� ���ϸ� ���
		m_sessionSocket.SetRegistIOCP();
		IOCP::GetInst()->RegisterSocket(m_sessionSocket.GetSocket(), &m_IOCompletionCallback);
	}
}

void TCPSession::RequestRecv()
{

}

void TCPSession::RequestSend()
{
}


