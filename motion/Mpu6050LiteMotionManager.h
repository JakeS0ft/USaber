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
 * Mpu6050LiteMotionManager.h
 *
 *  Created on: 14 Feb 2017
 *      Author: JakeSoft
 */

#ifndef MPU6050LITEMOTIONMANAGER_H_
#define MPU6050LITEMOTIONMANAGER_H_

#include <motion/AMotionManager.h>
#include <Arduino.h>

#define GYRO_FS_RANGE250  (0b00000000) //250 deg/sec
#define GYRO_FS_RANGE500  (0b00001000) //500 deg/sec
#define GYRO_FS_RANGE1000 (0b00010000) //1000 deg/sec
#define GYRO_FS_RANGE2000 (0b00011000) //2000 deg/sec
#define MPU6050_RA_INT_ENABLE 0x38

//Container to define motion tolerance data
struct MPU6050LiteTolData
{
	//Tolerance for large swings
	unsigned int mSwingLarge;
	//Tolerance for medium swings
	unsigned int mSwingMedium;
	//Tolerance for small swings
	unsigned int mSwingSmall;
	//Tolerance for clashes
	unsigned int mClash;
	//Tolerance for twist
	unsigned int mTwist;
};

/**
 * Light weight version of the MPU6050 Motion manager. Primary design
 * consideration for this class is to keep compiled size down while
 * still offering serviceable performance.
 *
 * Note: This class assumes that the Y-axis of the MPU6050 is parallel with
 * the saber's blade.
 */
class Mpu6050LiteMotionManager : public AMotionManager
{
public:

	/**
	 * Constructor.
	 * Args: apTolData - Swing tolerance data
	 */
	Mpu6050LiteMotionManager(MPU6050LiteTolData* apTolData);

	virtual ~Mpu6050LiteMotionManager();

	virtual void Init();

	virtual bool IsSwing();

	virtual bool IsClash();

	virtual bool IsTwist();

	virtual void Update();

	virtual EMagnitudes GetSwingMagnitude();

protected :
	//Container for time-stamped axis data
	typedef struct
	{
		//X-axis reading
		int16_t mnX;
		//Y-axis reading
		int16_t mnY;
		//Z-axis reading
		int16_t mnZ;
		//When data was captured
		unsigned long mTimeStamp;
	}AxisData;

	/**
	 * Check for swing event.
	 */
	bool SwingDetect();

	/**
	 * Check for clash event.
	 */
	bool ClashDetect();

	/**
	 * Sends I2C command to the MPU6050.
	 * Args: aAddr - Register address to write to
	 *       aByte - Value to write
	 */
	void I2CWrite(uint8_t aAddr, uint8_t aByte);

	// Magnitude of last detected swing
	EMagnitudes mSwingMagnitude;

	// Tolerance data
	MPU6050LiteTolData* mpTolData;

	// Previous accelerometer sensor readings
	AxisData mLastAcclReading;
	// Most recent accelerometer sensor reading
	AxisData mCurAcclReading;
	// Most recent gyro sensor reading
	AxisData mCurGyroReading;

	// Last time we checked for swing events
	unsigned long mLastSwingDetectTime;

	//Flags if swing was detected in last update cycle
	bool mIsSwing;

	//Flags if clash was detected in last update cycle
	bool mIsClash;

	//Flags if a twist gesture was detected last update cycle
	bool mIsTwist;

	// I2C address of the MPU-6050
	const int mMpuAddr=0x68;

	//Flag to keep track of if we started wire for I2C comms already or not
	bool mWireStarted;
};

#endif /* MPU6050LITEMOTIONMANAGER_H_ */
