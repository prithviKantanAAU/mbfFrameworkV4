#pragma once
#include <string>
#include "GaitParam_Single.h"
#include "BiQuad.h"

class gaitParamInfo
{
public:

	short numMovementParams = 0;					// TOTAL MP's - COUNT
	short activeGaitParam = 0;						// ACTIVE MP
	short num_UseScenarios = 6;						// NUM EXERCISE MODES
	short indices_MP[10][30] = { 0 };				// PER EXERCISE MODE - MP INDICES
	short num_MP[10] = { 0 };						// PER EXERCISE MODE - MP COUNT
	float apVal_DYN_TaskDependent[2] = { 0.0 };		// X and Y INFO
	short numSensorLocations = 3;					// BODILY LOCATIONS
	String exerciseModes[10] = { "Testing","Static Upright","Dynamic Trunk","Jerk Feedback","STS Angle Cue","Gait" };
	short exerciseMode_Present = 1;
	GaitParam_Single gaitParam_ObjectArray[30];
	float AP_Val = 0;
	float order_MapFunc = 1;
	float numQuant = 0;
	bool isSliderMode = true;
	bool isPolarityNormal = true;
	float sliderVal = 0;
	BiQuad apSmooth;
	float apSmooth_Fc = 0;
	float repTime_Sec[5000][10] = { 0.0 };
	int reps_Completed[10] = { 0 };
	float repTime_AVG_GLOBAL[10] = { 0.0 };
	float repTime_AVG_LAST5[10] = { 0.0 };
	void flush_repInfo()
	{
		for (int i = 0; i < 10; i++)
		{
			reps_Completed[i] = 0;
			repTime_AVG_GLOBAL[i] = 0;
			repTime_AVG_LAST5[i] = 0;
			for (int j = 0; j < 5000; j++)
				repTime_Sec[j][i] = 0;
		}
	}

