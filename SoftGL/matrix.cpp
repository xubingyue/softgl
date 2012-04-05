/************************************************************************/
/* File		: matrix.cpp
/* Matrix calculation functions
/* Date		: 2006/3/10
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/


#include "globdef.h"
#include "matrix.h"
#include <cassert>
#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;




namespace Matrix
{
	const GLfloat Identity[16] = { 
		1.0,  0,  0,   0,
		0,   1.0, 0,   0,
		0,    0,  1.0, 0,
		0,    0,  0,   1.0
	};

	const GLfloat ZeroMatrix[16] = {
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};

	// 4x4 matrix multiply
	void mul44(GLfloat * mo, GLfloat * ml, GLfloat * mr)
	{
		assert(mo != NULL && ml != NULL && mr != NULL);

		GLfloat mt[16];

		for(GLint r = 0; r < 4; r++)
			for (GLint c = 0; c < 4; c++) 
			{
				mt[I(r,c)] = 
					ml[I(r,0)] * mr[I(0,c)] +
					ml[I(r,1)] * mr[I(1,c)] +
					ml[I(r,2)] * mr[I(2,c)] +
					ml[I(r,3)] * mr[I(3,c)];
			}
		memcpy(mo, mt, sizeof(mt));
		return;
	}


	// set m to identity matrix
	void setIdentity(GLfloat * m)
	{
		assert(m != NULL);
		memcpy(m, Identity, sizeof(Identity));
		return;
	}


	// if a matrix is identity
	GLboolean	isIdentity(GLfloat * m)
	{
		assert(m != NULL);
		for (GLint i = 0 ; i < 16; i++) {
			if (m[i] != Identity[i]) {
				return GL_FALSE;
			}
		}
		return GL_TRUE;
	}

	GLfloat matDET(GLfloat * m)
	{
		assert(m != NULL);
		GLfloat det00 = M00(m) * (
			+	M11(m) * (M22(m) * M33(m) - M23(m) * M32(m))
			-   M12(m) * (M21(m) * M33(m) - M23(m) * M31(m))
			+   M13(m) * (M21(m) * M32(m) - M22(m) * M31(m))
			);
		GLfloat det10 = -1 *  M10(m) * (
			+	M01(m) * (M22(m) * M33(m) - M23(m) * M32(m))
			-	M02(m) * (M21(m) * M33(m) - M23(m) * M31(m))
			+	M03(m) * (M21(m) * M32(m) - M22(m) * M31(m))
			);
		GLfloat det20 = M20(m) * (
			+	M01(m) * (M12(m) * M33(m) - M13(m) * M32(m))
			-	M02(m) * (M11(m) * M33(m) - M13(m) * M31(m))
			+	M03(m) * (M11(m) * M32(m) - M12(m) * M31(m))
			);
		GLfloat det30 = -1 * M30(m) * (
			+	M01(m) * (M12(m) * M23(m) - M13(m) * M22(m))
			-	M02(m) * (M11(m) * M23(m) - M13(m) * M21(m))
			+	M03(m) * (M11(m) * M22(m) - M12(m) * M21(m))
			);

		return det00 + det10 + det20 + det30;
	}

	void transVector2f(GLfloat * vo, const GLfloat * m, const GLfloat * vi)
	{
		assert(vo != NULL && m != NULL && vi != NULL);
		GLfloat vt[4];
		// vi[3] = 1, vi[2] = 0
		for(GLint i = 0; i < 4; i++)
		{
			vt[i] = m[I(i,0)] * vi[0] +
				m[I(i,1)] * vi[1] +
				m[I(i,3)];
		}
		memcpy(vo, vt, sizeof(vt));
		return;
	}

	void transVector3f(GLfloat * vo, const GLfloat * m, const GLfloat * vi)
	{
		assert(vo != NULL && m != NULL && vi != NULL);
		GLfloat vt[4];
		// vi[3] = 1
		for(GLint i = 0; i < 4; i++)
		{
			vt[i] = m[I(i,0)] * vi[0] +
				m[I(i,1)] * vi[1] +
				m[I(i,2)] * vi[2] +
				m[I(i,3)];
		}
		memcpy(vo, vt, sizeof(vt));
		return;
	}

	void transVector4f(GLfloat * vo, const GLfloat * m, const GLfloat * vi)
	{
		assert(vo != NULL && m != NULL && vi != NULL);
		GLfloat vt[4];
		for(GLint i = 0; i < 4; i++)
		{
			vt[i] = m[I(i,0)] * vi[0] +
					m[I(i,1)] * vi[1] +
					m[I(i,2)] * vi[2] +
					m[I(i,3)] * vi[3];
		}
		memcpy(vo, vt, sizeof(vt));
		return;
	}

	void vecHomotrans(GLfloat * v)
	{   
		assert(v != NULL);

		GLfloat w = v[3];
		
		if (EQUAL0(w)) {
			return;
		}

		v[0] = v[0] / w;
		v[1] = v[1] / w;
		v[2] = v[2] / w;
		v[3] = 1;
	}

	void transpose(GLfloat * m)
	{
		for (GLint r = 0; r < 3; r++) {
			for (GLint c = r + 1; c < 4; c++) {
				GLfloat t = m[I(r,c)];
				m[I(r,c)] = m[I(c,r)];
				m[I(c,r)] = t;
			}
		}
	}

	// translate transformation
	void translate(GLfloat * m, GLfloat x, GLfloat y, GLfloat z)
	{
		assert(m != NULL);
		
		GLfloat vt[3];
		vt[0] = x; vt[1] = y; vt[2] = z;

		for (GLint c = 0; c < 4; c++) {
			for (GLint r = 0; r < 3; r++) {
				int i = I(r,c), j = I(3,c);
				m[I(r,c)] = m[I(r,c)] + vt[r] * m[I(3,c)];
			}
		}
	}

	// scale transformation
	void scale(GLfloat * m, GLfloat x, GLfloat y, GLfloat z)
	{
		assert(m != NULL);
		GLfloat vt[3];
		vt[0] = x; vt[1] = y; vt[2] = z;
		for (GLint r = 0; r < 3; r++) {
            for (GLint c = 0; c < 4; c++) {
				m[I(r,c)] = m[I(r,c)] * vt[r];
            }
		}
		return;
	}

	// rotate
	void rotateX(GLfloat * m, GLfloat angle, GLfloat x)
	{
		assert(m != NULL);
		GLfloat mt[16];
		angle = DEG2RAD(angle);
		GLfloat cs = cos(angle);
		GLfloat si = sin(angle);

		setIdentity(mt);
		M11(mt) = cs;
		M22(mt) = cs;
		if (x < 0.0F) {
			M12(mt) =   si;
			M21(mt) = -si;
		}
		else {
			M12(mt) = -si;
			M21(mt) = si;
		}
		mul44(m, m, mt);
	}


	void rotateY(GLfloat * m, GLfloat angle, GLfloat y)
	{
		assert(m != NULL);
		GLfloat mt[16];
		angle = DEG2RAD(angle);
		GLfloat cs = cos(angle);
		GLfloat si = sin(angle);

		setIdentity(mt);
		M00(mt) = cs;
		M22(mt) = cs;
		if (y < 0.0F) {
			M02(mt) = -si;
			M20(mt) = si;
		}
		else {
			M02(mt) = si;
			M20(mt) = -si;
		}
		mul44(m, m, mt);
	}

	void rotateZ(GLfloat * m, GLfloat angle, GLfloat z)
	{
		assert(m != NULL);
		GLfloat mt[16];
		angle = DEG2RAD(angle);
		GLfloat cs = cos(angle);
		GLfloat si = sin(angle);

		setIdentity(mt);

		M00(mt) = cs;
		M11(mt) = cs;
		if (z < 0.0F) {
			M01(mt) = si;
			M10(mt) = -si;
		}
		else {
			M01(mt) = -si;
			M10(mt) = si;
		}
		mul44(m, m, mt);
	}

	void rotate(GLfloat * m, GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
	{
		assert(m != NULL);

		if (EQUAL0(y) && EQUAL0(z)) {
			rotateX(m, angle, x);
			return;
		}
		if (EQUAL0(x) && EQUAL0(z)) {
			rotateY(m, angle, y);
			return;
		}
		if (EQUAL0(x) && EQUAL0(y)) {
			rotateZ(m, angle, z);
			return;
		}


		GLfloat mt[16];

		angle = DEG2RAD(angle);
		setIdentity(mt);
		GLfloat cs = cos(angle);
		GLfloat si = sin(angle);

		const GLfloat l = sqrt(x * x + y * y + z * z);

		x /= l;		y /= l;		z /= l;

		M00(mt) = ((1 - cs) * x * x) + cs;
		M01(mt) = ((1 - cs) * x * y) - z * si;
		M02(mt) = ((1 - cs) * z * x) + y * si;

		M10(mt) = ((1 - cs) * x * y) + z * si;
		M11(mt) = ((1 - cs) * y * y) + cs;
		M12(mt) = ((1 - cs) * y * z) - x * si;

		M20(mt) = ((1 - cs) * z * x) - y * si;
		M21(mt) = ((1 - cs) * y * z) + x * si;
		M22(mt) = ((1 - cs) * z * z) + cs;

		mul44(m, m, mt);
		return;
	}


	void frustum(GLfloat * m, 
				 GLfloat l, GLfloat r,
				 GLfloat b, GLfloat t,
				 GLfloat n, GLfloat f)
	{
		assert(m != NULL);
		GLfloat mt[16];
		memcpy(mt, ZeroMatrix, sizeof(ZeroMatrix));

		M00(mt) = (n * 2) / (r - l);
		M02(mt) = (r + l) / (r - l);
		M11(mt) = (n * 2) / (t - b);
		M12(mt) = (t + b) / (t - b);
        M22(mt) = -(f + n) / (f - n);
		M23(mt) = -(f * n * 2) / (f - n);
		M32(mt) = -1;
		mul44(m, m, mt);
		return;
	}


	void ortho(GLfloat *m, 
		GLfloat l, GLfloat r,
		GLfloat b, GLfloat t,
		GLfloat n, GLfloat f)
	{
		assert(m != NULL);
		GLfloat mt[16];
		memcpy(mt, Identity, sizeof(Identity));
        
		M00(mt) = 2 / (r - l);
		M11(mt) = 2 / (t - b);
		M22(mt) = -2 / (f - n);
		M03(mt) = (r + l) / (r - l);
		M13(mt) = (t + b) / (t - b);
		M23(mt) = (f + n) / (f - n);

		mul44(m, m, mt);
		return;
	}

	void viewport(GLfloat * m, GLint x, GLint y, GLsizei w, GLsizei h)
	{
		assert(m != NULL);

		memcpy(m, Identity, sizeof(Identity));
		// we don't transform z value
		M00(m) = (GLfloat)w / 2;  
		M03(m) = M00(m) + x;  // move to origin
		M11(m) = (GLfloat)h / 2;
		M13(m) = M11(m) + y;

		return;
	}
}





