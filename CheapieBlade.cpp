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
 * CheapieBlade.cpp
 *
 *  Created on: Mar 3, 2016
 *      Author: JakeSoft <http://forum.arduino.cc/index.php?topic=261980.0>
 */

#include <Arduino.h>
#include "blade/CheapieBlade.h"

CheapieBlade::CheapieBlade(int aLedPin)
{
	mLedPin = aLedPin;
	mLedPowerLevel = 0;
}

CheapieBlade::~CheapieBlade()
{
	//Ensure the blade is off
	Off();
}

void CheapieBlade::Init()
{
	pinMode(mLedPin, OUTPUT);
	Off();
}

bool CheapieBlade::PowerUp(int aRampTime)
{
	for(int i = 0; i < 2; i++)
	{
		int lDelay = aRampTime/4;
		analogWrite(mLedPin, 254);
		delay(lDelay);
		analogWrite(mLedPin, 0);
		delay(lDelay);
	}

	On();

	return true;
}

/**
 * Power down like a cheap toy saber!
 */
bool CheapieBlade::PowerDown(int aRampTime)
{
	for(int i = 0; i < 2; i++)
	{
		int lDelay = aRampTime/4;
		analogWrite(mLedPin, 0);
		delay(lDelay);
		analogWrite(mLedPin, 254);
		delay(lDelay);
	}

	Off();

	return true;
}

/**
 * Apply blade flicker effects.
 */
void CheapieBlade::ApplyFlicker(int aType)
{
	//Do nothing. No flicker effects for this simple blade.
}

void CheapieBlade::SetChannel(unsigned char aLevel, int aChannel = 0)
{
	this->mLedPowerLevel = aLevel;
}

void CheapieBlade::SetFlickerType(int aType)
{
	//Do nothing. No meaning for this simple blade.
}

void CheapieBlade::PerformIO()
{
	analogWrite(mLedPin, mLedPowerLevel);
}

void CheapieBlade::Off()
{
	mLedPowerLevel = 0;
	PerformIO();

}

void CheapieBlade::On()
{
	mLedPowerLevel = 255;
	PerformIO();
}

BladeMetadata  CheapieBlade::GetFeatures()
{
	BladeMetadata lFeatures;

	//Number of channels
	lFeatures.Channels = 1;
	//Number of flicker patterns
	lFeatures.Flickers = 0;

	return lFeatures;
}
