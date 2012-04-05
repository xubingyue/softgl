/************************************************************************/
/* File		: context.h
/* OS independent Context 
/* Date		: 2006/3/10
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/

#ifndef CONTEXT_H
#define CONTEXT_H

#include "oscontext.h"
#include "impltype.h"
#include "dispFuncTbl.h"
#include "vertex.h"
#include "texture.h"
#include "light.h"
#include <cassert>

#define ALLOCMAT(x) ((x) = memAllocf(16))
#define DELMAT(x)	(memFree(x))
#define CTXPTR		(pctx)



class MatrixStack 
{
public:
	GLmatrix	top();
	GLboolean	push(GLmatrix mat);
	void		pop();
	GLboolean	isEmpty();
	GLboolean	isFull();
	GLboolean	getSize();
	MatrixStack();
	~MatrixStack();
private:
	MatrixStack(MatrixStack &) {}  // disable copy ctor
	void operator = (const MatrixStack &) {}   // disable assignment operator

	GLmatrix	buffer[MATRIX_MAXDEPTH];
	GLint		current;
};



class GLcontext 
{
public:
	IDispFuncTbl *	pOsCtx;
	GLboolean initialize(IDispFuncTbl * ctx);
	void	  destroy();

// Vertex Buffer section
public:
	inline void setColor3f(GLfloat red, 
					GLfloat green, 
					GLfloat blue);

	inline void setColor4f(GLfloat red, 
					GLfloat green, 
					GLfloat blue, 
					GLfloat alpha);

	inline void setColor3fv(const GLfloat * v);
	inline void setColor4fv(const GLfloat * v);

	inline void setTexCoord2f(GLfloat s, GLfloat t);

	inline void inputVertex2f(GLfloat x, GLfloat y);
	inline void inputVertex3f(GLfloat x, GLfloat y, GLfloat z);
	inline void inputVertex2fv(const GLfloat * v);
	inline void inputVertex3fv(const GLfloat * v);
	VertexBuffer * getVertexBufferPtr() { return &vtxBuf;}
private:
	VertexBuffer vtxBuf;
	GLboolean initializeVertexBuffer();
	void destroyVertexBuffer();
// end of vertex buffer

// Texture
public:
	GLenum   texEnvMode;
	GLcolor	 texEnvColor;

	inline TexObject * getCurrentTexObj();
	void   bindTexture(GLenum target, GLuint texture);
	void   genTextures(GLsizei n, GLuint * textures);
	void   deleteTextures(GLsizei n,  const GLuint * textures);
	void   texImage2D(GLenum target,
						GLint level,
						GLint internalformat,
						GLsizei width,
						GLsizei height,
						GLint border,
						GLenum format,
						GLenum type,
						const GLvoid *pixels);
	void	setTexEnvModef( GLenum target,	GLenum pname,
							GLfloat param);

	void    setTexEnvModefv(GLenum target,
							GLenum pname,
							const GLfloat *params);

	void	setTexParameterf(GLenum target,
								GLenum pname,
								GLfloat param);

private:
	GLboolean initializeTexture();
	TexObjMgr texMgr;
// End of Texture

// Light
public:
	void		setMaterialf(GLenum face, GLenum pname, GLfloat param);
	void		setMaterialfv(GLenum face, GLenum pname, const GLfloat * params);

	inline void		setNormal3f( GLfloat nx, GLfloat ny, GLfloat nz);
	inline void		setNormal3fv(const GLfloat * v);

	void		setLightf(GLenum light, GLenum pname, GLfloat param);
	void		setLightfv(GLenum light, GLenum pname, const GLfloat * params);
	void		setLightModelf(GLenum pname, GLfloat param);
	void        setLightModelfv(GLenum pname, const GLfloat * params);

	LightObj	lightObj[GL_MAX_LIGHTS];
	GLfloat		lightModelAmbient[4];
	GLboolean	lightModelTwoSide;
private:
	GLboolean	initializeLight();
// End of light

//  Matrix Section
public:
	GLmatrix		projMatrix;
	GLmatrix		modelMatrix;
	GLmatrix		viewMatrix;

	GLubyte			matrixMode;

	// functions
	void setMatrixMode(GLubyte mode);

	void loadIdentity();

	void frustum(
		GLfloat l, GLfloat r,
		GLfloat b, GLfloat t,
		GLfloat n, GLfloat f);

	void ortho(
		GLfloat l, GLfloat r,
		GLfloat b, GLfloat t,
		GLfloat n, GLfloat f);

	void rotate( 
		GLfloat angle, GLfloat x, 
		GLfloat y, GLfloat z);

	void translate(GLfloat x, 
		GLfloat y, GLfloat z);

	void scale(GLfloat x, 
		GLfloat y, GLfloat z);

	void viewport(GLint x, GLint y, GLsizei w, GLsizei h);

	void perspective(GLdouble fovy, GLdouble aspect, 
					 GLdouble zNear, GLdouble zFar);

	void multMatrixf(const GLfloat * m);
	void multMatrixd(const GLdouble * m);

	void pushMatrix();
	void popMatrix();

	GLmatrix getModelViewMatrix();
	GLmatrix getProjectionMatrix();
	GLmatrix getViewPortMatrix();
	GLmatrix getCurrentMatrix();

	void     setProjectionMatrix(GLmatrix mat);
	void	 setModelViewMatrix(GLmatrix mat);
	void	 setViewPortMatrix(GLmatrix mat);
	void     setCurrentMatrix(GLmatrix mat);
private:
	MatrixStack		projStack;
	MatrixStack		modelStack;

	MatrixStack * getCurrentMatrixStack();
	void	  destroyMatrix();
	GLboolean initializeMatrix();
// end of matrix section

// pipeline
public:
	GLenum beginMode;
	void begin(GLenum mode);
	void end();
	void flush();
	Vertex * getPPBuffer() { return pPPBuf;}
	GLuint getPPBufSize() {return ppBufSize;}

	void   clear(GLbitfield mask);
	void   clearColorBuf();
	void   clearDepth();
	void   clearStencil();
	void   clearAccum();
	void   clearIndex();
private:
	Vertex * pPPBuf;
	GLuint   ppBufSize;
	GLboolean resizePPBuffer(GLuint newSize);
	GLboolean runPipeline();
	GLboolean initializePipeline();
	void	  destroyPipeline();
// end of pipeline

// extra data
public:
	GLenum			lastError;
	GLboolean		isBegun;
	GLboolean		currentState[GL_FLAGMAX];
	GLcolor			clearColor;	 // clear color
	GLenum			shadeModel;
	GLenum			frontFace;
	GLenum			culledFace;

	GLenum			getError();
	inline void		setError(GLenum code);
	void			setCullFace(GLenum mode);
	void			setShadeModel(GLenum mode);
	void			setFrontFace(GLenum mode);
	void			setClearColor(GLclampf red, GLclampf green, 
								  GLclampf blue, GLclampf alpha);
    void			setEnable(GLenum cap);
	void			setDisable(GLenum cap);

	inline GLint	getWidth();
	inline GLint	getHeight();
private:
	void		destroyExtra();
	GLboolean	initializeExtra();
// end of extra data

// export function sets
public:
	static GLcontext * createContext();
	static void destroyContext(GLcontext  * ctx);
// end of functions
private:
	void setContext(IDispFuncTbl * ctx);	

	GLcontext();
	~GLcontext();
};


inline void GLcontext::inputVertex2f(GLfloat x, GLfloat y)
{
	GLfloat v[2];
	v[0] = x, v[1] = y;
	vtxBuf.insertVtx2f(v);
}

inline void GLcontext::inputVertex3f(GLfloat x, GLfloat y, GLfloat z)
{
	GLfloat v[3];
	v[0] = x, v[1] = y, v[2] = z;
	vtxBuf.insertVtx3f(v);
}

inline void GLcontext::inputVertex2fv(const GLfloat * v)
{
	vtxBuf.insertVtx2f(v);
}

inline void GLcontext::inputVertex3fv(const GLfloat * v)
{
	vtxBuf.insertVtx3f(v);
}

inline void GLcontext::setColor3f(GLfloat red, 
								GLfloat green, 
								GLfloat blue)
{
	GLfloat v[3];

	v[IDXR] = red;
	v[IDXG] = green;
	v[IDXB] = blue;

	vtxBuf.setAttrib3f(VTX_COLOR, v);
}

inline void GLcontext::setColor4f(GLfloat red, 
								  GLfloat green, 
								  GLfloat blue, 
								  GLfloat alpha)
{
	GLfloat v[4];
	v[IDXR] = red, v[IDXG] = green;
	v[IDXB] = blue, v[IDXA] = alpha;
	vtxBuf.setAttrib4f(VTX_COLOR, v);
}

inline void GLcontext::setColor3fv(const GLfloat * v)
{
	vtxBuf.setAttrib3f(VTX_COLOR, v);
}

inline void GLcontext::setColor4fv(const GLfloat * v)
{
	vtxBuf.setAttrib4f(VTX_COLOR, v);
}


inline void GLcontext::setTexCoord2f(GLfloat s, GLfloat t)
{
	GLfloat v[2];
	v[0] = s, v[1] = t;
	vtxBuf.setAttrib2f(VTX_TEXCOORD, v);
}


void GLcontext::setNormal3f(GLfloat nx, GLfloat ny, GLfloat nz)
{
	GLfloat v[3];
	v[0] = nx, v[1] = ny, v[2] = nz;
	vtxBuf.setAttrib3f(VTX_NORMAL, v);
}

void GLcontext::setNormal3fv(const GLfloat * v)
{
	vtxBuf.setAttrib3f(VTX_NORMAL, v);
}


inline void GLcontext::setError(GLenum code)
{
	assert(code == GL_NO_ERROR);
	lastError = code;
}

inline TexObject * GLcontext::getCurrentTexObj()
{
	return this->texMgr.getCurrentObject();
}

GLint GLcontext::getWidth()
{
	return pOsCtx->getWidth();
}

GLint GLcontext::getHeight()
{
	return pOsCtx->getHeight();
}


extern GLcontext * pctx;

#ifdef _DEBUG

void testContext();

#endif


#endif