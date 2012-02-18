#pragma once

class SoundOutputDS : public SoundOutputBase
{
public:
	SoundOutputDS();
	virtual ~SoundOutputDS();

	bool Create( HWND hWnd, float bufferSec );
	void Release();

	virtual void Tick();
	virtual size_t GetBlockSize() const;
	virtual void Write( const void *pWaveData, size_t blockSize );

private:
	DWORD GetBufferByte() const;

	float m_bufferSec;
	LPDIRECTSOUND8	m_pDSDev;
	LPDIRECTSOUNDBUFFER	m_pDSB;
	WAVEFORMATEX m_wfx;

	DWORD m_writePos;
};
