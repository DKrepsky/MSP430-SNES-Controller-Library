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
 * @addtogroup SnesControllerLibrary
 * @{
 */

/*----------------------------------------------------------------------------*/

/*!
 * @file snes_ctrl.c
 *
 * @brief This files implements the SNES Controller Library.
 *
 * Implementation of all functions used by the SNES Controller Library.
 */
#include "snes_ctrl.h"

// Internal function that triggers the latch.
void _snesCtrlLatchPulse(SnesCtrl *ctrl) {
	// Trigger the latch for 12us.
	*(ctrl->portOut) |= ctrl->latchPin;
	DELAY_12us;
	*(ctrl->portOut) &= ~ctrl->latchPin;
}

// Internal function that reads one button status from the controller.
bool _snesBitRead(SnesCtrl *ctrl) {
	// Wait until half clock cycle.
	DELAY_6us;

	bool status = false;

	// Read the data pin state.
	// 1 mean the button is not pressed and 0 means the button is pressed.
	status = ((*(ctrl->portIn) & ctrl->dataPin) > 0) ? false : true;

	// 6us clock pulse.
	*(ctrl->portOut) &= ~ctrl->clockPin;
	DELAY_6us;
	*(ctrl->portOut) |= ctrl->clockPin;

	return status;
}

SnesCtrl snesCtrlSetup(int port, int dataPin, int clkPin, int latchPin) {
	SnesCtrl ctrl;

	// Select port 1 or 2 only. If another port is used, this function needs to
	// be extended.
	switch (port) {
	case 1:
		ctrl.portDir = (unsigned char *) &(P1DIR);
		ctrl.portOut = (unsigned char *) &(P1OUT);
		ctrl.portIn = (unsigned char *) &(P1IN);
		break;
	case 2:
		ctrl.portDir = (unsigned char *) &(P2DIR);
		ctrl.portOut = (unsigned char *) &(P2OUT);
		ctrl.portIn = (unsigned char *) &(P2IN);
		break;
	default:
		ctrl.portDir = 0;
		ctrl.portOut = 0;
		ctrl.portIn = 0;
	}

	// Map from integer to bit mask.
	ctrl.dataPin = 1 << dataPin;
	ctrl.clockPin = 1 << clkPin;
	ctrl.latchPin = 1 << latchPin;

	// Setup clock and latch initial state.
	// Clock is normally high.
	*(ctrl.portOut) |= ctrl.clockPin;
	// Latch is normally low.
	*(ctrl.portOut) &= ~ctrl.latchPin;

	// Configure data, latch and clock pins as input, output and output,
	// respectively.
	*(ctrl.portDir) &= ~ctrl.dataPin;
	*(ctrl.portDir) |= ctrl.latchPin + ctrl.clockPin;

	// Waits a little bit until thing set up.
	DELAY_12us;

	return ctrl;
}

SnesCtrlBtnPressed snesCtrlRead(SnesCtrl *ctrl) {

	// Pulses the latch.
	_snesCtrlLatchPulse(ctrl);

	SnesCtrlBtnPressed buttons;

	// Read buttons status.
	buttons.b = _snesBitRead(ctrl);
	buttons.y = _snesBitRead(ctrl);
	buttons.select = _snesBitRead(ctrl);
	buttons.start = _snesBitRead(ctrl);
	buttons.up = _snesBitRead(ctrl);
	buttons.down = _snesBitRead(ctrl);
	buttons.left = _snesBitRead(ctrl);
	buttons.right = _snesBitRead(ctrl);
	buttons.a = _snesBitRead(ctrl);
	buttons.x = _snesBitRead(ctrl);
	buttons.l = _snesBitRead(ctrl);
	buttons.r = _snesBitRead(ctrl);

	return buttons;
}

/*!
 * @}
 */
