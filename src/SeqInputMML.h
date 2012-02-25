#pragma once

#define MAX_TRACK	16

class SeqInputMML : public SeqInputBase
{
public:
	SeqInputMML();
	virtual ~SeqInputMML();

	bool Init( SoundManager *pManager );
	bool CompileMML( const wchar_t *pMML, int *pErrorCode, DWORD *pErrorLine );
	static const wchar_t* GetErrorString( int errorCode );
	virtual bool Tick( DWORD dwTime );
	void Play( DWORD dwTime );

private:

	typedef struct tagSOUNDSET {
		tagSOUNDSET(){
			pSoundSet = NULL;
			pGen = NULL;
			pADSR = NULL;
			pVolume = NULL;
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
		,CMD_NOTE_REST
		,CMD_PROGRAM_CHANGE
		,CMD_DUTY_CHANGE
		,CMD_ADSR
		,CMD_VOLUME
	};
	typedef struct tagTOKEN{
		eCMD		command;		// コマンド
		BYTE		track;			// 対象トラック番号
		DWORD		param;			// 汎用パラメータ
		union {
			struct tagADSR{			// CMD_ADSR 用パラメータ
				float aPower;
				float aTime;
				float dTime;
				float sPower;
				float rTime;
			}paramADSR;
			struct tagNOTEON{		// CMD_NOTE_ON 用パラメータ
				BYTE	note;
				BYTE	sweepNote;
				float	sweepTime;
			}paramNoteOn;
		}u1;
		DWORD		gateTick;		// ゲート時間(tick数)
	}TOKEN;

	const wchar_t *CompilePhase1( const wchar_t *pSource, int *pErrorCode, DWORD *pErrorLine ) const;
	std::vector<TOKEN> CompilePhase2( const wchar_t *pSource, int *pErrorCode, DWORD *pErrorLine ) const;
	std::vector<TOKEN> CompilePhase3( std::vector<TOKEN> tokens ) const;
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
