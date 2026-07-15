/*
 * ReceiveDemo.cpp
 *
 * Demonstrates receiving IR codes with the IRremote library and the use of the Arduino tone() function with this library.
 * If debug button is pressed (pin connected to ground) a long output is generated.
 *
 *  This file is part of Arduino-IRremote https://github.com/Arduino-IRremote/Arduino-IRremote.
 *
 ************************************************************************************
 * MIT License
 *
 * Copyright (c) 2020-2022 Armin Joachimsmeyer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ************************************************************************************
 */

#include <Arduino.h>

/*
 * Specify which protocol(s) should be used for decoding.
 * If no protocol is defined, all protocols are active.
 * This must be done before the #include <IRremote.hpp>
 */
//#define DECODE_LG
//#define DECODE_NEC
// etc. see IRremote.hpp
//

//#define RAW_BUFFER_LENGTH  750  // 750 is the value for air condition remotes.

//#define NO_LED_FEEDBACK_CODE // saves 92 bytes program memory
#if FLASHEND <= 0x1FFF  // For 8k flash or less, like ATtiny85. Exclude exotic protocols.
#define EXCLUDE_EXOTIC_PROTOCOLS
#  if !defined(DIGISTUMPCORE) // ATTinyCore is bigger than Digispark core
#define EXCLUDE_UNIVERSAL_PROTOCOLS // Saves up to 1000 bytes program memory.
#  endif
#endif
//#define EXCLUDE_UNIVERSAL_PROTOCOLS // Saves up to 1000 bytes program memory.
//#define EXCLUDE_EXOTIC_PROTOCOLS // saves around 650 bytes program memory if all other protocols are active
//#define _IR_MEASURE_TIMING

// MARK_EXCESS_MICROS is subtracted from all marks and added to all spaces before decoding,
// to compensate for the signal forming of different IR receiver modules.
//#define MARK_EXCESS_MICROS    20 // 20 is recommended for the cheap VS1838 modules

//#define RECORD_GAP_MICROS 12000 // Activate it for some LG air conditioner protocols

//#define DEBUG // Activate this for lots of lovely debug output from the decoders.

#include "PinDefinitionsAndMore.h" //Define macros for input and output pin etc.
#include <IRremote.hpp>

#if defined(APPLICATION_PIN)
#define DEBUG_BUTTON_PIN    APPLICATION_PIN // if low, print timing for each received data set
#else
#define DEBUG_BUTTON_PIN   2
#endif

void setup() {
#if FLASHEND >= 0x3FFF  // For 16k flash or more, like ATtiny1604. Code does not fit in program memory of ATtiny85 etc.
    pinMode(DEBUG_BUTTON_PIN, INPUT_PULLUP);
#endif

    Serial.begin(9600);
    
    // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
    printActiveIRProtocols(&Serial);

}

void loop() {
    /*
     * Check if received data is available and if yes, try to decode it.
     * Decoded result is in the IrReceiver.decodedIRData structure.
     *
     * E.g. command is in IrReceiver.decodedIRData.command
     * address is in command is in IrReceiver.decodedIRData.address
     * and up to 32 bit raw data in IrReceiver.decodedIRData.decodedRawData
     */
    if (IrReceiver.decode()) {
        Serial.println();
        if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_WAS_OVERFLOW) {
            Serial.println(F("Overflow detected"));
            Serial.println(F("Try to increase the \"RAW_BUFFER_LENGTH\" value of " STR(RAW_BUFFER_LENGTH) " in " __FILE__));
        } else {
            // Print a short summary of received data
            // IrReceiver.printIRResultShort(&Serial);

            switch(IrReceiver.decodedIRData.command) {
                case 0x45:  Serial.println("- press -\t[CH-]");         break;
                case 0x46:  Serial.println("- press -\t[CH]");          break;
                case 0x47:  Serial.println("- press -\t[CH+]");         break;
                case 0x44:  Serial.println("- press -\t[PREV]");        break;
                case 0x40:  Serial.println("- press -\t[NEXT]");        break;
                case 0x43:  Serial.println("- press -\t[PLAY/PAUSE]");  break;
                case 0x07:  Serial.println("- press -\t[VOL-]");        break;
                case 0x15:  Serial.println("- press -\t[VOL+]");        break;
                case 0x09:  Serial.println("- press -\t[EQ]");          break;
                case 0x16:  Serial.println("- press -\t[0]");           break;
                case 0x19:  Serial.println("- press -\t[100+]");        break;
                case 0x0D:  Serial.println("- press -\t[200+]");        break;
                case 0x0C:  Serial.println("- press -\t[1]");           break;
                case 0x18:  Serial.println("- press -\t[2]");           break;
                case 0x5E:  Serial.println("- press -\t[3]");           break;
                case 0x08:  Serial.println("- press -\t[4]");           break;
                case 0x1C:  Serial.println("- press -\t[5]");           break;
                case 0x5A:  Serial.println("- press -\t[6]");           break;
                case 0x42:  Serial.println("- press -\t[7]");           break;
                case 0x52:  Serial.println("- press -\t[8]");           break;
                case 0x4A:  Serial.println("- press -\t[9]");           break;
            }

            if (IrReceiver.decodedIRData.protocol == UNKNOWN || digitalRead(DEBUG_BUTTON_PIN) == LOW) {
                // We have an unknown protocol, print more info
                // IrReceiver.printIRResultRawFormatted(&Serial, true);
            }
        }

        // Print a minimal summary of received data
        // IrReceiver.printIRResultMinimal(&Serial);

        /*
         * !!!Important!!! Enable receiving of the next value,
         * since receiving has stopped after the end of the current received data packet.
         */
        IrReceiver.resume();

        /*
         * Finally check the received data and perform actions according to the received address and commands
         */
        if (IrReceiver.decodedIRData.address == 0) {
            if (IrReceiver.decodedIRData.command == 0x10) {
                // do something
            } else if (IrReceiver.decodedIRData.command == 0x11) {
                // do something else
            }
        }
    } // if (IrReceiver.decode())

    if (IrReceiver.isIdle()) {
    }
    delay(100);
}
