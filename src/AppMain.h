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
	bool ChangeOutputDS();
	bool ChangeOutputWaveFile();


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

	SoundOutputBase	*m_pSoundOutput;
	SeqInputBase	*m_pSeqInputBase;
	SoundManager	*m_pSoundMan;
	HANDLE			m_threadHandle;
	CRITICAL_SECTION m_cs;
	volatile bool	m_threadIsExit;

	static DWORD WINAPI TickThreadBase( LPVOID pParam );
	DWORD TickThread();
};
