/************************************************************************/
/* File		: SoftGL.h
/* GL Library API Output Functions definitions
/* Date		: 2006/3/10
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/

#pragma once

#include "gltypes.h"

#include <windows.h>
#include "Winuser.h"

#ifdef SOFTGL_EXPORTS
#define SOFTGL_API __declspec(dllexport)
#else
#define SOFTGL_API __declspec(dllimport)
#endif


#define MANGLE(x)			gl##x		
#define GLUMANGLE(x)		glu##x

// 此类是从 SoftGL.dll 导出的

SOFTGL_API BOOL softglCreateContext(HDC hdc);
SOFTGL_API BOOL softglDestroyContext();


// vertex.h
SOFTGL_API void MANGLE(Vertex2f)(GLfloat x,
						   GLfloat y);

SOFTGL_API void MANGLE(Vertex3f)(GLfloat x,
						   GLfloat y,
						   GLfloat z);

SOFTGL_API void MANGLE(Vertex2fv)(const GLfloat *v);

SOFTGL_API void MANGLE(Vertex3fv)(const GLfloat *v);

SOFTGL_API void MANGLE(Color3f)(GLfloat red, 
								GLfloat green, 
								GLfloat blue);

SOFTGL_API void MANGLE(Color3fv)(const GLfloat * v);

SOFTGL_API void MANGLE(Color4f)(GLfloat red, 
								GLfloat green, 
								GLfloat blue, 
								GLfloat alpha);

SOFTGL_API void MANGLE(Color4fv)(const GLfloat * v);


SOFTGL_API void MANGLE(Color4fv)(const GLfloat * v);

SOFTGL_API void MANGLE(TexCoord2f)(GLfloat s, GLfloat t);

// transform


SOFTGL_API void MANGLE(Frustum)( GLfloat l, GLfloat r,
						 GLfloat b, GLfloat t,
						 GLfloat n, GLfloat f);

SOFTGL_API void MANGLE(Ortho)(GLfloat l, GLfloat r,
						GLfloat b, GLfloat t,
					    GLfloat n, GLfloat f);

SOFTGL_API void MANGLE(Rotatef)(GLfloat angle, GLfloat x, 
						GLfloat y, GLfloat z);

SOFTGL_API void MANGLE(Translatef)(GLfloat x, 
						  GLfloat y, 
						  GLfloat z);

SOFTGL_API void MANGLE(Scalef)(GLfloat x, 
					  GLfloat y, 
					  GLfloat z);

SOFTGL_API void MANGLE(LoadIdentity)();

SOFTGL_API void MANGLE(Viewport)(GLint x, GLint y, GLsizei w, GLsizei h);

SOFTGL_API void MANGLE(MultMatrixf)(const GLfloat * m);
SOFTGL_API void MANGLE(MultMatrixd)(const GLdouble * m);

SOFTGL_API void MANGLE(PushMatrix)();

SOFTGL_API void MANGLE(PopMatrix)();

SOFTGL_API void MANGLE(MatrixMode)(GLenum mode);

// end of transform section


SOFTGL_API void MANGLE(Begin)(GLenum mode);

SOFTGL_API void MANGLE(End)();

SOFTGL_API void MANGLE(Flush)();

SOFTGL_API void MANGLE(ClearColor)();

SOFTGL_API void MANGLE(Clear)(GLbitfield mask);

SOFTGL_API void MANGLE(ClearColor)(GLclampf red, GLclampf green,
								   GLclampf blue, GLclampf alpha);


// state change

SOFTGL_API void MANGLE(ShadeModel)(GLenum mode);
SOFTGL_API void MANGLE(CullFace)(GLenum mode);
SOFTGL_API void MANGLE(FrontFace)(GLenum mode);

SOFTGL_API void MANGLE(Enable)(GLenum cap);
SOFTGL_API void MANGLE(Disable)(GLenum cap);

// texture
SOFTGL_API void MANGLE(DeleteTextures)(GLsizei n, const GLuint * textures);
SOFTGL_API void MANGLE(GenTextures)(GLsizei n, GLuint * textures);
SOFTGL_API void MANGLE(BindTexture)( GLenum target, GLuint texture);
SOFTGL_API void MANGLE(TexImage2D)(GLenum target,
								   GLint level,
								   GLint internalformat,
								   GLsizei width,
								   GLsizei height,
								   GLint border,
								   GLenum format,
								   GLenum type,
								   const GLvoid *pixels);


SOFTGL_API void MANGLE(TexEnvf)(GLenum target,	GLenum pname,
								GLfloat param);


SOFTGL_API void MANGLE(TexEnvf)(GLenum target,	GLenum pname,
								GLfloat param);

SOFTGL_API void MANGLE(TexEnvfv)(GLenum target,	GLenum pname,
								 const GLfloat * param);


SOFTGL_API void MANGLE(TexParameterf)(GLenum target,	GLenum pname,
								GLfloat param);

SOFTGL_API void GLUMANGLE(Perspective)(GLdouble fovy, GLdouble aspect, 
									   GLdouble zNear, GLdouble zFar);




SOFTGL_API void MANGLE(Lightf)(GLenum light,	GLenum pname,
									  GLfloat param);

SOFTGL_API void MANGLE(Lightfv)(GLenum light,	GLenum pname,
							    const GLfloat * params);

SOFTGL_API void MANGLE(LightModelf)(GLenum pname, GLfloat param);

SOFTGL_API void MANGLE(LightModelfv)(GLenum pname, const GLfloat * params);



SOFTGL_API void MANGLE(Materialfv)(GLenum face,	GLenum pname,
								  	 const GLfloat * params);


SOFTGL_API void MANGLE(Materialf)(GLenum face,	GLenum pname,
								  GLfloat param);


SOFTGL_API void MANGLE(Normal3f)(GLfloat nx, GLfloat ny, GLfloat nz);
SOFTGL_API void MANGLE(Normal3fv)(const GLfloat * v);


// end of state change

