/************************************************************************/
/* File		: globdef.h
/* global definitions
/* Date		: 2006/3/10
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/

#ifndef GLOBDEF_H
#define GLOBDEF_H

#include "..\inc\gltypes.h"
#include "impltype.h"



// only if USE_EXCEPTIONS defined we throw exceptions
#ifdef USE_EXCEPTIONS
#define GLTHROW(e) throw e; 
#else
#define GLTHROW(e)
#endif

#endif
