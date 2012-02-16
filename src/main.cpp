#include "stdafx.h"

int main(int , _TCHAR* [])
{
#ifdef _DEBUG
	::_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
//	_CrtSetBreakAlloc( 185 );
#endif

	// サウンドドライバとマネージャの作成
	SoundManager *pSoundMan = new SoundManager( 0.05f );
	pSoundMan->Create( GetDesktopWindow() );

	EffectGen *pGen;
	{
		SoundEffectSet *pSet = new SoundEffectSet( 0 );

		pGen = new EffectGen( 293.7f, EffectGen::SILENT );
		pSet->Push( pGen );

		pSoundMan->Push( pSet );
	}

	printf( "[q]:終了\n"
			"EffectGen\n"
			" [s]:矩形波 [a]:のこぎり波 [t]:三角波 [i]:正弦波 [n]:ノイズ [0]:無音\n"
			" [1]～[9]:周波数変更(プリセット)\n" );

	// [q]キー押すまでぐるぐる
	bool isLoop = true;
	while( isLoop ) {
		if( _kbhit() ) {
			switch( _getch() ) {
				case 'q':	isLoop = false;							break;
				case 's':	pGen->ChangeType( EffectGen::SQUARE );	break;
				case 'a':	pGen->ChangeType( EffectGen::SAW );		break;
				case 't':	pGen->ChangeType( EffectGen::TRIANGLE );break;
				case 'i':	pGen->ChangeType( EffectGen::SINEWAVE );break;
				case 'n':	pGen->ChangeType( EffectGen::NOISE );	break;
				case '0':	pGen->ChangeType( EffectGen::SILENT );	break;
				case '1':	pGen->ChangeFreq( 261.6f );				break;
				case '2':	pGen->ChangeFreq( 293.7f );				break;
				case '3':	pGen->ChangeFreq( 329.6f );				break;
				case '4':	pGen->ChangeFreq( 349.2f );				break;
				case '5':	pGen->ChangeFreq( 392.0f );				break;
				case '6':	pGen->ChangeFreq( 440.0f );				break;
				case '7':	pGen->ChangeFreq( 493.9f );				break;
				case '8':	pGen->ChangeFreq( 523.3f );				break;
				case '9':	pGen->ChangeFreq( 587.3f );				break;
			}
		}

		pSoundMan->Tick();
		Sleep( 1 );
	}

	pSoundMan->Release();
	delete pSoundMan;

	return 0;
}

