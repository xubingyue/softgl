/************************************************************************/
/* File		: pipeline.cpp
/* Pipeline functions
/* Date		: 2006/5/1
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/

#include "vertex.h"
#include "context.h"
#include "pipeline.h"
#include "matrix.h"
#include "rast.h"


// Internal Functions

GLboolean rastPoints(Vertex * pBuf, GLuint size);
GLboolean rastLines(Vertex * pBuf, GLuint size);
GLboolean rastLineStrip(Vertex * pBuf, GLuint size);
GLboolean rastLineLoop(Vertex * pBuf, GLuint size);
GLboolean rastTriangles(Vertex * pBuf, GLuint size);
GLboolean rastTriangleStrip(Vertex * pBuf, GLuint size);
GLboolean rastTriangleFan(Vertex * pBuf, GLuint size);
GLboolean rastQuads(Vertex * pBuf, GLuint size);
GLboolean rastQuadStrip(Vertex * pBuf, GLuint size);
GLboolean rastPolygon(Vertex * pBuf, GLuint size);

GLboolean copyColor();


// End of Internal Functions

namespace Pipeline
{
	GLboolean runVertexPipeline()
	{
		VertexBuffer * pVtxBuf = CTXPTR->getVertexBufferPtr();
		if (pVtxBuf == NULL) {
			return GL_FALSE;
		}

		Vertex * pPPBuf = CTXPTR->getPPBuffer();
		if (pPPBuf == NULL) {
			return GL_FALSE;
		}

		BufferDesc desc;
		pVtxBuf->getBufferDesc(&desc, VTX_COORD);

		BufferDesc normalDesc;
		pVtxBuf->getBufferDesc(&normalDesc, VTX_NORMAL);

		const GLfloat * pBuffer = desc.pBuf;
		if (pBuffer == NULL) {
			return GL_FALSE;
		}

		GLint size = desc.elemSize;
		GLint total = desc.total;
		GLint stride = desc.stride;

		void (*transVector)(GLfloat *, const GLfloat *, const GLfloat *) = NULL;

		switch(size)
		{
		case 4:
			transVector = Matrix::transVector4f;
			break;
		case 3:
			transVector = Matrix::transVector3f;
			break;
		case 2:
			transVector = Matrix::transVector2f;
			break;
		default:
			break;
		}

		if (transVector == NULL) {
			return GL_FALSE;
		}

		for (GLint i = 0; i < total; i++) {
			//transVector(pPPBuf->coord, mat, pBuffer);
			transVector(pPPBuf->coord, CTXPTR->modelMatrix, pBuffer);
			Matrix::vecHomotrans(pPPBuf->coord);
			pBuffer += stride;
			pPPBuf++;
		}
		return GL_TRUE;
	}


	GLboolean runNormalPipeline()
	{
		if (CTXPTR->currentState[GL_LIGHTING] == GL_FALSE) {
			return GL_TRUE;
		}

		VertexBuffer * pVtxBuf = CTXPTR->getVertexBufferPtr();
		if (pVtxBuf == NULL) {
			return GL_FALSE;
		}

		Vertex * pPPBuf = CTXPTR->getPPBuffer();
		if (pPPBuf == NULL) {
			return GL_FALSE;
		}

		BufferDesc desc;
		pVtxBuf->getBufferDesc(&desc, VTX_NORMAL);

		const GLfloat * pBuffer = desc.pBuf;

		if (pBuffer == NULL) {
			return GL_FALSE;
		}


		GLfloat mat[MATRIX_SIZE];
		Matrix::mul44(mat, CTXPTR->projMatrix, CTXPTR->modelMatrix);

		GLint size = desc.elemSize;
		GLint total = desc.total;
		GLint stride = desc.stride;

		for (GLint i = 0; i < total; i++) {
			Matrix::transVector3f(pPPBuf->normal, mat, pBuffer);
			pBuffer += stride;
			pPPBuf++;
		}
		return GL_TRUE;
	}

	GLboolean runLightPipeline()
	{
		if(CTXPTR->currentState[GL_LIGHTING] == GL_FALSE)
		{
			return copyColor();
		}

		for (GLint i = 0; i < GL_MAX_LIGHTS; i++) {
			if (CTXPTR->currentState[i + GL_LIGHTBASE] == GL_FALSE) {
				continue;
			}

			if(Light::calLight(GL_FRONT) == GL_FALSE)
				return GL_FALSE;

			if (CTXPTR->lightModelTwoSide == GL_TRUE) {
				// front face
				if(Light::calLight(GL_BACK) == GL_FALSE)
					return GL_FALSE;
			}
		}		
		return GL_TRUE;
	}

	GLboolean runTexPipeline()
	{
		if (CTXPTR->currentState[GL_TEXTURE_2D] == GL_FALSE) {
			return GL_TRUE;
		}
		VertexBuffer * pVtxBuf = CTXPTR->getVertexBufferPtr();
		if (pVtxBuf == NULL) {
			return GL_FALSE;
		}

		Vertex * pPPBuf = CTXPTR->getPPBuffer();
		if (pPPBuf == NULL) {
			return GL_FALSE;
		}

		BufferDesc desc;
		pVtxBuf->getBufferDesc(&desc, VTX_TEXCOORD);

		const GLfloat * pBuffer = desc.pBuf;
		if (pBuffer == NULL) {
			return GL_FALSE;
		}

		GLint size = desc.elemSize;
		GLint total = desc.total;
		GLint stride = desc.stride;

		if (size == 0) {
			return GL_FALSE;
		}
		else
		{
			switch(size)
			{
			case 4:
				for (GLint i = 0; i < total; i++) {
					COPY4V(pPPBuf->texCoord, pBuffer);
					pBuffer += stride;
					pPPBuf++;
				}
				break;
			case 3:
				for (GLint i = 0; i < total; i++) {
					COPY3V(pPPBuf->texCoord, pBuffer);
					pPPBuf->texCoord[T_Q] = attribDefVal[VTX_TEXCOORD][T_Q];
					pBuffer += stride;
					pPPBuf++;
				}
				break;
			case 2:
				for (GLint i = 0; i < total; i++) {
					COPY2V(pPPBuf->texCoord, pBuffer);
					COPY2V(&(pPPBuf->texCoord[2]), 
						&attribDefVal[VTX_TEXCOORD][2]);

					pBuffer += stride;
					pPPBuf++;
				}
				break;
			case 1:
				for (GLint i = 0; i < total; i++) {
					COPY1V(pPPBuf->texCoord, pBuffer);
					COPY3V(&(pPPBuf->texCoord[1]), 
						&attribDefVal[VTX_TEXCOORD][1]);

					pBuffer += stride;
					pPPBuf++;
				}
				break;
			default:
				assert(false);
				return GL_FALSE;
				break;
			}
		}

		return GL_TRUE;
	}


	GLboolean runRastPipeline()
	{
		Vertex * pPPBuf = CTXPTR->getPPBuffer();
		if (pPPBuf == NULL) {
			return GL_FALSE;
		}
		GLuint size = CTXPTR->getVertexBufferPtr()->getVertexCount();

		GLmatrix viewMat = CTXPTR->viewMatrix;
		if (viewMat == NULL) {
			return GL_FALSE;
		}

		Vertex * pTemp = pPPBuf;
		for(GLuint i = 0; i < size; i++)
		{
			Matrix::transVector3f(pTemp->coord, CTXPTR->projMatrix, pTemp->coord);		
			Matrix::transVector3f(pTemp->coord, viewMat, pTemp->coord);
			pTemp++;
		}

		switch(CTXPTR->beginMode)
		{
		case GL_POINTS:
			rastPoints(pPPBuf, size);
			break;
		case GL_LINES:
			rastLines(pPPBuf, size);
			break;
		case GL_LINE_LOOP:
			rastLineLoop(pPPBuf, size);
			break;
		case GL_LINE_STRIP:
			rastLineStrip(pPPBuf, size);
			break;
		case GL_TRIANGLES:
			rastTriangles(pPPBuf, size);
			break;
		case GL_TRIANGLE_STRIP:
			rastTriangleStrip(pPPBuf, size);
			break;
		case GL_TRIANGLE_FAN:
			rastTriangleFan(pPPBuf, size);
			break;
		case GL_QUADS:
			rastQuads(pPPBuf, size);
			break;
		case GL_QUAD_STRIP:
			rastQuadStrip(pPPBuf, size);
			break;
		case GL_POLYGON:
			rastPolygon(pPPBuf, size);
			break;
		default:
			return GL_FALSE;
		}

		return GL_TRUE;
	}
}

// internal functions


GLboolean copyColor()
{
	VertexBuffer * pVtxBuf = CTXPTR->getVertexBufferPtr();
	if (pVtxBuf == NULL) {
		return GL_FALSE;
	}

	Vertex * pPPBuf = CTXPTR->getPPBuffer();
	if (pPPBuf == NULL) {
		return GL_FALSE;
	}

	BufferDesc desc;
	pVtxBuf->getBufferDesc(&desc, VTX_COLOR);

	const GLfloat * pBuffer = desc.pBuf;
	if (pBuffer == NULL) {
		desc.elemSize = 0;
	}

	GLint size = desc.elemSize;
	GLint total = desc.total;
	GLint stride = desc.stride;

	if (size == 0) {
		// no color
		GLfloat c[4];
		memCopyf(c, attribDefVal[VTX_COLOR], 4);
		for (GLint i = 0; i < total; i++) {

			COPY4V(pPPBuf->color, c);
			pPPBuf++;
		}
	}
	else
	{
		switch(size)
		{
		case 4:
			for (GLint i = 0; i < total; i++) {
				COPY4V(pPPBuf->color, pBuffer);
				pBuffer += stride;
				pPPBuf++;
			}
			break;
		case 3:
			for (GLint i = 0; i < total; i++) {
				COPY3V(pPPBuf->color, pBuffer);
				pPPBuf->color[IDXA] = attribDefVal[VTX_COLOR][IDXA];
				pBuffer += stride;
				pPPBuf++;
			}
			break;
		default:
			assert(false);
			return GL_FALSE;
			break;
		}
	}

	return GL_TRUE;
}

GLboolean rastPoints(Vertex * pBuf, GLuint size)
{
	return GL_TRUE;
}

GLboolean rastLines(Vertex * pBuf, GLuint size)
{
	return GL_TRUE;
}

GLboolean rastLineStrip(Vertex * pBuf, GLuint size)
{
	return GL_TRUE;
}

GLboolean rastLineLoop(Vertex * pBuf, GLuint size)
{
	return GL_TRUE;
}

GLboolean rastTriangles(Vertex * pBuf, GLuint size)
{
	size /= 3;
	for (GLuint i = 0; i < size; i++) {
		Raster::drawTriangles(pBuf, pBuf + 1, pBuf + 2);
		pBuf += 3;
	}
	return GL_TRUE;
}

GLboolean rastTriangleStrip(Vertex * pBuf, GLuint size)
{
	return GL_TRUE;
}

GLboolean rastTriangleFan(Vertex * pBuf, GLuint size)
{
	return GL_TRUE;
}

GLboolean rastQuads(Vertex * pBuf, GLuint size)
{
	size /= 4;
	for (GLuint i = 0; i < size; i++) {
		Raster::drawQuads(pBuf, pBuf + 1, pBuf + 2, pBuf + 3);
		pBuf += 4;
	}
	return GL_TRUE;
}

GLboolean rastQuadStrip(Vertex * pBuf, GLuint size)
{
	return GL_TRUE;
}

GLboolean rastPolygon(Vertex * pBuf, GLuint size)
{
	return GL_TRUE;
}