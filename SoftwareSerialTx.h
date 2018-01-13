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
 * SoftwareSerialTxTx.h
 *
 *	One-way transmit-only serial class.
 *
 *	This work is based on SoftwareSerial included in Arduino IDE version 1.8.1.
 *	(https://github.com/arduino/Arduino/tree/1.8.1)
 *
 *	Credits from original SoftwareSerial class:
 *	-- 20MHz processor support by Garrett Mace (http://www.macetech.com)
*   -- ATmega1280/2560 support by Brett Hagman (http://www.roguerobotics.com/)
 *
 *  Created on: Jan 9, 2018
 *      Author: JakeSoft
 */

#ifndef SoftwareSerialTX_H_
#define SoftwareSerialTX_H_


#include <inttypes.h>
#include <Stream.h>

/******************************************************************************
* Definitions
******************************************************************************/

#ifndef GCC_VERSION
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

class SoftwareSerialTx
{
public:
  // public methods
  SoftwareSerialTx(uint8_t transmitPin, bool inverse_logic = false);
  virtual ~SoftwareSerialTx();
  void begin(long speed);

  void end();

  bool overflow() { bool ret = _buffer_overflow; _buffer_overflow = false; return ret; }
  int peek();

  virtual size_t write(uint8_t byte);
protected:
  // per object data
  uint8_t _transmitBitMask;
  volatile uint8_t *_transmitPortRegister;

  uint16_t _tx_delay;

  uint16_t _buffer_overflow:1;
  uint16_t _inverse_logic:1;

  // static data
  static SoftwareSerialTx *active_object;

  // private methods
  void tx_pin_write(uint8_t pin_state);
  void setTX(uint8_t transmitPin);

  // private static method for timing
  static inline void tunedDelay(uint16_t delay);
};

// Arduino 0012 workaround
#undef int
#undef char
#undef long
#undef byte
#undef float
#undef abs
#undef round



#endif /* SoftwareSerialTX_H_ */
