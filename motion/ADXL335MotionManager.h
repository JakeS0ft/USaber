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
 * ADXL335MotionManager.h
 *
 *  Created on: Oct 1, 2016
 *      Author: JakeSoft
 */

#ifndef _ADXL335MOTIONMANAGER_H_
#define _ADXL335MOTIONMANAGER_H_

#include "AMotionManager.h"
#include "../support/LinkedList.h"

//Container to define motion tolerance data
struct Adxl335TolData
{
	//Tolerance for large swings
	unsigned int mSwingLarge;
	//Tolerance for medium swings
	unsigned int mSwingMedium;
	//Tolerance for small swings
	unsigned int mSwingSmall;
	//Tolerance for clash detection
	unsigned int mClash;
};

//Container for time-stamped axis data
typedef struct
{
	//X-axis reading
	int mnX;
	//Y-axis reading
	int mnY;
	//Z-axis reading
	int mnZ;
	//When data was captured
	unsigned long mTimeStamp;
}AxisData;

class Adxl335MotionManager : public AMotionManager
{
public:
	/**
	 * Constructor.
	 */
	Adxl335MotionManager(int aXPin, int aYPin, int aZPin, Adxl335TolData* apTolData);

	/**
	 * Destructor.
	 */
	virtual ~Adxl335MotionManager();

	virtual void Init();

	virtual bool IsSwing();

	virtual bool IsClash();

	virtual void Update();

	virtual EMagnitudes GetSwingMagnitude();

protected:

	/**
	 * Scan the history buffer for swing events.
	 */
	bool SwingDetect();

	/**
	 * Scan the history buffer for clash events.
	 */
	bool ClashDetect();

	/**
	 * Calculates a composite vector based on positional data.
	 * Args:
	 *   arAxisDataIn - Raw axis data to process
	 */
	int CalculateVector(AxisData& arAxisDataIn);

	/**
	 * Calculates the delta between two positional data points as a composite vector.
	 * Time delta between the samples is taken into account to yield magnitude over time.
	 *
	 * Note: The aFirst argument is expected to come first chronologically (lower time stamp).
	 *
	 * Args:
	 *   aFirst - First position to compare
	 *   aSecond - Second position to compare
	 */
	int CalculateVectorDelta(AxisData& aFirst, AxisData& aSecond);

	//Input pins for X,Y,Z axis
	int mXPin;
	int mYPin;
	int mZPin;

	//Motion tolerance parameters
	Adxl335TolData* mpTolData;

	//Flags if last update cycle detected a swing
	bool mIsSwing;
	//Flags if last update cycle detected a clash
	bool mIsClash;

	//Time of last update cycle
	unsigned long mLastUpdateTime;

	//History buffer
	LinkedList<AxisData> mHistoryBuffer;

	//Magnitude of last swing
	EMagnitudes mSwingMagnitude;

	//Time when last clash was detected
	unsigned long mLastClashTime;
};


#endif /* _ADXL335MOTIONMANAGER_H_ */
