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

#include "motion/Mpu6050MotionManager.h"
#include <Arduino.h>
//#include "support/MPU6050/MPU6050_6Axis_MotionApps20.h"
#include "support/MPU6050/MPU6050_9Axis_MotionApps41.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include <Wire.h>
#endif

/*
 * Those offsets are specific to each MPU6050 device.
 * they are found via calibration process.
 * See this script http://www.i2cdevlib.com/forums/index.php?app=core&module=attach&section=attach&attach_id=27
 */
//#define XACCELOFFSET	69
//#define YACCELOFFSET	-5022
//#define ZACCELOFFSET	4747
//#define XGYROOFFSET		25
//#define YGYROOFFSET		-7
//#define ZGYROOFFSET		53
#define XACCELOFFSET	-51
#define YACCELOFFSET	809
#define ZACCELOFFSET	1147
#define XGYROOFFSET		-11
#define YGYROOFFSET		24
#define ZGYROOFFSET		-39

//#define DEBUG_MPU

//Flags when a clash interrupt is detected
volatile bool tClashIntr;

/**
 * Function: ClashInterupt()
 * Description: Handles the interrupt that is triggered when the
 *              clash sensor is hit.
 * Args:    NONE
 * Returns: NONE
 */
void clashInterupt() {
	tClashIntr = true;
}

/**
 * Constructor.
 * Args:
 */
Mpu6050MotionManager::Mpu6050MotionManager(MPU6050TolData* apTolData) :
		dmpReady(false), mpTolData(apTolData) {
	mpu = new MPU6050();
	curRotation = new Quaternion();
	prevRotation = new Quaternion();
	curOrientation = new Quaternion();
	prevOrientation = new Quaternion();
}

/**
 * Destructor.
 * Args:
 */
Mpu6050MotionManager::~Mpu6050MotionManager() {
}

void Mpu6050MotionManager::Init() {

	/***** MP6050 MOTION DETECTOR INITIALISATION  *****/
	// join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
	Wire.begin();
	TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz). Comment this line if having compilation difficulties with TWBR.
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
			Fastwire::setup(400, true);
#endif

	// initialize device
#ifdef DEBUG_MPU
	Serial.println(F("Initializing I2C devices..."));
#endif

	mpu->initialize();
	//clear any data residu
	mpu->reset();

	// verify connection
#ifdef DEBUG_MPU
	Serial.println(F("Testing device connections..."));
	Serial.println(
			mpu->testConnection() ?
			F("MPU6050 connection successful") :
			F("MPU6050 connection failed"));
	Serial.println(mpu->getDeviceID());

	// load and configure the DMP
	Serial.println(F("Initializing DMP..."));
#endif
	devStatus = mpu->dmpInitialize();
	/*
	 * Those offsets are specific to each MPU6050 device.
	 * they are found via calibration process.
	 * See this script http://www.i2cdevlib.com/forums/index.php?app=core&module=attach&section=attach&attach_id=27
	 */
	mpu->setXAccelOffset(XACCELOFFSET);
	mpu->setYAccelOffset(YACCELOFFSET);
	mpu->setZAccelOffset(ZACCELOFFSET);
	mpu->setXGyroOffset(XGYROOFFSET);
	mpu->setYGyroOffset(YGYROOFFSET);
	mpu->setZGyroOffset(ZGYROOFFSET);

	// make sure it worked (returns 0 if so)
	if (devStatus == 0) {
		// turn on the DMP, now that it's ready
#ifdef DEBUG_MPU
		Serial.println(F("Enabling DMP..."));
#endif
		mpu->setDMPEnabled(true);

		// enable Arduino interrupt detection
#ifdef DEBUG_MPU
		Serial.println(
				F(
						"Enabling interrupt detection (Arduino external interrupt 0)..."));
#endif
		attachInterrupt(mClashInt, clashInterupt, RISING);

		// set our DMP Ready flag so the main loop() function knows it's okay to use it
#ifdef DEBUG_MPU
		Serial.println(F("DMP ready! Waiting for first interrupt..."));
#endif
		dmpReady = true;

		// get expected DMP packet size for later comparison
		packetSize = mpu->dmpGetFIFOPacketSize();
	} else {
		// ERROR!
		// 1 = initial memory load failed
		// 2 = DMP configuration updates failed
		// (if it's going to break, usually the code will be 1)
#ifdef DEBUG_MPU
		Serial.print(F("DMP Initialization failed (code "));
		Serial.print(devStatus);
		Serial.println(F(")"));
#endif
	}

	// configure the motion interrupt for clash recognition
	// INT_PIN_CFG register
	// in the working code of MPU6050_DMP all bits of the INT_PIN_CFG are false (0)

