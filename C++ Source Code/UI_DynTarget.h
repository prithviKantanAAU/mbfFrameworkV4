#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
class UI_DynTarget
{
public:
	UI_DynTarget()
	{
		configure();
	};
	~UI_DynTarget() {};

	Label dynTargetLabel;
	Slider desired_Amplitude;
	Label desired_Amplitude_Label;
	Slider movementFunc_Order;
	Label movementFunc_Order_Label;
	Slider movement_errorTolerance;
	Label movement_errorTolerance_Label;
	ComboBox desired_MovementFreq;
	Label desired_MovementFreq_Label;

	void configure()
	{
		//Desired Movement Amplitude
		desired_Amplitude.setValue(1);
		desired_Amplitude.setRange(0, 1);
		desired_Amplitude.setNumDecimalPlacesToDisplay(2);
		desired_Amplitude_Label.setText("Desired Movement %", dontSendNotification);
		desired_Amplitude_Label.attachToComponent(&desired_Amplitude, false);

		//Desired Movement Frequency
		desired_MovementFreq.addItem("1/8 Tempo", 1);
		desired_MovementFreq.addItem("1/16 Tempo", 2);
		desired_MovementFreq.setSelectedId(1);
		desired_MovementFreq_Label.setText("Movement Rhythm", dontSendNotification);
		desired_MovementFreq_Label.attachToComponent(&desired_MovementFreq, false);

		//Movement Function Order
		movementFunc_Order.setValue(1);
		movementFunc_Order.setRange(0.05, 3);
		movementFunc_Order.setNumDecimalPlacesToDisplay(2);
		movementFunc_Order_Label.setText("Movement Func Order", dontSendNotification);
		movementFunc_Order_Label.attachToComponent(&movementFunc_Order, false);

		//Movement Error Tolerance
		movement_errorTolerance.setValue(0);
		movement_errorTolerance.setRange(0.0, 0.3);
		movement_errorTolerance.setNumDecimalPlacesToDisplay(2);
		movement_errorTolerance_Label.setText("Error Tolerance", dontSendNotification);
		movement_errorTolerance_Label.attachToComponent(&movement_errorTolerance, false);
	}

	void toggleVisible(bool on)
	{
		dynTargetLabel.setVisible(on);
		desired_Amplitude.setVisible(on);
		desired_Amplitude_Label.setVisible(on);
		desired_MovementFreq.setVisible(on);
		desired_MovementFreq_Label.setVisible(on);
		movementFunc_Order.setVisible(on);
		movementFunc_Order_Label.setVisible(on);
		movement_errorTolerance.setVisible(on);
		movement_errorTolerance_Label.setVisible(on);
	}

	void setLayout()
	{
		dynTargetLabel.setBounds(710, 110, 200, 40);
		desired_Amplitude.setBounds(710, 190, 200, 40);
		desired_MovementFreq.setBounds(710, 270, 200, 40);
		movementFunc_Order.setBounds(710, 350, 200, 40);
		movement_errorTolerance.setBounds(710, 430, 200, 40);
	}
};

