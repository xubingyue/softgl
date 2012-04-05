/************************************************************************/
/* File		: impltype.h
/* Internal GL Type  
/* Date		: 2006/4/6
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/


#ifndef IMPLTYPE_H
#define IMPLTYPE_H


#define IDXR	0
#define IDXG	1
#define IDXB	2
#define IDXA	3

#define IDXX	0
#define IDXY	1
#define IDXZ	2
#define IDXW	3

#define T_S		0
#define T_T		1
#define T_R		2
#define T_Q		3

#define GLRGBA(color, r, g, b, a) (color)[IDXR]	 = (r); \
								  (color)[IDXG]	 = (g); \
								  (color)[IDXB]	 = (b); \
								  (color)[IDXA]  = (a);

#define GLRGB(color, r, g, b)	  (color)[IDXR]	 = (r); \
								  (color)[IDXG]	 = (g); \
								  (color)[IDXB]	 = (b); \
								  (color)[IDXA]  = 1.0f;
								 
#define COLORF2B(c) (GLubyte)((c) * 255.0f)

#define AR2RGB(rgb)   (RGB((rgb)[IDXR], (rgb)[IDXG], (rgb)[IDXB]))


#define RF2UB(color)  COLORF2B(((color)[IDXR]))
#define GF2UB(color)  COLORF2B(((color)[IDXG]))
#define BF2UB(color)  COLORF2B(((color)[IDXB]))
#define AF2UB(color)  COLORF2B(((color)[IDXA]))

#define GLCOLOR2RGB(color)  RGB(RF2UB(color),	\
							GF2UB(color),  \
							BF2UB(color))

#define GLC2RGBCOPY(dest, src) dest[IDXR] = RF2UB(src); \
								dest[IDXG] = GF2UB(src); \
								dest[IDXB] = BF2UB(src); \
								dest[IDXA] = AF2UB(src);

#define F3VASSIGN(v, x, y, z) (v)[0] = (x), (v)[1] = (y), (v)[2] = (z) 
#define F4VASSIGN(v, x, y, z, w) (v)[0] = (x), (v)[1] = (y), (v)[2] = (z), (v)[3] = (w) 

#define F4VMUL(o, v0, v1) (o)[0] = (v0)[0] * (v1)[0];  \
						  (o)[1] = (v0)[1] * (v1)[1];  \
						  (o)[2] = (v0)[2] * (v1)[2];  \
						  (o)[3] = (v0)[3] * (v1)[3];  

#define F4VADD(o, v0, v1) (o)[0] = (v0)[0] + (v1)[0];  \
							(o)[1] = (v0)[1] + (v1)[1];  \
							(o)[2] = (v0)[2] + (v1)[2];  \
							(o)[3] = (v0)[3] + (v1)[3];  


#define F4VMIN(o, v0, v1) (o)[0] = (v0)[0] - (v1)[0];  \
							(o)[1] = (v0)[1] - (v1)[1];  \
							(o)[2] = (v0)[2] - (v1)[2];  \
							(o)[3] = (v0)[3] - (v1)[3];  


#define F4VDIV(o, v0, v1) (o)[0] = (v0)[0] / (v1)[0];  \
							(o)[1] = (v0)[1] / (v1)[1];  \
							(o)[2] = (v0)[2] / (v1)[2];  \
							(o)[3] = (v0)[3] / (v1)[3];  


#define F3VMUL(o, v0, v1) (o)[0] = (v0)[0] * (v1)[0];  \
							(o)[1] = (v0)[1] * (v1)[1];  \
							(o)[2] = (v0)[2] * (v1)[2];

#define F3VADD(o, v0, v1) (o)[0] = (v0)[0] + (v1)[0];  \
							(o)[1] = (v0)[1] + (v1)[1];  \
							(o)[2] = (v0)[2] + (v1)[2]; 


#define F3VMIN(o, v0, v1) (o)[0] = (v0)[0] - (v1)[0];  \
							(o)[1] = (v0)[1] - (v1)[1];  \
							(o)[2] = (v0)[2] - (v1)[2];  


#define F3VDIV(o, v0, v1) (o)[0] = (v0)[0] / (v1)[0];  \
							(o)[1] = (v0)[1] / (v1)[1];  \
							(o)[2] = (v0)[2] / (v1)[2];


#define F4MULI(v, i) (v)[0] *= (i), (v)[1] *= (i), (v)[2] *= (i), (v)[3] *= (i);
#define F4DIVI(v, i) (v)[0] /= (i), (v)[1] /= (i), (v)[2] /= (i), (v)[3] /= (i);


#define LENGTH3V2(v) (v)[0] * (v)[0] + (v)[1] * (v)[1] + (v)[2] * (v)[2]

#define LENGTH3V(v) (sqrt(LENGTH3V2(v)))


#define DISTANCE2(v1, v2)  ((v1)[0] * (v2)[0] + (v1)[1] * (v2)[1] + (v1)[3] * (v2)[3])
#define DOTPROD3V(v1, v2) ((v1)[0] * (v2)[0] + (v1)[1] * (v2)[1] + (v1)[2] * (v2)[2])

typedef GLfloat GLcolor[4];
typedef GLfloat * GLmatrix;

#define CMINF 0.0f
#define CMAXF 255.0f

#define CMINI 0
#define CMAXI 255

#define CLAMP(x, min, max) x = ((x)<(min) ? (min) : ((x)>(max) ? (max) : (x)))



#define COPY1V(dest, src)  (dest)[0] = (src)[0]; 



#define COPY2V(dest, src)  (dest)[0] = (src)[0]; \
							(dest)[1] = (src)[1]; 



#define COPY3V(dest, src)  (dest)[0] = (src)[0]; \
							(dest)[1] = (src)[1]; \
							(dest)[2] = (src)[2]; 


#define COPY4V(dest, src)  (dest)[0] = (src)[0]; \
						   (dest)[1] = (src)[1]; \
						   (dest)[2] = (src)[2]; \
						   (dest)[3] = (src)[3];

#define FIXEDSHIFT 16
#define FIXED_ONE  (1 << FIXEDSHIFT)


#define GL_CLEAR_MAX				(GL_COLOR_BUFFER_BIT   | \
										GL_ACCUM_BUFFER_BIT   | \
										GL_STENCIL_BUFFER_BIT | \
										GL_DEPTH_BUFFER_BIT)

#define BGR32(c) (unsigned long)((DWORD)(((BYTE)((c)[IDXB])| \
										((WORD)((BYTE)((c)[IDXG]))<<8))| \
										(((DWORD)(BYTE)((c)[IDXR]))<<16)))


#define _SETPIXEL32(x, y, rgb) { LPDWORD lpdw = ((LPDWORD)	\
	(pPixelBuffer + ScanWidth * (y)) + (x)); \
	*lpdw = BGR32(rgb); }



#define M_PI       3.14159265358979323846f
#define EPS			0.00001f
#define EQUAL0(f) ((f) < EPS) && ((f) > -EPS)
#define DEG2RAD(x) ((x) * (M_PI / 180.0f))

#define EAUALRGB(c1, c2) ( EQUAL0((c1)[IDXR] - (c2)[IDXG]) &&   \
							 EQUAL0((c1)[IDXG] - (c2)[IDXG]) && \
						     EQUAL0((c1)[IDXB] - (c2)[IDXB]))   


#define EAUALRGBA(c1, c2) ( EQUAL0((c1)[IDXR] - (c2)[IDXG]) &&   \
							 EQUAL0((c1)[IDXG] - (c2)[IDXG]) && \
							 EQUAL0((c1)[IDXB] - (c2)[IDXB]) && \
							EQUAL0((c1)[IDXA] - (c2)[IDXA]))

// GL specify that matrix is column-major
// below is the access macro
#define M00(m) (m)[0]
#define M10(m) (m)[1]
#define M20(m) (m)[2]
#define M30(m) (m)[3]
#define M01(m) (m)[4]
#define M11(m) (m)[5]
#define M21(m) (m)[6]
#define M31(m) (m)[7]
#define M02(m) (m)[8]
#define M12(m) (m)[9]
#define M22(m) (m)[10]
#define M32(m) (m)[11]
#define M03(m) (m)[12]
#define M13(m) (m)[13]
#define M23(m) (m)[14]
#define M33(m) (m)[15]

#define I(r,c) (((c) << 2) + (r))  // row * 4 + col 


// attribute set
#define VTX_COORD				0
#define VTX_COLOR				1
#define VTX_NORMAL				2
#define VTX_TEXCOORD			3
#define VTX_F_AMBIENT			4
#define VTX_B_AMBIENT			5
#define VTX_F_DIFFUSE			6
#define VTX_B_DIFFUSE			7
#define VTX_F_SPECULAR			8
#define VTX_B_SPECULAR			9
#define VTX_F_SHININESS			10
#define VTX_B_SHININESS			11
#define VTX_F_EMISSION			12
#define VTX_B_EMISSION			13
#define VTX_ATTRIB_COUNT		14	// end of attribute set


const GLfloat attribDefVal[VTX_ATTRIB_COUNT][4] = {
	{0.0f, 0.0f, 0.0f, 1.0f},   // VTX_COORD
	{1.0f, 1.0f, 1.0f, 1.0f},   // VTX_COLOR
	{0.0f, 0.0f, 0.0f, 1.0f},   // VTX_NORMAL
	{0.0f, 0.0f, 0.0f, 1.0f},   // VTX_TEXCOORD
	{0.2f, 0.2f, 0.2f, 1.0f},   // VTX_F_AMBIENT
	{0.2f, 0.2f, 0.2f, 1.0f},   // VTX_B_AMBIENT
	{0.8f, 0.8f, 0.8f, 1.0f},   // VTX_F_DIFFUSE
	{0.8f, 0.8f, 0.8f, 1.0f},   // VTX_B_DIFFUSE
	{0.0f, 0.0f, 0.0f, 1.0f},   // VTX_F_SPECULAR
	{0.0f, 0.0f, 0.0f, 1.0f},   // VTX_B_SPECULAR
	{0.0f, 0.0f, 0.0f, 0.0f},   // VTX_F_SHININESS
	{0.0f, 0.0f, 0.0f, 0.0f},   // VTX_B_SHININESS
	{0.0f, 0.0f, 0.0f, 1.0f},   // VTX_F_EMISSION
	{0.0f, 0.0f, 0.0f, 1.0f}    // VTX_B_EMISSION
};

#define VTXBUF_INITBUFSIZE		4800
#define DEFPIPELINESIZE			4096
#define MAXSPANLENGTH			16384
#define MATRIX_MAXDEPTH			32
#define MATRIX_SIZE				16




#endif