/************************************************************************/
/* File		: SoftGL.cpp
/* GL Library API Output Functions definitions
/* Date		: 2006/3/10
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/


#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料

#include <windows.h>
#include "..\inc\SoftGL.h"
#include "vertex.h"
#include "glwin.h"
#include "context.h"
#include "rast.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}


// Output functions

SOFTGL_API BOOL softglCreateContext(HDC hdc)
{
	FUNC_HOOK("softglCreateContext");
	return WGLCreateContext(hdc);
}

SOFTGL_API BOOL softglDestroyContext()
{
	FUNC_HOOK("softglDestroyContext");
	return WGLDestroyContext();
}

SOFTGL_API void MANGLE(Vertex2f)(GLfloat x,
						   GLfloat y)
{
	FUNC_HOOK("Vertex2f");
	CTXPTR->inputVertex2f(x, y);
}

SOFTGL_API void MANGLE(Vertex3f)(GLfloat x,
						   GLfloat y,
						   GLfloat z)
{
	FUNC_HOOK("Vertex3f");
	CTXPTR->inputVertex3f(x, y, z);
}

SOFTGL_API void MANGLE(Vertex2fv)(const GLfloat *v)
{
	FUNC_HOOK("Vertex2fv");
	CTXPTR->inputVertex2fv(v);
}

SOFTGL_API void MANGLE(Vertex3fv)(const GLfloat *v)
{
	FUNC_HOOK("Vertex3fv");
	CTXPTR->inputVertex3fv(v);
}

SOFTGL_API void MANGLE(Color3f)(GLfloat red, 
								GLfloat green, 
								GLfloat blue)
{
	FUNC_HOOK("Color3f");
	CTXPTR->setColor3f(red, green, blue);
}

SOFTGL_API void MANGLE(Color3fv)(const GLfloat * v)
{
	FUNC_HOOK("Color3fv");
    CTXPTR->setColor3fv(v);
}

SOFTGL_API void MANGLE(Color4f)(GLfloat red, 
								GLfloat green, 
								GLfloat blue, 
								GLfloat alpha)
{
	FUNC_HOOK("Color4f");
	CTXPTR->setColor4f(red, green, blue, alpha);
}

SOFTGL_API void MANGLE(Color4fv)(const GLfloat * v)
{
	FUNC_HOOK("Color4fv");
	CTXPTR->setColor4fv(v);
}


SOFTGL_API void MANGLE(TexCoord2f)(GLfloat s, GLfloat t)
{
	FUNC_HOOK("TexCoord2f");
	CTXPTR->setTexCoord2f(s, t);
}


SOFTGL_API void MANGLE(Frustum)( GLfloat l, GLfloat r,
								GLfloat b, GLfloat t,
								GLfloat n, GLfloat f)
{
	FUNC_HOOK("Frustum");
	CTXPTR->frustum(l, r, b, t, n, f);
}

SOFTGL_API void MANGLE(Ortho)(GLfloat l, GLfloat r,
							  GLfloat b, GLfloat t,
							  GLfloat n, GLfloat f)
{
	FUNC_HOOK("Ortho");
	CTXPTR->ortho(l, r, b, t, n, f);
}

SOFTGL_API void MANGLE(Rotatef)(GLfloat angle, GLfloat x, 
							   GLfloat y, GLfloat z)
{
	FUNC_HOOK("Rotatef");
	CTXPTR->rotate(angle, x, y, z);
}


SOFTGL_API void MANGLE(Translatef)(GLfloat x, 
								  GLfloat y, 
								  GLfloat z)
{
	FUNC_HOOK("Translatef");
	CTXPTR->translate(x, y, z);
}

SOFTGL_API void MANGLE(Scalef)(GLfloat x, 
							  GLfloat y, 
							  GLfloat z)
{
	FUNC_HOOK("Scalef");
	CTXPTR->scale(x, y, z);
}

SOFTGL_API void MANGLE(LoadIdentity)()
{
	FUNC_HOOK("LoadIdentity");
	CTXPTR->loadIdentity();
}

SOFTGL_API void MANGLE(Viewport)(GLint x, GLint y, GLsizei w, GLsizei h)
{
	FUNC_HOOK("Viewport");
	CTXPTR->viewport(x, y, w, h);
}


SOFTGL_API void MANGLE(MultMatrixf)(const GLfloat * m)
{
	FUNC_HOOK("MultMatrixf");
	CTXPTR->multMatrixf(m);
}


SOFTGL_API void MANGLE(MultMatrixd)(const GLdouble * m)
{
	FUNC_HOOK("MultMatrixd");
	CTXPTR->multMatrixd(m);
}



SOFTGL_API void MANGLE(MatrixMode)(GLenum mode)
{
	FUNC_HOOK("MatrixMode");
	CTXPTR->setMatrixMode(mode);
}

SOFTGL_API void MANGLE(PushMatrix)()
{
	FUNC_HOOK("PushMatrix");
	CTXPTR->pushMatrix();
}

SOFTGL_API void MANGLE(PopMatrix)()
{
	FUNC_HOOK("PopMatrix");
	CTXPTR->popMatrix();
}


SOFTGL_API void MANGLE(ShadeModel)(GLenum mode)
{
	FUNC_HOOK("ShadeModel");
	CTXPTR->setShadeModel(mode);
}

SOFTGL_API void MANGLE(CullFace)(GLenum mode)
{
	FUNC_HOOK("CullFace");
	CTXPTR->setCullFace(mode);
}

SOFTGL_API void MANGLE(FrontFace)(GLenum mode)
{
	FUNC_HOOK("FrontFace");
	CTXPTR->setFrontFace(mode);
}

SOFTGL_API void MANGLE(Enable)(GLenum cap)
{
	FUNC_HOOK("Enable");
	CTXPTR->setEnable(cap);
}


SOFTGL_API void MANGLE(Disable)(GLenum cap)
{
	FUNC_HOOK("Disable");
	CTXPTR->setDisable(cap);
}



SOFTGL_API void MANGLE(Begin)(GLenum mode)
{
	FUNC_HOOK("Begin");
	CTXPTR->begin(mode);
}

SOFTGL_API void MANGLE(End)()
{
	FUNC_HOOK("End");
	CTXPTR->end();
}


SOFTGL_API void MANGLE(Flush)()
{
	FUNC_HOOK("Flush");
	CTXPTR->flush();
}

SOFTGL_API void MANGLE(ClearColor)()
{
	FUNC_HOOK("ClearColor");
	CTXPTR->clearColorBuf();
}


SOFTGL_API void MANGLE(Clear)(GLbitfield mask)
{	
	FUNC_HOOK("Clear");
	CTXPTR->clear(mask);
}

SOFTGL_API void MANGLE(ClearColor)(GLclampf red, GLclampf green,
								   GLclampf blue, GLclampf alpha)
{
	FUNC_HOOK("ClearColor");
	CTXPTR->setClearColor(red, green, blue, alpha);
}

SOFTGL_API void GLUMANGLE(Perspective)(GLdouble fovy, GLdouble aspect, 
									GLdouble zNear, GLdouble zFar)
{
	FUNC_HOOK("Perspective");
	CTXPTR->perspective(fovy, aspect, zNear, zFar);
}


SOFTGL_API void MANGLE(DeleteTextures)(GLsizei n, const GLuint * textures)
{
	FUNC_HOOK("DeleteTextures");
	CTXPTR->deleteTextures(n, textures);
}


SOFTGL_API void MANGLE(GenTextures)(GLsizei n, GLuint * textures)
{
	FUNC_HOOK("GenTextures");
	CTXPTR->genTextures(n, textures);
}


SOFTGL_API void MANGLE(BindTexture)( GLenum target, GLuint texture)
{
	FUNC_HOOK("BindTexture");
	CTXPTR->bindTexture(target, texture);
}

SOFTGL_API void MANGLE(TexImage2D)(GLenum target,
								   GLint level,
								   GLint internalformat,
								   GLsizei width,
								   GLsizei height,
								   GLint border,
								   GLenum format,
								   GLenum type,
								   const GLvoid *pixels)
{
	FUNC_HOOK("TexImage2D");
	CTXPTR->texImage2D(target, level, internalformat, 
						width, height, border, format, 
						type, pixels);
}


SOFTGL_API void MANGLE(TexEnvf)(GLenum target,	GLenum pname,
								GLfloat param)
{
	FUNC_HOOK("TexEnvf");
    CTXPTR->setTexEnvModef(target, pname, param);
}


SOFTGL_API void MANGLE(TexEnvfv)(GLenum target,	GLenum pname,
								 const GLfloat * param)
{
	FUNC_HOOK("TexEnvfv");
	CTXPTR->setTexEnvModefv(target, pname, param);
}


SOFTGL_API void MANGLE(TexParameterf)(GLenum target,	GLenum pname,
									  GLfloat param)
{
	FUNC_HOOK("TexParameterf");
	CTXPTR->setTexParameterf(target, pname, param);
}


SOFTGL_API void MANGLE(Lightf)(GLenum light,	GLenum pname,
							   GLfloat param)
{
	FUNC_HOOK("Lightf");
	CTXPTR->setLightf(light, pname, param);
}




SOFTGL_API void MANGLE(Lightfv)(GLenum light,	GLenum pname,
								const GLfloat * params)
{
	FUNC_HOOK("Lightfv");
	CTXPTR->setLightfv(light, pname, params);
}




SOFTGL_API void MANGLE(Materialfv)(GLenum face,	GLenum pname,
								   const GLfloat * params)
{
	FUNC_HOOK("Materialfv");
	CTXPTR->setMaterialfv(face, pname, params);
}





SOFTGL_API void MANGLE(Materialf)(GLenum face,	GLenum pname,
								  GLfloat param)
{
	FUNC_HOOK("Materialf");
	CTXPTR->setMaterialf(face, pname, param);
}


SOFTGL_API void MANGLE(Normal3f)(GLfloat nx, GLfloat ny, GLfloat nz)
{	
	FUNC_HOOK("Normal3f");
	CTXPTR->setNormal3f(nx, ny, nz);
}


SOFTGL_API void MANGLE(Normal3fv)(const GLfloat * v)
{	
	FUNC_HOOK("Normal3fv");
	CTXPTR->setNormal3fv(v);
}

SOFTGL_API void MANGLE(LightModelf)(GLenum pname, GLfloat param)
{
	FUNC_HOOK("LightModelf");
	CTXPTR->setLightModelf(pname, param);
}

SOFTGL_API void MANGLE(LightModelfv)(GLenum pname, const GLfloat * params)
{
	FUNC_HOOK("LightModelfv");
	CTXPTR->setLightModelfv(pname, params);
}


// end of output

