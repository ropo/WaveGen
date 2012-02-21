#pragma once

class SeqInputMML : public SeqInputBase
{
public:
	SeqInputMML();
	virtual ~SeqInputMML();

	bool Init( SoundManager *pManager );
	bool ComplieMML( const wchar_t *pMML );
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
	}SOUNDSET;

	enum eCMD {
		 CMD_TEMPO
		,CMD_END
		,CMD_OCTAVE
		,CMD_NOTE_ON
		,CMD_NOTE_OFF
		,CMD_BASELENGTH
		,CMD_PROGRAM_CHANGE
	};
	typedef struct tagTOKEN{
		eCMD	command;
		DWORD	param;
		DWORD	gateTick;
	}TOKEN;

	const wchar_t *CompliePhase1( const wchar_t *pSource );
	std::vector<TOKEN> CompliePhase2( const wchar_t *pSource );
	DWORD GetNoteTick( const wchar_t *pNoteSize, DWORD defaultTick, const wchar_t **ppExit ) const;
	DWORD GetNumber( const wchar_t *pString, const wchar_t **ppExit ) const;
	bool  GetNote( const wchar_t *pString, DWORD defaultTick, char *pNote, DWORD *pGateTime, const wchar_t **ppExit ) const;
	DWORD GetTempoToTick( DWORD tempo ) const;
	DWORD PlaySeq( DWORD index );
	void  Release();
	float GetFreq( BYTE note ) const;

	SOUNDSET				m_holder;
	SoundManager			*m_pManager;
	std::vector<TOKEN>		m_sequence;			// �V�[�P���X�f�[�^
	DWORD					m_playIndex;		// ���݂̃V�[�P���X�Đ��C���f�b�N�X
	DWORD					m_totalTick;		// �Đ��J�n���猻�݂܂ł�Tick�l
	DWORD					m_nextTick;			// ����Sequence�����s����Tick
	DWORD					m_tickParSec;		// �P�b�Ԃ�Tick�l
	DWORD					m_startTime;		// �Đ��J�n����(ms)

	BYTE					m_curProgram;		// ���݂̉��F
};
