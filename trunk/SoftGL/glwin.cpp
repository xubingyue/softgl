/************************************************************************/
/* File		: glwin.cpp
/* Context about OS layer
/* Date		: 2006/4/10
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/

#include "glwin.h"


BOOL WGLCreateContext(HDC hdc)
{
	CTXPTR = GLcontext::createContext();
	if (CTXPTR == NULL) {
		return FALSE;
	}
	IDispFuncTbl * osc =  OScontext::createContext(hdc, pctx->clearColor);
	if (osc == NULL) {
		return FALSE;
	}
	CTXPTR->initialize(osc);
	return TRUE;
}



BOOL WGLDestroyContext()
{
	return TRUE;
}