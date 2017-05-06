/*
 * MPU6050AdvancedMotionManager.h
 *
 *  Created on: May 2, 2017
 *      Author: JakeSoft
 */

#ifndef MPU6050ADVANCEDMOTIONMANAGER_H_
#define MPU6050ADVANCEDMOTIONMANAGER_H_

#include "Mpu6050LiteMotionManager.h"

//Container to define motion tolerance data
struct MPU6050AdvancedTolData : public MPU6050LiteTolData
{
	//Exact copy of the parent class
	//Only put here to avoid confusion since all the other
	//Accelerometer-enabled motion manager classes have
	//their own Tolerance configuration structures.
};

/**
 * An extended version of the Light weight MPU6050 Motion manager.
 * This class has extra methods to allow users to fetch raw
 * accelerometer and gyro readings. This allows for more advanced processing
 * to be done by application code that may need to know more detail about
 * motion beyond simply knowing if a swing, clash, or twist has happened.
 *
 * Note: This class assumes that the Y-axis of the MPU6050 is parallel with
 * the saber's blade.
 *
 * Note: The raw readings are truncated to the 10 most significant bits.
 * This was done to avoid jitter. So, be mindful that the lower 6 bits are
 * chopped off from the values reported by the MPU6050 chip's register values.
 */
class Mpu6050AdvancedMotionManager : public Mpu6050LiteMotionManager
{
public:

	/**
	 * Constructor.
	 * Args:
	 *   apTolData - Pointer to tolerance data structure
	 */
	Mpu6050AdvancedMotionManager(MPU6050AdvancedTolData* apTolData);

	/**
	 * Fetch raw accelerometer readings
	 * Args:
	 *   arAclX - Reference to populate with X acceleration
	 *   arAclY - Reference to populate with Y acceleration
	 *   arAclZ - Reference to populate with Z acceleration
	 */
	virtual void GetRawAcclData(uint16_t& arAclX, uint16_t& arAclY, uint16_t& arAclZ);

	/**
	 * Fetch raw gyro readings
	 * Args:
	 *   arAclX - Reference to populate with X gyro reading
	 *   arAclY - Reference to populate with Y gyro reading
	 *   arAclZ - Reference to populate with Z gyro reading
	 */
	virtual void GetRawGyroData(uint16_t& arGyX, uint16_t& arGyY, uint16_t& arGyZ);
};


#endif /* MPU6050ADVANCEDMOTIONMANAGER_H_ */
