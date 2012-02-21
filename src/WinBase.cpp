#include "stdafx.h"

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			DrawText( hdc,  L"SoundOutput\n"
							L" [D]:スピーカー出力\n"
							L" [F]:ファイル出力 (output.wav)\n"
			, -1, &rect, DT_NOCLIP );
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

static HWND InitInstance( HINSTANCE hInst )
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
	m_hWnd = InitInstance( hInstance );
	if( m_hWnd == NULL )
		return 1;

	if( Startup() )
		return 2;

	ShowWindow( m_hWnd, nCmdShow );

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

		if( Tick() )
			DestroyWindow( m_hWnd );
	}

	return 0;
}

