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

/**
 The purpose of this sketch is to provide a concrete example of simple saber
 core code to demonstrate basic and intended usage of the USaber architecture.
 The sketch is written for the DIYino Prime V1 prop board and assumes that
 hardware, however the sketch should also work with the component parts if you
 built your own with an MPU6050 and DFPlayer MP3 module.

 Hardware
 --------
   Sound: DIYinoPlayer (Should be compatible with DFPlayer MP3 modules)
   Blade: Single LED in-hilt blade
   Motion: MPU6050

 Activation switch is on Pin 4. Assumed to be a normally open momentary switch
 that shorts to ground when closed. The activations switch turns the saber on
 and off.

 USaber Components Used
 ----------------------
 - DIYinoPlayer (for sound)
 - SingleLedBlade (for blade control using pin 3, A.K.A. "LS1" on the DIYino)
 - MPU6050MotionManager (for motion detection)

 USaber Build Directions
 -----------------------
 In USaberConfig.h, ensure BUILD_MPU6050, BUILD_I2CDEV, and BUILD_DIYINOSOUNDPLAYER
 are all #defined to enable them to build. They should be on by default, but verify
 if your build fails.

 */


/*
 * DIYinoSaber.ino
 *
 *  Created on: Jul 12, 2016
 *      Author: JakeSoft
 */

#include <USaber.h>
#include <Wire.h>
#include "Pins_DIYinoV1.h"

#define ACTIVATE_PIN 4 //Pin for the activation button

/**
 * Milliseconds for power-up sequence. Typically, should set this to the
 * length of your power-up sound. An arbitrary value is used here and should
 * be adjusted to match the sounds you are using.
 */
#define POWER_UP_TIME 1150
/**
 * Milliseconds for power-down sequence. Typically, should set this to the
 * length of your power-down sound. An arbitrary value is used here and should
 * be adjusted to match the sounds you are using.
 */
#define POWER_DOWN_TIME 620

#define AUTO_SHUTDOWN_TIME 90000 //Milliseconds of idle time before automatic shutdown

/**
 * Milliseconds to wait after a clash event before checking for motion again.
 * This will typically be the length of your longest clash sound.
 */
#define SWING_SUPPRESS_TIME 1000

/**
 * Milliseconds to wait after a swing event before another swing event can
 * occur. Swing detections will be ignored until the interval expires.
 */
#define SWING_DEBOUCE_INTERVAL 350

/**
 * Milliseconds to wait after a clash event before another clash event can occur.
 * Clash detections will be ignored until the interval expires.
 */
#define CLASH_SUPPRESS_TIME 100

/**
 * Milliseconds to wait after playing a sound. This is necessary to avoid jamming the
 * sound module with commands faster than it can respond.
 */
#define SOUND_DELAY_TIME 60

//Saber components
DIYinoSoundMap gSoundMap;
ASoundPlayer* gpSoundPlayer;
IBladeManager* gpBlade;
AMotionManager* gpMotion;

//Global variables
bool gSaberIsOn; //Flag indicates if saber is on
unsigned long gLastClashTime; //Time when the last clash event occurred
unsigned long gLastSwingTime; //Time when last swing event occurred
MPU6050TolData gToleranceData; //Tolerance threshold data for MPU6050 motion manager
MPU6050CalibrationData gMpuCalData; //Calibration data for the MPU6050 motion manager
unsigned long gMotionUpdateTime; //Last time motion detection was updated

