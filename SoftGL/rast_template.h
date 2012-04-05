/************************************************************************/
/* File		: rast_template.h
/* Raster Function template
/* Date		: 2006/5/6
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/

#ifdef RAST_TRI_FUNC

#define CLAMP_COORD(sp, xlimit, ylimit) { \
					if((sp).x >= xlimit) goto ENDTAG; \
					if((sp).y >= ylimit || (sp).y < 0) goto ENDTAG;\
					GLint rx = (sp).x + (sp).len;	\
					if(rx >= xlimit) \
							(sp).len -= (rx - xlimit);	\
					}

	void FUNCNAME(Vertex * v0, Vertex * v1, Vertex * v2)
	{
		struct Edge{
			const Vertex *v0, *v1;   // Y(v0) < Y(v1) 
			GLdouble dx;	// X(v1) - X(v0) 
			GLdouble dy;	// Y(v1) - Y(v0) 
			GLdouble dxdy;	// dx/dy */
			GLdouble fsx;	// first sample point x coord  
			GLdouble fsy;
			GLint lines;	// number of lines to be sampled on this edge  

			// color
			GLfloat fsr, fsg, fsb;
#ifdef RAST_SMOOTH
			GLfloat dr;		// v1->red - v0->red
			GLfloat dg;		
			GLfloat db;
			GLdouble drdy;
			GLdouble dgdy;
			GLdouble dbdy;
#endif

#ifdef RAST_ALPHA
			GLfloat  fsa;
	#ifdef RAST_SMOOTH
			GLfloat  da;
			GLdouble dady;
	#endif
#endif

#ifdef RAST_TEX
			GLfloat fss, fst;
			GLfloat ds, dt;
			GLdouble dsdy, dtdy;
#endif
			GLboolean setVertex(const Vertex * v0, const Vertex * v1)
			{
				this->v0 = v0, this->v1 = v1;
				dx = v1->coord[IDXX] - v0->coord[IDXX];
				dy = v1->coord[IDXY] - v0->coord[IDXY];
				dxdy = dx / dy;
				fsy = ((GLfloat)std::ceil(v0->coord[IDXY]));   // fsy >= Y(v0)
				fsx = v0->coord[IDXX];


				lines =  (GLint)((GLfloat)std::ceil(v1->coord[IDXY] - fsy));
				if (lines <= 0)
					return GL_FALSE;

#ifdef RAST_TEX
				fss = v0->texCoord[T_S];
				fst = v0->texCoord[T_T];

				ds = v1->texCoord[T_S] - v0->texCoord[T_S];
				dt = v1->texCoord[T_T] - v0->texCoord[T_T];

				dsdy = ds / dy;
				dtdy = dt / dy;
#endif
				return GL_TRUE;
			}

			GLboolean initColor(GLenum face)
			{
				if (face == GL_BACK) {
					fsr = v0->colorBack[IDXR];
					fsg = v0->colorBack[IDXG];
					fsb = v0->colorBack[IDXB];
#ifdef RAST_SMOOTH
					dr = v1->colorBack[IDXR] - v0->colorBack[IDXR];
					dg = v1->colorBack[IDXG] - v0->colorBack[IDXG];
					db = v1->colorBack[IDXB] - v0->colorBack[IDXB];

					drdy = dr / dy;
					dgdy = dg / dy;
					dbdy = db / dy;
#endif

#ifdef RAST_ALPHA
					fsa = v0->colorBack[IDXA];
#ifdef RAST_SMOOTH
					dady = da / dy;
					da = v1->colorBack[IDXA] - v0->colorBack[IDXA];
#endif
#endif
					return GL_TRUE;
				}
				// color section
				fsr = v0->color[IDXR];
				fsg = v0->color[IDXG];
				fsb = v0->color[IDXB];
#ifdef RAST_SMOOTH
				dr = v1->color[IDXR] - v0->color[IDXR];
				dg = v1->color[IDXG] - v0->color[IDXG];
				db = v1->color[IDXB] - v0->color[IDXB];

				drdy = dr / dy;
				dgdy = dg / dy;
				dbdy = db / dy;
#endif

#ifdef RAST_ALPHA
				fsa = v0->color[IDXA];
#ifdef RAST_SMOOTH
				dady = da / dy;
				da = v1->color[IDXA] - v0->color[IDXA];
#endif
#endif
				return GL_TRUE;
			}
		};

		Edge maj, top, bot;
		GLdouble y0 = v0->coord[IDXY];
		GLdouble y1 = v1->coord[IDXY];
		GLdouble y2 = v2->coord[IDXY];

		Vertex * min = NULL;
		Vertex * mid = NULL;
		Vertex * max = NULL;

		const GLint xLimit = CTXPTR->getWidth();
		const GLint yLimit = CTXPTR->getHeight();

		GLenum culledFace = CTXPTR->culledFace;
		GLboolean enableCull = CTXPTR->currentState[GL_CULL_FACE];

		if (culledFace == GL_FRONT_AND_BACK) {
			return;
		}

		// by default, anti-clockwise is the front face
		GLint culled = CTXPTR->frontFace == GL_CCW ? 1 : -1;

		if (y0 <= y1) {
			if (y1 <= y2) {
				min = v0, mid = v1, max = v2;
			}
			else if (y2 <= y0) {
				min = v2, mid = v0, max = v1;
			}
			else {
				min = v0, mid = v2, max = v1;
				culled = -culled;
			}
		}
		else {
			if (y0 <= y2) {
				min = v1, mid = v0, max = v2;
				culled = - culled;
			}
			else if (y2 <= y1) {
				min = v2, mid = v1, max = v0;
				culled = -culled;
			}
			else {
				min = v1, mid = v2, max = v0;
			}
		}
		if(maj.setVertex(min, max) == GL_FALSE) 
			return;
		top.setVertex(mid, max);
		bot.setVertex(min, mid);

		Span sp;

		GLdouble crossProduct = maj.dx * bot.dy - bot.dx * maj.dy;
		bool l2r = (crossProduct < 0);
		culled = l2r ? culled : -culled;

		GLenum faceFlag = culled < 0 ? GL_BACK : GL_FRONT;
		maj.initColor(faceFlag);
		bot.initColor(faceFlag);
		top.initColor(faceFlag);

		if (culledFace == GL_FRONT) {
			culled = -culled;
		}

