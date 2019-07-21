/**
DO NOT EDIT DIRECTLY!!

This file was 'objectified' by SCons as a pre-processing
step for the building a Python extension module.

This was done on 2015-01-19 16:35:28.454000.
*/
#ifndef __UECHO_H
#define __UECHO_H

/**
\addtogroup AppUdp
\{
\addtogroup uecho
\{
*/

//=========================== define ==========================================

//=========================== typedef =========================================

//=========================== variables =======================================

#include "openwsnmodule_obj.h"
typedef struct OpenMote OpenMote;

//=========================== prototypes ======================================

void uecho_init(OpenMote* self);
void uecho_receive(OpenMote* self, OpenQueueEntry_t* msg);
void uecho_sendDone(OpenMote* self, OpenQueueEntry_t* msg, owerror_t error);
bool uecho_debugPrint(OpenMote* self);

/**
\}
\}
*/

#endif