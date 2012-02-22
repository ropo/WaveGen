#pragma once

#define MAX_TRACK	16

class SeqInputMML : public SeqInputBase
{
public:
	SeqInputMML();
	virtual ~SeqInputMML();

	bool Init( SoundManager *pManager );
	bool CompileMML( const wchar_t *pMML );
	virtual bool Tick( DWORD dwTime );
	void Play( DWORD dwTime );

private:

	typedef struct tagSOUNDSET {
		tagSOUNDSET(){
			pSoundSet = NULL;
			pGen = NULL;
		}

		SoundEffectSet	*pSoundSet;
		EffectGen		*pGen;
		SoundEffectADSR	*pADSR;
		SoundEffectVolume *pVolume;
	}SOUNDSET;

	enum eCMD {
		 CMD_TEMPO
		,CMD_END
		,CMD_NOTE_ON
		,CMD_NOTE_OFF
		,CMD_PROGRAM_CHANGE
		,CMD_ADSR
		,CMD_VOLUME
	};
	typedef struct tagADSRPARAM{
		float aPower;
		float aTime;
		float dTime;
		float sPower;
		float rTime;
	}ADSRPARAM;
	typedef struct tagTOKEN{
		eCMD		command;
		BYTE		track;
		DWORD		param;
		ADSRPARAM	paramADSR;
		DWORD		gateTick;
	}TOKEN;

	const wchar_t *CompilePhase1( const wchar_t *pSource );
	std::vector<TOKEN> CompilePhase2( const wchar_t *pSource );
	DWORD GetNoteTick( const wchar_t *pNoteSize, DWORD defaultTick, const wchar_t **ppExit ) const;
	DWORD GetNumber( const wchar_t *pString, const wchar_t **ppExit ) const;
	bool  GetNote( const wchar_t *pString, DWORD defaultTick, char *pNote, DWORD *pGateTime, const wchar_t **ppExit ) const;
	DWORD GetTempoToTick( DWORD tempo ) const;
	std::vector<std::wstring> GetParams( const wchar_t *pSource, const wchar_t **ppExit ) const;
	DWORD PlaySeq( DWORD index );
	void  Release();
	float GetFreq( BYTE note ) const;

	SOUNDSET				m_holder[MAX_TRACK];
	SoundManager			*m_pManager;
	std::vector<TOKEN>		m_sequence;			// シーケンスデータ
	DWORD					m_playIndex;		// 現在のシーケンス再生インデックス
	DWORD					m_totalTick;		// 再生開始から現在までのTick値
	DWORD					m_nextTick;			// 次のSequenceを実行するTick
	DWORD					m_tickParSec;		// １秒間のTick値
	DWORD					m_startTime;		// 再生開始時間(ms)
};
