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
 * DFPlayerSoundPlayer.cpp
 *
 *  Created on: May 6, 2016
 *      Author: JakeSoft <http://forum.arduino.cc/index.php?topic=261980.0>
 */

#include <USaberConfig.h>

#ifdef BUILD_DFPLAYERSOUNDPLAYER
#include <Arduino.h>
#include "sound/DFPlayerSoundPlayer.h"
#include "SoftwareSerial.h"

DFPlayerSoundPlayer::DFPlayerSoundPlayer(int aRxPin,
									     int aTxPin,
		                                 DFPlayerSoundMap* apSoundMap,
										 int aBusyPin,
										 bool aBusyState)
{
	mTxPin = aTxPin;
	mRxPin = aRxPin;
	mBusyState = aBusyState;
	mBusyPin = aBusyPin;

	mFontIdx = 0;

	mpSoundMap = apSoundMap;

	mpSerial = new SoftwareSerial(aRxPin, aTxPin);

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

DFPlayerSoundPlayer::~DFPlayerSoundPlayer()
{
	mpSerial->end();

	delete mpSerial;
}

void DFPlayerSoundPlayer::Init()
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

	//Start serial comms
	mpSerial->begin(9600);
}

void DFPlayerSoundPlayer::PlayFile(int aFile)
{
	uint8_t lFileNumber = (uint8_t)aFile;

	// Play command serial sequence is: 7E FF 06 03 00 00 <file index> EF
	// Play command serial sequence is: 7E FF 06 22 00 <volume> <file index> EF
	//uint8_t lSendBuffer[8] = {0x7E, 0xFF, 0x06, 0x03, 0x00, 0x00, lFileNumber, 0xEF};
	uint8_t lSendBuffer[8] = {0x7E, 0xFF, 0x06, 0x22, 0x00, mVolume, lFileNumber, 0xEF};

	for(int lByte = 0; lByte < 8; lByte++)
	{
		mpSerial->write(lSendBuffer[lByte]);
	}

}

void DFPlayerSoundPlayer::PlayRepeat(int aFile)
{
	uint8_t lFileNumber = (uint8_t)aFile;

	// Play repeat command serial sequence is: 7E FF 06 08 00 00 <file index> EF
	uint8_t lSendBuffer[8] = {0x7E, 0xFF, 0x06, 0x08, 0x00, 0x00, lFileNumber, 0xEF};

	for(int lByte = 0; lByte < 8; lByte++)
	{
		mpSerial->write(lSendBuffer[lByte]);
	}
}

void DFPlayerSoundPlayer::SetVolume(int aVolume)
{
	uint8_t lVolNumber = (uint8_t)aVolume;

	// Set volume command serial sequence is 7E FF 06 06 00 00 <Volume> EF
	uint8_t lSendBuffer[8] = {0x7E, 0xFF, 0x06, 0x06, 0x00, 0x00, lVolNumber, 0xEF};

	for(int lByte = 0; lByte < 8; lByte++)
	{
		mpSerial->write(lSendBuffer[lByte]);
	}

	mVolume = lVolNumber;
}

void DFPlayerSoundPlayer::Stop()
{
	// Stop command serial sequence is: 7E FF 06 16 00 00 00 EF
	uint8_t lSendBuffer[8] = {0x7E, 0xFF, 0x06, 0x16, 0x00, 0x00, 0x00, 0xEF};

	for(int lByte = 0; lByte < 8; lByte++)
	{
		mpSerial->write(lSendBuffer[lByte]);
	}
}

bool DFPlayerSoundPlayer::PlaySound(ESoundTypes aSoundType, uint16_t aIndex)
{
	uint16_t lSoundIndex = mpSoundMap->Locations.BaseAddr;
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

bool DFPlayerSoundPlayer::PlayRandomSound(ESoundTypes aSoundType)
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

bool DFPlayerSoundPlayer::IsBusy()
{
	bool lBusy = false;

		//If busy monitoring is enabled, then read the pin and return the result
		if(mBusyPin > 0)
		{
			lBusy = (mBusyState == digitalRead(mBusyPin));
		}

		return lBusy;
}

const SoundFeatures& DFPlayerSoundPlayer::Features()
{
	return mpSoundMap->Features;
}


void DFPlayerSoundPlayer::SetFont(unsigned char aFontIdx)
{
	mFontIdx = aFontIdx;
}

#endif //BUILD_DFPLAYERSOUNDPLAYER
