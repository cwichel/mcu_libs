/**
 * @file 	libs/device/lis3dh.cpp
 * @brief	LIS3DH driver library.
 */
//###############################################
#include <device/lis3dh.h>
//###############################################
#define update_field(reg, addr, name, field, value) {	\
	get_register(&reg.raw, addr);						\
	reg.name.field = value;								\
	set_register(reg.raw, addr);						\
}

//###############################################
LIS3DH::LIS3DH(TWI* twi, uint8_t addr): SPI::Device(), TWI::Device(twi, addr), _is_spi(false) {
	// Start the timing functions
	timing_enable();
}

LIS3DH::LIS3DH(SPI* spi, pins_t ss_pin): SPI::Device(spi, ss_pin), TWI::Device(), _is_spi(true) {
	// Configure the SPI device
	SPI::Device::configure(F_CPU/4, SPI_MODE_3, MSB_FIRST);

	// Start the timing functions
	timing_enable();
}

//###############################################
bool LIS3DH::is_available(void) {
	lis3dh_reg_t reg;
	// Read ID
	get_register(&reg.raw, LIS3DH_REG_ID);
	// Return
	return (reg.raw == LIS3DH_DEF_ID);
}

bool LIS3DH::enable(void) {
	// Check if the device is available
	if (!is_available())
		return false;
	// Set default values
	set_defaults();
	// Set BDU on
	lis3dh_reg_t reg;
	update_field(reg, LIS3DH_REG_CTRL4, ctrl4, BDU, LIS3DH_BDU_ON_READ);
	// Set configuration
	_config.scale = LIS3DH_SCALE_2G;
	_config.fifo_mode = LIS3DH_BYPASS;
	_config.resolution = LIS3DH_LOWPOWER;
	_config.mg_dig = get_accel_conversion();
	// Return
	return true;
}

void LIS3DH::set_defaults(void) {
	// Reboot memory content
	set_register(LIS3DH_DEF_TEMP_CFG, LIS3DH_REG_TEMP_CFG);
	set_register(LIS3DH_DEF_CTRL0, 		LIS3DH_REG_CTRL0);
	set_register(LIS3DH_DEF_CTRL1, 		LIS3DH_REG_CTRL1);
	set_register(LIS3DH_DEF_CTRL2, 		LIS3DH_REG_CTRL2);
	set_register(LIS3DH_DEF_CTRL3, 		LIS3DH_REG_CTRL3);
	set_register(LIS3DH_DEF_CTRL4, 		LIS3DH_REG_CTRL4);
	set_register(LIS3DH_DEF_CTRL5, 		LIS3DH_REG_CTRL5);
	set_register(LIS3DH_DEF_CTRL6, 		LIS3DH_REG_CTRL6);
	set_register(LIS3DH_DEF_REF, 		LIS3DH_REG_REF);
	set_register(LIS3DH_DEF_FIFO_CTRL,	LIS3DH_REG_FIFO_CTRL);
	set_register(LIS3DH_DEF_INT1_CFG,   LIS3DH_REG_INT1_CFG);
	set_register(LIS3DH_DEF_INT1_THS,   LIS3DH_REG_INT1_THS);
	set_register(LIS3DH_DEF_INT1_DUR,  	LIS3DH_REG_INT1_DUR);
	set_register(LIS3DH_DEF_INT2_CFG,   LIS3DH_REG_INT2_CFG);
	set_register(LIS3DH_DEF_INT2_THS,   LIS3DH_REG_INT2_THS);
	set_register(LIS3DH_DEF_INT2_DUR,  	LIS3DH_REG_INT2_DUR);
	set_register(LIS3DH_DEF_CLICK_CFG,  LIS3DH_REG_CLICK_CFG);
	set_register(LIS3DH_DEF_CLICK_THS,  LIS3DH_REG_CLICK_THS);
	set_register(LIS3DH_DEF_TIME_LIM,   LIS3DH_REG_TIME_LIM);
	set_register(LIS3DH_DEF_TIME_LAT,   LIS3DH_REG_TIME_LAT);
	set_register(LIS3DH_DEF_TIME_WIN,   LIS3DH_REG_TIME_WIN);
	set_register(LIS3DH_DEF_ACT_THS,    LIS3DH_REG_ACT_THS);
	set_register(LIS3DH_DEF_ACT_DUR,   	LIS3DH_REG_ACT_DUR);
}

