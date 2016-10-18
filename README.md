The MSP430 SNES Controller Library
==================================

Overview
--------

This library purpose is to enable the operation of one or several SNES
controllers with a MSP430 microcontroller. It handles the communication
protocol with the controller's shift register, returning a structure with all
buttons states.

To create end initialize a controller, use the snesCtrlSetup() to create a
SnesCtrl instance. Each gamepad is represented by a single SnesCtrl instance.
The snesCtrlRead() function read the controler's state, from the SnesCtrl
instance, and returns a SnesCtrlBtnPressed structure instance with all
buttons states.

Installation
------------
The installation process is as simple as coping both files (snes_ctrl.h and snes_ctrl.c) inside your project and including the .h header where the routines will be used.

Build Instructions
------------------

The library should be build statically whitin your project.

It's intent is to be build with the TI MSP430 compiler, if you want to use the msp430gcc, it is necessary to replace the DELAY_12us and DELAY_6us macros inside the snes_ctrl.h file to some function/macro that will skip the right amount of time.

Electrical Connections
----------------------

The following diagram show how to wire the SNES controller to the MSP430:

                MSP430xxxx
            -----------------
        /|\|        PORT.DATA|<--10kR<--CONTROLLER DATA PIN
         | |                 |
         --|RST    PORT.CLOCK|-->10kR-->CONTROLLER CLOCK PIN
           |                 |
           |       PORT.LATCH|-->10kR-->CONTROLLER LATCH PIN
           ...................                                                 

NOTES:

 * The port can be 1 or 2, but it must be the same for all pins.
 * DATA, CLOCK and LATCH can be any pins.
 * The 10k Ohms resistor is needed to limit the current, since the SNES controller operates at 5V.

Usage Example
-------------

The code was compiled with the TI MSP430 compiler and successfully tested
with a MSP430G2452 microcontroller.

    #include "snes_ctrl.h"
    
    // Set clock speed to 2Mhz (this is only necessary if the clock frequency is
    // not 1Mhz).
    #define _MCU_CLOCK_  2000000
  
    int main(){
      // Create the controller configuration and initialize peripherals.
      // The controller is hooked to pins P2.2 (data), P2.0 (clock) and P2.1
      // (latch).
      SnesCtrl ctrl = snesCtrlSetup(2, 2, 0, 1);
  
      // Create the button structure to receive the buttons state.
      SnesCtrlBtnPressed btn;
  
      // Read the buttons state.
      btn = snesCtrlRead(&ctrl);
  
      // Do something if button B is pressed.
      if(btn.b)
        something();

      // infinite loop to hold the program.
      while(1);
    }

License
-------
The MSP430 SNES Controller Library is licensed under the MTI license. Please, see the [license](@ref license) terms for detailed info.

Copyright David Krepsky - 2016.


Acknowledgments
----------------

Many thanks to:

  + [TotoroMatt](http://totoromatt.deviantart.com) for the controller image;
  + [Jim Christy](https://www.gamefaqs.com/snes/916396-super-nintendo/faqs/5395) for the SNES controller protocol;
  + [Velron](https://github.com/Velron/doxygen-bootstrapped) for the beautiful doxygen style;
