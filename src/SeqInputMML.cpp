#include "stdafx.h"

#define TICKCOUNT	3840		// １小節の分解度 3840=256分音符長の三連符まで可

SeqInputMML::SeqInputMML()
	: m_pManager( NULL )
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
	if( pManager == NULL )
		return true;

	m_pManager = pManager;

	// サウンドセットを作成&登録
	for( int i=0; i<MAX_TRACK; i++ ) {
		SOUNDSET ss;
		ss.pSoundSet = new SoundEffectSet();
		ss.pGen		 = new EffectGen( 220, EffectGen::SILENT );
		ss.pSoundSet->Push( ss.pGen );
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
float SeqInputMML::GetFreq( BYTE note ) const
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

DWORD SeqInputMML::PlaySeq( DWORD index )
{
	if( index >= m_sequence.size() )
		return 0;

	TOKEN token = m_sequence[ index ];
	if( token.track >= MAX_TRACK )
		return 0;

	SOUNDSET *pSS = &m_holder[ token.track ];

	switch( token.command )
	{
		case CMD_NOTE_ON:
					if( token.u1.paramNoteOn.sweepTime > 0 )
						pSS->pGen->ChangeFreqSweep( GetFreq( token.u1.paramNoteOn.note ), GetFreq( token.u1.paramNoteOn.sweepNote ), token.u1.paramNoteOn.sweepTime );
					else
						pSS->pGen->ChangeFreq( GetFreq( token.u1.paramNoteOn.note ) );
					pSS->pADSR->NoteOn();
				break;
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
						case 4:	pg = EffectGen::NOISE;		break;
					}
					pSS->pGen->ChangeType( pg );
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
					pSS->pVolume->ChangeVolume( token.param/127.0f );
				break;
	}

	return token.gateTick;
}

bool SeqInputMML::Tick( DWORD dwTime )
{
	if( m_pManager == NULL )
		return true;
	if( m_sequence.size() <= m_playIndex )
		return true;

	DWORD totalTick = (dwTime - m_startTime) * m_tickParSec / 1000;
	while( m_nextTick <= totalTick ) {
		if( m_sequence.size() <= m_playIndex )
			return true;

		if( m_nextTick > totalTick )
			return false;

		m_nextTick += PlaySeq( m_playIndex++ );
	}
	return false;
}

// 再生
void SeqInputMML::Play( DWORD dwTime )
{
	m_startTime = dwTime;
	m_totalTick = 0;
	m_playIndex = 0;
	m_nextTick = 0;

	Tick( dwTime );
}

