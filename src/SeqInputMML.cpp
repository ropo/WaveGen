#include "stdafx.h"

#define TICKCOUNT	3840		// １小節の分解度 3840=256分音符長の三連符まで可
#define ERR(x){	errorCode=(x); goto err; }

SeqInputMML::SeqInputMML()
	: m_pManager( nullptr )
	, m_isStop( true )
	, m_fncPlayFinidhed( nullptr )
{
}

SeqInputMML::~SeqInputMML()
{
	Release();
}

void SeqInputMML::Release()
{
	m_sequence.clear();
}

bool SeqInputMML::Init( SoundManager *pManager )
{
	if( pManager == nullptr )
		return true;

	m_pManager = pManager;

	// サウンドセットを作成&登録
	for( int i=0; i<MAX_TRACK; i++ ) {
		SOUNDSET ss;
		ss.pSoundSet = new SoundEffectSet();
		ss.pGen		 = new EffectGen( 220, EffectGen::SILENT );
		ss.pVibrato  = new SoundEffectVibrato();
		ss.pVibrato->SetEffectGen( ss.pGen );
		ss.pSoundSet->Push( ss.pVibrato );
		ss.pADSR	 = new SoundEffectADSR();						// ADSR エフェクタを連結
		ss.pSoundSet->Push( ss.pADSR );									// …をサウンドセットにつっこむ
		ss.pVolume	 = new SoundEffectVolume();						// Volume エフェクタを連結
		ss.pSoundSet->Push( ss.pVolume );								// …をサウンドセットにつっこむ
		m_pManager->Push( ss.pSoundSet );
		m_holder[i] = ss;
	}

	m_playIndex = 0;

	return false;
}

// 指定ノート番号の周波者数を取得
float SeqInputMML::GetFreq( BYTE note )
{
	double freq = 28160;
	const double equalTempera = pow( 2, 1./12. );
	for( int i=129-(note&0x7f); i; i-- )
		freq /= equalTempera;
	return (float)freq;
}

DWORD SeqInputMML::PlaySeq( DWORD index )
{
	CriticalBlock cb( &m_pManager->m_cs );

	if( index >= m_sequence.size() )
		return 0;

	TOKEN token = m_sequence[ index ];
	if( token.track >= MAX_TRACK )
		return 0;

	SOUNDSET *pSS = &m_holder[ token.track ];

	switch( token.command )
	{
		case CMD_VIBRATO:{
					pSS->isVibrato = true;
					pSS->vibratoParam = token.u1.paramVibrato;
				}break;
		case CMD_NOTE_ON:{
					if( pSS->programNo == EffectGen::FCNOISE_S || pSS->programNo == EffectGen::FCNOISE_L ) {
						pSS->pGen->ChangeFCNoiseFreq( token.u1.paramNoteOn.note );
					}else{
						float freq = GetFreq( token.u1.paramNoteOn.note );
						if( token.u1.paramNoteOn.sweepTime > 0 ){
							pSS->pGen->ChangeFreqSweep( freq, GetFreq( token.u1.paramNoteOn.sweepNote ), token.u1.paramNoteOn.sweepTime );
						}else if( pSS->isVibrato ){
							pSS->pVibrato->ChangeParam( freq, pSS->vibratoParam.delayTime	, pSS->vibratoParam.sPower
															, pSS->vibratoParam.hz			, pSS->vibratoParam.aPower
															, pSS->vibratoParam.aTime		, pSS->vibratoParam.dTime
															, pSS->vibratoParam.sTime		, pSS->vibratoParam.rTime );
						}else{
							pSS->pGen->ChangeFreq( freq );
						}
					}
					pSS->pADSR->NoteOn();
				}break;
		case CMD_NOTE_OFF:
					pSS->pADSR->NoteOff();
				break;
		case CMD_PROGRAM_CHANGE: {
					EffectGen::eTYPE pg;
					switch( token.param ) {
						default:
						case 0:	pg = EffectGen::SQUARE;		break;
						case 1:	pg = EffectGen::TRIANGLE;	break;
						case 2:	pg = EffectGen::SAW;		break;
						case 3:	pg = EffectGen::SINEWAVE;	break;
						case 4:	pg = EffectGen::FCNOISE_L;	break;
						case 5:	pg = EffectGen::FCNOISE_S;	break;
					}
					pSS->pGen->ChangeType( pg );
					pSS->programNo = pg;
				}break;
		case CMD_DUTY_CHANGE: {
					pSS->pGen->ChangeSquareDuty( token.param / 1000.0f );
				}break;
		case CMD_TEMPO:
					m_tickParSec = token.param;
				break;
		case CMD_ADSR:
					pSS->pADSR->ChangeParam(  token.u1.paramADSR.aPower
											, token.u1.paramADSR.aTime
											, token.u1.paramADSR.dTime
											, token.u1.paramADSR.sPower
											, token.u1.paramADSR.rTime );
				break;
		case CMD_VOLUME:
					pSS->pVolume->ChangeVolume( token.param/381.0f );
				break;
	}

	return token.gateTick;
}

