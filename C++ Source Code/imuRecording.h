#pragma once
#include <ctime>
#include "../JuceLibraryCode/JuceHeader.h"
#include "windows.h"

class imuRecording
{
public:
	imuRecording() {};
	~imuRecording() {};

	Time currentDateTime;										// Current Date and Time
	FILE *sensorRecording;										// File Obj - Movement Parameter + Sensor Log
	FILE *rawSensorData[3];										// File Obj - Raw IMU Data
	FILE *repetitionData;										// File Obj - Rep Data
	File forRootDirectory;										// File Obj - To get Solution Root Dir						
	String filePath_SensorRec = "";								// File Path - MP + Sensor Log
	String filePath_SensorRec_RAW = "";							// File Path - Raw IMU Data
	String filePath_Reps = "";
	bool isRecording_Sensor = false;							// Flag - is sensor logging enabled?
	float timeElapsed_REC = 0;									// Time Elapsed - Sensor Logging

	std::string format_GP = "%f,%f,%f,%f,%s,%f,%s,%f,%f,%s,%f,%f,%f,\n";	// Format - Full Log
	std::string format_RAW = "%f,%f,%f,%f,%f,%f,%f,\n";						// Format - RAW IMU Log
	std::string format_REPS = "%f,%f,\n";									// Format - REPS

	float currentRow_FullLog_FLOAT[20];
	String currentRow_FullLog_STRING[10];
	float currentRow_RawLog_FLOAT[7];
	float currentRow_Reps_FLOAT[5];

	void writeToLog_Line_FullLog()
	{
		fprintf(
			sensorRecording,
			format_GP.c_str(),
			currentRow_FullLog_FLOAT[0],
			currentRow_FullLog_FLOAT[1],
			currentRow_FullLog_FLOAT[2],
			currentRow_FullLog_FLOAT[3],
			currentRow_FullLog_STRING[0],
			currentRow_FullLog_FLOAT[4],
			currentRow_FullLog_STRING[1],
			currentRow_FullLog_FLOAT[5],
			currentRow_FullLog_FLOAT[6],
			currentRow_FullLog_STRING[2],
			currentRow_FullLog_FLOAT[7],
			currentRow_FullLog_FLOAT[8],
			currentRow_FullLog_FLOAT[9]
			   );
	}

	void writeToLog_Line_RAW(int logNum)
	{
		fprintf(
			rawSensorData[logNum], 
			format_RAW.c_str(),
			currentRow_RawLog_FLOAT[0],
			currentRow_RawLog_FLOAT[1],
			currentRow_RawLog_FLOAT[2],
			currentRow_RawLog_FLOAT[3],
			currentRow_RawLog_FLOAT[4],
			currentRow_RawLog_FLOAT[5],
			currentRow_RawLog_FLOAT[6]
			   );
	}

	void writeToLog_Line_Reps()
	{
		fprintf(
			repetitionData,
			format_REPS.c_str(),
			currentRow_Reps_FLOAT[0],
			currentRow_Reps_FLOAT[1]
		);
	}

	// Returns Current Time
	String getCurrentTime()
	{
		time_t rawtime;
		struct tm * timeinfo;
		char buffer[80];

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(buffer, sizeof(buffer), "%d-%m-%Y %H-%M-%S", timeinfo);
		std::string str(buffer);

		return String(str);
	}
};

