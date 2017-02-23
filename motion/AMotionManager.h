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
 * IMotionManager.h
 *
 *  Created on: Mar 10, 2016
 *      Author: JakeSoft
 */

#if not defined IMOTIONMANAGER_H_
#define IMOTIONMANAGER_H_

enum EMagnitudes {
	eeSmall, eeMedium, eeLarge
};



/**
 * This abstract class provides generic methods to detect motion. Subclasses
 * should provide motion detection specific to whatever specific sensors are
 * being used to detect motion.
 */
class AMotionManager {
public:
	/**
	 * Returns TRUE if the last update cycle detected a swing event.
	 * Returns:
	 * 	TRUE if a swing has happened, FALSE otherwise.
	 */
	virtual bool IsSwing() = 0;

	/**
	 * Returns TRUE if the last update cycle detected a clash event.
	 * Returns:
	 *  TRUE if a clash happened, FALSE otherwise.
	 */
	virtual bool IsClash() = 0;

	/**
	 * Returns TRUE if the last update cycle detected a twist event.
	 * Subclasses are expected to provide an overrride if they support this
	 * optional feature. By default, it will always return FALSE indicating
	 * no twist has occured.
	 *
	 * Returns:
	 *  TRUE if a twist happened, FALSE otherwise.
	 */
	virtual bool IsTwist()
	{
		return false;
	}

	/**
	 * This method should be called every cycle or at some reasonable interval
	 * to update the motion detection with the latest sensor readings.
	 */
	virtual void Update() = 0;

	/**
	 * Initialize and prepare to run. Implement things that should happen
	 * on startup such as setting I/O pins.
	 */
	virtual void Init() = 0;

	/**
	 * Fetch the magnitude of the last swing. Overriding this method is
	 * optional in subclasses if they wish to provide a way to
	 * distinguish between different types of swings (slow vs. fast).
	 *
	 * Returns:
	 *   eeLarge by default, subclasses can choose to override this behavior.
	 */
	virtual EMagnitudes GetSwingMagnitude() {
		return eeLarge;
	}

	/**
	 * Fetch the magnitude of the last clash. Overriding this method is
	 * optional in subclasses if they wish to provide a way to
	 * distinguish between different types of clash (hard vs. soft).
	 *
	 * Returns:
	 *   eeLarge by default, subclasses can choose to override this behavior.
	 */
	virtual EMagnitudes GetClashMagnitude() {
		return eeLarge;
	}

};

#endif /* IMOTIONMANAGER_H_ */
