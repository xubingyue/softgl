/************************************************************************/
/* File		: light.cpp
/* Light calculation
/* Date		: 2006/5/11
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/

#include "globdef.h"
#include "light.h"
#include "context.h"
#include <math.h>


void LightObj::initLight(GLint num)
{
	F4VASSIGN(ambient, 0.0f, 0.0f, 0.0f, 1.0f);
	if (num == 0) {
		F4VASSIGN(diffuse, 1.0f, 1.0f, 1.0f, 1.0f);
		F4VASSIGN(specular, 1.0f, 1.0f, 1.0f, 1.0f);
	}
	else
	{
		F4VASSIGN(diffuse, 0.0f, 0.0f, 0.0f, 1.0f);
		F4VASSIGN(specular, 0.0f, 0.0f, 0.0f, 1.0f);
	}

	F4VASSIGN(lightPos, 0.0f, 0.0f, 1.0f, 0.0f);
	F3VASSIGN(spotDir, 0.0f, 0.0f, -1.0f);

	spotExponent = 0.0f;
	spotCutoff = 180.0f;
	constAttenuation = 1.0f;
	linearAttenuation = 0.0f;
	quadAttenuation = 0.0f;
	directional = GL_TRUE;
}

namespace Light
{
	GLboolean calLight(GLenum face)
	{
		VertexBuffer * pVtxBuf = CTXPTR->getVertexBufferPtr();
		if (pVtxBuf == NULL) {
			return GL_FALSE;
		}

		Vertex * pPPBuf = CTXPTR->getPPBuffer();
		if (pPPBuf == NULL) {
			return GL_FALSE;
		}

		GLint f = (face == GL_BACK ? 1 : 0);

		BufferDesc ambientDesc;
		BufferDesc diffuseDesc;
		BufferDesc emissionDesc;
		BufferDesc specDesc;
		BufferDesc normalDesc;
		BufferDesc shinDesc;

		pVtxBuf->getBufferDesc(&ambientDesc, VTX_F_AMBIENT + f);
		pVtxBuf->getBufferDesc(&diffuseDesc, VTX_F_DIFFUSE + f);
		pVtxBuf->getBufferDesc(&emissionDesc, VTX_F_EMISSION + f);
		pVtxBuf->getBufferDesc(&specDesc, VTX_F_SPECULAR + f);
		pVtxBuf->getBufferDesc(&normalDesc, VTX_NORMAL);
		pVtxBuf->getBufferDesc(&shinDesc, VTX_F_SHININESS);

		GLint total = ambientDesc.total;

		const GLfloat * pEmission = emissionDesc.pBuf;
		const GLfloat * pSpecular = specDesc.pBuf;
		const GLfloat * pShine = shinDesc.pBuf;
		const GLfloat * pDiffuse = diffuseDesc.pBuf;
		const GLfloat * pAmbient = ambientDesc.pBuf;

		GLint sEmission = emissionDesc.stride;
		GLint sSpecular = specDesc.stride;
		GLint sShine = shinDesc.stride;
		GLint sDiffuse = diffuseDesc.stride;
		GLint sAmbient = ambientDesc.stride;

		GLfloat * gLightAmbient = CTXPTR->lightModelAmbient;
		GLboolean defAmbient = ambientDesc.elemSize == 0 ? GL_TRUE : GL_FALSE;
		GLfloat gAmbient[4];
		if (defAmbient == GL_TRUE) {
			F4VMUL(gAmbient, gLightAmbient, pAmbient);
		}

		for (GLint i = 0; i < total; i++) {
			// global ambient color
			GLfloat vFinal[4];

			if (defAmbient == GL_FALSE) {
				F4VMUL(gAmbient, gLightAmbient, pAmbient);
			}

			for(GLint lightId = 0; lightId < GL_MAX_LIGHTS; lightId++)
			{
				if (CTXPTR->currentState[lightId+GL_LIGHTBASE] == GL_FALSE) {
					continue;
				}
				LightObj * light = &(CTXPTR->lightObj[lightId]);
				const GLfloat * spotPos = light->lightPos;
				GLboolean directional = light->directional;
				const GLfloat kc = light->constAttenuation;
				const GLfloat kl = light->linearAttenuation;
				const GLfloat kq = light->quadAttenuation;

				const GLboolean needSpot = EQUAL0(light->spotCutoff - 180.0f) ? 
															GL_FALSE : GL_TRUE;
				const GLfloat * spotDir = light->spotDir;
				const GLfloat spotExponent = light->spotExponent;
				
				GLfloat attenuation = 1.0;
				GLfloat spotEffect = 1.0f;

				const GLfloat * lightAmbient = light->ambient;
				const GLfloat * lightDiffuse = light->diffuse;
				const GLfloat * lightSpecular = light->specular;

				// spot light
				if (directional == GL_FALSE) {
					GLfloat p = 0.0f;
					GLfloat d2 = 0.0f, d = 0.0f;
					if (!EQUAL0(kq)) {
						d2 = DISTANCE2(spotPos, pPPBuf->coord);
						p += d2 * kq;
					}
					if (!EQUAL0(kl)) {
						if (EQUAL0(d2)) {
							d2 = DISTANCE2(spotPos, pPPBuf->coord);
						}
						d = sqrt(d2);
						p += kl * d;
					}
					attenuation = 1.0f / (p + kc);
				}


				GLfloat v[3];
				F3VMIN(v, pPPBuf->coord, spotPos);
				GLfloat vLength = LENGTH3V(v);
				v[0] /= vLength, v[1] /= vLength, v[2] /= vLength;

				if (needSpot) {
					GLfloat dotProd = DOTPROD3V(spotDir, v);
					if (v < 0) {
						spotEffect = 0.0f;
					}
					else{
						spotEffect = pow(dotProd, spotExponent);
					}
				}

				// ambient
				GLfloat vAmbient[4];
				F4VMUL(vAmbient, pAmbient, lightAmbient);

				// diffuse
				GLfloat vDiffuse[4];
				// v --> light
				v[0] = -v[0], v[1] = -v[1], v[2] = -v[2];

				const GLfloat fDiffuse = DOTPROD3V(v, pPPBuf->normal);
				if (fDiffuse < 0.0f) {
					F4VASSIGN(vDiffuse, 0.0f, 0.0f, 0.0f, 0.0f);
				}
				else{
					F4VMUL(vDiffuse, pDiffuse, lightDiffuse);
					F4MULI(vDiffuse, fDiffuse);
				}

				// specular
				GLfloat vSpecular[4];
				if (EQUAL0(fDiffuse)) {
					F4VASSIGN(vSpecular, 0.0f, 0.0f, 0.0f, 0.0f);
				}
				else
				{
					v[2] += 1.0f;
					vLength = LENGTH3V(v);
					v[0] /= vLength, v[1] /= vLength, v[2] /= vLength;
					GLfloat dotProd = DOTPROD3V(pPPBuf->normal, v);
					if (dotProd < 0.0f) {
						F4VASSIGN(vSpecular, 0.0f, 0.0f, 0.0f, 0.0f);
					}
					else
					{
						GLfloat fSpecular = pow(dotProd, *pShine);
						F4VMUL(vSpecular, pSpecular, lightSpecular);
						F4MULI(vSpecular, fSpecular);
					}
				} // end of specular
                
                F4VADD(vFinal, vAmbient, vDiffuse);
				F4VADD(vFinal, vFinal, vSpecular);

				F4MULI(vFinal, spotEffect);
				F4MULI(vFinal, attenuation);
			}

            F4VADD(vFinal, vFinal, pEmission);
			F4VADD(vFinal, vFinal, gAmbient);

			if (face == GL_FRONT) {
				COPY4V(pPPBuf->color, vFinal);
			}
			if (face == GL_BACK) {
				COPY4V(pPPBuf->colorBack, vFinal);
			}

			pAmbient += sAmbient;
			pEmission += sEmission;
			pSpecular += sSpecular;
			pShine += sShine;
			pDiffuse += sDiffuse;
			
			pPPBuf++;
		}
		return GL_TRUE;
	}
}