#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
class UI_BMBFSettings_ExerciseSpecific
{
public:
	UI_BMBFSettings_ExerciseSpecific() 
	{ 
		configure();
	};
	~UI_BMBFSettings_ExerciseSpecific() {};

	Slider staticBalance_Div_Roll;	
	Label staticBalance_Div_Roll_Label;
	Slider staticBalance_Div_Pitch; 
	Label staticBalance_Div_Pitch_Label;
	Slider staticBalance_Ctrl_X;
	Slider staticBalance_Ctrl_Y;
	ComboBox staticBalance_FeedbackSlope; 
	Label staticBalance_FeedbackSlope_Label;

	Slider dynTrajectory_Radius;
	Label dynTrajectory_Radius_Label;
	ComboBox dynTrajectory_Shape;
	ComboBox dynTrajectory_Period;
	TextButton dynTrajectory_Mirror;

	ComboBox HS_TimingMode;
	Slider HS_Tolerance;
	Label HS_Tolerance_Label;
	Slider HS_AccThresh;
	Label HS_AccThresh_Label;

	Slider sitStand_Thresh_Stand;
	Label sitStand_Thresh_Stand_Label;
	Slider sitStand_Thresh_Sit;
	Label sitStand_Thresh_Sit_Label;
	TextButton sitStand_FlipState;
	Label sitStand_isStanding;

	Label rep_NumDone;
	Label rep_AvgTime_TOTAL;
	Label rep_AvgTime_LAST5;
	TextButton rep_RESTART;

	ComboBox dynTarget_FB_TYPE;
	ComboBox dynTarget_FB_DATA;
	ComboBox dynTarget_FB_X;
	ComboBox dynTarget_FB_Y;

	String rep_str_numDone[10] =
	{
		"",
		"",
		"",
		"Num. Reaches: ",
		"Num. Sit-Stand: ",
		"Num. Sit-Stand: ",
		"Num Steps: ",
		"Num Steps: "
	};

	String rep_str_AvgTime_TOTAL[10] =
	{
		"",
		"",
		"",
		"Avg. Movement Time: ",
		"Avg. Movement Time: ",
		"Avg. Movement Time: ",
		"Avg. Step Time: ",
		"Avg. Step Time: "
	};
	String rep_str_AvgTime_LAST5[10] =
	{
		"",
		"",
		"",
		"Last 5: ",
		"Last 5: ",
		"Last 5: ",
		"Last 5: ",
		"Last 5: "
	};

	void configure()
	{
		//Trunk Balance - Center X
		staticBalance_Ctrl_X.setValue(0);
		staticBalance_Ctrl_X.setRange(-20, 20);
		staticBalance_Ctrl_X.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 10, 10);

