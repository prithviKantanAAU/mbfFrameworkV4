#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class FaustStrings_AP
{
public:
	FaustStrings_AP()
	{
	};
	~FaustStrings_AP() {};

	String baseName = "/GaitSoni/";

	String SonificationNames[20] =
	{
		"Punishment_Strategies/x_P1_Detune",
		"Punishment_Strategies/x_P2_EnsembleDensity",
		"Punishment_Strategies/x_P3_ChordFreqDist",

		"Reward_Strategies/x_R1_InstLevel",
		"",
		"Reward_Strategies/x_R3_OverallBrightness",

		"JerkSoni_Strategies/x_J1_MelBaseFreq",
		"JerkSoni_Strategies/x_J2_Pitched",
		"JerkSoni_Strategies/x_J3_Whoosh",

		"HStrike_Strategies/x_H1_SnareTrigger",
		"Punishment_Strategies/x_SB1_Ambulance",
		"Punishment_Strategies/x_SB2_Instrumentation",
		"Punishment_Strategies/x_SB3_EnsembleIntensity",
		"JerkSoni_Strategies/x_STS1_Bell",
		"JerkSoni_Strategies/x_STS2_MusicStop",
		"JerkSoni_Strategies/x_STS2_Wah",
		"",
		"",
	};
};



