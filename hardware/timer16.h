/**
 * @code 	#include "hardware/timer16.h" @endcode
 * @file	libs/hardware/timer16.h
 * @brief	AVR 16bit Timer library.
 */
//###############################################
#ifndef _HARDWARE_TIMER16_H_
#define _HARDWARE_TIMER16_H_
//###############################################
#include <hardware/timer16_defs.h>
//###############################################
#if defined(AVR_TIMER1)
ISR(TIMER1_ISR);					//!< Declare this to simplify the TIMER1 custom (ICR) ISR declaration.
#endif
#if defined(AVR_TIMER1)
ISR(TIMER3_ISR);					//!< Declare this to simplify the TIMER3 custom (ICR) ISR declaration.
#endif

//###############################################
/**
 * @class	TIMER16
 * @brief	16bit Timer general implementation.
 */
class TIMER16 {
public:
	//======================================
	/**
	 * @brief	Get an instance for an specific device.
	 * @param	timer		TIMER16 device.
	 * @return	Device singleton.
	 */
	static TIMER16* get_instance(timer16_inst_t timer);

	/**
	 * @brief	Deletes the instance of an specific device. Note that this
	 * 			action will turn off the interface and delete all the
	 * 			configurations.
	 * @param	timer		TIMER16 device.
	 */
	static void delete_instance(timer16_inst_t timer);

	//======================================
	/**
	 * @brief 	Initialize the timer with a given period.
	 * @param	us			Timer period on [us].
	 * @param	mode		Timer waveform generation mode.
	 */
	void init(uint32_t us=1000000UL, timer16_om_t mode=T16_OM_CPWM_PF);

	/**
	 * @brief	Configure the timer period.
	 * @param	us			Timer period on [us].
	 */
	void set_period(uint32_t us);

	/**
	 * @brief	Attach a given function to be executed every time the configured
	 * 			period is completed.
	 * @param	fun			Function to be executed.
	 */
	void int_attach(fun_ptr_vd_vd fun);

	/**
	 * @brief	Detach the current function from the timer interruption.
	 */
	void int_detach(void);

	//======================================
	/**
	 * @brief	Start the timer.
	 */
	void start(void);

	/**
	 * @brief	Stop the timer.
	 */
	void stop(void);

	/**
	 * @brief	Resume the timer.
	 */
	void resume(void);

	/**
	 * @brief	Restart the timer.
	 */
	void restart(void);

	//======================================
	/**
	 * @brief	Enable the PWM function on the given pin.
	 * @param	pin			Board pin value.
	 * @param	duty		PWM duty cycle. The value goes from 0 to 1024.
	 * @param	mode		Output signal behavior.
	 */
	bool pwm_enable(pins_t pin, uint16_t duty, timer16_com_t mode=T16_COM_PWM);

	/**
	 * @brief	Disable the PWM signal on the given pin.
	 * @param	pin			Board pin value.
	 */
	void pwm_disable(pins_t pin);

	/**
	 * @brief	Set the PWM period.
	 * @param	pin			Board pin value.
	 * @param	duty		PWM duty cycle. The value goes from 0 to 1024.
	 */
	void set_pwm_duty(pins_t pin, uint16_t duty);

protected:
	//======================================
	#if defined(AVR_TIMER1)
	static TIMER16*	_inst_1;			//!< TIMER3 device Singleton.
	friend void TIMER1_ISR();			//!< Used to make available private TIMER1 methods on the custom (ICR) ISR.
	#endif
	#if defined(AVR_TIMER3)
	static TIMER16*	_inst_3;			//!< TIMER3 device Singleton.
	friend void TIMER3_ISR();			//!< Used to make available private TIMER3 methods on the custom (ICR) ISR.
	#endif

	//======================================
	timer16_inst_t	_inst;				//!< TIMER8 instance ID.
	timer16_ctrl_t*	_ctrl;				//!< TIMER16 control registers.
	timer16_cfg_t*	_cfg;               //!< TIMER16 counter registers.
	timer16_int_t*	_int;               //!< TIMER16 interruption configuration register.
	timer16_om_t	_mode;				//!< Timer waveform generation mode.
	timer16_cs_t	_clk_sel;      		//!< Timer prescaler.
	uint16_t		_period;        	//!< Timer period.
	fun_ptr_vd_vd	_timer_callback;    //!< Timer ISR callback.

	//======================================
	/**
	 * @brief	TIMER16 constructor.
	 * @param	inst		Timer instance ID.
	 * @param	tccra		Control register A address.
	 * @param	tcnt		Timer counter register address.
	 * @param	timsk		Timer interruption register address.
	 */
	inline TIMER16(timer16_inst_t inst, volatile uint8_t* tccra, volatile uint16_t* tcnt, volatile uint8_t* timsk);

	//======================================
	/**
	 * @brief	Get the PWM channel for a given pin.
	 * @param	pin			Board pin value.
	 */
	inline uint8_t get_pwm_from_pin(pins_t pin);

};

//###############################################
typedef TIMER16 T16;					//!< Alternative timer8 type.

//###############################################
#endif /* _HARDWARE_TIMER16_H_ */
//###############################################
