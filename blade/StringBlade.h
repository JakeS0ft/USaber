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
 * StringBlade.h
 *
 *  Created on: Mar 7, 2016
 *      Author: JakeSoft <http://forum.arduino.cc/index.php?topic=261980.0>
 */

#ifndef STRINGBLADE_H_
#define STRINGBLADE_H_

#include "IBladeManager.h"

class StringBlade : public IBladeManager
{
public:
	StringBlade(int aSeg1, int Seg2, int Seg3, int Seg4, int Seg5, int Seg6);
	virtual ~StringBlade();

	virtual void Init();
	virtual void SetChannel(unsigned char aPower, int aChannel);
	virtual void On();
	virtual void Off();
	virtual bool PowerUp(int aRampTime);
	virtual bool PowerDown(int aRampTime);
	virtual void ApplyFlicker(int aType);
	virtual void PerformIO();
	virtual BladeMetadata GetFeatures();

protected:
	int mChannelPins[6];
	unsigned char mPowerLevels[6];

	bool mIsOn;
};

#endif /* UNIVERSALSABER_TCSTRINGBLADE_H_ */
