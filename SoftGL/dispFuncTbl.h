/************************************************************************/
/* File		: dispFuncTbl.h
/* display function table
/* Date		: 2006/3/10
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/

#ifndef DISPFUNCTBL_H
#define DISPFUNCTBL_H

#include "globdef.h"


struct IDispFuncTbl
{
	virtual GLint getWidth() = 0;
	virtual GLint getHeight() = 0;
	virtual void writePixel(GLint x, GLint y, const GLubyte rgb[4]) = 0;
	virtual void writePixels(GLint x[], GLint y[], const GLubyte rgb[][4], GLint n) = 0;
	virtual void writeSpan(GLint n, GLint x, GLint y, const GLubyte rgb[][4]) = 0;
	virtual void writeSpan(GLint n, GLint x, GLint y, const GLubyte rgb[4]) = 0;
	virtual GLboolean setClearColor(GLcolor color) = 0;
	virtual void clearBuffer() = 0;
	virtual GLboolean resizeBuffer(GLint x, GLint y, GLint w, GLint h) = 0;
	virtual void flip() = 0;
};


#endif
