#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	::_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
//	_CrtSetBreakAlloc( 185 );

	// サウンドドライバとマネージャの作成
	SoundManager *pSoundMan = new SoundManager( 0.5 );	// 44.1kHzでバッファは 0.5秒
	pSoundMan->Create( GetDesktopWindow() );

	// センタリング
	SoundEffectSet *pSet = new SoundEffectSet( 0 );

	// 440hzの単純トーンエフェクト
	pSet->Push( new EffectTone( 440.000f ) );

	// マネージャにセット
	pSoundMan->Push( pSet );

	// [X]ボタン押すまでぐるぐる
	while( true )
	{
		pSoundMan->Tick();
		Sleep( 100 );
	}

	// 飛んでこないんだけどね
	pSoundMan->Release();
	delete pSoundMan;

	return 0;
}

