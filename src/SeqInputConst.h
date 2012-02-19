#pragma once

class SeqInputConst : public SeqInputBase
{
public:
	SeqInputConst();
	virtual ~SeqInputConst();

	bool Init( SoundManager *pManager );
	virtual bool Tick( DWORD dwTime );

private:

	typedef struct tagSOUNDSET {
		tagSOUNDSET(){
			pSoundSet = NULL;
			pGen = NULL;
		}

		SoundEffectSet	*pSoundSet;
		EffectGen		*pGen;
	}SOUNDSET;

	typedef struct tagSEQDATA {
		tagSEQDATA()
			: pNext( NULL ) {}
		void PushNext( tagSEQDATA *_pNext ) {
			pNext = _pNext;
		}


		BYTE	note;
		DWORD	gateTime;

		tagSEQDATA	*pNext;
	}SEQDATA;

	void Release();
	float GetFreq( BYTE note ) const;
	void PlaySeq( const SEQDATA *pSeq );

	SOUNDSET				m_holder;
	SoundManager			*m_pManager;
	SEQDATA					*m_pSeqDataRoot;
	SEQDATA					*m_pCurSeqData;
	DWORD					m_nextPlayTime;
};
