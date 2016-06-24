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
The purpose of this sketch is to test the WT588D implimentation of the Universal 
Saber Library's sound interface. It may be modified to test other sound modules.
The only things that should need changing is what type of module is instaitated
in the loop() function and add whatever extra parameterization may be needed.
*/

//Note: Ardino IDE Versions previous to 1.6.6 must include Wire.h
#include <Wire.h>
#include <USaber.h>
#define PLAY_DURATION 2000 //How long sound will be allowed to play before next
#define RANDOM_ATTEMPTS 5 //How many times to play during random playback test

#define WT588D_SDA 13 //Module pin for DATA

static WT588DSoundMap gSoundMap;
 
/**
 * Plays all sounds in the player's sound map.
 * Args:
 *  apSoundPlayer - Pointer to the sound player to test.
 */
void PlayAllSounds(ASoundPlayer* apSoundPlayer)
{
	Serial.println("Boot");
	apSoundPlayer->PlaySound(eeBootSnd);
	delay(PLAY_DURATION);

	Serial.print("Font ID");
	for(int lnIdx = 0; lnIdx < apSoundPlayer->Features().FontIdsPerFont; lnIdx++)
	{
		Serial.print(" ");
		Serial.print(lnIdx);
		apSoundPlayer->PlaySound(eeFontIdSnd, lnIdx);
		delay(PLAY_DURATION);
	}

	Serial.println(".");
    Serial.print("Powerup");
	for(int lnIdx = 0; lnIdx < apSoundPlayer->Features().PowerUpSoundsPerFont; lnIdx++)
	{
		Serial.print(" ");
		Serial.print(lnIdx);
		apSoundPlayer->PlaySound(eePowerUpSnd, lnIdx);
		delay(PLAY_DURATION);
	}

	Serial.println(".");
    Serial.print("Swing");
	for(int lnIdx = 0; lnIdx < apSoundPlayer->Features().SwingSoundsPerFont; lnIdx++)
	{
		Serial.print(" ");
		Serial.print(lnIdx);
		apSoundPlayer->PlaySound(eeSwingSnd, lnIdx);
		delay(PLAY_DURATION);
	}

	Serial.println(".");
	Serial.print("Blaster ");
	for(int lnIdx = 0; lnIdx < apSoundPlayer->Features().BlasterSoundsPerFont; lnIdx++)
	{
		Serial.print(" ");
		Serial.print(lnIdx);
		apSoundPlayer->PlaySound(eeBlasterSnd, lnIdx);
		delay(PLAY_DURATION);
	}

	Serial.println(".");
	Serial.print("Clash");
	for(int lnIdx = 0; lnIdx < apSoundPlayer->Features().ClashSoundsPerFont; lnIdx++)
	{
		Serial.print(" ");
		Serial.print(lnIdx);
		apSoundPlayer->PlaySound(eeClashSnd, lnIdx);
		delay(PLAY_DURATION);
	}

	Serial.println(".");
	Serial.print("Custom");
	for(int lnIdx = 0; lnIdx < apSoundPlayer->Features().CustomSoundsPerFont; lnIdx++)
	{
		Serial.print(" ");
		Serial.print(lnIdx);
		apSoundPlayer->PlaySound(eeCustomSnd, lnIdx);
		delay(PLAY_DURATION);
	}

	Serial.println(".");
    Serial.print("Force");
	for(int lnIdx = 0; lnIdx < apSoundPlayer->Features().ForceSoundsPerFont; lnIdx++)
	{
		Serial.print(" ");
		Serial.print(lnIdx);
		apSoundPlayer->PlaySound(eeForceSnd, lnIdx);
		delay(PLAY_DURATION);
	}

	Serial.println(".");
    Serial.print("Lockup");
	for(int lnIdx = 0; lnIdx < apSoundPlayer->Features().LockupSoundsPerFont; lnIdx++)
	{
		Serial.print(" ");
		Serial.print(lnIdx);
		apSoundPlayer->PlaySound(eeLockupSnd, lnIdx);
		delay(PLAY_DURATION);
	}

	Serial.println(".");
    Serial.print("Powerdown");
	for(int lnIdx = 0; lnIdx < apSoundPlayer->Features().PowerDownSoundsPerFont; lnIdx++)
	{
		Serial.print(" ");
		Serial.print(lnIdx);
		apSoundPlayer->PlaySound(eePowerDownSnd, lnIdx);
		delay(PLAY_DURATION);
	}

	Serial.println(".");
    Serial.print("Hum");
	for(int lnIdx = 0; lnIdx < apSoundPlayer->Features().HumSoundsPerFont; lnIdx++)
	{
		Serial.print(" ");
		Serial.print(lnIdx);
		apSoundPlayer->PlaySound(eeHumSnd, lnIdx);
		delay(PLAY_DURATION);
	}

	Serial.println(".");
    Serial.print("Menu");
	for(int lnIdx = 0; lnIdx < apSoundPlayer->Features().MenuSounds; lnIdx++)
	{
		Serial.print(" ");
		Serial.print(lnIdx);
		apSoundPlayer->PlaySound(eeMenuSoundSnd, lnIdx);
		delay(PLAY_DURATION);
	}

}

