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
 * BY8301SoundPlayer.h
 *
 *  Created on: Jan 9, 2018
 *      Author: JakeSoft <http://forum.arduino.cc/index.php?topic=261980.0>
 */

#ifndef _BY8301SoundPlayer_H_
#define _BY8301SoundPlayer_H_

#include "../SoftwareSerialTx.h"
#include "ASoundPlayer.h"

struct BY8301SoundMap : public SoundMap
{
	//Just use all of the standard data from the base class
	//Any features specific to the sound chip would go here
};

/**
 * This class handles saber sound playback for a BY8301 sound module. The
 * class is agnostic about if and SD card or built-in flash is used as the
 * storage media. If any additional setup is required by your particular module
 * to ensure the play-back chip is looking at the right media, that is to be
 * handled by external code outside of this class. This class assumes the
 * device is already configured to run with the correct media and that sound
 * files have been loaded properly.
 * NOTE: When commanded to play a hum sound (eeHumSnd), the class methods will
 * automatically loop the hum. So, for continuous hum, PlaySound() need only be
 * called once.
 */
class BY8301SoundPlayer :public ASoundPlayer
{
public:
	/**
	 * Constructor.
	 * Args:
	 * 	aTxPin - Receive serial pin
	 * 	apSoundMap - Sound map describes where the sounds are on the SD card
	 * 	            or FlashPROM the DFPlayer uses for storage
	 * 	aBusyPin - (optional) Pin for busy state
	 * 	aBusyState = (optional) TRUE = High when busy, FALSE = LOW when busy
	 */
	BY8301SoundPlayer(int aTxPin, SoundMap* apSoundMap, int aBusyPin = -1, bool aBusyState = true);

	virtual ~BY8301SoundPlayer();

	virtual void Init();

	virtual bool PlaySound(ESoundTypes aSoundType, uint16_t aIndex);

	virtual bool PlayRandomSound(ESoundTypes aSoundType);

	virtual bool IsBusy();

	virtual const SoundFeatures& Features();

	void Stop();

	virtual void SetVolume(int aVolume);

	virtual void SetFont(unsigned char aFontIdx);
protected:

	/**
	 * Plays a single file. The argument provided is the physcial address of
	 * the file on the storage media and is base 1. So, to play the first file
	 * stored, the argument would be 1. To play the second, 2, and so on.
	 *
	 * Args:
	 *   aFile - Physical address of the file on the storage media.
	 */
	virtual void PlayFile(int aFile);

	/**
	 * Plays a file in repeat mode.
	 *
	 * Args:
	 *   aFile - Physical address of the file on the storage media.
	 */
	virtual void PlayRepeat(int aFile);

	//Serial comms with the DFPlayer module
	SoftwareSerialTx* mpSerial;

	//Serial Tx pin
	int mTxPin;
	// Busy pin
	int mBusyPin;
	//Busy state
	bool mBusyState;
	//Sound map
	SoundMap* mpSoundMap;
	//Current font
	unsigned char mFontIdx;
	//Total number of sounds per font
	unsigned char mSoundsPerFont;
	//Current volume setting
	unsigned char mVolume;
};




#endif /* _BY8301SoundPlayer_H_ */
