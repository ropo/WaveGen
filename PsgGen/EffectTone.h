#pragma once

// 短波長
class EffectTone : public SoundEffectBase
{
public:
	EffectTone( float freq );
	virtual ~EffectTone();
	void Release();
	void Reset();
	float Effect( float in );

private:
	float	m_freq;
	float	m_bph;
	float	m_blocks;
};
