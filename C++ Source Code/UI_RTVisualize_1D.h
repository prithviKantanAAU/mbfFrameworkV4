#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
class UI_RTVisualize_1D
{
public:
	UI_RTVisualize_1D()
	{
		configure();
	};
	~UI_RTVisualize_1D() {};

	Label realTimeVisualize_Label;
	Label rtv_minBound;		Label rtv_maxBound;
	Label rtv_fullRangeBar;
	Label rtv_currentValue;
	Label rtv_targetRange;
	Label rtv_outerBound;
	short dispMode = 1;
	short numDispModes = 2;
	int rtv_startX[5] = { 50, 50 };
	int rtv_width[5] = { 600, 1100 };
	int rtv_ht[5] = { 50, 300 };
	int rtv_startY[5] = { 550, 300 };
	int outerbound[5][4] = 
	{
		{48,435,625,145},
		{48,50,1100,625}
	};

	int minbound[5][4] =
	{
		{50,600,100,30},
		{50,710,100,30}
	};

	int maxbound[5][4] =
	{
		{650,600,100,30},
		{1150,710,100,30}
	};

	void configure()
	{
		rtv_outerBound.setColour(rtv_outerBound.outlineColourId, Colours::blue);
	}

	void configureBounds(float minValue, float maxValue)
	{
		rtv_minBound.setText(std::to_string((int)minValue), dontSendNotification);
		rtv_maxBound.setText(std::to_string((int)maxValue), dontSendNotification);
		rtv_fullRangeBar.setColour(rtv_fullRangeBar.backgroundColourId, Colours::grey);
		rtv_targetRange.setColour(rtv_targetRange.backgroundColourId, Colours::green);
		rtv_targetRange.setText("T", dontSendNotification);
		rtv_targetRange.setJustificationType(Justification::centred);
		rtv_currentValue.setColour(rtv_currentValue.backgroundColourId, Colours::red);
		rtv_currentValue.setText("C", dontSendNotification);
		rtv_currentValue.setJustificationType(Justification::centred);
	}

	void toggleVisible(bool on)
	{
		rtv_minBound.setVisible(on);
		rtv_maxBound.setVisible(on);
		rtv_currentValue.setVisible(on);
		rtv_targetRange.setVisible(on);
		rtv_fullRangeBar.setVisible(on);
		rtv_outerBound.setVisible(on);
	}

	void setLayout()
	{
		rtv_outerBound.setBounds(rtv_startX[0] - 2, rtv_startY[0] - 2.5 * rtv_ht[0] + 10,rtv_width[0] + 25, 2.5 *rtv_ht[0] + 20);
		rtv_minBound.setBounds(rtv_startX[0], rtv_startY[0], 100, 30);
		rtv_maxBound.setBounds(rtv_startX[0] + rtv_width[0] - 15, rtv_startY[0], 100, 30);
		rtv_targetRange.setBounds(rtv_startX[0], rtv_startY[0] - 40 - rtv_ht[0] / 2, 100, rtv_ht[0]);
		rtv_currentValue.setBounds(rtv_startX[0] + 20, rtv_startY[0] - 40 + rtv_ht[0] / 2, 40, rtv_ht[0]);
	}
};

