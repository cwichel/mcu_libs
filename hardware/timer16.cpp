/**
 * @file 	libs/hardware/timer16.cpp
 * @brief 	AVR 16bit Timer library.
 */
//###############################################
#include <hardware/timer16.h>
//###############################################
#if defined(AVR_TIMER1) || defined(AVR_TIMER3)
//###############################################
/**
 * @brief	Used to simplify the 16bit TIMER ISR definition.
 * @param	vect			ISR vector.
 * @param	timer			16bit TIMER object.
 * @param	fun				Timer Overflow Callback.
 */
#define TIMER_ISR(vect, inst, fun)	\
	ISR(vect) {						\
		inst->fun();				\
	}

//###############################################
#if defined(AVR_TIMER1)
TIMER16* TIMER16::_inst_1 = NULL;							//!< TIMER1 device singleton initialization.
TIMER_ISR(TIMER1_ISR, TIMER16::_inst_1, _timer_callback)    //!< TIMER1 ISR handler for custom (ICR) mode.
#endif
#if defined(AVR_TIMER3)
TIMER16* TIMER16::_inst_3 = NULL;							//!< TIMER3 device singleton initialization.
TIMER_ISR(TIMER3_ISR, TIMER16::_inst_3, _timer_callback)    //!< TIMER3 ISR handler for custom (ICR) mode.
#endif

//###############################################
TIMER16* TIMER16::get_instance(timer16_inst_t timer) {
	switch (timer) {
		#if defined(AVR_TIMER1)
		case TIMER_1: {
			if (!_inst_1) _inst_1 = new TIMER16(TIMER_1, &TIMER1_CTRL, &TIMER1_TCNT, &TIMER1_TIMSK);
			return _inst_1;
		} break;
		#endif
		#if defined(AVR_TIMER3)
		case TIMER_3: {
			if (!_inst_3) _inst_3 = new TIMER16(TIMER_3, &TIMER3_CTRL, &TIMER3_TCNT, &TIMER3_TIMSK);
			return _inst_3;
		} break;
		#endif
		default: return NULL; break;
	}
}

void TIMER16::delete_instance(timer16_inst_t timer) {
	switch (timer) {
		#if defined(AVR_TIMER1)
		case TIMER_1: {
			if (!_inst_1) return;
			_inst_1->stop();
			delete _inst_1;
			_inst_1 = NULL;
		} break;
		#endif
		#if defined(AVR_TIMER3)
		case TIMER_3: {
			if (!_inst_3) return;
			_inst_3->stop();
			delete _inst_3;
			_inst_3 = NULL;
		} break;
		#endif
	}
}

//===============================================
void TIMER16::init(uint32_t us, timer16_om_t mode) {
	// Clear Interruptions and configurations
	_int->raw = 0x00;
	_ctrl->ctrla.raw = 0x00;
	_ctrl->ctrlb.raw = 0x00;
	// Set Timer Operation Mode
	_mode = mode;
	ATOMIC({
		_ctrl->ctrlb.field.wgm23 = 0x0F & (_mode >> 4);
		_ctrl->ctrla.field.wgm01 = 0x0F & _mode;
	});
	set_period(us);
}

void TIMER16::set_period(uint32_t us) {
	// Get the period length
	const uint32_t cycles = ((_mode == T16_OM_CPWM_PF)? CYCLES_PER_US()/2 : CYCLES_PER_US()) * us;
	// Set the best configuration
	if (cycles < TIMER16_RESOLUTION) {
		_clk_sel = T16_CS_1;
		_period = cycles;
	} else if (cycles < (TIMER16_RESOLUTION << 3)) {
		_clk_sel = T16_CS_8;
		_period = cycles >> 3;
	} else if (cycles < (TIMER16_RESOLUTION << 6)) {
		_clk_sel = T16_CS_64;
		_period = cycles >> 6;
	} else if (cycles < (TIMER16_RESOLUTION << 8)) {
		_clk_sel = T16_CS_256;
		_period = cycles >> 8;
	} else if (cycles < (TIMER16_RESOLUTION << 10)) {
		_clk_sel = T16_CS_1024;
		_period = cycles >> 10;
	} else {
		_clk_sel = T16_CS_1024;
		_period = TIMER16_RESOLUTION - 1;
	}
	// Set
	_ctrl->ctrlb.field.cs = _clk_sel;
	_cfg->icr = _period;
}

