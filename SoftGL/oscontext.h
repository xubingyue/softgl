/************************************************************************/
/* File		: oscontext.h
/* Context about OS layer
/* Date		: 2006/3/10
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/

#ifndef OSCONTEXT_H
#define OSCONTEXT_H

#include "globdef.h"
#include "impltype.h"
#include <windows.h>
#include <cassert>
#include "dispFuncTbl.h"
#include "util.h"


//#define TY(y)		  (height - (y) - 1)
#define TY(y)		  y



class WDDBContext : public IDispFuncTbl
{
public:
	// from IDispFuncTbl
	void writePixel(GLint x, GLint y, const GLubyte rgb[4])
	{
		SetPixel(hbdc, x, TY(y), AR2RGB(rgb));
	}

	void writePixels(GLint x[], GLint y[], const GLubyte rgb[][4], GLint n)
	{
		for(GLint i = 0; i < n; i++)
			SetPixel(hbdc, x[i], TY(y[i]), AR2RGB(rgb[i]));
	}

	void writeSpan(GLint n, GLint x, GLint y, const GLubyte rgb[][4])
	{
		y = TY(y);
		for (GLint i = 0; i < n; i++) {
			SetPixel(hbdc, x + i, y, AR2RGB(rgb[i]));
		}
	}

	void writeSpan(GLint n, GLint x, GLint y, const GLubyte rgb[4])
	{
		y = TY(y);
		for (GLint i = 0; i < n; i++) {
			SetPixel(hbdc, x + i, y, AR2RGB(rgb));
		}
	}

	void clearBuffer()
	{
		RECT rc;
		rc.left = 0;
		rc.top = 0;
		rc.right = width;
		rc.bottom = height;
#ifdef _DEBUG
		int ret = 
#endif
		FillRect(hbdc, &rc, hbr);
	}

	GLboolean setClearColor(GLcolor color)
	{
		assert(color != NULL);
		if(color == NULL) {
			return GL_FALSE;
		}
#ifdef _DEBUG
		COLORREF ref = GLCOLOR2RGB(color);
		int r = GetRValue(ref);
		int g = GetGValue(ref);
		int b = GetBValue(ref);
#endif 
		HBRUSH hbr = CreateSolidBrush(GLCOLOR2RGB(color));
		if (hbr == 0) {
			return GL_FALSE;
		}
		this->hbr = hbr;

		return GL_TRUE;
	}

	void flip()
	{
#ifdef _DEBUG
		BOOL ret;
		ret = 
#endif
		BitBlt(hdc, x, y, width, height, hbdc, 0, 0, SRCCOPY);
	}

	GLboolean resizeBuffer(GLint x, GLint y, GLint w, GLint h)
	{
		assert(x >= 0 && y >= 0);
		this->x = x;
		this->y = y;
		if(!deleteDDBBuffer()) 
			return GL_FALSE;
		if (!createDDBBuffer(w, h)) {
			return GL_FALSE;
		}

		return GL_TRUE;
	}

	GLint getWidth()
	{
		return this->width;
	}

	GLint getHeight()
	{
		return this->height;
	}


	// factory method
	static IDispFuncTbl * createContext(HDC hdc, GLcolor color);
	~WDDBContext();
private:
	HDC			hdc;
	HDC			hbdc;

	HBITMAP		hbbm;
	HBITMAP		hOldBmp;

	HBRUSH		hbr;

	GLint		x;			// back buffer left
	GLint		y;			// back buffer top
	GLint		width;		// back buffer width
	GLint		height;		// back buffer height

	// should not be derived
	WDDBContext(){}

	// for windows itself
	GLboolean deleteDDBBuffer();
	GLboolean createDDBBuffer(GLint w, GLint h);

	// prepare OS display context
	// initially set to attach to the whole window area
	GLboolean prepare(HDC hdc, GLcolor color);
};



class WDIBContext : public IDispFuncTbl
{
public:
	// from IDispFuncTbl
	void writePixel(GLint x, GLint y, const GLubyte rgb[4])
	{
		_SETPIXEL32(x, TY(y), rgb);
	}

	void writePixels(GLint x[], GLint y[], const GLubyte rgb[][4], GLint n)
	{
		for(GLint i = 0; i < n; i++)
			_SETPIXEL32(x[i], TY(y[i]), rgb[i]);
	}

	void writeSpan(GLint n, GLint x, GLint y, const GLubyte rgb[][4])
	{
		y = TY(y);

		LPDWORD lpdw = ((LPDWORD)
			(pPixelBuffer + ScanWidth * (y)) + (x)); 

		for (GLint i = 0; i < n; i++) {
			*lpdw = BGR32(rgb[i]); 
			lpdw++;
		}
	}

	void writeSpan(GLint n, GLint x, GLint y, const GLubyte rgb[4])
	{
		y = TY(y);

		LPDWORD lpdw = ((LPDWORD)
			(pPixelBuffer + ScanWidth * (y)) + (x)); 

		DWORD color = BGR32(rgb);

		for (GLint i = 0; i < n; i++) {
			*lpdw = color; 
			lpdw++;
		}
	}

	void clearBuffer()
	{
		GLubyte c[4];
		GLC2RGBCOPY(c, clearColor);
		DWORD dwColor = BGR32(c);
		GLuint size = (ScanWidth * (height)) >> 2;
		LPDWORD lpdw = (LPDWORD)pPixelBuffer;

		for(GLuint i = 0; i < size; i++){
			*(lpdw) = dwColor;
			lpdw++;
		}
	}

	GLboolean setClearColor(GLcolor color)
	{
		assert(color != NULL);
		if(color == NULL) {
			return GL_FALSE;
		}
		memCopyf(clearColor, color, 4);

		return GL_TRUE;
	}

	void flip()
	{
		BitBlt(hdc, x, y, width, height, hbdc, 0, 0, SRCCOPY);
	}

	GLboolean resizeBuffer(GLint x, GLint y, GLint w, GLint h)
	{
		assert(x >= 0 && y >= 0);
		this->x = x;
		this->y = y;
		if(!deleteDIBBuffer()) 
			return GL_FALSE;
		if (!createDIBBuffer(w, h)) {
			return GL_FALSE;
		}

		return GL_TRUE;
	}

	GLint getWidth()
	{
		return this->width;
	}

	GLint getHeight()
	{
		return this->height;
	}


	// factory method
	static IDispFuncTbl * createContext(HDC hdc, GLcolor color);
	~WDIBContext();
private:
	HDC			hdc;
	HDC			hbdc;
	BITMAPINFO  bmi;
	HBITMAP     hbmDIB;
	HBITMAP     hOldBitmap;
	PBYTE       pPixelBuffer;
	GLuint		ScanWidth;
	GLcolor		clearColor;

	GLint		x;			// back buffer left
	GLint		y;			// back buffer top
	GLint		width;		// back buffer width
	GLint		height;		// back buffer height

	// should not be derived
	WDIBContext(){}

	// for windows itself
	GLboolean deleteDIBBuffer();
	GLboolean createDIBBuffer(GLint w, GLint h);

	// prepare OS display context
	// initially set to attach to the whole window area
	GLboolean prepare(HDC hdc, GLcolor color);
};


typedef WDIBContext OScontext;


#endif