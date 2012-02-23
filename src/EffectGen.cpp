#include "stdafx.h"

EffectGen::EffectGen( float freq, eTYPE type )
{
	ChangeFreq( freq );
	ChangeType( type );
	ChangeSquareDuty( 0.5f );
}

EffectGen::~EffectGen()
{
}
void EffectGen::ChangeFreq( float freq )
{
	m_freq = freq;
	m_tph = BASE_FREQ / freq;
	m_blockCount = 0;
	m_sweepFreq = 0;
}
void EffectGen::ChangeType( eTYPE type )
{
	m_type = type;
	switch( type )
	{
		case SQUARE:
			m_fncEffect = &EffectGen::EffectSquare;
			break;
		case TRIANGLE:
			m_fncEffect = &EffectGen::EffectTriangle;
			break;
		case SAW:
			m_fncEffect = &EffectGen::EffectSaw;
			break;
		case SINEWAVE:
			m_fncEffect = &EffectGen::EffectSinewave;
			break;
		case NOISE:
			EffectNoise( true );
			m_fncEffect = &EffectGen::EffectNoise;
			break;
		case SILENT:
		default:
			m_fncEffect = &EffectGen::EffectSilent;
			break;
	}
}
void EffectGen::ChangeSquareDuty( float duty )
{
	m_squareDuty = MinMax( duty, FLT_EPSILON, 1.0f-FLT_EPSILON );
}

void EffectGen::ChangeFreqSweep( float fromFreq, float toFreq, float time )
{
	if( time <= 0 ) {
		ChangeFreq( toFreq );
		m_sweepFreq = 0;
	}else{
		ChangeFreq( fromFreq );
		m_sweepFreq = (toFreq - fromFreq) / (BASE_FREQ * time);
	}
}

void EffectGen::Release()
{
}
void EffectGen::Reset()
{
	m_sweepFreq = 0;
}

void EffectGen::Effect( float *pBuffer, size_t bloackSize )
{
	for( ; bloackSize; bloackSize--, pBuffer++ ) {
		bool isFirst = false;
		m_blockCount++;
		while( m_tph <= m_blockCount ) {
			m_blockCount -= m_tph;
			isFirst = true;
		}
		*pBuffer += (this->*m_fncEffect)( isFirst );
		if( m_sweepFreq ) {
			m_freq += m_sweepFreq;
			m_tph = BASE_FREQ / m_freq;
		}
	}
}

// 矩形波
float EffectGen::EffectSquare( bool )
{
	return ( ( m_tph * m_squareDuty < m_blockCount ) ? 1.0f : -1.0f );
}

// 三角波
float EffectGen::EffectTriangle( bool )
{
	float v = CalcLiner( 0, m_tph, m_blockCount ) * 4.0f;

	if( v >= 2.0f )	return 3.0f-v;
	else			return v-1.0f;
}

// のこぎり波
float EffectGen::EffectSaw( bool )
{
	return 1.0f - CalcLiner( 0, m_tph, m_blockCount ) * 2.0f;
}

// 正弦波
float EffectGen::EffectSinewave( bool )
{
	return sin( CalcLiner( 0, m_tph, m_blockCount ) * 3.141592f * 2.0f );
}

// ノイズ
float EffectGen::EffectNoise( bool isFirst )
{
	if( isFirst )
		m_noise = (float)((rand()%3)-1);

	return m_noise;
}

// 無音
float EffectGen::EffectSilent( bool )
{
	return 0;
}
