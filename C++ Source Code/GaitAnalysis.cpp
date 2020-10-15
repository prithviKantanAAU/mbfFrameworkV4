#include "GaitAnalysis.h"

// CONSTRUCTOR - INITIALIZE FILTERS
GaitAnalysis::GaitAnalysis()
{
	//INITIALIZE FILTERS // GYR TURN SMOOTHING // STS AP ANGLE SMOOTHING
	LPF_Gyr_Turn_Smooth.flushDelays();
	LPF_Gyr_Turn_Smooth.calculateLPFCoeffs(1, 0.7, fs);
	smooth_Angle_AP.flushDelays();
	smooth_Angle_AP.calculateLPFCoeffs(3, 0.7, fs);
}

// DESTRUCTOR - EMPTY
GaitAnalysis::~GaitAnalysis()
{
}

// SETUP OSC UDP RECEIVERS - PORT, LISTENER, SAMPLE RATE
void GaitAnalysis::setupReceivers()
{
	for (int i = 0; i < sensorInfo.numSensorsMax; i++)
	{
		sensors_OSCReceivers[i].setupPortAndListener(sensorInfo.UDP_Ports[i], sensorInfo.sensors_OSCAddress[i]);
		sensors_OSCReceivers[i].setSampleRate(sensorInfo.IMU_SampleRate);
	}
	setSampleRate(sensorInfo.IMU_SampleRate);
}

// SETUP SAMPLE RATE OF HFEN FILTERS (HPF)
void GaitAnalysis::setSampleRate(int fs_ext)
{
	fs = fs_ext;
	hfen_trunk.setSampleRate(fs_ext);
	hfen_foot_L.setSampleRate(fs_ext);
	hfen_foot_R.setSampleRate(fs_ext);
}

// PERIODIC CALL - MAIN FUNCTION - CHECK IF SENSORS ONLINE, CALL REQUIRED MP CALCULATION FUNCTION
void GaitAnalysis::compute(short currentGaitParam, bool isCalibrating)
{
	// ONLY PROCEED IF REQD SENSORS ON
	if (!areRequiredSensorsOnline()) return;					
	
	// ASSIGN SENSOR LOCATIONS
	for (int j = 0; j < sensorInfo.numSensorsMax; j++)
	{
		switch (sensorInfo.bodyLocation[j])
		{
		case 1:
			idx_Sensor_Trunk = j;
			break;
		case 2:
			idx_Sensor_Foot_L = j;
			break;
		case 3:
			idx_Sensor_Foot_R = j;
			break;
		}	
	} 

	// NAME - WISE MP SEARCH AND HANDLE
	int mp_Present_Idx = gaitParams.activeGaitParam;
	String mp_Name_Present = gaitParams.gaitParam_ObjectArray[mp_Present_Idx].name;
	calc_CurrentMP(mp_Name_Present, isCalibrating);
	repTime_Current += 1.0 / (float)fs;
}

