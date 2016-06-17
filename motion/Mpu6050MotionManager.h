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


#include <MPU6050_6Axis_MotionApps20.h>

#ifndef MPU6050MOTIONMANAGER_H_
#define MPU6050MOTIONMANAGER_H_



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

	virtual void setInterrupt(bool interrupt);



protected :
	MPU6050 mpu;
	volatile  bool interrupt; // indicates whether MPU interrupt pin has gone high
	bool dmpReady = false;  // set true if DMP init was successful
	uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
	uint8_t devStatus; // return status after each device operation (0 = success, !0 = error)
	uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
	uint8_t fifoBuffer[64]; // FIFO storage buffer
	uint16_t mpuFifoCount;     // count of all bytes currently in FIFO


	// orientation/motion vars
	Quaternion quaternion;           // [w, x, y, z]         quaternion container
	static Quaternion quaternion_last;  // [w, x, y, z]         quaternion container
	static Quaternion quaternion_reading; // [w, x, y, z]         quaternion container

	VectorInt16 aaWorld; // [x, y, z]            world-frame accel sensor measurements
	static VectorInt16 aaWorld_last; // [x, y, z]            world-frame accel sensor measurements
	static VectorInt16 aaWorld_reading; // [x, y, z]            world-frame accel sensor measurements


private :

};

#endif /* LIBRARIES_USABER_MOTION_MPU6050MOTIONMANAGER_H_ */
