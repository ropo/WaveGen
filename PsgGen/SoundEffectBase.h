#pragma once

class SoundEffectBase
{
public:
	SoundEffectBase();
	virtual ~SoundEffectBase();
	virtual void Release()=0;
	virtual void Reset()=0;
	virtual float Effect( float in )=0;

	const SoundManager *m_pSoundManager;
};