#ifdef RAST_TEX
		TexObject * texobj = CTXPTR->getCurrentTexObj();
		GLfloat texWidth = (GLfloat)texobj->width;
		GLfloat texHeight = (GLfloat)texobj->height;
		GLdouble texCrossProduct = (maj.ds * texWidth) * (bot.dt * texHeight)
								  - (bot.ds * texWidth) * (maj.dt * texHeight);
		sp.filter = GL_NEAREST;
		if (texCrossProduct < crossProduct) {
			// magnify
            sp.filter = texobj->magFilter;			
		}
		else
			sp.filter = texobj->minFilter;

		GLfloat * texEnvColor = CTXPTR->texEnvColor;
		GLC2RGBCOPY(sp.texEnvColor, texEnvColor);
#endif

		if (culled < 0 && enableCull == GL_TRUE) {
			return;
		}

		GLdouble fxl = 0, fxr = 0;
		GLdouble fdxl = 0, fdxr = 0;

		// RGBA Interp
		GLdouble frl = 0;
		GLdouble fgl = 0; 
		GLdouble fbl = 0;

#ifdef RAST_SMOOTH
		GLdouble frr = 0;
		GLdouble fgr = 0;
		GLdouble fbr = 0;

		GLdouble fdrl = 0, fdrr = 0;   // delta of red over y
		GLdouble fdgl = 0, fdgr = 0;
		GLdouble fdbl = 0, fdbr = 0;
#endif

#ifdef RAST_ALPHA
		GLdouble fal = 0; 
		GLdouble _far = 0;
	#ifdef RAST_SMOOTH
		GLdouble fdal = 0, fdar = 0;
	#endif
#endif

#ifdef RAST_TEX
		GLdouble fsl = 0, fsr = 0;
		GLdouble ftl = 0, ftr = 0;

		GLdouble fdsl = 0, fdsr = 0;
		GLdouble fdtl = 0, fdtr = 0;
