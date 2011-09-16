#include "stdafx.h"

EffectTone::EffectTone( float freq )
{
	m_bph = BASE_FREQ / freq;
	m_blocks = 0;
}

EffectTone::~EffectTone()
{
}

void EffectTone::Release()
{
}
void EffectTone::Reset()
{
}
float EffectTone::Effect( float in )
{
	m_blocks++;
	while( m_bph < m_blocks )
		m_blocks -= m_bph;
	return ( m_bph/2 > m_blocks ) ? 0.2f : -0.2f;
}
