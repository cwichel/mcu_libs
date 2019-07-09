/**
 * @code	#include "device/lis3dh.h" @endcode
 * @file 	libs/device/lis3dh.h
 * @brief	LIS3DH driver library.
 */
//###############################################
#ifndef _DEVICES_LIS3DH_H_
#define _DEVICES_LIS3DH_H_
//###############################################
#include <device/lis3dh_defs.h>

//###############################################
/**
 * @class	LIS3DH
 * @brief	LIS3DH driver class.
 */
class LIS3DH: protected SPI::Device, protected TWI::Device {
public:
	/**
	 * @brief	LIS3DH constructor. For TWI mode.
	 * @param	twi			TWI bus manager.
	 * @param	addr		Device address (SDO pin dependent).
	 */
	LIS3DH(TWI* twi, uint8_t addr);

	/**
	 * @brief	LIS3DH constructor. For SPI mode.
	 * @param	spi			SPI bus manager.
	 * @param	ss_pin		Device SS pin.
	 */
	LIS3DH(SPI* spi, pins_t ss_pin);

	//======================================
	/**
	 * @brief	Check if the device is available.
	 * @return 	If the device is available true, else false.
	 */
	bool is_available(void);

	/**
	 * @brief	Initialize the device.
	 * @return	If was possible to initialize the device.
	 */
	bool enable(void);

	/**
	 * @brief	Set all the configuration registers to the default values.
	 */
	void set_defaults(void);

	/**
	 * @brief	Set sensor mode.
	 * @param	odr			Sensor Output Data Rate (ODR).
	 * @param	res			Sensor resolution.
	 * @param	bdu			Set block data update.
	 * @param	x_en		Enable X-axis.
	 * @param	y_en		Enable Y-axis.
	 * @param	z_en		Enable Z-axis.
	 */
	void set_mode(lis3dh_odr_t odr, lis3dh_res_t res, lis3dh_bdu_t bdu, bool x_en, bool y_en, bool z_en);

	/**
	 * @brief	Set sensor scale.
	 * @param	scale		Full range scale.
	 */
	void set_scale(lis3dh_scale_t scale);

	/**
	 * @brief	Get the sensor scale.
	 * @return	Full range scale.
	 */
	lis3dh_scale_t get_scale(void);

	/**
	 * @brief	Get the sensor resolution.
	 * @return	Sensor resolution.
	 */
	lis3dh_res_t get_resolution(void);

	/**
	 * @brief	Get if data is available.
	 * @return	True if is data available, false otherwise.
	 */
	bool is_data_available(void);

	//======================================
	/**
	 * @brief	Read the acceleration value.
	 * @param	accel		Acceleration values (in [mg]).
	 */
	void get_accel(lis3dh_accel_t* accel);

	//======================================
	/**
	 * @brief	Enable/Disable the ADC lines or temperature sensor.
	 * @param	adc			Enable ADC.
	 * @param	temp		Enable temperature sensor on ADC line 3.
	 */
	void adc_enable(bool adc, bool temp);

	/**
	 * @brief	Read all the ADC lines.
	 * @param	adc			ADC values.
	 */
	void get_adc(lis3dh_adc_t* adc);

	//======================================
	/**
	 * @brief	Enable the FIFO with a given mode. In the case that
	 * 			"stream to fifo" mode is configured, the interruption
	 * 			source has to be defined.
	 * @param	mode		FIFO mode.
	 * @param	wtm			Watermark level.
	 * @param	wtm_int		If a watermark interruption is generated.
	 * @param	ovr_int		If an overrun interrupt is generated.
	 * @param	tr			In case of "stream to fifo" mode is defined, this
	 * 						allow to select the interruption source: INT1(false),
	 * 						INT2(true).
	 */
	void fifo_enable(lis3dh_fifo_t mode, uint8_t wtm, bool wtm_int=true, bool ovr_int=false, bool tr=false);

	/**
	 * @brief	Disable the FIFO mode.
	 */
	void fifo_disable(void);

	/**
	 * @brief	Get the FIFO state.
	 * @return	FIFO source register value.
	 */
	lis3dh_reg_fifo_src_t fifo_get_state(void);

	/**
	 * @brief	Read bulk data from FIFO buffer.
	 * @param	data		Pointer to accel data array.
	 * @param	len			Number of samples to retrieve.
	 * @return	FIFO source register value.
	 */
	lis3dh_reg_fifo_src_t fifo_get_data(lis3dh_accel_t* data, uint8_t len);

	//======================================
	/**
	 * @brief	Set the interruption polarity.
	 * @param	pol			Polarity. On interrupt low(true), high(false).
	 */
	void int_polarity(bool pol=false);

	/**
	 * @brief	Set the interrupt general parameters on signal INT1 or INT2.
	 * @param	signal		Interrupt signal.
	 * @param	ths			Interrupt threshold.
	 * @param	dur			Interrupt duration.
	 * @param	latch		Define if it's necessary to read the INTx source register
	 * 						to stop the interrupt.
	 */
	void int_set_parameters(lis3dh_int_t signal, uint8_t ths, uint8_t dur, bool latch);

	/**
	 * @brief	Configure an interrupt on signal INT1 or INT2.
	 * @param	signal		Interrupt signal.
	 * @param	type		Interrupt to be enabled/disabled.
	 * @param	enable		Enable flag.
	 */
	void int_data_enable(lis3dh_int_t signal, lis3dh_int_type_t type, bool enable);
	/**
	 * @brief	Get the source of the interruption (FIFO or data ready) for
	 * 			signal INT1.
	 * @param	source		Pointer to interrupt source flags.
	 */
	void get_int_data_src(lis3dh_int_src_t* source);

