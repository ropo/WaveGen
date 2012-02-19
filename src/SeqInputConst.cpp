#include "stdafx.h"


SeqInputConst::SeqInputConst()
	: m_pCurSeqData( NULL )
	, m_pSeqDataRoot( NULL )
	, m_pManager( NULL )
{
}

SeqInputConst::~SeqInputConst()
{
	Release();
}

void SeqInputConst::Release()
{
	SEQDATA *pSeqData = m_pSeqDataRoot;
	while( pSeqData ) {
		SEQDATA *pSeqNext = pSeqData->pNext;
		delete pSeqData;
		pSeqData = pSeqNext;
	}
}

bool SeqInputConst::Init( SoundManager *pManager )
{
	if( pManager == NULL )
		return true;

	m_pManager = pManager;

	// サウンドセットを作成&登録
	SOUNDSET ss;
	ss.pSoundSet = new SoundEffectSet();
	ss.pGen		 = new EffectGen( 220, EffectGen::SILENT );
	ss.pSoundSet->Push( ss.pGen );
	m_pManager->Push( ss.pSoundSet );

	m_holder = ss;

	// 適当な譜面を作成
	m_pSeqDataRoot = m_pCurSeqData = new SEQDATA;
	m_pSeqDataRoot->note = 48;	// ド
	m_pSeqDataRoot->gateTime = 1000;

	SEQDATA *pSeqCur = m_pSeqDataRoot;
	{
		SEQDATA *pSeq = new SEQDATA;
		pSeq->note = 50;		// レ
		pSeq->gateTime = 1000;
		pSeqCur->PushNext( pSeq );
		pSeqCur = pSeq;
	}
	{
		SEQDATA *pSeq = new SEQDATA;
		pSeq->note = 52;		// ミ
		pSeq->gateTime = 1000;
		pSeqCur->PushNext( pSeq );
		pSeqCur = pSeq;
	}
	{
		SEQDATA *pSeq = new SEQDATA;
		pSeq->note = 255;
		pSeq->gateTime = 0;
		pSeqCur->PushNext( pSeq );
		pSeqCur = pSeq;
	}

	m_nextPlayTime = timeGetTime() + m_pCurSeqData->gateTime;
	PlaySeq( m_pCurSeqData );

	return false;
}

float SeqInputConst::GetFreq( BYTE note ) const
{
	if( note >= 128 )
		return 0;

	static const float freqs[127] = {
			16.3516f,   17.3239f,   18.3540f,   19.4454f,   20.6017f,   21.8268f,   23.1247f,   24.4997f,   25.9565f,   27.5000f,   29.1352f,   30.8677f
		,   32.7032f,   34.6478f,   36.7081f,   38.8909f,   41.2034f,   43.6535f,   46.2493f,   48.9994f,   51.9131f,   55.0000f,   58.2705f,   61.7354f
		,   65.4064f,   69.2957f,   73.4162f,   77.7817f,   82.4069f,   87.3071f,   92.4986f,   97.9989f,  103.8262f,  110.0000f,  116.5409f,  123.4708f
		,  130.8128f,  138.5913f,  146.8324f,  155.5635f,  164.8138f,  174.6141f,  184.9972f,  195.9977f,  207.6523f,  220.0000f,  233.0819f,  246.9417f
		,  261.6256f,  277.1826f,  293.6648f,  311.1270f,  329.6276f,  349.2282f,  369.9944f,  391.9954f,  415.3047f,  440.0000f,  466.1638f,  493.8833f
		,  523.2511f,  554.3653f,  587.3295f,  622.2540f,  659.2551f,  698.4565f,  739.9888f,  783.9909f,  830.6094f,  880.0000f,  932.3275f,  987.7666f
		, 1046.5023f, 1108.7305f, 1174.6591f, 1244.5079f, 1318.5102f, 1396.9129f, 1479.9777f, 1567.9817f, 1661.2188f, 1760.0000f, 1864.6550f, 1975.5332f
		, 2093.0045f, 2217.4610f, 2349.3181f, 2489.0159f, 2637.0205f, 2793.8259f, 2959.9554f, 3135.9635f, 3322.4376f, 3520.0000f, 3729.3101f, 3951.0664f
		, 4186.0090f, 4434.9221f, 4698.6363f, 4978.0317f, 5274.0409f, 5587.6517f, 5919.9108f, 6271.9270f, 6644.8752f, 7040.0000f, 7458.6202f, 7902.1328f
		, 8372.0181f, 8869.8442f, 9397.2726f, 9956.0635f,10548.0818f,11175.3034f,11839.8215f,12543.8540f,13289.7503f,14080.0000f,14917.2404f,15804.2656f
		,16744.0362f,17739.6884f,18794.5451f,19912.1270f,21096.1636f,22350.6068f,23679.6431f
	};

	return freqs[ note ];
}

void SeqInputConst::PlaySeq( const SEQDATA *pSeq )
{
	float freq = GetFreq( pSeq->note );
	if( freq == 0 )	m_holder.pGen->ChangeType( EffectGen::SILENT );
	else {			m_holder.pGen->ChangeFreq( freq );
					m_holder.pGen->ChangeType( EffectGen::SQUARE );
	}
}

bool SeqInputConst::Tick( DWORD dwTime )
{
	if( m_holder.pSoundSet == NULL || m_pManager == NULL || m_pCurSeqData == NULL )
		return true;

	if( m_nextPlayTime > dwTime )
		return false;

	m_pCurSeqData = m_pCurSeqData->pNext;
	if( m_pCurSeqData == NULL )
		return true;

	m_nextPlayTime += m_pCurSeqData->gateTime;
	PlaySeq( m_pCurSeqData );

	return false;
}

