#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
# define M_PI           3.14159265358979323846  /* pi */

class dynTrajectory
{
public:
	dynTrajectory() {};
	~dynTrajectory() {};

	String shapes[10] = 
	{
		"Manual",
		"Linear ML",
		"Linear AP",
		"Linear Diagonal",
		"Circle",
		"Diamond",
		"Square",
		"Plus Shape",
		"Cross Shape",
		""
	};
	
	int numShapes = 9;
	int currentShape = 8;
	bool isMirrored = false;
	float radius_Deg = 15;
	float period_Bars = 1;
	int barsElapsed = 0;
	float anticipationPhase = 0;
	float anticipationTime_Sec = 0.15;

	void calcAnticipationPhase(float tempo)
	{
		float barTime = 60.0 / tempo * 4;
		anticipationPhase = anticipationTime_Sec / barTime * (2 * M_PI);
	}

	int numSegments[10] =
	{
		0,
		4,
		4,
		4,
		0,
		4,
		4,
		8,
		8,
		0
	};

	int segment_X[10][10] = 
	{
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},			// Manual
		{1, 0, -1, 0, 0, 0, 0, 0, 0, 0},		// Linear ML
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},			// Linear AP
		{1, 0, -1, 0, 0, 0, 0, 0, 0, 0},		// Diagonal
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},			// Circle
		{0, 1, 0, -1, 0, 0, 0, 0, 0, 0},		// Diamond
		{1, 1, -1, -1, 0, 0, 0, 0, 0, 0},		// Square
		{0, 0, 0, 1, 0, 0, 0, -1, 0, 0},		// Plus Sign
		{0, 1, 0, 1, 0, -1, 0, -1, 0, 0},		// Cross Shape
	};

	int segment_Y[10][10] =
	{
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},			// Manual
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},			// Linear ML
		{1, 0, -1, 0, 0, 0, 0, 0, 0, 0},		// Linear AP
		{1, 0, -1, 0, 0, 0, 0, 0, 0, 0},		// Diagonal
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},			// Circle
		{1, 0, -1, 0, 0, 0, 0, 0, 0, 0},		// Diamond
		{1, -1, -1, 1, 0, 0, 0, 0, 0, 0},		// Square
		{0, 1, 0, 0, 0, -1, 0, 0, 0, 0},		// Plus Sign
		{0, 1, 0, -1, 0, -1, 0, 1, 0, 0},		// Cross Shape
	};

	void getCenterCoordinates(float phase, float *outArray_XY)
	{
		int currentBar_InPeriod = barsElapsed % (int)period_Bars;
		float rad_perBar = 2 * M_PI / period_Bars;
		phase = rad_perBar * currentBar_InPeriod + phase / period_Bars;

		if (currentShape == 0)
			return;

		if (numSegments[currentShape] > 0)
			getCenterCoordinates_Segmented(phase, outArray_XY);

		if (currentShape == 4)
			getCenterCoordinates_Circle(phase, outArray_XY);
	};

	void getCenterCoordinates_Segmented(float phase, float* xyArray)
	{
		float rad_perSegment = 2 * M_PI / numSegments[currentShape];
		int currentSegment = (int)(phase / rad_perSegment);
		
		float phaseStart_currentSegment = currentSegment * rad_perSegment;
		float phaseEnd_currentSegment = (currentSegment + 1) * rad_perSegment;
		
		float phase_sinceSegStart = phase - phaseStart_currentSegment;
		float phase_tillSegEnd = phaseEnd_currentSegment - phase;
		
		float currentSegment_start_X = segment_X[currentShape][currentSegment];
		float currentSegment_end_X = segment_X[currentShape][(currentSegment + 1) % numSegments[currentShape]];
		float currentSegment_start_Y = segment_Y[currentShape][currentSegment];
		float currentSegment_end_Y = segment_Y[currentShape][(currentSegment + 1) % numSegments[currentShape]];
		
		float val_X = 0;
		float val_Y = 0;

		val_X = radius_Deg * (currentSegment_start_X * phase_tillSegEnd
				+ currentSegment_end_X * phase_sinceSegStart) / rad_perSegment;

		val_Y = radius_Deg * (currentSegment_start_Y * phase_tillSegEnd
			+ currentSegment_end_Y * phase_sinceSegStart ) / rad_perSegment;

		xyArray[0] = isMirrored ? val_X : -1 * val_X;
		xyArray[1] = val_Y;
	}

	void getCenterCoordinates_Circle(float phase, float* xyArray)
	{
		float val_X = radius_Deg * cos(phase);
		float val_Y = radius_Deg * sin(phase);
		xyArray[0] = isMirrored ? val_X : -1 * val_X;
		xyArray[1] = val_Y;
	}
};
