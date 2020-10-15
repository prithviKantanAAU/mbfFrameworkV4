#pragma once
#define barsInMelMeasure 4
#define FLT_MIN_PLUS          1.175494351e-38         /* min positive value */

#include "../JuceLibraryCode/JuceHeader.h"
#include "DspFaust.h"
#include "MusicInfoRead.h"
#include "MidiTrack_Drum.h"
#include "MusicPhaseCalc.h"
#include "ScaleTonicTrans.h"
#include "mixerSettings.h"
#include "FaustStrings.h"

class Sequencer
{
public:
	Sequencer();
	~Sequencer();
	MusicPhaseCalc musicPhase;
	ScaleTonicTrans scaleTonicTrans;
	MusicInfoRead currentMusic;
	MidiTrack_Drum* percObj;
	Random randGen;
	DspFaust dspFaust;
	MixerSettings mixerSettings;
	FaustStrings faustStrings;
	
	void resetCounters();
	short musicMode = 1;
	bool isFileLoaded = false;
	short index_baseBeat = 0;
	
	// FETCH RANDOM INDEX IN INTEGER RANGE SPECIFIED
	int fetchNewRandomIndex(int range)
	{
		return randGen.nextInt(range - 1);
	}

	// LOAD NEW MIDI SONG FILE
	void loadNewFile_MIDI(String name)
	{
		currentMusic.loadMidiFile(name);
		if (!isFileLoaded)
			isFileLoaded = true;
	}

	// BEAT AND BAR COUNTERS
	int pulsesElapsed = -1;	
	int beatsElapsed = -1; 	
	int barsElapsed = -1;
	int beatsElapsed_withinBar = 0;

	// SET RHYTHM TIMING MODE DEPENDING ON SONG NAME
	void setTimingMode(String path)
	{
		if (path.contains("3by4"))
		{
			timingMode = 2;
			isTripletMode = false;
			is3by4Mode = true;
		}
		else if (path.contains("Triplet"))
		{
			timingMode = 1;
			isTripletMode = true;
			is3by4Mode = false;
		}
		else
		{
			timingMode = 0;
			isTripletMode = false;
			is3by4Mode = false;
		}
		// SET SELECTED RHYTHM TO FIRST CHOICE WITH APPROPRIATE TIMING MODE
		for (int i = 0; i < currentMusic.drum_numBase; i++)
		{
			if (currentMusic.drum_beatTypes[i] == timingMode)
			{
				index_baseBeat = i;
				break;
			}
		}
	}

	// TIMING MODE
	int ticksPerMeasure = 15360;
	short beatsInBar_TimingMode[10] = { 4, 4, 3 };
	short timingMode = 0;
	bool is3by4Mode = false;
	bool isTripletMode = false;
	float tapTempoCounter = 0;									
	bool isPlaying = false;										
	float timeElapsed_Song = 0;									
	float timeLeft_Song = 0;									
	float timeTotal_Song = 0;
	float tempo = 120;
	float timeElapsed_SONG = 0.0;								// PLAYBACK Time Elapsed
	
	double lastPulseTime = 0.0;									// Last 16th Pulse Time
	double nextPulseTime = 0.0;									// Next 16th Pulse Time
	double ticksPerMS = 0.0;									// MIDI Ticks per ms
	long double midiTicksElapsed = 0.0;								// #MIDI Ticks elapsed
	int midiTickIncrement = 240;								// MIDI Tick Increment per 16th note
	double songProgress = 0;									// Song Process Fraction -> Sequencer

	// SEQUENCER SONIFICATION
	float AP_Val = 0;
	float AP_Val_2D_X = 0;
	float AP_Val_2D_Y = 0;
	String soni_AP_Name = "";

