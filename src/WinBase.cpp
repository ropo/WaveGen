#include "stdafx.h"

//--------------------------------------------------------------------------
// ウインドウサイズを変更する
//--------------------------------------------------------------------------
void changeWindowSize( HWND hWnd, int w, int h )
{
	RECT rect;
	GetWindowRect( hWnd, &rect );
	ClientToScreen( hWnd, (POINT*)&rect );
	rect.right = rect.left + w;
	rect.bottom= rect.top  + h;
	AdjustWindowRect( &rect, GetWindowLong( hWnd, GWL_STYLE ), GetMenu(hWnd)?TRUE:FALSE );
	MoveWindow( hWnd, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, TRUE );
}

//--------------------------------------------------------------------------
// メインウインドウプロシージャー
//--------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
		//-------------------------
		// 描画
		//-------------------------
		case WM_PAINT:{
			PAINTSTRUCT ps;
			HDC hdc;
			hdc = BeginPaint(hWnd, &ps);

			RECT rect={10,10,200,200};
			DrawText( hdc,  L"[Q]:終了\n\n"
							L"EffectGen\n"
							L" [S]:矩形波 [A]:のこぎり波 [T]:三角波 [I]:正弦波 [N]:ノイズ [0]:無音\n"
							L" [1]～[9]:周波数変更(プリセット)\n", -1, &rect, DT_NOCLIP );

			EndPaint(hWnd, &ps);
			}break;

		//-------------------------
		// 破棄
		//-------------------------
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//--------------------------------------------------------------------------
// メインウインドウの構築
//--------------------------------------------------------------------------
HWND InitInstance( HINSTANCE hInst )
{
	WNDCLASSEX wcex;
	ZeroMemory( &wcex, sizeof(wcex) );
	wcex.cbSize			= sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.hInstance		= hInst;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.lpszClassName	= L"waveGenMainCN";
	wcex.hIconSm		= NULL;
	RegisterClassEx(&wcex);

	HWND hWnd = CreateWindowEx(
		  WS_EX_ACCEPTFILES
		, wcex.lpszClassName
		, L"WaveGen" 
		, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME
		, CW_USEDEFAULT, 0
		, CW_USEDEFAULT, 0
		, NULL
		, NULL
		, wcex.hInstance
		, NULL
	);

	return hWnd;
}

WinBase::WinBase()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
}

WinBase::~WinBase()
{
	CoUninitialize();
}

int WinBase::Run(HINSTANCE hInstance, HINSTANCE, LPSTR , int nCmdShow )
{
	// アプリケーションの初期化を実行します。
	m_hWnd = InitInstance( hInstance );
	if( m_hWnd == NULL )
		return 1;

	if( Startup() == false )
		return 2;

	ShowWindow( m_hWnd, nCmdShow );


#ifdef _DEBUG
	::_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
//	_CrtSetBreakAlloc( 185 );
#endif

	// メインループ
	bool exit = false;
	while( !exit ) {
		MSG msg;
		while( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) {
			if (msg.message == WM_QUIT) {
				PostQuitMessage( (int)msg.wParam );
				Release();
				return 0;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Tick();
	}

	return 0;
}

