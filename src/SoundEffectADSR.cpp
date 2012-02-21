#include "stdafx.h"

SoundEffectADSR::SoundEffectADSR()
	: m_fncEffect( &SoundEffectADSR::EffectMute )
{
	ChangeParam( 0, 0, 0, 1, 0 );
}

SoundEffectADSR::~SoundEffectADSR()
{
}
void SoundEffectADSR::ChangeParam( float aPower, float aTime, float dTime, float sPower, float rTime )
{
	m_aPower = aPower;
	m_sPower = sPower;
	m_aTime = (DWORD)(BASE_FREQ * aTime);
	m_dTime = (DWORD)(BASE_FREQ * dTime);
	m_rTime = (DWORD)(BASE_FREQ * rTime);
}
void SoundEffectADSR::Release()
{
}
void SoundEffectADSR::Reset()
{
	
}

void SoundEffectADSR::NoteOn()
{
	m_startTime = 0;
	m_fncEffect = &SoundEffectADSR::EffectAttack;
}

void SoundEffectADSR::NoteOff()
{
	m_startTime = 0;
	m_fncEffect = &SoundEffectADSR::EffectRelease;
}

void SoundEffectADSR::Effect( float *pBuffer, size_t bloackSize )
{
	for( ; bloackSize; bloackSize--, pBuffer++ ) {
		*pBuffer *= (this->*m_fncEffect)();
		m_startTime++;
	}
}

float SoundEffectADSR::EffectAttack()
{
	if( m_startTime >= m_aTime ) {
		m_startTime -= m_rTime;
		m_fncEffect = &SoundEffectADSR::EffectDecay;
		return EffectDecay();
	}

	return CalcLiner( 0, (float)m_aTime, (float)m_startTime ) * m_aPower;
}

float SoundEffectADSR::EffectDecay()
{
	if( m_startTime >= m_aTime ) {
		m_fncEffect = &SoundEffectADSR::EffectSustain;
		return EffectSustain();
	}

	return m_aPower - CalcLiner( 0, (float)m_aTime, (float)m_startTime ) * (m_aPower-m_sPower);
}

float SoundEffectADSR::EffectSustain()
{
	return m_sPower;
}

float SoundEffectADSR::EffectRelease()
{
	if( m_startTime >= m_rTime ) {
		m_startTime -= m_rTime;
		m_fncEffect = &SoundEffectADSR::EffectMute;
		return EffectMute();
	}

	return (1.0f-CalcLiner( 0, (float)m_rTime, (float)m_startTime )) * m_sPower;
}

float SoundEffectADSR::EffectMute()
{
	return 0;
}

