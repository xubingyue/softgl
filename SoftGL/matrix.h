/************************************************************************/
/* File		: matrix.h
/* Matrix calculation functions
/* Date		: 2006/3/10
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/

#ifndef MATRIX_H
#define MATRIX_H

#include "globdef.h"

namespace Matrix
{
	// 4x4 matrix multiply
	void mul44(GLfloat * mo, GLfloat * ml, GLfloat * mr);

	// set m to identity matrix
	void setIdentity(GLfloat * m);

	// if a matrix is identity
	GLboolean isIdentity(GLfloat * m);

	// perform transform on a vector
	void transVector2f(GLfloat * vo, const GLfloat * m, const GLfloat * vi);
	void transVector3f(GLfloat * vo, const GLfloat * m, const GLfloat * vi);
	void transVector4f(GLfloat * vo, const GLfloat * m, const GLfloat * vi);

	// calculate DET of the matrix
	GLfloat matDET(GLfloat * m);

	// divide x,y,z with w
	void vecHomotrans(GLfloat * v);

	// transpose matrix
	void transpose(GLfloat * m);

	// translate transformation
	void translate(GLfloat * m, GLfloat x, GLfloat y, GLfloat z);

	// scale transformation
	void scale(GLfloat * m, GLfloat x, GLfloat y, GLfloat z);

	// rotate by x,y,z axis
	void rotateX(GLfloat * m, GLfloat angle, GLfloat x);
	void rotateY(GLfloat * m, GLfloat angle, GLfloat y);
	void rotateZ(GLfloat * m, GLfloat angle, GLfloat z);
	void rotate(GLfloat * m, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

	// projection matrix
	void frustum(GLfloat *m, 
				 GLfloat l, GLfloat r,
				 GLfloat b, GLfloat t,
				 GLfloat n, GLfloat f);

	void ortho(GLfloat *m, 
				GLfloat l, GLfloat r,
				GLfloat b, GLfloat t,
				GLfloat n, GLfloat f);

	// viewport matrix
	void viewport(GLfloat * m, GLint x, GLint y, GLsizei w, GLsizei h);
}



#endif




