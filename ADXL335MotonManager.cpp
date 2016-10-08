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
 * ADXL335MotonManager.cpp
 *
 *  Created on: Oct 2, 2016
 *      Author: JakeSoft
 */

#include "motion/ADXL335MotionManager.h"
#include <Arduino.h>

#define ADXL335_HISTORY_SIZE 5 //Must be at least 2
#define CLASH_DEBOUCE_TIME 100 //Minimum time (in milliseconds) between clash events

Adxl335MotionManager::Adxl335MotionManager(int aXPin, int aYPin, int aZPin, Adxl335TolData* apTolData)
{
	mXPin = aXPin;
	mYPin = aYPin;
	mZPin = aZPin;
	mpTolData = apTolData;

	mIsSwing = false;
	mIsClash = false;

	mLastUpdateTime = millis();

	mSwingMagnitude = eeSmall;

	mLastClashTime = millis();

}


Adxl335MotionManager::~Adxl335MotionManager()
{

}

void Adxl335MotionManager::Init()
{
	pinMode(mXPin, INPUT);
	pinMode(mYPin, INPUT);
	pinMode(mZPin, INPUT);

	//Pre-fill history buffer. Guarantees a certain size.
	while(this->mHistoryBuffer.size() < ADXL335_HISTORY_SIZE)
	{
		Update();
	}
}

bool Adxl335MotionManager::IsSwing()
{
	return mIsSwing;
}

bool Adxl335MotionManager::IsClash()
{
	bool lTemp = mIsClash;

	mIsClash = false;
	return lTemp;
}

void Adxl335MotionManager::Update()
{

	//Don't update more than once per millisecond
	if(millis() - mLastUpdateTime < 1)
	{
		return;
	}
	else
	{
		//Capture the current time and proceed with update
		mLastUpdateTime = millis();
	}

	//Capture current readings from ADXL335
	AxisData lCurrentReading;
	lCurrentReading.mTimeStamp = millis();
	lCurrentReading.mnX = analogRead(mXPin) * 10;
	lCurrentReading.mnY = analogRead(mYPin) * 10;
	lCurrentReading.mnZ = analogRead(mZPin) * 10;

	//Store current readings
	mHistoryBuffer.add(lCurrentReading);

	//Check and correct for history buffer size overflow
	if(mHistoryBuffer.size() > ADXL335_HISTORY_SIZE)
	{
		//Chop off the first entry
		mHistoryBuffer.shift();
	}

	//Check for clashes
	mIsClash = ClashDetect();

	//Only try to detect swings if a clash isn't happening
	if(false == mIsClash)
	{
		mIsSwing = SwingDetect();
	}
	else
	{
		mIsSwing = false;
	}

}

EMagnitudes Adxl335MotionManager::GetSwingMagnitude()
{
	return mSwingMagnitude;
}

bool Adxl335MotionManager::SwingDetect()
{
	bool lSwingDetected = false;

	int lSwingCount = 0; //Counts how many samples are beyond the swing threshold
	int lVectorDeltaMax = 0; //Maxium detected vector delta

	//Compare most recent reading to all previous readings to detect swings
	for(int lIdx = mHistoryBuffer.size() - 1;
		lIdx > 0;
		lIdx--)
	{
		AxisData lData1 = mHistoryBuffer.get(lIdx);
		AxisData lData2 = mHistoryBuffer.get(lIdx-1);

		int lVectorDelta = CalculateVectorDelta(lData1, lData2);

		//Detect if motion is beyond tolerance threashold.
		//Ignore swings for a short time after a clash
		if(abs(lVectorDelta) >= mpTolData->mSwingSmall && millis() - mLastClashTime > CLASH_DEBOUCE_TIME)
		{
			lSwingCount++;
			lVectorDeltaMax = max(lVectorDeltaMax, abs(lVectorDelta));
		}
	}

	if(lSwingCount >= mHistoryBuffer.size() - 1)
	{
		lSwingDetected = true;

		mSwingMagnitude = eeSmall;

			if(lVectorDeltaMax >= (int)mpTolData->mSwingMedium)
			{
				mSwingMagnitude = eeMedium;
			}
			if(lVectorDeltaMax >= (int)mpTolData->mSwingLarge)
			{
				mSwingMagnitude = eeLarge;
			}
	}

	return lSwingDetected;
}

bool Adxl335MotionManager::ClashDetect()
{
	bool lClashDetected = false;

	int lCurIdx = mHistoryBuffer.size() - 1;

	AxisData lCurrent = mHistoryBuffer.get(lCurIdx);
	AxisData lPrev = mHistoryBuffer.get(lCurIdx - 1);

	int lVectorDelta = CalculateVectorDelta(lPrev, lCurrent);

	if(abs(lVectorDelta) > mpTolData->mClash && millis() - mLastClashTime >= CLASH_DEBOUCE_TIME)
	{
		mLastClashTime = millis();
		lClashDetected = true;
	}

	return lClashDetected;
}

int Adxl335MotionManager::CalculateVector(AxisData& arAxisDataIn)
{
	int lVector = 0;

	float lXaxis = (float)arAxisDataIn.mnX;
	float lYaxis = (float)arAxisDataIn.mnY;
	float lZaxis = (float)arAxisDataIn.mnZ;

	float lVectorFloat = (lXaxis*lXaxis) + (lYaxis*lYaxis) + (lZaxis*lZaxis);
	lVectorFloat = sqrt(lVectorFloat);

	lVector = (int)lVectorFloat;

	return lVector;
}

int Adxl335MotionManager::CalculateVectorDelta(AxisData& arFirst, AxisData& arSecond)
{
	int lDelta = 0;

	int lFirstVector = CalculateVector(arFirst);
	int lSecondVector = CalculateVector(arSecond);

	lDelta = lFirstVector - lSecondVector;

	//Now factor in the time delta
	unsigned long lTimeDelta = arFirst.mTimeStamp - arSecond.mTimeStamp;

	lDelta = lDelta / (int) lTimeDelta;

	return lDelta;
}
