#pragma once

class WinBase
{
public:
	WinBase();
	virtual ~WinBase();

	int Run( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCommand, int nCmdShow );

	virtual bool Startup()=0;
	virtual bool Tick()=0;
	virtual void Release()=0;

public:
	HWND	m_hWnd;
};
