#pragma once
#include "stdafx.h"
#include <ppl.h>
using namespace Concurrency;

#define USING_PPL 0
#define USING_OPEN_MP 1
#define OPT_BY_ACTIVEUSER 0

#define Img_OpenClac __clacIntegrationImage(); \
	__corrosion(true); \
	__clacIntegrationImage(); \
	__corrosion(false);

#define Img_CloseClac __clacIntegrationImage(); \
	__corrosion(false); \
	__clacIntegrationImage(); \
	__corrosion(true);

class CJForegroundSmoothFilter
{
	//DataStruct
protected:
	//滤波窗口四个直角所在游标
	struct FilterWIndowCorner 
	{
		void clac(int center, int winHalfLength, int imgWidth, int imgHeight)
		{
			this->left_up = _makeValidLeftUp(center, winHalfLength, imgWidth);
			this->left_down = _makeValidLeftDown(center, winHalfLength, imgWidth, imgHeight);
			this->right_up = _makeValidRightUp(center, winHalfLength, imgWidth);
			this->right_down = _makeValidRightDown(center, winHalfLength, imgWidth, imgHeight);
		}

		int left_up;
		int left_down;
		int right_up;
		int right_down;

		inline int _makeValidLeftUp(int poskernel, int winHalfLength, int imgWidth)
		{
			int posLeftUp = poskernel;
			if(winHalfLength <= posLeftUp % imgWidth) 
				posLeftUp -= winHalfLength;
			else 
				posLeftUp -= poskernel % imgWidth;

			if(0 <= posLeftUp - imgWidth * winHalfLength)
				posLeftUp -= imgWidth * winHalfLength;
			else
				posLeftUp -= (posLeftUp / imgWidth) * imgWidth;

			return posLeftUp;
		}

		inline int _makeValidRightDown(int poskernel, int winHalfLength, int imgWidth, int imgHeight)
		{
			int posRightDown = poskernel;
			if(imgWidth - winHalfLength > posRightDown % imgWidth)
				posRightDown += winHalfLength;
			else
				posRightDown += imgWidth - imgWidth % posRightDown;

			if(imgWidth * imgHeight > posRightDown + imgWidth * winHalfLength)
				posRightDown += imgWidth * winHalfLength;
			else
				posRightDown += imgWidth * (imgHeight - posRightDown / imgWidth - 1);

			return posRightDown;
		}

		inline int _makeValidLeftDown(int poskernel, int winHalfLength, int imgWidth, int imgHeight)
		{
			int posLeftDown = poskernel;
			if(winHalfLength <= posLeftDown % imgWidth) 
				posLeftDown -= winHalfLength;
			else 
				posLeftDown -= poskernel % imgWidth;

			if(imgWidth * imgHeight > posLeftDown + imgWidth * winHalfLength)
				posLeftDown += imgWidth * winHalfLength;
			else
				posLeftDown += imgWidth * (imgHeight - posLeftDown / imgWidth - 1);

			return posLeftDown;
		}

		inline int _makeValidRightUp(int poskernel, int winHalfLength, int imgWidth)
		{
			int posRightUp = poskernel;
			if(imgWidth - winHalfLength > posRightUp % imgWidth)
				posRightUp += winHalfLength;
			else
				posRightUp += imgWidth - imgWidth % posRightUp;

			if(0 <= posRightUp - imgWidth * winHalfLength)
				posRightUp -= imgWidth * winHalfLength;
			else
				posRightUp -= (posRightUp / imgWidth) * imgWidth;

			return posRightUp;
		}
	};

	struct IntegratDirectionLeftUpPosition
	{
		void clac(int toIntegratPosition, int imgWidth)
		{			
			this->m_toIntegratedLeft = _makeIntegrationDir_LeftTop(toIntegratPosition, imgWidth, 0, 1);
			this->m_toIntegratedUp = _makeIntegrationDir_LeftTop(toIntegratPosition, imgWidth, 1, 0);
			this->m_toIntegratedLeftUp = _makeIntegrationDir_LeftTop(toIntegratPosition, imgWidth, 1, 1);
		}

