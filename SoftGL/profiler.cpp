/************************************************************************/
/* File		: profiler.cpp
/* performance profiler
/* Date		: 2006/4/7
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/

#include "profiler.h"
#include <fstream>
#include <string>

ProfilerManager::MODDATASET ProfilerManager::setModData;


ProfilerData * ProfilerManager::initProfilerData(const char * strModName)
{
	ProfilerPointer ptr;
	ptr.p = new ProfilerData(strModName);
	pair<MODDATASET::iterator, bool> res;
	res = setModData.insert(ptr);
	if(res.second)
		return ptr.p;
	else
	{
		assert(false);
		return NULL;
	}
}

void ProfilerManager::clearAll()
{
	MODDATASET::iterator it = setModData.begin();
	for (; it != setModData.end(); it++) {
		it->p->clearData();
	}
}


void ProfilerManager::output(const char * strFileName)
{
	ofstream out(strFileName);
	MODDATASET::iterator it = setModData.begin();

	for (; it != setModData.end(); it++) {
		out << "Module Name : " << it->p->modName << endl;
		out << "Hit Count : " << it->p->hitCount << endl;
		out << "Total Time Elapsed : " << it->p->timeElps << "ms" << endl;
		out << "First Call Time Elapsed : " << it->p->firstCall << "ms" << endl;
		out << "Last Call Time Elapsed : " << it->p->lastCall << "ms" << endl;
		out << endl;
	}
	out.close();
}

ProfilerTrigger::ProfilerTrigger(const char * strFileName)
{
	this->strFileName = strFileName;
	ProfilerManager::clearAll();
}

ProfilerTrigger::~ProfilerTrigger()
{
	ProfilerManager::output(this->strFileName.c_str());
}