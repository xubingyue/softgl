/************************************************************************/
/* File		: vertex.cpp
/* Vertex related
/* Date		: 2006/4/9
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/


#include "vertex.h"
#include "util.h"
#include <cassert>
#include <memory>


// default vaule of attributes


VertexBuffer::VertexBuffer()
{
	// set all attribute size to 0
	for (GLint i = 0; i < VTX_ATTRIB_COUNT; i++) {
		vtxAttrSize[i] = 0;
	}
	vtxCount = 0;
	MaxBufferCount = VTXBUF_INITBUFSIZE;
	vtxBuf = memAllocf(MaxBufferCount);
	pCurrentPos = vtxBuf;
}



// set current attributes
GLboolean VertexBuffer::setAttrib(GLubyte idx, GLubyte size, const GLfloat * v)
{
	if (size > 4 || size < 0 || vtxBuf == NULL ||
		v == NULL || idx >= VTX_ATTRIB_COUNT) {
		assert(false);
		return GL_FALSE;
	}
	if (this->vtxAttrSize[idx] < size) {
		// need resize
		if(resizeAttrib(idx, size) == GL_FALSE)
			return GL_FALSE;
	}

	GLfloat * dest = curAttrib[idx];
	const GLfloat * def  = attribDefVal[idx];
	switch(size)
	{
	case 4:	dest[3] = v[3];
	case 3:	dest[2] = v[2];
	case 2:	dest[1] = v[1];
	case 1:	dest[0] = v[0];
	default: 
		break;
	}
	// fill in default value
	switch(size)
	{
	case 1: dest[1] = def[1];
	case 2: dest[2] = def[2];
	case 3: dest[3] = def[3];
	case 4: break;
	default: 
		break;
	}
	
	return GL_TRUE;
}


void VertexBuffer::clearBuffer()
{
	vtxCount = 0;
	pCurrentPos = vtxBuf;
	// set all attribute size to 0
	for (GLint i = 0; i < VTX_ATTRIB_COUNT; i++) {
		vtxAttrSize[i] = 0;
	}	
}


GLboolean VertexBuffer::insertVtx(GLubyte size, const GLfloat * v)
{
	if (size > 4 || size < 0 || v == NULL || vtxBuf == NULL) {
			assert(false);
			return GL_FALSE;
	}
	GLuint oldSize = vtxAttrSize[VTX_COORD];
	if (oldSize < size) {
		// need resize
		if(resizeAttrib(VTX_COORD, size) == GL_FALSE)
			return GL_FALSE;
	}
	setAttrib(VTX_COORD, size, v);
	GLfloat * pEnd = &vtxBuf[MaxBufferCount];
	for(GLuint i = 0; i < VTX_ATTRIB_COUNT; i++)
	{
		GLuint fragSize = vtxAttrSize[i];
		if (fragSize != 0) {
			if (pCurrentPos + fragSize > pEnd) {
				// buffer overflow
				if(reallocBuffer() == GL_FALSE)
					return GL_FALSE;
				pEnd = &vtxBuf[MaxBufferCount];
			}
			memCopyf(pCurrentPos, curAttrib[i], fragSize);
			pCurrentPos += fragSize;
		}
	}
	GLfloat var[100];
	memCopyf(var, vtxBuf, 100);
	this->vtxCount ++;

	return GL_TRUE;
}


GLboolean VertexBuffer::resizeAttrib(GLubyte idx, GLubyte size)
{
	if (idx >= VTX_ATTRIB_COUNT || vtxAttrSize[idx] >= size) {
		return GL_FALSE;
	}
	GLfloat	* backupBuf = memAllocf(MaxBufferCount);
	
	std::auto_ptr<GLfloat> trash_ptr(backupBuf);
	
	if (backupBuf == NULL) {
		return GL_FALSE;
	}
	GLuint elementSize = 0;
	GLfloat * src = this->vtxBuf;
	GLuint i = 0;
	for (i = 0; i < VTX_ATTRIB_COUNT; i++) {
		elementSize += vtxAttrSize[i];
	}
	GLuint bufferSize = elementSize * vtxCount;

	// backup buffer
	memCopyf(backupBuf, src, bufferSize);
	GLfloat * pCurBack = backupBuf;
	GLuint	  oldSize = vtxAttrSize[idx]; 
	GLuint	  delta = size - oldSize;
	pCurrentPos = vtxBuf;
	// reconstruct buffer
	for (i = 0; i < vtxCount; i++) {
		for (GLubyte j = 0; j < VTX_ATTRIB_COUNT; j ++) {
			GLuint fragSize = vtxAttrSize[j];
			GLuint fragSizeInByte = FLOATSIZE(fragSize);
			if (j == idx) {
				// the attrib needs to be enlarge
				if (pCurrentPos + delta + fragSize >= &vtxBuf[MaxBufferCount] &&
					reallocBuffer() == GL_FALSE) {
						return GL_FALSE;					
				}						
				memCopy(pCurrentPos, pCurBack, fragSizeInByte);
				pCurrentPos += fragSize;
				memCopyf(pCurrentPos, attribDefVal[idx] + fragSize, delta);
				pCurrentPos += delta;
				pCurBack += oldSize;
				continue;
			}
			if (fragSize == 0) {
				continue;
			}
			if (pCurrentPos + fragSize >= &vtxBuf[MaxBufferCount] && 
				reallocBuffer() == GL_FALSE) {
				return GL_FALSE;
			}
			memCopy(pCurrentPos, pCurBack, fragSizeInByte);
			pCurrentPos += fragSize; 
			pCurBack += fragSize;
		}
	}
	
	vtxAttrSize[idx] = size;
	return GL_TRUE;
}


GLboolean VertexBuffer::reallocBuffer()
{
	size_t offset = pCurrentPos - vtxBuf;
	if (vtxBuf == NULL) {
		return GL_FALSE;
	}

	GLuint size = MaxBufferCount << 1;
	GLfloat * pTmp = memAllocf(size);
	if ( pTmp == NULL) {
		return GL_FALSE;
	}
	memCopyf(pTmp, vtxBuf, MaxBufferCount);
	memFree(vtxBuf);
	vtxBuf = pTmp;
	pCurrentPos = pTmp + offset;
	MaxBufferCount = size;
	return GL_TRUE;
}



void VertexBuffer::getBufferDesc(BufferDesc * pDesc, GLubyte attribid)
{
	GLubyte attSize = vtxAttrSize[attribid];
	pDesc->attribId = attribid;
	pDesc->total = vtxCount;
	if (attSize == 0) {
		// empty attribute
		pDesc->pBuf = attribDefVal[attribid];
		pDesc->elemSize = 0;
		pDesc->stride = 0;
		return;
	}
	GLfloat * ptr = this->vtxBuf;

	//GLfloat var[100];
	//memCopyf(var, ptr, 100);

	GLuint fragSize = 0;
	GLubyte i = 0;
	for (i = 0; i < attribid; i ++) {
		fragSize += vtxAttrSize[i];
	}
	ptr += fragSize;

	for(; i < VTX_ATTRIB_COUNT; i++)
		fragSize += vtxAttrSize[i];

	pDesc->stride = fragSize;
	pDesc->elemSize = attSize;
	pDesc->pBuf = ptr;
}