/************************************************************************/
/* File		: texture.cpp
/* Texture functions
/* Date		: 2006/5/6
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/

#include "texture.h"
#include "util.h"
#include "vertex.h"
#include <cmath>

static GLubyte  trash[4];


GLboolean TexObject::initImage( GLenum target,
					GLint level,
					GLint internalformat,
					GLsizei width,
					GLsizei height,
					GLint border,
					GLenum format,
					GLenum type,
					const GLvoid *pixels)
{
	freeImage();
	if (target != this->target) {
		return GL_FALSE;
	}

	assert(internalformat == 4);
	assert(type == GL_UNSIGNED_BYTE);

	GLvoid * pTemp = memAlloc(sizeof(GLcolor) * width * height);
	if (pTemp == NULL) {
		this->width = 0;
		this->height = 0;
		return GL_FALSE;
	}
	this->data = pTemp;

	this->width = width;
	this->height = height;
	
	this->log2Width = 0;
	while(width != 1)
	{
		width >>= 1;
		this->log2Width++;
	}
	GLubyte * pData = (GLubyte *)this->data;
	GLubyte * pSrc = (GLubyte *)pixels;

	const GLsizei total = this->width * this->height;

	GLint i = 0;
	for (i = 0; i < total; i++) {
		GLC2RGBCOPY(pData, attribDefVal[VTX_COLOR]);
		pData += 4;
	}
	
	pData = (GLubyte *)this->data;



	switch(format)
	{
	case GL_RGB:
		for (i = 0; i < total; i++) {
			COPY3V(pData, pSrc);
			pSrc += 3;
			pData += 4;
		}
		break;
	case GL_RGBA:
		for (i = 0; i < total; i++) {
			COPY4V(pData, pSrc);
			pSrc += 4;
			pData += 4;
		}
		break;
	case GL_LUMINANCE:
		for (i = 0; i < total; i++) {
			COPY4V(pData, pSrc);
			pSrc += 1;
			pData += 4;
		}
		break;
	default:
		return GL_FALSE;
	}

	return GL_TRUE;
}


void TexObject::freeImage()
{
	if (data != trash) {
		memFree(data);
		data = trash;
	}
}


TexObject::TexObject()
{
	memSet(this, 0, sizeof(TexObject));
	data = trash;
}


TexObject * TexObjMgr::getCurrentObject()
{
	if (curObjPtr != NULL) {
		return curObjPtr;
	}
	return &defTexObj;
}

TexObjMgr::TexObjMgr()
{
	this->curMaxId = 0;
	curObjPtr = &defTexObj;
}


void TexObjMgr::genTextures(GLsizei count, GLuint * textures)
{
	if (textures == NULL) {
		return;
	}
	for (GLsizei i = 0; i < count; i++) {
		GLuint newId;
		if (freedName.size() > 0) {
			newId = freedName.back();
			*textures = newId;
			freedName.pop_back();
		}
		else
		{
			newId = (++curMaxId);
			*textures = newId;
		}

		pair<GLuint, TexObject> texPair(newId, TexObject());
		texPool.insert(texPair);
		textures++;
	}
}


void TexObjMgr::delTexture(GLsizei n, const GLuint * textures)
{
	if (textures == NULL) {
		return;
	}
	for (GLsizei i = 0; i < n; i++, textures++) {
		GLuint tex = *textures;
		if (texPool.find(tex) == texPool.end()) {
			continue;
		}

		TexObject * ptr = &texPool[tex];
		if (ptr == curObjPtr) {
			curObjPtr = &defTexObj;
		}
		ptr->freeImage();
		texPool.erase(tex);
		freedName.push_back(tex);
	}
}



void TexObjMgr::bindTexture(GLenum target, GLuint tex)
{
	if(texPool.find(tex) == texPool.end())
	{
		if (curMaxId < tex) {
			curMaxId = tex;
			pair<GLuint, TexObject> texPair(tex, TexObject());
			texPool.insert(texPair);
		}
	}
	this->curObjPtr = &texPool[tex];
	curObjPtr->target = target;
}



void TexObjMgr::texImage(GLenum target,
					 GLint level,
					 GLint internalformat,
					 GLsizei width,
					 GLsizei height,
					 GLint border,
					 GLenum format,
					 GLenum type,
					 const GLvoid *pixels)
{
	TexObject * pObj = getCurrentObject();
	pObj->initImage(target, level, internalformat, 
						width, height, border, format, type, pixels);
}


void TexObjMgr::texParameterf(GLenum target,
						  GLenum pname,
						  GLenum param)
{
	TexObject * pObj = getCurrentObject();
	if (target != pObj->target) {
		assert(false);
		return;
	}

	switch(pname)
	{
	case GL_TEXTURE_MIN_FILTER:	
	case GL_TEXTURE_MAG_FILTER:
		if (param != GL_LINEAR && param != GL_NEAREST) {
			assert(false);
			return;
		}
		if (GL_TEXTURE_MIN_FILTER == pname) {
			pObj->minFilter = param;
		}
		else {  // GL_TEXTURE_MAG_FILTER
			pObj->magFilter = param;
		}
		break;
	case GL_TEXTURE_WRAP_S:
	case GL_TEXTURE_WRAP_T:
		if (param != GL_CLAMP && param != GL_REPEAT) {
			assert(false);
			return;
		}
		if (GL_TEXTURE_WRAP_S == pname) {
			pObj->wrapS = param;
		}
		else {
			pObj->wrapT = param;
		}
		break;
	default:
		assert(false);
		return;
	}
}