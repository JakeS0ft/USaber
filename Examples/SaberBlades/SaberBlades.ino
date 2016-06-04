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
 * SaberBlades.ino
 *  This will test saber blades!
 *  Created on: Mar 8, 2016
 *      Author: JakeSoft <http://forum.arduino.cc/index.php?topic=261980.0>
 */

#include <USaber.h>

//These are all of the PWM-capable pins on an Arduino Pro Mini or Nano.
//Modify these if necessary for whatever flavor board you are using.
#define LED_PIN1 3
#define LED_PIN2 5
#define LED_PIN3 6
#define LED_PIN4 9
#define LED_PIN5 10
#define LED_PIN6 11

/**
 * This function will test a blade manager. It stimulates all of the control
 * methods so that output response can be verified.
 */
void TestBlade(IBladeManager* apBlade)
{
	bool lPowerupComplete = false;
	bool lPowerdownComplete = false;

	apBlade->Init();

	Serial.println("On()");
    apBlade->On();
	delay(2000);

	Serial.println("Off()");
	apBlade->Off();
	delay(2000);

	Serial.println("PowerUp(2000)");
	while(!lPowerupComplete)
	{
		apBlade->SetChannel(255, 0);
		lPowerupComplete = apBlade->PowerUp(2000);
	}
	delay(1500);

	Serial.println("PowerDown(2000)");
	while(!lPowerdownComplete)
	{
		lPowerdownComplete = apBlade->PowerDown(2000);
	}
	delay(1500);

	//Note: Testing flicker patterns is skipped if no patterns are supported
	for(int lFlicker = 0; lFlicker < apBlade->GetFeatures().Flickers; lFlicker++)
	{
		Serial.print("Flicker pattern ");
		Serial.println((int)lFlicker);

		long lStartTime = millis();

		//Turn on the blade
		apBlade->On();

		//Apply flicker for 5 seconds for
		while(millis() - lStartTime < 5000)
		{
			apBlade->ApplyFlicker(lFlicker);
		}

		//Turn off the blade
		apBlade->Off();

		delay(1000);
	}

	//Test setting individual channels
	for(int lChannel = 0; lChannel < apBlade->GetFeatures().Channels; lChannel++)
	{
		Serial.print("Channel ");
		Serial.print(lChannel);
		for(int lPower = 0; lPower <= 255; lPower++)
		{
			apBlade->SetChannel(lPower, lChannel);
			apBlade->PerformIO();
			delay(1);
		}
		delay(1500);
		for(int lPower = 255; lPower >= 0; lPower--)
		{
			apBlade->SetChannel(lPower, lChannel);
			apBlade->PerformIO();
			delay(1);
		}

		apBlade->Off();
	}

	delay(1000);
}

/**
 * The setup function is called once at startup of the sketch
 */
void setup()
{
  Serial.begin(9600);
}

/**
 * The loop function is called in an endless loop
 */
void loop()
{

  Serial.println("\nTesting CheapieBlade");
  IBladeManager* lpBlade = new CheapieBlade(LED_PIN1);
  TestBlade(lpBlade);
  delete lpBlade;

  Serial.println("\n\nTesting SingleLedBlade");
  lpBlade = new SingleLedBlade(LED_PIN1);
  TestBlade(lpBlade);
  delete lpBlade;

  Serial.println("\n\nTesting StringBlade");
  lpBlade = new StringBlade(LED_PIN1, LED_PIN2, LED_PIN3,
                                  LED_PIN4, LED_PIN5, LED_PIN6);
  TestBlade(lpBlade);
  delete lpBlade;

  Serial.println("\n\nTesting CrossGuardBlade");
  lpBlade = new CrossGuardBlade(LED_PIN1, LED_PIN2, 500);
  TestBlade(lpBlade);
  delete lpBlade;
  
  delay(5000);

}

