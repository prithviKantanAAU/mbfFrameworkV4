#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
class UI_SensorConfig
{
public:
	UI_SensorConfig()
	{
		configure();
	};
	~UI_SensorConfig() {};

	String Locations[4] = { "Trunk", "Left Foot", "Right Foot", "<UNASSIGNED>" };
	Label IP_Header;					// Column Header - Sr. No
	Label Status_Header;				// Column Header - Online status
	Label Status[3];					// Column Values - Online status
	Label Port_Header;					// Column Header - UDP Port
	Label Port[3];						// Column Values - UDP Port
	Label Location_Header;				// Column Header - Sensor Bodily Location
	Label BiasComp_Header;				// Column Header - IMU Bias Compensation
	TextButton BiasComp[3];				// Column Button - IMU Bias Compensation
	ComboBox Location[3];				// Column Values - Sensor Bodily Location
	Label BatteryLevel_Header;			// Column Header - Battery Level
	Label BatteryLevel[3];				// Column Values - Battery Level
	TextEditor WLAN_IP[3];				// WLAN
	Label PacketPercent_Header;			// Column Header - Fraction of Packets Received
	Label PacketPercent[3];				// Column Values - Fraction of Packets Received

	void addElements()
	{

	}

	void configure()
	{
		IP_Header.setText("IP Address", dontSendNotification);
		Status_Header.setText("Status", dontSendNotification);
		Port_Header.setText("UDP Port", dontSendNotification);
		Location_Header.setText("Body Location", dontSendNotification);
		BiasComp_Header.setText("Bias Compensation", dontSendNotification);
		BatteryLevel_Header.setText("Battery %", dontSendNotification);
		PacketPercent_Header.setText("Recd. Packet %", dontSendNotification);
		for (int i = 0; i < 3; i++)
		{
			Location[i].addItem(Locations[0], 1);
			Location[i].addItem(Locations[1], 2);
			Location[i].addItem(Locations[2], 3);
			Location[i].addItem(Locations[3], 4);
			Location[i].setSelectedId(4);						
			
			BiasComp[i].setButtonText("Calibrate");				
			BiasComp[i].setColour(BiasComp[i].buttonColourId, Colours::red);
		}
	}

	void toggleVisible(bool on)
	{
		IP_Header.setVisible(on);
		Status_Header.setVisible(on);
		Location_Header.setVisible(on);
		Port_Header.setVisible(on);
		BiasComp_Header.setVisible(on);
		BatteryLevel_Header.setVisible(on);
		PacketPercent_Header.setVisible(on);
		for (int i = 0; i < 3; i++)
		{
			Location[i].setVisible(on);
			Port[i].setVisible(on);
			WLAN_IP[i].setVisible(on);
			Status[i].setVisible(on);
			BiasComp[i].setVisible(on);
			BatteryLevel[i].setVisible(on);
			PacketPercent[i].setVisible(on);
		}
	}

	void setLayout()
	{
		IP_Header.setBounds(100, 400, 100, 40);
		Status_Header.setBounds(200, 400, 100, 40);
		Port_Header.setBounds(300, 400, 100, 40);
		Location_Header.setBounds(400, 400, 100, 40);
		BiasComp_Header.setBounds(550, 400, 150, 40);
		BatteryLevel_Header.setBounds(710, 400, 80, 40);
		PacketPercent_Header.setBounds(800, 400, 150, 40);

		for (int i = 0; i < 3; i++)
		{
			WLAN_IP[i].setBounds(80, 400 + 50 * (i + 1), 120, 40);
			Status[i].setBounds(200, 400 + 50 * (i + 1), 100, 40);
			Port[i].setBounds(300, 400 + 50 * (i + 1), 100, 40);
			Location[i].setBounds(400, 400 + 50 * (i + 1), 100, 40);
			BiasComp[i].setBounds(550, 400 + 50 * (i + 1), 120, 40);
			BatteryLevel[i].setBounds(710, 400 + 50 * (i + 1), 80, 40);
			PacketPercent[i].setBounds(800, 400 + 50 * (i + 1), 150, 40);
		}
	}
};

