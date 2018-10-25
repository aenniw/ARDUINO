// This file is part of the PinChangeInt library for the Arduino.  This library will work on any ATmega328-based
// or ATmega2560-based Arduino, as well as the Sanguino or Mioduino.

// Most of the pins of an Arduino Uno use Pin Change Interrupts, and because of the way the ATmega interrupt
// system is designed it is difficult to trigger an Interrupt Service Request off of any single pin, and on
// any change of state (either rising, or falling, or both).  The goal of this library is to make it easy for
// the programmer to attach an ISR so it will trigger on any change of state on any Pin Change Interrupt pin.

// (NOTE TO SELF: Update the PCINT_VERSION define, below) -----------------
#define PCINT_VERSION 2402
/*
Copyright 2008 Chris J. Kiick
Copyright 2009-2011 Lex Talionis
Copyright 2010-2014 Michael Schwager (aka, "GreyGnome")

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/*
 * QUICKSTART
 *
 * For the beginners/lazy/busy/wreckless:
 * To attach an interrupt to your Arduino Pin, calling your function "userFunc", and acting on
 * the "mode", which is a change in the pin's state; either RISING or FALLING or CHANGE:
 *		attachPinChangeInterrupt(pin,userFunc,mode)
 * Your function must not return any values and it cannot take any arguments (that is, its definition
 * has to look like this:
 *		void userFunc() {
 *		  ...your code here...
 *		}
 *
 * That's it. Everything else are details.
 *
 * If you need to exchange information to/from the interrupt, you can use global volatile variables.
 * See the example for more information.
 *
 *	You probably will not need to do this, but later in your sketch you can detach the interrupt:
 *		detachPinChangeInterrupt(pin)
 *
 *	If you want to see what the *last* pin that triggered an interrupt was, you can get it this way:
 *		getInterruptedPin()
 *	Note: If you have multiple pins that are triggering interrupts and they are sufficiently fast,
 *	you will not be able to find all the pins that interrupted.
*/

//
// For the beginners
//
#define	detachPinChangeInterrupt(pin)				PCintPort::detachInterrupt(pin)
#define	attachPinChangeInterrupt(pin,userFunc,mode)	PCintPort::attachInterrupt(pin, &userFunc,mode)
#define getInterruptedPin()							PCintPort::getArduinoPin()

// We use 4-character tabstops, so IN VIM:  <esc>:set ts=4 sw=4 sts=4
// ...that's: ESCAPE key, colon key, then
//		"s-e-t SPACE key t-s = 4 SPACE key s-w = 4 SPACE key s-t-s = 4"

/*
 * 	This is the PinChangeInt library for the Arduino.
	This library provides an extension to the interrupt support for arduino by adding pin change
	interrupts, giving a way for users to have interrupts drive off of any pin (ATmega328-based
	Arduinos) and by the Port B, J, and K pins on the Arduino Mega and its ilk (see the README file).

	See the README for license, acknowledgments, and other details (especially concerning the Arduino MEGA).

	See google code project for latest, bugs and info http://code.google.com/p/arduino-pinchangeint/
	See github for the bleeding edge code: https://github.com/GreyGnome/PinChangeInt
	For more information Refer to avr-gcc header files, arduino source and atmega datasheet.

	This library was inspired by and derived from Chris J. Kiick's PCInt Arduino Playground
	example here: http://www.arduino.cc/playground/Main/PcInt
	Nice job, Chris!
*/

//-------- define these in your sketch, if applicable ----------------------------------------------------------
//-------- These must go in your sketch ahead of the #include <PinChangeInt.h> statement -----------------------
// You can reduce the memory footprint of this handler by declaring that there will be no pin change interrupts
// on any one or two of the three ports.  If only a single port remains, the handler will be declared inline
// reducing the size and latency of the handler.
// #define NO_PORTB_PINCHANGES // to indicate that port b will not be used for pin change interrupts
// #define NO_PORTC_PINCHANGES // to indicate that port c will not be used for pin change interrupts
// #define NO_PORTD_PINCHANGES // to indicate that port d will not be used for pin change interrupts
// --- Mega support ---
// #define NO_PORTB_PINCHANGES // to indicate that port b will not be used for pin change interrupts
// #define NO_PORTJ_PINCHANGES // to indicate that port j will not be used for pin change interrupts
// #define NO_PORTK_PINCHANGES // to indicate that port k will not be used for pin change interrupts
// In the Mega, there is no Port C, no Port D.  Instead, you get Port J and Port K.  Port B remains.
// Port J, however, is practically useless because there is only 1 pin available for interrupts.  Most
// of the Port J pins are not even connected to a header connection.  // </end> "Mega Support" notes
// --- Sanguino, Mioduino support ---
// #define NO_PORTA_PINCHANGES // to indicate that port a will not be used for pin change interrupts