		int m_toIntegratedLeftUp;
		int m_toIntegratedLeft;
		int m_toIntegratedUp;

		inline int _makeIntegrationDir_LeftTop(int poskernel, int imgWidth, int mSIze = 1, int nSIze = 1)
		{
			if((nSIze > poskernel % imgWidth) || (0 > poskernel - imgWidth * mSIze))
				return -1;
			else
				return poskernel - imgWidth * mSIze - nSIze;
		}
	};
public: 
	static CJForegroundSmoothFilter *CreateInstance(IN BYTE sfIndex);
	static CJForegroundSmoothFilter *Instance(IN BYTE sfIndex);
	static HRESULT DestoryInstance(IN BYTE sfIndex);

	HRESULT Init(IN int videoWidth, IN int videoHeight, IN int *pDepthInfo, 
		IN int timeDnrExpFnThresholdValue,
		IN int timeDnrFilterWIndowSize,
		IN bool enableMorphologySmooth,
		IN int spaceDnrFrontPixelCountThresholdValue,
		IN int spaceDnrBackPixelCountThresholdValue,
		IN int spaceDnrFilterWIndowSize,
		IN int openClacStepCount,
		IN int closeClacStepCount,
		IN bool enableMedianFilteringAlgorithm,
		IN UINT bgClr,
		IN UINT frontClr,
		OUT int *pTargetTexturePixels, OUT int *pTargetDepthInfo)
	{
		m_bAlgorithm = true;
		m_backgroundColor = bgClr;
		m_frontColor = frontClr;

		m_videoWidth = videoWidth;
		m_videoHeight = videoHeight;
		m_pixelBuffLength = m_videoWidth * m_videoHeight;

		m_pDepthInfo = pDepthInfo;

		m_timeDnrExpFnThresholdValue = timeDnrExpFnThresholdValue;
		m_timeDnrFilterWIndowSize = timeDnrFilterWIndowSize;

		m_bIsEnableMorphologySmooth = enableMorphologySmooth;
		m_spaceDnrFrontPixelCountThresholdValue = spaceDnrFrontPixelCountThresholdValue;
		m_spaceDnrBackPixelCountThresholdValue = spaceDnrBackPixelCountThresholdValue;
		m_spaceDnrFilterWIndowSize = spaceDnrFilterWIndowSize;
		m_openClacStepCount = openClacStepCount;
		m_closeClacStepCount = closeClacStepCount;

		m_bIsEnableMedianFilteringAlgorithm = enableMedianFilteringAlgorithm;

		m_pTargetTexturePixels = pTargetTexturePixels;
		if(m_pTargetTexturePixels) memset(m_pTargetTexturePixels, 0xff808080, m_pixelBuffLength * sizeof(m_pTargetTexturePixels[0]));

		m_pTargetDepthInfo = pTargetDepthInfo;
		if(m_pTargetDepthInfo) memset(m_pTargetDepthInfo, 0x7fff0000, m_pixelBuffLength * sizeof(m_pTargetDepthInfo[0]));

		m_usrCount = 0;
		m_pHashUsrId = new bool [1 << 8];
		if(m_pHashUsrId) memset(m_pHashUsrId, false, (1 << 8 ) * sizeof(m_pHashUsrId[0]));

		m_pOriginalBitColors = new bool [m_pixelBuffLength];
		if(m_pOriginalBitColors) memset(m_pOriginalBitColors, false, m_pixelBuffLength * sizeof(m_pOriginalBitColors[0]));

		m_pSeqHistoryPinData = new unsigned int [m_pixelBuffLength];
		if(m_pSeqHistoryPinData) memset(m_pSeqHistoryPinData, (UINT)0, m_pixelBuffLength * sizeof(m_pSeqHistoryPinData[0]));

		m_pHds = new int [m_pixelBuffLength];
		if(m_pHds) memset(m_pHds, 0, m_pixelBuffLength * sizeof(m_pHds[0]));

		m_pHistoryPinData_ForegoundPinCount = new int [m_pixelBuffLength];
		if(m_pHistoryPinData_ForegoundPinCount) memset(m_pHistoryPinData_ForegoundPinCount, 0, m_pixelBuffLength * sizeof(m_pHistoryPinData_ForegoundPinCount[0]));

		m_pPfwpp = 0;
		m_pPosForWaitProcPixel = new int [m_pixelBuffLength];
		if(m_pPosForWaitProcPixel) memset(m_pPosForWaitProcPixel, 0, m_pixelBuffLength * sizeof(m_pPosForWaitProcPixel[0]));

		m_pToDnrBitColors = new bool[m_pixelBuffLength];
		if(m_pToDnrBitColors) memset(m_pToDnrBitColors, false, m_pixelBuffLength * sizeof(m_pToDnrBitColors[0]));

		m_pIntegrationImage = new int[m_pixelBuffLength];
		if(m_pIntegrationImage) memset(m_pIntegrationImage, 0, m_pixelBuffLength * sizeof(m_pIntegrationImage[0]));

		m_pIntegratDirectionLeftUpPosition = new IntegratDirectionLeftUpPosition[m_pixelBuffLength];
		if(m_pIntegratDirectionLeftUpPosition)
		{
			for (int i = 0; i < m_pixelBuffLength; ++i)
			{
				(m_pIntegratDirectionLeftUpPosition[i]).clac(i, m_videoWidth);
			}
		}

		m_pFilterWIndowCorner = new FilterWIndowCorner[m_pixelBuffLength];
		if (m_pFilterWIndowCorner)
		{
			for (int i = 0; i < m_pixelBuffLength; ++i)
			{
				(m_pFilterWIndowCorner[i]).clac(i, m_spaceDnrFilterWIndowSize / 2, m_videoWidth, m_videoHeight);
			}
		}
		return S_OK;
	}

