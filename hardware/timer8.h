/**
 * @code 	#include "hardware/timer8.h" @endcode
 * @file	libs/hardware/timer8.h
 * @brief	AVR 8bit Timer library.
 */
//###############################################
#ifndef _HARDWARE_TIMER8_H_
#define _HARDWARE_TIMER8_H_
//###############################################
#include <hardware/timer8_defs.h>
//###############################################
#if defined(AVR_TIMER0)
ISR(TIMER0_ISR_F);						//!< Declare this to simplify the TIMER0 fixed (OVF) ISR declaration.
ISR(TIMER0_ISR_C);						//!< Declare this to simplify the TIMER0 custom (OCRA) ISR declaration.
#endif
#if defined(AVR_TIMER2)
ISR(TIMER2_ISR_F);						//!< Declare this to simplify the TIMER2 fixed (OVF) ISR declaration.
ISR(TIMER2_ISR_C);						//!< Declare this to simplify the TIMER2 custom (OCRA) ISR declaration.
#endif

//###############################################
/**
 * @class	TIMER8
 * @brief	8bit Timer general implementation.
 */
class TIMER8 {
public:
	//======================================
	/**
	 * @brief	Get an instance for an specific device.
	 * @param	timer		TIMER8 device.
	 * @return	Device singleton.
	 */
	static TIMER8* get_instance(timer8_inst_t timer);

	/**
	 * @brief	Deletes the instance of an specific device. Note that this
	 * 			action will turn off the interface and delete all the
	 * 			configurations.
	 * @param	timer		TIMER8 device.
	 */
	static void delete_instance(timer8_inst_t timer);

	//======================================
	/**
	 * @brief 	Initialize the timer with a custom period. This method
	 * 			forces the timer to uses OCRA as reference for the period
	 * 			settings. With this configuration only OCRB can be used as
	 * 			output.
	 * @param	us			Timer period on [us].
	 * @param	mode		Timer waveform generation mode.
	 */
	void init(uint32_t us=1000, timer8_om_t mode=T8_OM_FPWM_OCRA);

	/**
	 * @brief	Initialize the timer with a fixed period given by the
	 * 			overflow with a given prescaler. With this configuration
	 * 			the possible time periods are less, but both OCRx can be
	 * 			used as PWM.
	 * @param	clk_sel		Timer prescaler.
	 * @param	mode		Timer waveform generation mode.
	 */
	void init(timer8_cs_t clk_sel=T8_CS_64, timer8_om_t mode=T8_OM_FPWM);

	/**
	 * @brief	Configure the timer period (for custom mode).
	 * @param	us			Timer period on [us].
	 */
	void set_period(uint32_t us);

	/**
	 * @brief	Configure the timer period (for fixed mode).
	 * @param	clk_sel		Timer prescaler.
	 */
	void set_period(timer8_cs_t clk_sel);

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
	bool pwm_enable(pins_t pin, uint8_t duty, timer8_com_t mode=T8_COM_PWM);

	/**
	 * @brief	Disable the PWM signal on the given pin.
	 * @param	pin			Board pin value.
	 */
	void pwm_disable(pins_t pin);

	/**
	 * @brief	Set the PWM period.
	 * @param	pin			Board pin value.
	 * @param	duty		PWM duty cycle. The value goes from 0 to 255.
	 */
	void set_pwm_duty(pins_t pin, uint8_t duty);

private:
	//======================================
	#if defined(AVR_TIMER0)
	static TIMER8*	_inst_0;			//!< TIMER0 device Singleton.
	friend void TIMER0_ISR_F();			//!< Used to make available private TIMER0 methods on the fixed (OVF) ISR.
	friend void TIMER0_ISR_C();			//!< Used to make available private TIMER0 methods on the custom (OCRA) ISR.
	#endif
	#if defined(AVR_TIMER2)
	static TIMER8*	_inst_2;			//!< TIMER2 device Singleton.
	friend void TIMER2_ISR_F();			//!< Used to make available private TIMER2 methods on the fixed (OVF) ISR.
	friend void TIMER2_ISR_C();			//!< Used to make available private TIMER2 methods on the custom (OCRA) ISR.
	#endif

	//======================================
	timer8_inst_t	_inst;				//!< TIMER8 instance ID.
	timer8_ctrl_t*	_ctrl;				//!< TIMER8 control registers.
	timer8_cfg_t*	_cfg;               //!< TIMER8 counter registers.
	timer8_int_t*	_int;               //!< TIMER8 interruption configuration register.
	timer8_om_t		_mode;              //!< Timer waveform generation mode.
	timer8_cs_t		_clk_sel;     		//!< Timer prescaler.
	uint8_t			_period;            //!< Timer period.
	bool			_is_fixed;			//!< Shows if the timer uses OCRA to define the period.
	fun_ptr_vd_vd	_timer_callback;    //!< Timer ISR callback.

	//======================================
	/**
	 * @brief	TIMER8 constructor.
	 * @param	inst		Timer instance ID.
	 * @param	tccra		Control register A address.
	 * @param	tcnt		Timer counter register address.
	 * @param	timsk		Timer interruption register address.
	 */
	inline TIMER8(timer8_inst_t inst, volatile uint8_t* tccra, volatile uint8_t* tcnt, volatile uint8_t* timsk);

	//======================================
	/**
	 * @brief	Get the PWM channel for a given pin.
	 * @param	pin			Board pin value.
	 */
	inline uint8_t get_pwm_from_pin(pins_t pin);
};

//###############################################
typedef TIMER8 T8;						//!< Alternative timer8 type.

//###############################################
#endif /* _HARDWARE_TIMER8_H_ */
//###############################################
