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
 * IBladeManager.h
 *
 *  Created on: Mar 1, 2016
 *      Author: JakeSoft <http://forum.arduino.cc/index.php?topic=261980.0>
 */

#ifndef UNIVERSALSABER_IBLADEMANAGER_H_
#define UNIVERSALSABER_IBLADEMANAGER_H_

/**
 * Used to describe features of a blade. This can be  used
 * to inform control logic about what the blade can do and avoid
 * invalid or useless calls to subclass methods of IBladeManager.
 */
struct BladeMetadata
{
	//Number of channels supported
	int Channels;
	//Number of flicker effects
	int Flickers;
};

/**
 * This interface class provides a framework for  management of the LEDs that
 * illuminate the main blade. The abstract concept is that each LED is a
 * channel. Each channel can be individually controlled. Convenience functions
 * for common operations are also provided.
 */
class IBladeManager {
public:

	/**
	 * Initialize and prepare to run. Implement things that should happen
	 * on startup such as setting I/O pins.
	 */
	virtual void Init() = 0;

	/**
	 * Power on the blade. Subclasses should implement a power-up sequence.
	 * The sequence can be blocking or non-blocking. Callers should loop until
	 * this method returns TRUE indicating that the power up sequence is
	 * complete. Assumes targets have been set by calling SetChannel() method.
	 *
	 * Args:
	 * 	aRampTime - Time (in milliseconds) from full off to full on.
	 *
	 * 	Returns:
	 * 		TRUE when complete, FALSE otherwise.
	 */
	virtual bool PowerUp(int aRampTime) = 0;

	/**
	 * Ramp down blade power to off. Subclasses should implement a
	 * power-down sequence. Callers should loop until this method
	 * returns TRUE indicating that the power up sequence is complete.
	 *
	 * Args:
	 *  aRampTime - Time (in milliseconds) from full on to full off.
	 * 	Returns:
	 * 		TRUE when complete, FALSE otherwise.
	 */
     virtual bool PowerDown(int aRampTime) = 0;

	/**
	 * Set the PWM values directly for the LED(s). This is intended to
	 * allow for a target to be set on a specific output channel but
	 * no I/O is performed. To perform the I/O and see the effect of
	 * the level adjustment, the PerformIO() method should be called
	 * right after the call to this method. For color mixing, multiple
	 * calls may be made to this method to set individual channels then
	 * a single call to PerformIO() will apply the effect.
	 *
	 * Args:
	 *  aLevel - Level to use (0 to 255 for PWM)
	 *  aChannel - Which channel to set
	 */
	virtual void SetChannel(unsigned char aLevel, int aChannel) = 0;

	/**
	 * Sets flicker/shimmer special effect type. The intention is that this
	 * function be non-blocking and be called cyclicly to apply the desired
	 * effect. Use this to apply flicker patterns or whatever else you want.
	 *
	 * Args:
	 *  aType - Flicker effect to apply.
	 */
	virtual void ApplyFlicker(int aType) = 0;

	/**
	 * Instantly Sets the output pins per output level variables. If the
	 * levels are set to non-zero values, then this has the effect of
	 * turning on the LEDs for the blade. To set the levels manually,
	 * the SetChannel() method should be used.
	 */
	virtual void PerformIO() = 0;

	/**
	 * Instantly turns the blade off. Should set all power levels to zero and
	 * perform any I/O necessary to turn off the blade fully. This is a
	 * convenience function shortcut for setting all levels to zero and calling
	 * PerformIO().
	 */
	virtual void Off() = 0;

	/**
	 * Instantly turns the blade on without any powerup effects. Assumes that
	 * channels have been set by SetChannel() function.
	 */
	virtual void On() = 0;

	/**
	 * Return a structure describing the capabilities of this blade. This is
	 * useful for giving the caller information such as how many flicker patterns
	 * are supported.
	 *
	 * Returns:
	 *  A description of capabilities.
	 */
    virtual BladeMetadata GetFeatures() = 0;

};

#endif /* UNIVERSALSABER_IBLADEMANAGER_H_ */
