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
	SeqInputBase	*m_pSeqInputBase;
	SoundManager	*m_pSoundMan;
};