	virtual HRESULT UnInit()
	{
		m_pDepthInfo = NULL;
		m_pTargetTexturePixels = NULL;
		m_pTargetDepthInfo = NULL;

		if(m_pHashUsrId) delete m_pHashUsrId;
		if(m_pOriginalBitColors) delete m_pOriginalBitColors;
		if(m_pSeqHistoryPinData) delete m_pSeqHistoryPinData;
		if(m_pHds) delete m_pHds;
		if(m_pHistoryPinData_ForegoundPinCount) delete m_pHistoryPinData_ForegoundPinCount;
		if(m_pPosForWaitProcPixel) delete m_pPosForWaitProcPixel;

		if(m_pToDnrBitColors) delete m_pToDnrBitColors;
		if(m_pIntegrationImage) delete m_pIntegrationImage;
		if(m_pFilterWIndowCorner) delete m_pFilterWIndowCorner;
		if(m_pIntegratDirectionLeftUpPosition) delete m_pIntegratDirectionLeftUpPosition;

		m_pPfwpp = m_usrCount = 0;

		return S_OK;
	}

	HRESULT AddUser(int usrId)
	{
		if (!m_pHashUsrId[usrId])
		{
			m_pHashUsrId[usrId] = 1;
			++m_usrCount;
			return S_OK;
		}else
		{
			return E_FAIL;
		}
	}

	HRESULT ExitUser(int usrId)
	{
		if (m_pHashUsrId[usrId])
		{
			m_pHashUsrId[usrId] = 0;
			--m_usrCount;
			return S_OK;
		}else
		{
			return E_FAIL;
		}
	}

	HRESULT SmoothPerframe()
	{
		if (m_bAlgorithm)
		{
			return __smoothPerframe();
		}else
		{
			return __drawRawDepthInfoColors();
		}
	}

