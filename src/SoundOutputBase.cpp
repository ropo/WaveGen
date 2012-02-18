#include "stdafx.h"

#define SAFE_RELEASE(x)	{if(x){(x)->Release();(x)=NULL;}}

SoundOutputBase::SoundOutputBase()
{
}

SoundOutputBase::~SoundOutputBase()
{
}
