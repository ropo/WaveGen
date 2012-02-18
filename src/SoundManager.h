#pragma once

class SoundEffectBase;
class SoundEffectSet;

class SoundManager
{
public:
	SoundManager();
	virtual ~SoundManager();
	void Release();

	void SetOutput( SoundOutputBase *pSoundOutput );
	DWORD Push( SoundEffectSet *pSBB );
	DWORD Push( SoundEffectSet *pSEB, int updateHandle );
	bool Remove( DWORD handle );
	void Tick();

private:
	void Store( void *pBuf, size_t byteSize );

private:
	typedef struct tagEFFECTINFO{
		DWORD			handle;
		SoundEffectSet *pBase;
	}EFFECTINFO;

	SoundOutputBase *m_pOutputBase;

	typedef std::vector<EFFECTINFO>		EFFECTSETLIST;
	typedef EFFECTSETLIST::iterator		EFFECTSETLISTITR;
	EFFECTSETLIST m_effectSet;
	DWORD m_effectHandle;
};