void LIS3DH::set_mode(lis3dh_odr_t odr, lis3dh_res_t res, lis3dh_bdu_t bdu, bool x, bool y, bool z) {
	lis3dh_reg_t reg;
	// Set resolution and BDU
	get_register(&reg.raw, LIS3DH_REG_CTRL4);
	reg.ctrl4.BDU = bdu;
	reg.ctrl4.HR = (res == LIS3DH_HIGHRES);
	set_register(reg.raw, LIS3DH_REG_CTRL4);
	// Set mode
	get_register(&reg.raw, LIS3DH_REG_CTRL1);
	reg.ctrl1.LP_EN = (res == LIS3DH_LOWPOWER);
	reg.ctrl1.X_EN = x;
	reg.ctrl1.X_EN = y;
	reg.ctrl1.X_EN = z;
	reg.ctrl1.ODR  = odr;
	set_register(reg.raw, LIS3DH_REG_CTRL1);
	// Give the device time to restart
	delay_micros(200);
	// Update config
	_config.resolution = res;
	_config.mg_dig = get_accel_conversion();
}

void LIS3DH::set_scale(lis3dh_scale_t scale) {
	lis3dh_reg_t reg;
	// Set scale
	update_field(reg, LIS3DH_REG_CTRL4, ctrl4, FS, scale);
	// Update config
	_config.scale = scale;
	_config.mg_dig = get_accel_conversion();
}

lis3dh_scale_t LIS3DH::get_scale(void) {
	return _config.scale;
}

lis3dh_res_t LIS3DH::get_resolution(void) {
	return _config.resolution;
}

bool LIS3DH::is_data_available(void) {
	lis3dh_reg_t reg;
	if (_config.fifo_mode == LIS3DH_BYPASS) {
		get_register(&reg.raw, LIS3DH_REG_STAT);
		return reg.stat.ZYXDA;
	} else {
		get_register(&reg.raw, LIS3DH_REG_FIFO_SRC);
		return !reg.fifo_src.EMPTY;
	}
}

//###############################################
void LIS3DH::get_accel(lis3dh_accel_t* accel) {
	lis3dh_data_t 	data[3];
	// Get values
	get_registers((uint8_t*)(&data), LIS3DH_REG_X_OUT_L, 6);
	// Transform
	switch(_config.resolution) {
		case LIS3DH_LOWPOWER: {
			accel->x = (data[0].accel.low_power.value << _config.mg_dig);
			accel->y = (data[1].accel.low_power.value << _config.mg_dig);
			accel->z = (data[2].accel.low_power.value << _config.mg_dig);
		} break;
		case LIS3DH_NORMAL: {
			accel->x = (data[0].accel.normal.value << _config.mg_dig);
			accel->y = (data[1].accel.normal.value << _config.mg_dig);
			accel->z = (data[2].accel.normal.value << _config.mg_dig);
		} break;
		default: {
			accel->x = (data[0].accel.high_res.value << _config.mg_dig);
			accel->y = (data[1].accel.high_res.value << _config.mg_dig);
			accel->z = (data[2].accel.high_res.value << _config.mg_dig);
		} break;
	}
}

//###############################################
void LIS3DH::adc_enable(bool adc, bool temp) {
	lis3dh_reg_t reg;
	// Enable ADC on TEMP_CFG
	get_register(&reg.raw, LIS3DH_REG_TEMP_CFG);
	reg.temp_cfg.ADC_EN = adc;
	reg.temp_cfg.TEMP_EN = temp;
	set_register(reg.raw, LIS3DH_REG_TEMP_CFG);
}

void LIS3DH::get_adc(lis3dh_adc_t* adc) {
	lis3dh_reg_t 	reg;
	lis3dh_data_t	data[3];
	// Get values
	get_register(&reg.raw, LIS3DH_REG_TEMP_CFG);
	get_registers((uint8_t*)(&data), LIS3DH_REG_ADC1_OUT_L, 6);
	// Transform
	switch(get_resolution()) {
		case LIS3DH_LOWPOWER: {
			adc->l1 = data[0].adc.ADC8.value;
			adc->l2 = data[1].adc.ADC8.value;
			adc->l3 = data[2].adc.ADC8.value;
		} break;
		default: {
			adc->l1 = data[0].adc.ADC10.value;
			adc->l2 = data[1].adc.ADC10.value;
			adc->l3 = (reg.temp_cfg.TEMP_EN)? data[2].adc.ADC8.value : data[2].adc.ADC10.value;
		} break;
	}

}

