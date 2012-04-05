/************************************************************************/
/* File		: rast.h
/* rasterization fcuntions
/* Date		: 2006/4/25
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/

#ifndef  RAST_H
#define	 RAST_H

struct Span;
struct Vertex;

namespace Raster
{
//  Triangles
	void drawTriangles(Vertex * v0, Vertex * v1, Vertex * v2);
// Quads
	void drawQuads(Vertex * v0, Vertex * v1, Vertex * v2, Vertex * v3);
}




#ifdef _DEBUG
	void loadTex();

	void testRaster();
#endif


#endif
