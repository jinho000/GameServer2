#include "pch.h"
#include "TestPacket.h"

TestPacket::TestPacket()
	: RecvPacket(PACKET_TYPE::TEST_PACKET)
	, m_test(1)
{
}

void TestPacket::Deserialize()
{
	// IOBuffer�� �����͸� ����
	UINT currPos = 0;
	
}
