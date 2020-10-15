#pragma once
#include "BiQuad.h"

class HFEN_HPF
{
public:
	HFEN_HPF() {};
	~HFEN_HPF() {};

	BiQuad HPF_acc[6];
	float fs = 100;

	void setSampleRate(float fs_ext)
	{
		fs = fs_ext;
		for (int i = 0; i < 6; i++)
				HPF_acc[i].calculateHPFCoeffs(0.2, 0.7, fs);
	}

	float applyHFEN_PreProcess(float inputSample, char axis)
	{
		float out = 0;
		switch (axis)
		{
		case 'x':
			out = HPF_acc[0].doBiQuad(inputSample, 0);
			out = HPF_acc[1].doBiQuad(out, 0);
			break;
		case 'y':
			out = HPF_acc[2].doBiQuad(inputSample, 0);
			out = HPF_acc[3].doBiQuad(out, 0);
			break;
		case 'z':
			out = HPF_acc[4].doBiQuad(inputSample, 0);
			out = HPF_acc[5].doBiQuad(out, 0);
			break;
		}
		return out;
	}
};


