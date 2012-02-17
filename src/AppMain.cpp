#include "stdafx.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCommand, int nCmdShow )
{
	AppMain app;
	return app.Run( hInstance, hPrevInstance, lpCommand, nCmdShow );
}

AppMain::AppMain()
{
}

AppMain::~AppMain()
{
}

bool AppMain::Startup()
{
	// サウンドドライバとマネージャの作成
	m_pSoundMan = new SoundManager( 0.05f );
	m_pSoundMan->Create( m_hWnd );

	{
		SoundEffectSet *pSet = new SoundEffectSet( 0 );

		m_pGen = new EffectGen( 293.7f, EffectGen::SILENT );
		pSet->Push( m_pGen );

		m_pSoundMan->Push( pSet );
	}
	return true;
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

	m_pSoundMan->Tick();
	Sleep( 1 );

	return true;
}

void AppMain::Release()
{
	m_pSoundMan->Release();
	delete m_pSoundMan;
	m_pSoundMan = NULL;
}

