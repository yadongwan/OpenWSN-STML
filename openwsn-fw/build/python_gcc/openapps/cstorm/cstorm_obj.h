/**
DO NOT EDIT DIRECTLY!!

This file was 'objectified' by SCons as a pre-processing
step for the building a Python extension module.

This was done on 2015-01-19 16:35:20.244000.
*/
#ifndef __CSTORM_H
#define __CSTORM_H

/**
\addtogroup AppUdp
\{
\addtogroup cstorm
\{
*/

#include "opencoap_obj.h"

//=========================== define ==========================================

//=========================== typedef =========================================

//=========================== variables =======================================

typedef struct {
   coap_resource_desc_t desc;
   opentimer_id_t       timerId;
   uint16_t             period;   ///< inter-packet period (in ms)
} cstorm_vars_t;

#include "openwsnmodule_obj.h"
typedef struct OpenMote OpenMote;

//=========================== prototypes ======================================

void cstorm_init(OpenMote* self);

/**
\}
\}
*/

#endif
