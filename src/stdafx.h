#pragma once

#include "targetver.h"

#include <windows.h>
#include <mmsystem.h>
#pragma comment ( lib, "winmm.lib" )
#include <dsound.h> 
#pragma comment ( lib, "dsound.lib" )

#include <vector>


#include "SoundOutputBase.h"
#include "SoundOutputDS.h"
#include "SoundOutputWaveFile.h"
#include "WinBase.h"
#include "SoundManager.h"
#include "SoundEffectSet.h"
#include "SoundEffectBase.h"
#include "EffectGen.h"
#include "AppMain.h"

template<typename T> inline float MinMax( T v, T min, T max ) {
	if( v > max )	return max;
	if( v < min )	return min;
	return v;
}
#define SAFE_RELEASE(x)	{if(x){(x)->Release();(x)=NULL;}}
#define SAFE_DELETE(x)	{if(x){delete(x);(x)=NULL;}}

#define BASE_FREQ	44100
