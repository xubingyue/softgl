/************************************************************************/
/* File		: util.h
/* OS related utility functions
/* Date		: 2006/4/9
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/


#ifndef UTIL_H
#define UTIL_H

#include "globdef.h"
#include <memory>

#define FLOATSIZESHIFT	2
#define FLOATSIZE(s)		((s) << FLOATSIZESHIFT)

inline void * memAlloc(size_t size)
{
	return new GLubyte[size];
}

inline GLfloat * memAllocf(size_t count)
{
	return new GLfloat[count];
}


inline void memFree(void * ptr)
{
	delete [] ptr;
}


inline void * memSet(void *dest, int c, size_t count)
{
	return memset(dest, c, count);
}

inline void * memSetf(void *dest, int c, size_t count)
{
	return memset(dest, c, count << FLOATSIZESHIFT);
}


inline void * memCopy(void *dest, const void *src, size_t count)
{
	return memcpy(dest, src, count);
}

inline void * memCopyf(void *dest, const void *src, size_t count)
{
	return memcpy(dest, src, count << FLOATSIZESHIFT);
}

#endif
