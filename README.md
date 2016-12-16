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
* WT588D
* DFPlayer
* DIYino
 
### Motion Detection
* Swing & Clash sensors (generic, but recommend SW-200D and SW-18020P)
* MPU-6050
* ADXL335

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

### DFPlayerSoundPlayer[Depreciated]
Plays sounds using DFPlayer mini MP3 player from DFRobot. (http://www.dfrobot.com/wiki/index.php/DFPlayer_Mini_SKU:DFR0299)
Note: This class is only known to work with modules with built-in Flash and won't work with SD card versions. DIYinoSoundPlayer is perfered is more proven to work with the SD-card-based modules.

### DIYinoSoundPlayer
SoundPlayer for the DIYino prop board. 
(http://www.fx-sabers.com/forum/index.php?topic=49722.0)

### WT588DSoundPlayer
Plays sounds using the Wayronic's WT588D sound module.
(http://www.waytronic.com/voicemodule_show.asp?/35.html)

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
### ADXL335MotionManager
Detects motion with an ADXL335 Accelerometer

## Build Control
It is possible to streamline or control the build somewhat by editing the *USaberConfig.h* file. Some features are not cross compatable and others may lead to bloated hex file sizes. The latter may not matter for your project, but the former definately will. If you are having build issues, make sure the feature is enabled in this file or try disabiling some features that you aren't using. A best attempt is made in the comments of the file itself to track cross-compatability issues with the various features.

##Videos
Arduino Saber with ATTiny85 and WT588D

[![Arduino Saber with ATTiny85 and WT588D] (https://i.ytimg.com/vi/cuFtVQqLWX0/hqdefault.jpg?custom=true&w=336&h=188&stc=true&jpg444=true&jpgq=90&sp=68&sigh=9_fkhpRB-PIi4VJ7V15Ql1q142s)](https://www.youtube.com/watch?v=cuFtVQqLWX0)

Color Change Saber with DIYino

[![Arduino Saber with DIYino] (https://i.ytimg.com/vi/-mzwswVP-_k/hqdefault.jpg?custom=true&w=336&h=188&stc=true&jpg444=true&jpgq=90&sp=68&sigh=1gtYF6e0i89uI1B60G7QsCUM1XI)](https://www.youtube.com/watch?v=-mzwswVP-_k)
