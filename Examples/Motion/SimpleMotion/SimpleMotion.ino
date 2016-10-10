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
 * SimpleMotion.ino
 *    This will test the simple motion manager!
 *  Created on: Oct 9, 2016
 *      Author: JakeSoft <http://forum.arduino.cc/index.php?topic=261980.0>
 */

//Note: Ardino IDE Versions previous to 1.6.6 must include Wire.h
#include <USaber.h>

/**
 * This function will test a motion manager. Run it in a loop and use
 * serial monitoring to see when motion events are triggered.
 */
void TestMotion(AMotionManager* apMotion)
{
	apMotion->Init();

	unsigned long lTestStartTime = millis();

	//Run the test for 30 seconds
	while(millis() - lTestStartTime <= 30000)
	{
		apMotion->Update();

		if(apMotion->IsSwing())
		{
			Serial.print("Swing detected:");
			switch(apMotion->GetSwingMagnitude())
			{
			case eeSmall:
				Serial.println("Small");
				break;
			case eeMedium:
				Serial.println("Medium");
				break;
			case eeLarge:
				Serial.println("Large");
				break;
			default:
				Serial.println("Unknown");
				break;
			}
		}

		if(apMotion->IsClash())
		{
			Serial.print("Clash detected:");
			switch(apMotion->GetClashMagnitude())
			{
			case eeSmall:
				Serial.println("Small");
				break;
			case eeMedium:
				Serial.println("Medium");
				break;
			case eeLarge:
				Serial.println("Large");
				break;
			default:
				Serial.println("Unknown");
				break;
			}
		}
	}

}

void setup()
{
	Serial.begin(9600);
}

void loop()
{
	Serial.println("Testing SimpleMotionManager");

	//Set this to the pin connected to your swing sensor
	const int lSwingPin = 7;
	//Set this to the pin connect to your clash sensor
	//NOTE: This must be a pin capable of handling an external interrupt
	const int lClashPin = 2;

	AMotionManager* lpMotion = new SimpleMotionManager(lSwingPin, lClashPin);

  TestMotion(lpMotion);
  Serial.println("Test ends.");

	delete lpMotion;
	delay(2000);

}
