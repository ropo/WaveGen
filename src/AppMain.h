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
	SoundOutputDS* ChangeOutputDS( HWND hWnd );
	SoundOutputWaveFile* ChangeOutputWaveFile( const wchar_t *pWriteFile=L"output.wav" );

private:

	typedef struct tagCOMMANDPARAM {
		std::wstring	mmlFile;
		bool			isDiskWrite;
	}COMMANDPARAM;
	COMMANDPARAM	m_commandParams;

	static COMMANDPARAM ParseCommandLine();
	SoundManager	*m_pSoundMan;
	bool			m_exit;

	static void fncPlayFinishedBase( void *pThis );
	void fncPlayFinished();
};
