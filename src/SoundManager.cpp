#include "stdafx.h"

#define SAFE_RELEASE(o)	if(o){(o)->Release();(o)=NULL;}else 0

SoundManager::SoundManager( float bufferSec )
	: m_pDSDev( NULL )
	, m_pDSB( NULL )
	, m_bufferSec( bufferSec )
	, m_writePos( 0 )
{
}
SoundManager::~SoundManager()
{
	Release();
}
DWORD SoundManager::GetBufferByte() const
{
	return (DWORD)(m_wfx.nSamplesPerSec * m_wfx.nBlockAlign * m_bufferSec);
}
DWORD SoundManager::Push( SoundEffectSet *pSES )
{
	EFFECTINFO effectInfo;
	effectInfo.handle = m_effectHandle++;
	effectInfo.pBase = pSES;
	m_effectSet.push_back( effectInfo );
	return effectInfo.handle;
}
DWORD SoundManager::Push( SoundEffectSet *pSEB, int updateHandle )
{
	Remove( updateHandle );
	return Push( pSEB );
}
bool SoundManager::Remove( DWORD handle )
{
	for( EFFECTSETLISTITR itr=m_effectSet.begin(); itr!=m_effectSet.end(); ++itr ) {
		if( itr->handle == handle ) {
			m_effectSet.erase( itr );
			return false;
		}
	}
	return true;
}
void SoundManager::Store( void *pBuf, size_t byteSize )
{
	size_t blockSize = byteSize/4;
	float *l = new float[blockSize];
	float *r = new float[blockSize];
	ZeroMemory( l, blockSize*sizeof(float) );
	ZeroMemory( r, blockSize*sizeof(float) );

	for( EFFECTSETLISTITR itr=m_effectSet.begin(); itr!=m_effectSet.end(); ++itr ) {
		itr->pBase->GetWave( l, r, blockSize );
	}

	float v;
	short *w = (short*)pBuf;
	const float *rl = l;
	const float *rr = r;
	for( ; blockSize; blockSize-- ) {
		v = MinMax( *rl++, -1.0f, 1.0f );
		*w++ = (short)( v * 32767.0f * 0.5f );

		v = MinMax( *rr++, -1.0f, 1.0f );
		*w++ = (short)( v * 32767.0f * 0.5f );
	}
	delete l;
	delete r;
}
void SoundManager::Tick()
{
	if( m_pDSB == NULL )
		return;

	LPVOID lpvWrite1, lpvWrite2;
	DWORD dwLength1, dwLength2, lockSize, playCursor;
	const DWORD bufSize =  GetBufferByte();

	m_pDSB->GetCurrentPosition( &playCursor, NULL );
	if( playCursor < m_writePos )	lockSize = bufSize - m_writePos + playCursor;
	else							lockSize = playCursor - m_writePos;
	if( lockSize == 0 )
		return;

	if( DS_OK == m_pDSB->Lock( m_writePos, lockSize, &lpvWrite1, &dwLength1, &lpvWrite2, &dwLength2, 0 ) ) {
		Store( lpvWrite1, dwLength1 );
		Store( lpvWrite2, dwLength2 );
		m_pDSB->Unlock( lpvWrite1, dwLength1, lpvWrite2, dwLength2 );
		m_writePos += dwLength1 + dwLength2;
		if( m_writePos >= bufSize )
			m_writePos -= bufSize;
	}else{
		dwLength1 = dwLength2 = 0;
	}
}
void SoundManager::Release()
{
	for( EFFECTSETLISTITR itr = m_effectSet.begin(); itr!=m_effectSet.end(); ++itr ) {
		itr->pBase->Release();
		delete itr->pBase;
	}
	m_effectSet.clear();
	SAFE_RELEASE( m_pDSB );
	SAFE_RELEASE( m_pDSDev );
}
bool SoundManager::Create( HWND hWnd )
{
	HRESULT ret;
	ret = DirectSoundCreate8( NULL, &m_pDSDev, NULL );
	if( FAILED(ret) )
		return true;
	m_pDSDev->SetCooperativeLevel( hWnd, DSSCL_NORMAL );

 	// サウンドバッファの作成
	ZeroMemory(&m_wfx, sizeof(m_wfx));
	m_wfx.wFormatTag = WAVE_FORMAT_PCM;
	m_wfx.nChannels = 2;
	m_wfx.nSamplesPerSec = BASE_FREQ;
	m_wfx.wBitsPerSample = 16;
	m_wfx.nBlockAlign = (WORD)(m_wfx.wBitsPerSample * m_wfx.nChannels / 8);
	m_wfx.nAvgBytesPerSec = (DWORD)(m_wfx.nSamplesPerSec * m_wfx.nBlockAlign);

	DSBUFFERDESC dsdesc;
	ZeroMemory( &dsdesc, sizeof(dsdesc) );
	dsdesc.dwSize = sizeof(dsdesc);
	dsdesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_STATIC | DSBCAPS_LOCDEFER | DSBCAPS_GLOBALFOCUS;
	dsdesc.dwBufferBytes = GetBufferByte();
	dsdesc.lpwfxFormat = &m_wfx;
	dsdesc.guid3DAlgorithm = DS3DALG_DEFAULT;
	ret = m_pDSDev->CreateSoundBuffer( &dsdesc, &m_pDSB, NULL );
	if( FAILED(ret) )
		return true;

	// バッファクリア
	LPVOID lpvWrite = 0;
	DWORD dwLength = 0;
	if( DS_OK == m_pDSB->Lock( 0, 0, &lpvWrite, &dwLength, NULL, NULL, DSBLOCK_ENTIREBUFFER ) ) {
		ZeroMemory( lpvWrite, dwLength );
		m_pDSB->Unlock( lpvWrite, dwLength, NULL, 0);
	}

	// 再生
	m_pDSB->Play( NULL, 0, DSBPLAY_LOOPING );
	return false;
}