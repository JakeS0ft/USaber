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
 * DIYinoSoundPlayer.h
 *
 *  Created on: Aug 28, 2016
 *      Author: JakeSoft
 */

#ifndef _DIYINOSOUNDPLAYER_H_
#define _DIYINOSOUNDPLAYER_H_

#include "ASoundPlayer.h"
#include "DFPlayer.h"

struct DIYinoSoundOffsets
{
	//Global parameters
	unsigned char BaseAddr; //Base address for all sounds (typically zero)

	//Sound font parameters
	unsigned char FontIdBase; //Base address for Font ID sounds
	unsigned char PowerupBase; //Base address for powerup sounds
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
struct DIYinoSoundMap
{
	DIYinoSoundOffsets Locations;
	SoundFeatures Features;
};


class DIYinoSoundPlayer :public ASoundPlayer
{
public:
	/**
	 * Constructor.
	 * Args:
	 * 	aTxPin - Transmit serial pin
	 * 	aRxPin - Receive serial pin
	 * 	apSoundMap - Sound map describes where the sounds are on the SD card
	 * 	aBusyPin - (optional) Pin for busy state
	 * 	aBusyState = (optional) TRUE = High when busy, FALSE = LOW when busy
	 */
	DIYinoSoundPlayer(uint8_t aTxPin, uint8_t aRxPin, DIYinoSoundMap* apSoundMap, int aBusyPin = -1, bool aBusyState = true);

	virtual ~DIYinoSoundPlayer();

	virtual void Init();

	virtual bool PlaySound(ESoundTypes aSoundType, unsigned char aIndex);

	virtual bool PlayRandomSound(ESoundTypes aSoundType);

	virtual bool IsBusy();

	virtual const SoundFeatures& Features();

	void Stop();

	virtual void SetVolume(int aVolume);

	virtual void SetFont(unsigned char aFontIdx);

	/**
	 * Plays a single file. The argument provided is the physcial address of
	 * the file on the storage media and is base 1. So, to play the first file
	 * stored, the argument would be 1. To play the second, 2, and so on.
	 *
	 * Args:
	 *   aFile - Physical address of the file on the storage media.
	 */
	virtual void PlayFile(int aFile);

protected:

	/**
	 * Plays a file in repeat mode.
	 *
	 * Args:
	 *   aFile - Physical address of the file on the storage media.
	 */
	virtual void PlayRepeat(int aFile);

	//DF Player does most of the work communicating with the sound chip
	DFPlayer mDFPlayer;

	//Serial Tx pin
	int mTxPin;
	//Serial Rx pin
	int mRxPin;
	// Busy pin
	int mBusyPin;
	//Busy state
	bool mBusyState;
	//Sound map
	DIYinoSoundMap* mpSoundMap;
	//Current font
	unsigned char mFontIdx;
	//Total number of sounds per font
	unsigned char mSoundsPerFont;
	//Current volume setting
	unsigned char mVolume;


};
#endif /* _DIYINOSOUNDPLAYER_H_ */
