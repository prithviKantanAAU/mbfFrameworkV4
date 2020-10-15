#pragma once
#include <string>

class FaustStrings
{
public:

	FaustStrings()
	{
	};

	~FaustStrings()
	{

	};

	std::string baseName = "/GaitSoni/";

	std::string Tempo = "Mapping_Tabs/Music_Info/Tempo";

	// Master Volume and Track Mute
	std::string MasterVol = "Mapping_Tabs/Mixer_And_Master/Master_Gain/Master_Gain";
	std::string SoniVol = baseName + "Mapping_Tabs/Mixer_And_Master/Master_Gain/Soni_Buss_Gain";
	std::string trackVariant_Base = "Mapping_Tabs/Mixer_And_Master/Track_Variant/Variant_";

	std::string trackGain_Base = "Mapping_Tabs/Mixer_And_Master/Track_Gain/";
	std::string trackGains[8] =
	{
		"Track_1","Track_2","Track_3","Track_4","Track_5","Track_6","Track_7","Track_8"
	};

	String SonificationBase = "Mapping_Tabs/Sonification_Types/";

	String getSonificationStringAddress(int sonificationIndex)
	{
		return String(baseName) + SonificationBase + "AP_Values/Soni_" + String(sonificationIndex);
	};

	std::string getMusicAddress(int trackNum, String pOrV, int voiceNum)
	{
		std::string attribute = "";
		if (pOrV == "P") attribute = "Pitch";
		if (pOrV == "V") attribute = "Velocity";
		std::string address = baseName + "Mapping_Tabs/Music_Info/" + attribute + "/T"
			+ std::to_string(trackNum) + "_" + pOrV.toStdString() + "_" + std::to_string(voiceNum);
		return address;
	}

	std::string getTrackGainString(int trackIndex)
	{
		return baseName + trackGain_Base + trackGains[trackIndex];
	}

	std::string getTrackMuteString(int trackIndex)
	{
		return baseName + trackMutes_Base + trackMutes[trackIndex];
	}

	std::string trackMutes_Base = "Mapping_Tabs/Mixer_And_Master/Track_Mute/";
	std::string trackMutes[8] = { "1","2","3","4","5","6","7","8" };

	std::string trackIDs[8] 
	= { "Track_1/","Track_2/","Track_3/","Track_4/","Track_5/","Track_6/","Track_7/","Track_8/" };

	std::string MasterEQ_1_F = baseName + "Mapping_Tabs/Mixer_And_Master/Master_EQ/2_Parametric_1/Freq";
	std::string MasterEQ_2_F = baseName + "Mapping_Tabs/Mixer_And_Master/Master_EQ/3_Parametric_2/Freq";

	std::string MasterEQ_1_Q = baseName + "Mapping_Tabs/Mixer_And_Master/Master_EQ/2_Parametric_1/Q";
	std::string MasterEQ_2_Q = baseName + "Mapping_Tabs/Mixer_And_Master/Master_EQ/3_Parametric_2/Q";

	std::string MasterEQ_1_Gain = baseName + "Mapping_Tabs/Mixer_And_Master/Master_EQ/2_Parametric_1/Gain";
	std::string MasterEQ_2_Gain = baseName + "Mapping_Tabs/Mixer_And_Master/Master_EQ/3_Parametric_2/Gain";

	//EQ Setters
	std::string EQ_base = "Mapping_Tabs/EQ_Section/";
	std::string EQ_filterIDs[4] = { "1_HPF/", "2_Parametric_1/", "3_Parametric_2/", "4_LPF/" };
	std::string EQ_filterParamIDs[3] = { "Freq", "Gain", "Q" };
	std::string FetchEQ_String(int trackID, int filterID, int paramID)
	{
		return baseName + EQ_base + trackIDs[trackID] + EQ_filterIDs[filterID] + EQ_filterParamIDs[paramID];
	};

	//Comp Setters
	std::string Comp_base = "Mapping_Tabs/Comp_Section/";
	std::string Comp_ParamIDs[4] = { "Threshold","Ratio","Attack","Release" };
	std::string FetchComp_String(int trackID, int paramID)
	{
		return baseName + Comp_base + trackIDs[trackID] + Comp_ParamIDs[paramID];
	};
};