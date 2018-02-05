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
#include "sound/DIYinoSoundPlayer.h"

DFPlayerSoundPlayer::DFPlayerSoundPlayer(int aRxPin,
									     int aTxPin,
		                                 SoundMap* apSoundMap,
										 int aBusyPin,
										 bool aBusyState)
: DIYinoSoundPlayer(aRxPin, aTxPin, apSoundMap, aBusyPin, aBusyState)
{

}

DFPlayerSoundPlayer::~DFPlayerSoundPlayer()
{
	//Do nothing
}

#endif //BUILD_DFPLAYERSOUNDPLAYER
