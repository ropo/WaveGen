#pragma once

#define MAX_TRACK	16

class SeqInputMML : public SeqInputBase
{
public:
	typedef struct tagCOMPILEDINFO {
		int		errorCode;
		DWORD	errorLine;
		DWORD	maxTickCount;
	}COMPILEDINFO;

	SeqInputMML();
	virtual ~SeqInputMML();

	bool Init( SoundManager *pManager );
	COMPILEDINFO CompileMML( const wchar_t *pMML );
	static const wchar_t* GetErrorString( int errorCode );
	virtual bool Tick( DWORD dwTime );
	static float GetFreq( BYTE note );
	void Play( DWORD dwTime, void(*playFinished)(void*)=nullptr, void *pPlayFinishedParam=nullptr );
	void Stop();

private:
	typedef struct tagPARAMVIBRATO{
		float	delayTime;
		float	sPower;
		float	aPower;
		float	aTime;
		float	dTime;
		float	sTime;
		float	rTime;
		float	hz;
	}PARAMVIBRATO;

	typedef struct tagSOUNDSET {
		tagSOUNDSET(){
			pSoundSet = NULL;
			pGen = nullptr;
			pADSR = nullptr;
			pVolume = nullptr;
			pVibrato = nullptr;
			isVibrato = false;
		}
		SoundEffectSet	*pSoundSet;
		EffectGen		*pGen;
		SoundEffectADSR	*pADSR;
		SoundEffectVolume *pVolume;
		SoundEffectVibrato *pVibrato;
		bool isVibrato;
		PARAMVIBRATO	vibratoParam;
		EffectGen::eTYPE programNo;
	}SOUNDSET;

	typedef struct tagGLOBALINFO {
		bool		isFcMode;
		SOUNDSET	holder[MAX_TRACK];
	}GLOBALINFO;

	enum eCMD {
		 CMD_TEMPO
		,CMD_END
		,CMD_NOTE_ON
		,CMD_NOTE_OFF
		,CMD_NOTE_REST
		,CMD_PROGRAM_CHANGE
		,CMD_DUTY_CHANGE
		,CMD_ADSR
		,CMD_VIBRATO
		,CMD_VOLUME
		,CMD_FCMODE
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
			PARAMVIBRATO paramVibrato;
		}u1;
		DWORD		gateTick;		// ゲート時間(tick数)
	}TOKEN;

	const wchar_t *CompilePhase1( const wchar_t *pSource, int *pErrorCode, DWORD *pErrorLine ) const;
	std::vector<TOKEN> CompilePhase2( const wchar_t *pSource, int *pErrorCode, DWORD *pErrorLine ) const;
	std::vector<TOKEN> CompilePhase3( std::vector<TOKEN> tokens ) const;
	DWORD GetNoteTick( const wchar_t *pNoteSize, DWORD defaultTick, const wchar_t **ppExit ) const;
	DWORD GetNumber( const wchar_t *pString, const wchar_t **ppExit ) const;
	bool  GetNote( const wchar_t *pString, DWORD defaultTick, char octave, char *pNote, DWORD *pGateTime, const wchar_t **ppExit ) const;
	DWORD GetTempoToTick( DWORD tempo ) const;
	std::vector<std::wstring> GetParams( const wchar_t *pSource, const wchar_t **ppExit ) const;
	DWORD PlaySeq( DWORD index );
	void  Release();

	GLOBALINFO				m_info;
	SoundManager			*m_pManager;
	std::vector<TOKEN>		m_sequence;			// シーケンスデータ
	DWORD					m_playIndex;		// 現在のシーケンス再生インデックス
	DWORD					m_totalTick;		// 再生開始から現在までのTick値
	DWORD					m_nextTick;			// 次のSequenceを実行するTick
	DWORD					m_tickParSec;		// １秒間のTick値
	DWORD					m_startTime;		// 再生開始時間(ms)
	bool					m_isStop;			// 停止中ならtrue
	COMPILEDINFO			m_compiledInfo;		// 最上にコンパイルされた情報

	void(*m_fncPlayFinidhed)( void* );			// 再生終了時にコールバックされる
	void					*m_playFinishedParam;
};
