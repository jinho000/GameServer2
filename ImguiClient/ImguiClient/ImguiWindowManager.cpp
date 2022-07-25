#include "ImguiWindowManager.h"
#include "LoginWindow.h"
#include "MainWindow.h"

ImguiWindowManager* ImguiWindowManager::pInst = nullptr;

ImguiWindowManager::ImguiWindowManager()
	: m_imguiWindow()
	, m_currentUI(WINDOW_UI::LOGIN)
{
	// �α���â, ����â �߰�
	m_imguiWindow[static_cast<int>(WINDOW_UI::LOGIN)] = new LoginWindow;
	m_imguiWindow[static_cast<int>(WINDOW_UI::MAIN)] = new MainWindow;
}

ImguiWindowManager::~ImguiWindowManager()
{
	for (size_t i = 0; i < static_cast<int>(WINDOW_UI::MAIN) + 1; i++)
	{
		delete m_imguiWindow[i];
		m_imguiWindow[i] = nullptr;
	}
}

void ImguiWindowManager::CreateIntance()
{
	if (pInst != nullptr)
		return;

	pInst = new ImguiWindowManager;
}

void ImguiWindowManager::Destroy()
{
	if (pInst != nullptr)
	{
		delete pInst;
		pInst = nullptr;
	}
}

void ImguiWindowManager::UpdateWindow()
{
	m_imguiWindow[static_cast<int>(m_currentUI)]->UpdateWindow();
}

void ImguiWindowManager::ChangeMainWindow(WINDOW_UI _window)
{
	// UI ����� ����?
	// UI ���۽� ����?
	m_currentUI = _window;
}