	void EnableAlgorithm(bool bEnable)
	{
		this->m_bAlgorithm = bEnable;
	}

private:
	CJForegroundSmoothFilter()
	{

	}

	virtual ~CJForegroundSmoothFilter(void)
	{

	}

	HRESULT  __drawRawDepthInfoColors()
	{
#if OPT_BY_ACTIVEUSER
		if(!m_usrCount) return S_OK;
#endif
		for (int i = 0; i < m_pixelBuffLength; ++i)
		{
			unsigned int usrId = m_pDepthInfo[i] & 0x0000ffff;
			if (0 < usrId)
			{
				m_pTargetTexturePixels[i] = 0xff000080;
			}else
			{
				m_pTargetTexturePixels[i] = 0x00000000;
			}
		}
		return S_OK;
	}

	HRESULT __smoothPerframe()
	{
#if OPT_BY_ACTIVEUSER
		if(!m_usrCount) return S_OK;
#endif
		for (int i = 0; i < m_pixelBuffLength; ++i)
		{
			unsigned int usrId = m_pDepthInfo[i] & 0x0000ffff;
			if (0 < usrId)
			{
				m_pOriginalBitColors[i] = 1;
			}else
			{
				m_pOriginalBitColors[i] = 0;
			}
		}

		//时域抑噪
		__DNR_TimeDomain();

		//空域抑噪
		__DNR_SpaceDomain();

		__makeTargetDepthAndColorData();

		return S_OK;
	}

