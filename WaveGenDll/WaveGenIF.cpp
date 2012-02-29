#include "stdafx.h"


// ハンドル
typedef struct tagWAVEGENIF{
	HWND			hWnd;
	SoundManager	*pManager;
	SeqInputMML		*pInputMML;
	SoundOutputBase	*pOutputBase;
	std::wstring	waveFile;
}WAVEGENIF;

SoundOutputDS* CreateOutputDS( HWND hWnd )
{
	SoundOutputDS *pOutput = new SoundOutputDS();
	pOutput->Create( hWnd, 0.05f );
	return pOutput;
}

SoundOutputWaveFile* CreateOutputWaveFile( const wchar_t *pFileName )
{
	SoundOutputWaveFile *pOutput = new SoundOutputWaveFile();
	pOutput->Create( pFileName );
	return pOutput;
}

WAVEGENDLL_API HANDLE WINAPI CreateWaveGen(HWND hWnd)
{
	// ハンドラの作成
	WAVEGENIF *pHandler = new WAVEGENIF;
	pHandler->hWnd = hWnd;

	// サウンドマネージャの作成
	pHandler->pManager = new SoundManager();

	// 入力デバイスを作成
	pHandler->pInputMML = new SeqInputMML();
	pHandler->pInputMML->Init( pHandler->pManager );

	// 出力デバイスの作成
	pHandler->pOutputBase = CreateOutputDS( hWnd );

	// マネージャにI/O登録
	pHandler->pManager->ChangeInput( pHandler->pInputMML );
	pHandler->pManager->ChangeOutput( pHandler->pOutputBase );
	
	return pHandler;
}

WAVEGENDLL_API void WINAPI ReleaseWaveGen( HANDLE hInterface )
{
	if( hInterface ) {
		WAVEGENIF *pThis = (WAVEGENIF*)hInterface;
		if( pThis->pManager )
			delete pThis->pManager;
		delete pThis;
	}
	hInterface = nullptr;
}

WAVEGENDLL_API void WINAPI SetWaveFileName( HANDLE hInterface, LPCWSTR fileName )
{
	if( hInterface == nullptr )
		return;

	WAVEGENIF *pThis = (WAVEGENIF*)hInterface;
	pThis->waveFile = fileName;
}

WAVEGENDLL_API bool WINAPI CompileMML( HANDLE hInterface, LPCWSTR pMML, int *pErrorCode, DWORD *pErrorLine, bool isWave, void(*fncPlayFinished)(void*), int param )
{
	if( hInterface == nullptr )
		return true;

	WAVEGENIF *pThis = (WAVEGENIF*)hInterface;

	// WAVE保存なのにファイル名が指定されていない？
	if( isWave==true && pThis->waveFile.length()==0 )
		return true;

	// MMLをコンパイル
	SeqInputMML::COMPILEDINFO compileInfo;
	compileInfo = pThis->pInputMML->CompileMML( pMML );
	if( compileInfo.errorCode != 0 )
		return true;

	// 出力デバイスの切り替え
	if( isWave == true && dynamic_cast<SoundOutputWaveFile*>(pThis->pOutputBase) == nullptr ) {
		CriticalBlock cb( &pThis->pManager->m_cs );
		SoundOutputWaveFile *pOutput = CreateOutputWaveFile( pThis->waveFile.c_str() );
		pThis->pManager->ChangeOutput( pOutput );
		pThis->pOutputBase = pOutput;
		pOutput->Start();
	}
	if( isWave == false && dynamic_cast<SoundOutputDS*>(pThis->pOutputBase) == nullptr ) {
		CriticalBlock cb( &pThis->pManager->m_cs );
		pThis->pOutputBase = CreateOutputDS( pThis->hWnd );
		pThis->pManager->ChangeOutput( pThis->pOutputBase );
	}

	// 再生
	pThis->pInputMML->Play( timeGetTime(), fncPlayFinished, (void*)param );

	return false;
}

WAVEGENDLL_API void WINAPI GetErrorString( int errorCode, LPTSTR pErrorMessage, int messageBufferMaxCount )
{
	const wchar_t *pMsg = SeqInputMML::GetErrorString( errorCode );
	wcscpy_s( pErrorMessage, messageBufferMaxCount, pMsg );
}

WAVEGENDLL_API void WINAPI Stop( HANDLE hInterface )
{
	if( hInterface == NULL )
		return;

	WAVEGENIF *pThis = (WAVEGENIF*)hInterface;
	pThis->pInputMML->Stop();

	// 出力をDSにする
	if( dynamic_cast<SoundOutputDS*>(pThis->pOutputBase) == nullptr ) {
		pThis->pOutputBase = CreateOutputDS( pThis->hWnd );
		pThis->pManager->ChangeOutput( pThis->pOutputBase );
	}
}
