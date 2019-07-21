/**
DO NOT EDIT DIRECTLY!!

This file was 'objectified' by SCons as a pre-processing
step for the building a Python extension module.

This was done on 2015-01-19 16:43:47.599000.
*/
/**
\brief Definition of the "openserial" driver.

\author Fabien Chraim <chraim@eecs.berkeley.edu>, March 2012.
*/

#include "opendefs_obj.h"
#include "openserial_obj.h"
#include "IEEE802154E_obj.h"
#include "neighbors_obj.h"
#include "sixtop_obj.h"
#include "icmpv6echo_obj.h"
#include "idmanager_obj.h"
#include "openqueue_obj.h"
#include "openbridge_obj.h"
#include "leds_obj.h"
#include "schedule_obj.h"
#include "uart_obj.h"
#include "opentimers_obj.h"
#include "openhdlc_obj.h"

//=========================== variables =======================================

// declaration of global variable _openserial_vars_ removed during objectification.

//=========================== prototypes ======================================

owerror_t openserial_printInfoErrorCritical(OpenMote* self, 
   char             severity,
   uint8_t          calling_component,
   uint8_t          error_code,
   errorparameter_t arg1,
   errorparameter_t arg2
);
// HDLC output
void outputHdlcOpen(OpenMote* self);
void outputHdlcWrite(OpenMote* self, uint8_t b);
void outputHdlcClose(OpenMote* self);
// HDLC input
void inputHdlcOpen(OpenMote* self);
void inputHdlcWrite(OpenMote* self, uint8_t b);
void inputHdlcClose(OpenMote* self);

//=========================== public ==========================================

void openserial_init(OpenMote* self) {
   uint16_t crc;
   
   // reset variable
   memset(&(self->openserial_vars),0,sizeof(openserial_vars_t));
   
   // admin
   (self->openserial_vars).mode                = MODE_OFF;
   (self->openserial_vars).debugPrintCounter   = 0;
   
   // input
   (self->openserial_vars).reqFrame[0]         = HDLC_FLAG;
   (self->openserial_vars).reqFrame[1]         = SERFRAME_MOTE2PC_REQUEST;
   crc = HDLC_CRCINIT;
   crc = crcIteration(crc,(self->openserial_vars).reqFrame[1]);
   crc = ~crc;
   (self->openserial_vars).reqFrame[2]         = (crc>>0)&0xff;
   (self->openserial_vars).reqFrame[3]         = (crc>>8)&0xff;
   (self->openserial_vars).reqFrame[4]         = HDLC_FLAG;
   (self->openserial_vars).reqFrameIdx         = 0;
   (self->openserial_vars).lastRxByte          = HDLC_FLAG;
   (self->openserial_vars).busyReceiving       = FALSE;
   (self->openserial_vars).inputEscaping       = FALSE;
   (self->openserial_vars).inputBufFill        = 0;
   
   // ouput
   (self->openserial_vars).outputBufFilled     = FALSE;
   (self->openserial_vars).outputBufIdxR       = 0;
   (self->openserial_vars).outputBufIdxW       = 0;
   
   // set callbacks
 uart_setCallbacks(self, isr_openserial_tx,
                     isr_openserial_rx);
}

owerror_t openserial_printStatus(OpenMote* self, uint8_t statusElement,uint8_t* buffer, uint8_t length) {
   uint8_t i;
   INTERRUPT_DECLARATION();
   
   DISABLE_INTERRUPTS();
   (self->openserial_vars).outputBufFilled  = TRUE;
 outputHdlcOpen(self);
 outputHdlcWrite(self, SERFRAME_MOTE2PC_STATUS);
 outputHdlcWrite(self, idmanager_getMyID(self, ADDR_16B)->addr_16b[0]);
 outputHdlcWrite(self, idmanager_getMyID(self, ADDR_16B)->addr_16b[1]);
 outputHdlcWrite(self, statusElement);
   for (i=0;i<length;i++){
 outputHdlcWrite(self, buffer[i]);
   }
 outputHdlcClose(self);
   ENABLE_INTERRUPTS();
   
   return E_SUCCESS;
}

