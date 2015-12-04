#ifndef _TIMER_H_
#define _TIMER_H_

/* 3.25 MHz */
#define OS_CLOCK					3250000
/* invoke timer interrupt every 10 ms */
#define INTERVAL					10
/* OS match register value */
#define MATCH_COUNT				OS_CLOCK * INTERVAL / 1000
/* since reading from the os count register is not atomic, typically
 * it takes a LDR, a SUB, and a STR, which add up to 2 clock cycles */
#define EXECUTION_OFFSET	2

unsigned long _time;

#endif