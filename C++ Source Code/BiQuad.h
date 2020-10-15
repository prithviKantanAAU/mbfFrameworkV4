#pragma once
// includes for the project
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FLT_MIN_PLUS          1.175494351e-38         /* min positive value */
#define FLT_MIN_MINUS        -1.175494351e-38         /* min negative value */                
#define M_PI		          3.141592653589793238462643383279502884L /* pi */

class BiQuad
{
public:
	float fsDefault = 100;
	BiQuad(void)
	{
		calculateLPFCoeffs(0.5, 1,fsDefault);
		flushDelays();
	}
	virtual ~BiQuad(void)
	{}

protected:
	float m_f_Xz_1; // x z-1 delay element
	float m_f_Xz_2; // x z-2 delay element
	float m_f_Yz_1; // y z-1 delay element
	float m_f_Yz_2; // y z-2 delay element

	// allow other objects to set these directly since we have no cooking
	// function or intelligence

public:
	double m_f_a0;
	double m_f_a1;
	double m_f_a2;
	double m_f_b1;
	double m_f_b2;

	// Analysis
	bool isMaxima = false;
	bool isMinima = false;
	bool isZeroCrossing_Pos = false;
	bool isZeroCrossing_Neg = false;
	bool isThreshCrossing_Pos = false;
	bool isThreshCrossing_Neg = false;
	bool lastMin_NEG = false;

	float lastMaximum = 0;
	float lastMinimum = 0;

	// FLUSH DELAY ELEMENTS
	void flushDelays()
	{
		m_f_Xz_1 = 0;
		m_f_Xz_2 = 0;
		m_f_Yz_1 = 0;
		m_f_Yz_2 = 0;
	}
	
	// LOW PASS COEFFICIENTS
	void calculateLPFCoeffs(float fCutoffFreq, float fQ, float fs)
	{
		// use same terms as in book:
		float theta_c = 2.0*M_PI*fCutoffFreq / fs;
		float d = 1.0 / fQ;

		// intermediate values
		float fBetaNumerator = 1.0 - ((d / 2.0)*(sin(theta_c)));
		float fBetaDenominator = 1.0 + ((d / 2.0)*(sin(theta_c)));

		// beta
		float fBeta = 0.5*(fBetaNumerator / fBetaDenominator);

		// gamma
		float fGamma = (0.5 + fBeta)*(cos(theta_c));

		// alpha
		float fAlpha = (0.5 + fBeta - fGamma) / 2.0;

		// left channel
		m_f_a0 = (0.5 + fBeta - fGamma) / 2.0;
		m_f_a1 = 0.5 + fBeta - fGamma;
		m_f_a2 = (0.5 + fBeta - fGamma) / 2.0;
		m_f_b1 = -2 * fGamma;
		m_f_b2 = 2 * fBeta;
	}

	// HIGH PASS COEFFICIENTS
	void calculateHPFCoeffs(float fCutoffFreq, float fQ, float fs)
	{
		// use same terms as in book:
		float theta_c = 2.0*M_PI*fCutoffFreq / fs;
		float d = 1.0 / fQ;

		// intermediate values
		float fBetaNumerator = 1.0 - ((d / 2.0)*(sin(theta_c)));
		float fBetaDenominator = 1.0 + ((d / 2.0)*(sin(theta_c)));

		// beta
		float fBeta = 0.5*(fBetaNumerator / fBetaDenominator);

		// gamma
		float fGamma = (0.5 + fBeta)*(cos(theta_c));

		// alpha
		float fAlpha = (0.5 + fBeta - fGamma) / 2.0;

		// left channel
		m_f_a0 = (0.5 + fBeta + fGamma) / 2.0;
		m_f_a1 = -(0.5 + fBeta + fGamma);
		m_f_a2 = (0.5 + fBeta + fGamma) / 2.0;
		m_f_b1 = -2 * fGamma;
		m_f_b2 = 2 * fBeta;
	}

	// FILTER EACH SAMPLE
	float doBiQuad(float f_xn,float thresh)
	{
		isMaxima = false;
		isMinima = false;
		isZeroCrossing_Pos = false;
		isZeroCrossing_Neg = false;
		isThreshCrossing_Pos = false;
		isThreshCrossing_Neg = false;

		float yn = m_f_a0 * f_xn + m_f_a1 * m_f_Xz_1 + m_f_a2 * m_f_Xz_2 
				   - m_f_b1 * m_f_Yz_1 - m_f_b2 * m_f_Yz_2;

		// underflow check
		if (yn > 0.0 && yn < FLT_MIN_PLUS) yn = 0;
		if (yn < 0.0 && yn > FLT_MIN_MINUS) yn = 0;

		// X delay Shuffle
		m_f_Xz_2 = m_f_Xz_1;
		m_f_Xz_1 = f_xn;		

		bool toCheckMaxMin = false;
		toCheckMaxMin = thresh > 0 ? m_f_Yz_1 > thresh : m_f_Yz_1 < thresh;

		//Check Maxima and Minima
		if (toCheckMaxMin)
		{
			//Check Maxima
			if (m_f_Yz_1 > m_f_Yz_2 && m_f_Yz_1 > yn)
			{
				isMaxima = true;
				lastMaximum = m_f_Yz_1;
			}

			if (m_f_Yz_1 < m_f_Yz_2 && m_f_Yz_1 < yn)
			{
				isMinima = true;
				if (m_f_Yz_1 < 0)
					lastMin_NEG = true;
				else
					lastMin_NEG = false;
				lastMinimum = m_f_Yz_1;
			}
		}

		if (yn >= 0 && m_f_Yz_1 < 0)
			isZeroCrossing_Pos = true;

		if (yn <= 0 && m_f_Yz_1 > 0)
			isZeroCrossing_Neg = true;

		if (yn >= thresh && m_f_Yz_1 < thresh)
			isThreshCrossing_Pos = true;

		if (yn <= thresh && m_f_Yz_1 > thresh)
			isThreshCrossing_Neg = true;
				
		m_f_Yz_2 = m_f_Yz_1;
		m_f_Yz_1 = yn;

		return yn;
	}	
};

