#include "PluginProcessor.h"
#include "PluginEditor.h"

// CONSTRUCTOR - INITIALIZATIONS AND START TIMER
GaitSonificationAudioProcessor::GaitSonificationAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	populateGrooves();
	setTempo(120);
	gaitAnalysis.setupReceivers();
	updateAudioParameter(1, 1);
	initializeClocking();
	startTimer(1);	
}

// DESTRUCTOR - STOP DSPFAUST AND TIMER
GaitSonificationAudioProcessor::~GaitSonificationAudioProcessor()
{
	sequencer.dspFaust.stop();
	stopTimer();
}

// HI RES TIMER CALLBACK - EVERY 1 MS
void GaitSonificationAudioProcessor::hiResTimerCallback()
{
	// INCREMENT GLOBAL TIME AND CLOCK PULSES ELAPSED
	gaitAnalysis.timeElapsed = globalTimeMs++ / 1000.0;
	pulsesElapsed += 1;

	// UPDATE SENSOR ONLINE STATUS EVERY 2 SECONDS
	if (globalTimeMs % 2000 == 0)
		gaitAnalysis.updateSensorStatus();

	// INCREMENT IMU RECORDING TIME IF ENABLED
	if (imuRecord.isRecording_Sensor)
		imuRecord.timeElapsed_REC += 0.001;

	// HANDLE SONG PLAYBACK
	if (sequencer.isPlaying)
	{
		// INCREMENT BASIC PLAYBACK COUNTERS
		sequencer.timeElapsed_SONG += 0.001;
		pulsesElapsed = pulsesElapsed % 100;
		sequencer.midiTicksElapsed += sequencer.ticksPerMS;
		sequencer.tapTempoCounter = min(1, sequencer.tapTempoCounter + 0.001);		// Limit to 1

		// RUN MUSICAL CLOCK CALLBACK
		clockCallback();

		// STOP MUSIC IF SONG COMPLETE
		if (sequencer.getSongProgress())
			sequencer.stopMusic();
	}

	// IF SAMPLE INTERVAL EXCEEDED, RUN SENSOR AND SONIFICATION CALLBACK
	if (pulsesElapsed % pulseInterval_sensorCallback == 0)
		sensorCallback();
}

// MUSIC CLOCK CALLBACK - EVERY 1 MS
void GaitSonificationAudioProcessor::clockCallback()
{
	// IF CLOCK PULSE DUE, TRIGGER CLOCK
	if (checkIfPulseDue())
		triggerClock(true);

	// CHECK FOR NEW MIDI EVENTS, HANDLE IF NEEDED
	sequencer.check_Handle_New_MIDIEvents();

	// UPDATE MUSIC PHASE
	sequencer.musicPhase.updatePhase();
}

// COMPUTE MP, AP, STORE SENSOR RECORDING EVERY 10 MS
void GaitSonificationAudioProcessor::sensorCallback()
{
	// IF ASSIGNED BUT NOT CONNECTED, SEND OUT OSC PACKETS FOR IP VERIFICATION
	ipVerify_AssignedSensors();

	// UPDATE IMU BUFFERS FOR ALL ACTIVE SENSORS
	for (int i = 0; i < gaitAnalysis.sensorInfo.numSensorsMax; i++)
		if (gaitAnalysis.sensorInfo.isOnline[i])
			gaitAnalysis.sensors_OSCReceivers[i].updateBuffers();

	// IF DYNAMIC REACHING, ENABLE DYN TRAJECTORY, UPDATE CENTER COORDINATES
	float dynReach_CenterCoordinates[2] = { 0.0, 0.0 };
	float dynReach_CenterCoordinates_ANTICIPATED[2] = { 0.0, 0.0 };
	
	// DYNAMIC REACHING TRAJECTORY UPDATE
	if (exerciseMode_Present == 3)
	{
		// FETCH PRESENT INFO
		dynZoneCenter.barsElapsed = sequencer.barsElapsed;
		dynReach_CenterCoordinates[0] = gaitAnalysis.staticBalance_BoundsCoordinates[0][0];
		dynReach_CenterCoordinates[1] = gaitAnalysis.staticBalance_BoundsCoordinates[0][1];

		// GET NEW 2D TARGET CENTER COORDINATES AND ASSIGN 
		dynZoneCenter.getCenterCoordinates(sequencer.musicPhase.presentPhase_Rad,
											dynReach_CenterCoordinates);
		gaitAnalysis.staticBalance_BoundsCoordinates[0][0] = dynReach_CenterCoordinates[0];
		gaitAnalysis.staticBalance_BoundsCoordinates[0][1] = dynReach_CenterCoordinates[1];

		// Feedback Type: Anticipated Distance Error (2D)
		if (gaitAnalysis.staticBalance_FB_TYPE == 3)
		{
			// GET ANTICIPATED CENTER COORDINATES AND ASSIGN
			dynZoneCenter.getCenterCoordinates(sequencer.musicPhase.presentPhase_Rad + dynZoneCenter.anticipationPhase,
				dynReach_CenterCoordinates_ANTICIPATED);
			gaitAnalysis.staticBalance_CenterXY_ANTICIPATED[0] = dynReach_CenterCoordinates[0];
			gaitAnalysis.staticBalance_CenterXY_ANTICIPATED[1] = dynReach_CenterCoordinates[1];
		}

		// ASSIGN AP VAL AFTER CALCULATION
		sequencer.AP_Val_2D_X = isStandby ? audioParams.audioParam_ObjectArray[audioParams.activeAudioParam_DynTarget_X].resetVal
		: gaitAnalysis.gaitParams.apVal_DYN_TaskDependent[0];
		sequencer.AP_Val_2D_Y = isStandby ? audioParams.audioParam_ObjectArray[audioParams.activeAudioParam_DynTarget_Y].resetVal
		: gaitAnalysis.gaitParams.apVal_DYN_TaskDependent[1];
	}

	// COMPUTE CHOSEN MOVEMENT PARAM
	gaitAnalysis.compute(gaitAnalysis.gaitParams.activeGaitParam, isCalibrating);

	// RECALIBRATE TRUNK SENSOR REST EVERY 3 SECONDS
	if (pulsesElapsed % 3000 == 0)
		gaitAnalysis.trunk_CalibrateRest(gaitAnalysis.sensors_OSCReceivers[gaitAnalysis.idx_Sensor_Trunk].acc_Buf);

	// COMPUTE PRESENT AP VALUE IF STANDBY DISABLED
	mapVal = isStandby ? audioParams.audioParam_ObjectArray[audioParams.activeAudioParam].resetVal
						 : jlimit(0.0, 1.0, gaitAnalysis.gaitParams.calc_AP_Val());
	sequencer.AP_Val = mapVal;

	// MAP X AND Y AP TO FAUST / WHEREVER
	if (gaitAnalysis.gaitParams.gaitParam_ObjectArray[gaitAnalysis.gaitParams.activeGaitParam].name ==
		"Trunk Projection Zone" && gaitAnalysis.staticBalance_FB_TYPE != 1)
	{
		sequencer.dspFaust.setParamValue
		(soniAddress_2D_X.c_str(), gaitAnalysis.gaitParams.apVal_DYN_TaskDependent[0]);

		sequencer.dspFaust.setParamValue
		(soniAddress_2D_Y.c_str(), gaitAnalysis.gaitParams.apVal_DYN_TaskDependent[1]);
	}

	// CHECK MUSIC PLAYBACK
	if (sequencer.isPlaying && gaitAnalysis.staticBalance_FB_TYPE == 1)
	{
		// CHECK AP TYPE
		switch (audioParams.audioParam_ObjectArray[audioParams.activeAudioParam].type)
		{
		case 1:	// AUDIO - BASED
			sequencer.dspFaust.setParamValue(soniAddress_Primary.c_str(), mapVal);
			break;
		case 2: // SEQUENCER - BASED
			applySequencerSonifications();
			break;
		}
	}

	// IF SENSOR RECORDING ENABLED, WRITE PRESENT LINE TO FILE
	if (imuRecord.isRecording_Sensor)
		writeSensorValue_ToFile(gaitAnalysis.gaitParams.gaitParam_ObjectArray
			[gaitAnalysis.gaitParams.activeGaitParam].currentValue);
}