bool SeqInputMML::Tick( DWORD dwTime )
{
	if( m_isStop )
		return true;
	if( m_pManager == nullptr )
		return true;
	if( m_sequence.size() <= m_playIndex )
		return true;

	DWORD totalTick = (dwTime - m_startTime) * m_tickParSec / 1000;
	while( m_nextTick <= totalTick ) {
		if( m_sequence.size() <= m_playIndex ) {
			m_isStop = true;
			if( m_fncPlayFinidhed )
				m_fncPlayFinidhed( m_playFinishedParam );
			return true;
		}


		if( m_nextTick > totalTick )
			return false;

		m_nextTick += PlaySeq( m_playIndex++ );
	}
	return false;
}

// 再生
void SeqInputMML::Play( DWORD dwTime, void(*playFinished)(void*), void *pPlayFinishedParam )
{
	m_playFinishedParam = pPlayFinishedParam;
	m_fncPlayFinidhed = playFinished;

	m_startTime = dwTime;
	m_totalTick = 0;
	m_playIndex = 0;
	m_nextTick = 0;
	m_isStop = false;

	Tick( dwTime );
}

// 停止
void SeqInputMML::Stop()
{
	m_isStop = true;

	// ノートオフ
	CriticalBlock cb( &m_pManager->m_cs );
	for( int i=0; i<MAX_TRACK; i++ ) {
		m_holder[i].pADSR->NoteOff();
	}
}

// MMLをコンパイルする
SeqInputMML::COMPILEDINFO SeqInputMML::CompileMML( const wchar_t *pMML )
{
	COMPILEDINFO compiledInfo;
	compiledInfo.errorCode = 0;

	// フェーズ１
	const wchar_t *pPreCompiled = CompilePhase1( pMML, &compiledInfo.errorCode, &compiledInfo.errorLine );
	if( compiledInfo.errorCode != 0 )
		return std::move( compiledInfo );

	// フェーズ２
	m_sequence = CompilePhase2( pPreCompiled, &compiledInfo.errorCode, &compiledInfo.errorLine );
	delete pPreCompiled;
	if( compiledInfo.errorCode != 0 )
		return std::move( compiledInfo );

	// フェーズ３
	m_sequence = CompilePhase3( std::move( m_sequence ) );

	// Tick数を数える
	compiledInfo.maxTickCount = 0;
	for( std::vector<TOKEN>::iterator itr=m_sequence.begin(); itr!=m_sequence.end(); ++itr )
		compiledInfo.maxTickCount += itr->gateTick;
	m_compiledInfo = compiledInfo;

	return std::move( compiledInfo );
}

// 数字を返す
DWORD SeqInputMML::GetNumber( const wchar_t *pString, const wchar_t **ppExit ) const
{
	DWORD num = 0;
	while( *pString>='0' && *pString<='9' ) {
		num *= 10;
		num += *pString - '0';
		pString++;
	}
	if( ppExit )
		*ppExit = pString;
	return num;
}

// 音長を取得 -1 は負数
bool SeqInputMML::GetNote( const wchar_t *pString, DWORD defaultTick, char *pNote, DWORD *pGateTime, const wchar_t **ppExit ) const
{
	int note = -1;
	if( *pString>='a' && *pString<='g' ) {
		static const int tbl[7] = {9,11,0,2,4,5,7};
		note = tbl[*pString-'a'];
		pString++;
		if( *pString == '+' ) {
			note++;
			pString++;
			if( note == 12 )
				note = 0;
		} else if( *pString == '-' ) {
			note--;
			pString++;
			if( note == -1 )
				note = 11;
		}
		if( note < 0 || note > 11 )
			return true;
	}else if( *pString >= 'r' ) {
		note = -1;
		pString++;
	}

	DWORD gateTime = GetNoteTick( pString, defaultTick, &pString );

	if( pNote )
		*pNote = (char)note;
	if( pGateTime )
		*pGateTime = gateTime;
	if( ppExit )
		*ppExit = pString;

	return false;
}

