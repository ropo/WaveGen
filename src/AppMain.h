#pragma once

class AppMain : public WinBase
{
public:
	AppMain();
	virtual ~AppMain();

	virtual bool Startup();
	virtual bool Tick();
	virtual void Release();

private:
	SoundManager	*m_pSoundMan;
	EffectGen		*m_pGen;
};