// CALCULATE SELECTED MP BASED ON SELECTED NAME
void GaitAnalysis::calc_CurrentMP(String mpName, bool isCalibrating)
{
	// 0
	if (mpName == "Inclination (+-) - ML")
	{
		getOrientation_Fused(sensors_OSCReceivers[idx_Sensor_Trunk].acc_Buf,
							 sensors_OSCReceivers[idx_Sensor_Trunk].gyr_Buf);
		calibrateMaximum(mpName, isCalibrating);
		return;
	}
	// 1
	if (mpName == "Inclination (+-) - AP")
	{
		getOrientation_Fused(sensors_OSCReceivers[idx_Sensor_Trunk].acc_Buf,
							 sensors_OSCReceivers[idx_Sensor_Trunk].gyr_Buf);
		calibrateMaximum(mpName, isCalibrating);
		return;
	}
	// 2
	if (mpName == "Trunk Projection Zone")
	{
		getOrientation_Fused(sensors_OSCReceivers[idx_Sensor_Trunk].acc_Buf,
							 sensors_OSCReceivers[idx_Sensor_Trunk].gyr_Buf);
		getProjection_ML_AP();
		return;
	}
	// 3
	if (mpName == "RMS Acc ML")
	{
		getRMS_Acc(sensors_OSCReceivers[idx_Sensor_Trunk].acc_Buf);
		calibrateMaximum(mpName, isCalibrating);
		return;
	}
	// 4
	if (mpName == "RMS Acc AP")
	{
		getRMS_Acc(sensors_OSCReceivers[idx_Sensor_Trunk].acc_Buf);
		calibrateMaximum(mpName, isCalibrating);
		return;
	}
	// 5
	if (mpName == "Scalar Jerk")
	{
		getJerkParams(sensors_OSCReceivers[idx_Sensor_Trunk].acc_Buf);
		getOrientation_Fused(sensors_OSCReceivers[idx_Sensor_Trunk].acc_Buf,
			sensors_OSCReceivers[idx_Sensor_Trunk].gyr_Buf);
		getSitStandCueFeature();
		calibrateMaximum(mpName, isCalibrating);
		return;
	}
	// 6
	if (mpName == "Jerk - X")
	{
		getJerkParams(sensors_OSCReceivers[idx_Sensor_Trunk].acc_Buf);
		getOrientation_Fused(sensors_OSCReceivers[idx_Sensor_Trunk].acc_Buf,
			sensors_OSCReceivers[idx_Sensor_Trunk].gyr_Buf);
		getSitStandCueFeature();
		calibrateMaximum(mpName, isCalibrating);
		return;
	}
	// 7
	if (mpName == "Jerk - Y")
	{
		getJerkParams(sensors_OSCReceivers[idx_Sensor_Trunk].acc_Buf);
		getOrientation_Fused(sensors_OSCReceivers[idx_Sensor_Trunk].acc_Buf,
			sensors_OSCReceivers[idx_Sensor_Trunk].gyr_Buf);
		getSitStandCueFeature();
		calibrateMaximum(mpName, isCalibrating);
		return;
	}
	// 8
	if (mpName == "Jerk - Z")
	{
		getJerkParams(sensors_OSCReceivers[idx_Sensor_Trunk].acc_Buf);
		getOrientation_Fused(sensors_OSCReceivers[idx_Sensor_Trunk].acc_Buf,
			sensors_OSCReceivers[idx_Sensor_Trunk].gyr_Buf);
		getSitStandCueFeature();
		calibrateMaximum(mpName, isCalibrating);
		return;
	}
	// 9
	if (mpName == "Sway Vel - ML")
	{
		getSwayVelocity(sensors_OSCReceivers[idx_Sensor_Trunk].gyr_Buf);
		calibrateMaximum(mpName, isCalibrating);
		return;
	}
	// 10
	if (mpName == "Sway Vel - AP")
	{
		getSwayVelocity(sensors_OSCReceivers[idx_Sensor_Trunk].gyr_Buf);
		calibrateMaximum(mpName, isCalibrating);
		return;
	}
	// 11
	if (mpName == "HS Timing")
	{
		calcTimingPerformanceFeature_2(sensors_OSCReceivers[idx_Sensor_Foot_L].acc_Buf,
			sensors_OSCReceivers[idx_Sensor_Foot_R].acc_Buf,
			sensors_OSCReceivers[idx_Sensor_Foot_L].gyr_Buf,
			isCalibrating);
		return;
	}
	// 12
	if (mpName == "HS Trigger")
	{
		drumTrigger(sensors_OSCReceivers[idx_Sensor_Foot_L].acc_Buf,
		sensors_OSCReceivers[idx_Sensor_Foot_R].acc_Buf, isCalibrating);
		return;
	}
	// 13
	if (mpName == "STS Cue")
	{
		getOrientation_Fused(sensors_OSCReceivers[idx_Sensor_Trunk].acc_Buf,
		sensors_OSCReceivers[idx_Sensor_Trunk].gyr_Buf);
		getSitStandCueFeature();
		return;
	}
}

