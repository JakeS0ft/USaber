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
 * CrossGuardBlade.h
 *
 *  Created on: Mar 11, 2016
 *      Author: JakeSoft <http://forum.arduino.cc/index.php?topic=261980.0>
 */

#ifndef CROSSGUARDBLADE_H_
#define CROSSGUARDBLADE_H_

#include "SingleLedBlade.h"

/**
 * This class implements a cross-guard style blade. Two channels are used.
 * The first channel is the main blade, the second controls the cross-
 * guard blades. The cross-guard blades can not be controlled independently.
 */
class CrossGuardBlade : public SingleLedBlade
{
public:
	/**
	 * Constructor.
	 * Args:
	 *  aMainPin - Pin for the main blade
	 *  aCrossPin - Pin for the cross-guard blades
	 *  aCorssDelay - Delay between main blade ignition and
	 *                cross-guard ignition during powerup
	 */
	CrossGuardBlade(int aMainPin, int aCrossPin, int aCrossDelay);

	virtual ~CrossGuardBlade();

	virtual void Init();
	virtual bool PowerUp(int aRampTime);
	virtual bool PowerDown(int aRampTime);
	virtual void ApplyFlicker(int aEffect);
	virtual void SetChannel(unsigned char aLevel, int aChannel);
	virtual void PerformIO();
	virtual void Off();
	virtual void On();
	virtual BladeMetadata GetFeatures();
protected:
	// Pin for the cross-guard blades
	int mCrossPin;
	// Delay after main blade blade ignition and cross-guard ignition
	unsigned int mCrossDelay;
	//Target power for the cross-guard LED (0 = off to 255 = max brightness)
	unsigned char mCrossPowerLevel;

};

#endif /* LIBRARIES_USABER_CROSSGUARDBLADE_H_ */
