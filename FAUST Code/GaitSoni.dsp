// // // // // // // // // // // // // // // // // // // // // // // // // // // // D E F I N I T I O N S // // // // // // // // // // // // // // // // // // // // // // // // // // // // 
import("stdfaust.lib");
// Panner
NUM_PANPOS = 7;
// Dotted Delay
DOTDEL_FC_HP = 800;					DOTDEL_FC_LP = 2000;
// FM Synth
FM_DISTMIX = 0.05;
// Piano Single Note
PIANO_WAVEWIDTH1 = 33;				PIANO_WAVEWIDTH2 = 10;					PIANO_WAVEWIDTH3 = 66;
// Full Chord Synth			
CS_DEL_N1 = 0.015;					CS_DEL_N2 = 0.030;						CS_DEL_N3 = 0.045;		

// MUSIC Trackwise SYNTH //
// 1, 2, 3, 8 are sample-based
// 4 - CHORD SYNTH
PANPOS_NOTES = 3,4,5,6;
AT_C = 0.001;			RL_C = 1;					FV_C = 1;
// MALLET FM
MALLET_MRATIO = 2;		MALLET_I_FIXED = 2.4;	MALLET_I_ENV = 6.86;	MALLET_A = 0.001;	MALLET_D = 0.001;	MALLET_S = 0.5;		MALLET_R = 0.4;			MALLET_ENVTYPE = 0;
// 5 - BASSLINE
AT_R = 0.001;			RL_R = 0.6 + (tempo < 120)*(120-tempo)*0.06;
MOD_NUM_R = 5;			FREQ_FACTOR_R = 3;		MOD_DEPTH_R = 35;
// BASSLINE FM
BASSLINE_MRATIO = 2;	BASSLINE_I_FIXED = 6.75;	BASSLINE_I_ENV = 7.46;		BASSLINE_A = 0.007; 	BASSLINE_D = 0.001;		BASSLINE_S = 0.6553;		BASSLINE_R = 0.65;
BASSLINE_ENVTYPE = 1;
// AGGRO FM
AGGRO_MRATIO = 3;		AGGRO_I_FIXED = 10;	AGGRO_I_ENV = 10;		AGGRO_A = 0.01;	AGGRO_D = 0.0001;	AGGRO_S = 0.5;	AGGRO_R = 0.867;		AGGRO_ENVTYPE = 1;
// 6 - MAIN MELODY
AT_M = 0.001;							RL_M = 4;
FB_DEL_M = 0.5;							WET_DEL_M = 0.8;
FC_LP_M = 20000;						G_SIN_M = -90;				G_NS_M = -90;				RL_NS_M = 0.05;			FC_HP_NS_M = 3000;
// TRUMPETISH FM
TRUMPET_MRATIO = 1;		TRUMPET_I_FIXED = 10;	TRUMPET_I_ENV = 10;		TRUMPET_A = 0.05;	TRUMPET_D = 0.0001;	TRUMPET_S = 0.5;	TRUMPET_R = 0.78;		TRUMPET_ENVTYPE = 1;
// 7
AT_CS = 0.001;							RL_CS = 5;
FB_DEL_CS = 0.8;						WET_DEL_CS = 0.4;
DMP_KS_CS = 0.99;						FREQ_OFFSET_LR = 0.7;

//Sonifications
LIST_FREQ_DISTFACTORS = 0.05,-0.03,0.31,-0.27;																// CHORD FREQ DIST FACTORS
SONI_SB_Z2_ONOFF = waveform {1, 1, 1, 1, 0, 0,		1, 1, 1, 0, 0, 1,		1, 1, 1, 0, 1, 0,		1, 1, 0, 0, 0, 0,			
				   			 1, 1, 0, 0, 0, 0,		1, 0, 0, 0, 0, 0,		1, 1, 0, 0, 0, 0,		1, 1, 1, 0, 0, 1};
SONI_SB_Z2_ONOFF_RD(i) = SONI_SB_Z2_ONOFF,i : rdtable;

Soni_R1_maxLevel = 0;	Soni_R1_minLevel = -80;																// INSTRUMENTATION
Soni_R3_Fc_Max = 20000; Soni_R3_Fc_Min = 200;																// BRIGHTNESS

Soni_J1_MaxWarpFactor = 10;											// TONIC PITCH 
Soni_J2_minFreq = 250;		Soni_J2_maxFreq = 5000;					// PITCHED WAVE
Soni_J3_minFreq = 1000;		Soni_J3_maxFreq = 20000;				// WHOOSH NOISE
Soni_J3_HPF_FC = 150;		Soni_J3_LPF_Q = 5;						// "

// BUSS REVERB
REV_MST_PDEL_MS = 10;												// PRE-DELAY
REV_MST_F_DC_MID = 100;												// DC/MID CROSSOVER FREQ
REV_MST_F_MID_HI = 2000;											// MID/HI CROSSOVER FREQ
REV_MST_T60_DC = 1;													// RT60 - DC
REV_MST_FsMax = 48000;												// FS MAX
REV_MST_HPF_FC = 200;												// FC - LOWER
REV_MST_LPF_FC = 3500;												// FC - UPPER

// REVERB SENDS
REV_SND = -8,-10,-5,0,-5;

// // // // // // // // // // // // // // // // // // // // // // // // // // // // U S E R  I N T E R F A C E // // // // // // // // // // // // // // // // // // // // // // // // // // //
// TABS
musicInfoTab(x) = tgroup("Mapping_Tabs",hgroup("Music_Info",x));
compTab(x) = tgroup("Mapping_Tabs",vgroup("Comp_Section",x));
eqTab(x) = tgroup("Mapping_Tabs",vgroup("EQ_Section",x));
sonificationTab(x) = tgroup("Mapping_Tabs",hgroup("Sonification Types",x));
mixMasterTab(x) = tgroup("Mapping_Tabs",hgroup("Mixer_And_Master",x)); 		

// SLIDER GROUPS
// MUSIC INFO TAB
pitchInfo(x) = musicInfoTab(vgroup("Pitch",x));
velocityInfo(x) = musicInfoTab(vgroup("Velocity",x));
// MIX AND MASTER TAB
trackGainGroup(x) = mixMasterTab(vgroup("Track Gain",x));
trackMuteGroup(x) = mixMasterTab(vgroup("Track Mute",x));
masterGainGroup(x) = mixMasterTab(vgroup("Master Gain",x));
masterEQGroup(x) = mixMasterTab(vgroup("Master EQ",x));
variantGroup(x) = mixMasterTab(vgroup("Track Variant",x));
// EQ TAB
eqTrackGroup(i,x) = eqTab(hgroup("Track %i",x));
// COMP TAB
compTrackGroup(i,x) = compTab(hgroup("Track %i",x));