//###############################################
void LIS3DH::fifo_enable(lis3dh_fifo_t mode, uint8_t wtm, bool wtm_int, bool ovr_int, bool tr) {
	lis3dh_reg_t reg;
	// Turn FIFO on/off
	update_field(reg, LIS3DH_REG_CTRL5, ctrl5, FIFO_EN, (mode !=LIS3DH_BYPASS));
	// Set FIFO CTRL
	get_register(&reg.raw, LIS3DH_REG_FIFO_CTRL);
	reg.fifo_ctrl.FM = mode;
	reg.fifo_ctrl.TR = tr;
	reg.fifo_ctrl.FTH = wtm;
	set_register(reg.raw, LIS3DH_REG_FIFO_CTRL);
	// Configure interrupts
	get_register(&reg.raw, LIS3DH_REG_CTRL3);
	reg.ctrl3.I1_WTM = wtm_int;
	reg.ctrl3.I1_OVR = ovr_int;
	set_register(reg.raw, LIS3DH_REG_CTRL3);
}

void LIS3DH::fifo_disable(void) {
	fifo_enable(LIS3DH_BYPASS, 0x00, false, false);
}

lis3dh_reg_fifo_src_t LIS3DH::fifo_get_state(void) {
	lis3dh_reg_t reg;
	// Get the source register
	get_register(&reg.raw, LIS3DH_REG_FIFO_SRC);
	// Return
	return reg.fifo_src;
}

lis3dh_reg_fifo_src_t LIS3DH::fifo_get_data(lis3dh_accel_t* data, uint8_t len) {
	// Check buffer
	if ((len == 0) || (data == NULL)) {
		return fifo_get_state();
	}
	// Check size
	len = (len > LIS3DH_FIFO_BUFF_LEN)? LIS3DH_FIFO_BUFF_LEN : len;
	// Retrieve data
	uint8_t idx = 0;
	while (idx++ < len) {
		get_accel(data++);
	}
	// Read and return FIFO state
	return fifo_get_state();
}

//###############################################
void LIS3DH::int_polarity(bool pol) {
	lis3dh_reg_t reg;
	update_field(reg, LIS3DH_REG_CTRL6, ctrl6, INT_POL, pol);
}

void LIS3DH::int_set_parameters(lis3dh_int_t signal, uint8_t ths, uint8_t dur, bool latch) {
	bool		 is_int1 = (signal == LIS3DH_INT1);
	lis3dh_reg_t reg;
	// Configure
	update_field(reg, LIS3DH_REG_CTRL5, ctrl5, I1_LIR, latch);
	set_register((0x7F & ths), (is_int1)? LIS3DH_REG_INT1_THS  : LIS3DH_REG_INT2_THS);
	set_register((0x7F & dur), (is_int1)? LIS3DH_REG_INT1_DUR : LIS3DH_REG_INT2_DUR);
}

void LIS3DH::int_data_enable(lis3dh_int_t signal, lis3dh_int_type_t type, bool enable) {
	bool		 is_int1  = (signal == LIS3DH_INT1);
	lis3dh_reg_t reg;
	// Configure
	if (is_int1) {
		get_register(&reg.raw, LIS3DH_REG_CTRL3);
		switch(type) {
			case LIS3DH_INT_IA1:            reg.ctrl3.I1_IA1 = enable;		break;
			case LIS3DH_INT_IA2:            reg.ctrl3.I1_IA2 = enable;   	break;
			case LIS3DH_INT_CLICK:          reg.ctrl3.I1_CLICK = enable; 	break;
			case LIS3DH_INT1_DATA_READY:    reg.ctrl3.I1_ZYXDA = enable; 	break;
			case LIS3DH_INT1_FIFO_WTM:      reg.ctrl3.I1_WTM = enable;   	break;
			case LIS3DH_INT1_FIFO_OVR:		reg.ctrl3.I1_OVR = enable;   	break;
			default: return;
		}
		set_register(reg.raw, LIS3DH_REG_CTRL3);
	} else {
		get_register(&reg.raw, LIS3DH_REG_CTRL6);
		switch(type) {
			case LIS3DH_INT_IA1:            reg.ctrl6.I2_IA1 = enable;	 	break;
			case LIS3DH_INT_IA2:            reg.ctrl6.I2_IA2 = enable;   	break;
			case LIS3DH_INT_CLICK:          reg.ctrl6.I2_CLICK = enable; 	break;
			case LIS3DH_INT2_BOOT:    		reg.ctrl6.I2_BOOT = enable;  	break;
			case LIS3DH_INT2_ACT:    		reg.ctrl6.I2_ACT = enable;   	break;
			default: return;
		}
		set_register(reg.raw, LIS3DH_REG_CTRL6);
	}
}

