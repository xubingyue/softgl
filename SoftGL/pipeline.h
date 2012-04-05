/************************************************************************/
/* File		: pipeline.h
/* Pipeline functions
/* Date		: 2006/5/1
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/

#ifndef PIPELINE_H
#define PIPELINE_H

namespace Pipeline
{
	GLboolean runNormalPipeline();
	GLboolean runVertexPipeline();
	GLboolean runLightPipeline();
	GLboolean runTexPipeline();
	GLboolean runRastPipeline();
}


#endif
