#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class AudioParam_Single
{
public:

	String name = "";
	int faustIndex = 1;
	short type = 1;										// 1 = FAUST // 2 = SEQUENCER
	void set_nameTypeAddress(String stratNAME, int idx, short TYPE)	// Mandatory Call 1
	{
		name = stratNAME;
		faustIndex = idx;
		type = TYPE;
	}
	bool isIncluded_UseScenarios[10] = { false };
	void set_isIncluded_UseScenarios(bool* boolArray, short numExerciseModes)	// Mandatory Call 2
	{
		for (int i = 0; i < numExerciseModes; i++)
			isIncluded_UseScenarios[i] = boolArray[i];
	}

	float mappingOrder = 1;
	float minVal = 0;
	float maxVal = 1;
	float toleranceBW = 0;
	int quantLevels = 0;
	float smoothingFc = 100;
	float resetVal = 0;

	void initializeSoniParams(float mapOrder, float mini, float maxi,
		float tolBW, float qLevels, float smoothFc, float resetValue)				// Mandatory Call 3
	{
		mappingOrder = mapOrder;
		minVal = mini;
		maxVal = maxi;
		toleranceBW = tolBW;
		quantLevels = qLevels;
		smoothingFc = smoothFc;
		resetVal = resetValue;
	}
	
	AudioParam_Single() 
	{
	};
	
	~AudioParam_Single()
	{
	};
};

