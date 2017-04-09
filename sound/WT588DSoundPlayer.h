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
 * WT588DSoundPlayer.h
 *
 *  Created on: Mar 13, 2016
 *      Author: JakeSoft <http://forum.arduino.cc/index.php?topic=261980.0>
 */

#ifndef _WT588DSOUNDPLAYER_H_
#define _WT588DSOUNDPLAYER_H_

#include "ASoundPlayer.h"

const unsigned char WTD588D_VOL_MIN = 0xE0;
const unsigned char WTD588D_VOL_MAX = 0xE7;
const unsigned char WT588D_STOP_COMMAND = 0xFE;

/**
 * Structure helps describe how the WT588D is programmed by specifying where
 * each type of sound begins. It is assumed that sounds of the same type
 * will be grouped with consecutive addresses.
 */
struct WT588DSoundOffsets
{
	//Global parameters
	unsigned char BaseAddr; //Base address for all sounds (typically zero)

	//Sound font parameters
	unsigned char FontIdBase; //Base address for Font ID sounds
	unsigned char PowerupBase; //Base address for power-up sounds
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
 * This structure helps tell the player how the WT588D is programmed. It is
 * assumed that sounds belonging to the same font will be programmed at
 * consecutive addresses on the WT588D and that sounds of the same type
 * will be likewise grouped with consecutive addresses.
 *
 * EXAMPLE of WT588D sound grouping layout:
 * Locations.ClashBase         : 4
 * Features.ClashSoundsPerFont : 8 (indexes 4 through 11)
 * Locations.SwingBase         : 12 (4 + 8, inclusive)
 */
struct WT588DSoundMap
{
	WT588DSoundOffsets Locations;
	SoundFeatures Features;
};

class WT588DSoundPlayer : public ASoundPlayer
{
public:

	/**
	 * Constructor.
	 * Args:
	 *  aDataPin - Data pin for one-line serial comms with the WT588D
	 *  aSoundMap - Sound map describes where the sounds are on the WT588D
	 *  aBusyPin - (optional) Pin for busy state
	 *  aBusyState - (optional) TRUE = High when busy, FALSE = LOW when busy
	 */
	WT588DSoundPlayer(int aDataPin, WT588DSoundMap* aSoundMap, int aBusyPin = -1, bool aBusyState = true);

	/**
	 * Destructor.
	 */
	virtual ~WT588DSoundPlayer();

	virtual void Init();

	virtual bool PlaySound(ESoundTypes aSoundType, uint16_t aIndex);

	virtual bool PlayRandomSound(ESoundTypes aSoundType);

	virtual bool IsBusy();

	virtual const SoundFeatures& Features();

	void Stop();

	virtual void SetVolume(int aVolume);

	/**
	 * Sends a command to the WT588D.
	 * Args:
	 *  aCommand - Command to send
	 */
	void SendCommand(unsigned char aCommand);

protected:

	virtual void SetFont(unsigned char aFontIdx);

	// Data pin for serial comms
	int mDataPin;
	// Busy pin
	int mBusyPin;
	//Busy state
	bool mBusyState;
	//Sound map
	WT588DSoundMap* mpSoundMap;
	//Current font
	unsigned char mFontIdx;
	//Total number of sounds per font
	unsigned char mSoundsPerFont;
};

#endif /* _WT588DSOUNDPLAYER_H_ */
