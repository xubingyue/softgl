/************************************************************************/
/* File		: gltypes.h
/* GL library type definitions
/* Date		: 2006/3/10
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/


#ifndef GLTYPES_H
#define GLTYPES_H

typedef int				GLint;		
typedef unsigned int	GLuint;		
typedef float			GLfloat;	
typedef double			GLdouble;	
typedef unsigned char	GLboolean;
typedef int				GLsizei;
typedef unsigned char	GLubyte;
typedef unsigned int	GLenum;
typedef unsigned int	GLbitfield;
typedef float			GLclampf;
typedef void            GLvoid;

#define GL_FALSE				0x0
#define GL_TRUE					0x1

#define GL_PROJECTION	0x00
#define GL_MODELVIEW	0x01

#define GL_NO_ERROR				0x00
#define GL_INVALID_ENUM			0x01
#define GL_INVALID_VALUE		0x02
#define GL_INVALID_OPERATION	0x03
#define GL_STACK_OVERFLOW		0x04
#define GL_STACK_UNDERFLOW		0x05
#define GL_OUT_OF_MEMORY		0x06

// Enable flga
#define GL_LIGHTING				0
#define GL_NORMALIZE			1
#define GL_TEXTURE_1D			2
#define GL_TEXTURE_2D			3
#define GL_CULL_FACE			4
#define GL_LIGHT0				5
#define GL_LIGHT1				6
#define GL_LIGHT2				7
#define GL_LIGHT3				8
#define GL_LIGHT4				9
#define GL_LIGHT5				10
#define GL_LIGHT6				11
#define GL_LIGHT7				12
#define GL_FLAGMAX				13

#define GL_LIGHTBASE			5
#define GL_MAX_LIGHTS			8



// shade model
#define GL_SMOOTH				0
#define GL_FLAT					1

// Front face
#define GL_CW					0
#define GL_CCW					1

// Culled face			
#define GL_FRONT				0
#define GL_BACK					1
#define GL_FRONT_AND_BACK		2

// Begin mode
#define GL_POINTS                         0x0000
#define GL_LINES                          0x0001
#define GL_LINE_LOOP                      0x0002
#define GL_LINE_STRIP                     0x0003
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_STRIP                 0x0005
#define GL_TRIANGLE_FAN                   0x0006
#define GL_QUADS                          0x0007
#define GL_QUAD_STRIP                     0x0008
#define GL_POLYGON                        0x0009


// bit mask
#define GL_DEPTH_BUFFER_BIT			0x00000001
#define GL_ACCUM_BUFFER_BIT			0x00000002
#define GL_STENCIL_BUFFER_BIT		0x00000004
#define GL_COLOR_BUFFER_BIT			0x00000008


#define GL_RGB				0
#define GL_RGBA				1
#define GL_RED				2
#define GL_BLUE				3
#define GL_GREEN			4
#define GL_LUMINANCE		5

#define GL_UNSIGNED_BYTE    0
#define GL_BYTE				1
#define GL_UNSIGNED_SHORT   2
#define GL_SHORT			3
#define GL_UNSIGNED_INT		4
#define GL_INT				5
#define GL_FLOAT			6


#define GL_TEXTURE_ENV_MODE  0
#define GL_TEXTURE_ENV_COLOR 1
#define GL_TEXTURE_ENV		 2

#define GL_MODULATE			0
#define GL_DECAL			1
#define GL_BLEND			2
#define GL_REPLACE			3


#define GL_TEXTURE_MIN_FILTER		0
#define GL_TEXTURE_MAG_FILTER		1
#define GL_TEXTURE_WRAP_S			2
#define GL_TEXTURE_WRAP_T			3


#define GL_NEAREST					0
#define GL_LINEAR					1

#define GL_REPEAT			0
#define GL_CLAMP			1

#define GL_FRONT			0
#define GL_BACK				1
#define	GL_FRONT_AND_BACK	2

#define GL_AMBIENT					0
#define GL_DIFFUSE					1
#define GL_SPECULAR					2
#define GL_EMISSION					3
#define GL_SHININESS				4
#define GL_AMBIENT_AND_DIFFUSE		5
#define GL_POSITION					6
#define GL_SPOT_DIRECTION			7
#define GL_SPOT_EXPONENT			8
#define GL_SPOT_CUTOFF				9
#define GL_CONSTANT_ATTENUATION		10
#define GL_LINEAR_ATTENUATION		11
#define GL_QUADRATIC_ATTENUATION	12
#define GL_LIGHT_MODEL_LOCAL_VIEWER 13
#define GL_LIGHT_MODEL_TWO_SIDE		14
#define GL_LIGHT_MODEL_AMBIENT		15





#undef  NULL
#define NULL	0

#endif