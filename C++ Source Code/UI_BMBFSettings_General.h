#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
class UI_BMBFSettings_General
{
public:
	UI_BMBFSettings_General()
	{
		configure();
	};
	~UI_BMBFSettings_General() {};

	ToggleButton isStandbyToggle;
	Label isStandbyToggleLabel;
	Label gaitParam_Name;
	Label gaitParam_CurrentValue;
	TextButton recordGaitParam;
	Label gaitParam_targetValue;
	Slider gaitParam_setTarget;
	ToggleButton enable_dynTarget;
	Label enable_dynTarget_Label;
	ToggleButton soni_isSliderSource;
	Label soni_isMusical_Label;
	Label soni_isSliderSource_Label;
	Slider soni_sliderSource;
	Label soni_sliderSource_Label;
	ToggleButton realTimeVisualize;
	Label realTimeVisualize_Label;
	ComboBox exerciseMode;
	Label exerciseModeLabel;
	ComboBox gaitParam_Current;
	Label gaitParam_Current_Label;
	ComboBox audioParam_Current;
	Label audioParam_Current_Label;
	Slider soni_Order;
	Label soni_Order_Label;
	Slider soni_QuantLevels;
	Label soni_QuantLevels_Label;
	TextButton invertPolarity;
	TextButton saveSnapshot;
	TextButton loadSnapshot;
	Slider soni_Vol;
	Label soni_Vol_Label;
	Slider medianFilter_Length;
	Label medianFilter_Length_Label;
	Slider imuSmooth_Fc;
	Label imuSmooth_Fc_Label;
	TextButton rtv_Expand, zv_Expand;

	void configure()
	{
		// STANDBY
		isStandbyToggle.setToggleState(false, dontSendNotification);
		isStandbyToggleLabel.setText("Standby OFF", dontSendNotification);
		isStandbyToggleLabel.setColour(isStandbyToggleLabel.textColourId, Colours::black);
		isStandbyToggleLabel.setColour(isStandbyToggleLabel.backgroundColourId, Colours::green);
		isStandbyToggleLabel.attachToComponent(&isStandbyToggle, true);

		// RECORD GAIT PARAM
		recordGaitParam.setButtonText("Rec Gait Param");
		recordGaitParam.setColour(recordGaitParam.buttonColourId, Colours::red);

		// TARGET SETTER
		gaitParam_setTarget.setSliderStyle(Slider::SliderStyle::TwoValueHorizontal);
		gaitParam_setTarget.setRange(0, 1);
		gaitParam_setTarget.setNumDecimalPlacesToDisplay(2);
		gaitParam_setTarget.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 1, 1);
		gaitParam_setTarget.setColour(gaitParam_setTarget.backgroundColourId, Colours::red);
		gaitParam_setTarget.setColour(gaitParam_setTarget.trackColourId, Colours::green);

		// SONIFICATION MODE: SLIDER / SENSOR

		soni_isSliderSource.setToggleState(true, dontSendNotification);
		soni_isSliderSource_Label.setText("Source: Slider", dontSendNotification);
		soni_isSliderSource_Label.setColour(soni_isSliderSource_Label.backgroundColourId, Colours::red);
		soni_isSliderSource_Label.attachToComponent(&soni_isSliderSource, true);

		// SLIDER - SONIFICATION SOURCE

		soni_sliderSource.setRange(0, 1);
		soni_sliderSource.setNumDecimalPlacesToDisplay(2);
		soni_sliderSource.setValue(0);
		soni_sliderSource.setColour(soni_sliderSource.backgroundColourId, Colours::red);

		// EXERCISE MODE
		exerciseMode.setColour(exerciseMode.backgroundColourId, Colours::red);

		//Enable Dynamic Target
		enable_dynTarget.setToggleState(false, dontSendNotification);
		enable_dynTarget_Label.setText("Enable Dynamic Target", dontSendNotification);
		enable_dynTarget_Label.attachToComponent(&enable_dynTarget, true);

		//Enable Real Time Visualize
		realTimeVisualize.setToggleState(true, dontSendNotification);

		//Soni Order
		soni_Order.setRange(0.1, 10);
		soni_Order.setValue(1);
		soni_Order.setSkewFactor(0.4);
		soni_Order.setNumDecimalPlacesToDisplay(2);
		soni_Order.setColour(soni_Order.trackColourId, Colours::blue);
		soni_Order.setColour(soni_Order.backgroundColourId, Colours::yellow);
		soni_Order_Label.setText("Mapping Func Order", dontSendNotification);
		soni_Order_Label.attachToComponent(&soni_Order, false);

		//Soni Quant
		soni_QuantLevels.setRange(0, 50);
		soni_QuantLevels.setValue(0);
		soni_QuantLevels.setSkewFactor(0.4);
		soni_QuantLevels.setNumDecimalPlacesToDisplay(0);
		soni_QuantLevels.setColour(soni_QuantLevels.trackColourId, Colours::blue);
		soni_QuantLevels.setColour(soni_QuantLevels.backgroundColourId, Colours::yellow);
		soni_QuantLevels_Label.setText("Quant Levels", dontSendNotification);
		soni_QuantLevels_Label.attachToComponent(&soni_QuantLevels, false);

