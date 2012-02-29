#pragma once

class SoundOutputDS : public SoundOutputBase
{
public:
	SoundOutputDS();
	virtual ~SoundOutputDS();

	bool Create( HWND hWnd, float bufferSec );
	void Release();
	void Mute();

	virtual size_t GetBlockSize( DWORD time ) const;
	virtual void Write( const void *pWaveData, size_t blockSize, DWORD time );

private:
	DWORD GetBufferByte() const;

	float m_bufferSec;
	LPDIRECTSOUND8	m_pDSDev;
	LPDIRECTSOUNDBUFFER	m_pDSB;
	WAVEFORMATEX m_wfx;

	DWORD m_writePos;
};
