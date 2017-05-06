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
 * USaber.h
 *
 *  Created on: Mar 9, 2016
 *      Author: JakeSoft <http://forum.arduino.cc/index.php?topic=261980.0>
 */

#ifndef USABER_H_
#define USABER_H_

#include "USaberConfig.h"

#ifdef AUTO_INCLUDE_WIRE
#include <Wire.h>
#endif

//Blade managers
#include "blade/IBladeManager.h" //Abstract interface/base class
#include "blade/CheapieBlade.h"
#include "blade/SingleLedBlade.h"
#include "blade/CrossGuardBlade.h"
#include "blade/StringBlade.h"
#include "blade/RGBBlade.h"

//Motion managers
#include "motion/AMotionManager.h" //Abstract base class
#include "motion/SimpleMotionManager.h"
#include "motion/ADXL335MotionManager.h"

#ifdef BUILD_MPU6050LITE
#include "motion/Mpu6050LiteMotionManager.h"
#include "motion/Mpu6050AdvancedMotionManager.h"
#endif

#ifdef BUILD_MPU6050
#include "motion/Mpu6050MotionManager.h"
#endif

//Sound players
#include "sound/ASoundPlayer.h"
#include "sound/WT588DSoundPlayer.h"

#ifdef BUILD_DFPLAYERSOUNDPLAYER
#include "sound/DFPlayerSoundPlayer.h"
#endif

#ifdef BUILD_DIYINOSOUNDPLAYER
#include "sound/DIYinoSoundPlayer.h"
#endif

#endif