// MMLをコンパイルする
bool SeqInputMML::CompileMML( const wchar_t *pMML )
{
	// フェーズ１
	const wchar_t *pPreCompiled = CompilePhase1( pMML );
	if( pPreCompiled == NULL )
		return true;

	// フェーズ２
	m_sequence = CompilePhase2( pPreCompiled );
	delete pPreCompiled;

	// フェーズ３
	m_sequence = CompilePhase3( m_sequence );


	return false;
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
const wchar_t *SeqInputMML::CompilePhase1( const wchar_t *pSource ) const
{
	wchar_t *pBuffer = new wchar_t[ wcslen(pSource) ];
	wchar_t *w = pBuffer;

	bool isLineComment = false;
	bool isBlockComment = false;;
	int loopCount = 0;
	int parenCount = 0;

	while( *pSource ) {
		// 改行？
		if( pSource[0]=='\r' || pSource[0]=='\n' ) {
			isLineComment = false;
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
				else					goto err;	// 始まる前に終わっていた
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

		// 空白や改行は詰める
		if( *pSource == ' ' || *pSource == '\t' || pSource[0]=='\r' || pSource[0]=='\n' ) {
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
				goto err;
			loopCount--;
		}
		// 丸かっこ数のチェック
		if( *pSource == '(' )
			parenCount++;
		if( *pSource == ')' ) {
			// 開始より終了が多い
			if( parenCount == 0 )
				goto err;
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
		goto err;
	// 丸カッコが閉じられていない
	if( parenCount != 0 )
		goto err;

	*w = '\0';

	return pBuffer;

err:
	delete pBuffer;
	return NULL;
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

	return params;
}

// コンパイルフェーズ２：トークン生成
std::vector<SeqInputMML::TOKEN> SeqInputMML::CompilePhase2( const wchar_t *pSource ) const
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
	}

	// テンポは
	pTokens = &trackWork[ currentTrack ].tokens;
	token.command = CMD_TEMPO;
	token.param = GetTempoToTick( 128 );
	token.gateTick = 0;
	pTokens->push_back( token );

	while( *pSource ) {
		token.gateTick = 0;

		// ADSRエフェクト指定？
		if( wcsncmp( pSource, L"adsr(", 5 ) == 0 ) {
			std::vector<std::wstring> params = GetParams( pSource+5, &pSource );
			if( params.size() != 5 )
				goto err;	// 引数の数が不正
			token.command = CMD_ADSR;
			token.gateTick = 0;
			token.u1.paramADSR.aPower = (float)_wtof( params[0].c_str() );
			token.u1.paramADSR.aTime  = (float)_wtof( params[1].c_str() );
			token.u1.paramADSR.dTime  = (float)_wtof( params[2].c_str() );
			token.u1.paramADSR.sPower = (float)_wtof( params[3].c_str() );
			token.u1.paramADSR.rTime  = (float)_wtof( params[4].c_str() );
			pTokens->push_back( token );
		}

		// トラックチェンジ？
		if( wcsncmp( pSource, L"track(", 6 ) == 0 ) {
			if( !loopStack.empty() )
				goto err;	// ループが完結してない
			if( isSweep )
				goto err;	// スイープ中にトラックチェンジ
			std::vector<std::wstring> params = GetParams( pSource+6, &pSource );
			if( params.size() != 1 )
				goto err;	// 引数の数が不正
			currentTrack = _wtoi( params[0].c_str() ) % MAX_TRACK;
			pTokens = &trackWork[ currentTrack ].tokens;
		}

		switch( *pSource ) {
			case 'o':	currentOctave = GetNumber( pSource+1, &pSource );
						if( currentOctave == 0 )
							goto err;	// 引数がない
					break;
			case 'l':	defaultTick = GetNoteTick( pSource+1, defaultTick, &pSource );
						if( defaultTick == 0 )
							goto err;	// 引数がない
					break;
			case 't':	token.command = CMD_TEMPO;
						token.param = GetTempoToTick( GetNumber( pSource+1, &pSource ) );
						if( token.param == 0 )
							goto err;	// 引数がない
						pTokens->push_back( token );
					break;
			case '<':	currentOctave--;
						if( currentOctave < 0 )
							currentOctave = 0;
						pSource++;
					break;
			case '>':	currentOctave++;
						if( currentOctave > 9 )
							currentOctave = 9;
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
							goto err;		// 来ることないけども一応不正エラー

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
										goto err;	// 見つけた NOTE_ON の直後が OFF じゃない(ロジックエラー)
									pOn->gateTick += gateTime + pOff->gateTick;

									// 長さを混ぜる
									gateTime = pOn->gateTick;
									pOn->gateTick  = gateTime * noteOnGate / 100;
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
								goto err;		// スイープ後に休符が来ている 
							token.command = CMD_NOTE_OFF;
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
						loopInfo.pEscape = NULL;
						loopInfo.pEnd = NULL;
						loopStack.push( loopInfo );
					}break;
			case ':':{	if( loopStack.empty() )
							goto err;	// ループブロックじゃないのにエスケープ記号がある
						LOOPINFO loopInfo = loopStack.top();
						if( loopInfo.loopCount == 1 ) {
							loopStack.pop();
							pSource = loopInfo.pEnd;
						}else{
							pSource++;
							if( loopInfo.pEscape == NULL ) {
								loopStack.pop();
								loopInfo.pEscape = pSource;
								loopStack.push( loopInfo );
							}else if( loopInfo.pEscape != pSource )
								goto err;	// １つのループブロック中に : が複数あった
						}
					}break;
			case ']':{	if( loopStack.empty() )
							goto err;	// ループブロックじゃないのに、ループ終端がある
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
			default:	pSource++;
		}
		onceOctave = 0;
	}
	if( !loopStack.empty() )
		goto err;	// ループが完結していない

	// トラックごとに配置インデックスと再生開始時間(Tick)を書き込み１本のトラック情報にマージ
	DWORD tokenIndex = 0;
	for( BYTE i=0; i<MAX_TRACK; i++ ) {
		DWORD dwTotalTime = 0;
		for( std::vector<tagTOKENWORK>::iterator itr=trackWork[i].tokens.begin(); itr!=trackWork[i].tokens.end(); ++itr ) {
			itr->startTick = dwTotalTime;
			itr->track = i;
			itr->index = tokenIndex++;
			dwTotalTime += itr->gateTick;
			resultTokens.push_back( *itr );
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

	// 最後に終端マーカを入れる
	token.command = CMD_END;
	token.gateTick = 0;
	resultTokens.push_back( token );

	return result;

err:
	result.clear();
	return result;
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
	TOKEN *pToken = &tokens[0], *pPrevToken = NULL;
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

