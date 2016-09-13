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
 * WT588DSoundPlayer.cpp
 *
 *  Created on: Mar 13, 2016
 *      Author: JakeSoft <http://forum.arduino.cc/index.php?topic=261980.0>
 */

#include "sound/WT588DSoundPlayer.h"
#include <Arduino.h>


WT588DSoundPlayer::WT588DSoundPlayer(int aDataPin, WT588DSoundMap* aSoundMap,
		int aBusyPin, bool aBusyState) :
		mDataPin(aDataPin),
		mBusyPin(aBusyPin),
		mBusyState(aBusyState),
		mpSoundMap(aSoundMap),
		mFontIdx(0)
{
	//Calculate how many sounds are in each font
	mSoundsPerFont =
			aSoundMap->Features.FontIdsPerFont
			+ aSoundMap->Features.HumSoundsPerFont
			+ aSoundMap->Features.PowerUpSoundsPerFont
			+ aSoundMap->Features.PowerDownSoundsPerFont
			+ aSoundMap->Features.ClashSoundsPerFont
			+ aSoundMap->Features.SwingSoundsPerFont
			+ aSoundMap->Features.LockupSoundsPerFont
			+ aSoundMap->Features.BlasterSoundsPerFont
			+ aSoundMap->Features.ForceSoundsPerFont
			+ aSoundMap->Features.CustomSoundsPerFont;

	aSoundMap->Features.MaxVolume = 8;
	delay(2000);
}


WT588DSoundPlayer::~WT588DSoundPlayer()
{
	//Do nothing
}

void WT588DSoundPlayer::Init()
{
	pinMode(mDataPin, OUTPUT);

	if(mBusyPin > 0)
	{
		pinMode(mBusyPin, INPUT);
		if(false == mBusyState)
		{
			//Set the pull-up resistor
			digitalWrite(mBusyPin, HIGH);
		}
	}
}

bool WT588DSoundPlayer::PlaySound(ESoundTypes aSoundType, unsigned char aIndex = 0)
{
	unsigned char lSoundIndex = mpSoundMap->Locations.BaseAddr;
	bool lSuccess = true;

	switch(aSoundType)
	{
	case eeFontIdSnd:
		lSoundIndex += mpSoundMap->Locations.FontIdBase;
		break;
	case eeBootSnd:
		lSoundIndex += mpSoundMap->Locations.BootBase;
		break;
	case eePowerUpSnd:
		lSoundIndex += mpSoundMap->Locations.PowerupBase;
		if(aIndex <= mpSoundMap->Features.PowerUpSoundsPerFont)
		{
			lSoundIndex += aIndex;
		}
		else
		{
			lSuccess = false;
		}
		break;
	case eeSwingSnd:
		lSoundIndex += mpSoundMap->Locations.SwingBase;
		if(aIndex <= mpSoundMap->Features.SwingSoundsPerFont)
		{
			lSoundIndex += aIndex;
		}
		else
		{
			lSuccess = false;
		}
		break;
	case eeClashSnd:
		lSoundIndex += mpSoundMap->Locations.ClashBase;
		if(aIndex <= mpSoundMap->Features.ClashSoundsPerFont)
		{
			lSoundIndex += aIndex;
		}
		else
		{
			lSuccess = false;
		}
		break;
	case eeBlasterSnd:
		lSoundIndex += mpSoundMap->Locations.BlasterBase;
		if(aIndex <= mpSoundMap->Features.BlasterSoundsPerFont)
		{
			lSoundIndex += aIndex;
		}
		else
		{
			lSuccess = false;
		}
		break;
	case eeLockupSnd:
		lSoundIndex += mpSoundMap->Locations.LockupBase;
		if(aIndex <= mpSoundMap->Features.LockupSoundsPerFont)
		{
			lSoundIndex += aIndex;
		}
		else
		{
			lSuccess = false;
		}
		break;
	case eeForceSnd:
		lSoundIndex += mpSoundMap->Locations.ForceBase;
		if(aIndex <= mpSoundMap->Features.ForceSoundsPerFont)
		{
			lSoundIndex += aIndex;
		}
		else
		{
			lSuccess = false;
		}
		break;
	case eePowerDownSnd:
		lSoundIndex += mpSoundMap->Locations.PowerdownBase;
		if(aIndex <= mpSoundMap->Features.PowerDownSoundsPerFont)
		{
			lSoundIndex += aIndex;
		}
		else
		{
			lSuccess = false;
		}
		break;
	case eeHumSnd:
		lSoundIndex += mpSoundMap->Locations.HumBase;
		break;
	case eeCustomSnd:
		lSoundIndex += mpSoundMap->Locations.CustomBase;
		if(aIndex <= mpSoundMap->Features.CustomSoundsPerFont)
		{
			lSoundIndex += aIndex;
		}
		else
		{
			lSuccess = false;
		}
		break;
	case eeMenuSoundSnd:
		lSoundIndex += mpSoundMap->Locations.MenuBase;
		lSoundIndex += aIndex;
		break;
	}

	//Compensate for base address offset
	lSoundIndex += mpSoundMap->Locations.BaseAddr;

	//Compensate for selected font (but not for menu and boot sounds)
	if(aSoundType != ESoundTypes::eeBootSnd &&
			aSoundType != ESoundTypes::eeMenuSoundSnd)
	{
		lSoundIndex += (mSoundsPerFont * mFontIdx);
	}

	if(lSuccess)
	{
		//Send serial command to the WT588D to play the sound
		SendCommand(lSoundIndex);
	}

	return lSuccess;
}