	//MIDI
	short numTracks = 8;
	float vels[4][8] = { 0.0 };
	bool note_isOn[4][8] = { false };
	bool isPitched[8] = 
	{ 
		false,												// KICK
		false,												// SNARE
		false,												// HH
		true,												// CHORD MAIN
		true,												// BASSLINE
		true,												// MAIN MELODY
		true,												// CHORD HIGH
		false												// CRASH
	};
	int pitches[4][8] = { 0 };
	bool isVel_FromSongFile[8] =
	{
		false,
		false,
		false,
		false,
		false,
		true,
		false,
		false
	};
	short trackIdx_to_midiTrack_map[8] = { -1, -1, -1, 1, 2, 0, 1, -1 };

	bool isNewEvents_ToHandle[8] = {false};
	bool isNewEvents_PitchOnly[8] = { false };
	void check_Handle_New_MIDIEvents()
	{
		for (int i = 1; i <= numTracks; i++)
			checkNew_MIDIEvents_SINGLE(i);
		mapNew_MIDIEvents();
	}
	void checkNew_MIDIEvents_SINGLE(int trackIndex);
	void mapNew_MIDIEvents();
	bool infoMapped_CurrentPulse_MIDI = false;

	// INCREMENT BAR AND BEAT COUNTERS
	void incrementPulseCounter();
	
	// RESET MIDI PLAYBACK (EVENT COUNTERS TO ZERO)
	void resetMidiPlayback()
	{
		for (int i = 0; i < 4; i++)
		{
			nextVoiceIndex[i] = 0;
			currentMusic.midiTracks[i].resetPlayback();
		}
	};
	
	// MIDI EVENT HANDLING
	short nextVoiceIndex[8] = { 0 };
	double nextEventTimeStamp[8] = { 0 };

	// UPDATE CHOSEN RHYTHM WITH NEXT
	void nextRhythm(short musicMode) 
	{
		currentMusic.baseBeats[index_baseBeat].flush_nextEventIndices();
		index_baseBeat = currentMusic.getNextBeat(index_baseBeat, timingMode);
	};

	// SCALE AND TONIC TRANSFORMATIONS
	short scaleID_ORIG = 0;
	short scaleID_TRANS = 0;
	short tonicOffset_ORIG = 0;
	short tonicOffset_TRANS = 0;
	String cue_AP_Name = "";
	String X_2D_AP_Name = "";
	String Y_2D_AP_Name = "";

	// OCTAVE LIMIT MIDI NOTE WITHIN TWO BOUNDS
	int midiNoteLimit(short prelimValue, short lowerlim_MIDI, short upperlim_MIDI)
	{
		bool tooLow = lowerlim_MIDI > prelimValue; short tooLow_num = tooLow ? 1 : 0;
		bool tooHigh = upperlim_MIDI < prelimValue; short tooHigh_num = tooHigh ? 1 : 0;
		short octavesUnder = ((lowerlim_MIDI - prelimValue)*tooLow_num / 12) + tooLow_num;
		short octavesOver = ((prelimValue - upperlim_MIDI)*tooHigh_num / 12) + tooHigh_num;
		return prelimValue + 12 * octavesUnder - 12 * octavesOver;
	};

	// RESET PERC MIDI OBJECTS WHEN RHYTHM CHANGED - ADAPTS NEXT EVENT INDICES
	void resetPercMIDIOnChange(double midiTicksElapsed)
	{
		// FIND MOD MIDI TICKS ELAPSED
		double midiTicksElapsed_MOD = midiTicksElapsed - (int)((int)midiTicksElapsed / ticksPerMeasure) * ticksPerMeasure;
		
		// RESET NEXT EVENT INDICES TO ZERO
		currentMusic.baseBeats[index_baseBeat].flush_nextEventIndices();
		int eventIdx_Present_Trackwise = 0;

		for (int h = 0; h < numTracks; h++)
		{	// i REPRESENTS IDX OF TRACKWISE EVENTS
			for (int i = 0; i < currentMusic.baseBeats[index_baseBeat].eventCount_ByTrack[h]; i++)
			{
				eventIdx_Present_Trackwise = currentMusic.baseBeats[index_baseBeat].eventIdx_ByTrack_ALL[i][h];
				if (midiTicksElapsed_MOD < currentMusic.baseBeats[index_baseBeat].infoMatrix[eventIdx_Present_Trackwise][3])
				{
					currentMusic.baseBeats[index_baseBeat].eventIdx_ByTrack_NEXT[h] = i;
					break;
				}
			}
		}
	};