// 音長文字からTICK値に変換 ※付点対応
DWORD SeqInputMML::GetNoteTick( const wchar_t *pNoteSize, DWORD defaultTick, const wchar_t **ppExit ) const
{
	DWORD noteBase = GetNumber( pNoteSize, &pNoteSize );
	if( noteBase != 0 )
		defaultTick = TICKCOUNT / noteBase;
	noteBase = defaultTick;
	while( *pNoteSize=='.' ) {
		defaultTick /= 2;
		noteBase += defaultTick;
		pNoteSize++;
	}

	if( ppExit )
		*ppExit = pNoteSize;

	return noteBase;
}

// テンポから１秒間のTICK値に変換
DWORD SeqInputMML::GetTempoToTick( DWORD tempo ) const
{
	return TICKCOUNT * tempo / 240;
}

// コンパイルフェーズ１：文字整形
const wchar_t *SeqInputMML::CompilePhase1( const wchar_t *pSource, int *pErrorCode, DWORD *pErrorLine ) const
{
	bool isLineComment = false;
	bool isBlockComment = false;;
	int loopCount = 0;
	int parenCount = 0;
	wchar_t *pBuffer = new wchar_t[ wcslen(pSource)+1 ];
	wchar_t *w = pBuffer;
	int errorCode;
	DWORD dwLineCount = 1;


	// 改行の数を数えつつ、改行コードを \n に統一する
	while( *pSource ) {
		if( *pSource=='\r' ) {
			pSource++;
			if( *pSource=='\n' )
				pSource++;
			dwLineCount++;
			*(w++)='\n';
		}else if( *pSource=='\n' ) {
			pSource++;
			if( *pSource=='\r' )
				pSource++;
			dwLineCount++;
			*(w++)='\n';
		}else{
			*(w++) = *(pSource++);
		}
	}
	*w = '\0';

	wchar_t *pWork = new wchar_t[ wcslen(pBuffer)+sizeof(DWORD)*dwLineCount+1 ];
	wmemcpy( pWork, pBuffer, wcslen(pBuffer)+sizeof(wchar_t) );

	pSource = pBuffer;
	w = pWork;
	dwLineCount = 1;
	while( *pSource ) {
		// 改行？
		if( *pSource=='\n' ) {
			isLineComment = false;
			*(w++) = *(pSource++);
			*(DWORD*)w = ++dwLineCount;
			w += 2;
			continue;
		}

		// ラインコメント？
		if( wcsncmp( pSource, L"//", 2 ) == 0 ) {
			pSource += 2;
			isLineComment = true;
			continue;
		}

		// ラインコメント中でなければ
		if( isLineComment==false ) {
			// ブロックコメント終了？
			if(  wcsncmp( pSource, L"*/", 2 ) == 0 ) {
				pSource += 2;
				if( isBlockComment )	isBlockComment = false;
				else					ERR(15);
				continue;
			}
			// ブロックコメント開始？
			if( wcsncmp( pSource, L"/*", 2 ) == 0 ) {
				pSource += 2;
				isBlockComment = true;
				continue;
			}
		}

		// 以下コメント中なら何もしない
		if( isBlockComment || isLineComment ) {
			pSource++;
			continue;
		}

		// 空白は詰める
		if( *pSource == ' ' || *pSource == '\t' ) {
			pSource++;
			continue;
		}
		// S-JIS全角スペースも詰める
		if( pSource[0]==129 && pSource[1]==64 ) {
			pSource+=2;
			continue;
		}
		// ループブロック数のチェック
		if( *pSource == '[' )
			loopCount++;
		if( *pSource == ']' ) {
			// 開始より終了が多い
			if( loopCount == 0 )
				ERR(16);
			loopCount--;
		}
		// 丸かっこ数のチェック
		if( *pSource == '(' )
			parenCount++;
		if( *pSource == ')' ) {
			// 開始より終了が多い
			if( parenCount == 0 )
				ERR(17);
			parenCount--;
		}

		// 大文字は小文字に
		if( *pSource>='A' && *pSource<='Z' )
			*(w++) = *(pSource++) | 0x20;
		else			
			*(w++) = *(pSource++);
	}
	// ループが閉じられていない
	if( loopCount != 0 )
		ERR(14);
	// 丸カッコが閉じられていない
	if( parenCount != 0 )
		ERR(18);

	*w = '\0';
	delete pBuffer;

	return pWork;

err:
	*pErrorCode = errorCode;
	*pErrorLine = dwLineCount;
	delete pWork;
	delete pBuffer;
	return nullptr;
}

