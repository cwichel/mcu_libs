/**
 * @file 	libs/hardware/timer8.cpp
 * @brief 	AVR 8bit Timer library.
 */
//###############################################
#include <hardware/timer8.h>
//###############################################
#if defined(AVR_TIMER0) || defined(AVR_TIMER2)
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
#if defined(AVR_TIMER0)
TIMER8* TIMER8::_inst_0 = NULL;								//!< TIMER0 device singleton initialization.
TIMER_ISR(TIMER0_ISR_F, TIMER8::_inst_0, _timer_callback)	//!< TIMER0 ISR handler for fixed (OVF) mode.
TIMER_ISR(TIMER0_ISR_C, TIMER8::_inst_0, _timer_callback)   //!< TIMER0 ISR handler for custom (OCRA) mode.
#endif
#if defined (AVR_TIMER2)
TIMER8* TIMER8::_inst_2 = NULL;								//!< TIMER2 device singleton initialization.
TIMER_ISR(TIMER2_ISR_F, TIMER8::_inst_0, _timer_callback)   //!< TIMER2 ISR handler for fixed (OVF) mode.
TIMER_ISR(TIMER2_ISR_C, TIMER8::_inst_0, _timer_callback)   //!< TIMER2 ISR handler for custom (OCRA) mode.
#endif

//###############################################
TIMER8* TIMER8::get_instance(timer8_inst_t timer) {
	switch (timer) {
		#if defined(AVR_TIMER0)
		case TIMER_0: {
			if (!_inst_0) _inst_0 = new TIMER8(TIMER_0, &TIMER0_CTRL, &TIMER0_TCNT, &TIMER0_TIMSK);
			return _inst_0;
		} break;
		#endif
		#if defined(AVR_TIMER2)
		case TIMER_2: {
			if (!_inst_2) _inst_2 = new TIMER8(TIMER_2, &TIMER2_CTRL, &TIMER2_TCNT, &TIMER2_TIMSK);
			return _inst_2;
		} break;
		#endif
		default: return NULL; break;
	}
}

void TIMER8::delete_instance(timer8_inst_t timer) {
	switch (timer) {
		#if defined(AVR_TIMER0)
		case TIMER_0: {
			if (!_inst_0) return;
			_inst_0->stop();
			delete _inst_0;
			_inst_0 = NULL;
		} break;
		#endif
		#if defined(AVR_TIMER2)
		case TIMER_2: {
			if (!_inst_2) return;
			_inst_2->stop();
			delete _inst_2;
			_inst_2 = NULL;
		} break;
		#endif
	}
}

//===============================================
void TIMER8::init(uint32_t us, timer8_om_t mode) {
	// Do this only if the mode selected implies custom periods.
	switch (mode) {
		case T8_OM_CTC_OCRA: case T8_OM_FPWM_OCRA: case T8_OM_CPWM_P_OCRA: {
			_is_fixed = false;
			// Clear the timer configurations
			_int->raw = 0x00;
			_ctrl->ctrla.raw = 0x00;
			_ctrl->ctrlb.raw = 0x00;
			// Set Timer operation mode
			_mode = mode;
			ATOMIC({
				_ctrl->ctrlb.field.wgm2  = 0X01 & (_mode >> 4);
				_ctrl->ctrla.field.wgm01 = 0x0F & _mode;
			});
			// Set the timer prescaler and period
			set_period(us);
		} break;
		default: return; break;
	}
}

void TIMER8::init(timer8_cs_t clk_sel, timer8_om_t mode) {
	// Do this only if the mode selected implies fixed periods.
	switch (mode) {
		case T8_OM_CTC: case T8_OM_FPWM: case T8_OM_CPWM_P: {
			_is_fixed = true;
			// Clear the timer configurations
			_int->raw = 0x00;
			_ctrl->ctrla.raw = 0x00;
			_ctrl->ctrlb.raw = 0x00;
			// Set Timer operation mode
			_mode = mode;
			ATOMIC({
				_ctrl->ctrlb.field.wgm2  = 0X01 & (_mode >> 4);
				_ctrl->ctrla.field.wgm01 = 0x0F & _mode;
			});
			// Set the timer prescaler and period
			_clk_sel = clk_sel;
			_period = TIMER8_RESOLUTION - 1;
			_ctrl->ctrlb.field.cs = _clk_sel;
		} break;
		default: return; break;
	}
}

void TIMER8::set_period(uint32_t us) {
	// Only do this if the timer has custom periods.
	if (_is_fixed)
		return;
	// Get the period length
	const uint32_t cycles = ((_mode == T8_OM_CPWM_P_OCRA)? CYCLES_PER_US()/2 : CYCLES_PER_US()) * us;
	// Set the best configuration
	if (cycles < TIMER8_RESOLUTION) {
		_clk_sel = T8_CS_1;
		_period = cycles;
	} else if (cycles < (TIMER8_RESOLUTION << 3)) {
		_clk_sel = T8_CS_8;
		_period = cycles >> 3;
	} else if (cycles < (TIMER8_RESOLUTION << 6)) {
		_clk_sel = T8_CS_64;
		_period = cycles >> 6;
	} else if (cycles < (TIMER8_RESOLUTION << 8)) {
		_clk_sel = T8_CS_256;
		_period = cycles >> 8;
	} else if (cycles < (TIMER8_RESOLUTION << 10)) {
		_clk_sel = T8_CS_1024;
		_period = cycles >> 10;
	} else {
		_clk_sel = T8_CS_1024;
		_period = TIMER8_RESOLUTION - 1;
	}
	// Set
	_ctrl->ctrlb.field.cs = _clk_sel;
	_cfg->ocra = _period;
}

