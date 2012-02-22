#include "stdafx.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCommand, int nCmdShow )
{
#ifdef _DEBUG
//	::_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
//	_CrtSetBreakAlloc( 144 );
#endif

	AppMain app;
	return app.Run( hInstance, hPrevInstance, lpCommand, nCmdShow );
}

AppMain::AppMain()
	: m_pSoundOutput( NULL )
	, m_pSeqInputBase( NULL )
	, m_pSoundMan( NULL )
	, m_threadHandle( NULL )
{
}

AppMain::~AppMain()
{
}

DWORD WINAPI AppMain::TickThreadBase( LPVOID pParam )
{
	AppMain *pThis = (AppMain*)pParam;
	return pThis->TickThread();
}

DWORD AppMain::TickThread()
{
	while( !m_threadIsExit ) {
		{
			CriticalBlock mb( &m_cs );
			if( m_pSeqInputBase->Tick( timeGetTime() ) )
				m_threadIsExit = true;
			m_pSoundMan->Tick();
		}
		Sleep( 10 );
	}
	m_threadIsExit = true;
	return 0;
}

bool AppMain::Startup()
{
	InitializeCriticalSection( &m_cs );

	m_threadHandle = CreateThread( NULL, 0, AppMain::TickThreadBase, this, CREATE_SUSPENDED, NULL );
	if( m_threadHandle == NULL ) {
		DeleteCriticalSection( &m_cs );
		return true;
	}
	m_threadIsExit = false;

	// 出力デバイスの作成
	if( ChangeOutputDS() )
		return true;

	// サウンドマネージャの作成
	m_pSoundMan = new SoundManager();
	m_pSoundMan->SetOutput( m_pSoundOutput );

	//
	std::wifstream ifs( L"sample.mml", std::ios::binary );
	if( ifs.fail() ) {
		ifs.open( L"../sample.mml", std::ios::binary );
		if( ifs.fail() )
			return true;
	}

	size_t fileSize = (size_t)ifs.seekg( 0, std::ios::end ).tellg();
	ifs.seekg( 0, std::ios::beg );

	wchar_t *pMML = new wchar_t[fileSize+1];
	ifs.read( pMML, fileSize );
	ifs.close();
	pMML[ fileSize ] = '\0';

	// プレイヤーをセット
	m_pSeqInputBase = new SeqInputMML();
	((SeqInputMML*)m_pSeqInputBase)->Init( m_pSoundMan );
	((SeqInputMML*)m_pSeqInputBase)->CompileMML( pMML );
	((SeqInputMML*)m_pSeqInputBase)->Play( timeGetTime() );

	ResumeThread( m_threadHandle );

	delete pMML;

	return false;
}

bool AppMain::ChangeOutputDS()
{
	CriticalBlock mb( &m_cs );

	SAFE_DELETE( m_pSoundOutput );
	m_pSoundOutput = new SoundOutputDS();
	return ((SoundOutputDS*)m_pSoundOutput)->Create( m_hWnd, 0.05f );
}

bool AppMain::ChangeOutputWaveFile()
{
	CriticalBlock mb( &m_cs );

	SAFE_DELETE( m_pSoundOutput );
	m_pSoundOutput = new SoundOutputWaveFile();
	return ((SoundOutputWaveFile*)m_pSoundOutput)->Create( L"output.wav" );
}

bool AppMain::Tick()
{
	BYTE keys[256];
	::GetKeyboardState( keys );

	if( keys['D'] & 0x80 ){	ChangeOutputDS();		CriticalBlock mb( &m_cs );	m_pSoundMan->SetOutput( m_pSoundOutput );	}
	if( keys['F'] & 0x80 ){	ChangeOutputWaveFile();	CriticalBlock mb( &m_cs );	m_pSoundMan->SetOutput( m_pSoundOutput );	}

	Sleep( 100 );

	if(	m_threadIsExit )
		return true;

	return false;
}

void AppMain::Release()
{
	m_threadIsExit = true;
	if( m_threadHandle ) {
		DWORD dwExitCode;
		for(;;) {
			if( GetExitCodeThread( m_threadHandle, &dwExitCode )==0 || dwExitCode == STILL_ACTIVE ) {
				Sleep( 100 );
				continue;
			}
			DeleteCriticalSection( &m_cs );
			break;
		}
	}
	SAFE_DELETE( m_pSoundMan );
	SAFE_DELETE( m_pSoundOutput );
	SAFE_DELETE( m_pSeqInputBase );
}
