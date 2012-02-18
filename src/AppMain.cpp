#include "stdafx.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCommand, int nCmdShow )
{
#ifdef _DEBUG
	::_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
//	_CrtSetBreakAlloc( 144 );
#endif

	AppMain app;
	return app.Run( hInstance, hPrevInstance, lpCommand, nCmdShow );
}

AppMain::AppMain()
	: m_pSoundOutput( NULL )
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

	// 単純波形ジェネレータの作成
	{
		SoundEffectSet *pSet = new SoundEffectSet( 0 );

		m_pGen = new EffectGen( 293.7f, EffectGen::SILENT );
		pSet->Push( m_pGen );

		m_pSoundMan->Push( pSet );
	}

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

	if( keys['Q'] & 0x80 )	return false;
	if( keys['S'] & 0x80 )	m_pGen->ChangeType( EffectGen::SQUARE );
	if( keys['A'] & 0x80 )	m_pGen->ChangeType( EffectGen::SAW );	
	if( keys['T'] & 0x80 )	m_pGen->ChangeType( EffectGen::TRIANGLE );
	if( keys['I'] & 0x80 )	m_pGen->ChangeType( EffectGen::SINEWAVE );
	if( keys['N'] & 0x80 )	m_pGen->ChangeType( EffectGen::NOISE );
	if( keys['0'] & 0x80 )	m_pGen->ChangeType( EffectGen::SILENT );
	if( keys['1'] & 0x80 )	m_pGen->ChangeFreq( 261.6f );			
	if( keys['2'] & 0x80 )	m_pGen->ChangeFreq( 293.7f );			
	if( keys['3'] & 0x80 )	m_pGen->ChangeFreq( 329.6f );			
	if( keys['4'] & 0x80 )	m_pGen->ChangeFreq( 349.2f );			
	if( keys['5'] & 0x80 )	m_pGen->ChangeFreq( 392.0f );			
	if( keys['6'] & 0x80 )	m_pGen->ChangeFreq( 440.0f );			
	if( keys['7'] & 0x80 )	m_pGen->ChangeFreq( 493.9f );			
	if( keys['8'] & 0x80 )	m_pGen->ChangeFreq( 523.3f );			
	if( keys['9'] & 0x80 )	m_pGen->ChangeFreq( 587.3f );			

	if( keys['D'] & 0x80 ){	ChangeOutputDS();		m_pSoundMan->SetOutput( m_pSoundOutput );	}
	if( keys['F'] & 0x80 ){	ChangeOutputWaveFile();	m_pSoundMan->SetOutput( m_pSoundOutput );	}

	m_pSoundMan->Tick();
	Sleep( 1 );

	return true;
}

void AppMain::Release()
{
	SAFE_DELETE( m_pSoundMan );
	SAFE_DELETE( m_pSoundOutput );
}

