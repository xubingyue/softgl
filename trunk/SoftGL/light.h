/************************************************************************/
/* File		: light.h
/* Light calculation
/* Date		: 2006/5/11
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/

#ifndef LIGHT_H
#define LIGHT_H

struct LightObj
{
	GLfloat		lightPos[4];
	GLfloat		ambient[4];
	GLfloat		diffuse[4];
	GLfloat		specular[4];
	GLfloat     spotDir[3];
	GLfloat		spotExponent;
	GLfloat		spotCutoff;
	GLfloat		constAttenuation;
	GLfloat		linearAttenuation;
	GLfloat		quadAttenuation;
	GLboolean   directional;

	void		initLight(GLint num);
};

namespace Light
{
	GLboolean calLight(GLenum face);
}

#endif