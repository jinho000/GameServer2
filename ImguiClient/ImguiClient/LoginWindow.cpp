#include "LoginWindow.h"
#include "imgui.h"
#include "ImguiWindowManager.h"
#include "User.h"
#include "MainWindow.h"

LoginWindow::LoginWindow()
	: m_IDBuffer()
{
}

void LoginWindow::UpdateWindow()
{
	ImGui::Begin("LoginWindow");

	ImGui::Text("ID");
	ImGui::InputText("##ID", m_IDBuffer, 64);
	//ImGui::Text("Password");
	//ImGui::InputText("##Password", buf2, 64);

	if (ImGui::Button("Login", ImVec2(80, 40)))
	{
		// ���� ������ ������ ��Ŷ ����
		// send m_IDBuffer


		// ���� �����쿡 ����
		MainWindow* pMainWindow = static_cast<MainWindow*>(ImguiWindowManager::GetInst()->GetImguiWindow(WINDOW_UI::MAIN));


		// recv
		// ���� ������ �޴´�
		User* pLoginUser = new User;
		

		// ä�� ������ �޴´�


		// �α����� ������ ���
		ImguiWindowManager::GetInst()->ChangeMainWindow(WINDOW_UI::MAIN);
	}

	ImGui::End();
}
