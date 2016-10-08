/*
 This file is part of the Universal Saber library.

 The Universal Saber library is free software: you can redistribute it
 and/or modify it under the terms of the GNU General Public License as
 published by the Free Software Foundation, either version 3 of the License,
 or (at your option) any later version.

 The Universal Saber library is distributed in the hope that it will be
 useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with the Universal Saber library.  If not, see <http://www.gnu.org/licenses/>.
 */
/*
 * ADXL335MotionManagerTest.ino
 *
 *  This sketch will demonstrate the ADXL335-based motion detection. When motion
 *  is detected, notifications will be sent via serial comms.
 *
 *  Created on: Oct 2, 2016
 *      Author: JakeSoft <http://forum.arduino.cc/index.php?topic=261980.0>
 */

#include "Arduino.h"
#include <USaber.h>

//Input pins for sensor
#define AXDL335_X_PIN A0
#define AXDL335_Y_PIN A1
#define AXDL335_Z_PIN A2

//Input pins for adjustment switches (optional)
#define UP_BUTTON_PIN 7
#define DOWN_BUTTON_PIN 4

Adxl335MotionManager* gpMotion;

Adxl335TolData gTolData;

void setup()
{
	//You may have to adjust these values to your preferences
	gTolData.mSwingLarge = 45;
	gTolData.mSwingMedium = 30;
	gTolData.mSwingSmall = 10;
	gTolData.mClash = 210;

	pinMode(AXDL335_X_PIN, INPUT);
	pinMode(AXDL335_Y_PIN, INPUT);
	pinMode(AXDL335_Z_PIN, INPUT);

	//Set up clash adjustment buttons (optional)
	pinMode(UP_BUTTON_PIN, INPUT_PULLUP);
	pinMode(DOWN_BUTTON_PIN, INPUT_PULLUP);

	//Create a new motion manager object
	gpMotion = new Adxl335MotionManager(AXDL335_X_PIN, AXDL335_Y_PIN, AXDL335_Z_PIN, &gTolData);

	//Initialize it
	gpMotion->Init();

	delay(100);
	Serial.begin(9600);

	Serial.println("Boot up complete.");

}

void loop()
{
	//Can optionally wire switches to adjust clash sensitivity
	//Leaving these floating is OK, just have to adjust tolerance
	//data in setup() instead
	if(LOW == digitalRead(UP_BUTTON_PIN))
	{
		gTolData.mClash++;
		delay(100);

		Serial.print("Clash tol:");
		Serial.println(gTolData.mClash);
	}
	else if(LOW == digitalRead(DOWN_BUTTON_PIN))
	{
		gTolData.mClash--;
		delay(100);

		Serial.print("Clash tol:");
		Serial.println(gTolData.mClash);
	}

	//Check for motion
	gpMotion->Update();

	if(gpMotion->IsClash())
	{
		Serial.println("CLASH!");
	}
	else if(gpMotion->IsSwing())
	{
		Serial.print("Swing! ");

		if(eeSmall == gpMotion->GetSwingMagnitude())
		{
			Serial.println(" Small");
		}
		else if(eeMedium == gpMotion->GetSwingMagnitude())
		{
			Serial.println(" Medium");
		}
		else if(eeLarge == gpMotion->GetSwingMagnitude())
		{
			Serial.println(" Large");
		}

		//Wait for the swing to end, keep checking for clashes
		while(gpMotion->IsSwing())
		{
			gpMotion->Update();

			if(gpMotion->IsClash())
			{
				Serial.println("CLASH!");
			}
		}
	}

}