//	mpu->setInterruptMode(false); // INT_PIN_CFG register INT_LEVEL (0-active high, 1-active low)
//	mpu->setInterruptDrive(false); // INT_PIN_CFG register INT_OPEN (0-push/pull, 1-open drain)
//	mpu->setInterruptLatch(false); // INT_PIN_CFG register LATCH_INT_EN (0 - emits 50us pulse upon trigger, 1-pin is held until int is cleared)
//	mpu->setInterruptLatchClear(false); // INT_PIN_CFG register INT_RD_CLEAR (0-clear int only on reading int status reg, 1-any read clears int)
//	mpu->setFSyncInterruptLevel(false);
//	mpu->setFSyncInterruptEnabled(false);
//	mpu->setI2CBypassEnabled(false);
//	mpu->setI2CMasterModeEnabled(0);
//	mpu->setI2CBypassEnabled(1);
//	mpu->setTempSensorEnabled(true);
	// Enable/disable interrupt sources - enable only motion interrupt
//	mpu->setIntFreefallEnabled(false);
//	mpu->setIntFIFOBufferOverflowEnabled(false);
//	mpu->setIntI2CMasterEnabled(false);
//	mpu->setIntDataReadyEnabled(false);
//	mpu->setFullScaleGyroRange(0); //0: 250deg/s | 1: 500deg/s | 2: 1000deg/s | 3: 2000deg/s
//	mpu->setFullScaleAccelRange(0); //0: 2g | 1: 4g | 2: 8g | 3: 16g
//	mpu->setIntZeroMotionEnabled(false);
//	mpu->setZeroMotionDetectionThreshold(18); // 1mg/LSB
//	mpu->setZeroMotionDetectionDuration(1); // number of consecutive samples above threshold to trigger int
//	mpu->setDLPFMode(3);
//	mpu->setDHPFMode(0);
	mpu->setIntMotionEnabled(true); // INT_ENABLE register enable interrupt source  motion detection
	mpu->setMotionDetectionThreshold(10); // 1mg/LSB
	mpu->setMotionDetectionDuration(2); // number of consecutive samples above threshold to trigger int

#ifdef DEBUG_MPU
	Serial.println("MPU6050 register setup:");
	Serial.print("INT_PIN_CFG\t");
	Serial.print(mpu->getInterruptMode());
	Serial.print("\t");
	Serial.print(mpu->getInterruptDrive());
	Serial.print("\t");
	Serial.print(mpu->getInterruptLatch());
	Serial.print("\t");
	Serial.print(mpu->getInterruptLatchClear());
	Serial.print("\t");
	Serial.print(mpu->getFSyncInterruptLevel());
	Serial.print("\t");
	Serial.print(mpu->getFSyncInterruptEnabled());
	Serial.print("\t");
	Serial.println(mpu->getI2CBypassEnabled());
	// list INT_ENABLE register contents
	Serial.print("INT_ENABLE\t");
	Serial.print(mpu->getIntFreefallEnabled());
	Serial.print("\t");
	Serial.print(mpu->getIntMotionEnabled());
	Serial.print("\t");
	Serial.print(mpu->getIntZeroMotionEnabled());
	Serial.print("\t");
	Serial.print(mpu->getIntFIFOBufferOverflowEnabled());
	Serial.print("\t");
	Serial.print(mpu->getIntI2CMasterEnabled());
	Serial.print("\t");
	Serial.println(mpu->getIntDataReadyEnabled());
