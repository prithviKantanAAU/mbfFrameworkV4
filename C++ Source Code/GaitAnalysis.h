#pragma once
#include "OSC_Class.h"
#include "BiQuad.h"
#include <math.h>
#include "gaitParamInfo.h"
#include "SensorInfo.h"
#include "HFEN_HPF.h"
#include "../JuceLibraryCode/JuceHeader.h"
# define M_PI           3.14159265358979323846  /* pi */

class GaitAnalysis
{
public:
	GaitAnalysis();
	~GaitAnalysis();
	gaitParamInfo gaitParams;									// CONTAINS INFO ON ALL GAIT PARAMS
	SensorInfo sensorInfo;										// CONTAINS INFO ON SENSOR STATUS
	OSCReceiverUDP_Sensor sensors_OSCReceivers[3];				// ORDER: 0 - L // 1 - R // 2 - TRUNK
	int fs = 100;												// SENSOR SAMPLING RATE
	void compute(short currentGaitParam, bool isCalibrating);	// MAIN MP COMPUTATION FUNCTION
	void calc_CurrentMP(String mpName, bool isCalibrating);		// CALCULATE PRESENT MP FROM NAME
	
	// IMU Data Filters
	HFEN_HPF hfen_trunk;
	HFEN_HPF hfen_foot_L;
	HFEN_HPF hfen_foot_R;

	void setFc_imuSmooth(float freq)
	{
		for (int i = 0; i < sensorInfo.numSensorsMax; i++)
		{
			sensors_OSCReceivers[i].filterFc = freq;
			sensors_OSCReceivers[i].initializeFilters();
		}
	}

	void set_medianFilter_N(int n)
	{
		for (int i = 0; i < sensorInfo.numSensorsMax; i++)
		{
			sensors_OSCReceivers[i].set_medianFilter_N(n);
		}
	}

	// CHECK WHETHER ONLY REQUIRED SENSORS ARE ONLINE
	bool areRequiredSensorsOnline()
	{
		if
				(
				// CHECK TRUNK SENSOR
				gaitParams.gaitParam_ObjectArray[gaitParams.activeGaitParam].isSensorRequired[0]
				== sensorInfo.check_isAssignedAndOnline_TrunkSensor()
				&&
				// CHECK FOOT SENSORS
				gaitParams.gaitParam_ObjectArray[gaitParams.activeGaitParam].isSensorRequired[1]
				== sensorInfo.check_isAssignedAndOnline_FootSensors()
				)
			return true;
		return false;
	};

	// UPDATE ALL SENSOR STATUS - CHECK WHETHER ASSIGNED AND ONLINE
	void updateSensorStatus()
	{
		for (int i = 0; i < sensorInfo.numSensorsMax; i++)
		{
			sensorInfo.isOnline[i] = sensors_OSCReceivers[i].isSensorActive();
			if (sensorInfo.isOnline[i])
				sensorInfo.batteryPercent[i] = sensors_OSCReceivers[i].level_Battery;
		}
		sensorInfo.check_isAssignedAndOnline_FootSensors();
		sensorInfo.check_isAssignedAndOnline_TrunkSensor();
	};

	// SENSOR LOCATION INDICES
	short idx_Sensor_Trunk = 0;
	short idx_Sensor_Foot_L = 0;
	short idx_Sensor_Foot_R = 0;

	void setupReceivers();

	void boundValuesAndStore(String gaitParamName, float value)
	{
		// GET GAIT PARAM INDEX FROM NAME
		int gaitParamIndex = 0;
		for (int i = 0; i < gaitParams.numMovementParams; i++)
		{
			if (gaitParams.gaitParam_ObjectArray[i].name == gaitParamName)
			{
				gaitParamIndex = i; 
				break;
			}
		}
		
		// FETCH BOUNDS AND STORE PRESENT VALUE
		float minVal = gaitParams.gaitParam_ObjectArray[gaitParamIndex].minVal;
		float maxVal = gaitParams.gaitParam_ObjectArray[gaitParamIndex].maxVal;
		gaitParams.gaitParam_ObjectArray[gaitParamIndex].currentValue = jlimit(minVal, maxVal, value);
	};

	float repTime_Current = 0;
	int rms_Length = 10;													// RMS LENGTH IN SENSOR SAMPLES
	float rmsWindow_ML[100] = { 0.0 };										// RMS WINDOW - ML
	float rmsWindow_AP[100] = { 0.0 };										// RMS WINDOW - AP
	