owerror_t openserial_printInfoErrorCritical(OpenMote* self, 
      char             severity,
      uint8_t          calling_component,
      uint8_t          error_code,
      errorparameter_t arg1,
      errorparameter_t arg2
   ) {
   INTERRUPT_DECLARATION();
   
   DISABLE_INTERRUPTS();
   (self->openserial_vars).outputBufFilled  = TRUE;
 outputHdlcOpen(self);
 outputHdlcWrite(self, severity);
 outputHdlcWrite(self, idmanager_getMyID(self, ADDR_16B)->addr_16b[0]);
 outputHdlcWrite(self, idmanager_getMyID(self, ADDR_16B)->addr_16b[1]);
 outputHdlcWrite(self, calling_component);
 outputHdlcWrite(self, error_code);
 outputHdlcWrite(self, (uint8_t)((arg1 & 0xff00)>>8));
 outputHdlcWrite(self, (uint8_t) (arg1 & 0x00ff));
 outputHdlcWrite(self, (uint8_t)((arg2 & 0xff00)>>8));
 outputHdlcWrite(self, (uint8_t) (arg2 & 0x00ff));
 outputHdlcClose(self);
   ENABLE_INTERRUPTS();
   
   return E_SUCCESS;
}

owerror_t openserial_printData(OpenMote* self, uint8_t* buffer, uint8_t length) {
   uint8_t  i;
   uint8_t  asn[5];
   INTERRUPT_DECLARATION();
   
   // retrieve ASN
 ieee154e_getAsn(self, asn);// byte01,byte23,byte4
   
   DISABLE_INTERRUPTS();
   (self->openserial_vars).outputBufFilled  = TRUE;
 outputHdlcOpen(self);
 outputHdlcWrite(self, SERFRAME_MOTE2PC_DATA);
 outputHdlcWrite(self, idmanager_getMyID(self, ADDR_16B)->addr_16b[1]);
 outputHdlcWrite(self, idmanager_getMyID(self, ADDR_16B)->addr_16b[0]);
 outputHdlcWrite(self, asn[0]);
 outputHdlcWrite(self, asn[1]);
 outputHdlcWrite(self, asn[2]);
 outputHdlcWrite(self, asn[3]);
 outputHdlcWrite(self, asn[4]);
   for (i=0;i<length;i++){
 outputHdlcWrite(self, buffer[i]);
   }
 outputHdlcClose(self);
   ENABLE_INTERRUPTS();
   
   return E_SUCCESS;
}

owerror_t openserial_printInfo(OpenMote* self, uint8_t calling_component, uint8_t error_code,
                              errorparameter_t arg1,
                              errorparameter_t arg2) {
   return openserial_printInfoErrorCritical(self, 
      SERFRAME_MOTE2PC_INFO,
      calling_component,
      error_code,
      arg1,
      arg2
   );
}

owerror_t openserial_printError(OpenMote* self, uint8_t calling_component, uint8_t error_code,
                              errorparameter_t arg1,
                              errorparameter_t arg2) {
   // blink error LED, this is serious
 leds_error_toggle(self);
   
   return openserial_printInfoErrorCritical(self, 
      SERFRAME_MOTE2PC_ERROR,
      calling_component,
      error_code,
      arg1,
      arg2
   );
}

owerror_t openserial_printCritical(OpenMote* self, uint8_t calling_component, uint8_t error_code,
                              errorparameter_t arg1,
                              errorparameter_t arg2) {
   // blink error LED, this is serious
 leds_error_blink(self);
   
   // schedule for the mote to reboot in 10s
 opentimers_start(self, 10000,
                    TIMER_ONESHOT,TIME_MS,
                    board_reset);
   
   return openserial_printInfoErrorCritical(self, 
      SERFRAME_MOTE2PC_CRITICAL,
      calling_component,
      error_code,
      arg1,
      arg2
   );
}

