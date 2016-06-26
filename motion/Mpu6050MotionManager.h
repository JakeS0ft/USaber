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
 * MPU6050MotionManager.h
 *
 *  Created on: 27 May 2016
 *      Authors: Neskweek, JakeSoft
 */

#ifndef MPU6050MOTIONMANAGER_H_
#define MPU6050MOTIONMANAGER_H_

#include "AMotionManager.h"

class MPU6050;
class Quaternion;
//class VectorInt16;

//Container for define motion tolerance data
struct MPU6050TolData
{
	//Tolerance for large swings
	float mSwingLarge;
	//Tolerance for medium swings
	float mSwingMedium;
	//Tolerance for small swings
	float mSwingSmall;
};

class Mpu6050MotionManager : public AMotionManager
{
public:

	/**
	 * Constructor.
	 * Args: apTolData - Swing tolerance data
	 */
	Mpu6050MotionManager(MPU6050TolData* apTolData);

	/**
	 * Destructor.
	 * Args: NONE
	 */
	virtual ~Mpu6050MotionManager();

	virtual void Init();

	virtual bool IsSwing();

	virtual bool IsClash();

	virtual void Update();

	virtual EMagnitudes GetSwingMagnitude() ;


protected :


	//Interrupt for clash sensor.
	volatile bool mClashInt;

	bool dmpReady;  // set true if DMP init was successful

	unsigned short int mpuIntStatus;   // holds actual interrupt status byte from MPU

	unsigned short int devStatus; // return status after each device operation (0 = success, !0 = error)

	unsigned int packetSize;    // expected DMP packet size (default is 42 bytes)

	unsigned char fifoBuffer[64]; // FIFO storage buffer

	unsigned int mpuFifoCount;     // count of all bytes currently in FIFO


	MPU6050 *mpu;

	// orientation/motion vars
	//  - Quaternion containers [w, x, y, z]
	Quaternion *curOrientation;  	// holds the actual readings
	Quaternion *prevOrientation;    // holds the last readings
	Quaternion *curRotation;          // last readings - actual readings
	Quaternion *prevRotation;          // last readings - actual readings

	MPU6050TolData* mpTolData; //Pointer to swing tolerance data

private :

};

#endif /* MPU6050MOTIONMANAGER_H_ */
