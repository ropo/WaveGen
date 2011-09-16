#include "stdafx.h"



SoundManager::SoundManager( float bufferSec )
	: m_pDS8( NULL )
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
};
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
void SoundManager::Charge( void *pBuf, size_t byteSize )
{
	unsigned short *w = (unsigned short*)pBuf;
	byteSize /= 4;	// 16bit x stereo
	for( size_t i=0; i<byteSize; i++ ) {
		for( EFFECTSETLISTITR itr=m_effectSet.begin(); itr!=m_effectSet.end(); ++itr ) {
			float l, r;
			itr->pBase->GetWave( &l, &r );
			w[i*2+0] = (unsigned short)(l*32767.0f);
			w[i*2+1] = (unsigned short)(r*32767.0f);
		}
	}
}
void SoundManager::Tick()
{
	if( m_pDSB == NULL )
		return;

	// サウンドリングバッファへ転送
	LPVOID lpvWrite1,lpvWrite2;
	DWORD dwLength1,dwLength2;
	DWORD playdSize, playCursor, writeCursor;
	m_pDSB->GetCurrentPosition( &playCursor, &writeCursor );
	if( playCursor < writeCursor )
		playCursor += GetBufferByte();
	playdSize = playCursor - writeCursor;
	if( DS_OK == m_pDSB->Lock( m_writePos, playdSize, &lpvWrite1, &dwLength1, &lpvWrite2, &dwLength2, 0 ) ) {
		Charge( lpvWrite1, dwLength1 );
		Charge( lpvWrite2, dwLength2 );
		m_pDSB->Unlock( lpvWrite1, dwLength1, lpvWrite2, dwLength2 );
		m_writePos += dwLength1 + dwLength2;
		m_writePos %= GetBufferByte();
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
}
bool SoundManager::Create( HWND hWnd )
{
	LPDIRECTSOUNDBUFFER     lpPrimary = NULL;

	DirectSoundCreate8( NULL, &m_pDS8, NULL );
	m_pDS8->SetCooperativeLevel( hWnd, DSSCL_NORMAL );

    // サウンドバッファの作成
	ZeroMemory(&m_wfx, sizeof(m_wfx));
	m_wfx.cbSize = sizeof(m_wfx);
	m_wfx.wFormatTag = WAVE_FORMAT_PCM;
	m_wfx.nChannels = 2;
	m_wfx.nSamplesPerSec = BASE_FREQ;
	m_wfx.wBitsPerSample = 16;
	m_wfx.nBlockAlign = (WORD)(m_wfx.wBitsPerSample / 8 * m_wfx.nChannels);
	m_wfx.nAvgBytesPerSec = (DWORD)(m_wfx.nSamplesPerSec * m_wfx.nBlockAlign);

    DSBUFFERDESC dsdesc;
    ZeroMemory( &dsdesc, sizeof(dsdesc) );
    dsdesc.dwSize = sizeof(dsdesc);
    dsdesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_STATIC | DSBCAPS_LOCDEFER | DSBCAPS_GLOBALFOCUS;
    dsdesc.dwBufferBytes = GetBufferByte();
    dsdesc.lpwfxFormat = &m_wfx;
    dsdesc.guid3DAlgorithm = DS3DALG_DEFAULT;
    HRESULT ret = m_pDS8->CreateSoundBuffer( &dsdesc, &m_pDSB, NULL );
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