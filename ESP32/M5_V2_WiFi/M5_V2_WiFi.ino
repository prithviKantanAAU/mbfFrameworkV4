#define processing_out false
#define AHRS true         // Set to false for basic data read
#define SerialDebug true  // Set to true to get Serial output for debugging
#define LCD

#include <M5Stack.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include "utility/MPU9250.h"

// CHANGE WHEN REPROGRAMMING SENSORS

short sensorIdx = 1;                            // SENSOR NUMBER

// CHANGE WHEN PROGRAMMING FOR NEW NETWORK

char ssid[] = "Prithvi";                        // your network SSID (name)
char pass[] = "RhythmSynth";                    // your network password

// WIFI, IMU OBJECTS, METADATA

MPU9250 IMU;
WiFiUDP Udp;                                    // UDP INSTANCE
bool isConnected = false;                       // CONNECTED TO WIFI NETWORK OF PC
IPAddress remote_IP_REC(0,0,0,0);               // WILL AUTOMATICALLY FIND REMOTE IP FROM INCOMING PACKET
int Port_REMOTE[3] = {9999, 9998, 9997};        // REMOTE PORT (PC END FOR SENDING)
int Port_LOCAL[3] = {8888, 8887, 8886};         // LOCAL PORT (ESP32 END FOR RECEIVING)
const String OSCAddress[3] = {"/S1","/S2","S3"};      // OSC ADDRESSES FOR THE 3 SENSORS

// TIMING VARIABLES

int timeElapsedMS = 0;                          // TOTAL TIME ELAPSED (MS)
int screenTimeOutMS = 0;                        // SCREEN TIMEOUT FOR AUTOMATIC TURNOFF
bool isScreenOn = false;                        // IS SCREEN ON?
int lastBatteryValue = 0;                       // LAST BATTERY LEVEL
int sampleInterval = 8;                         // IMU SAMPLING INTERVAL IN MS
unsigned long currentMill = 0;              
unsigned long prevMill = 0;

void setup() {
    M5.begin();
    Wire.begin();
    M5.Lcd.setTextSize(2);
    M5.Lcd.setBrightness(0);
    IMU.initMPU9250();
    M5.Lcd.setCursor(0,0);
    M5.Lcd.print("Not Connected - IDLE");    
}

void updateBatteryInfo()
{
  if(M5.Power.isChargeFull()) 
    { 
      M5.Lcd.setCursor(0,220); 
      M5.Lcd.print("Charge: FUL");
      M5.Power.setCharge(false);
    }

    else if(M5.Power.isCharging())
    { 
      M5.Lcd.setCursor(0,220); 
      M5.Lcd.print("Charge: WIP");
    }
    else
    {
      M5.Lcd.setCursor(0,220); 
      M5.Lcd.print("Charge: NOT");
    }

   M5.Lcd.setCursor(170,220); 
   int currentBattery = getBatteryLevel();
   if (currentBattery != lastBatteryValue)
   {
      M5.Speaker.beep();
   }
   M5.Lcd.print("BAT: " + String(getBatteryLevel()));
   lastBatteryValue = currentBattery;
}

int8_t getBatteryLevel() {
    Wire.beginTransmission(0x75);
    Wire.write(0x78);
    if (Wire.endTransmission(false) == 0 && Wire.requestFrom(0x75, 1)) {
        switch (Wire.read() & 0xF0) {
        case 0xE0: return 25;
        case 0xC0: return 50;
        case 0x80: return 75;
        case 0x00: return 100;
        default: return 0;
        }
    }
    return -1;
}

void checkConnectionStatus()
{
  if (isConnected && WiFi.status() != WL_CONNECTED)
  {
    M5.Lcd.print("Connection Lost");
    isConnected = false;
    WiFi.begin(ssid, pass);
  }
  if (!isConnected && WiFi.status() == WL_CONNECTED)
  {
    M5.Lcd.print("Reconnected");
    isConnected = true;
  }
}

