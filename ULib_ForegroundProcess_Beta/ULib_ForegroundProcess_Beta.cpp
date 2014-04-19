#include "stdafx.h"
#include "ULib_ForegroundProcess_Beta.h"
#include "CJForegroundSmoothFilter.hpp"

HRESULT CJU_ForegroundSmoothFilter_Init(IN BYTE sfIndex, IN int videoWidth, IN int videoHeight, IN int *pDepthInfo, 
										IN int timeDnrExpFnThresholdValue = 11,
										IN int timeDnrFilterWIndowSize = 6,
										IN bool enableMorphologySmooth = true,
										IN int spaceDnrFrontPixelCountThresholdValue = 6,
										IN int spaceDnrBackPixelCountThresholdValue = 15,
										IN int spaceDnrFilterWIndowSize = 5,
										IN int openClacStepCount = 1,
										IN int closeClacStepCount = 1,
										IN UINT bgClr = 0x00000000, 
										IN UINT frontClr = 0xff0000ff,
										IN bool enableMedianFilteringAlgorithm = true,
										OUT int *pTargetTexturePixels = NULL, OUT int *pTargetDepthInfo = NULL)
{
	return (CJForegroundSmoothFilter::CreateInstance(sfIndex))->Init(videoWidth, videoHeight, pDepthInfo, 
																		timeDnrExpFnThresholdValue,
																		timeDnrFilterWIndowSize,
																		enableMorphologySmooth,
																		spaceDnrFrontPixelCountThresholdValue,
																		spaceDnrBackPixelCountThresholdValue,
																		spaceDnrFilterWIndowSize,
																		openClacStepCount,
																		closeClacStepCount,
																		bgClr,
																		frontClr,
																		enableMedianFilteringAlgorithm,
																		pTargetTexturePixels, pTargetDepthInfo);
}

HRESULT CJU_ForegroundSmoothFilter_UnInit(IN BYTE sfIndex)
{
	return CJForegroundSmoothFilter::DestoryInstance(sfIndex);
}
HRESULT CJU_ForegroundSmoothFilter_AddUser(IN BYTE sfIndex, IN int usrId)
{
	return (CJForegroundSmoothFilter::Instance(sfIndex))->AddUser(usrId);
}

HRESULT CJU_ForegroundSmoothFilter_ExitUser(IN BYTE sfIndex, IN int usrId)
{
	return (CJForegroundSmoothFilter::Instance(sfIndex))->ExitUser(usrId);
}

HRESULT CJU_ForegroundSmoothFilter_SmoothPerframe(IN BYTE sfIndex)
{
	return (CJForegroundSmoothFilter::Instance(sfIndex))->SmoothPerframe();
}

void CJU_ForegroundSmoothFilter_EnableAlgorithm(IN BYTE sfIndex, bool bEnable)
{
	(CJForegroundSmoothFilter::Instance(sfIndex))->EnableAlgorithm(bEnable);
}