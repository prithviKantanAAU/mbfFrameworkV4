#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
class UI_MPCalibrate
{
public:
	UI_MPCalibrate()
	{
		configure();
	};
	~UI_MPCalibrate() {};

	TextButton calibrateTarget;
	TextButton calibrateTarget_Save;
	TextButton calibrateTarget_Discard;
	Label calibrate_presentTarget;
	Label calibrate_timeElapsed;

	void configure()
	{
		//Calibrate
		calibrateTarget.setButtonText("Switch Exercise Mode");
		calibrateTarget.setVisible(false);

		//Save Calibration
		calibrateTarget_Save.setButtonText("Save Calibration");
		calibrateTarget_Save.setVisible(false);
		calibrateTarget_Save.setColour(calibrateTarget_Save.buttonColourId, Colours::green);

		//Discard Calibration
		calibrateTarget_Discard.setButtonText("Discard");
		calibrateTarget_Discard.setVisible(false);
		calibrateTarget_Discard.setColour(calibrateTarget_Discard.buttonColourId, Colours::red);
	}

	void changeButtonText(short exerciseMode)
	{
		switch (exerciseMode)
		{
		case 1:
			calibrateTarget.setButtonText("Switch Exercise Mode");
			break;
		case 2:
			calibrateTarget.setButtonText("Calibrate Rest Position");
			break;
		case 3:
			calibrateTarget.setButtonText("Calibrate Max Reach");
			break;
		case 4:
			calibrateTarget.setButtonText("Calibrate Jerk");
			break;
		case 5:
			calibrateTarget.setButtonText("Calibrate Stand Angle");
			break;
		case 6:
			calibrateTarget.setButtonText("Calibrate Tempo");
			break;
		}
	}

	void toggleVisible(bool on)
	{
		calibrateTarget.setVisible(on);
		calibrate_presentTarget.setVisible(on);
		calibrateTarget_Discard.setVisible(on);
		calibrateTarget_Save.setVisible(on);
	}

	void setLayout()
	{
		calibrateTarget.setBounds(50, 650, 200, 40);
		calibrate_presentTarget.setBounds(270, 650, 200, 40);
		calibrateTarget_Discard.setBounds(50, 700, 200, 40);
		calibrateTarget_Save.setBounds(270, 700, 200, 40);
	}
};