// Tempo
tempo = musicInfoTab(vslider("Tempo",120,50,150,0.1)) : limit(60,150);

//Variants
VAR_1 = variantGroup(nentry("Variant_1",1,1,3,1));
VAR_2 = variantGroup(nentry("Variant_2",1,1,3,1));
VAR_3 = variantGroup(nentry("Variant_3",1,1,3,1));
VAR_4 = variantGroup(nentry("Variant_4",1,1,3,1));
VAR_5 = variantGroup(nentry("Variant_5",1,1,3,1));
VAR_6 = variantGroup(nentry("Variant_6",1,1,3,1));
VAR_7 = variantGroup(nentry("Variant_7",1,1,3,1));
VAR_8 = variantGroup(nentry("Variant_8",1,1,3,1));

//Music Info
																						// NOTE NUMBERS
KEYNUM_C_1 = pitchInfo(hslider("T4_P_1",100,20,10000,0.001));									// 4-1
KEYNUM_C_2 = pitchInfo(hslider("T4_P_2",100,20,10000,0.001));									// 4-2
KEYNUM_C_3 = pitchInfo(hslider("T4_P_3",100,20,10000,0.001));									// 4-3
KEYNUM_C_4 = pitchInfo(hslider("T4_P_4",100,20,10000,0.001));									// 4-4
KEYNUM_R = pitchInfo(hslider("T5_P_1",100,20,10000,0.001));										// 5
KEYNUM_M = pitchInfo(hslider("T6_P_1",100,20,10000,0.001));										// 6
KEYNUM_CS_1 = pitchInfo(hslider("T7_P_1",100,20,10000,0.001));									// 7-1
KEYNUM_CS_2 = pitchInfo(hslider("T7_P_2",100,20,10000,0.001));									// 7-2
KEYNUM_CS_3 = pitchInfo(hslider("T7_P_3",100,20,10000,0.001));									// 7-3
KEYNUM_CS_4 = pitchInfo(hslider("T7_P_4",100,20,10000,0.001));									// 7-4			
																								// VELOCITIES
V_K = velocityInfo(hslider("T1_V_1",9,0,9,0.1));													// 1
V_S = velocityInfo(hslider("T2_V_1",9,0,9,0.1));													// 2
S_FNUM = ((V_S > 3) + (V_S > 6)) : ba.sAndH(V_S*TRG_S) : *(1 - S_isHEEL) + S_isHEEL*2;			// Snare - File Number
V_HH = velocityInfo(hslider("T3_V_1",9,0,9,0.1)); 												// 3
HH_FNUM = ((V_HH > 3) + (V_HH > 6)) : ba.sAndH(V_HH*TRG_HH);									// HH - File Number
V_C_1 = velocityInfo(hslider("T4_V_1",9,0,9,0.1));												// 4-1
V_C_2 = velocityInfo(hslider("T4_V_2",9,0,9,0.1));												// 4-2
V_C_3 = velocityInfo(hslider("T4_V_3",9,0,9,0.1));												// 4-3
V_C_4 = velocityInfo(hslider("T4_V_4",9,0,9,0.1));												// 4-4
V_LIST_C = V_C_1, V_C_2, V_C_3, V_C_4;															// 4 - LIST
V_R = velocityInfo(hslider("T5_V_1",9,1,9,0.1));													// 5
V_M = velocityInfo(hslider("T6_V_1",9,1,9,0.1));													// 6
V_CS = velocityInfo(hslider("T7_V_1",9,0,9,0.1));												// 7
V_CR = velocityInfo(hslider("T8_V_1",9,0,9,0.1));												// 8

//Sonification Sliders and Preprocessing
soniSlider(idx,defaultVal) = sonificationTab(vgroup("AP Values",hslider("Soni %idx",defaultVal,0,1,0.001)));
//Discrete
Soni_Z_SB1 = 			soniSlider(1,0) : zonePreProcess;									// Ambulance
Soni_Z_SB2 = 			soniSlider(2,0) : zonePreProcess;									// Instrumentation
Soni_X_STS1_Bell = 		soniSlider(3,0) : ba.impulsify;									// Bell Trigger
Soni_X_STS2_MusicStop = soniSlider(4,0) : >(0.1);											// Music Stop
Soni_X_STS3_Wah =		soniSlider(5,0) : si.smoo;										// Wah Wah
Soni_X_H1_TRG = 		soniSlider(6,0);													// HS Trigger

// HS Slider Cooking
HS_S_TRG = (abs(Soni_X_H1_TRG - 0.8) < 0.03) : ba.impulsify;
HS_K_TRG = (abs(Soni_X_H1_TRG - 0.7) < 0.03) : ba.impulsify;
S_isHEEL = Soni_X_H1_TRG > 0.49;

//Continuous
Soni_X_P3_ChordFreqDist = 				soniSlider(7,0);									// Chord Freq Distortion
Soni_X_R3_OverallBrightness = 			soniSlider(8,0);									// Overall Music Brightness
Soni_X_J1_MelBaseFreq = 				soniSlider(9,0.5);									// Melody Tonic Frequency
Soni_X_J2_Pitched = 					soniSlider(10,0);									// Pitched Disturbance
Soni_X_J3_Whoosh = 						soniSlider(11,0);									// Noise Disturbance
Soni_X_D1_Spatialize = 					soniSlider(12,0.5);									// Spatialization

SONI_GAIN_DB = masterGainGroup(vslider("Soni Buss Gain",-8,-10,2,0.01));
masterGain = masterGainGroup(vslider("Master Gain",-6,-96,12,0.01) : ba.db2linear);

// // // // // // // // // // // // // // // // // // // // // // // // // // // // F U N C T I O N S // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
// UTILITY
BT_SMPL = ba.tempo(tempo);
stereoLinGain(gain) = _,_ : *(gain),*(gain);
stereodBGain(gain) = _,_ : *(linGain),*(linGain) with { linGain = ba.db2linear(gain); };
stereoLinMult(gainPair) = _,_ : *(gainPair : select2(0)),*(gainPair : select2(1));
stereoEffect(effect) = _,_ : effect,effect;
limit(lower,higher) = _ : max(lower) : min(higher);
hard_clip(limit) = _ : min(limit) : max(-1*limit) : _;

