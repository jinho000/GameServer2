#pragma once
#include "SessionSocket.h"

class TCPSession
{
private: // member var
	SessionSocket			m_sessionSocket;
	IOCompletionData		m_IOCompletionAccept; 
	IOCompletionData		m_IOCompletionDisconnect; 
	IOCompletionData		m_IOCompletionRecv; 
	IOCompletionData		m_IOCompletionSend; 

	IOCompletionCallback	m_IOCompletionCallback;
	
	std::vector<uint8_t>	m_recvBuffer;

public: // default
	TCPSession();
	~TCPSession();

	TCPSession(const TCPSession& _other) = delete;
	TCPSession(TCPSession&& _other) = delete;
	TCPSession& operator=(const TCPSession& _other) = delete;
	TCPSession& operator=(const TCPSession&& _other) = delete;

private:
	void IOCompletionCallback(DWORD _transferredBytes, IOCompletionData* _IOData);
	

public: // member Func
	bool IsRecycleSession();

	void RequestAsyncAccept();
	void SetClientAddress();
	void RegistIOCP();
	void RequestRecv();
	void RequestSend(const std::vector<uint8_t>& _buffer);
};