	gaitParamInfo() 
	{
		// 0					  TEST   SB     DB    JERK  ANGLE  GAIT   PHASE
		bool useCaseArray0[10] = { true, true, true, false, false, false, false };
		//							TRUNK FEET
		bool sensorReqArray0[5] = { true, false };
		gaitParam_ObjectArray[0].setName_SensorReq("Inclination (+-) - ML",
			sensorReqArray0,numSensorLocations);
		gaitParam_ObjectArray[0].set_isIncluded_UseScenarios(useCaseArray0, num_UseScenarios);
		gaitParam_ObjectArray[0].storeFilterInfo(50, 1);
		gaitParam_ObjectArray[0].initialize(-45, 45, -15, 15);
		numMovementParams++;

		// 1					  TEST   SB     DB    JERK  ANGLE  GAIT   PHASE
		bool useCaseArray1[10] = { true, true, true, false, false, false, false };
		//							TRUNK FEET
		bool sensorReqArray1[5] = { true, false };
		gaitParam_ObjectArray[1].setName_SensorReq("Inclination (+-) - AP",
			sensorReqArray1, numSensorLocations);
		gaitParam_ObjectArray[1].set_isIncluded_UseScenarios(useCaseArray1, num_UseScenarios);
		gaitParam_ObjectArray[1].storeFilterInfo(50, 1);
		gaitParam_ObjectArray[1].initialize(-45, 45, -15, 15);
		numMovementParams++;

		// 2					  TEST   SB     DB    JERK  ANGLE  GAIT   PHASE
		bool useCaseArray2[10] = { true, true, true, false, false, false, false };
		//							TRUNK FEET
		bool sensorReqArray2[5] = { true, false };
		gaitParam_ObjectArray[2].setName_SensorReq("Trunk Projection Zone",
			sensorReqArray2, numSensorLocations);
		gaitParam_ObjectArray[2].set_isIncluded_UseScenarios(useCaseArray2, num_UseScenarios);
		gaitParam_ObjectArray[2].storeFilterInfo(50, 1);
		gaitParam_ObjectArray[2].initialize(1, 6, 1, 2);
		gaitParam_ObjectArray[2].setCalibrationType(3);
		numMovementParams++;

		// 3					  TEST   SB     DB    JERK  ANGLE  GAIT   PHASE
		bool useCaseArray3[10] = { false, false, false, false, false, false, false };
		//							TRUNK FEET
		bool sensorReqArray3[5] = { true, false };
		gaitParam_ObjectArray[3].setName_SensorReq("RMS Acc ML",
			sensorReqArray3, numSensorLocations);
		gaitParam_ObjectArray[3].set_isIncluded_UseScenarios(useCaseArray3, num_UseScenarios);
		gaitParam_ObjectArray[3].storeFilterInfo(50, 1);
		gaitParam_ObjectArray[3].initialize(0, 20, 0, 5);
		numMovementParams++;

		// 4					  TEST   SB     DB    JERK  ANGLE  GAIT   PHASE
		bool useCaseArray4[10] = { false, false, false, false, false, false, false };
		//							TRUNK FEET
		bool sensorReqArray4[5] = { true, false };
		gaitParam_ObjectArray[4].setName_SensorReq("RMS Acc AP",
			sensorReqArray4, numSensorLocations);
		gaitParam_ObjectArray[4].set_isIncluded_UseScenarios(useCaseArray4, num_UseScenarios);
		gaitParam_ObjectArray[4].storeFilterInfo(5, 5);
		gaitParam_ObjectArray[4].initialize(0, 20, 0, 5);
		numMovementParams++;

		// 5					   TEST   SB     DB    JERK  ANGLE  GAIT   PHASE
		bool useCaseArray5[10] = { true, false, false, true, false, false, false };
		//							TRUNK FEET
		bool sensorReqArray5[5] = { true, false };
		gaitParam_ObjectArray[5].setName_SensorReq("Scalar Jerk",
			sensorReqArray5, numSensorLocations);
		gaitParam_ObjectArray[5].set_isIncluded_UseScenarios(useCaseArray5, num_UseScenarios);
		gaitParam_ObjectArray[5].storeFilterInfo(50, 1);
		gaitParam_ObjectArray[5].initialize(0, 100, 0, 15);
		numMovementParams++;

		// 6					   TEST   SB     DB    JERK  ANGLE  GAIT   PHASE
		bool useCaseArray6[10] = { true, false, false, true, false, false, false };
		//							TRUNK FEET
		bool sensorReqArray6[5] = { true, false };
		gaitParam_ObjectArray[6].setName_SensorReq("Jerk - X",
			sensorReqArray6, numSensorLocations);
		gaitParam_ObjectArray[6].set_isIncluded_UseScenarios(useCaseArray6, num_UseScenarios);
		gaitParam_ObjectArray[6].storeFilterInfo(10, 2);
		gaitParam_ObjectArray[6].initialize(0, 25, 0, 5);
		numMovementParams++;

		// 7					   TEST   SB     DB    JERK  ANGLE  GAIT   PHASE
		bool useCaseArray7[10] = { true, false, false, true, false, false, false };
		//							TRUNK FEET
		bool sensorReqArray7[5] = { true, false };
		gaitParam_ObjectArray[7].setName_SensorReq("Jerk - Y",
			sensorReqArray7, numSensorLocations);
		gaitParam_ObjectArray[7].set_isIncluded_UseScenarios(useCaseArray7, num_UseScenarios);
		gaitParam_ObjectArray[7].storeFilterInfo(10, 2);
		gaitParam_ObjectArray[7].initialize(0, 25, 0, 5);
		numMovementParams++;

		// 8					   TEST   SB     DB    JERK  ANGLE  GAIT   PHASE
		bool useCaseArray8[10] = { true, false, false, true, false, false, false };
		//							TRUNK FEET
		bool sensorReqArray8[5] = { true, false };
		gaitParam_ObjectArray[8].setName_SensorReq("Jerk - Z",
			sensorReqArray8, numSensorLocations);
		gaitParam_ObjectArray[8].set_isIncluded_UseScenarios(useCaseArray8, num_UseScenarios);
		gaitParam_ObjectArray[8].storeFilterInfo(10, 2);
		gaitParam_ObjectArray[8].initialize(0, 25, 0, 5);
		numMovementParams++;

		// 9					   TEST   SB     DB    JERK  ANGLE  GAIT   PHASE
		bool useCaseArray9[10] = { true, true, false, false, false, false, false };
		//							TRUNK FEET
		bool sensorReqArray9[5] = { true, false };
		gaitParam_ObjectArray[9].setName_SensorReq("Sway Vel - ML",
			sensorReqArray9, numSensorLocations);
		gaitParam_ObjectArray[9].set_isIncluded_UseScenarios(useCaseArray9, num_UseScenarios);
		gaitParam_ObjectArray[9].storeFilterInfo(5, 2);
		gaitParam_ObjectArray[9].initialize(0, 200, 0, 20);
		numMovementParams++;

		// 10				   	   TEST   SB     DB    JERK  ANGLE  GAIT   PHASE
		bool useCaseArray10[10] = { true, true, false, false, false, false, false };
		//							TRUNK FEET
		bool sensorReqArray10[5] = { true, false };
		gaitParam_ObjectArray[10].setName_SensorReq("Sway Vel - AP",
			sensorReqArray10, numSensorLocations);
		gaitParam_ObjectArray[10].set_isIncluded_UseScenarios(useCaseArray10, num_UseScenarios);
		gaitParam_ObjectArray[10].storeFilterInfo(5, 2);
		gaitParam_ObjectArray[10].initialize(0, 200, 0, 20);
		numMovementParams++;

		// 11				   	    TEST   SB     DB    JERK  ANGLE  GAIT   PHASE
		bool useCaseArray11[10] = { true, false, false, false, false, true, false };
		//							TRUNK FEET
		bool sensorReqArray11[5] = { false, true };
		gaitParam_ObjectArray[11].setName_SensorReq("HS Timing",
			sensorReqArray11, numSensorLocations);
		gaitParam_ObjectArray[11].set_isIncluded_UseScenarios(useCaseArray11, num_UseScenarios);
		gaitParam_ObjectArray[11].storeFilterInfo(50, 1);
		gaitParam_ObjectArray[11].initialize(0, 1, 0, 0);
		gaitParam_ObjectArray[11].setCalibrationType(2);
		numMovementParams++;

		// 12				   	    TEST   SB     DB    JERK  ANGLE  GAIT   PHASE
		bool useCaseArray12[10] = { true, false, false, false, false, true, false };
		//							TRUNK FEET
		bool sensorReqArray12[5] = { false, true };
		gaitParam_ObjectArray[12].setName_SensorReq("HS Trigger",
			sensorReqArray12, numSensorLocations);
		gaitParam_ObjectArray[12].set_isIncluded_UseScenarios(useCaseArray12, num_UseScenarios);
		gaitParam_ObjectArray[12].storeFilterInfo(50, 1);
		gaitParam_ObjectArray[12].initialize(0, 1, 0, 0);
		gaitParam_ObjectArray[12].setCalibrationType(2);
		numMovementParams++;

		// 13				   	    TEST   SB     DB    JERK  ANGLE  GAIT   PHASE
		bool useCaseArray13[10] = { true, false, false, false, true, false, false };
		//							TRUNK FEET
		bool sensorReqArray13[5] = { true, false };
		gaitParam_ObjectArray[13].setName_SensorReq("STS Cue",
			sensorReqArray13, numSensorLocations);
		gaitParam_ObjectArray[13].set_isIncluded_UseScenarios(useCaseArray13, num_UseScenarios);
		gaitParam_ObjectArray[13].storeFilterInfo(50, 1);
		gaitParam_ObjectArray[13].initialize(0, 1, 0, 0);
		gaitParam_ObjectArray[13].setCalibrationType(1);
		numMovementParams++;
		
		populate_MP_Matrix();

		apSmooth.flushDelays();
		apSmooth.calculateLPFCoeffs(50, 0.7, 100);
	};