	/**
	 * @brief	Disable interrupts on signal INT1 or INT2.
	 * @param	signal		Interrupt signal.
	 */
	void int_data_disable(lis3dh_int_t signal);

	/**
	 * @brief	Set the configuration of an inertial event interrupt generator
	 * 			on signal INT1 or INT2. The interrupt occurs when the acceleration
	 * 			of the defined axis is lower than a defined threshold and one
	 * 			defined event occurs.
	 * @param	signal		Interrupt signal.
	 * @param	event		Inertial event.
	 * @param	axis_mask	The axis (LO or HI) selected to trigger the event.
	 * 						The mask is defined by the 6 lsb bits and starts from
	 * 						X_LOW.
	 */
	void int_event_enable(lis3dh_int_t signal, lis3dh_int_evt_t event, uint8_t axis_mask);

	/**
	 * @brief	Get the source of the inertial event for signal INT1/INT2.
	 * @param	signal		Interrupt signal.
	 * @param	source		Pointer to interrupt source register data.
	 */
	void get_int_event_src(lis3dh_int_t signal, lis3dh_reg_intx_src_t* source);

	/**
	 * @brief	Disable the configuration of an inertial event on signal INT1 or INT2.
	 * @param	signal		Interrupt signal.
	 */
	void int_event_disable(lis3dh_int_t signal);

	//======================================
	/**
	 * @brief	Enable the click driven interruption with the given
	 * 			configuration.
	 * @param	axis_mask	The axis selected to trigger the click
	 * 						interruption event. The mask is defined by the
	 * 						3 lsb bits and start from the X axis.
	 * @param	sclick		Enable single click detection on mask.
	 * @param	dclick		Enable double click detection on mask.
	 * @param	ths			Click interrupt threshold.
	 * @param	tlim		Click time limit. Maximum time that the signal can
	 * 						be over the threshold to be considered a click.
	 * @param	tlat		Click time latency. Time that stops the detection
	 * 						after a click (avoid false-positives because bounce).
	 * @param	twin		Click time window. Time after the latency in which a
	 * 						second click is expected to occur (begin of).
	 * @param	latch		Define if it's necessary to read the click source
	 * 						register to stop the interrupt.
	 */
	void click_enable(uint8_t axis_mask, bool sclick, bool dclick, uint8_t ths,
			uint8_t tlim, uint8_t tlat, uint8_t twin, bool latch);

	/**
	 * @brief	Disable the click driven interruption.
	 */
	void click_disable(void);

	/**
	 * @brief	Read the click source register.
	 * @param	source		Pointer to click source register data.
	 */
	void get_click_src(lis3dh_reg_click_src_t* source);

	//======================================
	/**
	 * @brief	Enable the activity dependent power mode.
	 * @param	ths			Sleep-Awake, Return-to-Sleep activation threshold.
	 * @param	dur			Sleep-Awake, Return-to-Sleep duration.
	 */
	void activity_enable(uint8_t ths, uint8_t dur);

	/**
	 * @brief	Disable the activity dependent power mode.
	 */
	void activity_disable(void);

	//======================================
	/**
	 * @brief	Configure the high-pass filter.
	 * @param	mode		High-pass filter mode.
	 * @param	cf			High-pass filter cut frequency.
	 * @param	fds			Filtered data selection: Internal filter bypassed(false), internal filter(true).
	 * @param	click		Filter enabled for click.
	 * @param	ia1			Filter enabled for event on INT1.
	 * @param	ia2			Filter enabled for event on INT2.
	 */
	void hpf_configure(lis3dh_hpm_t mode, lis3dh_hpcf_t cf, bool fds, bool click, bool ia1, bool ia2);

	/**
	 * @brief	Reset to default the filter configurations.
	 */
	void hpd_reset(void);

	/**
	 * @brief	Get the reference source register.
	 * @param	source		Pointer to reference source register data.
	 */
	void get_reference(uint8_t* source);

private:
	bool 		_is_spi;	//!< TWI/SPI selector.
	lis3dh_t	_config;	//!< Sensor configuration.

	//======================================
	/**
	 * @brief	Get the compensation value needed to adjust the accelerometer
	 * 			values in dependence with the resolution and power mode of the
	 * 			device.
	 * @return	Accelerometer conversion value (digit to [mg]).
	 */
	uint8_t get_accel_conversion(void);

	//======================================
	/**
	 * @brief	Write "src" to "reg" register.
	 * @param	src			Source byte.
	 * @param	reg			Device register address.
	 */
	void set_register(uint8_t src, uint8_t reg);

	/**
	 * @brief	LIS3DH::get_register(uint8_t*, uint8_t, uint8_t)
	 * 			handler to get a single byte.
	 * @param	rec			Receiver byte.
	 * @param	reg			Device register address.
	 */
	void get_register(uint8_t* rec, uint8_t reg);

	/**
	 * @brief	read a "len" number of registers on "rec" starting
	 * 			from "reg" register.
	 * @param	rec			Pointer to received byte array.
	 * @param	reg			Device register address.
	 * @param	len			Number of bytes to read.
	 */
	void get_registers(uint8_t* rec, uint8_t reg, uint8_t len);
};

//###############################################
#endif /* _DEVICES_LIS3DH_H_ */
//###############################################
