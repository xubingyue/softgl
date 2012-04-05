/************************************************************************/
/* File		: gldemo.cpp
/* Demo functions
/* Date		: 2006/5/9
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/

#include "stdafx.h"
#include "gldemo.h"
#include "tgaload.h"




GLuint tex[100];

int demoId;

int g_w, g_h;


namespace Demo2
{
	void resize()
	{
		glViewport(0, 0, g_w, g_h);	

		glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
		glLoadIdentity();									// Reset The Projection Matrix

		glOrtho(-2, 2, -2, 2, 1, -10);

		glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
		glLoadIdentity();	
	}

	void init()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);

		glClearColor(0, 0, 0, 0);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_CULL_FACE);
		loadTex("..\\crate.tga");
		resize();
	}

	void draw()
	{
		static float xRot = 0, yRot = 0, zRot = 0; 
		glLoadIdentity();
		glClear(GL_COLOR_BUFFER_BIT);
		glRotatef(xRot, 1, 0, 0);
		glRotatef(yRot, 0, 1, 0);
		glRotatef(zRot, 0, 0, 1);
		//glScalef(0.5, 0.5, 0.5);

		xRot += 1.0f;
		yRot += 0.5f;
		zRot += 1.5f;

		glBindTexture(GL_TEXTURE_2D, tex[demoId]);

		//glDisable(GL_TEXTURE_2D);

		glBegin ( GL_QUADS );
		// Front Face
		glColor3f(1.0f, 0, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glColor3f(1.0, 1.0f, 0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glColor3f(0, 1.0, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glColor3f(0, 0, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		// Back Face
		glColor3f(1.0f, 0, 0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glColor3f(1.0, 1.0f, 0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glColor3f(0, 1.0, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glColor3f(0, 0, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		// Top Face
		glColor3f(1.0f, 0, 0);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glColor3f(1.0, 1.0f, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glColor3f(0, 1.0, 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glColor3f(0, 0, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		// Bottom Face
		glColor3f(1.0f, 0, 0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glColor3f(1.0, 1.0f, 0);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glColor3f(0, 1.0, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glColor3f(0, 0, 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		// Right face
		glColor3f(1.0f, 0, 0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glColor3f(1.0, 1.0f, 0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glColor3f(0, 1.0, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glColor3f(0, 0, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		// Left Face
		glColor3f(1.0f, 0, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glColor3f(1.0, 1.0f, 0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glColor3f(0, 1.0, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glColor3f(0, 0, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glEnd();
		glFlush();
	}
}


namespace Demo3
{

	void resize()
	{
		glViewport(0, 0, g_w, g_h);	

		glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
		glLoadIdentity();									// Reset The Projection Matrix

		glOrtho(-2, 2, -2, 2, 1, -10);
		//gluPerspective(80, 1.0f * g_w / g_h, 1, 200);
		glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
		glLoadIdentity();	
	}

	void init()
	{
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_CULL_FACE);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//gluPerspective(80, 1.0f * g_w / g_h, 1, 200);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);

		glClearColor(0, 0, 0, 0);
		glShadeModel(GL_SMOOTH);
		resize();
	}

	void draw()
	{
		static float xRot = 0, yRot = 0, zRot = 0; 
		glLoadIdentity();
		glClear(GL_COLOR_BUFFER_BIT);

		glTranslatef(0, -0.2f, -3);

		glRotatef(xRot, 1, 0, 0);
		glRotatef(yRot, 0, 1, 0);
		glRotatef(zRot, 0, 0, 1);

		//glScalef(0.5, 0.5, 0.5);
		xRot += 1.0f;
		yRot += 0.5f;
		zRot += 1.5f;

		glBegin(GL_QUADS);									// Draw A Quad
		glColor3f(1.0f,0.0f,0.0f);			// Set The Color To Red
		glVertex3f( 1.0f, 1.0f,-1.0f);			// Top Right Of The Quad (Top)

		glColor3f(0.0f,1.0f,0.0f);			// Set The Color To Blue
		glVertex3f(-1.0f, 1.0f,-1.0f);			// Top Left Of The Quad (Top)

		glColor3f(0.0f,0.0f,1.0f);			// Set The Color To Blue
		glVertex3f(-1.0f, 1.0f, 1.0f);			// Bottom Left Of The Quad (Top)

		glColor3f(0.0f,1.0f,1.0f);			// Set The Color To Blue
		glVertex3f( 1.0f, 1.0f, 1.0f);			// Bottom Right Of The Quad (Top)

		glColor3f(1.0f,0.0f,0.0f);			// Set The Color To Red
		glVertex3f( 1.0f,-1.0f, 1.0f);			// Top Right Of The Quad (Bottom)

		glColor3f(0.0f,1.0f,0.0f);			// Set The Color To Blue
		glVertex3f(-1.0f,-1.0f, 1.0f);			// Top Left Of The Quad (Bottom)

		glColor3f(0.0f,0.0f,1.0f);			// Set The Color To Blue
		glVertex3f(-1.0f,-1.0f,-1.0f);			// Bottom Left Of The Quad (Bottom)

		glColor3f(0.0f,1.0f,1.0f);			// Set The Color To Blue
		glVertex3f( 1.0f,-1.0f,-1.0f);			// Bottom Right Of The Quad (Bottom)

		glColor3f(1.0f,0.0f,0.0f);			// Set The Color To Red
		glVertex3f( 1.0f, 1.0f, 1.0f);			// Top Right Of The Quad (Front)

		glColor3f(0.0f,1.0f,0.0f);			// Set The Color To Blue
		glVertex3f(-1.0f, 1.0f, 1.0f);			// Top Left Of The Quad (Front)

		glColor3f(0.0f,0.0f,1.0f);			// Set The Color To Blue
		glVertex3f(-1.0f,-1.0f, 1.0f);			// Bottom Left Of The Quad (Front)

		glColor3f(0.0f,1.0f,1.0f);			// Set The Color To Blue
		glVertex3f( 1.0f,-1.0f, 1.0f);			// Bottom Right Of The Quad (Front)

		glColor3f(1.0f,0.0f,0.0f);			// Set The Color To Red
		glVertex3f( 1.0f,-1.0f,-1.0f);			// Bottom Left Of The Quad (Back)

		glColor3f(1.0f,0.0f,0.0f);			// Set The Color To Red
		glVertex3f(-1.0f,-1.0f,-1.0f);			// Bottom Right Of The Quad (Back)

		glColor3f(0.0f,1.0f,1.0f);			// Set The Color To Blue
		glVertex3f(-1.0f, 1.0f,-1.0f);			// Top Right Of The Quad (Back)

		glColor3f(0.0f,1.0f,1.0f);			// Set The Color To Blue
		glVertex3f( 1.0f, 1.0f,-1.0f);			// Top Left Of The Quad (Back)


		glColor3f(1.0f,0.0f,0.0f);			// Set The Color To Red
		glVertex3f(-1.0f, 1.0f, 1.0f);			// Top Right Of The Quad (Left)

		glColor3f(1.0f,0.0f,0.0f);			// Set The Color To Red
		glVertex3f(-1.0f, 1.0f,-1.0f);			// Top Left Of The Quad (Left)

		glColor3f(0.0f,1.0f,1.0f);			// Set The Color To Blue
		glVertex3f(-1.0f,-1.0f,-1.0f);			// Bottom Left Of The Quad (Left)

		glColor3f(0.0f,1.0f,1.0f);			// Set The Color To Blue
		glVertex3f(-1.0f,-1.0f, 1.0f);			// Bottom Right Of The Quad (Left)

		glColor3f(1.0f,0.0f,1.0f);			// Set The Color To Violet
		glVertex3f( 1.0f, 1.0f,-1.0f);			// Top Right Of The Quad (Right)

		glColor3f(1.0f,0.0f,0.0f);			// Set The Color To Red
		glVertex3f( 1.0f, 1.0f, 1.0f);			// Top Left Of The Quad (Right)

		glColor3f(0.0f,1.0f,1.0f);			// Set The Color To Blue
		glVertex3f( 1.0f,-1.0f, 1.0f);			// Bottom Left Of The Quad (Right)

		glColor3f(1.0f,0.0f,1.0f);			// Set The Color To Violet
		glVertex3f( 1.0f,-1.0f,-1.0f);			// Bottom Right Of The Quad (Right)
		glEnd();
		glFlush();
	}
}



namespace Demo4
{
	void init()
	{
		GLfloat mat_mid[] = { 0.5f, 0.5f, 0.5f, 1.0f };
		GLfloat mat_low[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		GLfloat mat_non[] = { 0.0f, 0.0f, 0.0f, 1.0f };

		GLfloat mat_shininess[] = { 50.0f };
		GLfloat light_position[] = { 0.0f, 0.0f, 3.0f, 0.0f };

		glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
		glShadeModel (GL_SMOOTH);

		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_low);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_low);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_mid);

		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

		glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
		glLoadIdentity();
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);

		glEnable(GL_LIGHT0);
		glEnable(GL_CULL_FACE);
		loadTex("..\\crate.tga");
		resize();
	}

	void resize()
	{
		glViewport(0, 0, g_w, g_h);	

		glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
		glLoadIdentity();									// Reset The Projection Matrix

		glOrtho(-2, 2, -2, 2, 1, -10);

		glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
		glLoadIdentity();
	}

	void draw()
	{
		glEnable(GL_LIGHTING);
		//glEnable(GL_TEXTURE_2D);
		static float xRot = 10, yRot = 20, zRot = 40; 
		glLoadIdentity();
		glClear(GL_COLOR_BUFFER_BIT);

		glTranslatef(0, -0.2f, 0);

		glRotatef(xRot, 1, 0, 0);
		glRotatef(yRot, 0, 1, 0);
		glRotatef(zRot, 0, 0, 1);


		glBindTexture(GL_TEXTURE_2D, tex[demoId]);

		xRot += 1;
		yRot += 0.5;
		zRot += 1.5;

		glBegin(GL_QUADS);	
		// Draw A Quad
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f,  1.0f, -1.0f);
		glVertex3f(-1.0f,  1.0f,  1.0f);
		glVertex3f( 1.0f,  1.0f,  1.0f);
		glVertex3f( 1.0f,  1.0f, -1.0f);

		glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f( 1.0f, -1.0f, -1.0f);
		glVertex3f( 1.0f, -1.0f,  1.0f);
		glVertex3f(-1.0f, -1.0f,  1.0f);

		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f,  1.0f);
		glVertex3f( 1.0f, -1.0f,  1.0f);
		glVertex3f( 1.0f,  1.0f,  1.0f);
		glVertex3f(-1.0f,  1.0f,  1.0f);

		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f,  1.0f, -1.0f);
		glVertex3f( 1.0f,  1.0f, -1.0f);
		glVertex3f( 1.0f, -1.0f, -1.0f);

		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f,  1.0f);
		glVertex3f(-1.0f,  1.0f,  1.0f);
		glVertex3f(-1.0f,  1.0f, -1.0f);

		glNormal3f(1.0f, 0.0f, 0.0f);
		glVertex3f( 1.0f, -1.0f, -1.0f);
		glVertex3f( 1.0f,  1.0f, -1.0f);
		glVertex3f( 1.0f,  1.0f,  1.0f);
		glVertex3f( 1.0f, -1.0f,  1.0f);		
		
		glEnd();
		glFlush();
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
	}
}




void loadTex(char * pstrName)
{
	image_t   temp_image;
	tgaLoad  ( pstrName, &temp_image, TGA_DEFAULT);
	glGenTextures(1, &tex[demoId]);

	glBindTexture(GL_TEXTURE_2D, tex[demoId]);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, temp_image.info.width, temp_image.info.height, 
		0, GL_RGB, GL_UNSIGNED_BYTE, temp_image.data);

	GLfloat color[4];
	color[0] = 1.0f, color[1] = 0.0f, color[2] = 1.0f, color[3] = 0.5f;
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, color);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void GLChangeDemo()
{
	if (demoId == 3) {
		demoId = 1;
	}
	else
		demoId ++;

	//demoId = 3;

	Demo3::init();
	switch(demoId)
	{
	case 1:
		Demo2::init();
		break;
	case 2:
		Demo3::init();
		break;
	case 3:
		Demo4::init();
		break;
	}
}

void GLChangeFilter()
{
	static GLenum filter = GL_LINEAR;
	filter = (filter == GL_NEAREST ? GL_LINEAR : GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLfloat)filter);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLfloat)filter);
}

void GLChangeEnvMode()
{
	static int mode = 0;
	mode = (mode + 1) % 4;
	switch(mode)
	{
	case 0:
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		break;
	case 1:
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		break;
	case 2:
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		break;
	case 3:
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
		break;
	}
}



int GLCreate(HDC hdc)
{
	softglCreateContext(hdc);
	demoId = 1;
	GLChangeDemo();
	return 0;
}





int GLPaint()
{
	switch(demoId)
	{
	case 1:
		Demo2::draw();
		break;
	case 2:
		Demo3::draw();
		break;
	case 3:
		Demo4::draw();
		break;
	}
	return 0;
}


int GLResize(GLint w, GLint h)
{
	g_w = w, g_h = h;

	switch(demoId)
	{
	case 1:
		Demo2::resize();
		break;
	case 2:
		Demo3::resize();
		break;
	case 3:
		Demo4::resize();
		break;
	}
	return 0;
}