	HRESULT __DNR_TimeDomain()
	{
		m_pPfwpp = 0;
#if USING_PPL
		parallel_for<int>(0, m_pixelBuffLength, 1, [&](int i)
		{
			//前景帧计数
			m_pHistoryPinData_ForegoundPinCount[i] += (int)(m_pOriginalBitColors[i] - 
				((m_pSeqHistoryPinData[i] >> m_pHds[i]) & (UINT)1));
			//若不是稳定点则入队
			if ((m_pHistoryPinData_ForegoundPinCount[i]) && 
				(m_timeDnrFilterWIndowSize != m_pHistoryPinData_ForegoundPinCount[i]))
			{
				m_pPosForWaitProcPixel[m_pPfwpp++] = i;
			} 
			else
			{
				m_pToDnrBitColors[i] = m_pOriginalBitColors[i];
			}

			//帧数据入队
			m_pSeqHistoryPinData[i] = ((~((UINT)1 << m_pHds[i])) & (m_pSeqHistoryPinData[i])) 
				| (m_pOriginalBitColors[i] << m_pHds[i]);
			m_pHds[i] = (++m_pHds[i]) % m_timeDnrFilterWIndowSize;
		});
#endif

#if USING_OPEN_MP
#pragma loop(hint_parallel(4))
		for (int i = 0; i < m_pixelBuffLength; ++i)
		{
			//前景帧计数
			m_pHistoryPinData_ForegoundPinCount[i] += (int)(m_pOriginalBitColors[i] - 
				((m_pSeqHistoryPinData[i] >> m_pHds[i]) & (UINT)1));
			//若不是稳定点则入队
			if ((m_pHistoryPinData_ForegoundPinCount[i]) && 
				(m_timeDnrFilterWIndowSize != m_pHistoryPinData_ForegoundPinCount[i]))
			{
				m_pPosForWaitProcPixel[m_pPfwpp++] = i;
			} 
			else
			{
				m_pToDnrBitColors[i] = m_pOriginalBitColors[i];
			}

			//帧数据入队
			m_pSeqHistoryPinData[i] = ((~((UINT)1 << m_pHds[i])) & (m_pSeqHistoryPinData[i])) 
				| (m_pOriginalBitColors[i] << m_pHds[i]);
			m_pHds[i] = (++m_pHds[i]) % m_timeDnrFilterWIndowSize;
		}
#endif

#if USING_PPL
		//DNR
		parallel_for<int>(0, m_pPfwpp, 1, [&](int i)
		{
			int toProcPos = m_pPosForWaitProcPixel[i];

			int clrFlag = 0;
			int frontVal = 1, backVal = 1;
			for (int j = 0; j < m_timeDnrFilterWIndowSize; ++j)
			{
				int seqPointOffset = m_pHds[toProcPos] - 1 - j;
				if(seqPointOffset < 0) seqPointOffset += m_timeDnrFilterWIndowSize;

				UINT curVal = (m_pSeqHistoryPinData[toProcPos] >> seqPointOffset) & (UINT)1;
				switch (curVal)
				{
				case (UINT)1:
					frontVal <<= 1;
					break;
				case (UINT)0:
					backVal <<= 1;
					break;
				}
				if((frontVal < backVal) && (backVal - frontVal > m_timeDnrExpFnThresholdValue)){clrFlag = -1; break;}
				if((frontVal > backVal) && (frontVal - backVal > m_timeDnrExpFnThresholdValue)){clrFlag = 1; break;}
			}
			switch (clrFlag)
			{
			case 1:

#if DNR_ALL
				m_pToDnrBitColors[toProcPos] = 1;				
#else
				m_pTargetTexturePixels[toProcPos] = 0xff000080;
#endif // DNR_ALL
				break;
			case -1:

#if DNR_ALL
				m_pToDnrBitColors[toProcPos] = 0;
#else
				m_pTargetTexturePixels[toProcPos] = 0xff808080;
#endif
				break;
			case 0:
#if DNR_ALL
				//鬼影区域
				m_pToDnrBitColors[toProcPos] = m_pOriginalBitColors[toProcPos];
#else
				m_pTargetTexturePixels[toProcPos] = 0xff000000;
#endif
				break;
			default:
				break;
			}
		});
#endif

#if USING_OPEN_MP
#pragma loop(hint_parallel(4))
		for (int i = 0; i < m_pPfwpp; ++i)
		{
			int toProcPos = m_pPosForWaitProcPixel[i];

			int clrFlag = 0;
			int frontVal = 1, backVal = 1;
			for (int j = 0; j < m_timeDnrFilterWIndowSize; ++j)
			{
				int seqPointOffset = m_pHds[toProcPos] - 1 - j;
				if(seqPointOffset < 0) seqPointOffset += m_timeDnrFilterWIndowSize;

				UINT curVal = (m_pSeqHistoryPinData[toProcPos] >> seqPointOffset) & (UINT)1;
				switch (curVal)
				{
				case (UINT)1:
					frontVal <<= 1;
					break;
				case (UINT)0:
					backVal <<= 1;
					break;
				}
				if((frontVal < backVal) && (backVal - frontVal > m_timeDnrExpFnThresholdValue)){clrFlag = -1; break;}
				if((frontVal > backVal) && (frontVal - backVal > m_timeDnrExpFnThresholdValue)){clrFlag = 1; break;}
			}
			switch (clrFlag)
			{
			case 1:

#if DNR_ALL
				m_pToDnrBitColors[toProcPos] = 1;				
#else
				m_pTargetTexturePixels[toProcPos] = 0xff000080;
#endif // DNR_ALL
				break;
			case -1:

#if DNR_ALL
				m_pToDnrBitColors[toProcPos] = 0;
#else
				m_pTargetTexturePixels[toProcPos] = 0xff808080;
#endif
				break;
			case 0:
#if DNR_ALL
				//鬼影区域
				m_pToDnrBitColors[toProcPos] = m_pOriginalBitColors[toProcPos];
#else
				m_pTargetTexturePixels[toProcPos] = 0xff000000;
#endif
				break;
			default:
				break;
			}
		}
#endif
		return S_OK;
	}

	HRESULT __DNR_SpaceDomain()
	{
		if (m_bIsEnableMorphologySmooth)
		{
			__morphologySmooth();
		}
		return S_OK;
	}

