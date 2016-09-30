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
 * RGBBlade.h
 *
 *  Created on: Jul 9, 2016
 *      Author: JakeSoft <http://forum.arduino.cc/index.php?topic=261980.0>
 */

#ifndef _RGBBLADE_H_
#define _RGBBLADE_H_


#include "IBladeManager.h"

class RGBBlade : public IBladeManager
{
public:
	/**
	 * Constructor:
	 * 	Args:
	 * 	 aCh1Pin - I/O pin for channel 1 (Red).
	 * 	 aCh2Pin - I/O pin for channel 2 (Blue).
	 * 	 aCh3Pin - I/O pin for channel 3 (Green).
	 */
	RGBBlade(int aCh1Pin, int aCh2Pin, int aCh3Pin);

	/**
	 * Destructor.
	 */
	virtual ~RGBBlade();

	virtual void Init();
	virtual bool PowerUp(int aRampTime);
	virtual bool PowerDown(int aRampTime);
	virtual void ApplyFlicker(int aType);
	virtual void SetChannel(unsigned char aLevel, int aChannel);
	virtual void PerformIO();
	virtual void Off();
	virtual void On();
	virtual BladeMetadata GetFeatures();

protected:
	/**
	 * Applies random flicker while keeping color balance.
	 *  Args:
	 *   aLowerBound - Lowest number as a percentage of set point (100 = full on, 0 = full off)
	 *   aUpperBound - Highest number as a percentage of set point (100 = full on, 0 = full off)
	 *   aUpdatePeroid - How often (in milliseconds) to apply a new random power level
	 */
	void RandomFlicker(int aLowerBound, int aUpperBound, unsigned int aUpdatePeriod);

	//I/O Pins that control the LEDs
	int mLedPins[3];
	//Target power levels for LEDs
	unsigned char mPowerLevels[3];
	//Flag keeps track of blade on/off status
	bool mIsOn;

	//Blade features
	BladeMetadata mFeatures;

	unsigned long mLastFlickerUpdate;
};


#endif /* _RGBBLADE_H_ */
