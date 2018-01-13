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
 * ASoundPlayer.h
 *
 *  Created on: Mar 13, 2016
 *      Author: JakeSoft <http://forum.arduino.cc/index.php?topic=261980.0>
 */

#ifndef ASOUNDPLAYER_H_
#define ASOUNDPLAYER_H_

#include <Arduino.h> //For uint16_t

enum ESoundTypes
{
	eeFontIdSnd,
	eeBootSnd,
	eePowerUpSnd,
	eeSwingSnd,
	eeClashSnd,
	eeBlasterSnd,
	eeLockupSnd,
	eeForceSnd,
	eePowerDownSnd,
	eeHumSnd,
	eeCustomSnd,
	eeMenuSoundSnd,
};

/**
 * Identifies quantity of each sound type within a font.
 */
struct SoundFeatures
{
	unsigned char FontIdsPerFont; //Number of font identifiers per font
	unsigned char HumSoundsPerFont; //Number of hum sounds per font
	unsigned char PowerUpSoundsPerFont; //Number of power up sounds per font
	unsigned char PowerDownSoundsPerFont; //Number of power down sounds per font
	unsigned char ClashSoundsPerFont; //Number of clash sounds per font
	unsigned char SwingSoundsPerFont; //Number of swing sounds per font
	unsigned char LockupSoundsPerFont; //Number of lockup sounds per font
	unsigned char BlasterSoundsPerFont; //Number of blaster sounds per font
	unsigned char ForceSoundsPerFont; //Number of force sounds per font
	unsigned char CustomSoundsPerFont;
	unsigned char MenuSounds; //Number of menu sounds

	unsigned char MaxVolume; //Maximum volume (minimum assumed to be zero)
};

/**
 * Structure helps describe where the first of each sound is on the sound
 * player's storage media (FLASH, SD card, etc.) within sound each font.
 */
struct SoundOffsets
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
 * This structure helps tell the player how the FLASH/SD/etc. is programmed.
 * It is assumed that sounds belonging to the same font will be programmed at
 * consecutive addresses on the FLASH/SD/etc. and that sounds of the same type
 * will be likewise grouped with consecutive addresses.
 *
 * EXAMPLE of sound grouping layout:
 * Locations.ClashBase         : 4
 * Features.ClashSoundsPerFont : 8 (indexes 4 through 11)
 * Locations.SwingBase         : 12 (4 + 8, inclusive)
 */
struct SoundMap
{
	SoundOffsets Locations;
	SoundFeatures Features;
};

class ASoundPlayer
{
public:

	/**
	 * Initialize. Do anything that should happen on startup.
	 */
	virtual void Init() = 0;

	/**
	 * Play a specific sound. Subclasses should implement a method that can
	 * play a specific sound on demand. For example, a call to play the second
	 * clash sound within a sound font would be:
	 *  PlaySound(eeClash, 1);
	 * Remember that indexes are assumed to be base-zero, meaning that zero is
	 * the first element.
	 *
	 * Args:
	 *  aSoundType - Type of sound to play
	 *  aIndex - Index of the sound to play.
	 * Returns:
	 *  TRUE if successful, FALSE otherwise
	 */
	virtual bool PlaySound(ESoundTypes aSoundType, uint16_t aIndex = 0) = 0;

	/**
	 * Play a random sound within the selected category. Subclasses should
	 * implement a method that can play a random sound within the selected
	 * category on demand.
	 *
	 * Args:
	 *  aSoundType - Type of sound to play
	 * Returns:
	 *  TRUE if successful, FALSE otherwise
	 */
	virtual bool PlayRandomSound(ESoundTypes aSoundType) = 0;

	/**
	 * Stop playback. Subclasses should implement a method that will stop
	 * all sound playback from happening, if such a feature is supported
	 * by the sound module. Otherwise, this method can be left as is and
	 * will do nothing.
	 */
	virtual void Stop()
	{
		//Do nothing
	}

	/**
	 * Set volume. Subclasses should implement a routine that can adjust the
	 * volume of the sound module if such a feature is supported. If the
	 * feature is not supported, then this method can be left as is and will
	 * do nothing.
	 * Args:
	 *  aVolume - Volume level to use
	 */
	virtual void SetVolume(int aVolume)
	{
		//Do nothing
	}

	/**
	 * Fetch busy state. Subclasses should implement a method that returns the
	 * busy status of the sound module if such a feature is supported. If the
	 * feature is not supported by the underlying hardware, then this method
	 * can be left as is and will always return false.
	 * Returns:
	 *  TRUE if busy, FALSE otherwise
	 */
	virtual bool IsBusy()
	{
		return false;
	}

	/**
	 * Change the current font. This method is for support of multi-font saber
	 * sound. Subclasses should implement code that will cause subsequent calls
	 * to PlaySound() and PlayRandomSound() to play sounds from alternate fonts
	 * specified by the aFontIndex argument of this method.
	 * Args:
	 *   aFontIndex - Index of the font to use.
	 */
	virtual void SetFont(unsigned char aFontIndex) = 0;

	/**
	 * Fetch features. A reference to a structure will be returned containing
	 * information about the sound player's capabilities. Subclasses should
	 * maintain a feature set structure to be returned by this method.
	 * Returns:
	 *  SoundFeatures structure with sound player abilities.
	 */
	virtual const SoundFeatures& Features() = 0;
};


#endif /* ASOUNDPLAYER_H_ */
