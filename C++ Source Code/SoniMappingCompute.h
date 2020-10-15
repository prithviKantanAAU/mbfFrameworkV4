#pragma once
#include "BiQuad.h"

class SoniMappingCompute
{
public:
	SoniMappingCompute() { paramSmoothing.flushDelays(); };		// Constructor
	~SoniMappingCompute() {};									// Destructor
	BiQuad paramSmoothing;										// Smooth AP
	float soniVal_Slider = 0.0;									// Soni Slider Value
	void setSoniVal_Slider(float value)							// Soni Mode Slider - Set Value
	{ soniVal_Slider = value; };								
	bool isSoniSource_Slider = true;							// Flag - Soni Source Slider?
	void setSonificationMode(bool isSlider)						// Set Soni Mode - Slider OR Sensor
	{	isSoniSource_Slider = isSlider;	};
	float sampleRate = 100;										// Sensor Sample Rate for Filtering
	void setSampleRate(float fs) { sampleRate = fs; };			// Set Sample Rate
	void setFilterFc(float fc)
	{
		paramSmoothing.calculateLPFCoeffs(fc, 0.7, sampleRate);
	};

	// Compute AP Value - From Sensor
	float computeParamValue(float currentVal, float targetVal, float *functionParamSet)
	{
			float order_mappingFunc = functionParamSet[0];
			float toleranceBW = functionParamSet[1];
			int numQuantLevels = functionParamSet[2];
			float smoothingFc = functionParamSet[3];
			float originalRange = functionParamSet[4];
			short desiredBehavior = functionParamSet[5];
			float currentError = 0;
			float adaptiveRange = 0;
			float output = 0.0;

			switch (desiredBehavior)
			{
			case 0:
				adaptiveRange = fmax(originalRange - targetVal, originalRange / 10);
				if (currentVal <= targetVal)	currentError = 0;
				else currentError = fabs(currentVal - targetVal) / adaptiveRange;
				output = pow(fabs(currentError), order_mappingFunc);
				break;
			case 1:
				adaptiveRange = fmax(targetVal, originalRange / 4);
				currentError = fabs(currentVal - targetVal) / adaptiveRange;
				if (fabs(currentError) > toleranceBW)
					output = pow(fabs(currentError), order_mappingFunc);
				break;
			case 2:
				adaptiveRange = fmax(targetVal, originalRange / 4);
				if (currentVal > targetVal) 	currentError = 0;
				else currentError = fabs(currentVal - targetVal) / adaptiveRange;
				output = pow(fabs(currentError), order_mappingFunc);
				break;
			}

			if (smoothingFc != 100) 	output = paramSmoothing.doBiQuad(output, 0);
			return quantizeParam(output, numQuantLevels);
	};

	// Compute AP Value - From Slider
	float computeParamValue_SliderSource(float distance, float targetVal, float *functionParamSet)
	{
		float order_mappingFunc = functionParamSet[0];
		float toleranceBW = functionParamSet[1];
		int numQuantLevels = functionParamSet[2];
		float smoothingFc = functionParamSet[3];
		float originalRange = functionParamSet[4];
		short desiredBehavior = functionParamSet[5];
		float currentVal = distance * originalRange;
		float currentError = 0;
		float adaptiveRange = 0;
		float output = 0.0;

		switch (desiredBehavior)
		{
		case 0:
			adaptiveRange = fmax(originalRange - targetVal, originalRange / 10);
			if (currentVal <= targetVal)	currentError = 0;
			else currentError = fabs(currentVal - targetVal) / adaptiveRange;
			output = pow(fabs(currentError), order_mappingFunc);
			break;
		case 1:
			adaptiveRange = fmax(targetVal, originalRange / 4);
			currentError = fabs(currentVal - targetVal) / adaptiveRange;
			if (fabs(currentError) > toleranceBW)
				output = pow(fabs(currentError), order_mappingFunc);
			break;
		case 2:
			adaptiveRange = fmax(targetVal, originalRange / 4);
			if (currentVal > targetVal) 	currentError = 0;
			else currentError = fabs(currentVal - targetVal) / adaptiveRange;
			output = pow(fabs(currentError), order_mappingFunc);
			break;
		}

		if (smoothingFc != 100)		output = paramSmoothing.doBiQuad(output, 0);
		return quantizeParam(output, numQuantLevels);
	};

	// Quantize AP Value to a certain number of steps
	float quantizeParam(float currentParamValue, int numQuantizationSteps)
	{
		float quantizedParam = 0;
		if (numQuantizationSteps == 0)
			return currentParamValue;

		else
		{
			float quantizationStepSize = 1 / (float)numQuantizationSteps;

			float diff = 100;
			for (int i = 0; i < numQuantizationSteps + 1; i++)
			{
				float currentStepForTest = 0;
				currentStepForTest += i * quantizationStepSize;
				if (diff > fabs(currentParamValue - currentStepForTest))
				{
					diff = currentParamValue - currentStepForTest;
				}
			}
			quantizedParam = currentParamValue - diff;
			return quantizedParam;
		}
	};
};