#endif
	/***** MP6050 MOTION DETECTOR INITIALISATION  *****/
}

bool Mpu6050MotionManager::IsSwing() {
#ifdef DEBUG_MPU
	if (rotation->w <=0.999) {
		Serial.print(F("curOrientation\t\tw="));
		Serial.print(curOrientation->getNormalized().w);
		Serial.print(F("\t\tx="));
		Serial.print(curOrientation->getNormalized().x);
		Serial.print(F("\t\ty="));
		Serial.print(curOrientation->getNormalized().y);
		Serial.print(F("\t\tz="));
		Serial.print(curOrientation->getNormalized().z);
		Serial.print(F("\t\tcurRotation\t\tw="));
		Serial.print(curRotation->getNormalized().w);
		Serial.print(F("\t\tx="));
		Serial.print(curRotation->getNormalized().x);
		Serial.print(F("\t\ty="));
		Serial.print(curRotation->getNormalized().y);
		Serial.print(F("\t\tz="));
		Serial.println(curRotation->getNormalized().z);
	}
#endif
	return abs(curRotation->w) <= mpTolData->mSwingSmall //<= detect a movement...
			// ...but refuse to trigger event on pure blade axis rotations
			and not (abs(prevRotation->x - curRotation->x)
					> abs(prevRotation->y - curRotation->y)
					and abs(prevRotation->x - curRotation->x)
							> abs(prevRotation->z - curRotation->z));
}

bool Mpu6050MotionManager::IsClash() {
#ifdef DEBUG_MPU
	if (mpuIntStatus > 60 and mpuIntStatus < 70)
	Serial.println(mpuIntStatus);
#endif
	return mpuIntStatus > 60 and mpuIntStatus < 70;
}

void Mpu6050MotionManager::Update() {
// if programming failed, don't try to do anything
	if (!dmpReady)
		return;

// wait for MPU interrupt or extra packet(s) available
//	while (!interrupt && mpuFifoCount < packetSize) {
	/* other program behavior stuff here
	 *
	 * If you are really paranoid you can frequently test in between other
	 * stuff to see if interrupt is true, and if so, "break;" from the
	 * while() loop to immediately process the MPU data
	 */
//	}
// reset interrupt flag and get INT_STATUS byte
	mClashInt = false;
	mpuIntStatus = mpu->getIntStatus();

// get current FIFO count
	mpuFifoCount = mpu->getFIFOCount();

// check for overflow (this should never happen unless our code is too inefficient)
	if ((mpuIntStatus & 0x10) || mpuFifoCount == 1024) {
// reset so we can continue cleanly
		mpu->resetFIFO();

// otherwise, check for DMP data ready interrupt (this should happen frequently)
	} else if (mpuIntStatus & 0x02) {
// wait for correct available data length, should be a VERY short wait
		while (mpuFifoCount < packetSize)
			mpuFifoCount = mpu->getFIFOCount();

// read a packet from FIFO
		mpu->getFIFOBytes(fifoBuffer, packetSize);

// track FIFO count here in case there is > 1 packet available
// (this lets us immediately read more without waiting for an interrupt)
		mpuFifoCount -= packetSize;

//Making the last orientation the reference for next rotation
		*prevOrientation = curOrientation->getConjugate();

//retrieve current orientation value
		mpu->dmpGetQuaternion(curOrientation, fifoBuffer);

//We calculate the rotation quaternion since last orientation
		*prevRotation = *curRotation;
		*curRotation = prevOrientation->getProduct(
				curOrientation->getNormalized());
	}
}

EMagnitudes Mpu6050MotionManager::GetSwingMagnitude() {
	if (curRotation->w < mpTolData->mSwingMedium) {  // below 16 degrees rotations
		return eeLarge;
	} else if (curRotation->w >= mpTolData->mSwingMedium and curRotation->w < mpTolData->mSwingSmall) { // between 16 and 51,5 degrees rotations
		return eeMedium;
	} else if (curRotation->w >= mpTolData->mSwingSmall) { // above 51,5 degrees rotations
		return eeSmall;
	}
}
