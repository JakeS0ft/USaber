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
 * Mpu6050LiteMotionManager.ino
 *  This will test the MPU6050 light-weight motion manager.
 *  Note: The Y-axis of the MPU6050 is assumed to be parallel with the saber blade.
 *  Created on: 14 Feb 2017
 *      Author: JakeSoft <http://forum.arduino.cc/index.php?topic=261980.0>
 */
#include "Arduino.h"

#include <USaber.h>

#define SMALL_THRESHOLD   110
#define MEDIUM_THRESHOLD  200
#define LARGE_THRESHOLD   350
#define CLASH_THRESHOLD   32

//Tolerance threshold data for MPU6050 motion manager
MPU6050LiteTolData gToleranceData;

unsigned long gLastSwingTime;
unsigned long gLastClashTime;

/**
 * This function will test a motion manager. Run it in a loop and use
 * serial monitoring to see when motion events are triggered.
 */
void TestMotion(AMotionManager* apMotion)
{
  //Note: The Init() function must be called for changes
  //to clash tolerance to take effect. Must always
  //be called prior to calling Update() or the MPU6050
  //will remain in sleep mode.
  apMotion->Init();

  //Run the test
  while(true)
  {
    unsigned long lNow = millis();
    //Read the MPU6050 for data
    apMotion->Update();

    //Check for clashes, debounced a little
    if(apMotion->IsClash() && lNow - gLastClashTime > 100)
    {
      gLastClashTime = millis();
      Serial.print("CLASH detected:");

      //Note: This is generic motion manager test code
      //The class under test will only ever report
      //"Large" clashes.
      switch(apMotion->GetClashMagnitude())
      {
      case eeSmall:
        Serial.println("Small");
        break;
      case eeMedium:
        Serial.println("Medium");
        break;
      case eeLarge:
        Serial.println("Large");
        break;
      default:
        Serial.println("Unknown");
        break;
      }
    }
    //Check for swings. Debounce it a little so we don't get a ton of swing reports
    else if(apMotion->IsSwing() && lNow - gLastSwingTime > 100)
    {
      gLastSwingTime = millis();
      Serial.print("Swing detected:");
      switch(apMotion->GetSwingMagnitude())
      {
      case eeSmall:
        Serial.println("Small");
        break;
      case eeMedium:
        Serial.println("Medium");
        break;
      case eeLarge:
        Serial.println("Large");
        break;
      default:
        Serial.println("Unknown");
        break;
      }
    }
    else if(apMotion->IsTwist() && lNow - gLastSwingTime > 100)
    {
      gLastSwingTime = millis();
      Serial.println("Twist detected.");
    }
  }

}

void setup()
{
  /**
   * Set MPU6050 swing tolerance thresholds here.
   * Tweak as needed to adjust sensitivity
   */
  //Recommended swing tolerance values are between 512 and 50
  //The higher the number, the less sensitive
  gToleranceData.mSwingLarge  = LARGE_THRESHOLD;
  gToleranceData.mSwingMedium = MEDIUM_THRESHOLD;
  gToleranceData.mSwingSmall  = SMALL_THRESHOLD;
  //Recommended the clash threshold be no higher than 64 and no lower than 10
  //The higher the number, the less sensitive
  gToleranceData.mClash = CLASH_THRESHOLD;

  Serial.begin(9600);
}

void loop()
{
  Serial.println("Testing MPU6050LiteMotionManager");
  AMotionManager* lpMotion = new Mpu6050LiteMotionManager(&gToleranceData);

  TestMotion(lpMotion);
  Serial.println("Test ends.");

  delete lpMotion;
  delay(2000);
}

