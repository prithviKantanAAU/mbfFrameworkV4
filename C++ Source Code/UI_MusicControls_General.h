#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
class UI_MusicControls_General
{
public:
	UI_MusicControls_General() : song_Progress(song_Progress_Val)
	{
		configure();
	};
	~UI_MusicControls_General() {};

	TextButton song_PlayPause;					//Toggle play and pause
	double song_Progress_Val;					//Song Progress Value (0 - 1)
	ProgressBar song_Progress;					//Bar indicating song progress
	Label song_TimeLeft;						//Indicates remaining time in seconds
	String fileExtensions[10]					//Accepted song file extensions
		= {"", "*.mid", "*.csv", "*.mid" };		
	TextButton song_Stop;						//Stop playback, reset transport
	Slider tempo_Slider;						//Tempo slider (old name: tempo)
	Label tempo_Slider_Lab;						//Tempo slider label (old name: tempoLabel)
	TextButton rhythm_SetNext;					//Change to next rhythm (old name: randomizeRhythm)
	Label song_Filename;						//Loaded song filename (old name: currentFile)
	Label rhythm_Prev;							//Previous Rhythm Name (old name: lastBeat)
	Label rhythm_Now;							//Active Rhythm Name (old name: activeBeat)
	Label rhythm_Next;							//Next Rhythm Name (old name: nextBeat)
	ComboBox music_Mode;						//Music Mode (External MIDI / Inbuilt)
	ComboBox inst_Variant[8];					//Change individual instrument Variant (old name: inst_Variant)
	Label inst_Variant_Lab[8];					//Variant Label (old name: inst_Variant_Label)
	Label song_BarBeat_Counter;					//Bar and beat real time counter (old name: musicCounter)
	TextButton song_LoadFile;					//Browse for valid song file (old name: fetchFile)
	ToggleButton song_track_Mute[8];			//Toggles individual track mute (old name: trackMute)
	Label song_track_Mute_Lab[8];				//Track mute checkbox label
	Slider song_track_GainOffset[8];			//Individual track gain offset slider (+/- 10 dB)
	Label song_track_GainOffset_Lab[8];			//Track gain offset dB label (old name: trackGainLabel)
	Slider song_master_Gain;					//Overall Master Gain Slider (old name: masterGain)
	Label song_master_Gain_Lab;					//Overall Master Gain dB Label (old name: masterGainLabel)
	String inst_Names[8] =						//Intrument Names (old name: instVariant_Names)
	{
		"Kick",
		"Snare",
		"HiHat",
		"Chords",
		"Bass",
		"Melody",
		"Hi-Chord",
		"Crash"
	};
	Slider song_master_EQ_B1_F;					//Master EQ Band 1 Frequency (old name masterEQ_1_F)
	Label song_master_EQ_B1_F_Lab;				//Frequency Value in Hz (old name masterEQ_1_F_Label)
	Slider song_master_EQ_B1_G;					//Master EQ Band 1 Gain (old name masterEQ_1_Gain)
	Label song_master_EQ_B1_G_Lab;				//Gain Value in dB (old name masterEQ_1_Gain_Label)
	Slider song_master_EQ_B2_F;					//Master EQ Band 2 Frequency (old name masterEQ_2_F)
	Label song_master_EQ_B2_F_Lab;				//Frequency Value in Hz (old name masterEQ_2_F_Label)
	Slider song_master_EQ_B2_G;					//Master EQ Band 2 Gain (old name masterEQ_2_Gain)
	Label song_master_EQ_B2_G_Lab;				//Gain Value in dB (old name masterEQ_2_Gain_Label)
	TextButton tempo_Tap;						//Tap Tempo Manually (old name tapTempo)
	TextButton showControls_Channel;			//Press To Show Channel Controls
	TextButton showControls_RingVisualize;		//Press To Show Ring Visualize Controls

