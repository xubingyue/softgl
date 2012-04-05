/************************************************************************/
/* File		: vertex.h
/* Vertex related
/* Date		: 2006/4/9
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/


#ifndef VERTEX_H
#define VERTEX_H

#include "globdef.h"
#include "util.h"
#include "profiler.h"



struct BufferDesc
{
	const GLfloat * pBuf;
	GLint			stride;
	GLint			total;
	GLint			elemSize;
	GLubyte			attribId;
};


struct VertexBuffer
{
	GLint MaxBufferCount;
	VertexBuffer();
	~VertexBuffer();
	
	GLfloat * getBufferPtr();
	void	  getBufferDesc(BufferDesc * pDesc, GLubyte attribid);
	inline GLboolean setAttrib1f(GLubyte idx, const GLfloat * v);
	inline GLboolean setAttrib2f(GLubyte idx, const GLfloat * v);
	inline GLboolean setAttrib3f(GLubyte idx, const GLfloat * v);
	inline GLboolean setAttrib4f(GLubyte idx, const GLfloat * v);
	inline GLboolean insertVtx2f(const GLfloat * v);
	inline GLboolean insertVtx3f(const GLfloat * v);
	inline GLboolean insertVtx4f(const GLfloat * v);
	inline GLuint    getVertexCount();
	void			 clearBuffer();
private:
	GLfloat	 *	vtxBuf;
	GLubyte		vtxAttrSize[VTX_ATTRIB_COUNT];		// counts in 4-bytes
	GLfloat		curAttrib[VTX_ATTRIB_COUNT][4];	// current attributes buffer

	GLfloat  *	pCurrentPos;		// point to the next empty area of vtxbuffer
	GLuint		vtxCount;


	VertexBuffer(const VertexBuffer &) {}
	void operator = (const VertexBuffer &) {}

	GLboolean insertVtx(GLubyte size, const GLfloat * v);
	GLboolean setAttrib(GLubyte idx, GLubyte size, const GLfloat * v);
	GLboolean resizeAttrib(GLubyte idx, GLubyte size);	
	GLboolean reallocBuffer();
};

inline GLuint VertexBuffer::getVertexCount()
{
	return vtxCount;
}

inline GLfloat * VertexBuffer::getBufferPtr()
{
	return vtxBuf;
}

inline GLboolean VertexBuffer::setAttrib1f(GLubyte idx, const GLfloat * v)
{
	return setAttrib(idx, 1, v);
}

inline VertexBuffer::~VertexBuffer()
{
	if (vtxBuf != NULL) {
		memFree(vtxBuf);
	}
}


inline GLboolean VertexBuffer::insertVtx2f(const GLfloat * v)
{
	return insertVtx(2, v);
}

inline GLboolean VertexBuffer::insertVtx3f(const GLfloat * v)
{
	return insertVtx(3, v);
}

inline GLboolean VertexBuffer::insertVtx4f(const GLfloat * v)
{
	return insertVtx(4, v);
}

inline GLboolean VertexBuffer::setAttrib2f(GLubyte idx, const GLfloat * v)
{
	return setAttrib(idx, 2, v);
}

inline GLboolean VertexBuffer::setAttrib3f(GLubyte idx, const GLfloat * v)
{
	return setAttrib(idx, 3, v);
}

inline GLboolean VertexBuffer::setAttrib4f(GLubyte idx, const GLfloat * v)
{
	return setAttrib(idx, 4, v);
}


// should a pure struct
// do not add any ctor
struct Vertex
{
	GLfloat coord[4];   // x, y, z, w
	GLfloat color[4];   // r, g, b, a
	GLfloat	colorBack[4];
	GLfloat texCoord[4]; // s, t, r, q
	GLfloat normal[4];
};




#ifdef _DEBUG

inline void testVertexBuffer()
{
	VertexBuffer vtx;
	GLfloat v[4];
	GLfloat v1[4];
	GLfloat v2[4];
	v[0] = 1, v[1] = 2, v[2] = 3, v[3] = 4;
	v1[0] = 11, v1[1] = 22, v1[2] = 33, v1[3] = 44;
	v2[0] = 11, v2[1] = 22, v2[2] = 33, v2[3] = 44;

	vtx.setAttrib3f(VTX_COLOR, v);
	vtx.insertVtx3f(v1);
	vtx.insertVtx4f(v1);
	vtx.setAttrib4f(VTX_NORMAL, v2);
	vtx.insertVtx3f(v1);
	GLfloat ar[100];
	memCopyf(ar, vtx.getBufferPtr(), 100);
	return;
}

#endif




#endif