// 丸カッコ閉じまでの文字列を取得
std::vector<std::wstring> SeqInputMML::GetParams( const wchar_t *pSource, const wchar_t **ppExit ) const
{
	std::vector<std::wstring> params;
	const wchar_t *pEnd = pSource;
	while( *pEnd != ')' ) {
		if( *pEnd==',' ) {
			params.push_back( std::wstring( pSource, pEnd ) );
			pEnd++;
			pSource = pEnd;
		}else{
			pEnd++;
		}
	}
	if( pSource != pEnd ) {
		params.push_back( std::wstring( pSource, pEnd ) );
	}

	if( ppExit )
		*ppExit = pEnd+1;

	return std::move( params );
}

// コンパイルフェーズ２：トークン生成
std::vector<SeqInputMML::TOKEN> SeqInputMML::CompilePhase2( const wchar_t *pSource, int *pErrorCode, DWORD *pErrorLine ) const
{
	// コンパイル中に必要なワーク構造体
	typedef struct tagTOKENWORK : TOKEN {
		DWORD		startTick;
		DWORD		index;
	}TOKENWORK;

	// ループブロック情報
	typedef struct tagLOOPINFO {
		const wchar_t *pStart;
		const wchar_t *pEscape;
		const wchar_t *pEnd;
		DWORD loopCount;
	}LOOPINFO;
	std::stack<LOOPINFO> loopStack;

	// トラック情報
	typedef struct tagTRACKWORK {
		std::vector<TOKENWORK> tokens;
	}TRACKWORK;

	TOKENWORK token;
	std::vector<TOKENWORK> resultTokens, *pTokens;
	std::vector<TOKEN> result;
	TRACKWORK trackWork[MAX_TRACK];
	char	onceOctave = 0;
	bool	isSweep = false;
	DWORD	dwLineCount = 1;
	int		errorCode = 0;
	size_t	emptyTrackCount = 0;

	// 省略可能な値をすべて設定しておく
	int		currentOctave = 5;
	int		currentTrack = 0;
	DWORD	defaultTick = TICKCOUNT / 4;
	int		noteOnGate = 80;

	for( int i=0; i<MAX_TRACK; i++ ) {
		// 音色
		token.command = CMD_PROGRAM_CHANGE;
		token.param = 0;
		token.gateTick = 0;
		trackWork[i].tokens.push_back( token );

		// 音量
		token.command = CMD_VOLUME;
		token.param = 100;
		token.gateTick = 0;
		trackWork[i].tokens.push_back( token );
	}
	emptyTrackCount = trackWork[0].tokens.size();

	// テンポは
	pTokens = &trackWork[ currentTrack ].tokens;
	token.command = CMD_TEMPO;
	token.param = GetTempoToTick( 128 );
	token.gateTick = 0;
	pTokens->push_back( token );

	while( *pSource ) {
		token.gateTick = 0;

		// 改行コード
		if( *pSource == '\n' ) {
			pSource++;
			dwLineCount = *(DWORD*)pSource;
			pSource += 2;
			continue;
		}

		// ADSRエフェクト指定？
		if( wcsncmp( pSource, L"adsr(", 5 ) == 0 ) {
			std::vector<std::wstring> params = GetParams( pSource+5, &pSource );
			if( params.size() != 5 )
				ERR(1)	// ADSR引数の数が不正
			token.command = CMD_ADSR;
			token.gateTick = 0;
			token.u1.paramADSR.aPower = (float)_wtof( params[0].c_str() );
			token.u1.paramADSR.aTime  = (float)_wtof( params[1].c_str() );
			token.u1.paramADSR.dTime  = (float)_wtof( params[2].c_str() );
			token.u1.paramADSR.sPower = (float)_wtof( params[3].c_str() );
			token.u1.paramADSR.rTime  = (float)_wtof( params[4].c_str() );
			pTokens->push_back( token );
			continue;
		}

		// Vibratoエフェクト指定？
		if( wcsncmp( pSource, L"vibrato(", 8 ) == 0 ) {
			std::vector<std::wstring> params = GetParams( pSource+8, &pSource );
			if( params.size() != 8 ) 
					ERR(20)	// VIBRATO引数の数が不正
			token.command = CMD_VIBRATO;
			token.gateTick = 0;
			token.u1.paramVibrato.delayTime	= (float)_wtof( params[0].c_str() );
			token.u1.paramVibrato.hz		= (float)_wtof( params[1].c_str() );
			token.u1.paramVibrato.aPower	= (float)_wtof( params[2].c_str() );
			token.u1.paramVibrato.aTime		= (float)_wtof( params[3].c_str() );
			token.u1.paramVibrato.dTime		= (float)_wtof( params[4].c_str() );
			token.u1.paramVibrato.sPower	= (float)_wtof( params[5].c_str() );
			token.u1.paramVibrato.sTime		= (float)_wtof( params[6].c_str() );
			token.u1.paramVibrato.rTime		= (float)_wtof( params[7].c_str() );

			// hz が 0 もしくは Time がすべて 0 なら解除
			if( token.u1.paramVibrato.hz == 0 || (token.u1.paramVibrato.aTime == 0 && token.u1.paramVibrato.dTime == 0 && token.u1.paramVibrato.sTime == 0 && token.u1.paramVibrato.rTime == 0 ) )
			{
				token.u1.paramVibrato.delayTime	= 0;
				token.u1.paramVibrato.hz		= 0;
				token.u1.paramVibrato.aPower	= 0;
				token.u1.paramVibrato.aTime		= 0;
				token.u1.paramVibrato.dTime		= 0;
				token.u1.paramVibrato.sPower	= 0;
				token.u1.paramVibrato.sTime		= 0;
				token.u1.paramVibrato.rTime		= 0;
			}

			pTokens->push_back( token );
			continue;
		}

		// トラックチェンジ？
		if( wcsncmp( pSource, L"track(", 6 ) == 0 ) {
			if( !loopStack.empty() )
				ERR(2)
			if( isSweep )
				ERR(3)
			std::vector<std::wstring> params = GetParams( pSource+6, &pSource );
			if( params.size() != 1 )
				ERR(4)
			currentTrack = _wtoi( params[0].c_str() ) % MAX_TRACK;
			pTokens = &trackWork[ currentTrack ].tokens;
			continue;
		}

		switch( *pSource ) {
			case 'o':{	const wchar_t *pStart = pSource+1;
						currentOctave = GetNumber( pStart, &pSource );
						if( pStart == pSource || currentOctave>8 )
							ERR(5)
					}break;
			case 'l':{	const wchar_t *pStart = pSource+1;
						defaultTick = GetNoteTick( pStart, defaultTick, &pSource );
						if( pStart == pSource )
							ERR(6)
					}break;
			case 't':	token.command = CMD_TEMPO;
						token.param = GetTempoToTick( GetNumber( pSource+1, &pSource ) );
						if( token.param == 0 )
							ERR(7)
						pTokens->push_back( token );
					break;
			case '<':	currentOctave--;
						if( currentOctave < 0 )
							currentOctave = 0;
						pSource++;
					break;
			case '>':	currentOctave++;
						if( currentOctave > 8 )
							currentOctave = 8;
						pSource++;
					break;
			case 'q':	noteOnGate = GetNumber( pSource+1, &pSource );
						if( noteOnGate < 0 )		noteOnGate = 0;
						else if( noteOnGate > 100 )	noteOnGate = 100;
					break;
			case '@':	if( pSource[1] == 'w' ) {
							token.command = CMD_DUTY_CHANGE;
							token.param = GetNumber( pSource+2, &pSource );
						}else{
							token.command = CMD_PROGRAM_CHANGE;
							token.param = GetNumber( pSource+1, &pSource );
						}
						pTokens->push_back( token );
					break;
			case 'v':	token.command = CMD_VOLUME;
						token.param = GetNumber( pSource+1, &pSource );
						pTokens->push_back( token );
					break;
			case 'c':	case 'd':	case 'e':	case 'f':
			case 'g':	case 'a':	case 'b':	case 'r':{
						char note;
						DWORD gateTime;
						if( GetNote( pSource, defaultTick, &note, &gateTime, &pSource ) )
							ERR(8)

						if( note >= 0 ) {
							if( isSweep ) {
								// スイープ
								TOKENWORK *pOff, *pOn = &(*pTokens)[pTokens->size()-1];
								size_t i;
								for( i=pTokens->size(); i; --i, --pOn ) {
									if( pOn->command == CMD_NOTE_ON )
										break;
								}
								if( i ) {
									pOff = pOn+1;
									if( pOff->command != CMD_NOTE_OFF )
										ERR(9)
									pOn->gateTick += gateTime + pOff->gateTick;

									// 長さを混ぜる
									gateTime = pOn->gateTick;
									pOn->gateTick  = gateTime * noteOnGate / 100;
									pOn->u1.paramNoteOn.sweepTime = pOn->gateTick / (float)TICKCOUNT;
									pOff->gateTick = gateTime - pOn->gateTick;

									if( pOn->u1.paramNoteOn.note != note ) {
										// スラー
										pOn->u1.paramNoteOn.sweepNote = (BYTE)MinMax( (onceOctave + currentOctave) * 12 + note, 0, 127 );
										pOn->u1.paramNoteOn.sweepTime = pOn->gateTick / (float)TICKCOUNT;
										if( pOn->u1.paramNoteOn.sweepNote == pOn->u1.paramNoteOn.note )
											pOn->u1.paramNoteOn.sweepTime = 0;	// 巡り巡って起点と同じ音階ならタイになる
									}
								}
							}else{
								token.command = CMD_NOTE_ON;
								token.u1.paramNoteOn.note = (BYTE)MinMax( (onceOctave + currentOctave) * 12 + note, 0, 127 );
								token.gateTick = gateTime * noteOnGate / 100;
								token.u1.paramNoteOn.sweepTime = 0;
								pTokens->push_back( token );

								token.command = CMD_NOTE_OFF;
								token.gateTick = gateTime - token.gateTick;
								pTokens->push_back( token );
							}

							if( *pSource == '&' ) {
								isSweep = true;
								pSource++;
							}else{
								isSweep = false;
							}
						}else{
							if( isSweep )
								ERR(10)
							token.command = CMD_NOTE_REST;
							token.param = 1000;
							token.gateTick = gateTime;
							pTokens->push_back( token );
						}
					}break;
			case '`':	onceOctave = 1;
						pSource++;
					continue;
			case '"':	onceOctave = -1;
						pSource++;
					continue;
			case '[':{	LOOPINFO loopInfo;
						loopInfo.loopCount = GetNumber( pSource+1, &pSource );
						if( loopInfo.loopCount < 2 )
							loopInfo.loopCount = 2;	// 最低でも２周する
						loopInfo.pStart = pSource;
						loopInfo.pEscape = nullptr;
						loopInfo.pEnd = nullptr;
						loopStack.push( loopInfo );
					}break;
			case ':':{	if( loopStack.empty() )
							ERR(11)
						LOOPINFO loopInfo = loopStack.top();
						if( loopInfo.loopCount == 1 ) {
							loopStack.pop();
							pSource = loopInfo.pEnd;
						}else{
							pSource++;
							if( loopInfo.pEscape == nullptr ) {
								loopStack.pop();
								loopInfo.pEscape = pSource;
								loopStack.push( loopInfo );
							}else if( loopInfo.pEscape != pSource )
								ERR(12)
						}
					}break;
			case ']':{	if( loopStack.empty() )
							ERR(13)
						LOOPINFO loopInfo = loopStack.top();
						loopStack.pop();
						loopInfo.loopCount--;
						if( loopInfo.loopCount == 0 ) {
							// ループ終了
							pSource++;
						}else{
							loopInfo.pEnd = pSource+1;
							pSource = loopInfo.pStart;
							loopStack.push( loopInfo );
						}
					}break;
			default:	ERR(19)
		}
		onceOctave = 0;
	}
	if( !loopStack.empty() )
		ERR(14)

	// トラックごとに配置インデックスと再生開始時間(Tick)を書き込み１本のトラック情報にマージ
	DWORD tokenIndex = 0;
	for( BYTE i=0; i<MAX_TRACK; i++ ) {
		DWORD dwTotalTime = 0;
		if( trackWork[i].tokens.size() > emptyTrackCount ) {
			for( std::vector<tagTOKENWORK>::iterator itr=trackWork[i].tokens.begin(); itr!=trackWork[i].tokens.end(); ++itr ) {
				itr->startTick = dwTotalTime;
				itr->track = i;
				itr->index = tokenIndex++;
				dwTotalTime += itr->gateTick;
				resultTokens.push_back( *itr );
			}
		}
	}

	// マージされたトラックから再生開始時間でソート
	struct LessToken {
		bool operator()(const tagTOKENWORK& riLeft, const tagTOKENWORK& riRight) const {
			if( riLeft.startTick == riRight.startTick )	// 同じ再生時間なら速く定義された方
				return riLeft.index < riRight.index;
			return riLeft.startTick < riRight.startTick;
		}
	};
	std::sort( resultTokens.begin(), resultTokens.end(), LessToken() );

	// 前後の再生コマンド間の時間を再計算し、不要なワークを取り除いた配列に入れなおす
	size_t count = resultTokens.size()-1;
	for( size_t i=0; i<count; i++ ) {
		resultTokens[i].gateTick = resultTokens[i+1].startTick - resultTokens[i].startTick;
		result.push_back( resultTokens[i] );
	}
	result.push_back( resultTokens[resultTokens.size()-1] );

	// 終端マーカを入れる
	token.command = CMD_END;
	token.gateTick = 0;
	result.push_back( token );

	*pErrorCode = 0;
	*pErrorLine = 0;
	return std::move( result );

err:
	*pErrorCode = errorCode;
	*pErrorLine = dwLineCount;

	result.clear();
	return std::move( result );
}

