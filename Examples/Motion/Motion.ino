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
 * Motion.ino
 *    This will test the motion managers!
 *  Created on: Mar 12, 2016
 *      Author: JakeSoft <http://forum.arduino.cc/index.php?topic=261980.0>
 */

//Note: Ardino IDE Versions previous to 1.6.6 must include Wire.h
#include <Wire.h>
#include <USaber.h>



#define SMALL_TRESHOLD   0.999f
#define MEDIUM_TRESHOLD  0.99f
#define LARGE_TRESHOLD   0.98f


//Tolerance threshold data for MPU6050 motion manager
MPU6050TolData gToleranceData;

/**
 * This function will test a motion manager. Run it in a loop and use
 * serial monitoring to see when motion events are triggered.
 */
void TestMotion(AMotionManager* apMotion)
{
	apMotion->Init();

	unsigned long lTestStartTime = millis();
	unsigned long swingSuppress = 0;
	unsigned long clashSuppress = 0;

	//Run the test for 30 seconds
	while(millis() - lTestStartTime <= 30000)
	{
		apMotion->Update();

		if(apMotion->IsSwing() and millis() - swingSuppress > 400 )
		{
			swingSuppress = millis();
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

		if(apMotion->IsClash() and millis() - clashSuppress > 400)
		{
			clashSuppress = millis();
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

	//Set MPU6050 swing tolerance threasholds here.
	//Tweak as needed to adjust sensitivity
	gToleranceData.mSwingLarge  = SMALL_TRESHOLD;
	gToleranceData.mSwingMedium = MEDIUM_TRESHOLD;
	gToleranceData.mSwingSmall  = SMALL_TRESHOLD;

	Serial.begin(9600);
}

void loop()
{

	/** SIMPLE Devices (Uncomment to test Simple Motion Manager) **
	Serial.println("Testing SimpleMotionManager");

	//Set this to the pin connected to your swing sensor
	const int lSwingPin = 7;
	//Set this to the pin connect to your clash sensor
	//NOTE: This must be a pin capable of handling an external interrupt
	const int lClashPin = 2;

	AMotionManager* lpMotion = new SimpleMotionManager(lSwingPin, lClashPin);
	/** END SIMPLE Devices test setup **/


	/** MPU6050 Devices (Uncomment to test MPU6050 Motion Manager) **/
	Serial.println("Testing MPU6050MotionManager");
	AMotionManager* lpMotion = new Mpu6050MotionManager(&gToleranceData);

	lpMotion->Init();

	TestMotion(lpMotion);
	Serial.println("Test ends.");
	/** END MPU6050 Devices test setup **/

	delete lpMotion;
	delay(2000);

}