	void populate_MP_Matrix()
	{
		for (int i = 0; i < numMovementParams; i++)			// SONI
		{
			for (int j = 0; j < num_UseScenarios; j++)		// USE CASE SCENARIO
			{
				if (gaitParam_ObjectArray[i].isIncluded_UseScenarios[j])
				{
					indices_MP[j][num_MP[j]] = i;
					num_MP[j] += 1;
				}
			}
		}
	};

	void setTargetValue(float normVal, bool isMax)
	{
		float realVal = gaitParam_ObjectArray[activeGaitParam].minVal
			+ normVal * (gaitParam_ObjectArray[activeGaitParam].maxVal 
				- gaitParam_ObjectArray[activeGaitParam].minVal);

		gaitParam_ObjectArray[activeGaitParam].updateValue_Target(realVal, isMax);
	};

	double calc_AP_Val()
	{
		GaitParam_Single *present_MP_Obj = &gaitParam_ObjectArray[activeGaitParam];
		float MP_Val = 0;
		if (isSliderMode)
		{
			MP_Val = present_MP_Obj->minVal + sliderVal * (present_MP_Obj->maxVal - present_MP_Obj->minVal);
		}
		else
		{
			MP_Val = present_MP_Obj->currentValue;
		}

		float adaptiveRange = 0;
		float error = 0;
		float MP_MIN = present_MP_Obj->minVal;
		float MP_MAX = present_MP_Obj->maxVal;
		float MP_MIN_TGT = present_MP_Obj->target_MIN;
		float MP_MAX_TGT = present_MP_Obj->target_MAX;

		// IF TARGET MET, RETURN 0
		if (MP_Val <= MP_MAX_TGT && MP_Val >= MP_MIN_TGT)
			return isPolarityNormal ? 0 : 1;

		else
		{
			if (MP_Val < MP_MIN_TGT)
			{
				adaptiveRange = MP_MIN_TGT - MP_MIN;
				error = (MP_MIN_TGT - MP_Val) / adaptiveRange;
			}

			else if (MP_Val > MP_MAX_TGT)
			{
				adaptiveRange = MP_MAX - MP_MAX_TGT;
				error = (MP_Val - MP_MAX_TGT) / adaptiveRange;
			}

			error = isPolarityNormal ? error : (1 - error);
			AP_Val = pow(error, order_MapFunc);
			if (apSmooth_Fc > 0.01)
			AP_Val = apSmooth.doBiQuad(AP_Val, 0);
			return quantizeParam(AP_Val, numQuant);
		}
	};

	void set_APSmooth_fc(float freq)
	{
		apSmooth_Fc = freq;
		if (freq > 0.01)
		apSmooth.calculateLPFCoeffs(freq, 0.7, 100);
	};

	double quantizeParam(float currentParamValue, int numQuantizationSteps)
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

	void updateGaitParameter(int index) {	activeGaitParam = index - 1;   };
	
	~gaitParamInfo() {};

	void incrementNumReps(float *newRepTime)
	{
		short exMode = exerciseMode_Present - 1;
		repTime_Sec[reps_Completed[exMode]][exMode] = *newRepTime;
		reps_Completed[exMode]++;

		repTime_AVG_GLOBAL[exMode] =
			(repTime_AVG_GLOBAL[exMode] * (reps_Completed[exMode] - 1) + *newRepTime) / reps_Completed[exMode];

		int idx_latestRep = reps_Completed[exMode];
		int idx_latestRep_min5 = max(0, (reps_Completed[exMode] - 5));
		float avg_last5 = 0;
		for (int i = idx_latestRep_min5; i < idx_latestRep; i++)
			avg_last5 += repTime_Sec[i][exMode];
		repTime_AVG_LAST5[exMode] = avg_last5 / 5.0;
		*newRepTime = 0;
	}
};

