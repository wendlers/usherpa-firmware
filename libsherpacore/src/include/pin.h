/*
 * This file is part of the uSherpa project.
 *
 * Copyright (C) 2012 Stefan Wendler <sw@kaltpost.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __PIN_H_
#define __PIN_H_

#ifdef __MSP430__
#include "pin_msp430.h"
#endif

/**
 * PIN function input float
 */
#define PIN_FUNCTION_INPUT_FLOAT	0x00

/**
 * PIN function input with internal pull-up enabled
 */
#define PIN_FUNCTION_INPUT_PULLUP	0x01

/**
 * PIN function input with internal pull-down enabled
 */
#define PIN_FUNCTION_INPUT_PULLDOWN	0x02

/**
 * PIN function output 
 */
#define PIN_FUNCTION_OUTPUT			0x03

/**
 * PIN function analog input
 */
#define PIN_FUNCTION_ANALOG_IN		0x04

/**
 * PIN function PWM output
 */
#define PIN_FUNCTION_PWM			0x05

/**
 * PIN function is reserved for internal use 
 */
#define PIN_FUNCTION_RESERVED		0xFE

/**
 * PIN function is reserved for internal use 
 */
#define PIN_FUNCTION_UNKNOWN		0xFF

/**
 * PIN function EXTERNAL INTERRUPT DISABLE 
 */
#define PIN_FUNCTION_EXTI_DISABLE	0x00

/**
 * PIN function EXTERNAL INTERRUPT HIGH-LOW 
 */
#define PIN_FUNCTION_EXTI_LOWHIGH	0x01

/**
 * PIN function EXTERNAL INTERRUPT HIGH-LOW 
 */
#define PIN_FUNCTION_EXTI_HIGHLOW	0x02

/**
 * PIN has no capability 
 */
#define PIN_CAP_NONE				0b00000000

/**
 * PIN has capability input 
 */
#define PIN_CAP_INPUT				0b00000001

/**
 * PIN has capability input with internal pull-up/down resistor 
 */
#define PIN_CAP_INPUT_RE			0b00000010

/**
 * PIN has capability output 
 */
#define PIN_CAP_OUTPUT				0b00000100

/**
 * PIN has capability analog input 
 */
#define PIN_CAP_ANALOG_IN			0b00001000

/**
 * PIN has capability PWM output 
 */
#define PIN_CAP_PWM					0b00010000


/**
 * PIN status was OK
 */
#define PIN_STAT_OK					0

/**
 * PIN status was unsupported function 
 */
#define PIN_STAT_ERR_UNSUPFUNC		-1

/**
 * PIN status was invalid port 
 */
#define PIN_STAT_ERR_INVALPORT		-2

/**
 * PIN status was invalid pin 
 */
#define PIN_STAT_ERR_INVALPIN		-3

/**
 * Mark the given PIN as reserved. A reserved PIN is dedicated to 
 * internal use only, and thus is not usable from outside. 
 *
 * @param[in]	pin		PIN to reserve 
 */
void pin_reserve(unsigned char pin);

/**
 * Get the capabilites for a PIN as defined with the PIN_CAP_x constants.
 *
 * @param[in]	pin		PIN for which to get capablities
 * @return				PIN capablities as defined with PIN_CAP_x
 */
int pin_capabilities(unsigned char pin); 

/**
 * Check if a given PIN has given capabilities.
 *
 * @param[in]	pin					PIN for which to check capablities
 * @param[in]	capabilities		capablities to check for as defined in PIN_CAP_x
 * @return							1 if PIN has capabilities, 0 otherwise	
 */
int pin_has_capabilities(unsigned char pin, int capabilities); 

/**
 * Get the function for which a given PIN is currently configured.
 *
 * @param[in]	pin		PIN for which to get the current function
 * @return 				current PIN function as defined with PIN_FUNCTION_x 
 */
unsigned char pin_function(unsigned char pin); 

/**
 * Check if there is already a PIN configured with the given function. 
 *
 * @param[in]	pin			PIN after which to start checking for the next PIN with this function
 * @param[in]	function	function to find a PIN for
 * @return 					first PIN found with given function or 0 if no PIN with this function was found
 */
unsigned char pin_with_function(unsigned char pin, unsigned char function); 

/** 
 * Setup up a PIN for a certain function.
 *
 * @param[in]	pin			PIN to setup 
 * @param[in]	function	function to assign to the PIN
 * @return					PIN_STAT_OK if function assigned
 * 							PIN_STAT_ERR_UNSUPFUNC if PIN does not support this function	
 * 							PIN_STAT_ERR_UNKPORT if given port for PIN is not known
 * 							PIN_STAT_ERR_UNKPIN  if given pin nr. for PIN is not known
 */
int pin_setup(unsigned char pin, unsigned char function);

/**
 * Clear the output for a given PIN
 *
 * @param[in]	pin			PIN to perform this action for 
 * @return					PIN_STAT_OK if function assigned
 * 							PIN_STAT_ERR_UNSUPFUNC if PIN does not support this function 
 * 							(since it is not configured as OUTPUT)	
 * 							PIN_STAT_ERR_UNKPORT if given port for PIN is not known
 * 							PIN_STAT_ERR_UNKPIN  if given pin nr. for PIN is not known
 */
int pin_clear(unsigned char pin);

/**
 * Set the output for a given PIN
 *
 * @param[in]	pin			PIN to perform this action for 
 * @return					PIN_STAT_OK if function assigned
 * 							PIN_STAT_ERR_UNSUPFUNC if PIN does not support this function 
 * 							(since it is not configured as OUTPUT)	
 * 							PIN_STAT_ERR_UNKPORT if given port for PIN is not known
 * 							PIN_STAT_ERR_UNKPIN  if given pin nr. for PIN is not known
 */
