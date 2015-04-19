# Arduino-Robot-Arm
The code is somewhat modified and brought from : letsmakerobots
Link : http://letsmakerobots.com/robot/project/micro-servo-robot

From author : 
4 axis robot arm with a small gripper and teach function. (In fact: it is a 3 axis robot)
This robot can follow. This robot can learn. And this robot can repeat endles. And he lost all his memory if the batterys are low. This makes it new all the time he is powered again. - The idea for this project is taken from http://letsmakerobots.com/node/21633 - Stoerpeak made me do it! -

Arduino mini pro 5V
4 micro servos 180degrees
4 22k potentiometer
NmH Battery 4.8V
Wires, Button, Switch

Balsa wood, Metal, Plastic
Zip Ties and Glue

a single micro servo can easily draw half an amp (well, officially its 650mA +/- 80mA). 4 would therefore require approximately 2 to 3 amps.

The USB 2.0 port of a PC is rated for half an amp only (higher depending on manufacturers implementation either shuts the port down safely, triggers a current limiting mode, trips a fuse/polyfuse or just destroys the port). Some laptops and in particular tablets will set the current limit much lower (usually with proper current limiting when the limit is set below official spec). USB 3.0 increases that limit to 1.2A. When an arduino is running from USB, its 5v bus is sourced directly from the USB power supply so the USB current limit is what you should be worried about.

When the arduino is run from batteries the 5v bus is sourced from a 5v regulator. The 5v regulator in use varies on model. On a standard arduino uno or mega it is a linear regulator in use which is rated for 900mA with a 7v input. Linear regulators shed the extra power as heat. Increasing the current through the regulator above 900mA increases the heat output, as does increasing the voltage, too much heat = bang goes the regulator. 3rd party clones, the various mini/nano/etc models and 3.3v arduinos may have differing setups.

Most of those micro servos are happiest to run between 4.8 and 6v. Just so happens to be that 4 NiMH cells in series have a nominal voltage of 4.8v, fully charged the voltage will be much closer to 6. A perfect match if you wish for a battery dedicated to your servos. Otherwise there are several switch mode regulators available which have far higher current limits than the onboard regulator. Just remember to connect the grounds of the servo power source with the arduino power source if they differ. As long as you can give them 4.8-6v and I'd say (with some back of napkin calculations) at least 3A (preferably higher) they should be happy. 4 rechargeable AAs should meet requirement just about.


Currently Developing.. 

More Will come later