void setup()
{
	//Default all values to zero in the sound map
    //Not strictly necessary, but a good idea
	memset(&gSoundMap, 0, sizeof(WT588DSoundMap));

	/***
	 * Set up sound map so sketch knows where sounds are on the SD card
	 * and what features are supported. You only need to set the fields
	 * you intend to use. These values should be adjusted to match how your
	 * SD card sounds are configured.
	 */
	gSoundMap.Features.FontIdsPerFont = 0;
	gSoundMap.Features.HumSoundsPerFont = 1;
	gSoundMap.Features.PowerUpSoundsPerFont = 1;
	gSoundMap.Features.PowerDownSoundsPerFont = 1;
	gSoundMap.Features.ClashSoundsPerFont = 3;
	gSoundMap.Features.SwingSoundsPerFont = 8;
	gSoundMap.Features.LockupSoundsPerFont = 0;
	gSoundMap.Features.BlasterSoundsPerFont = 3;
	gSoundMap.Features.ForceSoundsPerFont = 0;
	gSoundMap.Features.CustomSoundsPerFont = 1;
	gSoundMap.Features.MenuSounds = 19;

	//Sound locations
	gSoundMap.Locations.BaseAddr = 0;
	gSoundMap.Locations.BlasterBase = 35;
	gSoundMap.Locations.BootBase = 16;
	gSoundMap.Locations.ClashBase = 32;
	gSoundMap.Locations.SwingBase = 24;
	gSoundMap.Locations.LockupBase = 0;
	gSoundMap.Locations.PowerupBase = 21;
	gSoundMap.Locations.PowerdownBase = 22;
	gSoundMap.Locations.HumBase = 23;
	gSoundMap.Locations.FontIdBase = 0;
	gSoundMap.Locations.CustomBase = 39;
	gSoundMap.Locations.MenuBase = 1;

	//Create a sound player (using the map that was defined above)
	gpSoundPlayer = new DIYinoSoundPlayer(SOUND_TX_PIN, SOUND_RX_PIN, &gSoundMap);

	//Create a blade
	gpBlade = new SingleLedBlade(LED_LS1_PIN);

	//Motion manager swing tolerances
	gToleranceData.mSwingLarge  = 18;
	gToleranceData.mSwingMedium = 9;
	gToleranceData.mSwingSmall  = 5;
	//Calibration data (note, this will be different for your MPU. Calibrate it!
	gMpuCalData.mAccelXOffset = -1848;
	gMpuCalData.mAccelYOffset = -5409;
	gMpuCalData.mAccelZOffset = 1640;
	gMpuCalData.mGyXOffset = -5;
	gMpuCalData.mGyYOffset = 48;
	gMpuCalData.mGyZOffset = 23;
	//Create motion manager
	gpMotion = new Mpu6050MotionManager(&gToleranceData, &gMpuCalData);

	//Initialize objects
	gpBlade->Init();
	gpMotion->Init();
	gpSoundPlayer->Init();

	//Un-comment this to adjust volume automatically on startup
	//Comment out this line to use the sound module's default volume
	gpSoundPlayer->SetVolume((int)(gpSoundPlayer->Features().MaxVolume / 2));

	delay(100);

	//Play the boot sound
	gpSoundPlayer->PlaySound(eeBootSnd);

	gSaberIsOn = false;
	gLastClashTime = 0;
	gLastSwingTime = 0;

	//Set up the activation button pin
	pinMode(ACTIVATE_PIN, INPUT);
	digitalWrite(ACTIVATE_PIN, HIGH);

	delay(20);

	gMotionUpdateTime = 0;

}

// The loop function is called in an endless loop
void loop()
{
	bool lTimeout = false;
	if(millis() - gLastClashTime > AUTO_SHUTDOWN_TIME &&
	   millis() - gLastSwingTime > AUTO_SHUTDOWN_TIME &&
	   gSaberIsOn)
	{
		lTimeout = true;
	}

	//Update motion detection, but not more than once per millisecond
	if(millis() - gMotionUpdateTime > 1)
	{
		gpMotion->Update();
		gMotionUpdateTime = millis();
	}

	if(false == gSaberIsOn) //Saber is off
	{
		if(LOW == digitalRead(ACTIVATE_PIN))
		{

			//Play the power up sound
			gpSoundPlayer->PlaySound(eePowerUpSnd);
			//Do blade power up sequence
			gpBlade->SetChannel(255, 0); //Set channel 0 to full power
			while(!gpBlade->PowerUp(POWER_UP_TIME))
			{
				//Do nothing, just wait for the powerup to end
			}

			//Wait for the user to let off the button
			while(LOW == digitalRead(ACTIVATE_PIN))
			{
				delay(100);
			}
			gSaberIsOn = true; //Now the saber is on
			//Reset the motion event time tracker variables to the current time
			gLastSwingTime = millis();
			gLastClashTime = millis();
		}
	} //End of Saber Off state
	else //Saber is on
	{
		if(gpMotion->IsClash()) //A clash event is detected
		{
			//Calculate time since last clash event
			unsigned long lClashTimeDelta = millis() - gLastClashTime;

			if(lClashTimeDelta > CLASH_SUPPRESS_TIME)
			{
				//Record the time of the clash event
				gLastClashTime = millis();

				//Turn the blade on full
				gpBlade->On();

				//Play a clash sound
				gpSoundPlayer->PlayRandomSound(eeClashSnd);
				delay(SOUND_DELAY_TIME);
			}

		}
		else if(gpMotion->IsSwing()) //A swing event is detected
		{
			//Calculate time since last clash event
			unsigned long lClashTimeDelta = millis() - gLastClashTime;
			//Calculate time since last swing event
			unsigned long lSwingTimeDelta = millis() - gLastSwingTime;
			if(lClashTimeDelta > SWING_SUPPRESS_TIME
					&& lSwingTimeDelta > SWING_DEBOUCE_INTERVAL)
			{
				//Record time of the swing event
				gLastSwingTime = millis();

				//Play a swing sound
				gpSoundPlayer->PlayRandomSound(eeSwingSnd);
				delay(SOUND_DELAY_TIME);
			}
		}
		//User pressed the button and wants to power down
		else if(LOW == digitalRead(ACTIVATE_PIN) || lTimeout )
		{
			//Play a power down sound
			gpSoundPlayer->PlaySound(eePowerDownSnd);

			//Power down the blade
			while(!gpBlade->PowerDown(POWER_DOWN_TIME))
			{
				//Do nothing, just wait for the power down to end
			}

			//Wait for the user to let off the button
			while(LOW == digitalRead(ACTIVATE_PIN))
			{
				delay(100);
			}
			gSaberIsOn = false; //Now the saber is on
		}
		else
		{
			//Apply blade flicker effects
			gpBlade->ApplyFlicker(1);
		}
	} //End of saber on state

}
