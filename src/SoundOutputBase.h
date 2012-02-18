#pragma once

class SoundOutputBase
{
public:
	SoundOutputBase();
	virtual ~SoundOutputBase();

	virtual void Release()=0;
	virtual void Tick()=0;
	virtual size_t GetBlockSize() const=0;
	virtual void Write( const void *pWaveData, size_t blockSize )=0; 
};
