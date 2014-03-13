#include "stdafx.h"
#include "CJForegroundSmoothFilter.hpp"

#define MAX_SF_SIZE 1 << 8

CJForegroundSmoothFilter *CJForegroundSmoothFilter::CreateInstance(BYTE sfIndex)
{
	if(!ppCJForegroundSmoothFilter)
	{
		ppCJForegroundSmoothFilter = new (CJForegroundSmoothFilter* [MAX_SF_SIZE]);
		//memset(ppCJForegroundSmoothFilter, NULL, MAX_SF_SIZE * sizeof(ppCJForegroundSmoothFilter[0]));
		for(int i = 0; i < MAX_SF_SIZE; ++i) ppCJForegroundSmoothFilter[i] = NULL;
	}
	if(ppCJForegroundSmoothFilter[sfIndex])
	{
		delete ppCJForegroundSmoothFilter[sfIndex];
	}

	return ppCJForegroundSmoothFilter[sfIndex] = new CJForegroundSmoothFilter();
}

CJForegroundSmoothFilter *CJForegroundSmoothFilter::Instance(BYTE sfIndex)
{
	if(ppCJForegroundSmoothFilter && ppCJForegroundSmoothFilter[sfIndex])
	{
		return ppCJForegroundSmoothFilter[sfIndex];
	}else
	{
		return NULL;
	}
}

HRESULT CJForegroundSmoothFilter::DestoryInstance(IN BYTE sfIndex)
{
	if (ppCJForegroundSmoothFilter[sfIndex])
	{
		ppCJForegroundSmoothFilter[sfIndex]->UnInit();
		delete ppCJForegroundSmoothFilter[sfIndex];
		ppCJForegroundSmoothFilter[sfIndex] = NULL;
		return S_OK;
	}else
	{
		return E_FAIL;
	}
}

CJForegroundSmoothFilter **ppCJForegroundSmoothFilter = NULL;