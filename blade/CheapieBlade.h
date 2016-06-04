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
 * CheapieBlade.h
 *
 *  Created on: Mar 3, 2016
 *      Author: JakeSoft <http://forum.arduino.cc/index.php?topic=261980.0>
 */

#ifndef CHEAPIEBLADE_H_
#define CHEAPIEBLADE_H_

#include "IBladeManager.h"

class CheapieBlade : public IBladeManager
{
public:
  CheapieBlade(int aLedPin);
  virtual ~CheapieBlade();

  virtual void Init();
  virtual bool PowerUp(int aRampTime);
  virtual bool PowerDown(int aRampTime);
  virtual void ApplyFlicker(int aType);
  virtual void SetChannel(unsigned char aLevel, int aChannel);
  virtual void SetFlickerType(int aType);
  virtual void PerformIO();
  virtual void Off();
  virtual void On();
  virtual BladeMetadata GetFeatures();

protected:
  //Pin that controls the LED
  int mLedPin;
  //Target power level
  unsigned char mLedPowerLevel;
  //Blade features
  BladeMetadata mFeatures;
};

#endif /* CHEAPIEBLADE_H_ */
