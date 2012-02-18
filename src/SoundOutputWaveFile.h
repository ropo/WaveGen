#pragma once

class SoundOutputWaveFile : public SoundOutputBase
{
public:
	SoundOutputWaveFile();
	virtual ~SoundOutputWaveFile();

	bool Create( const wchar_t *pFileName );
	void Release();

	virtual void Tick();
	virtual size_t GetBlockSize() const;
	virtual void Write( const void *pWaveData, size_t blockSize );

private:
	HANDLE	m_fp;
	DWORD	m_writeTime;
	DWORD	m_lengthDataPos;
};