	float accX_z1 = 0.0;	float accY_z1 = 0.0;	float accZ_z1 = 0.0;	// DELAYED ACC SAMPLES - XYZ
	float R_acc_est_z1[3] = { 0.0 };										// DELAYED ACC ESTIMATE - XYZ
	float R_acc_est[3] = { 0.0 };											// ACC ESTIMATE - XYZ
	float R_gyr_z1[3] = { 0.0 };											// DELAYED GYR SAMPLES - XYZ
	bool isCalibrated_Orientation = false;									// CHECK IF ORIENTATION CALIBRATED
	float wt_Gyr = 5;														// GYR WEIGHT FOR ORIENTATION
	float staticBalance_Div_Roll = 2;										// MLAT DIVISOR (HIGHER = NARROWER)
	float staticBalance_Div_Pitch = 2;										// APOS DIVISOR (HIGHER = NARROWER
	short staticBalance_ZoneMap_Current = 1;								// DETERMINES NUMBER OF ZONES
	float currVal_Z1 = 0;
	// ENCODES MP (= AP) VALUES FOR ZONES
	float staticBalance_ZoneMaps[3][6] = 
	{
		{0.005,0.38,0.73,0.85,0.93,0.98},									// 6 ZONES
		{0.005,0.73,0.73,0.85,0.93,0.98},									// 5 ZONES
		{0.005,0.85,0.85,0.85,0.93,0.98}									// 4 ZONES
	};

	// ROW 1 - ML AP (rest) AND ROW 2 - ML AP (extreme)
	float staticBalance_BoundsCoordinates[2][2] = { 0.0 }; 
	short staticBalance_calibrationMode = 0;				// 0 - CALIBRATE REST // 1 - CALIBRATE EXTREME
	float staticBalance_calibrationLength = 0;				// CALIBRATION LENGTH IN SAMPLES
	float staticBalance_BoundsCoordinates_z1[2] = { 0.0 };
	float staticBalance_CenterXY_ANTICIPATED[2] = { 0.0 };
	short staticBalance_FB_TYPE = 1;							// 1 = ZONE // 2 = 2D TASK // 3 = ADE
	short staticBalance_FB_DATA = 1;							// 1 = POSITION // 2 = VELOCITY

	// CALIBRATE REST OR EXTREME - STATIC BALANCE
	void staticBalance_calibrateCoordinates()
	{
		float ML_mean_OLD = staticBalance_BoundsCoordinates[staticBalance_calibrationMode][0];
		float AP_mean_OLD = staticBalance_BoundsCoordinates[staticBalance_calibrationMode][1];
		float ML_newVal = gaitParams.gaitParam_ObjectArray[0].currentValue;
		float AP_newVal = gaitParams.gaitParam_ObjectArray[1].currentValue;
		staticBalance_BoundsCoordinates[staticBalance_calibrationMode][0] = updateMean(ML_mean_OLD, staticBalance_calibrationLength, ML_newVal);
		staticBalance_BoundsCoordinates[staticBalance_calibrationMode][1] = updateMean(AP_mean_OLD, staticBalance_calibrationLength, AP_newVal);
		staticBalance_calibrationLength++;
	};

	// HEELSTRIKE - VARIABLES - MISC
	BiQuad LPF_Gyr_Turn_Smooth;
	float beatInterval = 0.5;
	float strideInterval = 1.0;
	bool isHalfTime = false;

	// HEELSTRIKE - VARIABLES - TIMING PERFORMANCE FEATURE 
	float HS_IntervalTolerance = 0;
	float timeElapsed = 0;
	float timeStamp_HS_Last = 0;
	float timeStamp_HS_Next_Exp = 0;
	float timeStamp_HS_Last_Exp = 0;
	bool isEarly = false;
	bool isLate = false;

	// BUFFER TO HOLD TRIGGER MP HIGH
	float triggerBuffer[20] = { 0.0 };

	// HS ACCELEROMETER
	// DETECTION THRESHOLDS
	float HS_thresh_pos = 1.84;
	float HS_thresh_neg = 0.04;
	
	// CORRECTION VARIABLES
	float HS_timeOut = 0.1;
	float HS_timeDone = 0;
	bool HS_isExpected_L = true;
	bool HS_isExpected_R = true;
	float norm_acc_z1 = 0.0;
	bool HS_accNorm_crossed_pos = false;
	bool HS_accNorm_crossed_neg = true;