	HRESULT __morphologySmooth()
	{
		//开形态运算		
		for (int i = 0; i < m_openClacStepCount; ++i)
		{
			Img_OpenClac
		}
		//形态闭合运算
		for (int i = 0; i < m_closeClacStepCount; ++i)
		{
			Img_CloseClac
		}
		return S_OK;
	}

	inline void __clacIntegrationImage()
	{
		for (int i = 0; i < m_pixelBuffLength; ++i)
		{
			int valLeftUp = 0;
			if(-1 != (m_pIntegratDirectionLeftUpPosition[i]).m_toIntegratedLeftUp) valLeftUp = m_pIntegrationImage[(m_pIntegratDirectionLeftUpPosition[i]).m_toIntegratedLeftUp];

			int valLeft = 0;
			if(-1 != (m_pIntegratDirectionLeftUpPosition[i]).m_toIntegratedLeft) valLeft = m_pIntegrationImage[(m_pIntegratDirectionLeftUpPosition[i]).m_toIntegratedLeft];

			int valUp = 0;
			if(-1 != (m_pIntegratDirectionLeftUpPosition[i]).m_toIntegratedUp) valUp = m_pIntegrationImage[(m_pIntegratDirectionLeftUpPosition[i]).m_toIntegratedUp];

			m_pIntegrationImage[i] = valUp + valLeft - valLeftUp + m_pToDnrBitColors[i];
		}
	}

	void __corrosion(bool bIsRevertOpera = false)
	{
#if USING_PPL
		parallel_for<int>(0, m_pixelBuffLength, 1, [&](int i)
		{
			int plu = (m_pFilterWIndowCorner[i]).left_up;
			int prd = (m_pFilterWIndowCorner[i]).right_down;
			int pld = (m_pFilterWIndowCorner[i]).left_down;
			int pru = (m_pFilterWIndowCorner[i]).right_up;

			int frontPixelSumVal = m_pIntegrationImage[prd] - m_pIntegrationImage[pru] - m_pIntegrationImage[pld] + m_pIntegrationImage[plu];
			if (!bIsRevertOpera)
			{
				if (frontPixelSumVal > m_spaceDnrFrontPixelCountThresholdValue)
				{
					m_pToDnrBitColors[i] = 1;
				}else
				{
					m_pToDnrBitColors[i] = 0;
				}
			}else
			{
				if (m_spaceDnrFilterWIndowSize * m_spaceDnrFilterWIndowSize - frontPixelSumVal > m_spaceDnrBackPixelCountThresholdValue)
				{
					m_pToDnrBitColors[i] = 0;
				}else
				{
					m_pToDnrBitColors[i] = 1;
				}
			}
		});
#endif

#if USING_OPEN_MP
#pragma loop(hint_parallel(4))
		for (int i = 0; i < m_pixelBuffLength; ++i)
		{
			int plu = (m_pFilterWIndowCorner[i]).left_up;
			int prd = (m_pFilterWIndowCorner[i]).right_down;
			int pld = (m_pFilterWIndowCorner[i]).left_down;
			int pru = (m_pFilterWIndowCorner[i]).right_up;

			int frontPixelSumVal = m_pIntegrationImage[prd] - m_pIntegrationImage[pru] - m_pIntegrationImage[pld] + m_pIntegrationImage[plu];
			if (!bIsRevertOpera)
			{
				if (frontPixelSumVal > m_spaceDnrFrontPixelCountThresholdValue)
				{
					m_pToDnrBitColors[i] = 1;
				}else
				{
					m_pToDnrBitColors[i] = 0;
				}
			}else
			{
				if (m_spaceDnrFilterWIndowSize * m_spaceDnrFilterWIndowSize - frontPixelSumVal > m_spaceDnrBackPixelCountThresholdValue)
				{
					m_pToDnrBitColors[i] = 0;
				}else
				{
					m_pToDnrBitColors[i] = 1;
				}
			}
		}
#endif
	}

