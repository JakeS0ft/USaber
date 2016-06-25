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
 * USaberConfig.h
 *
 *  This file is to be included in certain "heavy weight" features of the
 *  Universal Saber library so that they can be optionally excluded from the
 *  build. The code in certain files is #ifdef wrapped such that they contain
 *  no code or only "Light Weight" code unless their corresponding BUILD_XXX
 *  #define exists.
 *
 *  To remove a feature by preventing it from building, simply comment out the
 *  corresponding #define for the feature.
 *
 *  The reason this is necessary is that some of the libraries introduce
 *  declarations that allocate static or global memory or inflate the compiled
 *  hex file size regardless of if the feature is actually used. Not only is
 *  this is an unnecessary waste of resources, it may also introduce
 *  incompatibility with certain MCUs such as the ATTiny chips which don't
 *  support I2C or Wire.
 *
 *	Note that features that do not perform these kinds of "Heavy" actions are
 *	always included as they cost little or nothing in the way of RAM or
 *	compiled hex file size unless they are actually used.
 *
 *	For now, all features are enabled by default.
 *
 *  Created on: Jun 24, 2016
 *      Author: JakeSoft
 */

#ifndef _USABER_USABERCONFIG_H_
#define _USABER_USABERCONFIG_H_

//Comment out the line for which you'd like to exclude the corresponding feature

/**
 * Build the MPU6050MotionManager.
 * Note: This requires that I2CDEV also be enabled below.
 */
//#define BUILD_MPU6050

/**
 * Build USaber's built-in I2CDev library.
 * Note: This requires that Wire also be included in the sketch.
 */
//#define BUILD_I2CDEV

/**
 * Build USaber's built-in Software Serial library.
 * Note: This must be included for the DFPlayerSouundPlayer to build.
 * Note: Some other features may not work if this is excluded from the build.
 */
//#define BUILD_SOFTWARE_SERIAL

/**
 * Build the DFPlayerSoundPlayer.
 * Note: Software serial must also be enabled for this to work.
 */
//#define BUILD_DFPLAYERSOUNDPLAYER

#endif /* LIBRARIES_USABER_USABERCONFIG_H_ */
