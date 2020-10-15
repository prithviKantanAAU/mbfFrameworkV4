#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
# define M_PI           3.14159265358979323846  /* pi */

class RingMusicVisualize
{
public:

	short numBoxes = 8;
	Label beatSubBox[32];
	double box_Pos_X[32] = { 0.0 };
	double box_Pos_Y[32] = { 0.0 };
	double box_Phase[32] = { 0.0 };
	double phaseInc = 0;
	double presentPhase_Rad = 0;
	double phaseDiff_Consecutive = 0;
	double center_X = 0;
	double center_Y = 0;
	double radius = 0;
	double box_Side[32] = { 0.0 };

	Slider emphFunc_MAX;
	Label emphFunc_MAX_Label;
	Slider emphFunc_MIN;
	Label emphFunc_MIN_Label;
	Slider emphFunc_C;
	Label emphFunc_C_Label;
	Slider emphFunc_WID;
	Label emphFunc_WID_Label;
	Slider emphFunc_ORDER;
	Label emphFunc_ORDER_Label;
	ComboBox emph_Strategy;

	void initSliders()
	{
		emphFunc_MAX.setRange(0, 10);
		emphFunc_MAX.setSkewFactor(0.3);
		emphFunc_MAX.setNumDecimalPlacesToDisplay(1);
		emphFunc_MAX.setValue(1);
		emphFunc_MAX_Label.setText("Emph MAX", dontSendNotification);
		emphFunc_MAX_Label.attachToComponent(&emphFunc_MAX, true);

		emphFunc_MIN.setRange(0, 10);
		emphFunc_MIN.setSkewFactor(0.3);
		emphFunc_MIN.setNumDecimalPlacesToDisplay(1);
		emphFunc_MIN.setValue(0);
		emphFunc_MIN_Label.setText("Emph MIN", dontSendNotification);
		emphFunc_MIN_Label.attachToComponent(&emphFunc_MIN, true);

		emphFunc_C.setRange(1, 4);
		emphFunc_C.setNumDecimalPlacesToDisplay(0);
		emphFunc_C.setValue(1);
		emphFunc_C_Label.setText("Emph Beat", dontSendNotification);
		emphFunc_C_Label.attachToComponent(&emphFunc_C, true);

		emphFunc_WID.setRange(1, 4);
		emphFunc_WID.setNumDecimalPlacesToDisplay(1);
		emphFunc_WID.setValue(2);
		emphFunc_WID_Label.setText("Emph W", dontSendNotification);
		emphFunc_WID_Label.attachToComponent(&emphFunc_WID, true);

		emphFunc_ORDER.setRange(0.2, 2);
		emphFunc_ORDER.setNumDecimalPlacesToDisplay(1);
		emphFunc_ORDER.setValue(1);
		emphFunc_ORDER_Label.setText("Emph Order", dontSendNotification);
		emphFunc_ORDER_Label.attachToComponent(&emphFunc_ORDER, true);
	}

	void setNumBoxes(int value)
	{
		numBoxes = value;
		phaseDiff_Consecutive = (2 * M_PI) / numBoxes;
		for (int i = 0; i < numBoxes; i++)
		{
			box_Side[i] = i % (numBoxes / 4) == 0 ? 50 : 20;
			box_Phase[i] = 2 * M_PI - i * phaseDiff_Consecutive;
		}
	};

	void setCircleCenter_Rad(double x, double y, double rad)
	{
		center_X = x;
		center_Y = y;
		radius = rad;
	}

	void refreshBoxPositions(double phase)
	{
		presentPhase_Rad = phase;
		for (int i = 0; i < numBoxes; i++)
		{
			box_Phase[i] = presentPhase_Rad - i * phaseDiff_Consecutive;
			box_Phase[i] = box_Phase[i] < 0 ? (box_Phase[i] + 2 * M_PI) : box_Phase[i];
			setBoxPosition(box_Phase[i], &box_Pos_X[i], &box_Pos_Y[i], box_Side[i]);
		}
	}

	void setPhaseInc(double tempo, double refreshFreq)
	{
		double beatTime = 60.0 / tempo;
		double rotationTime = 4 * beatTime;
		double refreshTime = 1.0 / refreshFreq;
		phaseInc = refreshTime / rotationTime * (2 * M_PI);
		presentPhase_Rad -= phaseInc;
	};

	void setBoxPosition(double box_Phase, double* box_X, double* box_Y, double box_Width)
	{
		*box_X = center_X + radius/5 * cos(box_Phase) - box_Width / 2;
		*box_Y = center_Y + radius * 1.2 * sin(box_Phase) - box_Width / 2;
	};

	void resetPhase()
	{
		presentPhase_Rad = 0;
	};

	RingMusicVisualize()
	{
		phaseDiff_Consecutive = (2 * M_PI) / numBoxes;
		for (int i = 0; i < numBoxes; i++)
		{
			box_Side[i] = i % (numBoxes/4) == 0 ? 50 : 20;
			box_Phase[i] = 2 * M_PI - i * phaseDiff_Consecutive;
		}
		initSliders();
	};

	void RGBFromEmph(float* rgbArray, double emphVal, float minVal, float maxVal)
	{
		emphVal = (emphVal - minVal) / (maxVal - minVal);
		emphVal = min(emphVal, 1);
		short emphVal_Int = (int)(emphVal * 3);
		rgbArray[0] = 0;	rgbArray[1] = 0; rgbArray[2] = 0;
		switch (emphVal_Int)
		{
		case 0:
			rgbArray[2] = 1;
			break;
		case 1:
			rgbArray[2] = 1;
			rgbArray[1] = sqrt(1 - 1 * (0.66 - emphVal) / 0.33);
			break;
		case 2:
			rgbArray[2] = sqrt(1 - 1 * (emphVal - 0.66) / 0.33);
			rgbArray[1] = 1;
			rgbArray[0] = sqrt(1 - 1 * (1 - emphVal) / 0.33);
			break;
		case 3:
			rgbArray[0] = 1;
			rgbArray[1] = 1;
			break;
		}
	};

	~RingMusicVisualize()
	{

	};

	void toggleVisibility(bool on, short musicMode)
	{
		for (int i = 0; i < numBoxes; i++)
			beatSubBox[i].setVisible(on);
		
		emphFunc_MAX.setVisible(on);
		emphFunc_MIN.setVisible(on);
		emphFunc_C.setVisible(on);
		emphFunc_WID.setVisible(on);
		emphFunc_ORDER.setVisible(on);
		emph_Strategy.setVisible(on);
		
	}

	void setLayout()
	{
		emphFunc_C.setBounds(780, 350, 150, 30);
		emphFunc_WID.setBounds(780, 380, 150, 30);
		emphFunc_MAX.setBounds(780, 410, 150, 30);
		emphFunc_MIN.setBounds(780, 440, 150, 30);
		emphFunc_ORDER.setBounds(780, 470, 150, 30);
		emph_Strategy.setBounds(780, 500, 150, 40);
	}
};

