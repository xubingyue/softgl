/************************************************************************/
/* File		: texture.h
/* Texture functions
/* Date		: 2006/5/6
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/

#ifndef TEXTURE_H
#define TEXTURE_H

#include "globdef.h"
#include <vector>
#include <map>

using namespace std;

struct TexObject
{
	GLenum	 target;
	GLenum	 magFilter;
	GLenum	 minFilter;
	GLenum	 wrapS;
	GLenum	 wrapT;
	GLsizei  width;
	GLsizei  height;
	GLint    log2Width;
	GLvoid * data;

	TexObject();

	GLboolean initImage( GLenum target,
					GLint level,
					GLint internalformat,
					GLsizei width,
					GLsizei height,
					GLint border,
					GLenum format,
					GLenum type,
					const GLvoid *pixels);

	void freeImage();
};


class TexObjMgr
{
public:
	TexObjMgr();
	TexObject * getCurrentObject();
	void        genTextures(GLsizei count, GLuint * textures);
	void        bindTexture(GLenum target, GLuint tex);
	void        delTexture(GLsizei n,  const GLuint * textures);
	void        texImage(GLenum target,
							GLint level,
							GLint internalformat,
							GLsizei width,
							GLsizei height,
							GLint border,
							GLenum format,
							GLenum type,
							const GLvoid *pixels);

	void		texParameterf(GLenum target,
								GLenum pname,
								GLenum param);
private:
	TexObject					defTexObj;
	GLuint						curTexId;
	vector<GLuint>				freedName;
	map<GLuint, TexObject>		texPool;
	GLuint						curMaxId;
	TexObject *					curObjPtr;
};

#endif
