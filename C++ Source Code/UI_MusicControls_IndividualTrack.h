#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
class UI_MusicControls_IndividualTrack
{
public:
	UI_MusicControls_IndividualTrack()
	{
		addElements();
		configure();
	};
	~UI_MusicControls_IndividualTrack() {};

	Label channel_EQ_Labels[4];
	Label channel_Comp_Labels[4];
	Slider channel_EQ_Settings[3];
	Slider channel_Comp_Settings[4];
	ComboBox channel_TrackIdx;
	ComboBox channel_EQ_FiltIdx;
	short channel_ActiveTrack = 0;
	short channel_EQ_ActiveFiltIdx = 0;
	String channel_EQ_Labels_Text[3] =
	{
		"Frequency",
		"Gain",
		"Q"
	};
	float channel_EQ_Bounds[3][2] =
	{
		{20, 20000},
		{-15, 15},
		{0.3, 5}
	};
	String channel_Comp_Labels_Text[4] =
	{
		"Threshold",
		"Ratio",
		"Attack",
		"Release"
	};
	float channel_Comp_Bounds[4][2] =
	{
		{-40, 0},
		{1, 10},
		{0.001, 0.100},
		{0.001, 1.000}
	};

	void addElements()
	{
	}

	void configure()
	{
		channel_TrackIdx.addItem("Track 1", 1);
		channel_TrackIdx.addItem("Track 2", 2);
		channel_TrackIdx.addItem("Track 3", 3);
		channel_TrackIdx.addItem("Track 4", 4);
		channel_TrackIdx.addItem("Track 5", 5);
		channel_TrackIdx.addItem("Track 6", 6);
		channel_TrackIdx.addItem("Track 7", 7);
		channel_TrackIdx.addItem("Track 8", 8);
		channel_TrackIdx.setSelectedId(1);
		
		channel_EQ_FiltIdx.addItem("HPF", 1);
		channel_EQ_FiltIdx.addItem("B1", 2);
		channel_EQ_FiltIdx.addItem("B2", 3);
		channel_EQ_FiltIdx.addItem("LPF", 4);
		channel_EQ_FiltIdx.setSelectedId(1);

		for (int i = 0; i < 4; i++)
		{
			channel_Comp_Settings[i].setRange(channel_Comp_Bounds[i][0], channel_Comp_Bounds[i][1]);
			channel_Comp_Settings[i].setNumDecimalPlacesToDisplay(2);
			channel_Comp_Settings[i].setColour(channel_Comp_Settings[i].trackColourId, Colours::yellow);
			channel_Comp_Labels[i].setText(channel_Comp_Labels_Text[i], dontSendNotification);
			channel_Comp_Labels[i].attachToComponent(&channel_Comp_Settings[i], true);
		}

		for (int i = 0; i < 3; i++)
		{
			channel_EQ_Settings[i].setRange(channel_EQ_Bounds[i][0], channel_EQ_Bounds[i][1]);
			channel_EQ_Settings[i].setNumDecimalPlacesToDisplay(2);
			if (i == 0)	
				channel_EQ_Settings[i].setSkewFactor(0.3);
			channel_EQ_Settings[i].setColour(channel_EQ_Settings[i].trackColourId, Colours::yellow);
			channel_EQ_Labels[i].setText(channel_EQ_Labels_Text[i], dontSendNotification);
			channel_EQ_Labels[i].attachToComponent(&channel_EQ_Settings[i], true);
		}
	}

	void toggleVisible(bool on)
	{
		channel_TrackIdx.setVisible(on);
		channel_EQ_FiltIdx.setVisible(on);
		for (int i = 0; i < 4; i++)
		{
			channel_Comp_Settings[i].setVisible(on);
			channel_Comp_Labels[i].setVisible(on);
		}
		for (int j = 0; j < 3; j++)
		{
			channel_EQ_Settings[j].setVisible(on);
			channel_EQ_Labels[j].setVisible(on);
		}
	}

	void setLayout()
	{
		channel_TrackIdx.setBounds(750, 320, 180, 30);
		channel_EQ_FiltIdx.setBounds(750, 360, 180, 30);
		for (int i = 0; i < 3; i++)
			channel_EQ_Settings[i].setBounds(750, 400 + i * 40, 180, 30);
		for (int i = 0; i < 4; i++)
			channel_Comp_Settings[i].setBounds(750, 540 + i * 40, 180, 30);
	}
};

