#pragma once

class IMMLEffectGen
{
public:
	typedef unsigned char progID_t;

	virtual void ChangeFreq(float freq) = 0;
	virtual void ChangeFCNoiseFreq(BYTE note) = 0;;
	virtual void ChangeFreqSweep(float fromFreq, float toFreq, float time) = 0;;
	virtual void ChangeProgram(progID_t prog) = 0;
	virtual progID_t GetProgram() = 0;
};