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
 * SimpleMotionManager.cpp
 *
 *  Created on: Mar 11, 2016
 *      Author: JakeSoft
 */

#include "motion/SimpleMotionManager.h"
#include <Arduino.h>

#define SIMPLEMOTION_SWING_DEBOUNCE 200 //Minimum time between swing events
#define SIMPLEMOTION_CLASH_DEBOUNCE 150 //Minimum time between clash events

//Flags when a clash interrupt is detected
volatile bool gClashIntr;

/**
 * Function: ClashInterupt()
 * Description: Handles the interrupt that is triggered when the
 *              clash sensor is hit.
 * Args:    NONE
 * Returns: NONE
 */
void ClashInterupt()
{
	gClashIntr = true;
}

SimpleMotionManager::SimpleMotionManager(int aSwingPin, int aClashPin, int aClashInt) :
mSwingPin(aSwingPin),
mClashPin(aClashPin),
mClashInt(aClashInt),
mIsSwing(false),
mIsClash(false)
{
	mLastSwingTime = millis();
	mLastClashTime = millis();
}

/**
 * Destructor.
 */
SimpleMotionManager::~SimpleMotionManager()
{
	//Do nothing
}

void SimpleMotionManager::Init()
{
	//Set pin modes
	pinMode(mSwingPin, INPUT);
	pinMode(mClashPin, INPUT);

	//Set pull-up resistors
	digitalWrite(mSwingPin, HIGH);
	digitalWrite(mClashPin, HIGH);

	attachInterrupt(mClashInt, ClashInterupt, FALLING);

}

bool SimpleMotionManager::IsSwing()
{
	return mIsSwing;
}

bool SimpleMotionManager::IsClash()
{
	return mIsClash;
}

void SimpleMotionManager::Update()
{
	int lCurrentSwingRead;
	static int lLastSwingRead;

	//Detect swings
	lCurrentSwingRead = digitalRead(mSwingPin);
	if(lCurrentSwingRead != lLastSwingRead)
	{
		//Calculate time since the last swing event
		unsigned long lSwingTimeDelta = millis() - mLastSwingTime;

		if(lSwingTimeDelta >= SIMPLEMOTION_SWING_DEBOUNCE)
		{
			mLastSwingTime = millis();
			mIsSwing = true;
		}
	}
	else
	{
		mIsSwing = false;
	}
	lLastSwingRead = lCurrentSwingRead;

	//Detect clashes
	if(gClashIntr)
	{

		//Calculate time since last clash event
		unsigned long lClashTimeDelta = millis() - mLastClashTime;


		if(lClashTimeDelta >= SIMPLEMOTION_CLASH_DEBOUNCE)
		{
			mLastClashTime = millis();
			mIsClash = true;
		}
		else
		{
			mIsClash = false;
		}

		gClashIntr = false;
	}
	else
	{
		mIsClash = false;
	}

}

