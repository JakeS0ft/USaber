# USaber
Universal Saber Library for Arduino

## Install
To install this library, download the zip file then extract all files to your Arduino libraries folder. Rename the folder to USaber. 

## Thoery Of Operation
This library is based on the abstraction of saber control into three major sub-functions:

1) Blade - Typically LEDs which illuminate the blade

2) Sound Player - Something to play back sounds

3) Motion Sense - Something to detect motion

This library provides classes to perform each of these major sub-functions using the various supported hardware. 

## Supported Hardware
### Sound Playback Devices
WT588D
DFPlayer
### Motion Detection
Swing & Clash sensors (generic, but recommend SW-200D and SW-18020P)
MPU-6050

## Blades

The following classes are provided to support different blade types. A brief description is provided here. For more details, see the header file for each class under the blade directory within the library's file structure.

### IBladeManager
This is the base class for all blades supported by the Universal Saber Library. Common functions are defined here and all blades must support them, although in some cases a "do nothing" method may be provided. See the header file IBladeManger.h for more details about each method and it's intended purpose.

#### Methods
Init()
Off()
On()
PowerUp()
PowerDown()
SetChannel()
ApplyFlicker()
GetFeatures()
PerformIO()

### CheapieBlade
This class supports one channel and emulates the blade operation of the so-called "econo sound" boards often harvested from inexpensive toys.

### CrossGuardBlade
This blade type supports two channels. One channel is for the main blade, the other controls the cross-guards.

### SingleLedBlade
This blade type supports one channel and is intended for a single LED style blade. It supports gradual "ramping" duirng power up and power down and various flicker/shimmer effects.

### StringBlade
This blade type supports a six-channel LED string blade. 

### RGBBlade
This blade type has three channels and is intended for color-mixing using three LEDs (Red, Green, and Blue).

## Sound Players

Sound player classes are used to play back sounds on various devices. The following classes are provided.

### ASoundPlayer
This abstract class is the base class for all sound players. Common functions are defined here and all sound players must support them, although some functions may be “do nothing” if a particular sound module does not support the feature. See the header file ASoundPlayer.h for more details about each method and it's intended purpose.
#### Methods
Init()
Features()
IsBusy()
PlayRandomSound()
PlaySound()
SetFont()
SetVolume()
Stop()

### DFPlayerSoundPlayer
Plays sounds using DFPlayer mini MP3 player from DFRobot. (http://www.dfrobot.com/wiki/index.php/DFPlayer_Mini_SKU:DFR0299)

### WT588DSoundPlayer
Plays sounds using the Wayronic's WT588D sound module.
(http://www.waytronic.com/voicemodule_show.asp?/35.html)



(TBD)

## Motion Managers

Motion manager classes are used to detect swing and clash event. A “swing” event means the saber is moving. A “clash” means an impact is detected. The following classes are provided.

### AMotionManager
This is the base class for all motion manager classes. Common functions are defined here and all motion mangers must support them. See the header file AMotionManger.h for more details about each method and it's intended purpose.

#### Methods
Init()
IsClash()
IsSwing()
GetClashMagnitude()
GetSwingMagnitude()
Update()

### Mpu6050MotionManger
Detects motion with an MPU-6050 Accelerometer + Gyro MPU.
### SimpleMotionManager
Detects motion with swing (tilt) and clash (impact) switches.
