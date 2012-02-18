#include "stdafx.h"

SoundOutputWaveFile::SoundOutputWaveFile()
	: m_fp( NULL )
	, m_writeTime( timeGetTime() )
{
}

SoundOutputWaveFile::~SoundOutputWaveFile()
{
	Release();
}

bool SoundOutputWaveFile::Create( const wchar_t *pFileName )
{
	m_fp = CreateFile( pFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS , FILE_ATTRIBUTE_NORMAL , NULL );
	if( m_fp == INVALID_HANDLE_VALUE ) {
		m_fp = NULL;
		return true;
	}

	DWORD dwSize;
	WriteFile( m_fp, "RIFF", 4, &dwSize, NULL );

	DWORD cbSize = sizeof(PCMWAVEFORMAT) + 12;
	WriteFile( m_fp, &cbSize, sizeof(cbSize), &dwSize, NULL );

	WriteFile( m_fp, "WAVEfmt ", 8, &dwSize, NULL );
	cbSize = sizeof(PCMWAVEFORMAT);
	WriteFile( m_fp, &cbSize, sizeof(cbSize), &dwSize, NULL );

	PCMWAVEFORMAT pcmwf;
	pcmwf.wf.wFormatTag		= WAVE_FORMAT_PCM;
	pcmwf.wf.nChannels		= 2;
	pcmwf.wf.nSamplesPerSec	= BASE_FREQ;
	pcmwf.wf.nBlockAlign	= 4;
	pcmwf.wf.nAvgBytesPerSec= pcmwf.wf.nSamplesPerSec * pcmwf.wf.nBlockAlign;
	pcmwf.wBitsPerSample	= 16;
	WriteFile( m_fp, &pcmwf, sizeof(pcmwf), &dwSize, NULL );

	WriteFile( m_fp, "data", 4, &dwSize, NULL );

	cbSize = 0;
	m_lengthDataPos = SetFilePointer( m_fp, 0, NULL, FILE_CURRENT );
	WriteFile( m_fp, &cbSize, sizeof(cbSize), &dwSize, NULL );

	return false;
}

void SoundOutputWaveFile::Release()
{
	if( m_fp == NULL )
		return;

	DWORD dwSize;
	DWORD pos = SetFilePointer( m_fp, 0, NULL, FILE_CURRENT );
	DWORD cbSize = pos - m_lengthDataPos;
	SetFilePointer( m_fp, m_lengthDataPos, NULL, FILE_BEGIN );
	WriteFile( m_fp, &cbSize, sizeof(cbSize), &dwSize, NULL );

	cbSize = pos - 8;
	SetFilePointer( m_fp, 4, NULL, FILE_BEGIN );
	WriteFile( m_fp, &cbSize, sizeof(cbSize), &dwSize, NULL );

	CloseHandle( m_fp );
	m_fp = NULL;
}

void SoundOutputWaveFile::Tick()
{
}

size_t SoundOutputWaveFile::GetBlockSize() const
{
	DWORD dwTime = timeGetTime();
	return (dwTime - m_writeTime) * BASE_FREQ / 1000;
}

void SoundOutputWaveFile::Write( const void *pWaveData, size_t blockSize )
{
	if( m_fp == NULL )
		return;

	m_writeTime = timeGetTime();

	DWORD writeSize, dwSize;
	writeSize = blockSize * 4;
	WriteFile( m_fp, pWaveData, writeSize, &dwSize, NULL );
}
