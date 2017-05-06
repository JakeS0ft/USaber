/*
 * Mpu6050AdvancedMotionManager.cpp
 *
 *  Created on: May 2, 2017
 *      Author: Jake
 */

#include "USaberConfig.h"

#ifdef BUILD_MPU6050LITE

#include "motion/Mpu6050AdvancedMotionManager.h"

Mpu6050AdvancedMotionManager::Mpu6050AdvancedMotionManager(MPU6050AdvancedTolData* apTolData)
:Mpu6050LiteMotionManager(apTolData)
{

}

void Mpu6050AdvancedMotionManager::GetRawAcclData
	(uint16_t& arAclX, uint16_t& arAclY, uint16_t& arAclZ)
{
	arAclX = mCurAcclReading.mnX;
	arAclY = mCurAcclReading.mnY;
	arAclZ = mCurAcclReading.mnZ;
}


void Mpu6050AdvancedMotionManager::GetRawGyroData
	(uint16_t& arGyX, uint16_t& arGyY, uint16_t& arGyZ)
{
	arGyX = mCurGyroReading.mnX;
	arGyY = mCurGyroReading.mnY;
	arGyZ = mCurGyroReading.mnZ;
}

#endif