	// COOK LOADED MIDI VELOCITY TO DSPFAUST COMPATIBLE
	float cookMIDIVel(float midiVel, short trackIndex, String APName)
	{
		float output = 0;
		if (midiVel < 64)
			output = fmin(0 + 3 * midiVel / 64.0, 2.999);
		else if (midiVel < 96)
			output = fmin(3 + 3 * (midiVel - 64) / 32.0, 5.999);
		else
			output = fmin(6 + 3 * (midiVel - 96) / 32.0, 9);

		return output;
	};

	//Toggle Mute Status
	short muteValues[8] = { 0 };

	// ARRANGE PITCHES ASCENDING
	void arrangePitches_Asc(int trackIndex)
	{	
		int temp[4] = { 0 };
		for (int i = 0; i < 4; i++)
			temp[i] = pitches[i][trackIndex];
		std::sort(temp, temp + currentMusic.numVoices[trackIndex]);
		for (int i = 0; i < 4; i++)
			pitches[i][trackIndex] = temp[i];
	}

	void copyPitches_ToDependentTracks(int trackIndex)
	{
		int targetTrackId = trackIdx_to_midiTrack_map[trackIndex];
		int trackVariant = 0;

		for (int i = 0; i < numTracks; i++)
		{
			trackVariant = currentMusic.baseBeats[index_baseBeat].variantConfig[i] - 1;
			if (i != trackIndex && trackIdx_to_midiTrack_map[i] == targetTrackId)
			{
				for (int j = 0; j < currentMusic.numVoices[trackIndex]; j++)
					pitches[j][i] = midiNoteLimit(pitches[j][trackIndex],
						mixerSettings.var_noteMins[trackVariant][j][i],
						mixerSettings.var_noteMaxs[trackVariant][j][i]);
			}
		}
	}

	// DSPFAUST RELATED
	//Start DSPFaust, Initialize Gain -> Sequencer
	void onStartMusic()
	{
		dspFaust.start();
		initializeTracksForPlayback();
	}
	void initializeTracksForPlayback();
	void applyCurrentVariantGain(int trackIndex);
	void setTrackGains(int trackIndex, float value);
	void setTrackMutes(int trackIndex, int value);
	void applyCurrentVariantEQ(int trackIndex);
	void applyCurrentVariantComp(int trackIndex);
	//Set Master Gain -> Sequencer		
	void applyMasterGain(float value)
	{
		mixerSettings.masterGain = value;
		std::string address = faustStrings.baseName + faustStrings.MasterVol;
		dspFaust.setParamValue(address.c_str(), value);
	}
	void switchInstVariant(int trackIndex, int newVariant);
	void setFilename(String name);
	bool handleTapTempoPress();
	void togglePlayPause();
	void stopMusic();
	// Song Completion Fraction -> Sequencer
	bool getSongProgress()
	{
		switch (musicMode)										// Depends on Music Mode
		{
		case 1:
			songProgress = midiTicksElapsed / currentMusic.finalTimeStamp;
			timeLeft_Song = (currentMusic.finalTimeStamp - midiTicksElapsed) / (1000 * ticksPerMS);
			break;
		case 2:
			songProgress = pulsesElapsed / 1536.0;
			timeLeft_Song = timeTotal_Song * (1 - songProgress);
			break;
		case 3:
			songProgress = midiTicksElapsed / currentMusic.finalTimeStamp;
			timeLeft_Song = (currentMusic.finalTimeStamp - midiTicksElapsed) / (1000 * ticksPerMS);
			break;
		}
		return songProgress >= 1 ? true : false;
	}
};