/************************************************************************/
/* File		: glwin.h
/* Win32 related Output functions
/* Date		: 2006/4/10
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/

#ifndef GLWIN_H
#define GLWIN_H

#include <windows.h>
#include "context.h"
#include "oscontext.h"
#include "globdef.h"

BOOL WGLCreateContext(HDC hdc);


BOOL WGLDestroyContext();



#endif
