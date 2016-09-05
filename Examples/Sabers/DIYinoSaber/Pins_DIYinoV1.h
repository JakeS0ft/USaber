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
 * Pins.h
 *   Defines the pre-mapped pins on a DIYino Prime V1 prop board.
 *
 *  Created on: Jul 12, 2016
 *      Author: JakeSoft
 */

#ifndef PINS_DIYINOV1_H_
#define PINS_DIYINOV1_H_

#define SOUND_TX_PIN 7   //Serial TX pin for sound
#define SOUND_RX_PIN 8   //Serial RX pin for sound

#define LED_LS1_PIN 3    //Low-side 1 for LED control
#define LED_LS2_PIN 5    //Low-side 2 for LED control
#define LED_LS3_PIN 6    //Low-side 3 for LED control
#define LED_LS4_PIN 9    //Low-side 4 for LED control
#define LED_LS5_PIN 10   //Low-side 5 for LED control
#define LED_LS6_PIN 11   //Low-side 6 for LED control

#define MPU_SDA_PIN A4   //I2C serial data line for MPU6050
#define MPU_SCL_PIN A5   //I2C serial clock for MPU6050

#endif /* PINS_DIYINOV1_H_ */