void LIS3DH::get_int_data_src(lis3dh_int_src_t* source) {
	lis3dh_reg_ctrl3_t 		ctrl3;
	lis3dh_reg_stat_t		stat;
	lis3dh_reg_fifo_src_t 	fifo;
	// Get registers
	get_register((uint8_t*)&ctrl3, LIS3DH_REG_CTRL3);
	get_register((uint8_t*)&stat, LIS3DH_REG_STAT);
	get_register((uint8_t*)&fifo, LIS3DH_REG_FIFO_SRC);
	// Set values
	source->data_ready = stat.ZYXDA & ctrl3.I1_ZYXDA;
	source->data_ready = fifo.WTM & ctrl3.I1_WTM;
	source->data_ready = fifo.OVR & ctrl3.I1_OVR;
}

void LIS3DH::int_data_disable(lis3dh_int_t signal) {
	bool		 is_int1  = (signal == LIS3DH_INT1);
	// Disable
	if (is_int1) {
		set_register(LIS3DH_DEF_CTRL3, LIS3DH_REG_CTRL3);
	} else {
		lis3dh_reg_t reg_old;
		lis3dh_reg_t reg_new;
		get_register(&reg_old.raw, LIS3DH_REG_CTRL6);
		reg_new.raw = LIS3DH_DEF_CTRL6;
		reg_new.ctrl6.INT_POL = reg_old.ctrl6.INT_POL;
		set_register(reg_new.raw, LIS3DH_REG_CTRL6);
	}
}

void LIS3DH::int_event_enable(lis3dh_int_t signal, lis3dh_int_evt_t event, uint8_t axis_mask) {
	bool		 is_int1 = (signal == LIS3DH_INT1);
	lis3dh_reg_t reg;
	// Get the event configuration
	lis3dh_int_evtcfg_t evt_cfg;
	evt_cfg.raw.u16 = ((event << 6) | axis_mask);
	// Write the interrupt configuration
	set_register(evt_cfg.raw.u8.lsb, (is_int1)? LIS3DH_REG_INT1_CFG  : LIS3DH_REG_INT2_CFG);
	// Set D4D
	if (is_int1) {
		update_field(reg, LIS3DH_REG_CTRL5, ctrl5, I1_D4D, evt_cfg.cfg.D4D);
	} else {
		update_field(reg, LIS3DH_REG_CTRL5, ctrl5, I2_D4D, evt_cfg.cfg.D4D);
	}
}

void LIS3DH::get_int_event_src(lis3dh_int_t signal, lis3dh_reg_intx_src_t* source) {
	get_register((uint8_t*)source, (signal == LIS3DH_INT1)? LIS3DH_REG_INT1_SRC : LIS3DH_REG_INT2_SRC);
}

void LIS3DH::int_event_disable(lis3dh_int_t signal) {
	bool		 is_int1  = (signal == LIS3DH_INT1);
	lis3dh_reg_t reg;
	// Set default configuration
	set_register(((is_int1)? LIS3DH_DEF_INT1_CFG : LIS3DH_DEF_INT2_CFG),
				 ((is_int1)? LIS3DH_REG_INT1_CFG : LIS3DH_REG_INT2_CFG));
	// Set default D4D
	if (is_int1) {
		update_field(reg, LIS3DH_REG_CTRL5, ctrl5, I1_D4D, 0);
	} else {
		update_field(reg, LIS3DH_REG_CTRL5, ctrl5, I2_D4D, 0);
	}
}

//###############################################
void LIS3DH::click_enable(uint8_t axis_mask, bool sclick, bool dclick,
		uint8_t ths, uint8_t tlim, uint8_t tlat, uint8_t twin, bool latch) {
	lis3dh_reg_t reg;
	// Configure click source
	get_register(&reg.raw, LIS3DH_REG_CLICK_CFG);
	reg.click_cfg.X_S = (sclick && (0x01 & axis_mask));
	reg.click_cfg.X_D = (dclick && (0x01 & axis_mask));
	reg.click_cfg.Y_S = (sclick && (0x02 & axis_mask));
	reg.click_cfg.Y_D = (dclick && (0x02 & axis_mask));
	reg.click_cfg.Z_S = (sclick && (0x04 & axis_mask));
	reg.click_cfg.Z_D = (dclick && (0x04 & axis_mask));
	set_register(reg.raw, LIS3DH_REG_CLICK_CFG);
	// Set threshold and latch
	get_register(&reg.raw, LIS3DH_REG_CLICK_THS);
	reg.click_tht.LIR_CLICK = latch;
	reg.click_tht.THS = ths;
	set_register(reg.raw, LIS3DH_REG_CLICK_THS);
	// Set time parameters
	set_register(tlim, LIS3DH_REG_TIME_LIM);
	set_register(tlat, LIS3DH_REG_TIME_LAT);
	set_register(twin, LIS3DH_REG_TIME_WIN);
}