#endif

		Edge * pLeft = NULL;
		Edge * pRight = NULL;

		for (GLint i = 0; i <= 1; i++) {
			// i == 0 => bottom; i == 1 => top
			GLint lines = 0;
			if (i == 0) { // bottom
				if (l2r) {
					pLeft = &maj;
					pRight = &bot;
				}
				else	// bottom, right to left
				{
					pLeft = &bot;
					pRight = &maj;
				}
				lines = bot.lines;
				fxl = pLeft->fsx;
				fxr = pRight->fsx;
				fdxl = pLeft->dxdy;
				fdxr = pRight->dxdy;
				sp.y = (GLint)pLeft->fsy;

			// RGBA Interp
#ifdef RAST_SMOOTH
				frl = pLeft->fsr, frr = pRight->fsr;
				fgl = pLeft->fsg, fgr = pRight->fsg;
				fbl = pLeft->fsb, fbr = pRight->fsb;
				fdrl = pLeft->drdy, fdrr = pRight->drdy;
				fdgl = pLeft->dgdy, fdgr = pRight->dgdy;
				fdbl = pLeft->dbdy, fdbr = pRight->dbdy;

#else	// flat mode
				frl = v2->color[IDXR];
				fgl = v2->color[IDXG];
				fbl = v2->color[IDXB];
#endif

#ifdef RAST_ALPHA
	#ifdef RAST_SMOOTH
				fal = pLeft->fsa, _far = pRight->fsa;
				fdal = pLeft->dady, fdar = pRight->dady;
	#else
				fal = v2->color[IDXA];
	#endif
#endif	
					// others remains zero
#ifdef RAST_TEX
				fsl = pLeft->fss, ftl = pLeft->fst;
				fsr = pRight->fss, ftr = pRight->fst;
				fdsl = pLeft->dsdy, fdtl = pLeft->dtdy;
				fdsr = pRight->dsdy, fdtr = pRight->dtdy;
#endif


			} // end of if(i == 0)
			else	// i == 1 => top
			{
				GLdouble adjx = 0.0;
				if (l2r) {   // top l to r
					pLeft = &maj;
					pRight = &top;
					adjx = fxr - pRight->fsx;
					fxr = pRight->fsx;
#ifdef RAST_SMOOTH
					frr = (pRight->fsr);
					fgr = pRight->fsg;
					fbr = pRight->fsb;
#endif

#ifdef RAST_ALPHA
					_far = pRight->fsa;
#endif

#ifdef RAST_TEX
					fsr = pRight->fss;// - adjx * pRight->dsdx;
					ftr = pRight->fst;// - adjx * pRight->dtdx;
#endif
				}
				else{
					pLeft = &top;
					pRight = &maj;
					fxl = pLeft->fsx;
					// although here we change to another edge  
					// we just remain the color unchanged
#ifdef RAST_SMOOTH
					frl = pLeft->fsr; 
					fgl = pLeft->fsg;
					fbl = pLeft->fsb;
#endif

#ifdef RAST_ALPHA
					fal = pLeft->fsa;
#endif

#ifdef RAST_TEX
					fsl = pLeft->fss;
					ftl = pLeft->fst;
#endif
						sp.y = (GLint)pLeft->fsy;
				}
				lines = top.lines;
				fdxl = pLeft->dxdy;
				fdxr = pRight->dxdy;

#ifdef RAST_SMOOTH
				fdrl = pLeft->drdy, fdrr = pRight->drdy;
				fdgl = pLeft->dgdy, fdgr = pRight->dgdy;
				fdbl = pLeft->dbdy, fdbr = pRight->dbdy;
#else
				frl = v2->color[IDXR];
				fgl = v2->color[IDXG];
				fbl = v2->color[IDXB];
					// others remains zero
#endif		

#ifdef RAST_ALPHA
	#ifdef RAST_SMOOTH
				fdal = pLeft->dady, fdar = pRight->dady;
	#else
				fal = v2->color[IDXA];
	#endif
#endif

#ifdef RAST_TEX
				fdsl = pLeft->dsdy, fdsr = pRight->dsdy;
				fdtl = pLeft->dtdy, fdtr = pRight->dtdy;
#endif

			}  // end of if(i == 0) else	

			while (lines > 0) {
				GLint r = (GLint)fxr;
				sp.x = (GLint)fxl;
				sp.len = r - sp.x;
				GLdouble len = fxr - fxl;

				CLAMP_COORD(sp, xLimit, yLimit);

				// RGBA Interp
				sp.color[IDXR] = (GLfloat)frl;
				sp.color[IDXG] = (GLfloat)fgl;
				sp.color[IDXB] = (GLfloat)fbl;

#ifdef RAST_ALPHA
				sp.color[IDXA] = (GLfloat)fal;
#endif

#ifdef RAST_SMOOTH
				sp.colorDelta[IDXR] = (GLfloat)((frr - frl) / len);
				sp.colorDelta[IDXG] = (GLfloat)((fgr - fgl) / len);
				sp.colorDelta[IDXB] = (GLfloat)((fbr - fbl) / len);
#endif

#ifdef RAST_TEX
				sp.texCoord[T_S] = (GLfloat)fsl;
				sp.texCoord[T_T] = (GLfloat)ftl;

				sp.texCoordDelta[T_S] = (GLfloat)((fsr - fsl) / len);
				sp.texCoordDelta[T_T] = (GLfloat)((ftr - ftl) / len);
#endif


#ifdef RAST_ALPHA
	#ifdef RAST_SMOOTH
				sp.colorDelta[IDXA] = (GLfloat)((_far - fal) / len);
	#endif
#endif

#ifdef RAST_TEX
	#ifdef RAST_SMOOTH
				drawSmoothTexSpan(&sp);
	#else
				drawFlatTexSpan(&sp);
	#endif
#else
	#ifdef RAST_SMOOTH
		#ifdef RAST_ALPHA
				drawSpanSmoothRGBA(&sp);
		#else
				drawSpanSmoothRGB(&sp);
		#endif
	#else  // Flat
		#ifdef RAST_ALPHA
				drawSpanFlatRGBA(&sp);
		#else
				drawSpanFlatRGB(&sp);
		#endif
	#endif
#endif

ENDTAG:
#ifdef RAST_SMOOTH
				frl += fdrl, frr += fdrr;
				fgl += fdgl, fgr += fdgr;
				fbl += fdbl, fbr += fdbr;
#endif

#ifdef RAST_ALPHA
	#ifdef RAST_SMOOTH
				fal += fdal, _far += fdar;
	#endif
#endif

#ifdef RAST_TEX
				fsl += fdsl, fsr += fdsr;
				ftl += fdtl, ftr += fdtr;
#endif
				sp.y++;
				lines --;
				fxl += fdxl;
				fxr += fdxr;
			}
		}  // end of for
	}

