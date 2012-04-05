/************************************************************************/
/* File		: context.cpp
/* OS independent Context 
/* Date		: 2006/3/29
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/

#include "context.h"
#include "util.h"
#include "matrix.h"
#include "pipeline.h"
#include <math.h>



#define INV_BTW_BEGIN_END 	if (isBegun == GL_TRUE) {   \
							setError(GL_INVALID_OPERATION);  \
							return;  \
							}

GLcontext * pctx = NULL;


void GLcontext::setMaterialf(GLenum face, GLenum pname, GLfloat param)
{
	if (pname !=  GL_SHININESS) {
		setError(GL_INVALID_ENUM);
	}

	switch(face)
	{
	case GL_FRONT_AND_BACK:
		vtxBuf.setAttrib1f(VTX_F_SHININESS, &param);
		vtxBuf.setAttrib1f(VTX_B_SHININESS, &param);
		break;
	case GL_FRONT:
		vtxBuf.setAttrib1f(VTX_F_SHININESS, &param);
		break;
	case GL_BACK:
		vtxBuf.setAttrib1f(VTX_B_SHININESS, &param);
		break;
	default:
		setError(GL_INVALID_ENUM);
		break;
	}

	setError(GL_NO_ERROR);
}


void GLcontext::setMaterialfv(GLenum face, GLenum pname, const GLfloat * params)
{
	GLint index = 0;
	switch(pname)
	{
	case GL_AMBIENT:
		index = VTX_F_AMBIENT;
		break;
	case GL_DIFFUSE:
		index = VTX_F_DIFFUSE;
		break;
	case GL_SPECULAR:
		index = VTX_F_SPECULAR;
		break;
	case GL_EMISSION:
		index = VTX_F_EMISSION;
		break;
	case GL_SHININESS:
		setMaterialf(face, GL_SHININESS, *params);
		return;
	case GL_AMBIENT_AND_DIFFUSE:
		setMaterialfv(face, GL_AMBIENT, params);
		setMaterialfv(face, GL_DIFFUSE, params);
		return;
	default:
		setError(GL_INVALID_ENUM);
		break;
	}
	switch(face)
	{
	case GL_FRONT_AND_BACK:
		vtxBuf.setAttrib4f(index, params);
		vtxBuf.setAttrib4f(index + 1, params);
		break;
	case GL_FRONT:
		vtxBuf.setAttrib4f(index, params);
		break;
	case GL_BACK:
		vtxBuf.setAttrib4f(index + 1, params);
		break;
	default:
		setError(GL_INVALID_ENUM);
		break;
	}
}


void GLcontext::setFrontFace(GLenum mode)
{
	INV_BTW_BEGIN_END;
	if (mode != GL_CW && mode != GL_CCW) {
		setError(GL_INVALID_ENUM);
		return;
	}

	frontFace = mode; 
	setError(GL_NO_ERROR);
}

void GLcontext::setCullFace(GLenum mode)
{
	INV_BTW_BEGIN_END;
	if (mode != GL_FRONT && mode != GL_BACK
		&& mode != GL_FRONT_AND_BACK) {
		setError(GL_INVALID_ENUM);
		return;
	}
	culledFace = mode;  
	setError(GL_NO_ERROR);
}


void GLcontext::setShadeModel(GLenum mode)
{
	INV_BTW_BEGIN_END;
	if (mode != GL_SMOOTH && mode != GL_FLAT) {
		setError(GL_INVALID_ENUM);
		return;
	}

	shadeModel = mode;  
	setError(GL_NO_ERROR);
}


void GLcontext::setClearColor(GLclampf red, GLclampf green, 
							  GLclampf blue, GLclampf alpha)
{
	INV_BTW_BEGIN_END;
	GLfloat color[4];
	GLRGBA(color, red, green, blue, alpha);
	pOsCtx->setClearColor(color);
}

MatrixStack * GLcontext::getCurrentMatrixStack()
{
	switch(this->matrixMode)
	{
	case GL_MODELVIEW:
		return &modelStack;
	case GL_PROJECTION:
		return &projStack;
	}
	return NULL;
}


void GLcontext::setMatrixMode(GLubyte mode)
{
	INV_BTW_BEGIN_END;
	if (mode != GL_PROJECTION &&
		mode != GL_MODELVIEW) {
		setError(GL_INVALID_ENUM);
		return;
	}
	this->matrixMode = mode;
	setError(GL_NO_ERROR);
}

void GLcontext::pushMatrix()
{
	INV_BTW_BEGIN_END;

	GLmatrix mat = getCurrentMatrix();
	MatrixStack * pStack = getCurrentMatrixStack();

	if (pStack->isFull()) {
		setError(GL_STACK_OVERFLOW);
	}
	pStack->push(mat);
	setError(GL_NO_ERROR);
}

void GLcontext::popMatrix()
{
	INV_BTW_BEGIN_END;
	MatrixStack * pStack = getCurrentMatrixStack();

	if (pStack->isEmpty()) {
		setError(GL_STACK_UNDERFLOW);
	}
	GLmatrix mat = pStack->top();
	pStack->pop();
	setCurrentMatrix(mat);

	setError(GL_NO_ERROR);
}


void GLcontext::setProjectionMatrix(GLmatrix mat)
{
	projMatrix = mat;
}

void GLcontext::setModelViewMatrix(GLmatrix mat)
{
	modelMatrix = mat;
}

void GLcontext::setViewPortMatrix(GLmatrix mat)
{
	viewMatrix = mat;
}

void GLcontext::setCurrentMatrix(GLmatrix mat)
{
	if (matrixMode == GL_PROJECTION) {
		projMatrix = mat;
	}
	if (matrixMode == GL_MODELVIEW) {
		modelMatrix = mat;
	}
}


GLmatrix GLcontext::getModelViewMatrix()
{
	return this->modelMatrix;
}

GLmatrix GLcontext::getProjectionMatrix()
{
	return this->projMatrix;
}

GLmatrix GLcontext::getViewPortMatrix()
{
	return this->viewMatrix;
}


GLmatrix GLcontext::getCurrentMatrix()
{
	if (matrixMode == GL_PROJECTION) {
		return this->projMatrix;
	}
	if (matrixMode == GL_MODELVIEW) {
		return this->modelMatrix;
	}	
	return NULL;
}



void GLcontext::loadIdentity()
{
	INV_BTW_BEGIN_END;

	GLfloat * mat = getCurrentMatrix();
	Matrix::setIdentity(mat);
	setError(GL_NO_ERROR);
	return;
}

void GLcontext::multMatrixf(const GLfloat * m)
{
	INV_BTW_BEGIN_END;
	
	GLfloat * mat = getCurrentMatrix();

	GLfloat tmp[16];
	for (GLint i = 0; i < 16; i++) {
		tmp[i] = m[i];
	}
	Matrix::mul44(mat, mat, tmp);

	setError(GL_NO_ERROR);
}

void GLcontext::multMatrixd(const GLdouble * m)
{
	INV_BTW_BEGIN_END;

	GLfloat * mat = getCurrentMatrix();

	GLfloat tmp[16];
	for (GLint i = 0; i < 16; i++) {
		tmp[i] = (GLfloat)m[i];
	}

	Matrix::mul44(mat, mat, tmp);

	setError(GL_NO_ERROR);
}


void GLcontext::perspective(GLdouble fovy, GLdouble aspect, 
				 GLdouble zNear, GLdouble zFar)
{
	INV_BTW_BEGIN_END;

	GLfloat m[4][4];
	double sine, cot, deltaZ;
	double radians = fovy / 2 * M_PI / 180;

	deltaZ = zFar - zNear;
	sine = sin(radians);
	if (deltaZ == 0 || sine == 0 || aspect == 0) {
		return;
	}
	cot = cos(radians) / sine;

	Matrix::setIdentity(&m[0][0]);

	m[0][0] = (GLfloat)(cot / aspect);
	m[1][1] = (GLfloat)cot;
	m[2][2] = (GLfloat)(-(zFar + zNear) / deltaZ);
	m[2][3] = -1;
	m[3][2] = (GLfloat)(-2 * zNear * zFar / deltaZ);
	m[3][3] = 0;
	multMatrixf(&m[0][0]);
}

void GLcontext::frustum(
			 GLfloat l, GLfloat r,
			 GLfloat b, GLfloat t,
			 GLfloat n, GLfloat f)
{
	INV_BTW_BEGIN_END;

	if (n <= 0 || f <= 0) {
		setError(GL_NO_ERROR);
		return;
	}

	GLfloat * mat = getCurrentMatrix();

	Matrix::frustum(mat, l, r, b, t, n, f);
	setError(GL_NO_ERROR);

	return;
}

void GLcontext::ortho(
		   GLfloat l, GLfloat r,
		   GLfloat b, GLfloat t,
		   GLfloat n, GLfloat f)
{
	INV_BTW_BEGIN_END;

	GLfloat * mat = getCurrentMatrix();

	Matrix::ortho(mat, l, r, b, t, n, f);
	setError(GL_NO_ERROR);
	
	return;
}

void GLcontext::rotate(
			GLfloat angle, GLfloat x, 
			GLfloat y, GLfloat z)
{
	INV_BTW_BEGIN_END;

	GLfloat * mat = getCurrentMatrix();
	
	Matrix::rotate(mat, angle, x, y, z);
	
	setError(GL_NO_ERROR);
}

void GLcontext::translate(GLfloat x, 
			   GLfloat y, GLfloat z)
{
	INV_BTW_BEGIN_END;

	GLfloat * mat = getCurrentMatrix();

	Matrix::translate(mat, x, y, z);
	setError(GL_NO_ERROR);
}

void GLcontext::scale(GLfloat x, 
		   GLfloat y, GLfloat z)
{
	INV_BTW_BEGIN_END;

	GLfloat * mat = getCurrentMatrix();
	
	Matrix::scale(mat, x, y, z);

	setError(GL_NO_ERROR);
}


void GLcontext::viewport(GLint x, GLint y, GLsizei w, GLsizei h)
{
	INV_BTW_BEGIN_END;

	if (w < 0 || h < 0) {
		setError(GL_INVALID_VALUE);
	}
	
	GLfloat * mat = getViewPortMatrix();

	Matrix::viewport(mat, x, y, w, h);	
	pOsCtx->resizeBuffer(x, y, w, h);

	setError(GL_NO_ERROR);
}



GLboolean GLcontext::initializeMatrix()
{
	GLfloat mat[16];
	memSetf(mat, 0, MATRIX_SIZE);

	projStack.push(mat);
	modelStack.push(mat);

	projMatrix = projStack.top();
	modelMatrix = modelStack.top();
	viewMatrix = (GLmatrix)memAllocf(MATRIX_SIZE);
	return GL_TRUE;
}

void GLcontext::destroyMatrix()
{
	if (viewMatrix != NULL) {
		memFree(viewMatrix);
	}
}


GLboolean GLcontext::initializePipeline()
{
	pPPBuf = NULL;
	if(resizePPBuffer(DEFPIPELINESIZE) == GL_FALSE)
		return GL_FALSE;
	return GL_TRUE;
}

void GLcontext::destroyPipeline()
{
	if(pPPBuf != NULL)
		memFree(pPPBuf);
}

GLboolean GLcontext::initializeExtra()
{
	// Clear color
	// set default clear color to black
	GLRGB(clearColor, 0, 0, 0);
	// clean all flags
	for (GLuint i = 0; i < GL_FLAGMAX; i++) {
		this->currentState[i] = GL_FALSE;
	}
	setError(GL_NO_ERROR);
	isBegun = GL_FALSE;
	shadeModel = GL_SMOOTH;
	frontFace = GL_CCW;
	culledFace = GL_BACK;
	return GL_TRUE;
}


void GLcontext::setEnable(GLenum cap)
{
	currentState[cap] = GL_TRUE;
}


void GLcontext::setDisable(GLenum cap)
{
	currentState[cap] = GL_FALSE;
}


void GLcontext::destroyExtra()
{}


GLenum GLcontext::getError()
{
	return this->lastError;
}

// Alloc all the buffer in pool
GLcontext::GLcontext()
{
	pOsCtx = NULL;
}


GLcontext::~GLcontext()
{
	memFree(viewMatrix);
}


GLboolean GLcontext::initialize(IDispFuncTbl * ctx)
{
	GLboolean ret;
	setContext(ctx);
	ret = initializeMatrix();
	if (ret == GL_FALSE) {
		return ret;
	}
	ret = initializeExtra();
	if (ret == GL_FALSE) {
		return ret;
	}
	ret = initializeVertexBuffer();
	if (ret == GL_FALSE) {
		return ret;
	}

	ret = initializePipeline();
	if (ret == GL_FALSE) {
		return ret;
	}

	ret = initializeTexture();
	if (ret == GL_FALSE) {
		return ret;
	}

	ret = initializeLight();
	if (ret == GL_FALSE) {
		return ret;
	}
	
	return GL_TRUE;
}

GLboolean GLcontext::initializeVertexBuffer()
{
	return GL_TRUE;
}


GLboolean GLcontext::resizePPBuffer(GLuint newSize)
{
	if (pPPBuf != NULL) {
		memFree(pPPBuf);
	}

	pPPBuf = (Vertex *)memAlloc(sizeof(Vertex) * newSize);
	if (pPPBuf == NULL) {
		return GL_FALSE;
	}

	ppBufSize = newSize;
	return GL_TRUE;
}

void GLcontext::begin(GLenum mode)
{
	isBegun = GL_TRUE;
	if (mode > GL_POLYGON) {
		setError(GL_INVALID_ENUM);
	}
	beginMode = mode;
	setError(GL_NO_ERROR);
}


void GLcontext::end()
{
	if (isBegun == GL_FALSE) {
		setError(GL_INVALID_OPERATION);
	}
	isBegun = GL_FALSE;
	runPipeline();
	setError(GL_NO_ERROR);
}

void GLcontext::flush()
{
	INV_BTW_BEGIN_END;

	pOsCtx->flip();
	setError(GL_NO_ERROR);
}


void GLcontext::clear(GLbitfield mask)
{
	INV_BTW_BEGIN_END;
	if (mask > GL_CLEAR_MAX) {
		setError(GL_INVALID_VALUE);
	}
	if (mask & GL_COLOR_BUFFER_BIT) {
		clearColorBuf();
	}
	if (mask & GL_DEPTH_BUFFER_BIT) {
		clearDepth();
	}
	if (mask & GL_ACCUM_BUFFER_BIT) {
		clearAccum();
	}
	if (mask & GL_STENCIL_BUFFER_BIT) {
		clearStencil();
	}
	setError(GL_NO_ERROR);
}

void GLcontext::clearColorBuf()
{
	INV_BTW_BEGIN_END;
	pOsCtx->clearBuffer();
	setError(GL_NO_ERROR);
}

void GLcontext::clearDepth()
{
	INV_BTW_BEGIN_END;

	setError(GL_NO_ERROR);
}

void GLcontext::clearStencil()
{
	INV_BTW_BEGIN_END;

	setError(GL_NO_ERROR);
}

void GLcontext::clearAccum()
{
	INV_BTW_BEGIN_END;

	setError(GL_NO_ERROR);
}

void GLcontext::clearIndex()
{
	INV_BTW_BEGIN_END;

	setError(GL_NO_ERROR);
}


void GLcontext::setTexEnvModef(GLenum target, GLenum pname,
								GLfloat param)
{
	INV_BTW_BEGIN_END;

	if (target != GL_TEXTURE_ENV ||
		pname != GL_TEXTURE_ENV_MODE) {
		setError(GL_INVALID_ENUM);
	}

	if (param != GL_MODULATE && param != GL_DECAL
		&& param != GL_BLEND && param != GL_REPLACE) {
		setError(GL_INVALID_ENUM);
	}

	this->texEnvMode = (GLenum)param;
	setError(GL_NO_ERROR);
}

void GLcontext::setTexEnvModefv(GLenum target,
								GLenum pname,
								const GLfloat *params)
{
	INV_BTW_BEGIN_END;

	if (pname != GL_TEXTURE_ENV_COLOR) {
		setTexEnvModef(target, pname, *params);
	}

	COPY4V(texEnvColor, params);

	setError(GL_NO_ERROR);
}


void GLcontext::setTexParameterf(GLenum target,
								GLenum pname,
								GLfloat param)
{
	INV_BTW_BEGIN_END;

	GLenum eParam = (GLenum)param;

	if (target != GL_TEXTURE_1D && 
		target != GL_TEXTURE_2D) {
		setError(GL_INVALID_ENUM);
	}

	if (pname != GL_TEXTURE_MIN_FILTER && pname != GL_TEXTURE_MAG_FILTER 
		&& pname != GL_TEXTURE_WRAP_S && pname != GL_TEXTURE_WRAP_T) {
		setError(GL_INVALID_ENUM);
	}

	if (eParam != GL_NEAREST && eParam != GL_LINEAR
		&& eParam != GL_REPEAT && eParam != GL_CLAMP) {
		setError(GL_INVALID_ENUM);
	}

	this->texMgr.texParameterf(target, pname, eParam);
	setError(GL_NO_ERROR);
}


GLboolean GLcontext::initializeTexture()
{
	texEnvMode = GL_MODULATE;
	COPY4V(texEnvColor, attribDefVal[VTX_COLOR]);
	return GL_TRUE;
}

void GLcontext::deleteTextures(GLsizei n,  const GLuint * textures)
{
	INV_BTW_BEGIN_END;

	this->texMgr.delTexture(n, textures);

	setError(GL_NO_ERROR);
}


void GLcontext::bindTexture(GLenum target, GLuint texture)
{
	INV_BTW_BEGIN_END;

	this->texMgr.bindTexture(target, texture);

	setError(GL_NO_ERROR);
}

void GLcontext::genTextures(GLsizei n, GLuint * textures)
{
	INV_BTW_BEGIN_END;

	this->texMgr.genTextures(n, textures);

	setError(GL_NO_ERROR);
}


void GLcontext::texImage2D(GLenum target,
				  GLint level,
				  GLint internalformat,
				  GLsizei width,
				  GLsizei height,
				  GLint border,
				  GLenum format,
				  GLenum type,
				  const GLvoid *pixels)
{
	INV_BTW_BEGIN_END;

	if (target != GL_TEXTURE_2D) {
		setError(GL_INVALID_ENUM);
		return;
	}

	this->texMgr.texImage(target, level, internalformat, 
		width, height, border, format, type, pixels);

	setError(GL_NO_ERROR);
}


GLboolean GLcontext::initializeLight()
{
	for (GLint i = 0; i < GL_MAX_LIGHTS; i++) {
		lightObj[i].initLight(i);
	}
	F4VASSIGN(lightModelAmbient, 0.2f, 0.2f, 0.2f, 1.0f);
	lightModelTwoSide = GL_FALSE;
	return GL_TRUE;
}

void GLcontext::setLightModelf(GLenum pname, GLfloat param)
{
	INV_BTW_BEGIN_END;
	switch(pname)
	{
	case GL_LIGHT_MODEL_TWO_SIDE:
		lightModelTwoSide = GL_TRUE;
		break;
	case GL_LIGHT_MODEL_LOCAL_VIEWER:
		break;
	default:
		break;
	}
	setError(GL_NO_ERROR);
}

void GLcontext::setLightModelfv(GLenum pname, const GLfloat * params)
{
	INV_BTW_BEGIN_END;
	switch(pname)
	{
	case GL_LIGHT_MODEL_TWO_SIDE:
	case GL_LIGHT_MODEL_LOCAL_VIEWER:
		setLightModelf(pname, *params);
		break;
	case GL_LIGHT_MODEL_AMBIENT:
		COPY4V(lightModelAmbient, params);
	default:
		break;
	}
	setError(GL_NO_ERROR);
}



void GLcontext::setLightf(GLenum light, GLenum pname, GLfloat param)
{
	INV_BTW_BEGIN_END;
	
	if (light >= GL_MAX_LIGHTS + GL_LIGHTBASE) {
		setError(GL_INVALID_ENUM);
	}
	light -= GL_LIGHTBASE;
	switch(pname)
	{
	case GL_SPOT_EXPONENT:
		lightObj[light].spotExponent = param;
		break;
	case GL_SPOT_CUTOFF:
		lightObj[light].spotCutoff = param;
		break;
	case GL_CONSTANT_ATTENUATION:
		lightObj[light].constAttenuation = param;
		break;
	case GL_LINEAR_ATTENUATION:
		lightObj[light].linearAttenuation = param;
		break;
	case GL_QUADRATIC_ATTENUATION:
		lightObj[light].quadAttenuation = param;
		break;
	default:
		setError(GL_INVALID_ENUM);
		break;
	}
	setError(GL_NO_ERROR);
}



void GLcontext::setLightfv(GLenum light, GLenum pname, const GLfloat * params)
{
	INV_BTW_BEGIN_END;

	if (light >= GL_MAX_LIGHTS + GL_LIGHTBASE) {
		setError(GL_INVALID_ENUM);
	}
	light -= GL_LIGHTBASE;

	switch(pname)
	{
	case GL_AMBIENT:
		COPY4V(lightObj[light].ambient, params);
		break;
	case GL_DIFFUSE:
		COPY4V(lightObj[light].diffuse, params);
		break;
	case GL_SPECULAR:
		COPY4V(lightObj[light].specular, params);
		break;
	case GL_POSITION:
		{
			if (EQUAL0(params[3])) {
				lightObj[light].directional = GL_TRUE;
			}
			else
				lightObj[light].directional = GL_FALSE;

			Matrix::transVector4f(lightObj[light].lightPos, 
								  CTXPTR->modelMatrix, params);
			Matrix::vecHomotrans(lightObj[light].lightPos);
		}
		break;
	case GL_SPOT_DIRECTION:
		COPY3V(lightObj[light].spotDir, params);
		break;			
	case GL_SPOT_EXPONENT:
	case GL_SPOT_CUTOFF:
	case GL_CONSTANT_ATTENUATION:
	case GL_LINEAR_ATTENUATION:
	case GL_QUADRATIC_ATTENUATION:
		setLightf(light, pname, *params);
		return;
	default:
		setError(GL_INVALID_ENUM);
		break;
	}
	setError(GL_NO_ERROR);
}


GLboolean GLcontext::runPipeline()
{
	if (this->ppBufSize < this->vtxBuf.getVertexCount()) {
		this->resizePPBuffer(vtxBuf.getVertexCount());
	}
	if(Pipeline::runVertexPipeline() == GL_FALSE)
		return GL_FALSE;

	if(Pipeline::runNormalPipeline() == GL_FALSE)
		return GL_FALSE;

	if (Pipeline::runLightPipeline() == GL_FALSE) {
		return GL_FALSE;
	}

	if (Pipeline::runTexPipeline() == GL_FALSE) {
		return GL_FALSE;
	}
	
	if (Pipeline::runRastPipeline() == GL_FALSE) {
		return GL_FALSE;
	}

	this->vtxBuf.clearBuffer();
	return GL_TRUE;
}

void GLcontext::destroyVertexBuffer()
{}

void GLcontext::destroy()
{
	this->destroyPipeline();
	this->destroyMatrix();
	this->destroyExtra();
	this->destroyVertexBuffer();
}

void GLcontext::setContext(IDispFuncTbl * ctx)
{
	assert(ctx != NULL);
	if (ctx == NULL) {
		assert(false);
		return;
	}
	this->pOsCtx = ctx;
	return;
}


GLcontext * GLcontext::createContext()
{
	GLcontext * pCtx = new GLcontext;
	if (pCtx == NULL) {
		assert(false);
		return NULL;
	}

	return pCtx;
}


void GLcontext::destroyContext(GLcontext  * ctx)
{
	if (ctx == NULL) {
		return;
	}
	delete ctx->pOsCtx;
	delete ctx;
}


GLmatrix MatrixStack::top()
{
	if (current == -1) {
		return NULL;
	}
	return buffer[current];
}


GLboolean MatrixStack::push(GLmatrix mat)
{
	if (isFull()) {
		return GL_FALSE;
	}
	memCopyf(buffer[++current], mat, MATRIX_SIZE);
	return GL_TRUE;
}



GLboolean MatrixStack::isFull()
{
	return current == MATRIX_MAXDEPTH - 1;
}



GLboolean MatrixStack::getSize()
{
	return current + 1;
}


void MatrixStack::pop()
{
	if (current == -1) {
		return;
	}
	current--;
}


GLboolean MatrixStack::isEmpty()
{
	return current == -1;
}

MatrixStack::MatrixStack()
{
	GLint size = MATRIX_MAXDEPTH;
	memSet(buffer, 0, sizeof(buffer));
	for (GLint i = 0; i < size; i++) {
		GLmatrix tmp = (GLmatrix)memAllocf(MATRIX_SIZE);
		if (tmp == NULL) {
			assert(false);
			return;
		}
        this->buffer[i] = tmp;
	}
	current = -1;
}


MatrixStack::~MatrixStack()
{
	GLint size = MATRIX_MAXDEPTH;

	for (GLint i = 0; i < size; i++) {
		if (buffer[i] != NULL) {
			memFree(buffer[i]);
		}
	}
}


#ifdef _DEBUG
void testContext()
{}
#endif


