/**
 * @code	#include "device/drv2605.h" @endcode
 * @file 	libs/device/drv2605.h
 * @brief	DRV2605 driver library.
 */
//###############################################
#ifndef _DEVICES_DRV2605_H_
#define _DEVICES_DRV2605_H_
//###############################################
#include <device/drv2605_defs.h>

//###############################################
/**
 * @class 	DRV2605
 * @brief	DRV2605 diver class
 */
class DRV2605: protected TWI::Device {
public:
	/**
	 * @brief	DRV2650 constructor (for devices with no access to the
	 * 			enable pin).
	 * @param	twi			TWI bus manager.
	 */
	DRV2605(TWI* twi);

	/**
	 * @brief	DRV2650 constructor.
	 * @param	twi			TWI bus manager.
	 * @param	en_pin		Enable pin.
	 */
	DRV2605(TWI* twi, pins_t en_pin);

	/**
	 * @brief	Set the registers to the default values (reset the device).
	 */
	void set_defaults(void);

	/**
	 * @brief	Get the device ID. This can be used to check if the I2C comms
	 * 			are working.
	 * @return	Device ID code (see datasheet).
	 */
	uint8_t get_ID(void);

	//======================================
	/**
	 * @brief	Set the motor parameters. Need to execute the auto-calibration
	 * 			afterwards.
	 * @param	is_lra		Motor type (LRA or ERM).
	 * @param	v_rat		Motor rated voltage.
	 * @param	odc_lamp	Motor over-drive voltage.
	 */
	void set_motor(bool is_lra, uint8_t v_rat, uint8_t odc_lamp);

	/**
	 * @brief	Set the motor parameters and a previous auto-calibration results.
	 * @param	is_lra		Motor type (LRA or ERM).
	 * @param	v_rat		Motor rated voltage.
	 * @param	odc_lamp	Motor over-drive voltage.
	 * @param	acal_comp	Auto-calibration compensation result.
	 * @param	acal_bemf	Auto-calibration back-EMF result.
	 * @param	acal_fbctrl	Auto-calibration back-EMF gain (feedback control register).
	 */
	void set_motor(bool is_lra, uint8_t v_rat, uint8_t odc_lamp,
			uint8_t acal_comp, uint8_t acal_bemf, uint8_t acal_fbctrl);

	//======================================
	/**
	 * @brief	Enable the DRV2605 driver.
	 */
	void set_active_mode(void);

	/**
	 * @brief	Disable the DRV2605 driver.
	 */
	void set_standby_mode(void);

	//======================================
	/**
	 * @brief	Configure the driver to work on PWM mode.
	 */
	void set_PWM_mode(void);

	//======================================
	/**
	 * @brief	Initialize the DRV2605 with previous auto-calibration results.
	 * 			See DRV2605::set_motor(bool, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t).
	 * @copydetails DRV2605::initialize_values(drv2605_bkfc_t, drv2605_lpg_t, uint8_t, bool, bool, bool, bool, bool)
	 */
	void initialize(drv2605_bkfc_t brake_factor, drv2605_lpg_t loop_gain, uint8_t drive_time,
			bool startup_boost, bool brake_stabilizer, bool bidir_input,
			bool open_loop, bool pwm_input);
	/**
	 * @brief	Perform an auto-calibration process on the DRV2605.
	 * @param	cal_time	Time used to perform the auto-calibration process.
	 * @copydetails DRV2605::initialize_values(drv2605_bkfc_t, drv2605_lpg_t, uint8_t, bool, bool, bool, bool, bool)
	 * @return	True if calibration process was succesful, False otherwise.
	 */
	bool calibrate(drv2605_bkfc_t brake_factor, drv2605_lpg_t loop_gain, drv2605_acalt_t cal_time,
			uint8_t drive_time, bool startup_boost, bool brake_stabilizer, bool bidir_input,
			bool open_loop, bool pwm_input);

	//======================================
	/**
	 * @brief	Get the LRA motor period value.
	 * @return	LRA period.
	 */
	double get_LRA_period(void);

private:
	/**
	 * @struct	_motor
	 * @brief	Motor parameters.
	 */
	struct {
		uint8_t v_rat;			//!< Rated voltage register value.
		uint8_t odc_lamp;		//!< OC Clamp voltage register value.
		uint8_t	acal_comp;		//!< Auto-calibration compensation result.
		uint8_t acal_BEMF;		//!< Auto-calibration back-EMF value.
		uint8_t acal_feedback;	//!< Auto-calibration feedback control register value.
		bool	is_LRA;			//!< Motor type.
	} _motor;

	/** Enable pin */
	pins_t _en_pin;

	//======================================
	/**
	 * @brief	Initialize the DRV2605.
	 * @param	brake_factor		Set the gain ratio between the braking gain and the driving gain.
	 * @param	loop_gain			Set how fast the driver attempts to make the BEMF match the input signal.
	 * @param	drive_time			Set the sample rate for the BEMF detection.
	 * @param	startup_boost		Define if startup boost is active: Active(true), Inactive(false).
	 * @param	brake_stabilizer	Define if brake stabilizer is active: Active(true), Inactive(false).
	 * @param	bidir_input			Define how the engine interprets data: bidirectional(true), unidirectional(false).
	 * @param	open_loop			Define the operation mode: open-loop(true), closed-loop(false).
	 * @param	pwm_input			Define if the input type: PWM(true), Analog(false).
	 */
	void initialize_values(drv2605_bkfc_t brake_factor, drv2605_lpg_t loop_gain, uint8_t drive_time,
			bool startup_boost, bool brake_stabilizer, bool bidir_input, bool open_loop,
			bool pwm_input);

	//======================================
	/**
	 * @brief	Write a register "reg" with the value given by "src".
	 * 			handler to set a single register.
	 * @param	src			Source byte.
	 * @param	reg			Device register address.
	 */
	void set_register(uint8_t src, uint8_t reg);

	/**
	 * @brief	DRV2605::get_register(uint8_t*, uint8_t, uint8_t)
	 * 			handler to get a single byte.
	 * @param	rec			Receiver byte.
	 * @param	reg			Device register address.
	 */
	void get_register(uint8_t* rec, uint8_t reg);

	/**
	 * @brief	read a "size" number of registers on "rec" starting
	 * 			from "reg" register.
	 * @param	rec			Pointer to received byte array.
	 * @param	reg			Device register address.
	 * @param	size		Number of bytes to read.
	 */
	void get_registers(uint8_t* rec, uint8_t reg, size_t size);

};

//###############################################
#endif /* _DEVICES_DRV2605_H_ */
//###############################################