// FUNCTION TO CALIBRATE TRUNK REST WHEN MEASURING ORIENTATION (AUTOMATIC AND PERIODIC)
void GaitAnalysis::trunk_CalibrateRest(float *accBuf)
{
	float magnitude = sqrt(pow(accBuf[0],2) + pow(accBuf[1], 2) + pow(accBuf[2], 2));
	
	R_acc_est[0] = accBuf[0] / magnitude;
	R_acc_est[1] = accBuf[2] / magnitude;
	R_acc_est[2] = accBuf[1] / magnitude;
}

// CALCULATE ML AND AP ORIENTATION
void GaitAnalysis::getOrientation_Fused(float *accBuf, float *gyrBuf)
{
	//Adjust Coordinates and Units
	float R_acc_X = accBuf[0];	float gyrReading_X = gyrBuf[0] * M_PI / 180;
	float R_acc_Y = accBuf[2];	float gyrReading_Y = gyrBuf[2] * M_PI / 180;
	float R_acc_Z = accBuf[1];	float gyrReading_Z = gyrBuf[1] * M_PI / 180;
	float R_gyro_Inter[3] = { 0.0 };
	float R_acc_Vector[3] = { R_acc_X,R_acc_Y,R_acc_Z };

	if (!isCalibrated_Orientation)
	{
		trunk_CalibrateRest(accBuf);
		isCalibrated_Orientation = true;
	}

	//Normalize
	float R_acc = sqrt(R_acc_X*R_acc_X + R_acc_Y * R_acc_Y + R_acc_Z * R_acc_Z);
	R_acc_X /= R_acc;	R_acc_Y /= R_acc;	R_acc_Z /= R_acc;

	//Get Angles
	R_acc_est[2] = fmax(0.0001, R_acc_est[2]);
	float angle_xz_z1 = atan2(R_acc_est[0], R_acc_est[2]);
	float angle_yz_z1 = atan2(R_acc_est[1], R_acc_est[2]);

	//Average Gyr Reading and store buffer
	float gyrReading_Y_avg = 0.5*(gyrReading_Y + R_gyr_z1[1]);
	float gyrReading_X_avg = 0.5*(gyrReading_X + R_gyr_z1[0]);
	R_gyr_z1[0] = gyrReading_X;
	R_gyr_z1[1] = gyrReading_Y;
	R_gyr_z1[2] = gyrReading_Z;

	float angle_xz = angle_xz_z1 + gyrReading_Y_avg * 1 / fs;
	float angle_yz = angle_yz_z1 + gyrReading_X_avg * 1 / fs;
	boundValuesAndStore("Inclination (+-) - ML", isnan(angle_xz) ? 0 : angle_xz * 180 / M_PI);
	boundValuesAndStore("Inclination (+-) - AP", isnan(angle_yz) ? 0 : angle_yz * 180 / M_PI);
	
	//INCREMENT REPETITIONS - ONLY FOR INCLINATION MP SELECTED
	String mpName = gaitParams.gaitParam_ObjectArray[gaitParams.activeGaitParam].name;
	float currVal = gaitParams.gaitParam_ObjectArray[gaitParams.activeGaitParam].currentValue;
	float targetMin = gaitParams.gaitParam_ObjectArray[gaitParams.activeGaitParam].target_MIN;
	float targetMax = gaitParams.gaitParam_ObjectArray[gaitParams.activeGaitParam].target_MAX;

	if (mpName == "Inclination (+-) - ML" || mpName == "Inclination (+-) - AP")
	{
		if (currVal_Z1 < targetMax && currVal >= targetMax)
			gaitParams.incrementNumReps(&repTime_Current);
		if (currVal_Z1 > targetMin && currVal <= targetMin)
			gaitParams.incrementNumReps(&repTime_Current);
	}
	currVal_Z1 = currVal;


	//Calculate R_Gyro_Inter
	R_gyro_Inter[0] = sin(angle_xz) / sqrt(1 + pow(cos(angle_xz), 2)*pow(tan(angle_yz), 2));
	R_gyro_Inter[1] = sin(angle_yz) / sqrt(1 + pow(cos(angle_yz), 2)*pow(tan(angle_xz), 2));
	R_gyro_Inter[2] = sqrt(1 - R_gyro_Inter[0] * R_gyro_Inter[0] - R_gyro_Inter[1] * R_gyro_Inter[1]);

	for (int i = 0; i < 3; i++)
	{
		R_acc_est_z1[i] = R_acc_est[i];
		R_acc_est[i] = (R_acc_Vector[i] + wt_Gyr * R_gyro_Inter[i]) / (1 + wt_Gyr);
	}
}