uint8_t openserial_getNumDataBytes(OpenMote* self) {
   uint8_t inputBufFill;
   INTERRUPT_DECLARATION();
   
   DISABLE_INTERRUPTS();
   inputBufFill = (self->openserial_vars).inputBufFill;
   ENABLE_INTERRUPTS();

   return inputBufFill-1; // removing the command byte
}

uint8_t openserial_getInputBuffer(OpenMote* self, uint8_t* bufferToWrite, uint8_t maxNumBytes) {
   uint8_t numBytesWritten;
   uint8_t inputBufFill;
   INTERRUPT_DECLARATION();
   
   DISABLE_INTERRUPTS();
   inputBufFill = (self->openserial_vars).inputBufFill;
   ENABLE_INTERRUPTS();
   
   if (maxNumBytes<inputBufFill-1) {
 openserial_printError(self, COMPONENT_OPENSERIAL,ERR_GETDATA_ASKS_TOO_FEW_BYTES,
                            (errorparameter_t)maxNumBytes,
                            (errorparameter_t)inputBufFill-1);
      numBytesWritten = 0;
   } else {
      numBytesWritten = inputBufFill-1;
      memcpy(bufferToWrite,&((self->openserial_vars).inputBuf[1]),numBytesWritten);
   }
   
   return numBytesWritten;
}

void openserial_startInput(OpenMote* self) {
   INTERRUPT_DECLARATION();
   
   if ((self->openserial_vars).inputBufFill>0) {
 openserial_printError(self, COMPONENT_OPENSERIAL,ERR_INPUTBUFFER_LENGTH,
                            (errorparameter_t)(self->openserial_vars).inputBufFill,
                            (errorparameter_t)0);
      DISABLE_INTERRUPTS();
      (self->openserial_vars).inputBufFill=0;
      ENABLE_INTERRUPTS();
   }
   
 uart_clearTxInterrupts(self);
 uart_clearRxInterrupts(self);      // clear possible pending interrupts
 uart_enableInterrupts(self);       // Enable USCI_A1 TX & RX interrupt
   
   DISABLE_INTERRUPTS();
   (self->openserial_vars).busyReceiving  = FALSE;
   (self->openserial_vars).mode           = MODE_INPUT;
   (self->openserial_vars).reqFrameIdx    = 0;
#ifdef FASTSIM
 uart_writeBufferByLen_FASTSIM(self, 
      (self->openserial_vars).reqFrame,
      sizeof((self->openserial_vars).reqFrame)
   );
   (self->openserial_vars).reqFrameIdx = sizeof((self->openserial_vars).reqFrame);
#else
 uart_writeByte(self, (self->openserial_vars).reqFrame[(self->openserial_vars).reqFrameIdx]);
#endif
   ENABLE_INTERRUPTS();
}

