#ifdef ULIB_FOREGROUNDPROCESS_BETA_EXPORTS
#define ULIB_FOREGROUNDPROCESS_BETA_API __declspec(dllexport)
#else
#define ULIB_FOREGROUNDPROCESS_BETA_API __declspec(dllimport)
#endif

//ULIB_FOREGROUNDPROCESS_BETA_API HRESULT CJU_ForegroundSmoothFilter_Init(IN BYTE sfIndex, IN int videoWidth, IN int videoHeight, IN int *pDepthInfo, OUT int *pTargetTexturePixels, OUT int *pTargetDepthInfo);
//ULIB_FOREGROUNDPROCESS_BETA_API HRESULT CJU_ForegroundSmoothFilter_UnInit(IN BYTE sfIndex);
//ULIB_FOREGROUNDPROCESS_BETA_API HRESULT CJU_ForegroundSmoothFilter_AddUser(IN BYTE sfIndex, IN int usrId);
//ULIB_FOREGROUNDPROCESS_BETA_API HRESULT CJU_ForegroundSmoothFilter_ExitUser(IN BYTE sfIndex, IN int usrId);
//ULIB_FOREGROUNDPROCESS_BETA_API HRESULT CJU_ForegroundSmoothFilter_SmoothPerframe(IN BYTE sfIndex);
//ULIB_FOREGROUNDPROCESS_BETA_API void CJU_ForegroundSmoothFilter_EnableAlgorithm(IN BYTE sfIndex, IN bool bEnable);

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /*__cplusplus*/
	ULIB_FOREGROUNDPROCESS_BETA_API HRESULT CJU_ForegroundSmoothFilter_Init(IN BYTE sfIndex, IN int videoWidth, IN int videoHeight, IN int *pDepthInfo, 
		IN int timeDnrExpFnThresholdValue,
		IN int timeDnrFilterWIndowSize,
		IN bool enableMorphologySmooth,
		IN int spaceDnrFrontPixelCountThresholdValue,
		IN int spaceDnrBackPixelCountThresholdValue,
		IN int spaceDnrFilterWIndowSize,
		IN int openClacStepCount,
		IN int closeClacStepCount,
		IN UINT bgClr, 
		IN UINT frontClr,
		IN bool enableMedianFilteringAlgorithm,
		OUT int *pTargetTexturePixels, OUT int *pTargetDepthInfo);

	ULIB_FOREGROUNDPROCESS_BETA_API HRESULT CJU_ForegroundSmoothFilter_UnInit(IN BYTE sfIndex);
	ULIB_FOREGROUNDPROCESS_BETA_API HRESULT CJU_ForegroundSmoothFilter_AddUser(IN BYTE sfIndex, IN int usrId);
	ULIB_FOREGROUNDPROCESS_BETA_API HRESULT CJU_ForegroundSmoothFilter_ExitUser(IN BYTE sfIndex, IN int usrId);
	ULIB_FOREGROUNDPROCESS_BETA_API HRESULT CJU_ForegroundSmoothFilter_SmoothPerframe(IN BYTE sfIndex);
	ULIB_FOREGROUNDPROCESS_BETA_API void CJU_ForegroundSmoothFilter_EnableAlgorithm(IN BYTE sfIndex, bool bEnable);
#ifdef __cplusplus
#if __cplusplus
};
#endif
#endif /*__cplusplus*/