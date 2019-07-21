/**
DO NOT EDIT DIRECTLY!!

This file was 'objectified' by SCons as a pre-processing
step for the building a Python extension module.

This was done on 2015-01-19 16:35:27.
*/
#ifndef __RRT_H
#define __RRT_H

/**
\addtogroup AppCoAP
\{
\addtogroup rrt
\{
*/

#include "opendefs_obj.h"
#include "opencoap_obj.h"
#include "schedule_obj.h"

//=========================== define ==========================================

//=========================== typedef =========================================

//=========================== variables =======================================

typedef struct {
	coap_resource_desc_t desc;
	uint8_t discovered;
} rrt_vars_t;

#include "openwsnmodule_obj.h"
typedef struct OpenMote OpenMote;

//=========================== prototypes ======================================

void rrt_init(OpenMote* self);

void rrt_sendCoAPMsg(OpenMote* self, char actionMsg, uint8_t *ipv6mote);
void rrt_sendDone(OpenMote* self, OpenQueueEntry_t* msg, owerror_t error);
/**
\}
\}
*/

#endif