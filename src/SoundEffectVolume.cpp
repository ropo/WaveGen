#include "stdafx.h"

SoundEffectVolume::SoundEffectVolume()
	: m_volume( 1.0f )
{
	Reset();
}

SoundEffectVolume::~SoundEffectVolume()
{
}

void SoundEffectVolume::Release()
{
	Reset();
}

void SoundEffectVolume::Reset()
{
}

void SoundEffectVolume::ChangeVolume( float volume )
{
	m_volume = volume;
}

void SoundEffectVolume::Effect( float *pBuffer, size_t bloackSize )
{
	for( ; bloackSize; bloackSize--, pBuffer++ ) {
		*pBuffer *= m_volume;
	}
}
