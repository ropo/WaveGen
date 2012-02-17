#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <vector>
#include <conio.h>

#include <dsound.h> 
#pragma comment ( lib, "dsound.lib" )

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

#define BASE_FREQ	44100