// CONVERT VELOCITY VALUE TO TRIGGER ON CHANGE
velToTrigger(vel) = trigger with
{
  trigger = (posTrig + negTrig)*(1 - Soni_X_STS2_MusicStop);
  posTrig = vel : ba.impulsify;
  negTrig = - 1 * (vel) : ba.impulsify : *(vel > 0);
};

// CONVERT 0-10 VELOCITY VALUE TO MONO GAIN MULTIPLIER
applyVelocity(velocity,trigger,maxVel) = _ : *(gainMult)	 with
{ 
  gainMult = sampledVel * sampledVel * 0.01;
  sampledVel = velCooked : ba.sAndH(velSampleInstant);
  velCooked = velocity * 9.0 / maxVel;
  velSampleInstant = trigger;
};

// CONVERT 0 - 1 SONI SLIDER VALUE TO 1 - 6 ZONE VALUE
zonePreProcess(input) = output
  with 
{ 
  output = par(i,6,isInZone(i)) :> int;
  isInZone(i) = (i+1) * (input > SONI_SB_THRESH_VALS_RD(i)) * (input < SONI_SB_THRESH_VALS_RD(i+1));
};

// ZONE THRESHOLD VALUES - STORED IN READ TABLE
SONI_SB_THRESH_VALS = waveform {-0.1, 0.33, 0.66, 0.8, 0.9, 0.95, 1.1};
SONI_SB_THRESH_VALS_RD(i) = SONI_SB_THRESH_VALS,i : rdtable;

// TEMPO-BASED INSTRUMENT RELEASE FACTOR
tempo_RelFactor = 1 + 1.5 * (120-tempo) / 40 * (tempo < 120);

// MASTER LIMITER
masterLimiter(ipGaindB) = _ : compLimiter(ipGaindB,10,0,0.001,0.05,0.050);

// SHORT STRUM DELAY
shortDelay(d_ms) = stereoEffect(de.fdelay(5000,d_ms*ma.SR));

// CHANNEL PROCESSORS - MONO AND STEREO
monoChannel(trackIndex) = *(0.25) : channelComp(compTrackGroup(trackIndex)) : parametricEQ(eqTrackGroup(trackIndex));
stereoChannel(trackIndex) = stereoLinGain(0.25) : stereoEffect(channelComp(compTrackGroup(trackIndex))) : stereoEffect(parametricEQ(eqTrackGroup(trackIndex)));

// PREPROCESS CHORD FREQUENCIES
getChordFinalFreqs(LIST) = freqs with 
{   
  freqs = par(i,4,freq(i));																							// CREATE FREQ LIST
  freq(i) = freqs_pre(i) : Soni_J1_FreqWarpFactor : *(Soni_P3_freqDistFactor(i));									// APPLY TUNING SONIFICATIONS
  Soni_P3_freqDistFactor(i) = LIST_FREQ_DISTFACTORS : ba.selectn(4,i) : *(Soni_X_P3_ChordFreqDist) : +(1);			// CALCULATE CHORD DIST AMOUNT
  freqs_pre(i) = LIST : ba.selectn(4,i);																			// GET INDIVIDUAL MIDI KEYS
};

// GET STEREO PAN GAINS
getPanFunction(panIndex) = panFunction	with	
{
  panFunction = _ <: _*(L_multiplier), _*(R_multiplier);																			// SPLIT AND MULTIPLY
  L_multiplier = 1,0.95,0.3,0.9,0.8,0.6,0.4 : ba.selectn(NUM_PANPOS,panIndex);														// L-MULTIPLIER
  R_multiplier = 1,0.3,0.95,0.4,0.6,0.8,0.9 : ba.selectn(NUM_PANPOS,panIndex);														// R-MULTIPLIER											
};

// CHANNEL COMP
channelComp(compTrackGroup) = compLimiter(0,ratio,thresh,att,rel,0.050)	 with	{
  ratio = compTrackGroup(hslider("Ratio[style:knob]",1,1,10,0.1));
  thresh = compTrackGroup(hslider("Threshold[style:knob]",0,-96,0,0.1));
  att = compTrackGroup(hslider("Attack[style:knob]",0.1,0.001,0.5,0.001));
  rel = compTrackGroup(hslider("Release[style:knob]",0.1,0.001,0.5,0.001));		};

// CHANNEL EQ
parametricEQ(eqTrackGroup) = fi.resonhp(fc_HP,Q_HP,1) : fi.peak_eq_cq(gain_P1,f_P1,Q_P1) : fi.peak_eq_cq(gain_P2,f_P2,Q_P2) : fi.resonlp(fc_LP,Q_LP,1)
  with	{ hpGroup(x) = hgroup("1_HPF",x);
  p1Group(x) = hgroup("2_Parametric_1",x);
  p2Group(x) = hgroup("3_Parametric_2",x);
  lpGroup(x) = hgroup("4_LPF",x);
  fc_HP = eqTrackGroup(hpGroup(hslider("Freq[style:knob]",50,50,1000,0.1)));
  Q_HP = eqTrackGroup(hpGroup(hslider("Q[style:knob]",0.7,0.5,10,0.01)));
  fc_LP = eqTrackGroup(lpGroup(hslider("Freq[style:knob]",20000,50,20000,0.1)));
  Q_LP = eqTrackGroup(lpGroup(hslider("Q[style:knob]",0.7,0.5,10,0.01)));
  gain_P1 = eqTrackGroup(p1Group(hslider("Gain[style:knob]",0,-24,24,0.01)));
  f_P1 = eqTrackGroup(p1Group(hslider("Freq[style:knob]",500,100,2000,0.01)));
  Q_P1 = eqTrackGroup(p1Group(hslider("Q[style:knob]",0.7,0.5,10,0.01)));
  gain_P2 = eqTrackGroup(p2Group(hslider("Gain[style:knob]",0,-24,24,0.01)));
  f_P2 = eqTrackGroup(p2Group(hslider("Freq[style:knob]",2000,1500,15000,0.01)));
  Q_P2 = eqTrackGroup(p2Group(hslider("Q[style:knob]",0.7,0.5,10,0.01))); };