	// THRESHOLD DETECTION AND MP FUNCTIONS
	void calcTimingPerformanceFeature_2(float *accBuf_L, float *accBuf_R, float *gyrBuf, bool isCalibrating);
	bool detectHS_acc(float *accBuf, bool isLeft);
	void drumTrigger(float *accBuf_L, float *accBuf_R, bool isCalibrating);
	bool isThreshCross(float input, float thresh, float input_z1, bool isUp)
	{
		bool output = false;
		if (isUp)
		{
			if (input > thresh && input_z1 <= thresh)
				output = true;
		}
		else
		{
			if (input < thresh && input_z1 >= thresh)
				output = true;
		}
		return output;
	};
	
	// TRUNK ORIENTATION CALCULATION
	void trunk_CalibrateRest(float *accBuf);						// CALIBRATE 
	void setSampleRate(int fs_ext);									// SET SAMPLE RATE IN SENSOR INFO
	void getOrientation_Fused(float *accBuf, float *gyrBuf);		// CALCULATE ORIENTATION - COMP FILTER
	void getProjection_ML_AP();										// GET PROJECTION ZONE
	
	void getRMS_Acc(float *accBuf);									// CALCULATE RMS ML / AP ACCELERATION

	void getJerkParams(float *accBuf);								// CALCULATE JERK
	
	void getSwayVelocity(float *gyrBuf);							// CALCULATE SWAY VELOCITY
	
	void getSitStandCueFeature();									// CALCULATION STS CUE FEATURE
	bool sitStand_feedbackMode = true;								// STS FEEDBACK MODE VARIABLE
	BiQuad smooth_Angle_AP;											// FILTER TO SMOOTH AP ANGLE FOR STS
	bool sitStand_isStanding = false;
	bool sitStand_isStabilized = true;
	float sitStand_Thresh_Stand = 15;
	float sitStand_Thresh_Sit = 10;
	float sitStand_Thresh_Current = 15;
	void sitStand_FlipState()
	{
		gaitParams.gaitParam_ObjectArray[13].currentValue = 
			gaitParams.gaitParam_ObjectArray[13].currentValue > 0.5 ? 0 : 1;
		sitStand_isStanding = sitStand_isStanding ? false : true;
		sitStand_isStabilized = true;
	};

	//Calibration
	float calibrationValues[30] = { 0.0 };
	float calibrationValues_Temp[30] = { 0.0 };
	short calibration_stepCount = 0;
	
	void calibrateMaximum(String paramName, bool isCalibrating)
	{
		short mp_Idx = 0;
		if (isCalibrating)
		{
			for (int i = 0; i < gaitParams.numMovementParams; i++)
			{
				if (paramName == gaitParams.gaitParam_ObjectArray[i].name)
					mp_Idx = i;
			}
			if (abs(gaitParams.gaitParam_ObjectArray[mp_Idx].currentValue) > calibrationValues_Temp[mp_Idx])
				calibrationValues_Temp[mp_Idx] = gaitParams.gaitParam_ObjectArray[mp_Idx].currentValue;
		}
	};

	void saveCalibration(short activeGaitParam)
	{
		calibrationValues[activeGaitParam] = calibrationValues_Temp[activeGaitParam];
		isParamCalibrated[activeGaitParam] = true;
	}

	void discardCalibration(short activeGaitParam)
	{
		calibrationValues_Temp[activeGaitParam] = 0.0;
	}
	
	// HS CALIBRATION - REPLACE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	void updateHS_Calibration(float strideDuration_Latest);
	float hs_Intervals[5] = { 0.0 };
	void flush_HSArray()
	{
		for (int i = 0; i < 5; i++)
		{
			updateHS_Calibration(0);
		}
	}
	float strideDur_Mean = 0;
	float strideDur_COV = 0;
	bool isParamCalibrated[30] = { false };

	// UPDATE A RUNNING MEAN WITH A NEW VALUE
	double updateMean(double oldMean, int count, float newVal)
	{
		return (oldMean*count + newVal) / (count + 1);
	};

	// CALCULATE STANDARD DEVIATION OF AN ARRAY
	float calcArraySTD(float *arr, float *mean, int count)
	{
		float standDev = 0.0; float variance = 0.0;
		for (int i = 0; i < count; i++)
		{
			variance += pow(arr[i] - *mean, 2);
		}
		variance = variance / float(count - 1);

		standDev = sqrt(variance);

		return standDev;
	};
};