void openserial_startOutput(OpenMote* self) {
   //schedule a task to get new status in the output buffer
   uint8_t debugPrintCounter;
   
   INTERRUPT_DECLARATION();
   DISABLE_INTERRUPTS();
   (self->openserial_vars).debugPrintCounter = ((self->openserial_vars).debugPrintCounter+1)%STATUS_MAX;
   debugPrintCounter = (self->openserial_vars).debugPrintCounter;
   ENABLE_INTERRUPTS();
   
   // print debug information
   switch (debugPrintCounter) {
      case STATUS_ISSYNC:
         if ( debugPrint_isSync(self)==TRUE) {
            break;
         }
      case STATUS_ID:
         if ( debugPrint_id(self)==TRUE) {
            break;
         }
      case STATUS_DAGRANK:
         if ( debugPrint_myDAGrank(self)==TRUE) {
            break;
         }
      case STATUS_OUTBUFFERINDEXES:
         if ( debugPrint_outBufferIndexes(self)==TRUE) {
            break;
         }
      case STATUS_ASN:
         if ( debugPrint_asn(self)==TRUE) {
            break;
         }
      case STATUS_MACSTATS:
         if ( debugPrint_macStats(self)==TRUE) {
            break;
         }
      case STATUS_SCHEDULE:
         if( debugPrint_schedule(self)==TRUE) {
            break;
         }
      case STATUS_BACKOFF:
         if( debugPrint_backoff(self)==TRUE) {
            break;
         }
      case STATUS_QUEUE:
         if( debugPrint_queue(self)==TRUE) {
            break;
         }
      case STATUS_NEIGHBORS:
         if ( debugPrint_neighbors(self)==TRUE) {
            break;
         }
      case STATUS_KAPERIOD:
         if ( debugPrint_kaPeriod(self)==TRUE) {
            break;
         }
      default:
         DISABLE_INTERRUPTS();
         (self->openserial_vars).debugPrintCounter=0;
         ENABLE_INTERRUPTS();
   }
   
   // flush buffer
 uart_clearTxInterrupts(self);
 uart_clearRxInterrupts(self);          // clear possible pending interrupts
 uart_enableInterrupts(self);           // Enable USCI_A1 TX & RX interrupt
   DISABLE_INTERRUPTS();
   (self->openserial_vars).mode=MODE_OUTPUT;
   if ((self->openserial_vars).outputBufFilled) {
#ifdef FASTSIM
 uart_writeCircularBuffer_FASTSIM(self, 
         (self->openserial_vars).outputBuf,
         &(self->openserial_vars).outputBufIdxR,
         &(self->openserial_vars).outputBufIdxW
      );
#else
 uart_writeByte(self, (self->openserial_vars).outputBuf[(self->openserial_vars).outputBufIdxR++]);
#endif
   } else {
 openserial_stop(self);
   }
   ENABLE_INTERRUPTS();
}

void openserial_stop(OpenMote* self) {
   uint8_t inputBufFill;
   uint8_t cmdByte;
   bool busyReceiving;
   INTERRUPT_DECLARATION();
   
   DISABLE_INTERRUPTS();
   busyReceiving = (self->openserial_vars).busyReceiving;
   inputBufFill = (self->openserial_vars).inputBufFill;
   ENABLE_INTERRUPTS();
   
   // disable USCI_A1 TX & RX interrupt
 uart_disableInterrupts(self);
   
   DISABLE_INTERRUPTS();
   (self->openserial_vars).mode=MODE_OFF;
   ENABLE_INTERRUPTS();
   //the inputBuffer has to be reset if it is not reset where the data is read.
   //or the function openserial_getInputBuffer is called (which resets the buffer)
   if (busyReceiving==TRUE){
 openserial_printError(self, COMPONENT_OPENSERIAL,ERR_BUSY_RECEIVING,
                                  (errorparameter_t)0,
                                  (errorparameter_t)inputBufFill);
   }
   
   if (busyReceiving == FALSE && inputBufFill>0) {
      DISABLE_INTERRUPTS();
      cmdByte = (self->openserial_vars).inputBuf[0];
      ENABLE_INTERRUPTS();
      switch (cmdByte) {
         case SERFRAME_PC2MOTE_SETROOT:
 idmanager_triggerAboutRoot(self);
            break;
         case SERFRAME_PC2MOTE_DATA:
 openbridge_triggerData(self);
            break;
         case SERFRAME_PC2MOTE_TRIGGERSERIALECHO:
            //echo function must reset input buffer after reading the data.
 openserial_echo(self, &(self->openserial_vars).inputBuf[1],inputBufFill-1);
            break;   
         default:
 openserial_printError(self, COMPONENT_OPENSERIAL,ERR_UNSUPPORTED_COMMAND,
                                  (errorparameter_t)cmdByte,
                                  (errorparameter_t)0);
            //reset here as it is not being reset in any other callback
            DISABLE_INTERRUPTS();
            (self->openserial_vars).inputBufFill = 0;
            ENABLE_INTERRUPTS();
            break;
      }
   }
   
   DISABLE_INTERRUPTS();
   (self->openserial_vars).inputBufFill  = 0;
   (self->openserial_vars).busyReceiving = FALSE;
   ENABLE_INTERRUPTS();
}

