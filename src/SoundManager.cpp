#include "stdafx.h"

SoundManager::SoundManager()
	: m_pOutputBase( NULL )
{
}
SoundManager::~SoundManager()
{
	Release();
}

DWORD SoundManager::Push( SoundEffectSet *pSES )
{
	EFFECTINFO effectInfo;
	effectInfo.handle = m_effectHandle++;
	effectInfo.pBase = pSES;
	m_effectSet.push_back( effectInfo );
	return effectInfo.handle;
}

DWORD SoundManager::Push( SoundEffectSet *pSES, int updateHandle )
{
	Remove( updateHandle );
	return Push( pSES );
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

void SoundManager::SetOutput( SoundOutputBase *pSoundOutput )
{
	m_pOutputBase = pSoundOutput;
}

void SoundManager::Store( void *pBuf, size_t byteSize )
{
	size_t blockSize = byteSize/4;
	if( blockSize == 0 )
		return;


	float *l = new float[blockSize];	// ワークバッファ(左ch)
	float *r = new float[blockSize];	// ワークバッファ(右ch)
	float *mixl = new float[blockSize];	// ミキシング用バッファ（左ch）
	float *mixr = new float[blockSize];	// ミキシング用バッファ（右ch）
	ZeroMemory( mixl, blockSize*sizeof(float) );
	ZeroMemory( mixr, blockSize*sizeof(float) );

	// エフェクトセットループ
	for( EFFECTSETLISTITR itr=m_effectSet.begin(); itr!=m_effectSet.end(); ++itr ) {
		ZeroMemory( l, blockSize*sizeof(float) );
		ZeroMemory( r, blockSize*sizeof(float) );

		// エフェクトセットより波形データを取得
		itr->pBase->GetWave( l, r, blockSize );

		// ミキシング
		float *prl = l, *prr = r;
		float *pwl = mixl, *pwr = mixr;
		for( size_t i=0; i<blockSize; i++ ) {
			*(pwl++) += *(prl++);
			*(pwr++) += *(prr++);
		}
	}
	delete l;
	delete r;

	// ミキシングされたデータを float から short へ変換
	float v;
	short *w = (short*)pBuf;
	const float *rl = mixl;
	const float *rr = mixr;
	for( ; blockSize; blockSize-- ) {
		v = MinMax( *rl++, -1.0f, 1.0f );
		*w++ = (short)( v * 32767.0f );

		v = MinMax( *rr++, -1.0f, 1.0f );
		*w++ = (short)( v * 32767.0f );
	}
	delete mixl;
	delete mixr;
}

void SoundManager::Tick()
{
	if( m_pOutputBase == NULL )
		return;

	m_pOutputBase->Tick();
	size_t blockSize = m_pOutputBase->GetBlockSize();
	void *pWaveData = new char[ blockSize*4 ];
	Store( pWaveData, blockSize*4 );
	m_pOutputBase->Write( pWaveData, blockSize );
	delete pWaveData;
}

void SoundManager::Release()
{
	for( EFFECTSETLISTITR itr = m_effectSet.begin(); itr!=m_effectSet.end(); ++itr ) {
		itr->pBase->Release();
		delete itr->pBase;
	}
	m_effectSet.clear();
}
