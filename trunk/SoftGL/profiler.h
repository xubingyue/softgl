/************************************************************************/
/* File		: profiler.h
/* performance profiler
/* Date		: 2006/4/7
/* Author	: Xiaoyu Ma
/* Copyright (c) 2006 Xiaoyu Ma, All Rights Reserved.
*************************************************************************/

#ifndef PROFILER_H
#define PROFILER_H

#include <cassert>
#include <string>
#include <set>
#include "windows.h"

using namespace std;

#define FUNC_HOOK(c)  


#define PROFILER(name)	static ProfilerData * _pData = \
								ProfilerManager::initProfilerData(name); \
								Profiler _oPro(_pData);

#define OPENPROFILER(name) ProfilerTrigger _oTrigger(name); \
							PROFILER("Total");


struct ProfilerData
{
	// should have an identical name
	ProfilerData(const char * strModName)
	{
		modName = strModName;
		timeElps = 0;
		hitCount = 0;
		firstCall = 0;
		lastCall = 0;
	}

	void clearData()
	{
		timeElps = 0;
		hitCount = 0;
		firstCall = 0;
		lastCall = 0;
	}

	string			modName;
	double			timeElps;
	double			firstCall;
	double			lastCall;
	int				hitCount;
};


class Profiler
{
public:
	Profiler(ProfilerData * pData)
	{
		this->pData = pData;
		QueryPerformanceCounter(&start);		
	}

	~Profiler()
	{
		LARGE_INTEGER freq;
		LARGE_INTEGER end;
		QueryPerformanceCounter(&end);
		QueryPerformanceFrequency(&freq);

		double time = (end.QuadPart - start.QuadPart) * 1000 /
										(double)freq.QuadPart;
		pData->hitCount++;
		pData->timeElps += time;
		if (pData->firstCall == 0) {
			// first time be called
			pData->firstCall = time;
		}
		pData->lastCall = time;
	}
private:
	ProfilerData *	pData;
	LARGE_INTEGER	start;
};


class ProfilerManager
{
public:
	static ProfilerData * initProfilerData(const char * strModName);	
	static void clearAll();
	static void output(const char * strFileName);
private:
	// for pointer less operator override
	struct ProfilerPointer
	{
		ProfilerData * p;

		bool operator < (ProfilerPointer rhs) const
		{
			return p->modName < rhs.p->modName;
		}
	};
public:
	typedef set<ProfilerPointer> MODDATASET;
private:
	static MODDATASET setModData;
};


class ProfilerTrigger
{
public:
	ProfilerTrigger(const char * strFileName);
	~ProfilerTrigger();
private:
	string strFileName;
};


#endif