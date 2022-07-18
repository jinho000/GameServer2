#pragma once
#include "Singleton.h"

// �������� �о�� �����ϴ� Ŭ����
// ������ �������� �ٸ� Ŭ�������� ������ ���
class ConfigManager : public Singleton<ConfigManager>
{
private: // member var
	friend class Singleton;

public:
	void LoadConfig();


	// Get Config
};