/**
 * Subfunction called by PlayRandomSounds() test. Randomly plays a sound of the
 * type specified.
 *
 * Args:
 *  apSoundPlayer - Pointer to the sound player to test.
 *  aSndType - Type of sound to randomly play.
 *  aSoundsPerFont - Count of the type of sound expected. For example: if
 *                   there were 4 clash sounds, then this would be 4 if 
 *                   testing playback of clash sounds.
 */
void PlayRandomSubtest
	(ASoundPlayer* apSoundPlayer, ESoundTypes aSndType,
	unsigned char aSoundsPerFont)
{
	if(aSoundsPerFont > 0)
	{
		for(int lnIdx = 0; lnIdx < RANDOM_ATTEMPTS; lnIdx++)
		{
			Serial.print(" ");
			Serial.print(lnIdx);
			apSoundPlayer->PlayRandomSound(aSndType);
			delay(PLAY_DURATION);
		}
	}
}

/**
 * Playback all sounds in random fashion by type.
 * Args:
 *  apSoundPlayer - Pointer to the sound player to test.
 */
void PlayRandomSounds(ASoundPlayer* apSoundPlayer)
{
	Serial.println("Random Boot");
	apSoundPlayer->PlayRandomSound(eeBootSnd);
	delay(PLAY_DURATION);

	Serial.print("Random Font ID");
	PlayRandomSubtest(apSoundPlayer, eeFontIdSnd, apSoundPlayer->Features().FontIdsPerFont);

	Serial.println(".");
	Serial.print("Random Powerup");
	PlayRandomSubtest(apSoundPlayer, eePowerUpSnd, apSoundPlayer->Features().PowerUpSoundsPerFont);

	Serial.println(".");
	Serial.print("Random Swing");
	PlayRandomSubtest(apSoundPlayer, eeSwingSnd, apSoundPlayer->Features().SwingSoundsPerFont);

	Serial.println(".");
	Serial.print("Random Blaster ");
	PlayRandomSubtest(apSoundPlayer, eeBlasterSnd, apSoundPlayer->Features().BlasterSoundsPerFont);

	Serial.println(".");
	Serial.print("Random Clash ");
	PlayRandomSubtest(apSoundPlayer, eeClashSnd, apSoundPlayer->Features().ClashSoundsPerFont);

	Serial.println(".");
	Serial.print("Random Custom");
	PlayRandomSubtest(apSoundPlayer, eeCustomSnd, apSoundPlayer->Features().CustomSoundsPerFont);

	Serial.println(".");
    Serial.print("Random Force");
    PlayRandomSubtest(apSoundPlayer, eeForceSnd, apSoundPlayer->Features().ForceSoundsPerFont);

	Serial.println(".");
    Serial.print("Random Lockup");
    PlayRandomSubtest(apSoundPlayer, eeLockupSnd, apSoundPlayer->Features().LockupSoundsPerFont);

	Serial.println(".");
    Serial.print("Random Powerdown");
    PlayRandomSubtest(apSoundPlayer, eePowerDownSnd, apSoundPlayer->Features().PowerDownSoundsPerFont);

	Serial.println(".");
    Serial.print("Random Hum");
    PlayRandomSubtest(apSoundPlayer, eeHumSnd, apSoundPlayer->Features().HumSoundsPerFont);

	Serial.println(".");
    Serial.print("Random Menu");
    PlayRandomSubtest(apSoundPlayer, eeMenuSoundSnd, apSoundPlayer->Features().MenuSounds);
}

