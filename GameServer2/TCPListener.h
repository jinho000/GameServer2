#pragma once
#include "Singleton.h"
#include "ListenSocket.h"

class TCPSession;
class TCPListener : public Singleton<TCPListener>
{
	friend class Singleton;

private: // member var
	ListenSocket			m_listenSocket;
	IOCompletionCallback	m_listenCompleteCallback;

	std::unordered_map<TCPSession*, TCPSession*> m_acceptingTCPSession;

	enum
	{
		InitialAcceptSocketCount = 10,
	};

private: 
	TCPListener();
	~TCPListener();

private:
	void ListenCompleteCallback(DWORD _transferredBytes, IOCompletionData* _ioData);

public:
	SOCKET GetListenSocket() { return m_listenSocket.GetSocket(); }

	void StartListen();

};