// CALCULATE ORIENTATION PROJECTION ZONE
void GaitAnalysis::getProjection_ML_AP()
{
	float roll_deg = (gaitParams.gaitParam_ObjectArray[0].currentValue);
	float pitch_deg =(gaitParams.gaitParam_ObjectArray[1].currentValue);

	float roll_deg_NORM = roll_deg + staticBalance_BoundsCoordinates[0][0];
	float pitch_deg_NORM = pitch_deg + staticBalance_BoundsCoordinates[0][1];

	float area = pow(staticBalance_Div_Roll, 2) + pow(staticBalance_Div_Pitch, 2);
	float maxVel_DegPerSec = 36;
	float sig_x1 = 0;
	float sig_x2 = 0;
	float sig_y1 = 0;
	float sig_y2 = 0;
	
	float fbVar_X = 0;
	float fbVar_Y = 0;

	// FIND AND STORE ZONE
	if (staticBalance_FB_TYPE == 1)
	{
		if (roll_deg_NORM < -1 * staticBalance_Div_Roll)
		{
			boundValuesAndStore("Trunk Projection Zone", 6);
			return;
		}

		else if (roll_deg_NORM > 1 * staticBalance_Div_Roll)
		{
			boundValuesAndStore("Trunk Projection Zone", 5);
			return;
		}

		else if (pow(roll_deg_NORM, 2) + pow(pitch_deg_NORM, 2) <= area)
		{
			boundValuesAndStore("Trunk Projection Zone", 1);
			return;
		}

		else if (pow(((pitch_deg_NORM - 0.5) / 2.25), 2) + pow((roll_deg_NORM / 1.5), 2) <= area)
		{
			boundValuesAndStore("Trunk Projection Zone", 2);
			return;
		}

		else if (pow(((pitch_deg_NORM - 0.5) / 3), 2) + pow((roll_deg_NORM / 2), 2) <= area)
		{
			boundValuesAndStore("Trunk Projection Zone", 3);
			return;
		}

		else
		{
			boundValuesAndStore("Trunk Projection Zone", 4);
			return;
		}
	}

	// STORE 2D AP - TASK INFO ONLY
	if (staticBalance_FB_TYPE == 2)
	{
		// POSITION INFO
		if (staticBalance_FB_DATA == 1)
		{
			fbVar_X = (staticBalance_BoundsCoordinates[0][0] - (-20)) / 40.0;
			fbVar_Y = 1 - (staticBalance_BoundsCoordinates[0][1] - (-20)) / 40.0;
			gaitParams.apVal_DYN_TaskDependent[0] = fbVar_X;
			gaitParams.apVal_DYN_TaskDependent[1] = fbVar_Y;
		}

		// VELOCITY INFO
		if (staticBalance_FB_DATA == 2)
		{
			// FIND DEGREE DISPLACEMENT IN LAST SAMPLING INTERVAL
			fbVar_X = staticBalance_BoundsCoordinates[0][0] - staticBalance_BoundsCoordinates_z1[0];
			fbVar_Y = staticBalance_BoundsCoordinates[0][1] - staticBalance_BoundsCoordinates_z1[1];

			// NORMALIZE
			maxVel_DegPerSec /= (float)fs;
			fbVar_X = (fbVar_X - (-maxVel_DegPerSec)) / 2 * maxVel_DegPerSec;
			fbVar_Y = 1 - (fbVar_Y - (-maxVel_DegPerSec)) / 2 * maxVel_DegPerSec;

			// STORE AND SHUFFLE DELAYS
			gaitParams.apVal_DYN_TaskDependent[0] = fbVar_X;
			gaitParams.apVal_DYN_TaskDependent[1] = fbVar_Y;
			staticBalance_BoundsCoordinates_z1[0] = staticBalance_BoundsCoordinates[0][0];
			staticBalance_BoundsCoordinates_z1[1] = staticBalance_BoundsCoordinates[0][1];
		}
	}

	// STORE 2D AP - ANTICIPATED DISTANCE ERROR
	if (staticBalance_FB_TYPE == 3)
	{
		roll_deg *= -1;
		pitch_deg *= -1;
		sig_x1 = staticBalance_CenterXY_ANTICIPATED[0] - staticBalance_Div_Roll;
		sig_x2 = staticBalance_CenterXY_ANTICIPATED[0] + staticBalance_Div_Roll;

		sig_y1 = staticBalance_CenterXY_ANTICIPATED[1] - staticBalance_Div_Pitch;
		sig_y2 = staticBalance_CenterXY_ANTICIPATED[1] + staticBalance_Div_Pitch;


		fbVar_X = 1 - 0.5* (exp(roll_deg - sig_x1) / (exp(roll_deg - sig_x1) + 1) +
						exp(roll_deg - sig_x2) / (exp(roll_deg - sig_x2) + 1));

		fbVar_X = abs(fbVar_X - 0.5) < 0.15 ? 0.5 : fbVar_X;

		fbVar_Y = 1 - 0.5* (exp(pitch_deg - sig_y1) / (exp(pitch_deg - sig_y1) + 1) +
						exp(pitch_deg - sig_y2) / (exp(pitch_deg - sig_y2) + 1));

		fbVar_Y = abs(fbVar_Y - 0.5) < 0.15 ? 0.5 : fbVar_Y;

		gaitParams.apVal_DYN_TaskDependent[0] = fbVar_X;
		gaitParams.apVal_DYN_TaskDependent[1] = fbVar_Y;
	}
}

