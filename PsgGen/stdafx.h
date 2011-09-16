#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <vector>

#include <dsound.h> 
#pragma comment ( lib, "dsound.lib" )

#include "SoundManager.h"
#include "SoundEffectSet.h"
#include "SoundEffectBase.h"
#include "EffectTone.h"

template <typename T> T MAX(T a, T b){	return (a>b)?a:b;	}
template <typename T> T MIN(T a, T b){	return (a>b)?a:b;	}
#define SAFE_RELEASE(o)	if(o){(o)->Release();(o)=NULL;}else 0

#define BASE_FREQ	44100