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
#include <Arduino.h>

class MPU6050;
class Quaternion;
class VectorInt16;

//Container to define motion tolerance data
struct MPU6050TolData
{
	//Tolerance for large swings
	unsigned int mSwingLarge;
	//Tolerance for medium swings
	unsigned int mSwingMedium;
	//Tolerance for small swings
	unsigned int mSwingSmall;
};

//Container to define calibration data
struct MPU6050CalibrationData
{
	//Accelerometer offsets
	uint16_t mAccelXOffset;
	uint16_t mAccelYOffset;
	uint16_t mAccelZOffset;
	//Gyro offsets
	uint16_t mGyXOffset;
	uint16_t mGyYOffset;
	uint16_t mGyZOffset;

};

class Mpu6050MotionManager : public AMotionManager
{
public:

	/**
	 * Constructor.
	 * Args: apTolData - Swing tolerance data
	 *       apCalData - Calibration data. Providing this optional data will improve accuracy.
	 */
	Mpu6050MotionManager(MPU6050TolData* apTolData, MPU6050CalibrationData* apCalData = NULL);

	/**
	 * Destructor.
	 * Args: NONE
	 */
	virtual ~Mpu6050MotionManager();

	virtual void Init();

	virtual bool IsSwing();

	virtual bool IsClash();

	virtual void Update();

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
	Quaternion *quaternion_reading;  // holds the actual readings
	Quaternion *quaternion_last;     // holds the last readings
	Quaternion *quaternion;           	   // last readings - actual readings


	MPU6050TolData* mpTolData; //Pointer to motion tolerance data

	MPU6050CalibrationData* mpCalData; //Pointer to calibration data

private :

};

#endif /* MPU6050MOTIONMANAGER_H_ */