// CALCULATE STS CUE FEATURE
void GaitAnalysis::getSitStandCueFeature()
{
	float angle_AP_signed = smooth_Angle_AP.doBiQuad(gaitParams.gaitParam_ObjectArray[1].currentValue, sitStand_Thresh_Current);
	if (smooth_Angle_AP.isThreshCrossing_Pos && sitStand_isStabilized)
	{
		sitStand_isStanding = !sitStand_isStanding;
		sitStand_isStabilized = false;
	}
	if (smooth_Angle_AP.isThreshCrossing_Neg)
	{
		sitStand_isStabilized = true;
		sitStand_Thresh_Current = sitStand_isStanding ? sitStand_Thresh_Sit : sitStand_Thresh_Stand;
		if (!sitStand_isStanding) gaitParams.incrementNumReps(&repTime_Current);	// INC REPS
	}
	bool feedbackCondition = sitStand_feedbackMode ? sitStand_isStabilized : sitStand_isStanding;
	boundValuesAndStore("STS Cue", feedbackCondition ? 0 : 1);
}

// CALCULATE RMS ML AND AP ACCELERATION
void GaitAnalysis::getRMS_Acc(float *accBuf)
{
	float accX = accBuf[0];
	float accZ = accBuf[2];
	
	//RAW

	accX = hfen_trunk.applyHFEN_PreProcess(accX,'x');
	accZ = hfen_trunk.applyHFEN_PreProcess(accZ,'z');

	//HPF APPLIED

	float horizComponent_ML = accX;
	float horizComponent_AP = accZ;

	float rmsSum_ML = 0;
	float rmsSum_AP = 0;

	rmsWindow_AP[1] = rmsWindow_AP[0]; 	rmsWindow_ML[1] = rmsWindow_ML[0];
	rmsWindow_ML[0] = accX;				rmsWindow_AP[0] = accZ;

	rmsSum_ML += pow(rmsWindow_ML[0], 2);
	rmsSum_AP += pow(rmsWindow_AP[0], 2);

	for (int i = 1; i < rms_Length - 1; i++)
	{
		rmsWindow_ML[i + 1] = rmsWindow_ML[i];
		rmsWindow_AP[i + 1] = rmsWindow_AP[i];
		rmsSum_ML += pow(rmsWindow_ML[i], 2);
		rmsSum_AP += pow(rmsWindow_AP[i], 2);
	}

	boundValuesAndStore("RMS Acc ML", rmsSum_ML / rms_Length * 100);
	boundValuesAndStore("RMS Acc AP", rmsSum_AP / rms_Length * 100);
}

