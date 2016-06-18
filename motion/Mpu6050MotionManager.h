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
 *  Created on: 27 mai 2016
 *      Author: Neskweek
 */



#ifndef MPU6050MOTIONMANAGER_H_
#define MPU6050MOTIONMANAGER_H_





class MPU6050;
class Quaternion;
class VectorInt16;


#include "AMotionManager.h"





class Mpu6050MotionManager : public AMotionManager
{
public:

	/**
	 * Constructor.
	 * Args:
	 */
	Mpu6050MotionManager();

	/**
	 * Destructor.
	 * Args:
	 */
	virtual ~Mpu6050MotionManager();

	virtual void Init();

	virtual bool IsSwing(unsigned int treshold);

	virtual bool IsClash();

	virtual void Update();



protected :


	//Interrupt for clash sensor.
	volatile bool mClashInt;

	bool dmpReady = false;  // set true if DMP init was successful

	unsigned short int mpuIntStatus;   // holds actual interrupt status byte from MPU

	unsigned short int devStatus; // return status after each device operation (0 = success, !0 = error)

	unsigned int packetSize;    // expected DMP packet size (default is 42 bytes)

	unsigned short int fifoBuffer[64]; // FIFO storage buffer

	unsigned int mpuFifoCount;     // count of all bytes currently in FIFO


	MPU6050 *mpu;

	// orientation/motion vars
	//  - Quaternion containers [w, x, y, z]
	Quaternion *quaternion_reading;  // holds the actual readings
	Quaternion *quaternion_last;     // holds the last readings
	Quaternion *quaternion;           	   // last readings - actual readings


	//  - World-frame accel sensor measurements [x, y, z] :
	//    Acceleration components with gravity removed and adjusted
	//    for the world frame of reference
	VectorInt16 *aaWorld_reading; // holds the actual readings
	VectorInt16 *aaWorld_last; 	// holds the last readings
	VectorInt16 *aaWorld; 				// last readings - actual readings




private :

};

#endif /* MPU6050MOTIONMANAGER_H_ */
