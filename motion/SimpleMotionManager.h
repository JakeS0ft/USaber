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
 * SimpleMotionManager.h
 *
 *  Created on: Mar 11, 2016
 *      Author: JakeSoft
 */

#ifndef SIMPLEMOTIONMANAGER_H_
#define SIMPLEMOTIONMANAGER_H_

#include "AMotionManager.h"

/**
 * This class implements motion sensing using a tilt switch to detect swing and
 * a "shake switch" for detecting clashes. This is the same method that cheap
 * toys use.
 */
class SimpleMotionManager : public AMotionManager
{
public:
	/**
	 * Constructor.
	 * Args:
	 *  aSwingPin - Pin for the swing sensor
	 *  aClashPin - Pin for the clash sensor (must be interrupt capable)
	 *  aClashInt - Interrupt to attach for clash sensing
	 */
	SimpleMotionManager(int aSwingPin, int aClashPin = 2, int aClashInt = 0);

	/**
	 * Destructor.
	 */
	virtual ~SimpleMotionManager();

	virtual void Init();

	virtual bool IsSwing(unsigned int TRESHOLD);

	virtual bool IsClash();

	virtual void Update();

protected:
	 //Pin for the swing sensor.
	int mSwingPin;

	//Pin for the clash sensor.
	int mClashPin;

	//Interrupt for clash sensor.
	int mClashInt;

	//Swing flag
	bool mIsSwing;

	// Clash flag
	bool mIsClash;

	// Time when last swing was detected
	unsigned long mLastSwingTime;

	//Time when last clash was detected
	unsigned long mLastClashTime;
};

#endif /* LIBRARIES_USABER_SIMPLEMOTIONMANAGER_H_ */
