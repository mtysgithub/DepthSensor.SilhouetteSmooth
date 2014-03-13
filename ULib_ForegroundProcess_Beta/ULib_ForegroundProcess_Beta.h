// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� ULIB_FOREGROUNDPROCESS_BETA_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// ULIB_FOREGROUNDPROCESS_BETA_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
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