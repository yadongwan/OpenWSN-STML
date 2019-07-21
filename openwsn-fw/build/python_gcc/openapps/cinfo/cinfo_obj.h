/**
DO NOT EDIT DIRECTLY!!

This file was 'objectified' by SCons as a pre-processing
step for the building a Python extension module.

This was done on 2015-01-19 16:35:19.978000.
*/
#ifndef __CINFO_H
#define __CINFO_H

/**
\addtogroup AppCoAP
\{
\addtogroup cinfo
\{
*/

#include "opencoap_obj.h"

//=========================== define ==========================================

//=========================== typedef =========================================

//=========================== variables =======================================

typedef struct {
   coap_resource_desc_t desc;
} cinfo_vars_t;

#include "openwsnmodule_obj.h"
typedef struct OpenMote OpenMote;

//=========================== prototypes ======================================

void cinfo_init(OpenMote* self);

/**
\}
\}
*/

#endif