		// INVERT MAPPING POLARITY
		invertPolarity.setButtonText("Polarity Normal");
		invertPolarity.setColour(invertPolarity.buttonColourId, Colours::green);

		// SAVE SNAPSHOT
		saveSnapshot.setButtonText("Save Settings");
		saveSnapshot.setColour(saveSnapshot.buttonColourId, Colours::blue);

		// LOAD SNAPSHOT
		loadSnapshot.setButtonText("Load Settings");
		loadSnapshot.setColour(loadSnapshot.buttonColourId, Colours::darkgoldenrod);

		// SONI VOLUME
		soni_Vol.setRange(-10, 20);
		soni_Vol.setValue(-8);
		soni_Vol.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 10, 10);
		soni_Vol.setColour(soni_Order.trackColourId, Colours::blue);
		soni_Vol.setColour(soni_Order.backgroundColourId, Colours::yellow);
		soni_Vol_Label.setText("Soni Vol", dontSendNotification);
		soni_Vol_Label.attachToComponent(&soni_Vol, false);

		// Median Filter Length
		medianFilter_Length.setRange(1, 30);
		medianFilter_Length.setValue(3);
		medianFilter_Length.setSkewFactor(0.4);
		medianFilter_Length.setNumDecimalPlacesToDisplay(0);
		medianFilter_Length.setColour(soni_Order.trackColourId, Colours::blue);
		medianFilter_Length.setColour(soni_Order.backgroundColourId, Colours::yellow);
		medianFilter_Length_Label.setText("Median Filter L", dontSendNotification);
		medianFilter_Length_Label.attachToComponent(&medianFilter_Length, false);

		// IMU Smoothing
		imuSmooth_Fc.setRange(0.5, 50);
		imuSmooth_Fc.setValue(20);
		imuSmooth_Fc.setSkewFactor(0.4);
		imuSmooth_Fc.setNumDecimalPlacesToDisplay(2);
		imuSmooth_Fc.setColour(soni_Order.trackColourId, Colours::blue);
		imuSmooth_Fc.setColour(soni_Order.backgroundColourId, Colours::yellow);
		imuSmooth_Fc_Label.setText("IMU Smoothing Fc", dontSendNotification);
		imuSmooth_Fc_Label.attachToComponent(&imuSmooth_Fc, false);

		// VISUALIZER EXPANDERS
		rtv_Expand.setButtonText("<>");
		zv_Expand.setButtonText("<>");
	}

	void toggleVisible(bool on)
	{
		isStandbyToggle.setVisible(on);
		gaitParam_CurrentValue.setVisible(on);
		recordGaitParam.setVisible(on);
		gaitParam_targetValue.setVisible(on);
		gaitParam_setTarget.setVisible(on);
		soni_isSliderSource.setVisible(on);
		soni_sliderSource.setVisible(on);
		soni_isMusical_Label.setVisible(on);
		soni_isSliderSource_Label.setVisible(on);
		soni_sliderSource_Label.setVisible(on);
		exerciseMode.setVisible(on);
		gaitParam_Current.setVisible(on);
		audioParam_Current.setVisible(on);
		enable_dynTarget.setVisible(on);
		enable_dynTarget_Label.setVisible(on);
		realTimeVisualize.setVisible(on);
		soni_Order.setVisible(on);
		soni_Order_Label.setVisible(on);
		soni_QuantLevels.setVisible(on);
		soni_QuantLevels_Label.setVisible(on);
		invertPolarity.setVisible(on);
		saveSnapshot.setVisible(on);
		loadSnapshot.setVisible(on);
		soni_Vol.setVisible(on);
		medianFilter_Length.setVisible(on);
		imuSmooth_Fc.setVisible(on);
		rtv_Expand.setVisible(on);
		zv_Expand.setVisible(on);
	}

	void setLayout()
	{
		// COLUMN 1
		exerciseMode.setBounds(50, 40, 200, 40);

		// COLUMN 2
		gaitParam_Current.setBounds(270, 40, 200, 40);
		soni_Order.setBounds(270, 110, 200, 40);
		soni_QuantLevels.setBounds(270, 190, 200, 40);
		gaitParam_targetValue.setBounds(270, 270, 200, 40);
		gaitParam_setTarget.setBounds(270, 350, 200, 40);

		// COLUMN 3
		audioParam_Current.setBounds(490, 40, 200, 40);
		soni_isSliderSource.setBounds(620, 110, 200, 40);
		soni_sliderSource.setBounds(490, 190, 200, 40);
		imuSmooth_Fc.setBounds(490, 270, 200, 40);
		medianFilter_Length.setBounds(490, 350, 200, 40);

		// COLUMN 4
		saveSnapshot.setBounds(710, 40, 200, 40);
		loadSnapshot.setBounds(710, 110, 200, 40);
		soni_Vol.setBounds(710, 190, 200, 40);
		recordGaitParam.setBounds(710, 270, 200, 40);
		invertPolarity.setBounds(710, 350, 200, 40);

		// TOP RIGHT
		isStandbyToggle.setBounds(1090, 40, 150, 40);

		// MISC
		rtv_Expand.setBounds(350, 590, 40, 40);
		zv_Expand.setBounds(1155, 410, 40, 40);
	}
};

