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

{
}

AppMain::~AppMain()
{
}

bool AppMain::Startup()
{
	// 出力デバイスの作成
	if( ChangeOutputDS() )
		return true;

	// サウンドマネージャの作成
	m_pSoundMan = new SoundManager();
	m_pSoundMan->SetOutput( m_pSoundOutput );

	// プレイヤーをセット
	m_pSeqInputBase = new SeqInputConst();
	((SeqInputConst*)m_pSeqInputBase)->Init( m_pSoundMan );

	return false;
}

bool AppMain::ChangeOutputDS()
{
	SAFE_DELETE( m_pSoundOutput );
	m_pSoundOutput = new SoundOutputDS();
	return ((SoundOutputDS*)m_pSoundOutput)->Create( m_hWnd, 0.25f );
}

bool AppMain::ChangeOutputWaveFile()
{
	SAFE_DELETE( m_pSoundOutput );
	m_pSoundOutput = new SoundOutputWaveFile();
	return ((SoundOutputWaveFile*)m_pSoundOutput)->Create( L"output.wav" );
}

bool AppMain::Tick()
{
	BYTE keys[256];
	::GetKeyboardState( keys );

	if( keys['D'] & 0x80 ){	ChangeOutputDS();		m_pSoundMan->SetOutput( m_pSoundOutput );	}
	if( keys['F'] & 0x80 ){	ChangeOutputWaveFile();	m_pSoundMan->SetOutput( m_pSoundOutput );	}

	m_pSeqInputBase->Tick( timeGetTime() );
	m_pSoundMan->Tick();
	Sleep( 1 );

	return true;
}

void AppMain::Release()
{
	SAFE_DELETE( m_pSoundMan );
	SAFE_DELETE( m_pSoundOutput );
	SAFE_DELETE( m_pSeqInputBase );
}

