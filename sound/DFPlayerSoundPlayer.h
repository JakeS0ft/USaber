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

#include "SoftwareSerial.h"
#include "DIYinoSoundPlayer.h"

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
struct DFPlayerSoundMap : public SoundMap
{
	//Use base features
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
class DFPlayerSoundPlayer :public DIYinoSoundPlayer
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
	DFPlayerSoundPlayer(int aRxPin, int aTxPin, SoundMap* apSoundMap, int aBusyPin = -1, bool aBusyState = true);

	/**
	 * Destructor.
	 */
	~DFPlayerSoundPlayer();
};




#endif /* _DFPLAYERSOUNDPLAYER_H_ */