void TIMER16::int_attach(fun_ptr_vd_vd fun) {
	if (fun != NULL) {
		_timer_callback = fun;
		_int->field.icie = true;
	}
}

void TIMER16::int_detach(void) {
	_int->raw = 0x00;
	_timer_callback = fun_vd_vd_dummy;
}

//===============================================
void TIMER16::start(void) {
	// Clear the configuration
	_ctrl->ctrlb.raw = 0x00;
	_cfg->tcnt = 0;
	// Prepare
	resume();
}

void TIMER16::stop(void) {
	// Turn off the timer
	_ctrl->ctrlb.field.cs = T16_CS_OFF;
}

void TIMER16::resume(void) {
	// Set Timer Mode
	ATOMIC({
		_ctrl->ctrlb.field.wgm23 = 0x0F & (_mode >> 4);
		_ctrl->ctrla.field.wgm01 = 0x0F & _mode;
	});
	// Enable timer.
	_ctrl->ctrlb.field.cs = _clk_sel;
}

void TIMER16::restart(void) {
	start();
}

//===============================================
bool TIMER16::pwm_enable(pins_t pin, uint16_t duty, timer16_com_t mode) {
	// Get PWM line
	uint8_t pwm_line = get_pwm_from_pin(pin);
	// Set operation mode
	switch (pwm_line) {
		case 'A': 	_ctrl->ctrla.field.coma = mode;	break;
		case 'B': 	_ctrl->ctrla.field.comb = mode;	break;
		case 'C': 	_ctrl->ctrla.field.comc = mode;	break;
		default: 	return false;					break;
	}
	// Configure pin as output and set the PWM duty
	pin_mode(pin, OUTPUT);
	set_pwm_duty(pin, duty);
	// Ensure the timer operation
	resume();
	return true;
}

void TIMER16::pwm_disable(pins_t pin) {
	// Get PWM line
	uint8_t pwm_line = get_pwm_from_pin(pin);
	// Disable operation
	switch (pwm_line) {
		case 'A': 	_ctrl->ctrla.field.coma = T16_COM_OFF;	break;
		case 'B': 	_ctrl->ctrla.field.comb = T16_COM_OFF;	break;
		case 'C': 	_ctrl->ctrla.field.comc = T16_COM_OFF;	break;
		default: 	return;									break;
	}
}

void TIMER16::set_pwm_duty(pins_t pin, uint16_t duty) {
	// Get PWM line
	uint8_t  pwm_line = get_pwm_from_pin(pin);
	// Define duty value
	uint32_t cycles = _period;
	cycles *= duty;
	cycles >>= 10;
	// Set
	switch (pwm_line) {
		case 'A': 	_cfg->ocra = cycles;	break;
		case 'B': 	_cfg->ocrb = cycles;	break;
		case 'C': 	_cfg->ocrc = cycles;	break;
		default: 	return;					break;
	}
}

//###############################################
TIMER16::TIMER16(timer16_inst_t inst, volatile uint8_t* tccra, volatile uint16_t* tcnt, volatile uint8_t* timsk):
	_inst(inst), _ctrl((timer16_ctrl_t*) tccra), _cfg((timer16_cfg_t*) tcnt), _int((timer16_int_t*) timsk),
	_mode(T16_OM_FPWM), _clk_sel(T16_CS_OFF), _period(TIMER16_RESOLUTION-1),
	_timer_callback(fun_vd_vd_dummy) {

}

//===============================================
uint8_t TIMER16::get_pwm_from_pin(pins_t pin) {
	switch (_inst) {
		#if defined(AVR_TIMER1)
		case TIMER_1: {
			#if defined(TIMER1_PIN_A)
			if (pin == TIMER1_PIN_A)	return 'A';
			#endif
			#if defined(TIMER1_PIN_B)
			if (pin == TIMER1_PIN_B)	return 'B';
			#endif
			#if defined(TIMER1_PIN_C)
			if (pin == TIMER1_PIN_C)	return 'C';
			#endif
		} break;
		#endif
		#if defined(AVR_TIMER3)
		case TIMER_3: {
			#if defined(TIMER3_PIN_A)
			if (pin == TIMER3_PIN_A)	return 'A';
			#endif
			#if defined(TIMER3_PIN_B)
			if (pin == TIMER3_PIN_B)	return 'B';
			#endif
			#if defined(TIMER3_PIN_C)
			if (pin == TIMER3_PIN_C)	return 'C';
			#endif
		} break;
		#endif
	}
	return 0;
}

//###############################################
#endif
//###############################################

