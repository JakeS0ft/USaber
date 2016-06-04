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
 * CrossGuardBlade.cpp
 *
 *  Created on: Mar 11, 2016
 *      Author: JakeSoft <http://forum.arduino.cc/index.php?topic=261980.0>
 */

#include "blade/CrossGuardBlade.h"
#include <Arduino.h>

CrossGuardBlade::CrossGuardBlade(int aMainPin, int aCrossPin, int aCrossDelay)
//Call superclass constructor
:SingleLedBlade(aMainPin)
{
	mCrossPin = aCrossPin;
	mCrossPowerLevel = 0;

	if(mCrossDelay > 0)
	{
		mCrossDelay = aCrossDelay;
	}
	else
	{
		mCrossDelay = 0;
	}

}

CrossGuardBlade::~CrossGuardBlade()
{
	Off();
}

void CrossGuardBlade::Init()
{
	SingleLedBlade::Init();

	pinMode(mCrossPin, OUTPUT);
}

bool CrossGuardBlade::PowerUp(int aRampTime)
{
	bool lMainBladeDone = false;
	while(!lMainBladeDone)
	{
		lMainBladeDone = SingleLedBlade::PowerUp(aRampTime - mCrossDelay);
	}

	mCrossPowerLevel = 255;
	PerformIO();

	delay(mCrossDelay);

	return true;
}

bool CrossGuardBlade::PowerDown(int aRampTime)
{
	mCrossPowerLevel = 0;
    PerformIO();

  	return SingleLedBlade::PowerDown(aRampTime);
}

void CrossGuardBlade::ApplyFlicker(int aEffect)
{
	SingleLedBlade::ApplyFlicker(aEffect);

	//Have the cross-guard copy whatever the main blade is doing
	mCrossPowerLevel = mLedPowerLevel;
	PerformIO();
}

void CrossGuardBlade::SetChannel(unsigned char aLevel, int aChannel)
{
	switch(aChannel)
	{
	case 0:
		SingleLedBlade::SetChannel(aLevel, aChannel);
		break;
	case 1:
		mCrossPowerLevel = aLevel;
		break;
	default:
		//Do nothing
		break;
	}
}

void CrossGuardBlade::PerformIO()
{
	SingleLedBlade::PerformIO();

	analogWrite(mCrossPin, mCrossPowerLevel);
}

void CrossGuardBlade::Off()
{
	SingleLedBlade::Off();
	mCrossPowerLevel = 0;
	PerformIO();
}

void CrossGuardBlade::On()
{
	SingleLedBlade::On();
	mCrossPowerLevel = 255;
	PerformIO();
}

BladeMetadata CrossGuardBlade::GetFeatures()
{
	//Call superclass to get features
	BladeMetadata lFeatures = SingleLedBlade::GetFeatures();

	//Override select features
	lFeatures.Channels = 2;

	return lFeatures;
}
