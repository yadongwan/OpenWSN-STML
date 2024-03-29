/**
DO NOT EDIT DIRECTLY!!

This file was 'objectified' by SCons as a pre-processing
step for the building a Python extension module.

This was done on 2015-01-19 16:35:27.141000.
*/
#ifndef __SCHEDULE_H
#define __SCHEDULE_H

/**
\addtogroup MAChigh
\{
\addtogroup Schedule
\{
*/

#include "opendefs_obj.h"

//=========================== define ==========================================

/**
\brief The length of the superframe, in slots.

The superframe repears over time and can be arbitrarly long.
*/
#define SUPERFRAME_LENGTH    11 //should be 101

#define NUMADVSLOTS          1
#define NUMSHAREDTXRX        5 
#define NUMSERIALRX          3

/**
\brief Maximum number of active slots in a superframe.

Note that this is merely used to allocate RAM memory for the schedule. The
schedule is represented, in RAM, by a table. There is one row per active slot
in that table; a slot is "active" when it is not of type CELLTYPE_OFF.

Set this number to the exact number of active slots you are planning on having
in your schedule, so not to waste RAM.
*/
#define MAXACTIVESLOTS       (NUMADVSLOTS+NUMSHAREDTXRX+NUMSERIALRX)

/**
\brief Minimum backoff exponent.

Backoff is used only in slots that are marked as shared in the schedule. When
not shared, the mote assumes that schedule is collision-free, and therefore
does not use any backoff mechanism when a transmission fails.
*/
#define MINBE                2

/**
\brief Maximum backoff exponent.

See MINBE for an explanation of backoff.
*/
#define MAXBE                4
//6tisch minimal draft
#define SCHEDULE_MINIMAL_6TISCH_ACTIVE_CELLS                      5
#define SCHEDULE_MINIMAL_6TISCH_EB_CELLS                          1
#define SCHEDULE_MINIMAL_6TISCH_SLOTFRAME_SIZE                  101
#define SCHEDULE_MINIMAL_6TISCH_DEFAULT_SLOTFRAME_HANDLE          1 //id of slotframe
#define SCHEDULE_MINIMAL_6TISCH_DEFAULT_SLOTFRAME_NUMBER          1 //1 slotframe by default.

//=========================== typedef =========================================

typedef uint8_t    channelOffset_t;
typedef uint16_t   slotOffset_t;
typedef uint16_t   frameLength_t;

typedef enum {
   CELLTYPE_OFF              = 0,
   CELLTYPE_ADV              = 1,
   CELLTYPE_TX               = 2,
   CELLTYPE_RX               = 3,
   CELLTYPE_TXRX             = 4,
   CELLTYPE_SERIALRX         = 5,
   CELLTYPE_MORESERIALRX     = 6
} cellType_t;

typedef struct {
   slotOffset_t    slotOffset;
   cellType_t      type;
   bool            shared;
   uint8_t         channelOffset;
   open_addr_t     neighbor;
   uint8_t         numRx;
   uint8_t         numTx;
   uint8_t         numTxACK;
   asn_t           lastUsedAsn;
   void*           next;
} scheduleEntry_t;

BEGIN_PACK
typedef struct {
   uint8_t         row;
   slotOffset_t    slotOffset;
   uint8_t         type;
   bool            shared;
   uint8_t         channelOffset;
   open_addr_t     neighbor;
   uint8_t         numRx;
   uint8_t         numTx;
   uint8_t         numTxACK;
   asn_t           lastUsedAsn;
} debugScheduleEntry_t;
END_PACK

typedef struct {
  uint8_t          address[LENGTH_ADDR64b];
  cellType_t       link_type;
  bool             shared;
  slotOffset_t     slotOffset;
  channelOffset_t  channelOffset;
}slotinfo_element_t;

//=========================== variables =======================================

typedef struct {
   scheduleEntry_t  scheduleBuf[MAXACTIVESLOTS];
   scheduleEntry_t* currentScheduleEntry;
   uint16_t         frameLength;
   uint8_t          backoffExponent;
   uint8_t          backoff;
   uint8_t          debugPrintRow;
} schedule_vars_t;

#include "openwsnmodule_obj.h"
typedef struct OpenMote OpenMote;

//=========================== prototypes ======================================

// admin
void schedule_init(OpenMote* self);
bool debugPrint_schedule(OpenMote* self);
bool debugPrint_backoff(OpenMote* self);

// from 6top
void schedule_setFrameLength(OpenMote* self, frameLength_t newFrameLength);
owerror_t schedule_addActiveSlot(OpenMote* self, 
   slotOffset_t         slotOffset,
   cellType_t           type,
   bool                 shared,
   uint8_t              channelOffset,
   open_addr_t*         neighbor
);

void schedule_getSlotInfo(OpenMote* self, 
   slotOffset_t         slotOffset,                      
   open_addr_t*         neighbor,
   slotinfo_element_t*  info
);

owerror_t schedule_removeActiveSlot(OpenMote* self, 
   slotOffset_t         slotOffset,
   open_addr_t*         neighbor
);
bool schedule_isSlotOffsetAvailable(OpenMote* self, uint16_t slotOffset);

// from IEEE802154E
void schedule_syncSlotOffset(OpenMote* self, slotOffset_t targetSlotOffset);
void schedule_advanceSlot(OpenMote* self);
slotOffset_t schedule_getNextActiveSlotOffset(OpenMote* self);
frameLength_t schedule_getFrameLength(OpenMote* self);
cellType_t schedule_getType(OpenMote* self);
void schedule_getNeighbor(OpenMote* self, open_addr_t* addrToWrite);
channelOffset_t schedule_getChannelOffset(OpenMote* self);
bool schedule_getOkToSend(OpenMote* self);
void schedule_resetBackoff(OpenMote* self);
void schedule_indicateRx(OpenMote* self, asn_t*   asnTimestamp);
void schedule_indicateTx(OpenMote* self, 
                        asn_t*    asnTimestamp,
                        bool      succesfullTx
                   );

/**
\}
\}
*/
          
#endif