/**
\brief Trigger this module to print status information, over serial.

debugPrint_* functions are used by the openserial module to continuously print
status information about several modules in the OpenWSN stack.

\returns TRUE if this function printed something, FALSE otherwise.
*/
bool debugPrint_outBufferIndexes(OpenMote* self) {
   uint16_t temp_buffer[2];
   INTERRUPT_DECLARATION();
   DISABLE_INTERRUPTS();
   temp_buffer[0] = (self->openserial_vars).outputBufIdxW;
   temp_buffer[1] = (self->openserial_vars).outputBufIdxR;
   ENABLE_INTERRUPTS();
 openserial_printStatus(self, STATUS_OUTBUFFERINDEXES,(uint8_t*)temp_buffer,sizeof(temp_buffer));
   return TRUE;
}

//=========================== private =========================================

//===== hdlc (output)

/**
\brief Start an HDLC frame in the output buffer.
*/
port_INLINE void outputHdlcOpen(OpenMote* self) {
   // initialize the value of the CRC
   (self->openserial_vars).outputCrc                          = HDLC_CRCINIT;
   
   // write the opening HDLC flag
   (self->openserial_vars).outputBuf[(self->openserial_vars).outputBufIdxW++]     = HDLC_FLAG;
}
/**
\brief Add a byte to the outgoing HDLC frame being built.
*/
port_INLINE void outputHdlcWrite(OpenMote* self, uint8_t b) {
   
   // iterate through CRC calculator
   (self->openserial_vars).outputCrc = crcIteration((self->openserial_vars).outputCrc,b);
   
   // add byte to buffer
   if (b==HDLC_FLAG || b==HDLC_ESCAPE) {
      (self->openserial_vars).outputBuf[(self->openserial_vars).outputBufIdxW++]  = HDLC_ESCAPE;
      b                                               = b^HDLC_ESCAPE_MASK;
   }
   (self->openserial_vars).outputBuf[(self->openserial_vars).outputBufIdxW++]     = b;
   
}
/**
\brief Finalize the outgoing HDLC frame.
*/
port_INLINE void outputHdlcClose(OpenMote* self) {
   uint16_t   finalCrc;
    
   // finalize the calculation of the CRC
   finalCrc   = ~(self->openserial_vars).outputCrc;
   
   // write the CRC value
 outputHdlcWrite(self, (finalCrc>>0)&0xff);
 outputHdlcWrite(self, (finalCrc>>8)&0xff);
   
   // write the closing HDLC flag
   (self->openserial_vars).outputBuf[(self->openserial_vars).outputBufIdxW++]   = HDLC_FLAG;
}

//===== hdlc (input)

/**
\brief Start an HDLC frame in the input buffer.
*/
port_INLINE void inputHdlcOpen(OpenMote* self) {
   // reset the input buffer index
   (self->openserial_vars).inputBufFill                       = 0;
   
   // initialize the value of the CRC
   (self->openserial_vars).inputCrc                           = HDLC_CRCINIT;
}
/**
\brief Add a byte to the incoming HDLC frame.
*/
port_INLINE void inputHdlcWrite(OpenMote* self, uint8_t b) {
   if (b==HDLC_ESCAPE) {
      (self->openserial_vars).inputEscaping = TRUE;
   } else {
      if ((self->openserial_vars).inputEscaping==TRUE) {
         b                             = b^HDLC_ESCAPE_MASK;
         (self->openserial_vars).inputEscaping = FALSE;
      }
      
      // add byte to input buffer
      (self->openserial_vars).inputBuf[(self->openserial_vars).inputBufFill] = b;
      (self->openserial_vars).inputBufFill++;
      
      // iterate through CRC calculator
      (self->openserial_vars).inputCrc = crcIteration((self->openserial_vars).inputCrc,b);
   }
}
/**
\brief Finalize the incoming HDLC frame.
*/
port_INLINE void inputHdlcClose(OpenMote* self) {
   
   // verify the validity of the frame
   if ((self->openserial_vars).inputCrc==HDLC_CRCGOOD) {
      // the CRC is correct
      
      // remove the CRC from the input buffer
      (self->openserial_vars).inputBufFill    -= 2;
   } else {
      // the CRC is incorrect
      
      // drop the incoming fram
      (self->openserial_vars).inputBufFill     = 0;
   }
}

