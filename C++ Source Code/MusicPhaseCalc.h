#pragma once
# define M_PI           3.14159265358979323846  /* pi */

class MusicPhaseCalc
{
public:
	MusicPhaseCalc() 
	{};
	~MusicPhaseCalc() 
	{};

	bool isEnabled = true;
	bool isHalfTime = false;
	bool isEvenBar = false;
	double phaseInc = 0;
	double presentPhase_Rad = 0;
	short beatsPerBar = 4;
	double fullRotation = 2 * M_PI;
	double emphFunc_Present = 0;
	double emphFunc_MAX = 1;
	double emphFunc_MIN = 0;
	double emphFunc_FREQMULT = 1;
	double emphFunc_C = 0;
	double emphFunc_WID = 2;
	double emphFunc_ORDER = 1;
	double func_Norm = 0;

	float emphFunc_getValue_Overall(float maxVal, float minVal, 
		double presentPhase, float emphasis_FreqMult, float emphasis_C,
		double emphasis_Width_Rad, double emphasis_FuncOrder)
	{
		emphasis_C *= M_PI / 2;
		emphasis_Width_Rad *= M_PI / 2;
		double emphasis_Phase = presentPhase;
		emphasis_Phase = emphasis_Phase > fullRotation ? (emphasis_Phase - fullRotation) : emphasis_Phase;
		double emphasis_C_U = emphasis_C + 2 * M_PI;
		double dist_Rad_L = fabs(emphasis_Phase - emphasis_C);
		double dist_Rad_U = fabs(emphasis_Phase - emphasis_C_U);
		double dist_Rad_Closest = dist_Rad_L > dist_Rad_U ? dist_Rad_U : dist_Rad_L;
		if (dist_Rad_Closest <= emphasis_Width_Rad)
		{
			func_Norm = 1 - pow((float)(dist_Rad_Closest / emphasis_Width_Rad),emphasis_FuncOrder);
			return (minVal + func_Norm * (maxVal - minVal));
		}
		else return minVal;
	}


	void setPhaseInc(double tempo, double refreshFreq)
	{
		double beatTime = 60.0 / tempo;
		double rotationTime = beatsPerBar * beatTime;
		double refreshTime = 1.0 / refreshFreq;
		phaseInc = refreshTime / rotationTime * (2 * M_PI);
		presentPhase_Rad -= phaseInc;
	};

	void setBeatsPerBar(short value)
	{
		beatsPerBar = value;
	}

	void updatePhase()
	{
		presentPhase_Rad = (presentPhase_Rad + phaseInc) > fullRotation ?
						  (presentPhase_Rad + phaseInc - fullRotation): (presentPhase_Rad + phaseInc);
		emphFunc_Present = emphFunc_getValue_Overall(emphFunc_MAX, emphFunc_MIN,
				presentPhase_Rad, emphFunc_FREQMULT, emphFunc_C, emphFunc_WID, emphFunc_ORDER);
	}

	void resetPhase()
	{
		presentPhase_Rad = 0;
	}
};