// 3 - Effects
// Dotted Delay
dotted_delay(feedback,delay,wet) = +~(de.delay(100000,del) : filt)*feedback*wet :> _	with	{ filt = fi.bandpass(2,DOTDEL_FC_HP,DOTDEL_FC_LP); del = 0.75*delay; };

// Compressor-Limiter
compLimiter(inputGain_dB,ratio,thresh,att,rel,kneeAtt) = _*inputGain_lin 
  <: _*(an.amp_follower_ud(att,rel) : ba.linear2db : outminusindb : kneesmooth : ba.db2linear)
with{inputGain_lin = ba.db2linear(inputGain_dB);
	 outminusindb(level) = max(level-thresh,0)*(1/ratio - 1);													// CALCULATE GAIN REDUCTION
	 kneesmooth = si.smooth(ba.tau2pole(kneeAtt));	};															// SMOOTH KNEE

// Master COMP
masterComp = stereoEffect(compLimiter(0,1.01,-20,100,300,0.05)) : stereoEffect(compLimiter(0,1.1,-5,50,200,0.05)) : stereodBGain(2);

//Reverb Master
reverbMaster = _,_ <: re.zita_rev1_stereo(REV_MST_PDEL_MS,REV_MST_F_DC_MID,REV_MST_F_MID_HI,REV_MST_T60_DC,rt_60,REV_MST_FsMax) : filter
with {
  rt_60 = min(0.3 + (150 - tempo) * 0.05,1.85); 																			// TEMPO DEPENDENT RT60
  filter = stereoEffect(fi.bandpass(1,REV_MST_HPF_FC,REV_MST_LPF_FC));														// OUTPUT FILTER
};

stereoMasterSection(trackIndex) = stereoOut
  with
{
  stereoOut = stereoLinGain(trackGain) : stereoLinGain(1-trackMute);
  trackGain = trackGainGroup(hslider("Track %trackIndex",0,-96,24,0.01) : ba.db2linear);
  trackMute = trackMuteGroup(checkbox("%trackIndex"));
};

//4 - Synthesis
samplePlayer(fileFunc,trigger) = fileFunc(ba.countup(96000,trigger));
K_FILES = soundfile("K_SMPL[url:{'D:\\\\GaitSonification\\\\Drum Samples\\\\Final Library\\\\K_V1.wav'; 'D:\\\\GaitSonification\\\\Drum Samples\\\\Final Library\\\\K_V2.wav';
										'D:\\\\GaitSonification\\\\Drum Samples\\\\Final Library\\\\K_V3.wav'}]",1) : !,!,_;
S_V1_FILES = soundfile("S_SMPL_V1[url:{'D:\\\\GaitSonification\\\\Drum Samples\\\\Final Library\\\\S_V1_1.wav'; 'D:\\\\GaitSonification\\\\Drum Samples\\\\Final Library\\\\S_V1_2.wav';
									   'D:\\\\GaitSonification\\\\Drum Samples\\\\Final Library\\\\S_V1_3.wav'}]",1) : !,!,_;
S_V2_FILES = soundfile("S_SMPL_V2[url:{'D:\\\\GaitSonification\\\\Drum Samples\\\\Final Library\\\\S_V2_1.wav'; 'D:\\\\GaitSonification\\\\Drum Samples\\\\Final Library\\\\S_V2_2.wav';
									   'D:\\\\GaitSonification\\\\Drum Samples\\\\Final Library\\\\S_V2_3.wav'}]",1) : !,!,_;
S_V3_FILES = soundfile("S_SMPL_V3[url:{'D:\\\\GaitSonification\\\\Drum Samples\\\\Final Library\\\\S_V3_1.wav'; 'D:\\\\GaitSonification\\\\Drum Samples\\\\Final Library\\\\S_V3_2.wav';
									   'D:\\\\GaitSonification\\\\Drum Samples\\\\Final Library\\\\S_V3_3.wav'}]",1) : !,!,_;
CR_FILES = soundfile("CR_SMPL[url:{'D:\\\\GaitSonification\\\\Drum Samples\\\\Final Library\\\\CR_V1.wav'; 	     'D:\\\\GaitSonification\\\\Drum Samples\\\\Final Library\\\\CR_V2.wav';
									   'D:\\\\GaitSonification\\\\Drum Samples\\\\Final Library\\\\CR_V3.wav'}]",1) : !,!,_;
HH_V1_FILES = soundfile("HH_SMPL_V1[url:{'D:\\\\GaitSonification\\\\Drum Samples\\\\Final Library\\\\HH_V1_1.wav'; 'D:\\\\GaitSonification\\\\Drum Samples\\\\Final Library\\\\HH_V1_2.wav';
										'D:\\\\GaitSonification\\\\Drum Samples\\\\Final Library\\\\HH_V1_3.wav'}]",1) : !,!,_;
HH_V2_FILES = soundfile("HH_SMPL_V2[url:{'D:\\\\GaitSonification\\\\Drum Samples\\\\Final Library\\\\HH_V2_1.wav'; 'D:\\\\GaitSonification\\\\Drum Samples\\\\Final Library\\\\HH_V2_2.wav';
										'D:\\\\GaitSonification\\\\Drum Samples\\\\Final Library\\\\HH_V2_3.wav'}]",1) : !,!,_;

K_SMPL_V1(i) =   0,i : K_FILES;			K_SMPL_V2(i) =   1,i : K_FILES;			K_SMPL_V3(i) =   2,i : K_FILES;
S_SMPL_V1_1(i) = 0,i : S_V1_FILES;		S_SMPL_V1_2(i) = 1,i : S_V1_FILES;		S_SMPL_V1_3(i) = 2,i : S_V1_FILES;
S_SMPL_V2_1(i) = 0,i : S_V2_FILES;		S_SMPL_V2_2(i) = 1,i : S_V2_FILES;		S_SMPL_V2_3(i) = 2,i : S_V2_FILES;
S_SMPL_V3_1(i) = 0,i : S_V3_FILES;		S_SMPL_V3_2(i) = 1,i : S_V3_FILES;		S_SMPL_V3_3(i) = 2,i : S_V3_FILES;
CR_SMPL_V1(i) = 0,i : CR_FILES;			CR_SMPL_V2(i) = 1,i : CR_FILES;			CR_SMPL_V3(i) = 2,i : CR_FILES;
HH_SMPL_V1_1(i) = 0,i : HH_V1_FILES;	HH_SMPL_V1_2(i) = 1,i : HH_V1_FILES;	HH_SMPL_V1_3(i) = 2,i : HH_V1_FILES;
HH_SMPL_V2_1(i) = 0,i : HH_V2_FILES;	HH_SMPL_V2_2(i) = 1,i : HH_V2_FILES;	HH_SMPL_V2_3(i) = 2,i : HH_V2_FILES;

