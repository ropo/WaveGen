#pragma once

class AppMain : public WinBase
{
public:
	AppMain();
	virtual ~AppMain();

	virtual bool Startup();
	virtual bool Tick();
	virtual void Release();

public:
	bool ChangeOutputDS();
	bool ChangeOutputWaveFile();


private:
	SoundOutputBase	*m_pSoundOutput;
	SoundManager	*m_pSoundMan;
	EffectGen		*m_pGen;
};
