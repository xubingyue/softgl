/************************************************************************/
/* File		: rast.cpp
/* rasterization fcuntions
/* Date		: 2006/4/25
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/

#include "rast.h"
#include "context.h"
#include "vertex.h"
#include "profiler.h"
#include <cmath>


struct SpanExtraData
{
	GLubyte color[MAXSPANLENGTH][4];
};

SpanExtraData spExtra;


struct Span
{
	GLint			x, y;
	GLuint			len;
	GLfloat			color[4];
	GLfloat			colorDelta[4];
	GLfloat			texCoord[4];
	GLfloat			texCoordDelta[4];
	SpanExtraData * pExtra;
	GLenum			filter;
	GLubyte			texEnvColor[4];

	Span()
	{
		x = 0, y = 0;
		len = 0;
		memSetf(color, 0, 4);
		memSetf(colorDelta, 0, 4);
		pExtra = &spExtra;
	}
};


namespace Raster
{
// template expanding
#define RAST_SPAN_FUNC
	// span RGBA
	#define FUNCNAME drawSpanSmoothRGBA
	#define SPAN_ALPHA
	#define SPAN_SMOOTH
	#include "rast_template.h"

	#define FUNCNAME   drawSpanFlatRGBA
	#define SPAN_ALPHA
	#include "rast_template.h"

	// span RGB
	#define FUNCNAME drawSpanSmoothRGB
	#define SPAN_SMOOTH
	#include "rast_template.h"	

	#define FUNCNAME   drawSpanFlatRGB
	#include "rast_template.h"	

	#define FUNCNAME drawSmoothTexSpan
	#define SPAN_SMOOTH
	#define SPAN_ALPHA
	#define SPAN_TEX
	#include "rast_template.h"

	#define FUNCNAME drawFlatTexSpan
	#define SPAN_TEX
	#include "rast_template.h"

#undef RAST_SPAN_FUNC



#define RAST_TRI_FUNC
//FlatRGBATriangle
	#define FUNCNAME FlatRGBATriangle 
	#define RAST_ALPHA
	#include "rast_template.h"

//SmoothRGBATriangle
	#define FUNCNAME SmoothRGBATriangle
	#define RAST_ALPHA
	#define RAST_SMOOTH 
	#include "rast_template.h"

//FlatRGBTriangle
	#define FUNCNAME FlatRGBTriangle 
	#include "rast_template.h"

//SmoothRGBTriangle
	#define FUNCNAME SmoothRGBTriangle
	#define RAST_SMOOTH
	#include "rast_template.h"

// smRGBATexTri
	#define FUNCNAME smRGBATexTri
	#define RAST_SMOOTH
	#define RAST_ALPHA
	#define RAST_TEX
	#include "rast_template.h"

// flatRGBATexTri
	#define FUNCNAME flatRGBATexTri
	#define RAST_ALPHA
	#define RAST_TEX
	#include "rast_template.h"

#undef RAST_TRI_FUNC
// end of template expanding

	void drawTriangles(Vertex * v0, Vertex * v1, Vertex * v2)
	{
		GLenum shadeMode = CTXPTR->shadeModel;
		GLenum tex2dMode = CTXPTR->currentState[GL_TEXTURE_2D];

		if (tex2dMode == GL_TRUE) {
			smRGBATexTri(v0, v1, v2);
			return;
		}

		if (shadeMode == GL_FLAT) {
			FlatRGBTriangle(v0, v1, v2);
			return;
		}

		// color test
		if(EAUALRGB(v0->color, v1->color) && 
			EAUALRGB(v1->color, v2->color))
		{
			FlatRGBTriangle(v0, v1, v2);
			return;
		}
		SmoothRGBTriangle(v0, v1, v2);
		return;
	}

	void drawQuads(Vertex * v0, Vertex * v1, Vertex * v2, Vertex * v3)
	{
		GLenum shadeMode = CTXPTR->shadeModel; 

		if (shadeMode == GL_FLAT) {  
			COPY3V(v0->color, v2->color);  
		}
		drawTriangles(v0, v1, v2); 
		drawTriangles(v2, v3, v0); 
	}
}



#ifdef _DEBUG



void testRaster()
{
	//OPENPROFILER("C:\\testRaster.txt");

	Vertex v0, v1, v2, v3;



	v0.color[0] = 0, v0.color[1] = 0, v0.color[2] = 1;
	v0.coord[0] = 0, v0.coord[1] = 0;
	v0.texCoord[T_S] = 0.0f, v0.texCoord[T_T] = 0.0f;

	v1.color[0] = 0, v1.color[1] = 1.0f, v1.color[2] = 0;
	v1.coord[0] = 500, v1.coord[1] = 0;
	v1.texCoord[T_S] = 1.0f, v1.texCoord[T_T] = 0.0f;
	
	v2.color[0] = 1.0, v2.color[1] = 0, v2.color[2] = 0;
	v2.coord[0] = 500, v2.coord[1] = 500;
	v2.texCoord[T_S] = 1.0f, v2.texCoord[T_T] = 1.0f;

	v3.color[0] = 1.0, v3.color[1] = 0, v3.color[2] = 0;
	v3.coord[0] = 0, v3.coord[1] = 500;
	v3.texCoord[T_S] = 0.0f, v3.texCoord[T_T] = 1.0f;

	Raster::smRGBATexTri(&v0, &v1, &v2);

	CTXPTR->pOsCtx->flip();
}
#endif