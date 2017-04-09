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
 * DFPlayerSoundPlayer.h
 *
 *  Created on: May 4, 2016
 *      Author: JakeSoft <http://forum.arduino.cc/index.php?topic=261980.0>
 */

#ifndef _DFPLAYERSOUNDPLAYER_H_
#define _DFPLAYERSOUNDPLAYER_H_

#include "ASoundPlayer.h"

class SoftwareSerial;

/**
 * Structure helps describe how the DFPlayer is programmed by specifying where
 * each type of sound begins. It is assumed that sounds of the same type
 * will be grouped with consecutive addresses.
 * NOTE: The addresses correspond to the relative physical address of the files
 * on the storage media and may not line up with the file names. Make sure the
 * files are copied to the media in the right order.
 */
struct DFPlayerSoundOffsets
{
	//Global parameters
	unsigned char BaseAddr; //Base address for all sounds (typically zero)

	//Sound font parameters
	unsigned char FontIdBase; //Base address for Font ID sounds
	unsigned char PowerupBase; //Baseaddress for powerup sounds
	unsigned char PowerdownBase; //Base address for power-down sounds
	unsigned char HumBase; //Base address for hum sound
	unsigned char ClashBase; //Base address for clash sounds
	unsigned char SwingBase; //Base address for swing sounds
	unsigned char LockupBase; //Base address for lockup sounds
	unsigned char BlasterBase; //Base address for blaster sounds
	unsigned char ForceBase; //Base address for force sounds
	unsigned char CustomBase; //Base address for custom sounds
	//Menu
	unsigned char MenuBase; //Base address for menu sounds
	unsigned char BootBase; //Base address for Boot sound
};

/**
 * This structure helps tell the player how the DFPlayer is programmed. It is
 * assumed that sounds belonging to the same font will be programmed at
 * consecutive addresses on the DFPlayer and that sounds of the same type
 * will be likewise grouped with consecutive addresses.
 *
 * EXAMPLE of DFPlayer sound grouping layout:
 * Locations.ClashBase         : 4
 * Features.ClashSoundsPerFont : 8 (indexes 4 through 11)
 * Locations.SwingBase         : 12 (4 + 8, inclusive)
 */
struct DFPlayerSoundMap
{
	DFPlayerSoundOffsets Locations;
	SoundFeatures Features;
};

/**
 * This class handles saber sound playback for a DFPlayer sound module. The
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
class DFPlayerSoundPlayer :public ASoundPlayer
{
public:
	/**
	 * Constructor.
	 * Args:
	 * 	aRxPin - Transmit serial pin
	 * 	aTxPin - Receive serial pin
	 * 	apSoundMap - Sound map describes where the sounds are on the SD card
	 * 	            or FlashPROM the DFPlayer uses for storage
	 * 	aBusyPin - (optional) Pin for busy state
	 * 	aBusyState = (optional) TRUE = High when busy, FALSE = LOW when busy
	 */
	DFPlayerSoundPlayer(int aRxPin, int aTxPin, DFPlayerSoundMap* apSoundMap, int aBusyPin = -1, bool aBusyState = true);

	virtual ~DFPlayerSoundPlayer();

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
	SoftwareSerial* mpSerial;

	//Serial Tx pin
	int mTxPin;
	//Serial Rx pin
	int mRxPin;
	// Busy pin
	int mBusyPin;
	//Busy state
	bool mBusyState;
	//Sound map
	DFPlayerSoundMap* mpSoundMap;
	//Current font
	unsigned char mFontIdx;
	//Total number of sounds per font
	unsigned char mSoundsPerFont;
	//Current volume setting
	unsigned char mVolume;


};




#endif /* _DFPLAYERSOUNDPLAYER_H_ */