// You can reduce the code size by 20-50 bytes, and you can speed up the interrupt routine
// slightly by declaring that you don't care if the static variables PCintPort::pinState and/or
// PCintPort::arduinoPin are set and made available to your interrupt routine.
// #define NO_PIN_STATE        // to indicate that you don't need the pinState
// #define NO_PIN_NUMBER       // to indicate that you don't need the arduinoPin
// #define DISABLE_PCINT_MULTI_SERVICE // to limit the handler to servicing a single interrupt per invocation.
// #define GET_PCINT_VERSION   // to enable the uint16_t getPCIintVersion () function.
// The following is intended for testing purposes.  If defined, then a whole host of static variables can be read
// in your interrupt subroutine.  It is not defined by default, and you DO NOT want to define this in
// Production code!:
// #define PINMODE
//-------- define the above in your sketch, if applicable ------------------------------------------------------

/*
	VERSIONS found in moved to RELEASE_NOTES.

	See the README file for the License and more details.
*/

#ifndef PinChangeInt_h
#define	PinChangeInt_h

#include "stddef.h"

// Maurice Beelen, nms277, Akesson Karlpetter, and Orly Andico
// sent in fixes to work with Arduino >= version 1.0
#include <Arduino.h>
#include <new.h>
#include <wiring_private.h> // cbi and sbi defined here

#undef DEBUG

/*
* Theory: For the IO pins covered by Pin Change Interrupts
* (== all of them on the Atmega168/328, and a subset on the Atmega2560),
* the PCINT corresponding to the pin must be enabled and masked, and
* an ISR routine provided.  Since PCINTs are per port, not per pin, the ISR
* must use some logic to actually implement a per-pin interrupt service.
*/

/* Pin to interrupt map, ATmega328:
* D0-D7 = PCINT 16-23 = PCIR2 = PD = PCIE2 = pcmsk2
* D8-D13 = PCINT 0-5 = PCIR0 = PB = PCIE0 = pcmsk0
* A0-A5 (D14-D19) = PCINT 8-13 = PCIR1 = PC = PCIE1 = pcmsk1
*/

#undef	INLINE_PCINT
#define INLINE_PCINT
// Thanks to cserveny...@gmail.com for MEGA support!
#if defined __AVR_ATmega2560__ || defined __AVR_ATmega1280__ || defined __AVR_ATmega1281__ || defined __AVR_ATmega2561__ || defined __AVR_ATmega640__
	#define __USE_PORT_JK
	// Mega does not have PORTA, C or D
	#define NO_PORTA_PINCHANGES
	#define NO_PORTC_PINCHANGES
	#define NO_PORTD_PINCHANGES
	#if ((defined(NO_PORTB_PINCHANGES) && defined(NO_PORTJ_PINCHANGES)) || \
			(defined(NO_PORTJ_PINCHANGES) && defined(NO_PORTK_PINCHANGES)) || \
			(defined(NO_PORTK_PINCHANGES) && defined(NO_PORTB_PINCHANGES)))
		#define	INLINE_PCINT inline
	#endif
#else
	#define NO_PORTJ_PINCHANGES
	#define NO_PORTK_PINCHANGES
	#if defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644__)
		#ifndef NO_PORTA_PINCHANGES
			#define __USE_PORT_A
		#endif
	#else
		#define NO_PORTA_PINCHANGES
	#endif
	// if defined only D .OR. only C .OR. only B .OR. only A, then inline it
	#if (   (defined(NO_PORTA_PINCHANGES) && defined(NO_PORTB_PINCHANGES) && defined(NO_PORTC_PINCHANGES)) || \
			(defined(NO_PORTA_PINCHANGES) && defined(NO_PORTB_PINCHANGES) && defined(NO_PORTD_PINCHANGES)) || \
			(defined(NO_PORTA_PINCHANGES) && defined(NO_PORTC_PINCHANGES) && defined(NO_PORTD_PINCHANGES)) || \
			(defined(NO_PORTB_PINCHANGES) && defined(NO_PORTC_PINCHANGES) && defined(NO_PORTD_PINCHANGES)) )
		#define	INLINE_PCINT inline
	#endif