void LIS3DH::click_disable(void) {
	// Set click registers as default
	set_register(LIS3DH_DEF_CLICK_CFG, LIS3DH_REG_CLICK_CFG);
	set_register(LIS3DH_DEF_CLICK_THS, LIS3DH_REG_CLICK_THS);
	set_register(LIS3DH_DEF_TIME_LIM, LIS3DH_REG_TIME_LIM);
	set_register(LIS3DH_DEF_TIME_LAT, LIS3DH_REG_TIME_LAT);
	set_register(LIS3DH_DEF_TIME_WIN, LIS3DH_REG_TIME_WIN);
}

void LIS3DH::get_click_src(lis3dh_reg_click_src_t* source) {
	get_register((uint8_t*)source, LIS3DH_REG_CLICK_SRC);
}

//###############################################
void LIS3DH::hpf_configure(lis3dh_hpm_t mode, lis3dh_hpcf_t cf, bool fds, bool click, bool ia1, bool ia2) {
	lis3dh_reg_t reg;
	// Configure
	get_register(&reg.raw, LIS3DH_REG_CTRL2);
	reg.ctrl2.HPM = mode;
	reg.ctrl2.HPCF = cf;
	reg.ctrl2.HPCLICK = click;
	reg.ctrl2.HP_IA1 = ia1;
	reg.ctrl2.HP_IA2 = ia2;
	set_register(reg.raw, LIS3DH_REG_CTRL2);
}

void LIS3DH::hpd_reset(void) {
	set_register(LIS3DH_DEF_CTRL2, LIS3DH_REG_CTRL2);
}

void LIS3DH::get_reference(uint8_t* source) {
	get_register(source, LIS3DH_REG_REF);
}

//###############################################
void LIS3DH::activity_enable(uint8_t ths, uint8_t dur) {
	set_register((0x7F & ths), LIS3DH_REG_ACT_THS);
	set_register((0x7F & dur), LIS3DH_REG_ACT_DUR);
}

void LIS3DH::activity_disable(void) {
	activity_enable(LIS3DH_DEF_ACT_THS, LIS3DH_DEF_ACT_DUR);
}

//###############################################
uint8_t LIS3DH::get_accel_conversion(void) {
	uint8_t scale;
	switch(_config.resolution) {
		case LIS3DH_LOWPOWER:	scale = 4; break;	//  8bit mode: raw * 16;
		case LIS3DH_NORMAL:		scale = 2; break;	// 10bit mode: raw * 4;
		default:				scale = 0; break;	// 12bit mode: raw * 1;
	}
	switch(_config.scale) {
		case LIS3DH_SCALE_4G:	scale += 1; break;	//  4G scale: 2G * 2;
		case LIS3DH_SCALE_8G:	scale += 2; break;	//  8G scale: 2G * 4;
		case LIS3DH_SCALE_16G:	scale += 3; break;	// 16G scale: 2G * 8;
		default:				scale += 0; break;	//  2G scale;
	}
	return scale;
}

//###############################################
void LIS3DH::set_register(uint8_t src, uint8_t reg) {
	// Write register
	if (_is_spi) {
		// Turn on write mode
		reg |= LIS3DH_SPI_W;
		// Write SPI
		SPI::Device::begin_transaction();
		SPI::Device::write_reg(reg, src);
		SPI::Device::end_transaction();
	} else {
		// Write I2C
		TWI::Device::write_reg(reg, src, true);
	}
}

void LIS3DH::get_register(uint8_t* rec, uint8_t reg) {
	get_registers(rec, reg, 1);
}

void LIS3DH::get_registers(uint8_t* rec, uint8_t reg, uint8_t len) {
	// Read registers
	if (_is_spi) {
		// Turn auto-increment on and read mode
		reg |= (LIS3DH_SPI_INC | LIS3DH_SPI_R);
		// Read SPI
		SPI::Device::begin_transaction();
		SPI::Device::write_reg(reg, rec, len);
		SPI::Device::end_transaction();
	} else {
		// Turn auto-increment on (bit 7)
		reg |= (len > 1)? LIS3DH_TWI_INC : 0x00;
		// Read I2C
		TWI::Device::read_reg(reg, rec, len, true);
	}
}
