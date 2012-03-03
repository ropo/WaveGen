#include "stdafx.h"

EffectGen::EffectGen()
{
	Reset();
	ChangeFreq( 440 );
	ChangeType( SQUARE );
	ChangeSquareDuty( 0.5f );
}

EffectGen::EffectGen( float freq, eTYPE type )
{
	Reset();
	ChangeFreq( freq );
	ChangeType( type );
	ChangeSquareDuty( 0.5f );
}

EffectGen::~EffectGen()
{
}
void EffectGen::ChangeFreq( float freq )
{
	if( freq <= 0 )
		freq = FLT_MIN;
	m_freq = freq;
	m_tph = BASE_FREQ / freq;
	while( m_blockCount >= m_tph )
		m_blockCount -= m_tph;
	m_sweepFreq = 0;
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
		case FCNOISE_S:
			EffectNoise( true );
			m_fncEffect = &EffectGen::EffectFcNoiseS;
			break;
		case FCNOISE_L:
			EffectNoise( true );
			m_fncEffect = &EffectGen::EffectFcNoiseL;
			break;
		case SILENT:
		default:
			m_fncEffect = &EffectGen::EffectSilent;
			break;
	}
}
void EffectGen::ChangeSquareDuty( float duty )
{
	m_squareDuty = MinMax( duty, FLT_MIN, 1.0f-FLT_EPSILON );
}

void EffectGen::Release()
{
}
void EffectGen::Reset()
{
	m_sweepFreq = 0;
	m_blockCount = 0;
	m_fcr = 0x8000;
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
		*pBuffer = (this->*m_fncEffect)( isFirst );
		if( m_sweepFreq ) {
			m_freq += m_sweepFreq;
			m_tph = BASE_FREQ / m_freq;
			if( m_tph < 0 )
				m_tph = -m_tph;
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

// ノイズショート
float EffectGen::EffectFcNoiseS( bool isFirst )
{
	if( isFirst ) {
		m_fcr >>= 1;
		m_fcr |= ((m_fcr^(m_fcr>>1))&1)<<15;
	}
	return (float)(m_fcr & 1);
}

// ノイズロング
float EffectGen::EffectFcNoiseL( bool isFirst )
{
	if( isFirst ) {
		m_fcr >>= 1;
		m_fcr |= ((m_fcr^(m_fcr>>6))&1)<<15;
	}

	return (float)(m_fcr & 1);
}

// 無音
float EffectGen::EffectSilent( bool )
{
	return 0;
}
