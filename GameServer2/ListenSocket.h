#pragma once
#include "Socket.h"


// �뵵 :
// �з� :
// ÷�� :
class ListenSocket : public Socket
{
private: // member var
	

public: // default
	ListenSocket(int _serverPort, std::string _serverIP);
	~ListenSocket() = default;

private:
	
public:
	int GetServerPort() const { return m_port; }
	std::string GetServerIP() const { return m_IP; }

public:
	void StartListen();
};

