/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 David Krepsky
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*!
 * @defgroup SnesControllerLibrary SnesControllerLibrary
 * @{
 * 
 * @brief This module groups the SNES Controller Library files.
 *
 * MSP430 SNES Controller Library
 * ==================================
 * 
 * This module contains all functions, data types and macros that are necessary to operate a SNES Controller.
 *
 * More Info
 * ---------
 * * The first souce of information for this library is the documentation provided with the source code.
 * * For more info about the SNES controller protocol, please read:
 *   [Gamefaqs Website](https: //www.gamefaqs.com/snes/916396-super-nintendo/faqs/5395 "Gamefaqs Website")
 * * Questions, suggestions or anything else, leave a comment at GitHub:
 *   [GitHub Repository](https://github.com/DKrepsky/MSP430-SNES-Controller-Library "Github Repository")
 *
 * @author David Krepsky
 * @version 1.0
 * @date 10/2016
 * @copyright David Krepsky
 *
 * @bug None known.
 */

/*----------------------------------------------------------------------------*/

/*!
 * @file snes_ctrl.h
 *
 * @brief This files contains the definitions of the SNES Controller Library.
 *
 * Defines the functions, macros and data types used by the library.
 */

#ifndef SNES_CTRL_H_
#define SNES_CTRL_H_

#include <msp430.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _MCU_CLOCK_
/*!
 *	@def _MCU_CLOCK_
 *
 * 	@brief Microcontroller main clock, in Hz.
 *
 * 	Defines the microcontroller main clock, in Hz.
 *
 * 	@warning Must be replaced in case the clock is not 1Mhz.
 */
#define _MCU_CLOCK_ 	1000000
#endif

/*!
 *	@def DELAY_12us
 *
 * 	@brief 12us delay constant.
 *
 * 	Defines the number of clock cycles to skip in order to obtain a 12us delay.
 */
#define DELAY_12us		__delay_cycles((int) 0.000012*_MCU_CLOCK_)

/*!
 *	@def DELAY_6us
 *
 * 	@brief 6us delay.
 *
 * 	Defines the number of clock cycles to skip in order to obtain a 6us delay.
 */
#define DELAY_6us		__delay_cycles((int) 0.000006*_MCU_CLOCK_)

/*!
 *	@struct SnesCtrl
 *
 *	@brief Structure to hold a controller configuration.
 *
 *	Each controller must have one instance of this structure.
 *
 *	@warning Use the snesCtrlSetup function to initialize this structure.
 */
typedef struct {
	/*! Port Direction register alias. */
	unsigned char * portDir;
	/*! Port Input register alias. */
	unsigned char * portIn;
	/*! Port Output register alias. */
	unsigned char * portOut;
	/*! Data pin (0 to 7). */
	int dataPin;
	/*! Clock pin (0 to 7). */
	int clockPin;
	/*! Latch pin (0 to 7). */
	int latchPin;
} SnesCtrl;

/*!
 *	@struct SnesCtrlBtnPressed
 *
 *	@brief Structure that holds the buttons status.
 *
 *	Each field of this structure represents a button.
 *	A true value means the button is pressed, if false, the button is not
 *	pressed.
 */
typedef struct {
	/*! B button. */
	bool b;
	/*! Y button. */
	bool y;
	/*! SELECT button. */
	bool select;
	/*! START button. */
	bool start;
	/*! UP button. */
	bool up;
	/*! DOWN button. */
	bool down;
	/*! LEFT button. */
	bool left;
	/*! RIGHT button. */
	bool right;
	/*! A button. */
	bool a;
	/*! X button. */
	bool x;
	/*! L button. */
	bool l;
	/*! R button. */
	bool r;
} SnesCtrlBtnPressed;

/*!
 * @fn SnesCtrl snesCtrlSetup(int port, int dataPin, int clkPin, int latchPin)
 *
 * @brief Initializes the control structure and the peripherals used by the
 * controller.
 *
 * This function initializes the control structure (SnesCtrl), port and pins
 * used by the controller.
 *
 * __Example__
 *    
 *     // This example shows how to configure and operate 2 gamepad, being the
 *     // first one connected to port 1, pins 1, 2 and 3 (data, clock and latch 
 *     // respectively), and the second one to port 2, pins 1, 2 and 3 (data, 
 *     // clock and latch respectively).
 *     
 *     // Create the controller structures.
 *     SnesCtrl controller1, controller2;
 *     
 *     // Configure and initialize the first gamepad.
 *     controller1 = snesCtrlSetup(1, 1, 2, 3);
 *     // Configure and initialize the second gamepad.
 *     controller2 = snesCtrlSetup(2, 1, 2, 3);
 *     
 *     // Read each gamepad B button and prints a message.
 *     SnesCtrlBtnPressed buttons1, buttons2;
 *     buttons1 = snesCtrlRead(&controller1);
 *     buttons2 = snesCtrlRead(&controller2);
 *     
 *     if(buttons1.b || buttons2.b)
 *         printf("Button B was pressed\n");
 *
 * @param[in] port Port number (only port 1 or 2 is allowed).
 * @param[in] dataPin Data line pin number (0 to 7).
 * @param[in] clkPin Clock line pin number (0 to 7).
 * @param[in] latchPin Latch line pin number (0 to 7).
 *
 * @return Returns an initialized SnesCtrl structure.
 */
SnesCtrl snesCtrlSetup(int port, int dataPin, int clkPin, int latchPin);

/*!
 * @fn SnesCtrlBtnPressed snesCtrlRead(SnesCtrl *ctrl)
 *
 * @brief Reads the controller status.
 *
 * Reads a controller status, defined by a SnesCtrl structure.
 *
 * __Example__
 *
 *     // Reads a button state.
 *     SnesCtrlBtnPressed buttons;
 *     buttons = snesCtrlRead(&ctrl);
 *     
 *     if(buttons.up)
 *         printf("Going UP into the sky\n");
 *     
 * @param[in] ctrl Pointer to the controller's control structure.
 *
 * @return Returns a structure that describes the controller status.
 */
SnesCtrlBtnPressed snesCtrlRead(SnesCtrl *ctrl);

#ifdef __cplusplus
}
#endif

#endif /* SNES_CTRL_H_ */

/*!
 * @}
 */