	void configure()
	{
		// Song Play - pause toggle
		song_PlayPause.setButtonText("Play / Pause");

		// Song stop
		song_Stop.setButtonText("Stop");

		// Song progress bar
		song_Progress.setColour(song_Progress.backgroundColourId, Colours::darkblue);
		song_Progress.setColour(song_Progress.foregroundColourId, Colours::darkred);
		song_Progress.setPercentageDisplay(false);

		// Tap Tempo
		tempo_Tap.setButtonText("Tap Tempo");

		// Load Song File
		song_LoadFile.setButtonText("Load \nMIDI");
		song_LoadFile.setColour(song_LoadFile.buttonColourId,Colours::red);
		song_Filename.setColour(song_Filename.backgroundColourId, Colours::green);
		
		// Set Next Rhythm
		rhythm_SetNext.setButtonText("Next Rhythm");
		
		// Tempo Slider
		tempo_Slider.setRange(60, 150);
		tempo_Slider.setNumDecimalPlacesToDisplay(0);
		tempo_Slider.setColour(tempo_Slider.trackColourId, Colours::blue);
		tempo_Slider.setValue(120);
		tempo_Slider_Lab.setText("Tempo", dontSendNotification);
		tempo_Slider_Lab.attachToComponent(&tempo_Slider, true);

		// Song Master Gain
		song_master_Gain.setRange(-96, 0);
		song_master_Gain.setSkewFactor(3);
		song_master_Gain.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
		song_master_Gain.setColour(song_master_Gain.trackColourId, Colours::red);
		song_master_Gain.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 10, 10);
		song_master_Gain.setNumDecimalPlacesToDisplay(1);
		song_master_Gain_Lab.setText("Vol", dontSendNotification);
		song_master_Gain_Lab.attachToComponent(&song_master_Gain, true);

		// Set Music Mode
		music_Mode.addItem("MIDI File", 1);
		music_Mode.addItem("Inbuilt", 3);
		music_Mode.setSelectedId(1);

