#include "Sequencer.h"

// CONSTRUCTOR - INITIALIZE
Sequencer::Sequencer()
{
	resetCounters();
}

// DESTRUCTOR
Sequencer::~Sequencer()
{}

// RESET TIMING COUNTERS AND EVENT COUNTERS
void Sequencer::resetCounters()
{
	pulsesElapsed = 0;
	beatsElapsed = 0;
	barsElapsed = 0;
	beatsElapsed_withinBar = 0;
}

// INCREMENT SIXTEENTH PULSE COUNTER, CHECK BEAT AND BAR COMPLETION
void Sequencer::incrementPulseCounter()
{
	// CHECK BEATS IN BAR, INCREMENT SIXTEENTH PULSE COUNTER
	int beatsInBar = beatsInBar_TimingMode[timingMode];
	pulsesElapsed++;
	
	// CHECK IF BEAT COMPLETED
	if ((pulsesElapsed - 1) % 4 == 0)
	{
		beatsElapsed++;
		beatsElapsed_withinBar = beatsElapsed % beatsInBar;
	}

	// CHECK IF BAR COMPLETED
	if ((pulsesElapsed - 1) % 16 == 0)
	{
		barsElapsed++;
		musicPhase.resetPhase();
		musicPhase.isEvenBar = (barsElapsed % 2 == 1);
	}
}

// FIRST TIME INITIALIZE MUTES, VARIANTS, MASTER GAIN, MAP
void Sequencer::initializeTracksForPlayback()
{
	for (int i = 0; i < 8; i++)
	{
		setTrackMutes(i, muteValues[i]);
		switchInstVariant(i, currentMusic.baseBeats[index_baseBeat].variantConfig[i]);

	}
	applyMasterGain(mixerSettings.masterGain);
	dspFaust.setParamValue(faustStrings.MasterEQ_1_Q.c_str(), 0.7);
	dspFaust.setParamValue(faustStrings.MasterEQ_2_Q.c_str(), 0.7);
}

// MAP CURRENT VARIANT GAIN - SINGLE TRACK
void Sequencer::applyCurrentVariantGain(int trackIndex)
{
	std::string address = "";
	float gain = 0;
	short variantNum = currentMusic.baseBeats[index_baseBeat].variantConfig[trackIndex] - 1;
	address = faustStrings.getTrackGainString(trackIndex);
	gain = mixerSettings.trackGains[trackIndex][variantNum]
		+ currentMusic.baseBeats[index_baseBeat].variantConfig_GAINS[trackIndex];
	dspFaust.setParamValue(address.c_str(), gain);
}

// APPLY AND MAP CURRENT VARIANT GAIN - SINGLE TRACK (on Variant Change)
void Sequencer::setTrackGains(int trackIndex, float value)
{
	currentMusic.baseBeats[index_baseBeat].variantConfig_GAINS[trackIndex] = value;
	applyCurrentVariantGain(trackIndex);
}

// APPLY AND MAP MUTE VALUE - SINGLE TRACK
void Sequencer::setTrackMutes(int trackIndex, int value)
{
	muteValues[trackIndex] = value;
	std::string address = faustStrings.getTrackMuteString(trackIndex);
	dspFaust.setParamValue(address.c_str(), muteValues[trackIndex]);
}

// APPLY AND MAP VARIANT EQ - SINGLE TRACK (on Variant Change)
void Sequencer::applyCurrentVariantEQ(int trackIndex)
{
	short variantNum = currentMusic.baseBeats[index_baseBeat].variantConfig[trackIndex] - 1;
	float value = 0;
	bool toMap = true;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			std::string address = faustStrings.FetchEQ_String(trackIndex, i, j);
			value = mixerSettings.fetchEQValue(variantNum, trackIndex, i, j);
			dspFaust.setParamValue(address.c_str(), value);
		}
	}
}

