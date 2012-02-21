#pragma once

// �P���g�`����
class SoundEffectADSR : public SoundEffectBase
{
public:
	enum eTYPE{
		 SQUARE		// ��`�g
		,TRIANGLE	// �O�p�g
		,SAW		// �̂�����g
		,SINEWAVE	// �����g
		,NOISE		// �m�C�Y
		,SILENT		// ����
	};

	SoundEffectADSR();
	virtual ~SoundEffectADSR();
	void Release();
	void Reset();
	void Effect( float *pBuffer, size_t size );

	void ChangeParam( float aPower, float aTime, float dTime, float sPower, float rTime );
	void NoteOn();
	void NoteOff();

private:
	float EffectAttack();
	float EffectDecay();
	float EffectSustain();
	float EffectRelease();
	float EffectMute();

	float(SoundEffectADSR::*m_fncEffect)();
	float	m_tph;
	float	m_blockCount;

	DWORD	m_startTime;
	float	m_aPower, m_sPower;
	DWORD	m_aTime, m_dTime, m_rTime;
};
