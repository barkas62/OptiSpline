#pragma once

#include "Basis.h"
#include "AppStroke.h"

class OptiSplineContext
{
	Basis      m_Basis;
	AppStroke  m_AppStrk;

public:
	OptiSplineContext(int nDim, int nSam, int nOrd, int nResam, float* pPnt)
	{
		m_Basis.Init(nOrd, nResam);
		m_AppStrk.Init(nDim, nSam, nOrd, nResam, pPnt);

		m_AppStrk.ParamApp(0);
	}

};

