#pragma once

class AppMain : public WinBase
{
public:
	AppMain();
	virtual ~AppMain();

	virtual bool Startup();
	virtual bool Tick();
	virtual void Release();

public:
	bool ChangeOutputDS( HWND hWnd );
	bool ChangeOutputWaveFile( const wchar_t *pWriteFile=L"output.wav" );
	bool CreatePlayThread();
	void ReleasePlayThread();

	SoundOutputBase	*m_pSoundOutput;
	SeqInputBase	*m_pSeqInputBase;
	SoundManager	*m_pSoundMan;

private:
	class CriticalBlock {
		public:
			CriticalBlock( const LPCRITICAL_SECTION hCS ) {
				EnterCriticalSection( m_hCS = hCS );
			}
			~CriticalBlock() {
				LeaveCriticalSection( m_hCS );
			}
		private:
			LPCRITICAL_SECTION	m_hCS;
	};

	typedef struct tagCOMMANDPARAM {
		std::wstring	mmlFile;
		bool			isDiskWrite;
	}COMMANDPARAM;

	HANDLE			m_threadHandle;
	CRITICAL_SECTION m_cs;
	volatile bool	m_threadIsExit;
	COMMANDPARAM	m_commandParams;

	static COMMANDPARAM ParseCommandLine();
	static DWORD WINAPI TickThreadBase( LPVOID pParam );
	DWORD TickThread();
};
