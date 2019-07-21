/**
DO NOT EDIT DIRECTLY!!

This file was 'objectified' by SCons as a pre-processing
step for the building a Python extension module.

This was done on 2015-01-19 16:43:40.392000.
*/
/**
\brief An example CoAP application.
*/

#include "opendefs_obj.h"
#include "cexample_obj.h"
#include "opencoap_obj.h"
#include "opentimers_obj.h"
#include "openqueue_obj.h"
#include "packetfunctions_obj.h"
#include "openserial_obj.h"
#include "openrandom_obj.h"
#include "scheduler_obj.h"
//#include "ADC_Channel.h"
#include "idmanager_obj.h"
#include "IEEE802154E_obj.h"

//=========================== defines =========================================

/// inter-packet period (in ms)
#define CEXAMPLEPERIOD  10000
#define PAYLOADLEN      62

const uint8_t cexample_path0[] = "ex";

//=========================== variables =======================================

cexample_vars_t cexample_vars;

//=========================== prototypes ======================================

owerror_t cexample_receive(OpenMote* self, OpenQueueEntry_t* msg,
                    coap_header_iht*  coap_header,
                    coap_option_iht*  coap_options);
void cexample_timer_cb(OpenMote* self);
void cexample_task_cb(OpenMote* self);
void cexample_sendDone(OpenMote* self, OpenQueueEntry_t* msg,
                       owerror_t error);

//=========================== public ==========================================

void cexample_init(OpenMote* self) {
   
   // prepare the resource descriptor for the /ex path
   cexample_vars.desc.path0len             = sizeof(cexample_path0)-1;
   cexample_vars.desc.path0val             = (uint8_t*)(&cexample_path0);
   cexample_vars.desc.path1len             = 0;
   cexample_vars.desc.path1val             = NULL;
   cexample_vars.desc.componentID          = COMPONENT_CEXAMPLE;
   cexample_vars.desc.callbackRx           = &cexample_receive;
   cexample_vars.desc.callbackSendDone     = &cexample_sendDone;
   
   
 opencoap_register(self, &cexample_vars.desc);
   cexample_vars.timerId    = opentimers_start(self, CEXAMPLEPERIOD,
                                                TIMER_PERIODIC,TIME_MS,
                                                cexample_timer_cb);
}

//=========================== private =========================================

owerror_t cexample_receive(OpenMote* self, OpenQueueEntry_t* msg,
                      coap_header_iht* coap_header,
                      coap_option_iht* coap_options) {
   return E_FAIL;
}

//timer fired, but we don't want to execute task in ISR mode
//instead, push task to scheduler with COAP priority, and let scheduler take care of it
void cexample_timer_cb(OpenMote* self){
 scheduler_push_task(self, cexample_task_cb,TASKPRIO_COAP);
}

void cexample_task_cb(OpenMote* self) {
   OpenQueueEntry_t*    pkt;
   owerror_t            outcome;
   uint8_t              numOptions;
   uint8_t              i;
   
   uint16_t             x_int       = 0;
   uint16_t             sum         = 0;
   uint16_t             avg         = 0;
   uint8_t              N_avg       = 10;
   
   // don't run if not synch
   if ( ieee154e_isSynch(self) == FALSE) return;
   
   // don't run on dagroot
   if ( idmanager_getIsDAGroot(self)) {
 opentimers_stop(self, cexample_vars.timerId);
      return;
   }
   
   for (i = 0; i < N_avg; i++) {
      sum += x_int;
   }
   avg = sum/N_avg;
   
   // create a CoAP RD packet
   pkt = openqueue_getFreePacketBuffer(self, COMPONENT_CEXAMPLE);
   if (pkt==NULL) {
 openserial_printError(self, 
         COMPONENT_CEXAMPLE,
         ERR_NO_FREE_PACKET_BUFFER,
         (errorparameter_t)0,
         (errorparameter_t)0
      );
 openqueue_freePacketBuffer(self, pkt);
      return;
   }
   // take ownership over that packet
   pkt->creator                   = COMPONENT_CEXAMPLE;
   pkt->owner                     = COMPONENT_CEXAMPLE;
   // CoAP payload
 packetfunctions_reserveHeaderSize(self, pkt,PAYLOADLEN);
   for (i=0;i<PAYLOADLEN;i++) {
      pkt->payload[i]             = i;
   }
   avg = openrandom_get16b(self);
   pkt->payload[0]                = (avg>>8)&0xff;
   pkt->payload[1]                = (avg>>0)&0xff;
   
   numOptions = 0;
   // location-path option
 packetfunctions_reserveHeaderSize(self, pkt,sizeof(cexample_path0)-1);
   memcpy(&pkt->payload[0],cexample_path0,sizeof(cexample_path0)-1);
 packetfunctions_reserveHeaderSize(self, pkt,1);
   pkt->payload[0]                = ((COAP_OPTION_NUM_URIPATH) << 4) | (sizeof(cexample_path0)-1);
   numOptions++;
   // content-type option
 packetfunctions_reserveHeaderSize(self, pkt,2);
   pkt->payload[0]                = (COAP_OPTION_NUM_CONTENTFORMAT << 4) | 1;
   pkt->payload[1]                = COAP_MEDTYPE_APPOCTETSTREAM;
   numOptions++;
   
   // metadata
   pkt->l4_destination_port       = WKP_UDP_COAP;
   pkt->l3_destinationAdd.type    = ADDR_128B;
   memcpy(&pkt->l3_destinationAdd.addr_128b[0],&ipAddr_motesEecs,16);
   
   // send
   outcome = opencoap_send(self, 
      pkt,
      COAP_TYPE_NON,
      COAP_CODE_REQ_PUT,
      numOptions,
      &cexample_vars.desc
   );
   
   // avoid overflowing the queue if fails
   if (outcome==E_FAIL) {
 openqueue_freePacketBuffer(self, pkt);
   }
   
   return;
}

void cexample_sendDone(OpenMote* self, OpenQueueEntry_t* msg, owerror_t error) {
 openqueue_freePacketBuffer(self, msg);
}
