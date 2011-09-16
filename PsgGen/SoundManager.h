#pragma once

class SoundEffectBase;
class SoundEffectSet;

class SoundManager
{
public:
	SoundManager( float bufferSec );
	virtual ~SoundManager();
	bool Create( HWND hWnd );
	void Release();

	DWORD Push( SoundEffectSet *pSBB );
	DWORD Push( SoundEffectSet *pSEB, int updateHandle );
	bool Remove( DWORD handle );
	void Tick();


private:
	void Charge( void *pBuf, size_t byteSize );

private:
	typedef struct tagEFFECTINFO{
		int	handle;
		SoundEffectSet *pBase;
	}EFFECTINFO;

	float	m_bufferSec;
	LPDIRECTSOUND8	m_pDS8;
	LPDIRECTSOUNDBUFFER	m_pDSB;
	WAVEFORMATEX m_wfx;
	typedef std::vector<EFFECTINFO>		EFFECTSETLIST;
	typedef EFFECTSETLIST::iterator		EFFECTSETLISTITR;
	EFFECTSETLIST m_effectSet;
	DWORD m_effectHandle;

	DWORD m_writePos;
	DWORD GetBufferByte() const;
};

