/**
 * @file 	libs/device/drv2605.cpp
 * @brief	DRV2605 driver library.
 */
//###############################################
#include <device/drv2605.h>
//###############################################
#define update_field(reg, addr, name, field, value) {	\
	get_register(&reg.raw, addr);						\
	reg.name.field = value;								\
	set_register(reg.raw, addr);						\
}

//###############################################
DRV2605::DRV2605(TWI* twi): TWI::Device(twi, DRV2605_ADDR), _en_pin(NO_PIN) {
	// Start the timing functions
	timing_enable();
}

DRV2605::DRV2605(TWI* twi, pins_t en_pin): TWI::Device(twi, DRV2605_ADDR), _en_pin(en_pin) {
	// Configure the enable pin
	pin_mode(_en_pin, OUTPUT);

	// Start the timing functions
	timing_enable();
}

void DRV2605::set_defaults(void) {
	drv2605_reg_t reg;
	// Reset the device
	update_field(reg, DRV2605_REG_MODE, mode, DEV_RST, 1);
	// Give the device time to restart
	delay_micros(300);
	// Activate the module again
	set_active_mode();
}

uint8_t DRV2605::get_ID(void) {
	drv2605_reg_t reg;
	get_register(&reg.raw, DRV2605_REG_STAT);
	return reg.stat.DEV_ID;
}

//###############################################
void DRV2605::set_motor(bool is_lra, uint8_t v_rat, uint8_t odc_lamp) {
	_motor.v_rat = v_rat;
	_motor.odc_lamp = odc_lamp;
	_motor.is_LRA = is_lra;
}

void DRV2605::set_motor(bool is_lra, uint8_t v_rat, uint8_t odc_lamp,
			uint8_t acal_comp, uint8_t acal_bemf, uint8_t acal_fbctrl) {
	set_motor(is_lra, v_rat, odc_lamp);
	_motor.acal_comp = acal_comp;
	_motor.acal_BEMF = acal_bemf;
	_motor.acal_feedback = acal_fbctrl;
}

//###############################################
void DRV2605::set_active_mode(void) {
	drv2605_reg_t reg;
	// If possible, set enable HIGH
	pin_write(_en_pin, true);
	// Set mode: Active
	update_field(reg, DRV2605_REG_MODE, mode, SBY, 0);
}

void DRV2605::set_standby_mode(void) {
	drv2605_reg_t reg;
	// Set mode: Standby
	update_field(reg, DRV2605_REG_MODE, mode, SBY, 1);
	// If possible, set enable LOW
	pin_write(_en_pin, false);
}

//###############################################
void DRV2605::set_PWM_mode(void) {
	drv2605_reg_t reg;
	// Set mode as PWM/Analog input
	update_field(reg, DRV2605_REG_MODE, mode, MODE, 0x03);
}

//###############################################
void DRV2605::initialize(drv2605_bkfc_t brake_factor, drv2605_lpg_t loop_gain, uint8_t drive_time,
		bool startup_boost, bool brake_stabilizer, bool bidir_input ,
		bool open_loop, bool pwm_input) {
	drv2605_reg_t reg;
	// Set defaults
	set_defaults();
	// Set auto-calibration compensation results
	set_register(_motor.acal_comp, DRV2605_REG_ACAL_COMP);
	// Set auto-calibration BEMF results
	set_register(_motor.acal_BEMF, DRV2605_REG_ACAL_BEMF);
	// Initialize values
	initialize_values(brake_factor, loop_gain, drive_time, startup_boost,
				brake_stabilizer, bidir_input, open_loop, pwm_input);
	// Set auto-calibration BEMF gain result
	update_field(reg, DRV2605_REG_FB_CTRL, fb_ctrl, BEMF_GN, (0x03 & _motor.acal_feedback));
}

