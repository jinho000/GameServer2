#pragma once
#include "Socket.h"
class SessionSocket : public Socket
{
private:
	bool m_bUsed; // ��Ȱ��� ���� ����

public:
	SessionSocket();
	~SessionSocket() = default;

public:
	void SetClientAddress(SOCKADDR_IN _address);
	void SetUsed() { m_bUsed = true; }

	int GetClientPort() const { return m_port; }
	std::string GetClientIP() const { return m_IP; }
	bool IsUsed() { return m_bUsed; }

};

