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

void SoundManager::SetOutput( SoundOutputBase *pSoundOutput )
{
	m_pOutputBase = pSoundOutput;
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