void connectToPC()
{
  M5.Lcd.setCursor(0,0);
  M5.Lcd.print("                       ");
  bool isIPVerified = false;
  WiFi.begin(ssid, pass);
    M5.Lcd.setCursor(0,0);
    M5.Lcd.print("Connecting");
    while (WiFi.status() != WL_CONNECTED) {
        M5.Lcd.print(".");
        delay(500);
    }

    M5.Lcd.setBrightness(0);
    isConnected = true;
    WiFi.setTxPower(WIFI_POWER_11dBm);
    M5.Lcd.setCursor(0,0);
    M5.Lcd.print("                 ");
    M5.Lcd.setCursor(0,20);
    M5.Lcd.print("Connected - " + String(Port_REMOTE[sensorIdx]));
    Udp.begin(Port_LOCAL[sensorIdx]);

    while(!isIPVerified)
    {
      M5.Lcd.setBrightness(50);
      M5.Lcd.setCursor(0,40);
      IPAddress local_IP = WiFi.localIP();
      M5.Lcd.print("CONNECT TO ");
      M5.Lcd.setCursor(0,60);
      M5.Lcd.print(local_IP);
      int packetSize = Udp.parsePacket();
      if(packetSize > 0)
        {
          remote_IP_REC = Udp.remoteIP();
          isIPVerified = true;
          M5.Lcd.setCursor(0,80);
          M5.Lcd.print(remote_IP_REC);
          M5.Lcd.setCursor(0,100);
          M5.Lcd.print("VERIFIED");
        }
        delay(500);
    }
}

void fetchIMUData()
{
  if (IMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
    {
    IMU.readAccelData(IMU.accelCount);  // Read the x/y/z adc values
    IMU.getAres();
    IMU.ax = (float)IMU.accelCount[0]*IMU.aRes; // - accelBias[0];
    IMU.ay = (float)IMU.accelCount[1]*IMU.aRes; // - accelBias[1];
    IMU.az = (float)IMU.accelCount[2]*IMU.aRes; // - accelBias[2];

    IMU.readGyroData(IMU.gyroCount);  // Read the x/y/z adc values
    IMU.getGres();
    IMU.gx = (float)IMU.gyroCount[0]*IMU.gRes;
    IMU.gy = (float)IMU.gyroCount[1]*IMU.gRes;
    IMU.gz = (float)IMU.gyroCount[2]*IMU.gRes;
    
    IMU.updateTime();
    IMU.count = millis();
    IMU.sumCount = 0;
    IMU.sum = 0;
    }
}
void buildAndSendOSCMsg()
{
  if (isConnected)
    {
    // BUILD OSC MESSAGE
    OSCMessage msg(OSCAddress[sensorIdx].c_str());
    msg.add(IMU.ax);
    msg.add(IMU.ay);
    msg.add(IMU.az);    
    msg.add(IMU.gx);
    msg.add(IMU.gy);
    msg.add(IMU.gz);
    msg.add((float)lastBatteryValue);

    // CREATE UDP PACKET AND SEND MESSAGE
    Udp.beginPacket(remote_IP_REC, Port_REMOTE[sensorIdx]);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty(); 
    }
}

void loop() 
{
  currentMill = millis();
  if (currentMill - prevMill > sampleInterval)
  {

  // HANDLE BUTTON A PRESS
    
  if(M5.BtnA.wasPressed()) { M5.Power.powerOFF(); }

  // HANDLE BUTTON B PRESS
  
  if(M5.BtnB.wasPressed())
  {
    if (!isConnected)
    connectToPC();
  }

  // HANDLE BUTTON C PRESS
  
  if(M5.BtnC.wasPressed()) 
  {  
    updateBatteryInfo();
    M5.Lcd.setBrightness(10);
    isScreenOn = true;
  }

  // HANDLE TIMERS
  
  timeElapsedMS = (timeElapsedMS + 10) % 1000;
  if (timeElapsedMS == 0) { updateBatteryInfo(); }

  // HANDLE SCREEN
  
  if (isScreenOn)
  {
    screenTimeOutMS += 10;
    if (screenTimeOutMS >= 1000)
    {
      M5.Lcd.setBrightness(0);
      isScreenOn = false;
      screenTimeOutMS = 0;
    }  
  }

  // HANDLE IMU AND OSC DATA

  if (isConnected)
  {
    fetchIMUData();
    buildAndSendOSCMsg();
  }

  // UPDATE M5 AND SHUFFLE
  
    M5.update();
    prevMill = currentMill;
  }
}