// APPLY AND MAP VARIANT COMP - SINGLE TRACK (on Variant Change)
void Sequencer::applyCurrentVariantComp(int trackIndex)
{
	std::string address = "";
	float value = 0;
	int currentRhythm = index_baseBeat;
	short variantNum = currentMusic.baseBeats[index_baseBeat].variantConfig[trackIndex] - 1;
	for (int j = 0; j < 4; j++)		//Param ID
	{
		address = faustStrings.FetchComp_String(trackIndex, j);
		value = mixerSettings.fetchCompValue(variantNum, trackIndex, j);
		dspFaust.setParamValue(address.c_str(), value);
	}
}

// SWITCH INSTRUMENT VARIANT - SINGLE TRACK
void Sequencer::switchInstVariant(int trackIndex, int newVariant)
{
	// UPDATE MIXER SETTINGS WITH NEW SETTING
	int currentRhythm = index_baseBeat;
	currentMusic.baseBeats[index_baseBeat].variantConfig[trackIndex] = newVariant;
	//MAP VARIANT TO FAUST
	// CHANNEL SETTINGS
	applyCurrentVariantGain(trackIndex);
	applyCurrentVariantComp(trackIndex);
	applyCurrentVariantEQ(trackIndex);
	// VARIANT NUMBER
	std::string address = faustStrings.baseName + faustStrings.trackVariant_Base + std::to_string(trackIndex + 1);
	dspFaust.setParamValue(address.c_str(), currentMusic.baseBeats[index_baseBeat].variantConfig[trackIndex]);
}

// LOAD MIDI SONG FILE
void Sequencer::setFilename(String name)
{
	stopMusic();
	loadNewFile_MIDI(name);									// Load MIDI Song File from PATH
	togglePlayPause();
}

// HANDLE TAP TEMPO BUTTON PRESS
bool Sequencer::handleTapTempoPress()
{
	float newTempo = 0;
	if (tapTempoCounter >= 1.0)
	{
		tapTempoCounter = 0;
		return false;
	}
	else
	{
		newTempo = 60 / tapTempoCounter;
		tempo = newTempo;
		tapTempoCounter = 0;
		return true;
	}
}

// PAUSE OR PLAY IF FILE HAS BEEN LOADED
void Sequencer::togglePlayPause()
{
	if (isPlaying)
	{
		dspFaust.stop();
		isPlaying = false;
	}
	else
	{
		if (isFileLoaded)
		{
			onStartMusic();
			isPlaying = true;
		}
	}
}

// STOP MUSIC, RESET COUNTERS AND TIME
void Sequencer::stopMusic()
{
	if (isPlaying)
	{
		isPlaying = false;
		dspFaust.stop();
		resetCounters();
		resetMidiPlayback();
		midiTicksElapsed = 0;
		timeElapsed_SONG = 0;
		nextPulseTime = 0;
		songProgress = 0;
		musicPhase.resetPhase();
		currentMusic.baseBeats[index_baseBeat].flush_nextEventIndices();
	}
}

