/************************************************************************/
/* File		: oscontext.cpp
/* Context about OS layer
/* Date		: 2006/3/10
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/

#include "oscontext.h"
#include <tchar.h>


IDispFuncTbl * WDDBContext::createContext(HDC hdc, GLcolor color)
{
	WDDBContext * lpctx = new WDDBContext;
	if (lpctx == NULL) {
		return NULL;
	}
	lpctx->prepare(hdc, color);
	return lpctx;
}

GLboolean WDDBContext::deleteDDBBuffer()
{
	SelectObject(hbdc, hOldBmp);
	DeleteDC(hbdc);
	DeleteObject(hbbm);

	width = 0;
	height = 0;

	return GL_TRUE;
}

GLboolean WDDBContext::createDDBBuffer(GLint w, GLint h)
{
	HBITMAP hbmp;
	HDC		hbdc;
	HDC     hic;

	hic = CreateIC(_T("display"), NULL, NULL, NULL);
	if (hic == 0) {
		return GL_FALSE;
	}

	hbdc = CreateCompatibleDC(hic);
	if (hbdc == 0) {
		return GL_FALSE;
	}
	this->hbdc = hbdc;

	hbmp = CreateCompatibleBitmap(hic, w, h);
	if (hbmp == 0) {
		return GL_FALSE;
	}
	this->hbbm = hbmp;

	this->hOldBmp = (HBITMAP)SelectObject(hbdc, hbmp);
	DeleteDC(hic);

	this->width = w;
	this->height = h;
	
	return GL_TRUE;
}



GLboolean WDDBContext::prepare(HDC hdc, 
						 GLcolor color)
{
	HWND hwnd;

	this->hdc = hdc;
	
	hwnd = WindowFromDC(hdc);
	if (hwnd == 0) {
		return GL_FALSE;
	}

	width = GetDeviceCaps(hdc, HORZRES);
	height = GetDeviceCaps(hdc, VERTRES);

	this->x = 0;
	this->y = 0;

	// to ensure that client can actually draw without calling glViewport
	if(!createDDBBuffer(width, height)){
		return GL_FALSE;
	}

	if(!setClearColor(color)) 
		return GL_FALSE;
	
	clearBuffer();
	flip();
	
	return GL_TRUE;
}

WDDBContext::~WDDBContext()
{}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// DIB

IDispFuncTbl * WDIBContext::createContext(HDC hdc, GLcolor color)
{
	WDIBContext * lpctx = new WDIBContext;
	if (lpctx == NULL) {
		return NULL;
	}
	lpctx->prepare(hdc, color);
	return lpctx;
}

GLboolean WDIBContext::deleteDIBBuffer()
{
	if (hbmDIB) {
		SelectObject(hbdc, hOldBitmap);
		DeleteDC(hbdc);
		DeleteObject(hbmDIB);
	}
	this->width = 0;
	this->height = 0;
	return GL_TRUE;
}

GLboolean WDIBContext::createDIBBuffer(GLint w, GLint h)
{
	LPBITMAPINFO pbmi = &bmi;
	HDC          hic;
	WORD		 cColorBits;	

	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = w;
	pbmi->bmiHeader.biHeight= h;
	pbmi->bmiHeader.biPlanes = 1;
	pbmi->bmiHeader.biBitCount = GetDeviceCaps(hdc, BITSPIXEL);
	pbmi->bmiHeader.biCompression = BI_RGB;
	pbmi->bmiHeader.biSizeImage = 0;
	pbmi->bmiHeader.biXPelsPerMeter = 0;
	pbmi->bmiHeader.biYPelsPerMeter = 0;
	pbmi->bmiHeader.biClrUsed = 0;
	pbmi->bmiHeader.biClrImportant = 0;

	cColorBits = pbmi->bmiHeader.biBitCount;
	ScanWidth = (w * (cColorBits / 8) + 3) & ~3;

	hic = CreateIC("display", NULL, NULL, NULL);
	hbdc = CreateCompatibleDC(hic);

	hbmDIB = CreateDIBSection(hic, &bmi, DIB_RGB_COLORS,
							  (void **)&(pPixelBuffer), 0, 0);

	hOldBitmap = (HBITMAP)SelectObject(hbdc, hbmDIB);

	DeleteDC(hic);

	cColorBits = GetDeviceCaps(hdc, BITSPIXEL);

	assert(cColorBits == 32);

	this->width = w;
	this->height = h;

	return GL_TRUE;
}



GLboolean WDIBContext::prepare(HDC hdc, 
							   GLcolor color)
{
	HWND hwnd;

	this->hdc = hdc;

	hwnd = WindowFromDC(hdc);
	if (hwnd == 0) {
		return GL_FALSE;
	}

	width = GetDeviceCaps(hdc, HORZRES);
	height = GetDeviceCaps(hdc, VERTRES);

	this->x = 0;
	this->y = 0;

	// to ensure that client can actually draw without calling glViewport
	if(!createDIBBuffer(width, height)){
		return GL_FALSE;
	}

	if(!setClearColor(color)) 
		return GL_FALSE;

	clearBuffer();
	flip();

	return GL_TRUE;
}

WDIBContext::~WDIBContext()
{}