#endif

// Provide drop in compatibility with Chris J. Kiick's PCInt project at
// http://www.arduino.cc/playground/Main/PcInt
#define	PCdetachInterrupt(pin)	PCintPort::detachInterrupt(pin)
#define	PCattachInterrupt(pin,userFunc,mode) PCintPort::attachInterrupt(pin, userFunc,mode)
#define PCgetArduinoPin() PCintPort::getArduinoPin()

typedef void (*PCIntvoidFuncPtr)(void);

class PCintPort {
public:
	// portB=PCintPort(2, 1,PCMSK1);
	// index:   portInputReg(*portInputRegister(index)), 
	// pcindex: PCICRbit(1 << pcindex)
	// maskReg: portPCMask(maskReg)
	PCintPort(int index,int pcindex, volatile uint8_t& maskReg) :
	portInputReg(*portInputRegister(index)),
	portPCMask(maskReg),
	PCICRbit(1 << pcindex),
	portRisingPins(0),
	portFallingPins(0),
	firstPin(NULL)
#ifdef PINMODE
	,intrCount(0)
#endif
	{
		#ifdef FLASH
		ledsetup();
		#endif
	}
	volatile	uint8_t&		portInputReg;
	static		int8_t attachInterrupt(uint8_t pin, PCIntvoidFuncPtr userFunc, int mode);
	static		void detachInterrupt(uint8_t pin);
	INLINE_PCINT void PCint();
	static volatile uint8_t curr;
	#ifndef NO_PIN_NUMBER
	static	volatile uint8_t	arduinoPin;
	#endif
	#ifndef NO_PIN_STATE
	static volatile	uint8_t	pinState;
	#endif
	#ifdef PINMODE
	static volatile uint8_t pinmode;
	static volatile uint8_t s_portRisingPins;
	static volatile uint8_t s_portFallingPins;
	static volatile uint8_t s_lastPinView;
	static volatile uint8_t s_pmask;
	static volatile char s_PORT;
	static volatile uint8_t s_changedPins;
	static volatile uint8_t s_portRisingPins_nCurr;
	static volatile uint8_t s_portFallingPins_nNCurr;
	static volatile uint8_t s_currXORlastPinView;
	volatile uint8_t intrCount;
	static volatile uint8_t s_count;
	static volatile uint8_t pcint_multi;
	static volatile uint8_t PCIFRbug;
	#endif
	#ifdef FLASH
	static void ledsetup(void);
	#endif

protected:
	class PCintPin {
	public:
		PCintPin() :
		PCintFunc((PCIntvoidFuncPtr)NULL),
		mode(0) {}
		PCIntvoidFuncPtr PCintFunc;
		uint8_t 	mode;
		uint8_t		mask;
		uint8_t arduinoPin;
		PCintPin* next;
	};
	void 		enable(PCintPin* pin, PCIntvoidFuncPtr userFunc, uint8_t mode);
	int8_t		addPin(uint8_t arduinoPin,PCIntvoidFuncPtr userFunc, uint8_t mode);
	volatile	uint8_t&		portPCMask;
	const		uint8_t			PCICRbit;
	volatile	uint8_t			portRisingPins;
	volatile	uint8_t			portFallingPins;
	volatile uint8_t		lastPinView;
	PCintPin*	firstPin;
};

#ifndef LIBCALL_PINCHANGEINT // LIBCALL_PINCHANGEINT ***********************************************
volatile uint8_t PCintPort::curr=0;
#ifndef NO_PIN_NUMBER
volatile uint8_t PCintPort::arduinoPin=0;
#endif
#ifndef NO_PIN_STATE
volatile uint8_t PCintPort::pinState=0;
#endif
#ifdef PINMODE
volatile uint8_t PCintPort::pinmode=0;
volatile uint8_t PCintPort::s_portRisingPins=0;
volatile uint8_t PCintPort::s_portFallingPins=0;
volatile uint8_t PCintPort::s_lastPinView=0;
volatile uint8_t PCintPort::s_pmask=0;
volatile char	 PCintPort::s_PORT='x';
volatile uint8_t PCintPort::s_changedPins=0;
volatile uint8_t PCintPort::s_portRisingPins_nCurr=0;
volatile uint8_t PCintPort::s_portFallingPins_nNCurr=0;
volatile uint8_t PCintPort::s_currXORlastPinView=0;
volatile uint8_t PCintPort::s_count=0;
volatile uint8_t PCintPort::pcint_multi=0;
volatile uint8_t PCintPort::PCIFRbug=0;
#endif