		//Trunk Balance - Center Y
		staticBalance_Ctrl_Y.setValue(0);
		staticBalance_Ctrl_Y.setSliderStyle(Slider::SliderStyle::LinearVertical);
		staticBalance_Ctrl_Y.setRange(-20, 20);
		staticBalance_Ctrl_Y.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 10, 10);
		
		//Trunk Balance - Zone Width
		staticBalance_Div_Roll.setValue(2);
		staticBalance_Div_Roll.setRange(2, 10);
		staticBalance_Div_Roll.setColour(staticBalance_Div_Roll.trackColourId, Colours::green);
		staticBalance_Div_Roll.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 10, 10);

		//Trunk Balance - Zone Height
		staticBalance_Div_Pitch.setValue(8);
		staticBalance_Div_Pitch.setRange(0, 8);
		staticBalance_Div_Pitch.setColour(staticBalance_Div_Pitch.backgroundColourId, Colours::green);
		staticBalance_Div_Pitch.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
		staticBalance_Div_Pitch.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 10, 10);

		// DYN RADIUS
		dynTrajectory_Radius.setValue(15);
		dynTrajectory_Radius.setRange(1, 20);
		dynTrajectory_Radius.setColour(dynTrajectory_Radius.backgroundColourId, Colours::blue);
		dynTrajectory_Radius.setColour(dynTrajectory_Radius.trackColourId, Colours::yellow);
		dynTrajectory_Radius.setNumDecimalPlacesToDisplay(0);
		dynTrajectory_Radius_Label.setText("Trajectory Radius", dontSendNotification);
		dynTrajectory_Radius_Label.attachToComponent(&dynTrajectory_Radius, false);

		// DYN TRAJECTORY PERIOD
		dynTrajectory_Period.addItem("1 Bar", 1);
		dynTrajectory_Period.addItem("2 Bars", 2);
		dynTrajectory_Period.addItem("4 Bars", 4);
		dynTrajectory_Period.addItem("8 Bars", 8);
		dynTrajectory_Period.setSelectedId(1);

		// DYN TRAJECTORY MIRROR
		dynTrajectory_Mirror.setButtonText("Mirror L/R");
		dynTrajectory_Mirror.setColour(dynTrajectory_Mirror.buttonColourId, Colours::blue);

		// SST - Stand Angle Thresh
		sitStand_Thresh_Stand.setValue(15);
		sitStand_Thresh_Stand.setRange(5, 30);
		sitStand_Thresh_Stand.setNumDecimalPlacesToDisplay(1);
		sitStand_Thresh_Stand_Label.setText("Pre-Stand Angle", dontSendNotification);
		sitStand_Thresh_Stand_Label.attachToComponent(&sitStand_Thresh_Stand, false);
		
		// SST - Sit Angle Thresh
		sitStand_Thresh_Sit.setValue(10);
		sitStand_Thresh_Sit.setRange(5, 30);
		sitStand_Thresh_Sit.setNumDecimalPlacesToDisplay(1);
		sitStand_Thresh_Sit_Label.setText("Pre-Sit Angle", dontSendNotification);
		sitStand_Thresh_Sit_Label.attachToComponent(&sitStand_Thresh_Sit, false);

		// SST - Flip State
		sitStand_FlipState.setButtonText("Flip State");
		sitStand_FlipState.setVisible(false);

		// SST - Is Standing Label
		String isStandingText = "Sitting";
		sitStand_isStanding.setText(isStandingText, dontSendNotification);

		// HS - Timing Mode
		HS_TimingMode.addItem("Standard Time", 1);
		HS_TimingMode.addItem("Half Time", 2);
		HS_TimingMode.setSelectedId(1);

		// HS - Timing Tolerance
		HS_Tolerance.setValue(0, dontSendNotification);
		HS_Tolerance.setRange(0, 1);
		HS_Tolerance.setNumDecimalPlacesToDisplay(2);
		HS_Tolerance_Label.setText("Error Tolerance", dontSendNotification);
		HS_Tolerance_Label.attachToComponent(&HS_Tolerance, false);

		// HS - Acc Thresh
		HS_AccThresh.setValue(1.84, dontSendNotification);
		HS_AccThresh.setRange(0, 5);
		HS_AccThresh.setNumDecimalPlacesToDisplay(2);
		HS_AccThresh_Label.setText("HS ACC Threshold", dontSendNotification);
		HS_AccThresh_Label.attachToComponent(&HS_AccThresh, false);

		// REPS - RESTART
		rep_RESTART.setButtonText("Restart Exercise");
		rep_RESTART.setColour(rep_RESTART.buttonColourId, Colours::blue);

		// DYN TARGET FEEDBACK TYPE
		dynTarget_FB_TYPE.addItem("Zone", 1);
		dynTarget_FB_TYPE.addItem("2D Task Only", 2);
		dynTarget_FB_TYPE.addItem("2D Distance Error", 3);
		dynTarget_FB_TYPE.setSelectedId(1);

		// DYN TARGET FEEDBACK DATA
		dynTarget_FB_DATA.addItem("Position", 1);
		dynTarget_FB_DATA.addItem("Velocity", 2);
		dynTarget_FB_DATA.setSelectedId(1);
	}

	void toggleVisible(short exMode, bool soniTab)
	{
		bool isTesting = (exMode == 1);
		bool isSB_orDB = (exMode == 2 || exMode == 3);
		bool isDB = (exMode == 3);
		bool isSTS = (exMode == 4 || exMode == 5);
		bool isGait = (exMode == 6);
		bool isRep = (exMode > 2);

		// SB DB
		staticBalance_Div_Roll.setVisible(soniTab && isSB_orDB);
		staticBalance_Div_Pitch.setVisible(soniTab && isSB_orDB);
		staticBalance_FeedbackSlope.setVisible(soniTab && isSB_orDB);
		staticBalance_Ctrl_X.setVisible(soniTab && isSB_orDB);
		staticBalance_Ctrl_Y.setVisible(soniTab && isSB_orDB);

		// DB
		dynTrajectory_Shape.setVisible(soniTab && isDB);
		dynTrajectory_Mirror.setVisible(soniTab && isDB);
		dynTrajectory_Period.setVisible(soniTab && isDB);
		dynTrajectory_Radius.setVisible(soniTab && isDB);
		
		dynTarget_FB_TYPE.setVisible(soniTab && isSB_orDB);
		dynTarget_FB_DATA.setVisible(soniTab && isSB_orDB);
		dynTarget_FB_X.setVisible(soniTab && isSB_orDB);
		dynTarget_FB_Y.setVisible(soniTab && isSB_orDB);

		// STS
		sitStand_Thresh_Stand.setVisible(soniTab && isSTS);
		sitStand_Thresh_Sit.setVisible(soniTab && isSTS);
		sitStand_FlipState.setVisible(soniTab && isSTS);
		sitStand_isStanding.setVisible(soniTab && isSTS);

		// Gait
		HS_TimingMode.setVisible(soniTab && isGait);
		HS_Tolerance.setVisible(soniTab && isGait);
		HS_AccThresh.setVisible(soniTab && isGait);

		// Repetitions
		rep_NumDone.setVisible(soniTab && isRep);
		rep_AvgTime_LAST5.setVisible(soniTab && isRep);
		rep_AvgTime_TOTAL.setVisible(soniTab && isRep);
		rep_RESTART.setVisible(soniTab && isRep);
	}

	void setLayout()
	{
		staticBalance_Ctrl_X.setBounds(950, 310, 200, 20);
		staticBalance_Ctrl_Y.setBounds(930, 330, 20, 200);
		staticBalance_Div_Pitch.setBounds(920, 330, 20, 100);
		staticBalance_Div_Roll.setBounds(950, 300, 100, 20);

		sitStand_Thresh_Sit.setBounds(50, 110, 200, 40);
		sitStand_Thresh_Stand.setBounds(50, 190, 200, 40);
		sitStand_FlipState.setBounds(50, 270, 200, 40);
		sitStand_isStanding.setBounds(50, 350, 200, 40);

		dynTrajectory_Shape.setBounds(50, 110, 200, 40);
		dynTrajectory_Mirror.setBounds(50, 190, 200, 40);
		dynTrajectory_Period.setBounds(50, 270, 200, 40);
		dynTrajectory_Radius.setBounds(50, 350, 200, 40);

		HS_TimingMode.setBounds(50, 110, 200, 40);
		HS_Tolerance.setBounds(50, 190, 200, 40);
		HS_AccThresh.setBounds(50, 270, 200, 40);

		rep_NumDone.setBounds(930, 110, 200, 40);
		rep_AvgTime_TOTAL.setBounds(930, 150, 200, 40);
		rep_AvgTime_LAST5.setBounds(930, 190, 200, 40);
		rep_RESTART.setBounds(710, 430, 200, 40);

		dynTarget_FB_TYPE.setBounds(950, 580, 200, 30);
		dynTarget_FB_DATA.setBounds(950, 620, 200, 30);
		dynTarget_FB_X.setBounds(950, 660, 200, 30);
		dynTarget_FB_Y.setBounds(950, 700, 200, 30);
	}
};