/**
 * This function tests sound players.
 * Args:
 *  aSoundPlayer - Pointer to the player to test.
 */
void TestSound(ASoundPlayer* apSoundPlayer)
{
	apSoundPlayer->Init();

	Serial.println("\nTesting sounds...");
	PlayAllSounds(apSoundPlayer);

	Serial.println("\nTesting random sounds...");
	PlayRandomSounds(apSoundPlayer);

	Serial.println("\nTesting Stop functionality...");
	for(int lnAttempt = 0; lnAttempt < 4; lnAttempt++)
	{
		Serial.println("Playing Hum sound.");
		apSoundPlayer->PlaySound(eeHumSnd, 0);
		delay(PLAY_DURATION);
		Serial.println("Stop");
		apSoundPlayer->Stop();
		delay(PLAY_DURATION);
	}

	Serial.println("Testing volume adjustment...");
	Serial.println("Playing Hum sound.");
	apSoundPlayer->PlaySound(eeHumSnd, 0);
	delay(PLAY_DURATION);
	Serial.print("Adjusting volume... ");
	for(int lVol = apSoundPlayer->Features().MaxVolume; lVol >= 0; lVol--)
	{
		Serial.println(lVol);
		delay(PLAY_DURATION);
		apSoundPlayer->SetVolume(lVol);
	}
	Serial.print("Adjusting volume... ");
	for(int lVol = 0; lVol <= apSoundPlayer->Features().MaxVolume; lVol++)
	{
		Serial.println(lVol);
		delay(PLAY_DURATION);
		apSoundPlayer->SetVolume(lVol);
	}

	Serial.println("Done with volume test.");
	apSoundPlayer->Stop();

}

void setup()
{
  Serial.begin(9600);

}

void loop()
{
	Serial.println("Testing WT588D Sound player");

	//Sound font features. Numbers here are arbitrary.
    //These values should be adjusted to match how your
    //sound module is programmed.
	gSoundMap.Features.FontIdsPerFont = 1;
	gSoundMap.Features.HumSoundsPerFont = 1;
	gSoundMap.Features.PowerUpSoundsPerFont = 1;
	gSoundMap.Features.PowerDownSoundsPerFont = 1;
	gSoundMap.Features.ClashSoundsPerFont = 4;
	gSoundMap.Features.SwingSoundsPerFont = 4;
	gSoundMap.Features.LockupSoundsPerFont = 1;
	gSoundMap.Features.BlasterSoundsPerFont = 1;
	gSoundMap.Features.ForceSoundsPerFont = 0;
	gSoundMap.Features.CustomSoundsPerFont = 0;

	//Other sounds
	gSoundMap.Features.MenuSounds = 12;

	//Sound locations
	gSoundMap.Locations.BaseAddr = 0;
	gSoundMap.Locations.BlasterBase = 12;
	gSoundMap.Locations.BootBase = 70;
	gSoundMap.Locations.ClashBase = 7;
	gSoundMap.Locations.SwingBase = 3;
	gSoundMap.Locations.LockupBase = 11;
	gSoundMap.Locations.PowerupBase = 1;
	gSoundMap.Locations.PowerdownBase = 13;
	gSoundMap.Locations.HumBase = 2;

	//Menu sound locations
	gSoundMap.Locations.MenuBase = 70;

	ASoundPlayer* lpSoundPlayer = new WT588DSoundPlayer(WT588D_SDA, &gSoundMap);

	TestSound(lpSoundPlayer);
	Serial.println("Done with sound test.");
	delay(5000);

	delete lpSoundPlayer;
}