#ifdef FLASH
#define PINLED 13
volatile uint8_t *led_port;
uint8_t led_mask;
uint8_t not_led_mask;
boolean ledsetup_run=false;
void PCintPort::ledsetup(void) {
	if (! ledsetup_run) {
		led_port=portOutputRegister(digitalPinToPort(PINLED));
		led_mask=digitalPinToBitMask(PINLED);
		not_led_mask=led_mask^0xFF;
		pinMode(PINLED, OUTPUT); digitalWrite(PINLED, LOW);
		ledsetup_run=true;
	}
};
#endif

//
// ATMEGA 644 
//
#if defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644__) // Sanguino, Mosquino uino bobino bonanafannafofino, me my momino...

#ifndef NO_PORTA_PINCHANGES
PCintPort portA=PCintPort(1, 0,PCMSK0); // port PA==1  (from Arduino.h, Arduino version 1.0)
#endif
#ifndef NO_PORTB_PINCHANGES
PCintPort portB=PCintPort(2, 1,PCMSK1); // port PB==2  (from Arduino.h, Arduino version 1.0)
#endif
#ifndef NO_PORTC_PINCHANGES
PCintPort portC=PCintPort(3, 2,PCMSK2); // port PC==3  (also in pins_arduino.c, Arduino version 022)
#endif
#ifndef NO_PORTD_PINCHANGES
PCintPort portD=PCintPort(4, 3,PCMSK3); // port PD==4
#endif

#else // others

#ifndef NO_PORTB_PINCHANGES
PCintPort portB=PCintPort(2, 0,PCMSK0); // port PB==2  (from Arduino.h, Arduino version 1.0)
#endif
#ifndef NO_PORTC_PINCHANGES  // note: no PORTC on MEGA
PCintPort portC=PCintPort(3, 1,PCMSK1); // port PC==3  (also in pins_arduino.c, Arduino version 022)
#endif
#ifndef NO_PORTD_PINCHANGES  // note: no PORTD on MEGA
PCintPort portD=PCintPort(4, 2,PCMSK2); // port PD==4
#endif

#endif // defined __AVR_ATmega644__

#ifdef __USE_PORT_JK
#ifndef NO_PORTJ_PINCHANGES
PCintPort portJ=PCintPort(10,1,PCMSK1); // port PJ==10 
#endif
#ifndef NO_PORTK_PINCHANGES
PCintPort portK=PCintPort(11,2,PCMSK2); // port PK==11
#endif
#endif // USE_PORT_JK

static PCintPort *lookupPortNumToPort( int portNum ) {
    PCintPort *port = NULL;

	switch (portNum) {
#ifndef NO_PORTA_PINCHANGES
	case 1:
		port=&portA;
		break;
#endif
#ifndef NO_PORTB_PINCHANGES
	case 2:
		port=&portB;
		break;
#endif
#ifndef NO_PORTC_PINCHANGES
	case 3:
		port=&portC;
		break;
#endif
#ifndef NO_PORTD_PINCHANGES
	case 4:
		port=&portD;
		break;
#endif
#ifdef __USE_PORT_JK

#ifndef NO_PORTJ_PINCHANGES
	case 10:
		port=&portJ;
		break;
#endif

#ifndef NO_PORTK_PINCHANGES
	case 11:
		port=&portK;
		break;
#endif

#endif // __USE_PORT_JK
    }

    return port;
}


void PCintPort::enable(PCintPin* p, PCIntvoidFuncPtr userFunc, uint8_t mode) {
	// Enable the pin for interrupts by adding to the PCMSKx register.
	// ...The final steps; at this point the interrupt is enabled on this pin.
	p->mode=mode;
	p->PCintFunc=userFunc;
#ifndef NO_PORTJ_PINCHANGES
	// A big shout out to jrhelbert for this fix! Thanks!!!
	if ((p->arduinoPin == 14) || (p->arduinoPin == 15)) {
		portPCMask |= (p->mask << 1); // PORTJ's PCMSK1 is a little odd...
	}
	else {
		portPCMask |= p->mask;
	}
#else
    portPCMask |= p->mask;
#endif
	if ((p->mode == RISING) || (p->mode == CHANGE)) portRisingPins |= p->mask;
	if ((p->mode == FALLING) || (p->mode == CHANGE)) portFallingPins |= p->mask;
	PCICR |= PCICRbit;
}

