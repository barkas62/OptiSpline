
#ifndef _BASIS_HPP_INCLUDED_
#define _BASIS_HPP_INCLUDED_

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

class Basis  
{
public:
	int		m_Ord; 
	int		m_ReSam; 
	std::vector<double> m_Basis;
	std::vector<double> m_Norms;

	using it_T = typename std::vector<double>::iterator;

public:

	void Init(int Ord, int ReSam)
	{
		m_Basis.resize(Ord*ReSam, 1.0);
		m_Norms.resize(Ord, 0.0);

		m_Ord = Ord;
		m_ReSam = ReSam;

		double dt = M_PI / ReSam;
		for (int i = 1; i < Ord; i++)
		{
			double t = dt / 2.0;
			it_T v = GetVector(i);
			for (int j = 0; j < ReSam; j++, t += dt, ++v)
				*v = cos(i * t);
		} // for(i)

		Normalize();
	}

	it_T GetVector(int i)
	{
		return m_Basis.begin() + i*m_ReSam;
	}

	void Normalize()
	{
		it_T v;

		for (int i = 0; i < m_Ord; i++)
		{ 
			v = GetVector(i);

			double Norm = 0.0f;
			for (int j = 0; j < m_ReSam; j++, ++v)
				Norm += (*v) * (*v);

			Norm = sqrt(Norm);
			m_Norms[i] = Norm;

			v = GetVector(i);
			for (int j = 0; j < m_ReSam; j++, ++v)
				*v /= Norm;
		} // for(i)
	}
};

#endif 
