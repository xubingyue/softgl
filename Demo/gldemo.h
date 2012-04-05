/************************************************************************/
/* File		: gldemo.cpp
/* Demo functions
/* Date		: 2006/5/9
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/

#ifndef TEST_H
#define TEST_H

#include "..\inc\SoftGL.h"


void loadTex(char * pstrName);

#define DEMO(x)   namespace Demo##x  \
{            \
	void draw();   \
	void resize();  \
	void init();				\
}

DEMO(2)
DEMO(3)
DEMO(4)

int GLCreate(HDC hdc);
int GLResize(GLint w, GLint h);
int GLPaint();
void GLChangeDemo();
void GLChangeFilter();
void GLChangeEnvMode();


#endif