int8_t PCintPort::addPin(uint8_t arduinoPin, PCIntvoidFuncPtr userFunc, uint8_t mode)
{
	PCintPin* tmp;

	tmp=firstPin;
	// Add to linked list, starting with firstPin. If pin already exists, just enable.
	if (firstPin != NULL) {
		do {
			if (tmp->arduinoPin == arduinoPin) { enable(tmp, userFunc, mode); return(0); }
			if (tmp->next == NULL) break;
			tmp=tmp->next;
		} while (true);
	}

	// Create pin p:  fill in the data.
	PCintPin* p=new PCintPin;
	if (p == NULL) return(-1);
	p->arduinoPin=arduinoPin;
	p->mode = mode;
	p->next=NULL;
	p->mask = digitalPinToBitMask(arduinoPin); // the mask

	if (firstPin == NULL) firstPin=p;
	else tmp->next=p; // NOTE that tmp cannot be NULL.

#ifdef DEBUG
	Serial.print("addPin. pin given: "); Serial.print(arduinoPin, DEC);
	int addr = (int) p;
	Serial.print(" instance addr: "); Serial.println(addr, HEX);
	Serial.print("userFunc addr: "); Serial.println((int)p->PCintFunc, HEX);
#endif

	enable(p, userFunc, mode);
#ifdef DEBUG
	Serial.print("addPin. pin given: "); Serial.print(arduinoPin, DEC), Serial.print (" pin stored: ");
	int addr = (int) p;
	Serial.print(" instance addr: "); Serial.println(addr, HEX);
#endif
	return(1);
}

/*
 * attach an interrupt to a specific pin using pin change interrupts.
 */
int8_t PCintPort::attachInterrupt(uint8_t arduinoPin, PCIntvoidFuncPtr userFunc, int mode)
{
	PCintPort *port;
	uint8_t portNum = digitalPinToPort(arduinoPin);
	if ((portNum == NOT_A_PORT) || (userFunc == NULL)) return(-1);

	port=lookupPortNumToPort(portNum);
	// Added by GreyGnome... must set the initial value of lastPinView for it to be correct on the 1st interrupt.
	// ...but even then, how do you define "correct"?  Ultimately, the user must specify (not provisioned for yet).
	port->lastPinView=port->portInputReg;
#ifdef DEBUG
	Serial.print("attachInterrupt- pin: "); Serial.println(arduinoPin, DEC);
#endif
	// map pin to PCIR register
	return(port->addPin(arduinoPin,userFunc,mode));
}

void PCintPort::detachInterrupt(uint8_t arduinoPin)
{
	PCintPort *port;
	PCintPin* current;
	uint8_t mask;
	uint8_t portNum = digitalPinToPort(arduinoPin);
	if (portNum == NOT_A_PORT) return;
	port=lookupPortNumToPort(portNum);
	mask=digitalPinToBitMask(arduinoPin);
	current=port->firstPin;
	while (current) {
		if (current->mask == mask) { // found the target
			uint8_t oldSREG = SREG;
			cli(); // disable interrupts
#ifndef NO_PORTJ_PINCHANGES
			// A big shout out to jrhelbert for this fix! Thanks!!!
			if ((arduinoPin == 14) || (arduinoPin == 15)) {
				port->portPCMask &= ~(mask << 1); // PORTJ's PCMSK1 is a little odd...
			}
			else {
				port->portPCMask &= ~mask; // disable the mask entry.
			}
#else
			port->portPCMask &= ~mask; // disable the mask entry.
#endif
			if (port->portPCMask == 0) PCICR &= ~(port->PCICRbit);
			port->portRisingPins &= ~current->mask; port->portFallingPins &= ~current->mask;
			// TODO: This is removed until we can add code that frees memory.
			// Note that in the addPin() function, above, we do not define a new pin if it was
			// once already defined.
			// ... ...
			// Link the previous' next to the found next. Then remove the found.
			//if (prev != NULL) prev->next=current->next; // linked list skips over current.
			//else firstPin=current->next; // at the first pin; save the new first pin
			SREG = oldSREG; // Restore register; reenables interrupts
			return;
		}
		current=current->next;
	}
}

