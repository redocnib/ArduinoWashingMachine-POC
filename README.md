# FullAutomaticWashingMachine
The Story : i have a fullyautomatic washing machine @ my home (Samsung WA65H4200HA) whose control board recently got burned, so i decided to make it back using arduino.

/*
 * Author : Kiran Abraham Korah
 * beep.exe@gmail.com
 * http://bincoder.com/
 * https://github.com/redocnib
 * Fully Automatic Washing Machine Logic Board 
 * Note: 
 * Added 16x2 display
 * The pressure switch is broken so i am using timers for drain and fill.
 * 
 * todo:
 * 1. Barometer and pressure sensing for drain and fill
 * 2. Control switches
 * 3. Mobile/Network based alerts and notifications
 * 
 */
 
<pre>

Note
-----
I use an inverted relay module, so RELAY_ON=LOW and RELAY_OFF=HIGH, swap them according to the type.
2,3,4,5 are the ports used, 
2=MotorPowerPort=Main Supply to the Mototr
</pre>
<pre>


Wash Process
----------------------------------------------------------------------------------------

                                            to Motor Coil 1<---- |         |
           |         | >---------------------------Out---------> | Relay 2 | ------- [3]
[2] -------| Relay 1 | <-Mains In--0        to Motor Coil 1<---- |         |
           |         |        

----------------------------------------------------------------------------------------

</pre>
#[2],[3] are arduino ports in which [2] cuts off power for a short period of time using |relay 1|, while [3] switches the direction of #the motor using |relay 2|.

doWash(cycle length,total wash time in minutes)
-----------------------------------------------
The doWash method switches the motor direction and a let it operate in each direction for a period (cycle length), 
and for a total duration of (total wash time).

fillWater(time in minutes)
-----------------------------------------------
As i mentioned before, currently water filling is set based on a time, see how much time your washing machine take to fillup the water and set it to (time in minutes).

drainWater(time in minutes)
-----------------------------------------------
As i mentioned before, currently water draining is set based on a time, see how much time your washing machine take to drasin the water and set it to (time in minutes).

doSpin(time in minutes,Direction);
-----------------------------------------------
For spin, we need to open the water draining valve, (my machine uses a gear so i set a 3 second delay) and then start rotating the whole drum in one direction (as set to Direction).

doSoak(time in minutes);
-----------------------------------------------
The process will wait for a specific period of time so that the clothes can soak.


Tools
---------------------
i use UnoArduSimV1.7.2 (https://www.sites.google.com/site/unoardusim/home) for simulating arduino before field test ;) 
