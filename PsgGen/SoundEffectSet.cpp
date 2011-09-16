#include "stdafx.h"

SoundEffectSet::SoundEffectSet( float pan /* -1(L) 0 +1(R) */ )
	: m_panpot( pan )
{
}
SoundEffectSet::~SoundEffectSet()
{
	Release();
}
void SoundEffectSet::GetWave( float *pOutLeft, float *pOutRight )
{
	float wave = 0;
	for( EFFECTLISTITR itr = m_effecters.begin(); itr!=m_effecters.end(); ++itr ) {
		wave += (*itr)->Effect( wave );
	}
	if( wave < -1.0f ) wave = -1.0f;
	else if( wave > 1.0f ) wave = 1.0f;

	*pOutLeft = *pOutRight = wave;
}
void SoundEffectSet::Push( SoundEffectBase *pSEB )
{
	pSEB->Reset();
	m_effecters.push_back( pSEB );
}
void SoundEffectSet::Release()
{
	for( EFFECTLISTITR itr = m_effecters.begin(); itr!=m_effecters.end(); ++itr ) {
		(*itr)->Release();
		delete (*itr);
	}
	m_effecters.clear();
}