fmSynth_Versatile(fc,modRatio,I_fixed,I_ampDependent,a,d,s,r,envType,trigger,vel) = output
  with
{
  fc_cooked = fc : si.smooth(ba.tau2pole(0.005));
  output = os.osc(fc_cooked + dev) * ampEnv;
  dev = I * ampEnv * modFreq * os.triangle(modFreq);
  I = I_fixed + (I_ampDependent * (0.5 + velFactor) + I_freq) * (ampEnv);
  I_freq = 4 * (fc - 300)/300;
  modFreq = (modRatio + Soni_X_P3_ChordFreqDist) * fc_cooked;
  release_basic = ba.tempo(ba.tempo(tempo))/ma.SR * (1 + velFactor * 2);
  env_basic = en.ar(a,release_basic,trigger);
  triggerCooked = (env_basic > 0.5) + (env_basic > env_basic');
  chosenEnv = en.adsr(a,d,s,r,triggerCooked),	en.adsre(a,d,s,r,triggerCooked), en.ar(a,r,trigger), en.are(a,r,trigger), en.arfe(a,r,0.3,trigger) : ba.selectn(5,envType); 
  ampEnv = chosenEnv : si.smooth(ba.tau2pole(a));
  velFactor = 1 : applyVelocity(vel,trigger,9);
};

leadSynth(fundamental,synthFunc,velocity,trigger,synthRelease,synthCutoff) = output
with
{																			
  output = melSynth,melSynth;																					// OUTPUT SUMMING
  melSynth = synthFunc(fundamentalCooked) * env : applyVelocity(velocity,trigger,9);							// MEL COMPONENT
  fundamentalCooked = 2 * fundamental * soniVibratoLFO * velVibrato : limit(20,5000);
  env = en.ar(0.001,synthRelease,trigger);																		// MEL ENVELOPE
  velVibrato = 1 + (0.02 * ((200 - tempo)/140) : applyVelocity(velocity,trigger,9)) * os.osc(tempo/15) : si.smooth(ba.tau2pole(0.001));
  soniVibratoLFO = 1 + Soni_X_P3_ChordFreqDist * os.osc(tempo/15) * 0.5 : si.smoo;								// CF DIST SONI - VIBRATO LFO
};

fmSynth(fundamental,numMod,freqFactor,release,depth,trigger) = (fmSynth + dirtyBus) * env 	with
{ 
  fmSynth = sy.fm(freqList,depthList);																			// SYNTH - FM 
  dirtyBus = fmSynth : ef.cubicnl(0.7,0) : _ *(FM_DISTMIX);														// DIST MIX
  freqList = par(i,numMod,fundamental * pow(freqFactor,i));														// (1)CARRIER + MOD FREQ LIST
  depthList = par(i,numMod-1,depthCooked);																		// MOD DEPTH LIST
  depthCooked = depth * env * 9;																				// COOKED DEPTH
  env = sqrt(en.ar(0.001,release, trigger)) : si.smooth(ba.tau2pole(0.001));									// AMP ENVELOPE
};

pulseWave(freq,widthPercent) = output with
{
  output = 2 * (ba.pulsen(duration,interval) - 0.5);															// DC CORRECTED OUTPUT WAVE
  duration = widthPercent * interval / 100;																		// PULSE UP TIME
  interval = ma.SR / freq;																						// PUlSE TOTAL PERIOD
};

pianoSim_singleNote(freq,trigger) = monoOut
  with
{
  monoOut = pulseWave(freq,PIANO_WAVEWIDTH1),pulseWave(freq,
			PIANO_WAVEWIDTH2),pulseWave(freq,PIANO_WAVEWIDTH3):> fi.lowpass(2,cutoff) * ampEnv;					// WAVESUMMING
  cutoff = (freqEnv + 0.01) * 4000 * freq / 600 * (1 - min(freq,1000)/2000) : limit(20,20000);					// FC
  freqEnv = en.arfe(0.001,1.6,0.4,trigger) : si.smooth(ba.tau2pole(0.0001));									// FREQUENCY ENV
  ampEnv = pow(en.ar(0.001,4,trigger),6)  : si.smooth(ba.tau2pole(0.0001));										// AMPLITUDE ENV
};

voiceSynth_FormantBP(freq,vel,trigger) = pm.SFFormantModelBP(1,vowel_H,0,freq/2.0,0.04) * env with
{
  	vowel_idx = _~+(trigger) : %(4) : _ + 0.4 * (0.5 + 0.5*os.osc(0.3));
	env = en.ar(0.04, 3  / tempo * 78.6, trigger);
  	vowel_H = vowel_idx : si.smooth(ba.tau2pole(0.04));
};

fullChordSynth(freqList,synthFunc,env) = stereoChordOut with
{ 
  freqSelector(n) = freqList : ba.selectn(4,n-1);																			// INDIVIDUAL FREQS
  strumDelay(dMax) = dMax * (tempo < 120) * (120 - tempo)/60;																// CALCULATE DELAY
  freq1Bus = synthFunc(freqSelector(1)),synthFunc(freqSelector(1) + FREQ_OFFSET_LR);										// F1
  freq2Bus = synthFunc(freqSelector(2)),synthFunc(freqSelector(2) + FREQ_OFFSET_LR) : shortDelay(strumDelay(CS_DEL_N1));	// F2	
  freq3Bus = synthFunc(freqSelector(3)),synthFunc(freqSelector(3) + FREQ_OFFSET_LR) : shortDelay(strumDelay(CS_DEL_N2)); 	// F3
  freq4Bus = synthFunc(freqSelector(4)),synthFunc(freqSelector(4) + FREQ_OFFSET_LR) : shortDelay(strumDelay(CS_DEL_N3)); 	// F4
  stereoChordOut = freq1Bus,freq2Bus,freq3Bus,freq4Bus :> stereoLinGain(env);												// SUM + ENVELOPE
};

chordSingle_Synth(freq,panFunc,synthFunc) = noteOut with
{
  noteOut = synthFunc(freq) : getPanFunction(panFunc);
};

ks(freq,damping) = +~((de.fdelay4(1024,del)*damping) : dampingFilter)
  with
{
  del = ma.SR/freqCooked - 0.5 - 1*(freqCooked > 600);																	// CALCULATE DELAY LENGTH + TUNING COMPENSATE
  dampingFilter = _ <: (_'+ _)/2;																						// DAMPING FB FILTER
  freqCooked = freq * 1.0116;
};

ks_excitation(trigger) = no.noise*en.ar(0.001,0.001,trigger);															// KARPLUS STRONG NOISE EXCITATION

staticBalanceMult(z) = 0.5 * (z == 3) + 1.2 * (z == 4) + 1.4 * (z == 5), 0.5 * (z == 3) + 1.2 * (z == 4) + 1.4 * (z == 6) : stereoEffect(si.smoo); 

// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // S O N I // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

transientMelDip = dipON * -24 : ba.db2linear : si.smoo				// J2 + J3 - MELODY DIP
with
{
  dipON = 1 * (Soni_X_J2_Pitched > 0.01) + 1 * (Soni_X_J3_Whoosh > 0.01) 												// JERK STRATEGY DIPS
	
	;
};

musicDuck = duckON * (-50) : ba.db2linear : si.smoo
  with
{
  duckON = 1 * (Soni_Z_SB1 >= 3) + 0.5 * (Soni_Z_SB1 == 2)
	;
};

SB_masterPan = out with
{
  out = _,_ : *(leftGain),*(rightGain);
  leftGain =  1 - ((Soni_Z_SB1 == 6) + (Soni_Z_SB2 == 6)) : si.smoo;
  rightGain = 1 - ((Soni_Z_SB1 == 5) + (Soni_Z_SB2 == 5)) : si.smoo;
};

// AMBULANCE
Soni_SB1_Signal_Ambulance = output <: stereoLinMult(Soni_SB1_StereoMult)
with
{
  output = os.triangle(frequencyCooked);
  frequencyCooked = (0.5 + abs(os.osc(modFreq))) * 925;
  modFreq = 0.15 * (Soni_Z_SB1 == 3) + 1.4 * (Soni_Z_SB1 > 3) + 0.5 * (Soni_Z_SB1 > 4);
  Soni_SB1_StereoMult = staticBalanceMult(Soni_Z_SB1);
};

// INSTRUMENTATION 
Soni_SB2_Instrumentation(trackIdx) = stereoLinGain(outGain)
  with
{
  outGain = SONI_SB_Z2_ONOFF_RD(max(0,((trackIdx - 1) * 6 + Soni_Z_SB2 - 1))) : si.smoo;
};

// TONIC PITCH MODULATION
Soni_J1_FreqWarpFactor = _* pow(Soni_J1_MaxWarpFactor,(2 * (Soni_X_J1_MelBaseFreq - 0.5)));

// MASTER FILTER - LPF
Soni_R3_Filter = fi.resonlp(cutoff,qCooked,1) with 																		// R3 - BRIGHTNESS
{
  cutoff = Soni_R3_Fc_Max - Soni_X_R3_OverallBrightness * (Soni_R3_Fc_Max - Soni_R3_Fc_Min) : si.smoo;					// CALCULTE CUTOFF
  qCooked = 4 - 3.3 * pow(Soni_X_R3_OverallBrightness,2);																// CALCULATE F-DEPENDENT Q
};

// PITCHED DISTURBANCE
Soni_J2_Pitched = os.sawtooth(frequency) * gain <: _,_ with																// J2 - PITCHED WAVE
{
  frequency = Soni_J2_minFreq + (Soni_J2_maxFreq - Soni_J2_minFreq) * pow(Soni_X_J2_Pitched,2);							// CALCULATE WAVE FREQUENCY
  gain = Soni_X_J2_Pitched > 0.01 : si.smoo;																			// CALCULATE ON/OFF CONDITION
};

// SCRAMBLE DELAY - DISTURBANCE ACCOMPANYING
Soni_J_Del = de.delay(ma.SR,delSamples),de.delay(ma.SR,delSamples) : filt,filt with
{
  jerkAP = (Soni_X_J2_Pitched + Soni_X_J3_Whoosh) : si.smooth(ba.tau2pole(30.0/tempo));
  delSamples = jerkAP * ma.SR * 0.5*(1 + os.osc(tempo/30));
  filt = fi.peak_eq_cq(10 * jerkAP,fc_filt,3);
  fc_filt = 2000 + 2000 * jerkAP;
};

// NOISE WHOOSH
Soni_J3_Whoosh = no.noise : filter(frequency) : _*(gain) <: _,_ with													// J3 - NOISE WHOOSH
{
  frequency = Soni_J3_minFreq + (Soni_J3_maxFreq - Soni_J3_minFreq) * Soni_X_J3_Whoosh * Soni_X_J3_Whoosh;				// CALCULATE FILTER FC
  filter(x) = _ : fi.resonhp(Soni_J3_HPF_FC,0.7,1) : fi.resonlp(frequency,Soni_J3_LPF_Q,1);								// NOISE FILTER
  gain = Soni_X_J3_Whoosh > 0.01 : si.smoo;																				// CALCULATE ON/OFF CONDITION
};

// BELL TRIGGER
Soni_STS1_Bell = pm.churchBell(1,10000,0.8,1,Soni_X_STS1_Bell) * en.ar(0.001,2,Soni_X_STS1_Bell) <: stereodBGain(15);

// WAH WAH
Soni_STS3_LFO = os.osc(tempo/60);
Soni_STS3_wahMinFreq = 250;			Soni_STS3_wahMaxFreq = 5000;
Soni_STS3_Wah(LFO,minFreq,maxFreq) = _,_ : singleChannelWah,singleChannelWah with
{
  singleChannelWah = _ : fi.peak_eq_cq(currentGain,currentFreq,3.1) : *(ba.db2linear(-0.4*currentGain));
  LFO_unipolar = 0.5*(LFO + 1);
  currentFreq = (minFreq + LFO_unipolar*(maxFreq - minFreq));
  currentGain = 22 * (Soni_X_STS3_Wah);
};

// SPATIALIZE
Soni_D1_Spatialize_PAN = _,_ : _*(M_L),_*(M_R) with
{
  M_L = sqrt(1 - Soni_X_D1_Spatialize) * 1.414;
  M_R = sqrt(Soni_X_D1_Spatialize)  * 1.414;
};

// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // Generation // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // 

// TRIGGERS
TRG_K = velToTrigger(V_K) * (1 - S_isHEEL);
TRG_S = velToTrigger(V_S) * (1 - S_isHEEL);
TRG_HH = velToTrigger(V_HH);
TRG_C_1 = velToTrigger(V_C_1);
TRG_C_2 = velToTrigger(V_C_2);
TRG_C_3 = velToTrigger(V_C_3);
TRG_C_4 = velToTrigger(V_C_4);
TRG_LIST_C = TRG_C_1,TRG_C_2,TRG_C_3,TRG_C_4;
TRG_R = velToTrigger(V_R);
TRG_M = velToTrigger(V_M);
TRG_CS = velToTrigger(V_CS);
TRG_CR = velToTrigger(V_CR);

//Kick

kick_V1 = samplePlayer(K_SMPL_V1,TRG_K + HS_K_TRG);
kick_V2 = samplePlayer(K_SMPL_V2,TRG_K + HS_K_TRG);
kick_V3 = samplePlayer(K_SMPL_V3,TRG_K + HS_K_TRG);
kickSynth = kick_V1,kick_V2,kick_V3 : ba.selectn(3,VAR_1 - 1);
kick_Normal = kickSynth : applyVelocity(V_K,TRG_K,9); 
kickTrack = kick_Normal,(kickSynth * 1.4) :> monoChannel(1) : getPanFunction(0);

//Snare

snare_V1 = samplePlayer(S_SMPL_V1_1,TRG_S + HS_S_TRG), samplePlayer(S_SMPL_V1_2,TRG_S + HS_S_TRG),samplePlayer(S_SMPL_V1_3,TRG_S + HS_S_TRG) : ba.selectn(3,S_FNUM);
snare_V2 = samplePlayer(S_SMPL_V2_1,TRG_S + HS_S_TRG), samplePlayer(S_SMPL_V2_2,TRG_S + HS_S_TRG),samplePlayer(S_SMPL_V2_3,TRG_S + HS_S_TRG) : ba.selectn(3,S_FNUM);
snare_V3 = samplePlayer(S_SMPL_V3_1,TRG_S + HS_S_TRG), samplePlayer(S_SMPL_V3_2,TRG_S + HS_S_TRG),samplePlayer(S_SMPL_V3_3,TRG_S + HS_S_TRG) : ba.selectn(3,S_FNUM);
snareSynth = snare_V1,snare_V2,snare_V3 : ba.selectn(3,VAR_2 - 1);
snare_Normal = snareSynth : applyVelocity(V_S,TRG_S,3);
snareTrack = snare_Normal,(snareSynth * 1.4) :> monoChannel(2) : getPanFunction(0) : Soni_J_Del;																		

//HH

hh_V1 = samplePlayer(HH_SMPL_V1_1,TRG_HH), samplePlayer(HH_SMPL_V1_2,TRG_HH),samplePlayer(HH_SMPL_V1_3,TRG_HH) : ba.selectn(3,HH_FNUM);
hh_V2 = samplePlayer(HH_SMPL_V2_1,TRG_HH), samplePlayer(HH_SMPL_V2_2,TRG_HH),samplePlayer(HH_SMPL_V2_3,TRG_HH) : ba.selectn(3,HH_FNUM);
hh_V3 = pm.marimba(800,0.75,5000,1,1,TRG_HH) * 10;
hhSynth = hh_V1,hh_V2,hh_V3 : ba.selectn(3,VAR_3 - 1);
hhTrack = hhSynth : applyVelocity(V_HH,TRG_HH,3) : monoChannel(3) : getPanFunction(1) : Soni_J_Del;	

//Chord

KEYNUM_LIST_C = KEYNUM_C_1,KEYNUM_C_2,KEYNUM_C_3,KEYNUM_C_4;																// LIST MIDI KEYS
F0_LIST_HZ_C = getChordFinalFreqs(KEYNUM_LIST_C);																			// MIDI KEYS TO FREQ
chordFreq(noteIdx) = F0_LIST_HZ_C : ba.selectn(4,noteIdx);																	// FREQ SELECTOR
chordVel(noteIdx) =  V_LIST_C 	  : ba.selectn(4,noteIdx);																	// VEL SELECTOR
chordTrg(noteIdx) =  TRG_LIST_C	  : ba.selectn(4,noteIdx);																	// TRG SELECTOR
chord_SF_V1(trigger,freq) = pianoSim_singleNote(freq,trigger);																// CHORD - SF VARIANT 1
chord_SF_V2(trigger,freq) = fmSynth_Versatile(freq,MALLET_MRATIO,MALLET_I_FIXED,MALLET_I_ENV,
											  MALLET_A,MALLET_D,MALLET_S,MALLET_R,MALLET_ENVTYPE,trigger,7);				// CHORD - SF VARIANT 2
chord_SF_V3(trigger,freq) = os.CZresTrap(0.5*(1+os.osc(freq)),4.54) * en.are(0.001,2,trigger);
chord_notePanFunc(idx) = ba.take(idx+1,PANPOS_NOTES);
chordSynthFunc(trigger,freq) = chord_SF_V1(trigger,freq), chord_SF_V2(trigger,freq), chord_SF_V3(trigger,freq) : ba.selectn(3,VAR_4 - 1);
chordSum = par(i,4,chordSingle_Synth(chordFreq(i), chord_notePanFunc(i), chordSynthFunc(chordTrg(i))) : stereoEffect(applyVelocity(chordVel(i),chordTrg(i),9))) :> _,_;
chordTrack = chordSum : stereoChannel(4);

//Riff

F0_R = KEYNUM_R : Soni_J1_FreqWarpFactor;																					// CALCULATE F0 HZ
riff_V1 = fmSynth(F0_R,MOD_NUM_R,FREQ_FACTOR_R,RL_R,MOD_DEPTH_R,TRG_R);														// RIFF VARIANT 1
riff_V2 = fmSynth_Versatile(F0_R,BASSLINE_MRATIO,BASSLINE_I_FIXED,BASSLINE_I_ENV,
											  BASSLINE_A,BASSLINE_D,BASSLINE_S,BASSLINE_R,BASSLINE_ENVTYPE,TRG_R,V_R);		// RIFF VARIANT 2
riff_V3 = fmSynth_Versatile(F0_R,AGGRO_MRATIO,AGGRO_I_FIXED,AGGRO_I_ENV,
											  AGGRO_A,AGGRO_D,AGGRO_S,AGGRO_R,AGGRO_ENVTYPE,TRG_R,V_R);						// RIFF VARIANT 2
riffSynth = riff_V1,riff_V2,riff_V3 : ba.selectn(3,VAR_5 - 1);
riffTrack = riffSynth : applyVelocity(V_R,TRG_R,9) : monoChannel(5) : getPanFunction(0);

//Melody Main

F0_M = KEYNUM_M : Soni_J1_FreqWarpFactor;
V_M_SUS = V_M : ba.sAndH(TRG_M);
M_FreqFactor = (F0_M - 300)/700 : si.smooth(ba.tau2pole(0.001));
M_V1(freq) = voiceSynth_FormantBP(freq,V_M_SUS,TRG_M);																		// MELODY SF - VARIANT 1
M_V2(freq) = fmSynth_Versatile(freq,MALLET_MRATIO,MALLET_I_FIXED,MALLET_I_ENV,
											  MALLET_A,MALLET_D,MALLET_S,MALLET_R,MALLET_ENVTYPE,TRG_M,V_M);				// MELODY SF - VARIANT 2
M_V3(freq) = fmSynth_Versatile(freq,TRUMPET_MRATIO,TRUMPET_I_FIXED,TRUMPET_I_ENV,
											  TRUMPET_A,TRUMPET_D,TRUMPET_S,TRUMPET_R,TRUMPET_ENVTYPE,TRG_M,V_M);			// MELODY SF - VARIANT 3
M_SynthFunc(freq) = M_V1(freq),M_V2(freq),M_V3(freq) : ba.selectn(3,VAR_6 - 1);
M_FX1 = dotted_delay(FB_DEL_M,BT_SMPL,WET_DEL_M),dotted_delay(FB_DEL_M,2*BT_SMPL,WET_DEL_M);								// DEFINE STEREO DOTTED DELAY
melodySynth = leadSynth(F0_M,M_SynthFunc,V_M,TRG_M,RL_M,FC_LP_M);														// SYNTHESIZE MELODY
melodyTrack = melodySynth : M_FX1 : stereoChannel(6);

//Chord Stabs

KEYNUM_LIST_CS = KEYNUM_CS_1,KEYNUM_CS_2,KEYNUM_CS_3,KEYNUM_CS_4;
F0_LIST_HZ_CS = getChordFinalFreqs(KEYNUM_LIST_CS);																			// GET F0 LIST
ENV_CS = en.ar(AT_CS,RL_CS,TRG_CS);																							// GET ENV
DEL_CS = stereoEffect(dotted_delay(FB_DEL_CS,BT_SMPL,WET_DEL_CS));															// DEFINE DOTTED DELAY
CS_V1(freq) = ks_excitation(TRG_CS) : ks(freq,DMP_KS_CS);
CS_V2(freq) = os.triangle(freq);
CS_V3(freq) = os.sawtooth(freq);
SynthFunc_CS(freq) = CS_V1(freq),CS_V2(freq),CS_V3(freq) : ba.selectn(3,VAR_7 - 1);
CSTrack = fullChordSynth(F0_LIST_HZ_CS,SynthFunc_CS,ENV_CS) : stereoLinGain(0.3) : DEL_CS;									// CS SYNTH
chordStabTrack = CSTrack : stereoChannel(7);

//Crash

crash_V1 = samplePlayer(CR_SMPL_V1,TRG_CR);
crash_V2 = samplePlayer(CR_SMPL_V2,TRG_CR);
crash_V3 = samplePlayer(CR_SMPL_V3,TRG_CR);
crashSynth = crash_V1,crash_V2,crash_V3 : ba.selectn(3,VAR_8 - 1);
crashTrack = crashSynth : applyVelocity(V_CR,TRG_CR,9) : monoChannel(8) : getPanFunction(2) : Soni_J_Del;

// // // // // // // // // // // // // // // // // // // // // // // // // // // // M A S T E R // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

track1 = kickTrack 		: Soni_SB2_Instrumentation(1)														: stereoMasterSection(1);
track2 = snareTrack 	: Soni_SB2_Instrumentation(2)														: stereoMasterSection(2);
track3 = hhTrack 		: Soni_SB2_Instrumentation(3)			 											: stereoMasterSection(3);
track4 = chordTrack 	: Soni_SB2_Instrumentation(4) 			 											: stereoMasterSection(4);
track5 = riffTrack 		: Soni_SB2_Instrumentation(5)										 				: stereoMasterSection(5);
track6 = melodyTrack 	: Soni_SB2_Instrumentation(6)													 	: stereoMasterSection(6);
track7 = chordStabTrack : Soni_SB2_Instrumentation(7)												 		: stereoMasterSection(7);
track8 = crashTrack 	: Soni_SB2_Instrumentation(8)													 	: stereoMasterSection(8);

percBus = track1,track2,track3,track8;
reverbBus = track2,track3,track4,track6,track7 : par(i,5,stereodBGain(ba.take(i+1,REV_SND))) :> reverbMaster : stereoLinGain(transientMelDip);
melBus = track4,track5,track6,track7,reverbBus :> Soni_STS3_Wah(Soni_STS3_LFO,Soni_STS3_wahMinFreq,Soni_STS3_wahMaxFreq) : stereoLinGain(transientMelDip);
soniBus = Soni_J2_Pitched,Soni_J3_Whoosh,Soni_SB1_Signal_Ambulance,Soni_STS1_Bell
  			:> stereodBGain(SONI_GAIN_DB);
masterChannel = masterComp : stereoEffect(parametricEQ(masterEQGroup)) : stereoEffect(Soni_R3_Filter) : SB_masterPan 
  						   : stereoLinGain(masterGain) : stereoEffect(masterLimiter(0)) : stereoEffect(hard_clip(1));
musicBus = melBus,percBus,reverbBus :> stereoLinGain(musicDuck);

process = musicBus,soniBus :> masterChannel : Soni_D1_Spatialize_PAN;