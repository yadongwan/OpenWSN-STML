/**
DO NOT EDIT DIRECTLY!!

This file was 'objectified' by SCons as a pre-processing
step for the building a Python extension module.

This was done on 2015-01-19 16:43:52.119000.
*/
/**
DO NOT EDIT DIRECTLY!!

This file was 'objectified' by SCons as a pre-processing
step for the building a Python extension module.

This was done on 2015-01-19 16:35:25.050000.
*/
/**
\brief Declaration of the "openserial" driver.

\author Fabien Chraim <chraim@eecs.berkeley.edu>, March 2012.
*/

#ifndef __OPENSERIAL_H
#define __OPENSERIAL_H

#include "opendefs_obj.h"

/**
\addtogroup drivers
\{
\addtogroup OpenSerial
\{
*/

//=========================== define ==========================================

/**
\brief Number of bytes of the serial output buffer, in bytes.

\warning should be exactly 256 so wrap-around on the index does not require
         the use of a slow modulo operator.
*/
#define SERIAL_OUTPUT_BUFFER_SIZE 256 // leave at 256!

/**
\brief Number of bytes of the serial input buffer, in bytes.

\warning Do not pick a number greater than 255, since its filling level is
         encoded by a single byte in the code.
*/
#define SERIAL_INPUT_BUFFER_SIZE  200

/// Modes of the openserial module.
enum {
   MODE_OFF    = 0, ///< The module is off, no serial activity.
   MODE_INPUT  = 1, ///< The serial is listening or receiving bytes.
   MODE_OUTPUT = 2  ///< The serial is transmitting bytes.
};

// frames sent mote->PC
#define SERFRAME_MOTE2PC_DATA               ((uint8_t)'D')
#define SERFRAME_MOTE2PC_STATUS             ((uint8_t)'S')
#define SERFRAME_MOTE2PC_INFO               ((uint8_t)'I')
#define SERFRAME_MOTE2PC_ERROR              ((uint8_t)'E')
#define SERFRAME_MOTE2PC_CRITICAL           ((uint8_t)'C')
#define SERFRAME_MOTE2PC_REQUEST            ((uint8_t)'R')

// frames sent PC->mote
#define SERFRAME_PC2MOTE_SETROOT            ((uint8_t)'R')
#define SERFRAME_PC2MOTE_DATA               ((uint8_t)'D')
#define SERFRAME_PC2MOTE_TRIGGERSERIALECHO  ((uint8_t)'S')

//=========================== typedef =========================================

//=========================== module variables ================================

typedef struct {
   // admin
   uint8_t    mode;
   uint8_t    debugPrintCounter;
   // input
   uint8_t    reqFrame[1+1+2+1]; // flag (1B), command (2B), CRC (2B), flag (1B)
   uint8_t    reqFrameIdx;
   uint8_t    lastRxByte;
   bool       busyReceiving;
   bool       inputEscaping;
   uint16_t   inputCrc;
   uint8_t    inputBufFill;
   uint8_t    inputBuf[SERIAL_INPUT_BUFFER_SIZE];
   // output
   bool       outputBufFilled;
   uint16_t   outputCrc;
   uint8_t    outputBufIdxW;
   uint8_t    outputBufIdxR;
   uint8_t    outputBuf[SERIAL_OUTPUT_BUFFER_SIZE];
} openserial_vars_t;

#include "openwsnmodule_obj.h"
typedef struct OpenMote OpenMote;

#include "openwsnmodule_obj.h"
typedef struct OpenMote OpenMote;

//=========================== prototypes ======================================

void openserial_init(OpenMote* self, OpenMote* self);
owerror_t openserial_printStatus(OpenMote* self, OpenMote* self, uint8_t statusElement, uint8_t* buffer, uint8_t length);
owerror_t openserial_printInfo(OpenMote* self, OpenMote* self, uint8_t calling_component, uint8_t error_code,
                              errorparameter_t arg1,
                              errorparameter_t arg2);
owerror_t openserial_printError(OpenMote* self, OpenMote* self, uint8_t calling_component, uint8_t error_code,
                              errorparameter_t arg1,
                              errorparameter_t arg2);
owerror_t openserial_printCritical(OpenMote* self, OpenMote* self, uint8_t calling_component, uint8_t error_code,
                              errorparameter_t arg1,
                              errorparameter_t arg2);
owerror_t openserial_printData(OpenMote* self, OpenMote* self, uint8_t* buffer, uint8_t length);
uint8_t openserial_getNumDataBytes(OpenMote* self, OpenMote* self);
uint8_t openserial_getInputBuffer(OpenMote* self, OpenMote* self, uint8_t* bufferToWrite, uint8_t maxNumBytes);
void openserial_startInput(OpenMote* self, OpenMote* self);
void openserial_startOutput(OpenMote* self, OpenMote* self);
void openserial_stop(OpenMote* self, OpenMote* self);
bool debugPrint_outBufferIndexes(OpenMote* self, OpenMote* self);
void openserial_echo(OpenMote* self, OpenMote* self, uint8_t* but, uint8_t bufLen);

// interrupt handlers
void isr_openserial_rx(OpenMote* self, OpenMote* self);
void isr_openserial_tx(OpenMote* self, OpenMote* self);

/**
\}
\}
*/

#endif