// CALCULATE JERK PARAMS
void GaitAnalysis::getJerkParams(float *accBuf)
{
	float accX = accBuf[0];
	float accY = accBuf[1];
	float accZ = accBuf[2];

	//Apply HFEN
	accX = hfen_trunk.applyHFEN_PreProcess(accX,'x');
	accY = hfen_trunk.applyHFEN_PreProcess(accY,'y');
	accZ = hfen_trunk.applyHFEN_PreProcess(accZ,'z');

	float jerkX = fabs(accX - accX_z1);
	float jerkY = fabs(accY - accY_z1);
	float jerkZ = fabs(accZ - accZ_z1);

	float scalarJerk = sqrt(jerkX*jerkX + jerkY * jerkY + jerkZ * jerkZ);	//Euclidean Norm

	boundValuesAndStore("Scalar Jerk", scalarJerk * 100);
	boundValuesAndStore("Jerk - X", jerkX * 100);
	boundValuesAndStore("Jerk - Y", jerkY * 100);
	boundValuesAndStore("Jerk - Z", jerkZ * 100);

	accX_z1 = accX;		accY_z1 = accY;		accZ_z1 = accZ;
}

// CALCULATE SWAY VELOCITY
void GaitAnalysis::getSwayVelocity(float *gyrBuf)
{
	boundValuesAndStore("Sway Vel - ML", fabs(gyrBuf[0]));
	boundValuesAndStore("Sway Vel - AP", fabs(gyrBuf[2]));
}

// DETECT HS INSTANTS FROM ACC IMPACT
bool GaitAnalysis::detectHS_acc(float *accBuf, bool isLeft)
{
	float accX_filt = isLeft ? hfen_foot_L.applyHFEN_PreProcess(accBuf[0], 'x')
		: hfen_foot_R.applyHFEN_PreProcess(accBuf[0], 'x');
	float accY_filt = isLeft ? hfen_foot_L.applyHFEN_PreProcess(accBuf[1], 'y')
		: hfen_foot_R.applyHFEN_PreProcess(accBuf[1], 'y');
	float accZ_filt = isLeft ? hfen_foot_L.applyHFEN_PreProcess(accBuf[2], 'z')
		: hfen_foot_R.applyHFEN_PreProcess(accBuf[2], 'z');
	float norm = pow(accX_filt, 2) + pow(accY_filt, 2) + pow(accZ_filt, 2);
	bool stepDetected = false;
	HS_timeOut = 0.8 * beatInterval;

	if (HS_timeDone > HS_timeOut)
	{
		if (isThreshCross(norm, HS_thresh_pos, norm_acc_z1, true))
		{
			if (HS_isExpected_L)
			{
				if (isLeft)
				{
					stepDetected = true;
					HS_isExpected_L = false;
					HS_isExpected_R = true;
					HS_timeDone = 0;
					gaitParams.incrementNumReps(&repTime_Current);
				}
			}

			if (HS_isExpected_R)
			{
				if (!isLeft)
				{
					stepDetected = true;
					HS_isExpected_L = true;
					HS_isExpected_R = false;
					HS_timeDone = 0;
					gaitParams.incrementNumReps(&repTime_Current);
				}
			}
		}
	}
	HS_timeDone += 1.0 / (float)fs;
	return stepDetected;
}