	void __medianFiltering()
	{
		__clacIntegrationImage();

#pragma loop(hint_parallel(4))
		for (int i = 0; i < m_pixelBuffLength; ++i)
		{
			int plu = (m_pFilterWIndowCorner[i]).left_up;
			int prd = (m_pFilterWIndowCorner[i]).right_down;
			int pld = (m_pFilterWIndowCorner[i]).left_down;
			int pru = (m_pFilterWIndowCorner[i]).right_up;

			int winWidth = pru - plu;
			if(!winWidth) winWidth = 1;

			int winHeight = (pld / m_videoWidth) - (plu / m_videoWidth);
			if (!winHeight) winHeight = 1;

			int winPixelsNum = winWidth * winHeight;
			int frontPixelSumVal = m_pIntegrationImage[prd] - m_pIntegrationImage[pru] - m_pIntegrationImage[pld] + m_pIntegrationImage[plu];

			BYTE clrVal = (BYTE)((0x80 * frontPixelSumVal) / winPixelsNum);
			if (0 < clrVal)
			{
				//gray scale
				m_pTargetTexturePixels[i] = 0xff000000 | (clrVal) | (clrVal << 8) | (clrVal << 16);
			}else
			{
				m_pTargetTexturePixels[i] = m_backgroundColor;
			}
		}
	}

	void __makeTargetDepthAndColorData()
	{
		if (m_bIsEnableMedianFilteringAlgorithm)
		{
			__medianFiltering();
		}else
		{
			if (m_pTargetTexturePixels)
			{
				for (int i = 0; i < m_pixelBuffLength; ++i)
				{
					m_pTargetTexturePixels[i] = (m_pToDnrBitColors[i]) ? (m_frontColor) : (m_backgroundColor);
				}
			}
		}
	}

private:
	bool m_bAlgorithm;
	UINT m_backgroundColor;
	UINT m_frontColor;

	int m_videoWidth;
	int m_videoHeight;
	int m_pixelBuffLength;

	int *m_pDepthInfo; //深度图结构化数据
	int *m_pTargetTexturePixels; //生成的纹理色值内存
	int *m_pTargetDepthInfo; //生成的深度结构数据

	int m_usrCount;
	bool *m_pHashUsrId;

	bool *m_pOriginalBitColors; //原始二值图

	int m_timeDnrExpFnThresholdValue; //时域抑噪的指数加权函数阈值 
	int m_timeDnrFilterWIndowSize;

	unsigned int *m_pSeqHistoryPinData; //历史帧数据
	int *m_pHds; //队列指针
	int *m_pHistoryPinData_ForegoundPinCount; //历史帧数据中前景帧计数

	int *m_pPosForWaitProcPixel; //存储待处理像素的位置游标的容器
	int m_pPfwpp; //游标容器指针

	bool m_bIsEnableMorphologySmooth; //是否开启形态学平滑算法
	bool *m_pToDnrBitColors; //抑噪二值图,用于积分化
	int *m_pIntegrationImage; //积分图
	IntegratDirectionLeftUpPosition *m_pIntegratDirectionLeftUpPosition; //积分方向游标缓存
	FilterWIndowCorner *m_pFilterWIndowCorner; //滤波窗四角游标缓存;
	int m_spaceDnrFrontPixelCountThresholdValue; //测量阈值，滑窗内前景点数量
	int m_spaceDnrBackPixelCountThresholdValue; //测量阈值，滑窗内背景点数量
	int m_spaceDnrFilterWIndowSize; //滑窗尺寸
	int m_openClacStepCount; //开运算计数
	int m_closeClacStepCount; //闭运算计数

	bool m_bIsEnableMedianFilteringAlgorithm; //是否启用中值滤波
};

extern CJForegroundSmoothFilter **ppCJForegroundSmoothFilter;
