// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 ULIB_FOREGROUNDPROCESS_BETA_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// ULIB_FOREGROUNDPROCESS_BETA_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef ULIB_FOREGROUNDPROCESS_BETA_EXPORTS
#define ULIB_FOREGROUNDPROCESS_BETA_API __declspec(dllexport)
#else
#define ULIB_FOREGROUNDPROCESS_BETA_API __declspec(dllimport)
#endif

ULIB_FOREGROUNDPROCESS_BETA_API HRESULT CJU_ForegroundSmoothFilter_Init(IN BYTE sfIndex, IN int videoWidth, IN int videoHeight, IN int *pDepthInfo, OUT int *pTargetTexturePixels, OUT int *pTargetDepthInfo);
ULIB_FOREGROUNDPROCESS_BETA_API HRESULT CJU_ForegroundSmoothFilter_UnInit(IN BYTE sfIndex);
ULIB_FOREGROUNDPROCESS_BETA_API HRESULT CJU_ForegroundSmoothFilter_AddUser(IN BYTE sfIndex, IN int usrId);
ULIB_FOREGROUNDPROCESS_BETA_API HRESULT CJU_ForegroundSmoothFilter_ExitUser(IN BYTE sfIndex, IN int usrId);
ULIB_FOREGROUNDPROCESS_BETA_API HRESULT CJU_ForegroundSmoothFilter_SmoothPerframe(IN BYTE sfIndex);
ULIB_FOREGROUNDPROCESS_BETA_API void CJU_ForegroundSmoothFilter_EnableAlgorithm(IN BYTE sfIndex, IN bool bEnable);