// コンパイルフェーズ３：最適化
std::vector<SeqInputMML::TOKEN> SeqInputMML::CompilePhase3( std::vector<TOKEN> tokens ) const
{
	{
		TOKEN dummyToken;
		dummyToken.command = (eCMD)-1;
		tokens.push_back( dummyToken );
	}
	std::vector<TOKEN> result;
	TOKEN *pToken = &tokens[0], *pPrevToken = nullptr;
	for( size_t i=tokens.size(); i>0; i--, pToken++ ) {
		if( pPrevToken ) {
			// 前後が同じトラックで休符ならゲート時間を混ぜる
			if( pPrevToken->track == pToken->track
			 && pToken->command == CMD_NOTE_OFF
			 && pPrevToken->command == CMD_NOTE_OFF )
			{
				pPrevToken->gateTick += pToken->gateTick;
			}else{
				result.push_back( *pPrevToken );
				pPrevToken = pToken;
			}
		}else{
			pPrevToken = pToken;
		}
	}

	return result;
}

const wchar_t* SeqInputMML::GetErrorString( int errorCode )
{
	switch( errorCode )
	{
		case 0:	return L"エラーなし";
		case 1:	return L"ADSRの引数が無効";
		case 2:	return L"ループブロック( [ )が閉じられずにトラックを変更している";
		case 3: return L"スイープ( & )中にトラックを変更している";
		case 4: return L"トラック変更の引数が不正";
		case 5: return L"オクターブ( o )引数が不正 0-8";
		case 6: return L"省略時の音調指定( l )が不正";
		case 7: return L"テンポ指定( t )が不正 0-8";
		case 8: return L"音階取得エラー";
		case 9: return L"スイープ解析エラー";
		case 10:return L"スイープ( & )後に休符( r )が指定されている";
		case 11:return L"ループブロック外に、ループエスケープマーク( : )がある";
		case 12:return L"ループブロック( [ ) 中に、エスケープマーク( : )が複数ある";
		case 13:return L"ループブロック( [ )がないのにループ終端マーク( ] )がある";
		case 14:return L"ループブロック( [ )が閉じられていない";
		case 15:return L"始まりのないコメントブロック終端がある( */ )";
		case 16:return L"始まりのないループブロック終端がある( ] )";
		case 17:return L"始まりのないパラメータブロック終端がある( ) )";
		case 18:return L"パラメータブロック( ( )が閉じられていない";
		case 19:return L"不明な命令があります";
		case 20:return L"VIBRATOの引数が無効";
	}
	return L"不明なエラー";
}