// CHECK NEW MIDI EVENTS FOR SINGLE TRACK
void Sequencer::checkNew_MIDIEvents_SINGLE(int trackIndex)
{
	// GET MIXER INFO
	int trackVariant = currentMusic.baseBeats[index_baseBeat].variantConfig[trackIndex - 1];
	short targetTrackIdx = 0;

	// DECREMENT TRACK INDEX AND INITIALIZE EVENT COUNT
	trackIndex--;
	int numEvents_toHandle = 0;

	// DEFINE TICK INTERVAL TO CHECK FOR NEW EVENTS
	double timeStamp_IntervalStart = midiTicksElapsed - ticksPerMS;
	double timeStamp_IntervalEnd = midiTicksElapsed;

	// DEFINE TICK INTERVAL FOR NEW LOOPED EVENTS
	double timeStamp_IntervalEnd_MOD = midiTicksElapsed
		- (int)(midiTicksElapsed / ticksPerMeasure) * ticksPerMeasure;
	double timeStamp_IntervalStart_MOD = timeStamp_IntervalEnd_MOD - ticksPerMS;

	// SET TICKS PER BAR DEPENDING ON TIME SIGNATURE 4/4 OR 3/4
	ticksPerMeasure = timingMode != 2 ? 16 * currentMusic.midi_ticksPerBeat : 12 * currentMusic.midi_ticksPerBeat;

	// INITIALIZE EVENT INDICES
	int nextEventIndex = 0;
	int finalEventIndex = 0;
	double eventTimeStamp = 0;
	bool isValidNote = false;

	// INITIALIZE VOICE VALUES
	short voiceToTurnOff = 0;

	// INITIALIZE ORIGINAL AND TRANSFORMED MIDI KEY NUMBER
	int originalKey = 0;
	int transformedKey = 0;

	// INITIALIZE MISC
	float velUncooked = 0;
	percObj = &currentMusic.baseBeats[index_baseBeat];
	int eventIdx_LOOP_Trackwise = 0;

	// PITCHED TRACKS - TAKE CARE OF PITCH SETTING, NOTE ON AND NOTE OFF WHERE NEEDED
	if (isPitched[trackIndex])
	{
		// FETCH NEXT AND FINAL INDICES - MAP TRACK IDX TO MIDI TRACK IDX
		nextEventIndex = currentMusic.midiTracks[trackIdx_to_midiTrack_map[trackIndex]].Idx_nextEvent;
		finalEventIndex = currentMusic.midiTracks[trackIdx_to_midiTrack_map[trackIndex]].numEvents;

		// CHECK ALL EVENTS - DO THEY FIT WITHIN BOUNDS? ANYTHING TO HANDLE?
		for (int i = nextEventIndex; i < finalEventIndex; i++)
		{
			// FETCH NEXT EVENT TIME STAMP
			eventTimeStamp = currentMusic.midiTracks[trackIdx_to_midiTrack_map[trackIndex]].infoMatrix[i][3];

			// CHECK IF IT LIES WITHIN BOUNDS
			if (eventTimeStamp >= timeStamp_IntervalStart && eventTimeStamp < timeStamp_IntervalEnd)
				numEvents_toHandle++;

			// IF IT EXCEEDS INTERVAL END, EXIT FOR LOOP
			if (eventTimeStamp >= timeStamp_IntervalEnd) break;
		}

		// IF NOTHING TO HANDLE, LEAVE FIRST PART. IF SOMETHING, THEN SET FLAG TRUE
		if (numEvents_toHandle == 0)
		{
			isNewEvents_ToHandle[trackIndex] = false;
		}
		else
		{
			isNewEvents_ToHandle[trackIndex] = true;
		}

		// IF THERE ARE PITCH / FILE VEL EVENTS TO HANDLE
		if (isNewEvents_ToHandle[trackIndex] == true)
		{
			// INITIALIZE VELOCITY, VOICE TO TURN OFF, NEXT VOICE INDEX FOR PRESENT TRACK
			velUncooked = 0;
			voiceToTurnOff = 0;

			// HANDLE ALL EVENTS THAT ARE TO BE HANDLED
			for (int j = nextEventIndex; j < nextEventIndex + numEvents_toHandle; j++)
			{
				// IF EVENT IS A NOTE ON EVENT
				if (currentMusic.midiTracks[trackIdx_to_midiTrack_map[trackIndex]].infoMatrix[j][0] == 1)
				{
					// HANDLE PITCH INFO
					switch (musicMode)
					{
					case 1:
						// LIMIT PITCHES AND STORE IN PITCH MATRIX
						targetTrackIdx = trackIdx_to_midiTrack_map[trackIndex];
						pitches[nextVoiceIndex[trackIndex]][trackIndex] = midiNoteLimit(
							currentMusic.midiTracks[trackIdx_to_midiTrack_map[trackIndex]].infoMatrix[j][1],
							mixerSettings.var_noteMins[trackVariant - 1][nextVoiceIndex[trackIndex]][trackIndex],
							mixerSettings.var_noteMaxs[trackVariant - 1][nextVoiceIndex[trackIndex]][trackIndex]
						);
						copyPitches_ToDependentTracks(trackIndex);
						break;
					case 2:
						break;
					case 3:
						// ADD TRANSFORMATION
						originalKey = currentMusic.midiTracks
							[trackIdx_to_midiTrack_map[trackIndex]].infoMatrix[j][1];
						transformedKey = scaleTonicTrans.transform_T1S1_TO_T2S2(originalKey,
							tonicOffset_ORIG, scaleID_ORIG, tonicOffset_TRANS, scaleID_TRANS,
							trackIndex, cue_AP_Name,X_2D_AP_Name,Y_2D_AP_Name,soni_AP_Name,AP_Val,AP_Val_2D_X,AP_Val_2D_Y);

						targetTrackIdx = trackIdx_to_midiTrack_map[trackIndex];
						for (int l = 0; l < numTracks; l++)
						{
							if (trackIdx_to_midiTrack_map[l] == targetTrackIdx)
								pitches[nextVoiceIndex[trackIndex]][l] = midiNoteLimit(
									transformedKey,
									mixerSettings.var_noteMins[trackVariant - 1][nextVoiceIndex[trackIndex]][l],
									mixerSettings.var_noteMaxs[trackVariant - 1][nextVoiceIndex[trackIndex]][l]
								);
						}

						// LIMIT PITCHES AND STORE IN PITCH MATRIX
						pitches[nextVoiceIndex[trackIndex]][trackIndex] = midiNoteLimit(
							transformedKey,
							mixerSettings.var_noteMins[trackVariant - 1][nextVoiceIndex[trackIndex]][trackIndex],
							mixerSettings.var_noteMaxs[trackVariant - 1][nextVoiceIndex[trackIndex]][trackIndex]);
						break;
					}

					// IF VELOCITY IS FROM SONG FILE, TAKE IT FROM HERE 
					if (isVel_FromSongFile[trackIndex])
					{
						vels[nextVoiceIndex[trackIndex]][trackIndex] =
							cookMIDIVel(currentMusic.midiTracks[trackIdx_to_midiTrack_map[trackIndex]].
								infoMatrix[j][2], trackIndex, cue_AP_Name);
					}
					nextVoiceIndex[trackIndex] = (nextVoiceIndex[trackIndex] + 1) % currentMusic.numVoices[trackIndex];
				}

				// IF EVENT IS A NOTE OFF EVENT
				if (currentMusic.midiTracks[trackIdx_to_midiTrack_map[trackIndex]].infoMatrix[j][0] == 2)
				{
					// IF VELOCITY IS FROM SONG FILE, TAKE IT FROM HERE
					if (isVel_FromSongFile[trackIndex])
					{
						for (int k = 0; k < currentMusic.numVoices[trackIndex]; k++)
						{
							if (pitches[k][trackIndex] == currentMusic.
								midiTracks[trackIdx_to_midiTrack_map[trackIndex]].infoMatrix[j][1])
								voiceToTurnOff = k;
							vels[voiceToTurnOff][trackIndex] = 0;
						}
					}
				}

				currentMusic.midiTracks[trackIdx_to_midiTrack_map[trackIndex]].incrementEventsHandled();				// INCREMENT EVENT
			}
		}
	}

	// ALL LOOPED TRACKS - TAKE CARE OF VELOCITY
	if (!isVel_FromSongFile[trackIndex])
	{
		nextEventIndex = percObj->eventIdx_ByTrack_NEXT[trackIndex];
		finalEventIndex = percObj->eventCount_ByTrack[trackIndex] - 1;
		numEvents_toHandle = 0;

		for (int i = nextEventIndex; i <= finalEventIndex; i++)
		{
			eventIdx_LOOP_Trackwise = percObj->eventIdx_ByTrack_ALL[i][trackIndex];
			eventTimeStamp = percObj->infoMatrix[eventIdx_LOOP_Trackwise][3];
			if (eventTimeStamp >= timeStamp_IntervalStart_MOD && eventTimeStamp < timeStamp_IntervalEnd_MOD)
					numEvents_toHandle++;
			if (eventTimeStamp >= timeStamp_IntervalEnd_MOD)
					break;
		}

		// RETURN IF ZERO EVENTS TO HANDLE - EXIT FUNCTION
		if (numEvents_toHandle == 0)
		{
			isNewEvents_ToHandle[trackIndex] = false;
			return;
		}
		else isNewEvents_ToHandle[trackIndex] = true;

		// IF LOOPED EVENTS TO HANDLE, THEN HANDLE THEM
		if (isNewEvents_ToHandle[trackIndex] == true)
		{
			for (int j = nextEventIndex; j < nextEventIndex + numEvents_toHandle; j++)
			{
				eventIdx_LOOP_Trackwise = percObj->eventIdx_ByTrack_ALL[j][trackIndex];

				// IF 0 THEN DO NOTHING
				if (percObj->infoMatrix[eventIdx_LOOP_Trackwise][0] == 0)
					percObj->incrementEventsHandled(trackIndex);

				//HANDLE NOTE ON
				if (percObj->infoMatrix[eventIdx_LOOP_Trackwise][0] == 1)
				{
					// CHECK WHICH VOICE THE NOTE ON BELONGS TO
					for (int i = 0; i < currentMusic.numVoices[trackIndex]; i++)
					{
						if (currentMusic.pitchesToMonitor[i][trackIndex] == (int)percObj->infoMatrix[eventIdx_LOOP_Trackwise][1])
						{
							vels[i][trackIndex] = cookMIDIVel(percObj->infoMatrix[eventIdx_LOOP_Trackwise][2], trackIndex, cue_AP_Name);
							
							percObj->incrementEventsHandled(trackIndex);	 			// INCREMENT EVENT COUNT
						}
					}
				}

				if (percObj->infoMatrix[eventIdx_LOOP_Trackwise][0] == 2)
				{
					for (int i = 0; i < currentMusic.numVoices[trackIndex]; i++)
					{
						if (currentMusic.pitchesToMonitor[i][trackIndex] == (int)percObj->infoMatrix[eventIdx_LOOP_Trackwise][1])
						{
							vels[i][trackIndex] = 0;
							percObj->incrementEventsHandled(trackIndex);				// INCREMENT EVENT COUNT
						}
					}
				}
			}
		}
	}
}

// MAP NEW MIDI EVENT FOR REQUIRED TRACKS
void Sequencer::mapNew_MIDIEvents()
{
	// CYCLE THROUGH ALL TRACKS
	for (int presentTrack = 1; presentTrack <= numTracks; presentTrack++)
	{
		std::string faustAddress = "";
		double pitch_Hz = 100;

		// CHECK ALL TRACKS FOR NEW EVENTS TO HANDLE
		if (isNewEvents_ToHandle[presentTrack - 1])
		{
			for (int currentVoice = 1; currentVoice <= currentMusic.numVoices[presentTrack - 1]; currentVoice++)
			{
				// PITCH
				if (isPitched[presentTrack - 1])
				{
					faustAddress = faustStrings.getMusicAddress(presentTrack, "P", currentVoice);
					pitch_Hz = MidiMessage::getMidiNoteInHertz(pitches[currentVoice - 1][presentTrack - 1]);
					dspFaust.setParamValue(faustAddress.c_str(), pitch_Hz);
				}

				// VELOCITY
				faustAddress = faustStrings.getMusicAddress(presentTrack, "V", currentVoice);
				dspFaust.setParamValue(faustAddress.c_str(), vels[currentVoice - 1][presentTrack - 1]);
			}
		}
	}
}