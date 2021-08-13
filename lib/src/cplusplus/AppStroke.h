#ifndef _APPSTROKE_HPP_INCLUDED_
#define _APPSTROKE_HPP_INCLUDED_

#include "Stroke.h"
#include "Basis.h"
 
template<int DIM> 
class AppStroke : public Stroke<DIM>
{
public:
	int  m_Ord = 0;
	int  m_ReSam = 0;

	std::vector<double> m_App[DIM];
	std::vector<double> m_Rsm[DIM];

	std::vector<double> m_Cfs[DIM];

	std::vector <_RSDATA> m_AppRS;

	double m_Lam = 1.0f;
	double m_RMSErr = 0.0f;
	double m_MaxErr = 0.0f;

	Basis  m_Basis;

public:
	void Init(int Ord, int ReSam, int nPoint, double* pPoints)
	{
		if (ReSam <= 0 || Ord <= 0 || nPoint <= 0 || pPoints == nullptr)
			return;

		Stroke<DIM>::Init(nPoint, pPoints);

		m_Basis.Init(Ord, ReSam);

		for (int k = 0; k < DIM; k++)
		{
			m_App[k].resize(ReSam, 0.0);
			m_Rsm[k].resize(ReSam, 0.0);
			m_Cfs[k].resize(Ord,   0.0);
		}

		m_AppRS.resize(ReSam, _RSDATA(0.0, 0.0));

		m_Ord = Ord;
		m_ReSam = ReSam;

		ResetParam();
		ParamApp(1, -1.0);
	}

	void	ResetParam()
	{
		int Sam = Stroke<2>::Size();
		double ds = Stroke<DIM>::m_OrgRS[Sam-1].r / (m_ReSam-1);

		m_AppRS[0].s = 0.0f;
		m_AppRS[0].r = 0.0f;
		for (int i = 1; i < m_ReSam; i++)
		{
			m_AppRS[i].s = ds;
			m_AppRS[i].r = m_AppRS[i - 1].r + ds;
		}
	}

	int	ParamApp(int nItr, float TargetErr = -1.0f)
	{
		int it = 0;
		for (; it < nItr; it++)
		{
			Repar();
			Approx();
			
			Tracing();
			AppErr();
			if (TargetErr > 0.0f && m_RMSErr < TargetErr)
				break;
		} // End of it

		return it;

	}

	void Repar()
	{
		int Sam = Stroke<2>::Size();

		for (int k = 0; k < DIM; k++)
		{
			m_Rsm[k][0] = Stroke<DIM>::m_Org[0][k];
			m_Rsm[k][m_ReSam - 1] = Stroke<DIM>::m_Org[Sam - 1][k];
		}

		m_Lam = Stroke<DIM>::m_OrgRS[Sam - 1].r / m_AppRS[m_ReSam - 1].r;

		int i_org = 1;
		for (int i_app = 1; i_app < (m_ReSam - 1); ++i_app)
		{
			double Curr = m_Lam * m_AppRS[i_app].r;

			while (Curr > Stroke<DIM>::m_OrgRS[i_org].r + 0.001)
			{
				i_org++;
				if (i_org == Sam)
				{
					// something went wrong!
					
					for (; i_app < m_ReSam; i_app++)
					{
						for (int k = 0; k < DIM; k++)
							m_Rsm[k][i_app] = Stroke<DIM>::m_Org[(Sam - 1)][k] / m_Lam;
					}
					
					break;
				}	 // if( j==Sam )
				
			}	// while

			if (i_org >= Sam)
				break;

			double Alf = 1.0;
			if (Stroke<DIM>::m_OrgRS[i_org].s > 0.000001)
				Alf = 1.0 - (Stroke<DIM>::m_OrgRS[i_org].r - Curr) / Stroke<DIM>::m_OrgRS[i_org].s;

			for (int k = 0; k < DIM; k++)
			{
				double  x2 = Stroke<DIM>::m_Org[i_org  ][k];
				double  x1 = Stroke<DIM>::m_Org[i_org-1][k];
				double  dx = x2 - x1;

				m_Rsm[k][i_app] = x1 + dx*Alf;
			}
		}
	}

	void Approx()
	{
		int Ord = m_Basis.m_Ord;

		for (int k = 0; k < DIM; k++)
		{
			m_App[k].assign(m_ReSam, 0.0);

			for (int i = 0; i < Ord; i++)
			{
				Basis::it_T BasV = m_Basis.GetVector(i);

				double C = 0.0;
				for (int j = 0; j < m_ReSam; j++, ++BasV)
					C +=  m_Rsm[k][j] * (*BasV);

				m_Cfs[k][i] = C;
			}

			for (int i = 0; i < Ord; i++)
			{
				Basis::it_T BasV = m_Basis.GetVector(i);
				double C = m_Cfs[k][i];
				for (int j = 0; j < m_ReSam; j++, ++BasV)
					m_App[k][j] += C * (*BasV);
			}
		}
	}

	void	Tracing()
	{
		int Sam = Stroke<2>::Size();

		m_AppRS[0].s = m_AppRS[0].r = 0.0;

		for (int i = 1; i < m_ReSam; i++)
		{
			double  ds = 0.0;
			for (int k = 0; k < DIM; k++)
			{
				double dx = m_App[k][i] - m_App[k][i-1];
				ds += dx * dx;
			}

			ds = sqrt(ds);
			m_AppRS[i].s = ds;
			m_AppRS[i].r = m_AppRS[i-1].r + ds;
		}

		m_Lam = Stroke<DIM>::m_OrgRS[Sam-1].r / m_AppRS[m_ReSam-1].r;
	}
	void	AppErr()
	{
		m_RMSErr = 0;
		m_MaxErr = 0;

		for (int i = 0; i < m_ReSam; i++)
		{
			double E = 0;
			for (int k = 0; k < DIM; k++)
			{
				double  T = m_App[k][i] - m_Rsm[k][i];
				E += T * T;
			}

			m_RMSErr += E;
			if (m_MaxErr < E)
				m_MaxErr = E;
		}

		m_RMSErr = (float)sqrt(m_RMSErr / m_ReSam);
		m_MaxErr = sqrt(m_MaxErr);
	}

	double   GetRMSErr() { return m_RMSErr; }
	double   GetMaxErr() { return m_MaxErr; }
};

#endif 