bool DRV2605::calibrate(drv2605_bkfc_t brake_factor, drv2605_lpg_t loop_gain, drv2605_acalt_t cal_time,
		uint8_t drive_time, bool startup_boost, bool brake_stabilizer, bool bidir_input ,
		bool open_loop, bool pwm_input) {
	uint8_t time = 0;
	drv2605_reg_t reg;
	// Set defaults
	set_defaults();
	// Initialize values
	initialize_values(brake_factor, loop_gain, drive_time, startup_boost,
			brake_stabilizer, bidir_input, open_loop, pwm_input);
	// Set mode
	set_register(0x07, DRV2605_REG_MODE);											// Auto-calibration mode
	// Set auto-calibration memory
	update_field(reg, DRV2605_REG_CTRL4, ctrl4, A_CAL_TM, cal_time);
	// Run auto-calibration
	update_field(reg, DRV2605_REG_GO, go, GO, 1);

	// Wait until it's ready... GO = 0
	// Maximum time: 2500[ms]
	do {
		delay_millis(10);
		get_register(&reg.raw, DRV2605_REG_GO);
		time++;
	} while (reg.go.GO && (time < 250));

	// Check for errors
	get_register(&reg.raw, DRV2605_REG_STAT);
	if (reg.stat.DIAG_RES)
		return false;

	// Get compensation, backEMF and feedback values
	get_register(&_motor.acal_comp, DRV2605_REG_ACAL_COMP);
	get_register(&_motor.acal_BEMF, DRV2605_REG_ACAL_BEMF);
	get_register(&_motor.acal_feedback, DRV2605_REG_FB_CTRL);

	// Process complete
	return true;
}

//###############################################
double DRV2605::get_LRA_period(void) {
	uint8_t lra_period;
	// Read the LRA period register (1 byte)
	get_register(&lra_period, DRV2605_REG_LRA_RES);
	// Return the LRA period:
	return 98.46 * (double)lra_period;
}

//###############################################
void DRV2605::initialize_values(drv2605_bkfc_t brake_factor, drv2605_lpg_t loop_gain, uint8_t drive_time,
		bool startup_boost, bool brake_stabilizer, bool bidir_input,
		bool open_loop, bool pwm_input) {
	drv2605_reg_t reg;
	// Set rated voltage
	set_register(_motor.v_rat, DRV2605_REG_RATEDV);
	// Set over-drive voltage
	set_register(_motor.odc_lamp, DRV2605_REG_CLAMPV);
	// Set feedback control
	get_register(&reg.raw, DRV2605_REG_FB_CTRL);
	reg.fb_ctrl.LP_GN = loop_gain;
	reg.fb_ctrl.IS_LRA = _motor.is_LRA;
	reg.fb_ctrl.BK_FCTR = brake_factor;
	set_register(reg.raw, DRV2605_REG_FB_CTRL);
	// Set control registers
	// Control 1
	get_register(&reg.raw, DRV2605_REG_CTRL1);
	reg.ctrl1.DRV_TM = drive_time;
	reg.ctrl1.ST_BST = startup_boost;
	set_register(reg.raw, DRV2605_REG_CTRL1);
	// Control 2
	get_register(&reg.raw, DRV2605_REG_CTRL2);
	reg.ctrl2.BDIR_IN = bidir_input;
	reg.ctrl2.BK_STB = brake_stabilizer;
	set_register(reg.raw, DRV2605_REG_CTRL2);
	// Control 3
	get_register(&reg.raw, DRV2605_REG_CTRL3);
	reg.ctrl3.PWM_IN = !pwm_input;
	if (_motor.is_LRA)
		reg.ctrl3.LRA_O_LP = open_loop;
	else
		reg.ctrl3.ERM_O_LP = open_loop;
	set_register(reg.raw, DRV2605_REG_CTRL3);
}

//###############################################
void DRV2605::set_register(uint8_t src, uint8_t reg) {
	write_reg(reg, src, true);
}

void DRV2605::get_register(uint8_t* rec, uint8_t reg) {
	get_registers(rec, reg, 1);
}

void DRV2605::get_registers(uint8_t* rec, uint8_t reg, size_t size) {
	read_reg(reg, rec, size);
}

//###############################################
/* NOTES:
 * 	*(1): Voltages must to be converted (see the section 8.5 on the datasheet).
 * 		In the case of the minimum voltage, this value is converted with the
 * 		same function that the rated voltage.
 *
 * 	*(2): All these values where obtained with a previous calibration process.
 */
