# mBot-Legs
This is for the mBot legs product. It includes some tips, an assembly guide, and some sample code for training the robot to walk and actually making it walk.

**mBot Legs Assembly Guide**
This is the assembly guide for connecting the mBot legs and the mBots together.

**AIGeneticWalkingTest**
This is Arduino code for training the robot to walk. Some things about it.
_Pin Connections_
The ultrasonic sensor connects to Junction 2. If you want to use another junction, then you have to change the variable "sigR" in the code; sigR is the digital pin that the sensor connects to. You can reference the first page of the **Makeblock mBot** pdf in the main branch, in order to determine which pin to set sigR to.

_Uploading Training Code_
You must upload this code to both robots. Change the boolean variable "compass" in the code, so that one robot has compass = true and the other compass = false. This determines the direction of the robot's walk. Once you have uploaded code to one robot, you must switch it off in less than 20 seconds after the "Done Uploading" message appears. 

_Actual Training_
Make sure that the ultrasonic sensors are facing two flat surfaces. It would be preferable if the robot were trained in a thin box, so that it cannot veer to one side and escape training. It is also helpful to have the robot be trained on a hard, flat, stable surface, such as a drawer as opposed to a movable box. In order to begin training, you must switch on both switches of the robots simultaneously. 


_EEPROM_
EEPROM is a special type of memory which stays even when you switch the mBot off. This means that if you write a float to EEPROM and then the robot powers off, you can power it back on and read the EEPROM value. Two things about EEPROM:
  1. You need to add the EEPROM library into your Arduino environment if it is not already there. You can do this by downloading the library from GitHub, or potentially by clicking on the "Tools" tab at the top of your Arduino IDE and clicking "Manage Libraries".
  2. There is something called EEPROM wearing, which is where, if you write a value to an EEPROM address too many times, it will eventually deteriorate and not work as well. The lifetime of EEPROM depends, such as on the temperature even, but will usually last at least 100,000 writes. By scrolling to the bottom of the code where EEPROM is written to, you can modify it to reduce this. For example, you can add some code so that it will only write once all the generations are over. You can also implement something called an EEPROM wear-levelling algorithm, where you spread out the writing among many EEPROM addresses.

**GeneticAITestingTryIt**
This is the code once the robot has been trained. Not much. There are some variables on top which, if you decide to edit, should be kept consistent with the **AIGeneticWalkingTest** code.