void TIMER8::set_period(timer8_cs_t clk_sel) {
	// Only do this if the timer has fixed periods
	if (!_is_fixed)
		return;
	// Change the timer prescaler only, the period always will be the
	// overflow value
	_clk_sel = clk_sel;
	_ctrl->ctrlb.field.cs = _clk_sel;
}

void TIMER8::int_attach(fun_ptr_vd_vd fun) {
	if (fun != NULL) {
		_int->raw = 0x00;
		_timer_callback = fun;
		if (_is_fixed)	_int->field.toie = true;
		else			_int->field.ociea = true;
	}
}

void TIMER8::int_detach(void) {
	_int->raw = 0x00;
	_timer_callback = fun_vd_vd_dummy;
}

//===============================================
void TIMER8::start(void) {
	// Clear the configuration
	_ctrl->ctrlb.raw = 0x00;
	_cfg->tcnt = 0;
	// Prepare
	resume();
}

void TIMER8::stop(void) {
	// Turn off the timer
	_ctrl->ctrlb.field.cs = T8_CS_OFF;
}

void TIMER8::resume(void) {
	// Set timer mode
	ATOMIC({
		_ctrl->ctrlb.field.wgm2  = 0X01 & (_mode >> 4);
		_ctrl->ctrla.field.wgm01 = 0x0F & _mode;
	})
	// Enable timer
	_ctrl->ctrlb.field.cs = _clk_sel;
}

void TIMER8::restart(void) {
	start();
}

//===============================================
bool TIMER8::pwm_enable(pins_t pin, uint8_t duty, timer8_com_t mode) {
	// Get PWM line
	uint8_t pwm_line = get_pwm_from_pin(pin);
	// Set operation mode
	switch (pwm_line) {
		case 'A': 	_ctrl->ctrla.field.coma = (_is_fixed) ? mode : T8_COM_OFF;	break;
		case 'B':	_ctrl->ctrla.field.comb = mode;								break;
		default:	return false;												break;
	}
	// Configure pin as output and set the PWM duty
	pin_mode(pin, OUTPUT);
	set_pwm_duty(pin, duty);
	// Ensure the timer operation
	resume();
	return true;
}

void TIMER8::pwm_disable(pins_t pin) {
	// Get PWM line
	uint8_t pwm_line = get_pwm_from_pin(pin);
	// Disable operation
	switch (pwm_line) {
		case 'A': 	_ctrl->ctrla.field.coma = T8_COM_OFF;	break;
		case 'B': 	_ctrl->ctrla.field.comb = T8_COM_OFF;	break;
		default: 	return;									break;
	}
}

void TIMER8::set_pwm_duty(pins_t pin, uint8_t duty) {
	// Get PWM line
	uint8_t  pwm_line = get_pwm_from_pin(pin);
	// Define duty value
	uint16_t cycles = _period;
	cycles *= duty;
	cycles >>= 8;
	// Set
	switch (pwm_line) {
		case 'A': 	if(_is_fixed) _cfg->ocra = cycles;	break;
		case 'B': 	_cfg->ocrb = cycles;				break;
		default: 	return;
	}
}

//###############################################
TIMER8::TIMER8(timer8_inst_t inst, volatile uint8_t* tccra, volatile uint8_t* tcnt, volatile uint8_t* timsk):
	_inst(inst), _ctrl((timer8_ctrl_t*) tccra), _cfg((timer8_cfg_t*) tcnt), _int((timer8_int_t*) timsk),
	_mode(T8_OM_FPWM), _clk_sel(T8_CS_OFF), _period(TIMER8_RESOLUTION - 1), _is_fixed(false),
	_timer_callback(fun_vd_vd_dummy) {

}

//===============================================
uint8_t TIMER8::get_pwm_from_pin(pins_t pin) {
	switch (_inst) {
		#if defined(AVR_TIMER0)
		case TIMER_0: {
			#if defined(TIMER0_PIN_A)
			if (pin == TIMER0_PIN_A)	return 'A';
			#endif
			#if defined(TIMER0_PIN_B)
			if (pin == TIMER0_PIN_B)	return 'B';
			#endif
		} break;
		#endif
		#if defined(AVR_TIMER2)
		case TIMER_2: {
			#if defined(TIMER2_PIN_A)
			if (pin == TIMER2_PIN_A)	return 'A';
			#endif
			#if defined(TIMER2_PIN_B)
			if (pin == TIMER2_PIN_B)	return 'B';
			#endif
		} break;
		#endif
	}
	return 0;
}

//###############################################
#endif
//###############################################