bool WT588DSoundPlayer::PlayRandomSound(ESoundTypes aSoundType)
{
	unsigned char lSoundIndex = 0;

	switch(aSoundType)
	{
	case eeFontIdSnd:
		lSoundIndex = random(0, mpSoundMap->Features.FontIdsPerFont);
		break;
	case eeBootSnd:
		//Do nothing
		break;
	case eePowerUpSnd:
		lSoundIndex = random(0, mpSoundMap->Features.PowerDownSoundsPerFont-1);
		break;
	case eeSwingSnd:
		lSoundIndex = random(0, mpSoundMap->Features.SwingSoundsPerFont-1);
		break;
	case eeClashSnd:
		lSoundIndex = random(0, mpSoundMap->Features.ClashSoundsPerFont-1);
		break;
	case eeBlasterSnd:
		lSoundIndex = random(0, mpSoundMap->Features.BlasterSoundsPerFont-1);
		break;
	case eeLockupSnd:
		lSoundIndex = random(0, mpSoundMap->Features.LockupSoundsPerFont-1);
		break;
	case eeForceSnd:
		lSoundIndex = random(0, mpSoundMap->Features.ForceSoundsPerFont-1);
		break;
	case eePowerDownSnd:
		lSoundIndex = random(0, mpSoundMap->Features.PowerDownSoundsPerFont-1);
		break;
	case eeHumSnd:
		lSoundIndex = random(0, mpSoundMap->Features.HumSoundsPerFont-1);
		break;
	case eeCustomSnd:
		lSoundIndex = random(0, mpSoundMap->Features.CustomSoundsPerFont-1);
		break;
	case eeMenuSoundSnd:
		lSoundIndex = random(0, mpSoundMap->Features.MenuSounds-1);
		break;
	default:
		//Do nothing
		break;
	}


	return PlaySound(aSoundType, lSoundIndex);

}

bool WT588DSoundPlayer::IsBusy()
{
	bool lBusy = false;

	//If busy monitoring is enabled, then read the pin and return the result
	if(mBusyPin > 0)
	{
		lBusy = (mBusyState == digitalRead(mBusyPin));
	}

	return lBusy;
}

void WT588DSoundPlayer::SendCommand(unsigned char aCommand)
{
	unsigned char lnCurrentBit = 0;

	digitalWrite(mDataPin, LOW);
	delay(5);

	for(int lnBitIdx = 0; lnBitIdx <= 7; lnBitIdx++)
	{
		//Isolate current bit
		lnCurrentBit = (aCommand >> lnBitIdx) & 0x1;
		digitalWrite(mDataPin, HIGH);

		//Set data pin according to current bit
		if(0 == lnCurrentBit)
		{
			delayMicroseconds(200);
			digitalWrite(mDataPin, LOW);
			delayMicroseconds(600);
		}else
		{
			delayMicroseconds(600);
			digitalWrite(mDataPin, LOW);
			delayMicroseconds(200);
		}
	}

	digitalWrite(mDataPin, HIGH);
	delay(15);
}

void WT588DSoundPlayer::SetFont(unsigned char aFontIdx)
{
	if(aFontIdx < mSoundsPerFont)
	{
		mFontIdx = aFontIdx;
	}
	else
	{
		mFontIdx = mSoundsPerFont - 1;
	}
}

const SoundFeatures& WT588DSoundPlayer::Features()
{
	return mpSoundMap->Features;
}

void WT588DSoundPlayer::Stop()
{
	SendCommand(WT588D_STOP_COMMAND);
}

void WT588DSoundPlayer::SetVolume(int aVolume)
{
	if(aVolume < WTD588D_VOL_MAX)
	{
		SendCommand(WTD588D_VOL_MIN + (unsigned char)aVolume);
	}
	else //Set to max volume
	{
		SendCommand(WTD588D_VOL_MIN + 7);
	}
}
