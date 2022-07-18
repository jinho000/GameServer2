#include "pch.h"
#include "IOCP.h"
#include "TCPListener.h"
#include "SessionManager.h"

IOCP* IOCP::pInstance = nullptr;

IOCP::IOCP()
	: m_threadCount(0)
	, m_completionPort(NULL)
{
	// CPU���� * 2 - 1��ŭ ������ �����
	// 1���� ���� ������
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);

	m_threadCount = sysinfo.dwNumberOfProcessors * 2 - 1;
	m_completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, m_threadCount);

	if (m_completionPort == NULL)
	{
		ServerHelper::PrintLastError("Create Completion Port Error");
		return;
	}

	// ������ ������ŭ ������ �����
	for (int i = 0; i < m_threadCount; ++i)
	{
		std::thread worker(std::bind(&IOCP::WorkThread, this));
		m_workerThreadArry.push_back(std::move(worker));
	}
}

IOCP::~IOCP()
{
	// IOCP�� ����Ǿ��ִ� ������ ��� close�� �� �����带 �����ؾ���
	// ����� ���ϵ��� ��� ����ɶ����� ���ؽ�Ʈ ����Ī
	while (TCPListener::GetInst() != nullptr)
	{
		Sleep(1);
	}

	while (SessionManager::GetInst() != nullptr)
	{
		Sleep(1);
	}

	// ������ ����
	for (size_t i = 0; i < m_workerThreadArry.size(); i++)
	{
		// �����带 �����Ҷ��� PostQueued �۾� ��û
		PostQueuedCompletionStatus(m_completionPort, THREAD_EXIT_CODE, THREAD_EXIT_CODE, nullptr);
	}

	for (size_t i = 0; i < m_workerThreadArry.size(); i++)
	{
		m_workerThreadArry[i].join();
	}
}

void IOCP::WorkThread()
{
	while (true)
	{
		DWORD			transferredBytes;
		ULONG_PTR		completionKey;
		LPOVERLAPPED	overlapped;
		BOOL result = GetQueuedCompletionStatus(m_completionPort, &transferredBytes, &completionKey, &overlapped, INFINITE);

		// Ŭ���̾�Ʈ�� ��������� result = false, transferredBytes = 0
		// Ŭ���̾�Ʈ�� ��������� result = true,  transferredBytes = 0
		if (result == FALSE)
		{
			// Completion Port�� AcceptEx�� ������ ����ص�ä �����ϸ� 995 ����
			ServerHelper::PrintLastError("GetQueuedCompletionStauts Error");
			return;
		}

		// ������ ����üũ
		if (transferredBytes == THREAD_EXIT_CODE && completionKey == THREAD_EXIT_CODE)
			break;

		// �񵿱� ����� ��� ó��
		IOCompletionCallback* pIOCallback = reinterpret_cast<IOCompletionCallback*>(completionKey);
		assert(pIOCallback != nullptr);

		(*pIOCallback)(transferredBytes, overlapped);

	}
}


void IOCP::RegisterSocket(SOCKET _socket, IOCompletionCallback* _pIoCompletionCallback)
{
	if (NULL == CreateIoCompletionPort(reinterpret_cast<HANDLE>(_socket), m_completionPort, reinterpret_cast<ULONG_PTR>(_pIoCompletionCallback), 0))
	{
		ServerHelper::PrintLastError("Register Socket Error");
		return;
	}
}
