#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
class UI_MusicControls_Inbuilt
{
public:

	ComboBox inbuilt_ChooseMIDI;
	ComboBox inbuilt_Tonic;
	ComboBox inbuilt_Scale;

	UI_MusicControls_Inbuilt()
	{
		configure();
	};
	~UI_MusicControls_Inbuilt() {};

	void configure()
	{

	}

	void toggleVisible(bool on)
	{
		inbuilt_ChooseMIDI.setVisible(on);
		inbuilt_Tonic.setVisible(on);
		inbuilt_Scale.setVisible(on);
	}

	void setLayout()
	{
		inbuilt_ChooseMIDI.setBounds(270, 355, 200, 40);
		inbuilt_Tonic.setBounds(270, 405, 200, 40);
		inbuilt_Scale.setBounds(270, 455, 200, 40);
	}
};
