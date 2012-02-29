#include "stdafx.h"
#define DEFAULTMML	L"sample.mml"

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
	: m_pSoundMan( nullptr )
	, m_exit( false )
{
	m_commandParams.isDiskWrite = false;
	m_commandParams.mmlFile = DEFAULTMML;
}

AppMain::~AppMain()
{
	Release();
}

AppMain::COMMANDPARAM AppMain::ParseCommandLine()
{
	COMMANDPARAM params;
	params.mmlFile = DEFAULTMML;
	params.isDiskWrite = false;

	int at;
	std::wstring commandLine = GetCommandLine();
	std::vector<std::wstring> tokens;
	while( (at = commandLine.find_first_of(L" ")) != commandLine.npos ) {
		if( at > 0 )	tokens.push_back( commandLine.substr(0,at) );
		commandLine = commandLine.substr(at+1);
	}
	if( commandLine.length() > 0 )
		tokens.push_back( commandLine );
	tokens.erase( tokens.begin() );

	for( std::vector<std::wstring>::iterator itr=tokens.begin(); itr!=tokens.end(); ++itr ) {
		std::wstring token = *itr;
		std::transform(token.begin(), token.end(), token.begin(), std::tolower);
		if( token.compare( L"-disk" )==0 )
			params.isDiskWrite = true;
		else
			params.mmlFile = *itr;
	}

	return params;
}

bool AppMain::Startup()
{
	// 引数解析
	m_commandParams = ParseCommandLine();

	// MML読み込み
	std::wifstream ifs( m_commandParams.mmlFile, std::ios::binary );
	if( ifs.fail() ) {
		return true;
	}
	size_t fileSize = (size_t)ifs.seekg( 0, std::ios::end ).tellg();
	ifs.seekg( 0, std::ios::beg );
	wchar_t *pMML = new wchar_t[fileSize+1];
	ifs.read( pMML, fileSize );
	ifs.close();
	pMML[ fileSize ] = '\0';

	// サウンドマネージャの作成
	m_pSoundMan = new SoundManager();

	// プレイヤーをセット
	SeqInputMML *pSeqInput = new SeqInputMML();
	pSeqInput->Init( m_pSoundMan );
	SeqInputMML::COMPILEDINFO compiledInfo;
	compiledInfo = pSeqInput->CompileMML( pMML );
	if( compiledInfo.errorCode != 0 ) {
		wchar_t errorMessage[1024];
		wsprintf( errorMessage, L"エラー\n %d行目\n%s", compiledInfo.errorLine, SeqInputMML::GetErrorString( compiledInfo.errorCode ) );
		MessageBox( m_hWnd, errorMessage, L"WAVEGEN", MB_ICONSTOP );
	}
	delete pMML;

	// マネージャにセット
	m_pSoundMan->ChangeInput( pSeqInput );

	// 出力デバイスのマネージャにセット
	SoundOutputBase *pOutputBase = m_commandParams.isDiskWrite ? (SoundOutputBase*)ChangeOutputWaveFile() : (SoundOutputBase*)ChangeOutputDS(m_hWnd);
	if( pOutputBase == nullptr )
		return true;

	if( m_commandParams.isDiskWrite )
		((SoundOutputWaveFile*)pOutputBase)->Start();

	// 再生開始
	pSeqInput->Play( timeGetTime(), fncPlayFinishedBase, this );

	return false;
}

void AppMain::fncPlayFinishedBase( void *pThis )
{
	((AppMain*)pThis)->m_exit = true;
}

SoundOutputDS* AppMain::ChangeOutputDS( HWND hWnd )
{
	if( m_pSoundMan == nullptr )
		return nullptr;

	SoundOutputDS *pSoundOutput = new SoundOutputDS();
	if( pSoundOutput->Create( hWnd, 0.05f ) ) {
		SAFE_DELETE( pSoundOutput );
		return nullptr;
	}
	m_pSoundMan->ChangeOutput( pSoundOutput );
	return pSoundOutput;
}

SoundOutputWaveFile* AppMain::ChangeOutputWaveFile( const wchar_t *pWriteFile)
{
	if( m_pSoundMan == nullptr )
		return nullptr;

	SoundOutputWaveFile *pSoundOutput = new SoundOutputWaveFile();
	if( pSoundOutput->Create( pWriteFile ) ) {
		SAFE_DELETE( pSoundOutput );
		return nullptr;
	}
	m_pSoundMan->ChangeOutput( pSoundOutput );
	return pSoundOutput;
}

bool AppMain::Tick()
{
	BYTE keys[256];
	::GetKeyboardState( keys );

	if( keys['D'] & 0x80 )	ChangeOutputDS(m_hWnd);
	if( keys['F'] & 0x80 ){
		SoundOutputWaveFile *pSoundOutput = ChangeOutputWaveFile();
		if( pSoundOutput )
			pSoundOutput->Start();
	}

	Sleep( 100 );

	return m_exit;
}

void AppMain::Release()
{
	SAFE_DELETE( m_pSoundMan );
}