int pin_set(unsigned char pin);

/**
 * Toggle the output for a given PIN
 *
 * @param[in]	pin			PIN to perform this action for 
 * @return					PIN_STAT_OK if function assigned
 * 							PIN_STAT_ERR_UNSUPFUNC if PIN does not support this function 
 * 							(since it is not configured as OUTPUT)	
 * 							PIN_STAT_ERR_UNKPORT if given port for PIN is not known
 * 							PIN_STAT_ERR_UNKPIN  if given pin nr. for PIN is not known
 */
int pin_toggle(unsigned char pin);

/**
 * Read the digital state of an input/output for a given PIN
 *
 * @param[in]	pin			PIN to perform this action for 
 * @return					PIN_STAT_OK if function assigned
 * 							PIN_STAT_ERR_UNSUPFUNC if PIN does not support this function 
 * 							(since it is not configured as INPUT/OUTPUT/PWM)	
 * 							PIN_STAT_ERR_UNKPORT if given port for PIN is not known
 * 							PIN_STAT_ERR_UNKPIN  if given pin nr. for PIN is not known
 */
int pin_digital_read(unsigned char pin);

/**
 * Read the analog state of an input/output for a given PIN
 *
 * @param[in]	pin			PIN to perform this action for 
 * @return				    analog value sampled on success (positive value)	
 * 							PIN_STAT_ERR_UNSUPFUNC if PIN does not support this function 
 * 							(since it is not configured as ANALOG INPUT)	
 * 							PIN_STAT_ERR_UNKPORT if given port for PIN is not known
 * 							PIN_STAT_ERR_UNKPIN  if given pin nr. for PIN is not known
 */
int pin_analog_read(unsigned char pin);

/**
 * Read the digital pulselengh for a given PIN. If PIN is LOW when function is called,
 * the function waits for next raising edge, then measures the time until a falling
 * edge is detected. If PIN is HIGH when functino is called, the function waits for next
 * falling edge, then measures the time until a rising edge is detexted. 
 *
 * @param[in]	pin			PIN to perform this action for 
 * @return				    pulse lenght in usec on succes (positive value)	
 * 							PIN_STAT_ERR_UNSUPFUNC if PIN does not support this function 
 * 							(since it is not configured as INPUT)	
 * 							PIN_STAT_ERR_UNKPORT if given port for PIN is not known
 * 							PIN_STAT_ERR_UNKPIN  if given pin nr. for PIN is not known
 */
int pin_pulselength_read(unsigned char pin);

/**
 * Read the digital pulselengh for a given PIN. The function for the given PIN is
 * changed first to OUTPUT, then the PIN is driven HIGH for at least 10us (e.g. 
 * to charge a condensator of a range finder), then the PIN function is restored 
 * to whatever INPUT it was before, and the function waits for next raising edge, 
 * then measures the time until a falling edge is detected. 
 *
 * @param[in]	pin			PIN to perform this action for 
 * @return				    pulse lenght in usec on succes (positive value)	
 * 							PIN_STAT_ERR_UNSUPFUNC if PIN does not support this function 
 * 							(since it is not configured as INPUT)	
 * 							PIN_STAT_ERR_UNKPORT if given port for PIN is not known
 * 							PIN_STAT_ERR_UNKPIN  if given pin nr. for PIN is not known
 */
int pin_pulselength_read_dhf(unsigned char pin);

/**
 * Set the period for a PIN configured for PWM. 
 *
 * @param[in]	pin			PIN to perform this action for 
 * @param[in]	period		the period in usec. 
 * @return				    pulse lenght in ms on succes (positive value)	
 * 							PIN_STAT_ERR_UNSUPFUNC if PIN does not support this function 
 * 							(since it is not configured as INPUT)	
 * 							PIN_STAT_ERR_UNKPORT if given port for PIN is not known
 * 							PIN_STAT_ERR_UNKPIN  if given pin nr. for PIN is not known
 */
int pin_pwm_function(unsigned char pin, int period);

/**
 * Set the duty-cycle for a PIN configured for PWM. The duty-cycle is given in percent.
 * A value of 0xFF equals 100%.  
 *
 * @param[in]	pin			PIN to perform this action for 
 * @param[in]	duty_cycle	duty-cycle in percent (0xFF = 100%) 
 * @return				    pulse lenght in ms on succes (positive value)	
 * 							PIN_STAT_ERR_UNSUPFUNC if PIN does not support this function 
 * 							(since it is not configured as INPUT)	
 * 							PIN_STAT_ERR_UNKPORT if given port for PIN is not known
 * 							PIN_STAT_ERR_UNKPIN  if given pin nr. for PIN is not known
 */
int pin_pwm_control(unsigned char pin, unsigned char duty_cycle);

/**
 * Enable the external interrupt function for a given pin. It could be specified when
 * an interrupt is triggered (never=PWM_FUNCTION_EXTI_DISABLE, low-high-edge=PWM_FUNCTION_EXIT_LOWHIGH,
 * high-low-edge=PWM_FUNCTION_EXTI_HIGHLOW).
 *
 * @param[in]	pin			PIN to perform this action for 
 * @return				    pulse lenght in ms on succes (positive value)	
 * 							PIN_STAT_ERR_UNSUPFUNC if PIN does not support this function 
 * 							(since it is not configured as INPUT)	
 * 							PIN_STAT_ERR_UNKPORT if given port for PIN is not known
 * 							PIN_STAT_ERR_UNKPIN  if given pin nr. for PIN is not known
 */
int pin_exti_function(unsigned char pin, unsigned char function);

#endif