//=========================== interrupt handlers ==============================

//executed in ISR, called from scheduler.c
void isr_openserial_tx(OpenMote* self) {
   switch ((self->openserial_vars).mode) {
      case MODE_INPUT:
         (self->openserial_vars).reqFrameIdx++;
         if ((self->openserial_vars).reqFrameIdx<sizeof((self->openserial_vars).reqFrame)) {
 uart_writeByte(self, (self->openserial_vars).reqFrame[(self->openserial_vars).reqFrameIdx]);
         }
         break;
      case MODE_OUTPUT:
         if ((self->openserial_vars).outputBufIdxW==(self->openserial_vars).outputBufIdxR) {
            (self->openserial_vars).outputBufFilled = FALSE;
         }
         if ((self->openserial_vars).outputBufFilled) {
 uart_writeByte(self, (self->openserial_vars).outputBuf[(self->openserial_vars).outputBufIdxR++]);
         }
         break;
      case MODE_OFF:
      default:
         break;
   }
}

// executed in ISR, called from scheduler.c
void isr_openserial_rx(OpenMote* self) {
   uint8_t rxbyte;
   uint8_t inputBufFill;
   
   // stop if I'm not in input mode
   if ((self->openserial_vars).mode!=MODE_INPUT) {
      return;
   }
   
   // read byte just received
   rxbyte = uart_readByte(self);
   //keep lenght
   inputBufFill=(self->openserial_vars).inputBufFill;
   
   if        (
                (self->openserial_vars).busyReceiving==FALSE  &&
                (self->openserial_vars).lastRxByte==HDLC_FLAG &&
                rxbyte!=HDLC_FLAG
              ) {
      // start of frame
      
      // I'm now receiving
      (self->openserial_vars).busyReceiving         = TRUE;
      
      // create the HDLC frame
 inputHdlcOpen(self);
      
      // add the byte just received
 inputHdlcWrite(self, rxbyte);
   } else if (
                (self->openserial_vars).busyReceiving==TRUE   &&
                rxbyte!=HDLC_FLAG
             ) {
      // middle of frame
      
      // add the byte just received
 inputHdlcWrite(self, rxbyte);
      if ((self->openserial_vars).inputBufFill+1>SERIAL_INPUT_BUFFER_SIZE){
         // input buffer overflow
 openserial_printError(self, COMPONENT_OPENSERIAL,ERR_INPUT_BUFFER_OVERFLOW,
                               (errorparameter_t)0,
                               (errorparameter_t)0);
         (self->openserial_vars).inputBufFill       = 0;
         (self->openserial_vars).busyReceiving      = FALSE;
 openserial_stop(self);
      }
   } else if (
                (self->openserial_vars).busyReceiving==TRUE   &&
                rxbyte==HDLC_FLAG
              ) {
         // end of frame
         
         // finalize the HDLC frame
 inputHdlcClose(self);
         
         if ((self->openserial_vars).inputBufFill==0){
            // invalid HDLC frame
 openserial_printError(self, COMPONENT_OPENSERIAL,ERR_WRONG_CRC_INPUT,
                                  (errorparameter_t)inputBufFill,
                                  (errorparameter_t)0);
         
         }
         
         (self->openserial_vars).busyReceiving      = FALSE;
 openserial_stop(self);
   }
   
   (self->openserial_vars).lastRxByte = rxbyte;
}

//======== SERIAL ECHO =============

void openserial_echo(OpenMote* self, uint8_t* buf, uint8_t bufLen){
   INTERRUPT_DECLARATION();
   // echo back what you received
 openserial_printData(self, 
      buf,
      bufLen
   );
   
    DISABLE_INTERRUPTS();
    (self->openserial_vars).inputBufFill = 0;
    ENABLE_INTERRUPTS();
}