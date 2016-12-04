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
 * DIYinoSoundPlayer.cpp
 *
 *  Created on: Aug 28, 2016
 *      Author: JakeSoft
 */

#include <USaberConfig.h>

#ifdef BUILD_DIYINOSOUNDPLAYER
#include "sound/DIYinoSoundPlayer.h"
#include <Arduino.h>

DIYinoSoundPlayer::DIYinoSoundPlayer(uint8_t aTxPin, uint8_t aRxPin, DIYinoSoundMap* apSoundMap, int aBusyPin, bool aBusyState)
{
	mTxPin = aTxPin;
	mRxPin = aRxPin;
	mBusyState = aBusyState;
	mBusyPin = aBusyPin;

	mFontIdx = 0;

	mpSoundMap = apSoundMap;
	mDFPlayer.setSerial(aTxPin, aRxPin);

	//Calculate how many sounds are in each font
	mSoundsPerFont =
			apSoundMap->Features.FontIdsPerFont
			+ apSoundMap->Features.HumSoundsPerFont
			+ apSoundMap->Features.PowerUpSoundsPerFont
			+ apSoundMap->Features.PowerDownSoundsPerFont
			+ apSoundMap->Features.ClashSoundsPerFont
			+ apSoundMap->Features.SwingSoundsPerFont
			+ apSoundMap->Features.LockupSoundsPerFont
			+ apSoundMap->Features.BlasterSoundsPerFont
			+ apSoundMap->Features.ForceSoundsPerFont
			+ apSoundMap->Features.CustomSoundsPerFont;

	apSoundMap->Features.MaxVolume = 30;
	mVolume = 30;
}

DIYinoSoundPlayer::~DIYinoSoundPlayer()
{
	mDFPlayer.stop();
}


void DIYinoSoundPlayer::Init()
{
	//Set up the busy pin
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

void DIYinoSoundPlayer::PlayFile(int aFile)
{
	uint8_t lFileNumber = (uint8_t)aFile;

	mDFPlayer.playPhysicalTrack(lFileNumber);
}

void DIYinoSoundPlayer::PlayRepeat(int aFile)
{
	uint8_t lFileNumber = (uint8_t)aFile;

	mDFPlayer.playSingleLoop(lFileNumber);
}

void DIYinoSoundPlayer::SetVolume(int aVolume)
{
	uint8_t lVolNumber = (uint8_t)aVolume;

	mDFPlayer.setVolume(lVolNumber);

	mVolume = lVolNumber;
}

void DIYinoSoundPlayer::Stop()
{
	mDFPlayer.stop();
}

bool DIYinoSoundPlayer::PlaySound(ESoundTypes aSoundType, unsigned char aIndex)
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
	if(aSoundType != eeBootSnd &&
		aSoundType != eeMenuSoundSnd)
	{
		lSoundIndex += (mSoundsPerFont * mFontIdx);
	}


	if(lSuccess)
	{
		if(eeHumSnd == aSoundType)
		{
			//Send serial command to the DFPlayer so that hum sound loops
			PlayRepeat(lSoundIndex);
		}
		else
		{
			//Send serial command to the DFPlayer to play the sound
			PlayFile(lSoundIndex);
		}
	}

	return lSuccess;
}

bool DIYinoSoundPlayer::PlayRandomSound(ESoundTypes aSoundType)
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

bool DIYinoSoundPlayer::IsBusy()
{
	bool lBusy = false;

		//If busy monitoring is enabled, then read the pin and return the result
		if(mBusyPin > 0)
		{
			lBusy = (mBusyState == digitalRead(mBusyPin));
		}

		return lBusy;
}

const SoundFeatures& DIYinoSoundPlayer::Features()
{
	return mpSoundMap->Features;
}


void DIYinoSoundPlayer::SetFont(unsigned char aFontIdx)
{
	mFontIdx = aFontIdx;
}

#endif