#undef RAST_ALPHA
#undef RAST_SMOOTH
#undef RAST_TEX
#undef FUNCNAME

#endif


// Span

#ifdef RAST_SPAN_FUNC


void FUNCNAME(Span * sp)
{
		const GLint colorMax = CMAXI << FIXEDSHIFT;

		GLcontext * ctx = CTXPTR;
		IDispFuncTbl * osctx = ctx->pOsCtx;
		SpanExtraData * pExt = sp->pExtra;
		GLuint len = sp->len;
		if (len > MAXSPANLENGTH) {
			return;
		}

		GLint	color[4];
#ifdef SPAN_ALPHA
	#ifdef SPAN_SMOOTH
		GLint	colorDelta[4];
	#endif
#else
	#ifdef SPAN_SMOOTH
		GLint	colorDelta[3];
	#endif
#endif

#ifdef SPAN_TEX
		TexObject * texobj = CTXPTR->getCurrentTexObj();
		const GLenum wrapS = texobj->wrapS;
		const GLenum wrapT = texobj->wrapT;
		const GLfloat ftexWidth = (GLfloat)texobj->width;
		const GLfloat ftexHeight = (GLfloat)texobj->height;
		const GLenum texEnvMode = CTXPTR->texEnvMode;
		const GLint itexWidthi = texobj->width;
		const GLint itexHeight = texobj->height;
		GLint texCoord[4];
		GLint texDelta[4];

		GLubyte * pTex = (GLubyte *)texobj->data;
#endif

#ifdef SPAN_SMOOTH
		color[IDXR] = COLORF2B(sp->color[IDXR]) << FIXEDSHIFT;
		color[IDXG] = COLORF2B(sp->color[IDXG]) << FIXEDSHIFT;
		color[IDXB] = COLORF2B(sp->color[IDXB]) << FIXEDSHIFT;

		colorDelta[IDXR] = (GLint)((sp->colorDelta[IDXR] * FIXED_ONE) * CMAXF);
		colorDelta[IDXG] = (GLint)((sp->colorDelta[IDXG] * FIXED_ONE) * CMAXF);
		colorDelta[IDXB] = (GLint)((sp->colorDelta[IDXB] * FIXED_ONE) * CMAXF);
#else // span flat
		color[IDXR] = COLORF2B(sp->color[IDXR]);
		color[IDXG] = COLORF2B(sp->color[IDXG]);
		color[IDXB] = COLORF2B(sp->color[IDXB]);
#endif

#ifdef SPAN_ALPHA
	#ifdef SPAN_SMOOTH   // alpha , smooth
		color[IDXA] = COLORF2B(sp->color[IDXA]) << FIXEDSHIFT;
		colorDelta[IDXA] = (GLint)((sp->colorDelta[IDXA] * FIXED_ONE) * CMAXF);
	#else   // alpha flat
		color[IDXA] = COLORF2B(sp->color[IDXA]);
	#endif
#else
		//  no alpha
		color[IDXA] = 
			COLORF2B(attribDefVal[VTX_COLOR][IDXA]);
#endif

#ifdef  SPAN_TEX
		texCoord[T_S] = (GLint)((sp->texCoord[T_S]) * FIXED_ONE * ftexWidth);
		texCoord[T_T] = (GLint)((sp->texCoord[T_T]) * FIXED_ONE * ftexHeight);

		texDelta[T_S] = (GLint)((sp->texCoordDelta[T_S]) * FIXED_ONE * ftexWidth);
		texDelta[T_T] = (GLint)((sp->texCoordDelta[T_T]) * FIXED_ONE * ftexHeight);

		GLenum filter = sp->filter;
#endif

		GLubyte (* pColor)[4] = pExt->color;
		
		GLint invalid = 0;
		if (sp->x < 0) {
			if ((sp->x + sp->len) <= 0) {
				return;
			}
			invalid = -sp->x;
			sp->x = 0;
			sp->len -= invalid;
#ifdef SPAN_SMOOTH
			color[IDXR] += invalid * colorDelta[IDXR];			
			color[IDXG] += invalid * colorDelta[IDXG];			
			color[IDXB] += invalid * colorDelta[IDXB];
	#ifdef SPAN_ALPHA
			color[IDXA] += invalid * colorDelta[IDXA];			
	#endif
#endif

#ifdef SPAN_TEX
			texCoord[T_S] += invalid * texDelta[T_S];
			texCoord[T_T] += invalid * texDelta[T_T];
#endif
		}

		
		
		for(GLuint i = 0; i < len; i ++, invalid--)
		{
			GLubyte * pBuf = pColor[i];
#ifdef SPAN_SMOOTH
			pBuf[IDXR] = color[IDXR] >> FIXEDSHIFT;
			pBuf[IDXG] = color[IDXG] >> FIXEDSHIFT;
			pBuf[IDXB] = color[IDXB] >> FIXEDSHIFT;

			color[IDXR] += colorDelta[IDXR];			
			color[IDXG] += colorDelta[IDXG];			
			color[IDXB] += colorDelta[IDXB];

			CLAMP(color[IDXR], CMINI, colorMax);
			CLAMP(color[IDXG], CMINI, colorMax);
			CLAMP(color[IDXB], CMINI, colorMax);
	#ifdef SPAN_ALPHA
			pBuf[IDXA] = color[IDXA] >> FIXEDSHIFT;
			color[IDXA] += colorDelta[IDXA];			
			CLAMP(color[IDXA], CMINI, colorMax);
	#endif
#else   // flat
			pBuf[IDXR] = color[IDXR];
			pBuf[IDXG] = color[IDXG];
			pBuf[IDXB] = color[IDXB];
			pBuf[IDXA] = color[IDXA];			
#endif

#ifdef SPAN_TEX
			GLubyte texSample[4];
			GLuint tmpSample[4];
			tmpSample[IDXR] = 0, tmpSample[IDXG] = 0;
			tmpSample[IDXB] = 0, tmpSample[IDXA] = 0;
			GLint offset = 0;

#define TEX_CLAMP(i, len)   {if(i >= len) i = len - 1; if(i < 0) i = 0;}
#define TEX_REPEAT(i, len)  { while(i >= len) i -= len; while(i < 0) i += len;}


#define TEX_COORD_ADJ(COORDS, COORDT) { if (wrapS == GL_CLAMP)   \
											TEX_CLAMP(COORDS, itexWidthi)  \
										else							\
											TEX_REPEAT(COORDS, itexWidthi)  \
										if (wrapT == GL_CLAMP)          \
											TEX_CLAMP(COORDT, itexHeight)    \
										else							\
											TEX_REPEAT(COORDT, itexHeight)	}



#define GETPIXEL 	TEX_COORD_ADJ(ts, tt);  \
					offset = (((tt) << (texobj->log2Width)) + (ts)) << 2;   \
					tmpSample[idx] += (pTex + offset)[idx];

			GLint s = texCoord[T_S] >> FIXEDSHIFT;
			GLint t = texCoord[T_T] >> FIXEDSHIFT;
			GLint tt = 0, ts = 0;

			switch(filter)
			{
			case GL_LINEAR:
				for(GLint idx = 0; idx < 4; idx++)  
				{ 
					ts = s - 1, tt = t;
					GETPIXEL
					ts = s + 1, tt = t;
					GETPIXEL
					ts = s, tt = t - 1;
					GETPIXEL
					ts = s, tt = t + 1;
					GETPIXEL

					texSample[idx] = (GLubyte)(tmpSample[idx] / 4);
				}  
				break;
			case GL_NEAREST:
				TEX_COORD_ADJ(s,t);   
				offset = ((t << (texobj->log2Width)) + s) << 2; 
				COPY4V(texSample, pTex + offset);  
				break;
			default:
				assert(false);
				break;
			}

			GLubyte * pEnvColor = sp->texEnvColor;

			switch(texEnvMode)
			{
			case GL_REPLACE:
				COPY3V(pBuf, texSample);
				break;
			case GL_MODULATE:
				pBuf[IDXR] = pBuf[IDXR] * (texSample[IDXR] + 1) >> 8; 
				pBuf[IDXG] = pBuf[IDXG] * (texSample[IDXG] + 1) >> 8; 
				pBuf[IDXB] = pBuf[IDXB] * (texSample[IDXB] + 1) >> 8; 
				pBuf[IDXA] = pBuf[IDXA] * (texSample[IDXA] + 1) >> 8; 
				break;
			case GL_DECAL:
				pBuf[IDXR] = ((CMAXI - texSample[IDXA]) * pBuf[IDXR] +  
					((texSample[IDXA] + 1) * texSample[IDXR])) >> 8;    
				pBuf[IDXG] = ((CMAXI - texSample[IDXA]) * pBuf[IDXG] +   
					((texSample[IDXA] + 1) * texSample[IDXG])) >> 8;       
				pBuf[IDXB] = ((CMAXI - texSample[IDXA]) * pBuf[IDXB] +   
					((texSample[IDXA] + 1) * texSample[IDXB])) >> 8;  
				break;
			case GL_BLEND:
				pBuf[IDXR] = ((CMAXI - texSample[IDXR]) * pBuf[IDXR]		
					+ (texSample[IDXR] + 1) * pEnvColor[IDXR]) >> 8;	
				pBuf[IDXG] = ((CMAXI - texSample[IDXG]) * pBuf[IDXG]		
					+ (texSample[IDXG] + 1) * pEnvColor[IDXG]) >> 8;	
				pBuf[IDXB] = ((CMAXI - texSample[IDXB]) * pBuf[IDXB]		
					+ (texSample[IDXB] + 1) * pEnvColor[IDXB]) >> 8;	
				pBuf[IDXA] = pBuf[IDXA] * (texSample[IDXA] + 1) >> 8;
				break;
			default:
				assert(false);
				break;
			}
			texCoord[T_S] += texDelta[T_S];
			texCoord[T_T] += texDelta[T_T];

#undef TEX_COORD_ADJ
#undef GETPIXEL
#undef TEX_CLAMP
#undef TEX_REPEAT

#endif
		}
		osctx->writeSpan(sp->len, sp->x, sp->y, pExt->color);
}

#undef FUNCNAME
#undef SPAN_SMOOTH
#undef SPAN_ALPHA
#undef SPAN_TEX

#endif