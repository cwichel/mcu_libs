/**
 * @code 	#include "hardware/timing.h" @endcode
 * @file 	libs/hardware/timing.cpp
 * @brief 	AVR general program timing functions.
 */
//###############################################
#ifndef _HARDWARE_TIMING_H_
#define _HARDWARE_TIMING_H_
//###############################################
#include <hardware/timer8.h>
//###############################################
/**
 * @brief	Initializes the timing functions.
 */
extern void timing_enable(void);

/**
 * @brief	Busy-delay implementation to delay in [ms].
 * @param	ms				Time in [ms] to perform the delay.
 */
extern void delay_millis(uint32_t ms);

/**
 * @brief	Busy-delay implementation to delay in [us]. Maximum
 * 			delay value recommended: 16[ms].
 * @param	us				Time in [us] to perform the delay.
 */
extern void delay_micros(uint16_t us);

/**
 * @brief	Get the value of the general timer.
 * @return	Time [ms].
 */
extern uint32_t get_millis(void);

/**
 * @brief	Get the value of the general timer.
 * @return	Time [us].
 */
extern uint32_t get_micros(void);

//###############################################
#endif /* _HARDWARE_TIMING_H_ */
//###############################################
