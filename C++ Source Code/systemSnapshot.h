#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class systemSnapshot
{
public:
	systemSnapshot() {};
	~systemSnapshot() {};

	FILE *snapshot;												// File Obj - Snapshot
	File forRootDirectory;										// File Obj - To get Solution Root Dir						
	String filePath_Snapshot = "";								// File Path - MP + Sensor Log
	
	// General
	int numLines = 1;
	String exerciseMode = "";
	String MP = "";
	float order_MapFunc = 1;
	int quantLevels = 0;
	float target_MIN = 0;
	float target_MAX = 0;
	int polarity = 1;
	float tempo = 120;
	String rhythm = "";
	float trackGains[8];
	short trackMutes[8];
	short variants[8];
	
	// Static Balance
	float SB_C_X = 0;
	float SB_C_Y = 0;
	float SB_Wid_X = 0;
	float SB_Wid_Y = 0;

	// Dynamic Balance
	String DB_Shape = "";
	short DB_Mirrored = 0;
	short DB_Period = 1;
	float DB_Radius = 10;

	// STS Cue
	float STS_angle_preSit = 0;
	float STS_angle_preStand = 0;

	// Gait
	String Gait_TimingMode = "";
	float Gait_ErrorTolerance = 0;
	float Gait_AccThresh = 0;

	void writeFile()
	{
		fprintf(snapshot, "%s,%s,\n", "Exercise Mode", exerciseMode);
		fprintf(snapshot, "%s,%s,\n", "Movement Param", MP);
		fprintf(snapshot, "%s,%f,\n", "Map Function Order", order_MapFunc);
		fprintf(snapshot, "%s,%s,\n", "Map Quant Levels", String(quantLevels));
		fprintf(snapshot, "%s,%f,\n", "Target Min Bound", target_MIN);
		fprintf(snapshot, "%s,%f,\n", "Target Max Bound", target_MAX);
		fprintf(snapshot, "%s,%s,\n", "Polarity", String(polarity));
		fprintf(snapshot, "%s,%f,\n", "Tempo", tempo);
		fprintf(snapshot, "%s,%s,\n", "Rhythm", rhythm);
		fprintf(snapshot, "%s,%s,%s,%s,%s,%s,%s,%s,%s,\n", "Track Gains", 
			String(trackGains[0]), String(trackGains[1]), String(trackGains[2]), String(trackGains[3]), 
			String(trackGains[4]), String(trackGains[5]), String(trackGains[6]), String(trackGains[7]));
		fprintf(snapshot, "%s,%s,%s,%s,%s,%s,%s,%s,%s,\n", "Track Mutes",
			String(trackMutes[0]), String(trackMutes[1]), String(trackMutes[2]), String(trackMutes[3]),
			String(trackMutes[4]), String(trackMutes[5]), String(trackMutes[6]), String(trackMutes[7]));
		fprintf(snapshot, "%s,%s,%s,%s,%s,%s,%s,%s,%s,\n", "Track Variants",
			String(variants[0]), String(variants[1]), String(variants[2]), String(variants[3]),
			String(variants[4]), String(variants[5]), String(variants[6]), String(variants[7]));
		
		if (exerciseMode == "Static Upright")
		{
			fprintf(snapshot, "%s,%f,\n", "Center X", SB_C_X);
			fprintf(snapshot, "%s,%f,\n", "Center Y", SB_C_Y);
			fprintf(snapshot, "%s,%f,\n", "Width X", SB_Wid_X);
			fprintf(snapshot, "%s,%f,\n", "Width Y", SB_Wid_Y);
		}

		if (exerciseMode == "Dynamic Trunk")
		{
			fprintf(snapshot, "%s,%s,\n", "Trajectory Shape", DB_Shape);
			fprintf(snapshot, "%s,%f,\n", "Trajectory Mirrored", DB_Mirrored);
			fprintf(snapshot, "%s,%s,\n", "Trajectory Period (Bars)", String(DB_Period));
			fprintf(snapshot, "%s,%f,\n", "Trajectory Radius", DB_Radius);
		}

		if (exerciseMode == "STS Angle Cue")
		{
			fprintf(snapshot, "%s,%f,\n", "Pre Sit Angle", STS_angle_preSit);
			fprintf(snapshot, "%s,%f,\n", "Pre Stand Angle", STS_angle_preStand);
		}

		if (exerciseMode == "Gait")
		{
			fprintf(snapshot, "%s,%s,\n", "Timing Mode", Gait_TimingMode);
			fprintf(snapshot, "%s,%f,\n", "Error Tolerance", Gait_ErrorTolerance);
			fprintf(snapshot, "%s,%f,\n", "Acc Thresh", Gait_AccThresh);
		}
		
		fclose(snapshot);
	}

	void readSnapshot(String path)
	{
		auto settingsFile = File(path);

		if (!settingsFile.existsAsFile())
			return;  // file doesn't exist

		juce::FileInputStream inputStream(settingsFile); // [2]

		if (!inputStream.openedOk())
			return;  // failed to open

		while (!inputStream.isExhausted())
		{
			auto line = inputStream.readNextLine();
			auto line_Rem = line;

			String line_header = line.upToFirstOccurrenceOf(",", false, true);
			line_Rem = line_Rem.fromFirstOccurrenceOf(",", false, true);

			// 1
			if (line_header == "Exercise Mode")
			{
				exerciseMode = line_Rem.upToFirstOccurrenceOf(",", false, true);
			}

			// 2
			if (line_header == "Movement Param")
			{
				MP = line_Rem.upToFirstOccurrenceOf(",", false, true);
			}

			// 3
			if (line_header == "Map Function Order")
			{
				order_MapFunc = line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
			}

			// 4
			if (line_header == "Map Quant Levels")
			{
				quantLevels = line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
			}

			// 5
			if (line_header == "Target Min Bound")
			{
				target_MIN = line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
			}

			// 6
			if (line_header == "Target Max Bound")
			{
				target_MAX = line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
			}

			// 7
			if (line_header == "Polarity")
			{
				polarity = line_Rem.upToFirstOccurrenceOf(",", false, true).getIntValue();
			}

			// 8
			if (line_header == "Tempo")
			{
				tempo = line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
			}

			// 9
			if (line_header == "Rhythm")
			{
				rhythm = line_Rem.upToFirstOccurrenceOf(",", false, true);
			}

			// 10
			if (line_header == "Track Gains")
			{
				for (int i = 0; i < 8; i++)
				{
					trackGains[i] = line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
					line_Rem = line_Rem.fromFirstOccurrenceOf(",", false, true);
				}
			}

			// 11
			if (line_header == "Track Mutes")
			{
				for (int i = 0; i < 8; i++)
				{
					trackMutes[i] = line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
					line_Rem = line_Rem.fromFirstOccurrenceOf(",", false, true);
				}
			}

			// 12
			if (line_header == "Track Variants")
			{
				for (int i = 0; i < 8; i++)
				{
					variants[i] = line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
					line_Rem = line_Rem.fromFirstOccurrenceOf(",", false, true);
				}
			}

			// 13
			if (line_header == "Center X")
			{
				SB_C_X = line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
			}

			// 14
			if (line_header == "Center Y")
			{
				SB_C_Y = line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
			}

			// 15
			if (line_header == "Width X")
			{
				SB_Wid_X = line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
			}

			// 16
			if (line_header == "Width Y")
			{
				SB_Wid_Y = line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
			}

			// 17
			if (line_header == "Trajectory Shape")
			{
				DB_Shape = line_Rem.upToFirstOccurrenceOf(",", false, true);
			}

			// 18
			if (line_header == "Trajectory Mirrored")
			{
				DB_Mirrored = line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
			}

			// 19
			if (line_header == "Trajectory Period (Bars)")
			{
				DB_Period = line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
			}

			// 20
			if (line_header == "Trajectory Radius")
			{
				DB_Radius = line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
			}

			// 21
			if (line_header == "Pre Sit Angle")
			{
				STS_angle_preSit = line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
			}

			// 22
			if (line_header == "Pre Stand Angle")
			{
				STS_angle_preStand = line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
			}

			// 23
			if (line_header == "Timing Mode")
			{
				Gait_TimingMode = line_Rem.upToFirstOccurrenceOf(",", false, true);
			}

			// 24
			if (line_header == "Error Tolerance")
			{
				Gait_ErrorTolerance = line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
			}

			// 25
			if (line_header == "Acc Thresh")
			{
				Gait_AccThresh = line_Rem.upToFirstOccurrenceOf(",", false, true).getFloatValue();
			}
		}

		int a = 0;
	}

};