// CALCULATE HS TIMING PERFORMANCE FEATURE
void GaitAnalysis::calcTimingPerformanceFeature_2(float *accBuf_L, float *accBuf_R, float *gyrBuf, bool isCalibrating)
{
	float featureValue = 0;
	float stepDuration_Exp = isHalfTime ? 2 * beatInterval : beatInterval;
	float timeTolerance = stepDuration_Exp * HS_IntervalTolerance;
	bool isTurning = abs(LPF_Gyr_Turn_Smooth.doBiQuad(gyrBuf[1], 0.001)) > 60 ? true : false;

	bool toTrigger = detectHS_acc(accBuf_L, true) || detectHS_acc(accBuf_R, false);								//MODIFY
	if (toTrigger)
	{
		updateHS_Calibration(timeElapsed - timeStamp_HS_Last);
		timeStamp_HS_Last = timeElapsed;

		if (timeStamp_HS_Last < (timeStamp_HS_Next_Exp - timeTolerance))// Check If Early
			isEarly = true;

		isLate = false;													// Reset isLate

		if (isEarly)
			timeStamp_HS_Last_Exp = timeStamp_HS_Next_Exp;				// Save Last Expected Time

		timeStamp_HS_Next_Exp = timeStamp_HS_Last + stepDuration_Exp; // Update Next Expected Time
	}

	float time_toNext_HS = timeStamp_HS_Next_Exp - timeElapsed;			// Time to Next Expected HS
	if (timeElapsed > timeStamp_HS_Last_Exp)
		isEarly = false;

	if (time_toNext_HS < -1 * timeTolerance)
	{
		isLate = true;		isEarly = false;
	}

	if (!isTurning)
		if (isEarly || isLate)		featureValue = 1;
	boundValuesAndStore("HS Timing", featureValue);
};

// CALCULATE HS TRIGGER FEATURE
void GaitAnalysis::drumTrigger(float *accBuf_L, float *accBuf_R, bool isCalibrating)
{
	bool toTrigger_L = detectHS_acc(accBuf_L, true);
	bool toTrigger_R = detectHS_acc(accBuf_R, false);
	if (toTrigger_L || toTrigger_R)
	{
		updateHS_Calibration(timeElapsed - timeStamp_HS_Last);
		timeStamp_HS_Last = timeElapsed;
	}
	float featVal = 0;
	if (toTrigger_L)
		featVal = 0.7;
	if (toTrigger_R)
		featVal = 0.8;
	float mapVal = 0.5;

	for (int i = 4; i > 0; i--)
	{
		triggerBuffer[i] = triggerBuffer[i - 1];
	}
	triggerBuffer[0] = featVal;
	for (int i = 0; i < 4; i++)
	{
		if (triggerBuffer[i] > 0.5)
			mapVal = triggerBuffer[i];
	}

	boundValuesAndStore("HS Trigger", mapVal);
}

// REWRITE - HS TIMING CALIBRATION
void GaitAnalysis::updateHS_Calibration(float strideDuration_Latest)
{
	calibration_stepCount++;
	hs_Intervals[(calibration_stepCount - 1) % 2] = strideDuration_Latest;
	strideDur_Mean = 0;
	for (int i = 0; i < 2; i++)
	{
		strideDur_Mean += hs_Intervals[i];
	}
	strideDur_Mean /= 2.0;
	strideInterval = strideDur_Mean / 2.0;
	strideDur_COV = calcArraySTD(hs_Intervals, &strideDur_Mean, 2);
	calibrationValues_Temp[11] = strideDur_Mean;
	calibrationValues_Temp[12] = strideDur_Mean;
}