// common code for isr handler. "port" is the PCINT number.
// there isn't really a good way to back-map ports and masks to pins.
void PCintPort::PCint() {

	#ifdef FLASH
	if (*led_port & led_mask) *led_port&=not_led_mask;
	else *led_port|=led_mask;
    #endif
	#ifndef DISABLE_PCINT_MULTI_SERVICE
	uint8_t pcifr;
	while (true) {
	#endif
		// get the pin states for the indicated port.
		#ifdef PINMODE
		PCintPort::s_lastPinView=lastPinView;
		intrCount++;
		PCintPort::s_count=intrCount;
		#endif
		uint8_t changedPins = (PCintPort::curr ^ lastPinView) &
							  ((portRisingPins & PCintPort::curr ) | ( portFallingPins & ~PCintPort::curr ));

		#ifdef PINMODE
		PCintPort::s_currXORlastPinView=PCintPort::curr ^ lastPinView;
		PCintPort::s_portRisingPins_nCurr=portRisingPins & PCintPort::curr;
		PCintPort::s_portFallingPins_nNCurr=portFallingPins & ~PCintPort::curr;
		#endif
		lastPinView = PCintPort::curr;

		PCintPin* p = firstPin;
		while (p) {
			// Trigger interrupt if the bit is high and it's set to trigger on mode RISING or CHANGE
			// Trigger interrupt if the bit is low and it's set to trigger on mode FALLING or CHANGE
			if (p->mask & changedPins) {
				#ifndef NO_PIN_STATE
				PCintPort::pinState=PCintPort::curr & p->mask ? HIGH : LOW;
				#endif
				#ifndef NO_PIN_NUMBER
				PCintPort::arduinoPin=p->arduinoPin;
				#endif
				#ifdef PINMODE
				PCintPort::pinmode=p->mode;
				PCintPort::s_portRisingPins=portRisingPins;
				PCintPort::s_portFallingPins=portFallingPins;
				PCintPort::s_pmask=p->mask;
				PCintPort::s_changedPins=changedPins;
				#endif
				p->PCintFunc();
			}
			p=p->next;
		}
	#ifndef DISABLE_PCINT_MULTI_SERVICE
		pcifr = PCIFR & PCICRbit;
		if (pcifr == 0) break;
		PCIFR |= PCICRbit;
		#ifdef PINMODE
		PCintPort::pcint_multi++;
		if (PCIFR & PCICRbit) PCintPort::PCIFRbug=1; // PCIFR & PCICRbit should ALWAYS be 0 here!
		#endif
		PCintPort::curr=portInputReg;
	}
	#endif
}

#ifndef NO_PORTA_PINCHANGES
ISR(PCINT0_vect) {
	#ifdef PINMODE
	PCintPort::s_PORT='A';
	#endif
	PCintPort::curr = portA.portInputReg;
	portA.PCint();
}
#define PORTBVECT PCINT1_vect
#define PORTCVECT PCINT2_vect
#define PORTDVECT PCINT3_vect
#else
#define PORTBVECT PCINT0_vect
#define PORTCVECT PCINT1_vect
#define PORTDVECT PCINT2_vect
#endif

#ifndef NO_PORTB_PINCHANGES
ISR(PORTBVECT) {
	#ifdef PINMODE
	PCintPort::s_PORT='B';
	#endif
	PCintPort::curr = portB.portInputReg;
	portB.PCint();
}
#endif

#ifndef NO_PORTC_PINCHANGES
ISR(PORTCVECT) {
	#ifdef PINMODE
	PCintPort::s_PORT='C';
	#endif
	PCintPort::curr = portC.portInputReg;
	portC.PCint();
}
#endif

#ifndef NO_PORTD_PINCHANGES
ISR(PORTDVECT){ 
	#ifdef PINMODE
	PCintPort::s_PORT='D';
	#endif
	PCintPort::curr = portD.portInputReg;
	portD.PCint();
}
#endif

#ifdef __USE_PORT_JK
#ifndef NO_PORTJ_PINCHANGES
ISR(PCINT1_vect) {
	#ifdef PINMODE
	PCintPort::s_PORT='J';
	#endif
	PCintPort::curr = portJ.portInputReg;
	portJ.PCint();
}
#endif

#ifndef NO_PORTK_PINCHANGES
ISR(PCINT2_vect){ 
	#ifdef PINMODE
	PCintPort::s_PORT='K';
	#endif
	PCintPort::curr = portK.portInputReg;
	portK.PCint();
}
#endif

#endif // __USE_PORT_JK

#ifdef GET_PCINT_VERSION
uint16_t getPCIntVersion () {
	return ((uint16_t) PCINT_VERSION);
}
#endif // GET_PCINT_VERSION
#endif // #ifndef LIBCALL_PINCHANGEINT *************************************************************
#endif // #ifndef PinChangeInt_h *******************************************************************
