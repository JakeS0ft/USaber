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
 * SingleLedBlade.cpp
 *
 *  Created on: Mar 1, 2016
 *      Author: JakeSoft <http://forum.arduino.cc/index.php?topic=261980.0>
 */

#include "blade/SingleLedBlade.h"
#include <Arduino.h>

SingleLedBlade::SingleLedBlade(int aLedPin)
{
	mLedPin = aLedPin;
	mLedPowerLevel = 0;
}

SingleLedBlade::~SingleLedBlade()
{
	//Turn off the blade
	Off();
}

void SingleLedBlade::Init()
{
	pinMode(mLedPin, OUTPUT);
	Off();
}

bool SingleLedBlade::PowerUp(int aRampTime)
{
	const int lRampStages = 10;
	int lDelay = aRampTime;
	lDelay = int((float)lDelay / (float)lRampStages);


	//Ramp up the blade to full brightness
	for(int lnStage = lRampStages; lnStage > 0; lnStage--)
	{
		delay(lDelay);
		int lPower = (int)((float)mLedPowerLevel/(float)lnStage);
		analogWrite(mLedPin, lPower );
	}

	On();

	return true;
}

 bool SingleLedBlade::PowerDown(int anRampTime)
 {
	 const int lRampStages = 10;
	 int lDelay = anRampTime;
	 lDelay = lDelay / lRampStages;

	 //Ramp down the blade to off
	 for(int lnStage = 1; lnStage < lRampStages; lnStage++)
	 {
		 delay(lDelay);
		 int lPower = (int)((float)mLedPowerLevel/(float)lnStage);
		 analogWrite(mLedPin,  lPower);
	 }

	 Off();

	 return true;
 }

void SingleLedBlade::ApplyFlicker(int aEffect)
{
	static int lLastUpdate;

	switch(aEffect)
	{
	case 0:
		//Do nothing
		break;
	case 1:
		if(millis() - lLastUpdate > 20)
		{
			this->mLedPowerLevel = random(100, 255);
			PerformIO();
			lLastUpdate = millis();
		}
		break;
	case 2:
		if(millis() - lLastUpdate > 20)
		{
			this->mLedPowerLevel = random(190, 255);
			PerformIO();
			lLastUpdate = millis();
		}
		break;
	}

}

void SingleLedBlade::SetChannel(unsigned char aLevel, int aChannel = 0)
{
	this->mLedPowerLevel = aLevel;
}

void SingleLedBlade::PerformIO()
{
	analogWrite(mLedPin, mLedPowerLevel);
}

void SingleLedBlade::Off()
{
	mLedPowerLevel = 0;
	PerformIO();

}

void SingleLedBlade::On()
{
	mLedPowerLevel = 255;
	PerformIO();
}

BladeMetadata  SingleLedBlade::GetFeatures()
{
	BladeMetadata lFeatures;

	//Number of channels
	lFeatures.Channels = 1;
	//Number of flicker patterns
	lFeatures.Flickers = 3;

	return lFeatures;
}
