#pragma once

// �{�����[���G�t�F�N�g
class SoundEffectVolume : public SoundEffectBase
{
public:
	SoundEffectVolume();
	virtual ~SoundEffectVolume();
	void Release();
	void Reset();
	void Effect( float *pBuffer, size_t size );

	void ChangeVolume( float volume );

private:
	float	m_volume;
};
