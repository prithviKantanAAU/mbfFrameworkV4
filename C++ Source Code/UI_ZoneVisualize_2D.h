#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
class UI_ZoneVisualize_2D
{
public:
	UI_ZoneVisualize_2D()
	{
		configure();
	};
	~UI_ZoneVisualize_2D() {};

	Label ZV_TotalSpace;
	Label ZV_InnerRect;
	Label ZV_CurrentPos;
	Label ZV_InnerRect_AP;
	Label ZV_F;
	Label ZV_B;
	Label ZV_L;
	Label ZV_R;
	short dispMode = 1;
	short numDispModes = 2;
	float ZV_totalLength[5] = { 200, 600 };
	float ZV_zeroPos_H[5] = { 1040, 570};
	float ZV_zeroPos_V[5] = { 420, 320 };
	float ZV_ML_startX_MIN[5] = { 950, 300 };
	float ZV_ML_endX_MAX[5] = { 1150, 900 };
	float ZV_AP_startY_MIN[5] = { 330, 50 };
	float ZV_AP_endY_MAX[5] = { 530, 650 };
	float ZV_Current_W[5] = { 20, 60 };
	float ZV_Current_H[5] = { 20, 60 };

	int ZV_F_Lab_Bounds[2][4] = 
	{
		{1040, 330, 20, 20},
		{590, 60, 20, 20}
	};

	int ZV_B_Lab_Bounds[2][4] =
	{
		{1040, 510, 20, 20},
		{590, 620, 20, 20}
	};

	int ZV_L_Lab_Bounds[2][4] =
	{
		{950, 420, 20, 20},
		{310, 340, 20, 20}
	};

	int ZV_R_Lab_Bounds[2][4] =
	{
		{1135, 420, 20, 20},
		{870, 340, 20, 20}
	};

	int ZV_TotalSpace_Bounds[2][4] = 
	{
		{950, 330, 200, 200},
		{300, 50, 600, 600}
	};

	void configure()
	{
		ZV_TotalSpace.setColour(ZV_TotalSpace.outlineColourId, Colours::blue);
		ZV_InnerRect.setColour(ZV_InnerRect.outlineColourId, Colours::green);
		ZV_InnerRect.setColour(ZV_InnerRect.backgroundColourId, Colour::fromFloatRGBA(0, 1, 0, 0.35));
		ZV_InnerRect_AP.setColour(ZV_InnerRect.outlineColourId, Colours::green);
		ZV_InnerRect_AP.setColour(ZV_InnerRect.backgroundColourId, Colour::fromFloatRGBA(0, 1, 0, 0.35));
		ZV_CurrentPos.setColour(ZV_CurrentPos.backgroundColourId, Colours::white);
		ZV_CurrentPos.setColour(ZV_CurrentPos.textColourId, Colours::black);
		ZV_CurrentPos.setText("1", dontSendNotification);
		ZV_F.setText("F", dontSendNotification);
		ZV_B.setText("B", dontSendNotification);
		ZV_L.setText("L", dontSendNotification);
		ZV_R.setText("R", dontSendNotification);
	}

	void toggleVisible(bool on)
	{
		ZV_TotalSpace.setVisible(on);
		ZV_InnerRect.setVisible(on);
		ZV_InnerRect_AP.setVisible(on);
		ZV_CurrentPos.setVisible(on);
		ZV_F.setVisible(on);
		ZV_B.setVisible(on);
		ZV_L.setVisible(on);
		ZV_R.setVisible(on);
	}

	void setLayout()
	{
		ZV_TotalSpace.setBounds(950, 330, 200, 200);
		ZV_InnerRect.setBounds(1030, 330, 40, 200);
		ZV_InnerRect_AP.setBounds(950, 420, 200, 20);
		ZV_CurrentPos.setBounds(1040, 420, 20, 20);
		ZV_F.setBounds(1040, 330, 20, 20);
		ZV_B.setBounds(1040, 510, 20, 20);
		ZV_L.setBounds(950, 420, 20, 20);
		ZV_R.setBounds(1135, 420, 20, 20);
	}
};