// TRIGGER MUSIC MASTER CLOCK AT 16TH NOTE INTERVAL FOR COUNTER UPDATE
void GaitSonificationAudioProcessor::triggerClock(bool polarity)
{
	if (polarity == true)
	{
		// FAUST SET ON
		sequencer.incrementPulseCounter();
		sequencer.lastPulseTime = sequencer.nextPulseTime;
		sequencer.nextPulseTime += sequencer.midiTickIncrement;
	}
}

// APPLY SEQUENCER SONIFICATIONS ON COMPUTED AP VALUE
void GaitSonificationAudioProcessor::applySequencerSonifications()
{
	switch (audioParams.activeAudioParam)
	{

	}
}

// FIRST TIME SET SENSOR PULSE INTERVALS, NEXT SIXTEENTH MUSIC PULSE TIME
void GaitSonificationAudioProcessor::initializeClocking()
{
	pulseInterval_sensorCallback = 1000 / gaitAnalysis.sensorInfo.IMU_SampleRate;
	sequencer.nextPulseTime += interPulseIntervalMs * 0.001;
}

// IF ASSIGNED BUT NOT CONNECTED, SEND OUT OSC PACKETS
void GaitSonificationAudioProcessor::ipVerify_AssignedSensors()
{
	int remotePort = 0;	 
	String remoteIP = "";

	for (int i = 0; i < gaitAnalysis.sensorInfo.numSensorsMax; i++)
		if (gaitAnalysis.sensorInfo.bodyLocation[i] != 4)
		{
			if (!gaitAnalysis.sensorInfo.isOnline[i])
			{
				remotePort = gaitAnalysis.sensorInfo.UDP_Ports_REMOTE[i];
				remoteIP = gaitAnalysis.sensorInfo.remoteIP[i];
				if (gaitAnalysis.sensorInfo.connectionVerify_IP[i].connect(remoteIP, remotePort))
				{
					OSCMessage message("/CONNECT");
					message.addString("Please Connect");
					gaitAnalysis.sensorInfo.connectionVerify_IP[i].send(message);
				}
			}
		}
}

const String GaitSonificationAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GaitSonificationAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GaitSonificationAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GaitSonificationAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GaitSonificationAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GaitSonificationAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
}

int GaitSonificationAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GaitSonificationAudioProcessor::setCurrentProgram (int index)
{
}

const String GaitSonificationAudioProcessor::getProgramName (int index)
{
    return {};
}

void GaitSonificationAudioProcessor::changeProgramName (int index, const String& newName)
{
}

void GaitSonificationAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void GaitSonificationAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GaitSonificationAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void GaitSonificationAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
}

bool GaitSonificationAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* GaitSonificationAudioProcessor::createEditor()
{
    return new GaitSonificationAudioProcessorEditor (*this);
}

void GaitSonificationAudioProcessor::getStateInformation (MemoryBlock& destData)
{
}

void GaitSonificationAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GaitSonificationAudioProcessor();
}