		for (int i = 0; i < 8; i++)					
		{
			// Set Instrument Variant Head Labels
			inst_Variant_Lab[i].setText(inst_Names[i], dontSendNotification);
			inst_Variant_Lab[i].attachToComponent(&inst_Variant[i], true);

			// Set Initial Track Mute Setting
			song_track_Mute[i].setToggleState(false, dontSendNotification);
			song_track_Mute_Lab[i].setText(std::to_string(i + 1), dontSendNotification);
			song_track_Mute_Lab[i].attachToComponent(&song_track_Mute[i], true);

			// Set Track Gain Offset
			song_track_GainOffset[i].setRange(-10, 10);
			song_track_GainOffset[i].setValue(0);
			song_track_GainOffset[i].setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
			song_track_GainOffset[i].setColour(song_track_GainOffset[i].trackColourId, Colours::yellow);
			song_track_GainOffset[i].setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 10, 10);
			song_track_GainOffset_Lab[i].setText(std::to_string(i + 1), dontSendNotification);
			song_track_GainOffset_Lab[i].attachToComponent(&song_track_GainOffset[i], false);
		}

		//Master EQ F1
		song_master_EQ_B1_F.setRange(20, 20000);
		song_master_EQ_B1_F.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
		song_master_EQ_B1_F.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 10, 10);
		song_master_EQ_B1_F.setColour(song_master_EQ_B1_F.trackColourId, Colours::blue);
		song_master_EQ_B1_F.setValue(200);
		song_master_EQ_B1_F.setSkewFactor(0.3, false);
		song_master_EQ_B1_F_Lab.setText("M1F", dontSendNotification);
		song_master_EQ_B1_F_Lab.attachToComponent(&song_master_EQ_B1_F, true);

		//Master EQ F2
		song_master_EQ_B2_F.setRange(20, 20000);
		song_master_EQ_B2_F.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
		song_master_EQ_B2_F.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 10, 10);
		song_master_EQ_B2_F.setColour(song_master_EQ_B2_F.trackColourId, Colours::blue);
		song_master_EQ_B2_F.setValue(2000);
		song_master_EQ_B2_F.setSkewFactor(0.3, false);
		song_master_EQ_B2_F_Lab.setText("M2F", dontSendNotification);
		song_master_EQ_B2_F_Lab.attachToComponent(&song_master_EQ_B2_F, true);

		//Master EQ G1
		song_master_EQ_B1_G.setRange(-24, 24);
		song_master_EQ_B1_G.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
		song_master_EQ_B1_G.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 10, 10);
		song_master_EQ_B1_G.setColour(song_master_EQ_B1_G.trackColourId, Colours::blue);
		song_master_EQ_B1_G.setValue(0);
		song_master_EQ_B1_G_Lab.setText("M1G", dontSendNotification);
		song_master_EQ_B1_G_Lab.attachToComponent(&song_master_EQ_B1_G, true);

		//Master EQ G2 - Move all but Lambda to subclass
		song_master_EQ_B2_G.setRange(-24, 24);
		song_master_EQ_B2_G.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
		song_master_EQ_B2_G.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 10, 10);
		song_master_EQ_B2_G.setColour(song_master_EQ_B2_G.trackColourId, Colours::blue);
		song_master_EQ_B2_G.setValue(0);
		song_master_EQ_B2_G_Lab.setText("M2G", dontSendNotification);
		song_master_EQ_B2_G_Lab.attachToComponent(&song_master_EQ_B2_G, true);

		//Show Channel Settings
		showControls_Channel.setButtonText("Channel Controls");
		showControls_Channel.setColour(showControls_Channel.buttonColourId, Colours::blue);

		//Show Ring Visualize
		showControls_RingVisualize.setButtonText("Music Visualize");
		showControls_Channel.setColour(showControls_Channel.buttonColourId, Colours::red);

	}

	void toggleVisible(bool on)
	{
		song_PlayPause.setVisible(on); 
		song_Stop.setVisible(on);
		tempo_Slider.setVisible(on); 
		song_Filename.setVisible(on);
		rhythm_SetNext.setVisible(on); 
		song_Progress.setVisible(on);
		song_TimeLeft.setVisible(on);	
		music_Mode.setVisible(on);
		song_LoadFile.setVisible(on); 
		tempo_Tap.setVisible(on);
		song_master_Gain.setVisible(on);	
		rhythm_Now.setVisible(on);
		rhythm_Prev.setVisible(on);	
		rhythm_Next.setVisible(on);
		song_master_EQ_B1_F.setVisible(on);	
		song_master_EQ_B2_F.setVisible(on);
		song_master_EQ_B1_G.setVisible(on);	
		song_master_EQ_B2_G.setVisible(on);
		song_BarBeat_Counter.setVisible(on);
		for (int i = 0; i < 8; i++)
		{
			song_track_GainOffset[i].setVisible(on);
			song_track_Mute[i].setVisible(on);
			inst_Variant[i].setVisible(on);
		}
		showControls_Channel.setVisible(on);
		showControls_RingVisualize.setVisible(on);
	}

	void setLayout()
	{
		song_PlayPause.setBounds(50, 50, 100, 30);
		song_Stop.setBounds(150, 50, 100, 30);
		tempo_Tap.setBounds(250, 50, 100, 30);
		tempo_Slider.setBounds(410, 50, 200, 30);
		song_Progress.setBounds(50, 250, 1100, 30);
		music_Mode.setBounds(50, 305, 200, 40);
		song_TimeLeft.setBounds(1000, 280, 200, 30);
		song_BarBeat_Counter.setBounds(800, 280, 200, 30);
		rhythm_SetNext.setBounds(50, 90, 100, 30);
		song_Filename.setBounds(900, 30, 300, 25);
		song_LoadFile.setBounds(1020, 70, 170, 150);
		rhythm_Prev.setBounds(50, 130, 100, 25);
		rhythm_Now.setBounds(30, 160, 140, 25);
		rhythm_Next.setBounds(50, 190, 100, 25);
		song_master_Gain.setBounds(680, 50, 50, 180);
		song_master_EQ_B1_F.setBounds(780, 50, 30, 180);
		song_master_EQ_B1_G.setBounds(840, 50, 30, 180);
		song_master_EQ_B2_F.setBounds(900, 50, 30, 180);
		song_master_EQ_B2_G.setBounds(960, 50, 30, 180);

		for (int i = 0; i < 8; i++)
		{
			int horizPos = 210 + 50 * i;
			song_track_Mute[i].setBounds(horizPos, 90, 50, 30);
			song_track_GainOffset[i].setBounds(horizPos - 3, 150, 35, 80);
			inst_Variant[i].setBounds(50, 355 + 50 * i, 200, 40);
		}

		showControls_RingVisualize.setBounds(550, 290, 200, 25);
		showControls_Channel.setBounds(750, 290, 200, 25);
	}
};

