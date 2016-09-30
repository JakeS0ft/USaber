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
 * RGBBlade.cpp
 *
 *  Created on: Jul 9, 2016
 *      Author: JakeSoft <http://forum.arduino.cc/index.php?topic=261980.0>
 */

#include "blade/RGBBlade.h"
#include <Arduino.h>

RGBBlade::RGBBlade(int aCh1Pin, int aCh2Pin, int aCh3Pin)
{
	mLedPins[0] = aCh1Pin;
	mLedPins[1] = aCh2Pin;
	mLedPins[2] = aCh3Pin;

	for(int lIdx = 0; lIdx < 3; lIdx++)
	{
		mPowerLevels[lIdx] = 0;
	}

	mIsOn = false;
	mLastFlickerUpdate = 0;
}

RGBBlade::~RGBBlade()
{
	Off();
}

void RGBBlade::Init()
{
	for(int lChnl = 0; lChnl < 3; lChnl++)
	{
		pinMode(mLedPins[lChnl], OUTPUT);
	}

	Off();
}

void RGBBlade::SetChannel(unsigned char aLevel, int aChannel = 0)
{

	if(aChannel < 3)
	{
		mPowerLevels[aChannel] = aLevel;
	}
}

void RGBBlade::On()
{
	PerformIO();
	mIsOn = true;
}

void RGBBlade::Off()
{
	for(int lIdx = 0; lIdx < 3; lIdx++)
	{
		mPowerLevels[lIdx] = 0;
	}

	PerformIO();
	mIsOn = false;
}

bool RGBBlade::PowerUp(int aRampTime)
{
	const int lRampStages = 20;
	int lDelay = aRampTime;
	lDelay = int((float)lDelay / (float)lRampStages);

	//Ramp up the blade to full brightness
	for(int lStage = lRampStages; lStage > 0; lStage--)
	{
		delay(lDelay);

		for(int lChannel = 0; lChannel < 3; lChannel++)
		{
			int lPower = (int)((float)mPowerLevels[lChannel]/(float)lStage);
			analogWrite(mLedPins[lChannel], lPower );
		}
	}

	On();

	return true;
}

bool RGBBlade::PowerDown(int aRampTime)
{
	 const int lRampStages = 10;
	 int lDelay = aRampTime;
	 lDelay = lDelay / lRampStages;

	 //Ramp down the blade to off
	 for(int lnStage = 1; lnStage < lRampStages; lnStage++)
	 {
		 delay(lDelay);
		 for(int lChannel = 0; lChannel < 3; lChannel++)
		 {
			 int lPower = (int)((float)mPowerLevels[lChannel]/(float)lnStage);
			 analogWrite(mLedPins[lChannel],  lPower);
		 }
	 }

	 Off();

	 return true;
}

void RGBBlade::ApplyFlicker(int aType)
{
	static unsigned long lLastUpdate;


	switch(aType)
	{
	case 0:
		//Do nothing
		break;
	case 1: //Random flicker
		RandomFlicker(80, 100, 20);
		break;
	case 2:
		RandomFlicker(60, 100, 20);
		break;
	case 3:
		RandomFlicker(50, 100, 50);
		break;
	default:
		//Do nothing
		break;
	}
}

void RGBBlade::PerformIO()
{
	for(int lIdx = 0; lIdx < 3; lIdx++)
	{
		if(mLedPins[lIdx] > 0)
		{
			analogWrite(mLedPins[lIdx], mPowerLevels[lIdx]);
		}
	}
}

BladeMetadata RGBBlade::GetFeatures()
{
	BladeMetadata lData;

	lData.Channels = 3;
	lData.Flickers = 4;

	return lData;
}

void RGBBlade::RandomFlicker(int aLowerBound, int aUpperBound, unsigned int aUpdatePeriod)
{
	unsigned char lTempPowerLevels[3];

	//Compensate for values too high
	if(aLowerBound > 100)
	{
		aLowerBound = 100;
	}
	if(aUpperBound > 100)
	{
		aUpperBound = 100;
	}

	if(millis() - mLastFlickerUpdate > aUpdatePeriod)
	{
		//Generate a random number between lower and upper bounds
		int lRandomModifier = random(aLowerBound, aUpperBound);
		//Convert the integer value to a floating point number
		float lRandomMultiplier = ((float)lRandomModifier)/100;

		for(int lChnl = 0; lChnl < 3; lChnl++)
		{
			float lTempPowerLevel = (float)mPowerLevels[lChnl] * lRandomMultiplier;
			lTempPowerLevels[lChnl] = (unsigned char)lTempPowerLevel;
		}

		//Do I/O to make flicker take effect
		for(int lIdx = 0; lIdx < 3; lIdx++)
		{
			if(mLedPins[lIdx] > 0)
			{
				analogWrite(mLedPins[lIdx], lTempPowerLevels[lIdx]);
			}
		}

		mLastFlickerUpdate = millis();
	}
}
