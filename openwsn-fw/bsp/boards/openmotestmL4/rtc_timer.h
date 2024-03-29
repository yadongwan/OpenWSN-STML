/**
\brief openmoteSTM32 declaration "rtctimer" bsp module.

\author Chang Tengfei <tengfei.chang@gmail.com>,  July 2012.
*/

#ifndef __RTC_TIMER_H
#define __RTC_TIMER_H

#include "stdint.h"
#include "board.h"

//=========================== define ==========================================

//=========================== typedef =========================================

typedef void (*rtc_timer_alarm_cbt)();

//=========================== variables =======================================

//=========================== prototypes ======================================

// admin
void     rtc_timer_init();
void     rtc_timer_setAlarmCb(rtc_timer_alarm_cbt cb);
void     rtc_timer_start(uint32_t alarmValue);
// direct access
PORT_TIMER_WIDTH     rtc_timer_getAlarm();
void    rtc_timer_resetCounter();
//set alarm

// capture
PORT_TIMER_WIDTH rtc_timer_getCapturedTime();


uint8_t  rtc_timer_isr();

#endif
