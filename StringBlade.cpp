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
 * StringBlade.cpp
 *
 *  Created on: Mar 7, 2016
 *      Author: JakeSoft <http://forum.arduino.cc/index.php?topic=261980.0>
 */

#include "blade/StringBlade.h"
#include <Arduino.h>

StringBlade::StringBlade(int aSeg1, int aSeg2, int aSeg3,
		                 int aSeg4, int aSeg5, int aSeg6)
{
	mChannelPins[0] = aSeg1;
	mChannelPins[1] = aSeg2;
	mChannelPins[2] = aSeg3;
	mChannelPins[3] = aSeg4;
	mChannelPins[4] = aSeg5;
	mChannelPins[5] = aSeg6;

	for(int lIdx = 0; lIdx < 6; lIdx++)
	{
		mPowerLevels[lIdx] = 0;
	}

	mIsOn = false;

}

StringBlade::~StringBlade()
{
	Off();
}

void StringBlade::Init()
{
	for(int lIdx = 0; lIdx < 6; lIdx++)
	{
		pinMode(mChannelPins[lIdx], OUTPUT);
	}

	PerformIO();
}

void StringBlade::SetChannel(unsigned char aPower, int aChannel)
{
	if(aChannel < 6)
	{
		mPowerLevels[aChannel] = aPower;
	}
}

void StringBlade::On()
{
	for(int lIdx = 0; lIdx < 6; lIdx++)
	{
		mPowerLevels[lIdx] = 255;
	}

	PerformIO();
	mIsOn = true;
}

void StringBlade::Off()
{
	for(int lIdx = 0; lIdx < 6; lIdx++)
	{
		mPowerLevels[lIdx] = 0;
	}

	PerformIO();
	mIsOn = false;
}

bool StringBlade::PowerUp(int aRampTime)
{
	int lDelay = aRampTime/6;

	for(int lPin = 0; lPin <= 6; lPin++)
	{
		analogWrite(mChannelPins[lPin], 255);
		if(lPin < 6)
		{
			analogWrite(mChannelPins[lPin+1], 128);
		}
		delay(lDelay);
	}
	On();
	return true;
}

bool StringBlade::PowerDown(int aRampTime)
{
	int lDelay = aRampTime/6;

	for(int lPin = 6; lPin >= 0; lPin--)
	{
		analogWrite(mChannelPins[lPin], 0);
		if(lPin > 0)
		{
			analogWrite(mChannelPins[lPin-1], 128);
		}
		delay(lDelay);
	}
	Off();
	return true;
}

void StringBlade::ApplyFlicker(int aType)
{
	//Do nothing
}

void StringBlade::PerformIO()
{
	for(int lIdx = 0; lIdx < 6; lIdx++)
	{
		if(mChannelPins[lIdx] > 0)
		{
			analogWrite(mChannelPins[lIdx], mPowerLevels[lIdx]);
		}
	}

}

BladeMetadata StringBlade::GetFeatures()
{
	BladeMetadata lData;

	lData.Channels = 6;
	lData.Flickers = 0;

	return lData;
}
