/**
 * @code	#include "device/lis3dh_defs.h" @endcode
 * @file 	libs/device/lis3dh_defs.h
 * @brief	LIS3DH driver definitions.
 */
//###############################################
#ifndef _DEVICES_LIS3DH_DEFS_H_
#define _DEVICES_LIS3DH_DEFS_H_
//###############################################
#include <hardware/spi.h>
#include <hardware/twi.h>

//###############################################
/* Device Address (TWI). */
#define LIS3DH_TWI_ADDR0			0x18	//!< Device TWI address for SDO/SA0 pin low
#define LIS3DH_TWI_ADDR1			0x19	//!< Device TWI address for SDO/SA0 pin high
#define	LIS3DH_TWI_INC				0x80	//!< Device TWI Increment register

/* Device comms (SPI) */
#define LIS3DH_SPI_R				0x00	//!< Device SPI Write modifier
#define LIS3DH_SPI_W				0x80	//!< Device SPI Read modifier
#define LIS3DH_SPI_INC				0x40	//!< Device SPI Increment register

#define LIS3DH_REGS_NUM				42		//!< Number of registers.
#define LIS3DH_FIFO_BUFF_LEN		32		//!< FIFO buffer length.

//###############################################
/** Register address. */
#define LIS3DH_REG_STAT				0x27	//!< Status register.
#define LIS3DH_REG_STAT_AUX			0x07	//!< Status AUX register.
#define LIS3DH_REG_ADC1_OUT_L		0x08	//!< ADC1 OUT LSB register.
#define LIS3DH_REG_ADC1_OUT_H		0x09	//!< ADC1 OUT MSB register.
#define LIS3DH_REG_ADC2_OUT_L		0x0A	//!< ADC2 OUT LSB register.
#define LIS3DH_REG_ADC2_OUT_H		0x0B	//!< ADC2 OUT MSB register.
#define LIS3DH_REG_ADC3_OUT_L		0x0C	//!< ADC3 OUT LSB register.
#define LIS3DH_REG_ADC3_OUT_H		0x0D	//!< ADC3 OUT MSB register.
#define LIS3DH_REG_ID				0x0F	//!< WHO AM I (Identification register) register.
#define LIS3DH_REG_TEMP_CFG			0x1F	//!< Temperature sensor configuration register.
#define LIS3DH_REG_CTRL0			0x1E	//!< Control register 0 register.
#define LIS3DH_REG_CTRL1			0x20	//!< Control register 1 register.
#define LIS3DH_REG_CTRL2			0x21	//!< Control register 2 register.
#define LIS3DH_REG_CTRL3			0x22	//!< Control register 3 register.
#define LIS3DH_REG_CTRL4			0x23	//!< Control register 4 register.
#define LIS3DH_REG_CTRL5			0x24	//!< Control register 5 register.
#define LIS3DH_REG_CTRL6			0x25	//!< Control register 6 register.
#define LIS3DH_REG_REF				0x26	//!< Reference value for interrupt generation register.
#define LIS3DH_REG_X_OUT_L			0x28	//!< X-axis acceleration data LSB register.
#define LIS3DH_REG_X_OUT_H			0x29	//!< X-axis acceleration data MSB register.
#define LIS3DH_REG_Y_OUT_L			0x2A	//!< Y-axis acceleration data LSB register.
#define LIS3DH_REG_Y_OUT_H			0x2B	//!< Y-axis acceleration data MSB register.
#define LIS3DH_REG_Z_OUT_L			0x2C	//!< Z-axis acceleration data LSB register.
#define LIS3DH_REG_Z_OUT_H			0x2D	//!< Z-axis acceleration data MSB register.
#define LIS3DH_REG_FIFO_CTRL		0x2E	//!< FIFO control register.
#define LIS3DH_REG_FIFO_SRC			0x2F	//!< FIFO source register.
#define LIS3DH_REG_INT1_CFG			0x30	//!< Interrupt 1 configuration register.
#define LIS3DH_REG_INT1_SRC			0x31	//!< Interrupt 1 source register.
#define LIS3DH_REG_INT1_THS			0x32	//!< Interrupt 1 threshold register.
#define LIS3DH_REG_INT1_DUR			0x33	//!< Interrupt 1 duration register.
#define LIS3DH_REG_INT2_CFG			0x34	//!< Interrupt 2 configuration register.
#define LIS3DH_REG_INT2_SRC			0x35	//!< Interrupt 2 source register.
#define LIS3DH_REG_INT2_THS			0x36	//!< Interrupt 2 threshold register.
#define LIS3DH_REG_INT2_DUR			0x37	//!< Interrupt 2 duration register.
#define LIS3DH_REG_CLICK_CFG		0x38	//!< Click configuration register.
#define LIS3DH_REG_CLICK_SRC		0x39	//!< Click source register.
#define LIS3DH_REG_CLICK_THS		0x3A	//!< Click threshold register.
#define LIS3DH_REG_TIME_LIM			0x3B	//!< Click time limit register.
#define LIS3DH_REG_TIME_LAT			0x3C	//!< Click time latency register.
#define LIS3DH_REG_TIME_WIN			0x3D	//!< Click time window register.
#define LIS3DH_REG_ACT_THS			0x3E	//!< LIS3DH activation threshold register.
#define LIS3DH_REG_ACT_DUR			0x3F	//!< LIS3DH activation duration register.

//###############################################
/* Register default values (only for r/w or w registers) */
#define LIS3DH_DEF_ID				0x33	//!< WHO AM I register default value.
#define LIS3DH_DEF_TEMP_CFG			0x00	//!< Temperature Config. register default value.
#define LIS3DH_DEF_CTRL0			0x10    //!< Control 0 register default value.
#define LIS3DH_DEF_CTRL1			0x07    //!< Control 1 register default value.
#define LIS3DH_DEF_CTRL2			0x00    //!< Control 2 register default value.
#define LIS3DH_DEF_CTRL3			0x00    //!< Control 3 register default value.
#define LIS3DH_DEF_CTRL4			0x00    //!< Control 4 register default value.
#define LIS3DH_DEF_CTRL5			0x00    //!< Control 5 register default value.
#define LIS3DH_DEF_CTRL6			0x00    //!< Control 6 register default value.
#define LIS3DH_DEF_REF				0x00    //!< Reference register default value.
#define LIS3DH_DEF_FIFO_CTRL		0x00    //!< FIFO control register default value.
#define LIS3DH_DEF_INT1_CFG			0x00	//!< Interrupt 1 Config. register default value.
#define LIS3DH_DEF_INT1_THS			0x00	//!< Interrupt 1 Threshold register default value.
#define LIS3DH_DEF_INT1_DUR			0x00	//!< Interrupt 1 Duration register default value.
#define LIS3DH_DEF_INT2_CFG			0x00	//!< Interrupt 2 Config. register default value.
#define LIS3DH_DEF_INT2_THS			0x00	//!< Interrupt 2 Threshold register default value.
#define LIS3DH_DEF_INT2_DUR			0x00	//!< Interrupt 2 Duration register default value.
#define LIS3DH_DEF_CLICK_CFG		0x00	//!< Click Config. register default value.
#define LIS3DH_DEF_CLICK_THS		0x00	//!< Click Threshold register default value.
#define LIS3DH_DEF_TIME_LIM			0x00	//!< Time Limit register default value.
#define LIS3DH_DEF_TIME_LAT			0x00	//!< Time Latency register default value.
#define LIS3DH_DEF_TIME_WIN			0x00	//!< Time Window register default value.
#define LIS3DH_DEF_ACT_THS			0x00	//!< Act. Threshold register default value.
#define LIS3DH_DEF_ACT_DUR			0x00	//!< Act. Duration register default value.

//###############################################
/* Register mapping */
/**
 * @struct	lis3dh_reg_stat_t
 * @brief	Status register structure.
 */
typedef struct {
	uint8_t	XDA		: 1;	//!< X-axis data available.
	uint8_t	YDA		: 1;	//!< Y-axis data available.
	uint8_t	ZDA		: 1;	//!< Z-axis data available.
	uint8_t	ZYXDA	: 1;	//!< All-axis data available.
	uint8_t	XOR		: 1;	//!< X-axis data overrun.
	uint8_t	YOR		: 1;	//!< Y-axis data overrun.
	uint8_t	ZOR	  	: 1;	//!< Z-axis data overrun.
	uint8_t	ZYXOR 	: 1;	//!< All-axis data overrun.
} lis3dh_reg_stat_t;

/**
 * @struct	lis3dh_reg_ctrl0_t
 * @brief	CTRL0 register structure.
 */
typedef struct {
	uint8_t	unused	: 7;	//!< Unused bit.
	uint8_t	SDO_PU	: 1;	//!< SDO pullup connected.
} lis3dh_reg_ctrl0_t;

/**
 * @struct	lis3dh_reg_ctrl1_t
 * @brief	CTRL1 register structure.
 */
typedef struct {
	uint8_t	X_EN	: 1;	//!< X-axis enable.
	uint8_t	Y_EN	: 1;	//!< Y-axis enable.
	uint8_t	Z_EN	: 1;	//!< Z-axis enable.
	uint8_t	LP_EN	: 1;	//!< Low power mode enable.
	uint8_t	ODR		: 4;	//!< Data rate selection.
} lis3dh_reg_ctrl1_t;

/**
 * @struct	lis3dh_reg_ctrl2_t
 * @brief	CTRL2 register structure.
 */
typedef struct {
	uint8_t	HP_IA1	 : 1;	//!< HPF enabled for AOI on INT1.
	uint8_t	HP_IA2	 : 1;	//!< HPF enabled for AOI on INT2.
	uint8_t	HPCLICK	 : 1;	//!< HPF enabled for CLICK.
	uint8_t FDS		 : 1;	//!< Filter data selection.
	uint8_t HPCF	 : 2;	//!< HPF cutoff frequency.
	uint8_t HPM		 : 2;	//!< HPF mode.
} lis3dh_reg_ctrl2_t;

/**
 * @struct	lis3dh_reg_ctrl3_t
 * @brief	CTRL3 register structure.
 */
typedef struct {
	uint8_t unused	 : 1;	//!< Unused bit.
	uint8_t I1_OVR	 : 1;	//!< FIFO overrun interrupt on INT1.
	uint8_t	I1_WTM	 : 1;	//!< FIFO watermark interrupt on INT1.
	uint8_t I1_321DA : 1;	//!< 321DA interrupt on INT1.
	uint8_t I1_ZYXDA : 1;	//!< ZYXDA interrupt on INT1.
	uint8_t I1_IA2	 : 1;	//!< AOI2 interrupt on INT1.
	uint8_t I1_IA1	 : 1;	//!< AOI1 interrupt on INT1.
	uint8_t I1_CLICK : 1;	//!< CLICK interrupt on INT1.
} lis3dh_reg_ctrl3_t;

/**
 * @struct	lis3dh_reg_ctrl4_t
 * @brief	CTRL4 register structure.
 */
typedef struct {
	uint8_t	SIM		 : 1;	//!< SPI bus interface selection.
	uint8_t ST		 : 2;	//!< Self-Test enable.
	uint8_t	HR		 : 1;	//!< High-Resolution output mode.
	uint8_t FS		 : 2;	//!< Full scale selection.
	uint8_t BLE		 : 1;	//!< Big/Little endian selection.
	uint8_t BDU		 : 1;	//!< Block Data Update.
} lis3dh_reg_ctrl4_t;

/**
 * @struct	lis3dh_reg_ctrl5_t
 * @brief	CTRL5 register structure.
 */
typedef struct {
	uint8_t	I2_D4D 	 : 1;	//!< 4D detection enabled on INT2.
	uint8_t I2_LIR 	 : 1;	//!< Latch interrupt request on INT2 (Interrupt on until INT2 SRC register is read).
	uint8_t	I1_D4D 	 : 1;	//!< 4D detection enabled on INT1.
	uint8_t I1_LIR 	 : 1;	//!< Latch interrupt request on INT1 (Interrupt on until INT1 SRC register is read).
	uint8_t unused	 : 2;	//!< Unused bit.
	uint8_t FIFO_EN	 : 1;	//!< FIFO enable.
	uint8_t BOOT	 : 1;	//!< Reboot memory content.
} lis3dh_reg_ctrl5_t;

/**
 * @struct	lis3dh_reg_ctrl6_t
 * @brief	CTRL6 register structure.
 */
typedef struct {
	uint8_t unused_1 : 1;	//!< Unused bit.
	uint8_t	INT_POL	 : 1;	//!< Interrupt polarity.
	uint8_t unused_2 : 1;	//!< Unused bit.
	uint8_t I2_ACT 	 : 1;	//!< Activity interrupt on INT2.
	uint8_t I2_BOOT	 : 1;	//!< Boot interrupt on INT2.
	uint8_t I2_IA2	 : 1;	//!< AOI2 interrupt on INT2.
	uint8_t I2_IA1	 : 1;	//!< AOI1 interrupt on INT2.
	uint8_t I2_CLICK : 1;	//!< CLICK interrupt on INT2.
} lis3dh_reg_ctrl6_t;

/**
 * @struct	lis3dh_reg_temp_cfg_t
 * @brief	Temperature configuration register structure.
 */
typedef struct {
	uint8_t unused	 : 6;	//!< Unused bit.
	uint8_t TEMP_EN	 : 1;	//!< Temperature sensor enable.
	uint8_t ADC_EN	 : 1;	//!< ADC enable.
} lis3dh_reg_temp_cfg_t;

/**
 * @struct	lis3dh_reg_fifo_ctrl_t
 * @brief	FIFO control register structure.
 */
typedef struct {
	uint8_t FTH		 : 4;	//!< FIFO watermark (sample threshold).
	uint8_t TR		 : 1;	//!< Trigger selection INT1/INT2.
	uint8_t FM		 : 2;	//!< FIFO mode.
} lis3dh_reg_fifo_ctrl_t;

/**
 * @struct 	lis3dh_reg_fifo_src_t
 * @brief	FIFO source register structure.
 */
typedef struct {
	uint8_t FSS		 : 4;	//!< FIFO samples stored.
	uint8_t EMPTY	 : 1;	//!< FIFO is empty.
	uint8_t OVR 	 : 1;	//!< FIFO is full.
	uint8_t WTM		 : 1;	//!< FIFO samples exceed watermark (FTH).
} lis3dh_reg_fifo_src_t;

/**
 * @struct	lis3dh_reg_intx_cfg_t
 * @brief	INTx configuration register structure.
 */
typedef struct {
	uint8_t X_LIE 	 : 1;	//!< X-axis low event/direction recognition interrupt request on INTx.
	uint8_t	X_HIE	 : 1;	//!< X-axis high event/direction recognition interrupt request on INTx.
	uint8_t Y_LIE 	 : 1;	//!< Y-axis low event/direction recognition interrupt request on INTx.
	uint8_t	Y_HIE	 : 1;	//!< Y-axis high event/direction recognition interrupt request on INTx.
	uint8_t Z_LIE 	 : 1;	//!< Z-axis low event/direction recognition interrupt request on INTx.
	uint8_t	Z_HIE	 : 1;	//!< Z-axis high event/direction recognition interrupt request on INTx.
	uint8_t AOI_6D	 : 2;	//!< Interrupt mode.
} lis3dh_reg_intx_cfg_t;

/**
 * @struct	lis3dh_reg_intx_src_t
 * @brief	INTx source register structure.
 */
typedef struct {
	uint8_t X_L	 	 : 1;	//!< X-axis low.
	uint8_t	X_H 	 : 1;	//!< X-axis high.
	uint8_t Y_L	 	 : 1;	//!< Y-axis low.
	uint8_t	Y_H 	 : 1;	//!< Y-axis high.
	uint8_t Z_L	 	 : 1;	//!< Z-axis low.
	uint8_t	Z_H 	 : 1;	//!< Z-axis high.
	uint8_t IA	 	 : 1;	//!< Interrupt active.
	uint8_t unused	 : 1;	//!< Unused bit.
} lis3dh_reg_intx_src_t;

/**
 * @struct	lis3dh_reg_click_cfg_t
 * @brief	CLICK configuration register structure.
 */
typedef struct {
	uint8_t X_S		 : 1;	//!< Enable simple click interrupt on X-axis.
	uint8_t X_D		 : 1;   //!< Enable double click interrupt on X-axis.
	uint8_t Y_S		 : 1;   //!< Enable simple click interrupt on Y-axis.
	uint8_t Y_D		 : 1;   //!< Enable double click interrupt on Y-axis.
	uint8_t Z_S		 : 1;   //!< Enable simple click interrupt on Z-axis.
	uint8_t Z_D		 : 1;   //!< Enable double click interrupt on Z-axis.
	uint8_t unused	 : 2;   //!< Unused bit.
} lis3dh_reg_click_cfg_t;

/**
 * @struct	lis3dh_reg_click_src_t
 * @brief	CLICK source register structure.
 */
typedef struct {
	uint8_t X_CLICK	 : 1;	//!< X-axis click detection.
	uint8_t Y_CLICK	 : 1;   //!< Y-axis click detection.
	uint8_t Z_CLICK	 : 1;   //!< Z-axis click detection.
	uint8_t SIGN	 : 1; 	//!< Click sign.
	uint8_t SCLICK	 : 1;	//!< Single click.
	uint8_t DCLICK	 : 1;	//!< Double click.
	uint8_t IA		 : 1; 	//!< Interrupt active.
} lis3dh_reg_click_src_t;

/**
 * @struct	lis3dh_reg_click_ths_t
 * @brief	CLICK threshold register structure.
 */
typedef struct {
	uint8_t THS	 	 : 7;	//!< Click threshold.
	uint8_t LIR_CLICK: 1;   //!< Latch interrupt on click interruption (Interrupt on until CLICK SRC register is read).
} lis3dh_reg_click_tht_t;

//###############################################
/* Definitions */
/**
 * @enum 	lis3dh_odr_t
 * @brief	Output Data Rates (ODR).
 */
typedef enum {
	LIS3DH_POWERDOWN	= 0x00,	//!< Power down the device.
	LIS3DH_ODR_1HZ	 	= 0x01,	//!< 1[Hz] rate.
	LIS3DH_ODR_10HZ		= 0x02, //!< 10[Hz] range.
	LIS3DH_ODR_25HZ		= 0x03, //!< 25[Hz] range.
	LIS3DH_ODR_50HZ		= 0x04, //!< 50[Hz] range.
	LIS3DH_ODR_100HZ 	= 0x05, //!< 100[Hz] range.
	LIS3DH_ODR_200HZ 	= 0x06, //!< 200[Hz] range.
	LIS3DH_ODR_400HZ 	= 0x07, //!< 400[Hz] range.
	LIS3DH_ODR_1K6HZ 	= 0x08, //!< Low power 1.6[KHz] range.
	LIS3DH_ODR_5KHZ		= 0x09, //!< Low power 5[KHz] range, normal 1.25[KHz] range.
} lis3dh_odr_t;

/**
 * @enum	lis3dh_res_t
 * @brief	Resolution modes.
 */
typedef enum {
	LIS3DH_LOWPOWER		= 0x10, //!< 8bit data output
	LIS3DH_NORMAL		= 0x00,	//!< 10bit data output
	LIS3DH_HIGHRES		= 0x01	//!< 12bit data output
} lis3dh_res_t;

/**
 * @enum	lis3dh_bdu_t
 * @brief	Block data update modes.
 */
typedef enum {
	LIS3DH_BDU_CONTINUOUS	= 0x00,
	LIS3DH_BDU_ON_READ		= 0x01,
} lis3dh_bdu_t;

/**
 * @enum 	lis3dh_scale_t
 * @brief	Accelerometer full scale measurement range.
 */
typedef enum {
	LIS3DH_SCALE_2G		= 0x00,	//!< +/- 2g.
	LIS3DH_SCALE_4G  	= 0x01,	//!< +/- 4g.
	LIS3DH_SCALE_8G  	= 0x02,	//!< +/- 8g.
	LIS3DH_SCALE_16G 	= 0x03	//!< +/- 16g.
} lis3dh_scale_t;

/**
 * @enum	lis3dh_fifo_t
 * @brief	FIFO modes.
 */
typedef enum {
	LIS3DH_BYPASS 		= 0x00,
	LIS3DH_FIFO			= 0x01,
	LIS3DH_STREAM		= 0x02,
	LIS3DH_STREAM_FIFO	= 0x03
} lis3dh_fifo_t;

/**
 * @enum	lis3dh_int_t
 * @brief	Interruption selection.
 */
typedef enum {
	LIS3DH_INT1 = 0x00,			//!< Interrupt signal 1.
	LIS3DH_INT2 = 0x01,			//!< Interrupt signal 2.
} lis3dh_int_t;

/**
 * @enum	lis3dh_int_type_t
 * @brief	Interruption types.
 */
typedef enum {
	LIS3DH_INT_IA1,				//!< Inertial event interrupt 1.
	LIS3DH_INT_IA2,             //!< Inertial event interrupt 2.
	LIS3DH_INT_CLICK,			//!< Click detection interrupt.
	LIS3DH_INT1_DATA_READY,		//!< INT1 only: Data ready for read.
	LIS3DH_INT1_FIFO_WTM,		//!< INT1 only: FIFO exceeds the threshold.
	LIS3DH_INT1_FIFO_OVR,		//!< INT1 only: FIFO is full.
	LIS3DH_INT2_BOOT,			//!< INT2 only: On boot.
	LIS3DH_INT2_ACT             //!< INT2 only: Activity interrupt.
} lis3dh_int_type_t;

/**
 * @enum	lis3dh_int_evt_t
 * @brief	Inertial event interruption mode.
 */
typedef enum {
	LIS3DH_INTEV_WAKE_UP		= 0b000,	//!< 0: D4D, AOI, D6D: b000
	LIS3DH_INTEV_FREE_FALL		= 0b010,    //!< 2: D4D, AOI, D6D: b010
	LIS3DH_INTEV_6D_MOVEMENT	= 0b001,    //!< 1: D4D, AOI, D6D: b001
	LIS3DH_INTEV_6D_POSITION	= 0b011,    //!< 3: D4D, AOI, D6D: b011
	LIS3DH_INTEV_4D_MOVEMENT	= 0b101,    //!< 5: D4D, AOI, D6D: b101
	LIS3DH_INTEV_4D_POSITION	= 0b111,    //!< 7: D4D, AOI, D6D: b111
} lis3dh_int_evt_t;

/**
 * @enum	lis3dh_hpm_t
 * @brief	High Pass filter modes.
 */
typedef enum {
	LIS3DH_HPM_NORMAL 			= 0x10,		//!< Normal mode.
	LIS3DH_HPM_NORMAL_REF_RST	= 0x00,     //!< Normal mode. Reset by reading reference.
	LIS3DH_HPM_REF_SIG			= 0x01,     //!< Reference signal for filtering.
	LIS3DH_HPM_ARST_INT			= 0x11      //!< Auto-reset on interrupt.
} lis3dh_hpm_t;

/**
 * @enum	lis3dh_hpcf_t
 * @brief	High Pass filter cut frequency selection.
 */
typedef enum {
	LIS3DH_HPCF_0 = 0x00,		//! Cut frequency 0: ~ 1/50 ODR
	LIS3DH_HPCF_1 = 0x01,       //! Cut frequency 1: ~1/100 ODR
	LIS3DH_HPCF_2 = 0x10,       //! Cut frequency 2: ~1/200 ODR
	LIS3DH_HPCF_3 = 0x11        //! Cut frequency 3: ~1/400 ODR
} lis3dh_hpcf_t;

//###############################################
/* Data types */
/**
 * @union	lis3dh_reg_t
 * @brief	General register handler.
 */
typedef union {
	uint8_t					raw;		//!< Raw data
	lis3dh_reg_stat_t		stat;		//!< Data formatted as STATUS register.
	lis3dh_reg_ctrl0_t  	ctrl0;		//!< Data formatted as CTRL0 register.
	lis3dh_reg_ctrl1_t		ctrl1;      //!< Data formatted as CTRL1 register.
	lis3dh_reg_ctrl2_t		ctrl2;      //!< Data formatted as CTRL2 register.
	lis3dh_reg_ctrl3_t		ctrl3;      //!< Data formatted as CTRL3 register.
	lis3dh_reg_ctrl4_t		ctrl4;      //!< Data formatted as CTRL4 register.
	lis3dh_reg_ctrl5_t		ctrl5;      //!< Data formatted as CTRL5 register.
	lis3dh_reg_ctrl6_t		ctrl6;      //!< Data formatted as CTRL6 register.
	lis3dh_reg_temp_cfg_t	temp_cfg;   //!< Data formatted as TEMP_CFG register.
	lis3dh_reg_fifo_ctrl_t	fifo_ctrl;  //!< Data formatted as FIFO_CTRL register.
	lis3dh_reg_fifo_src_t	fifo_src;   //!< Data formatted as FIFO_SRC register.
	lis3dh_reg_intx_cfg_t	int_cgf;	//!< Data formatted as INTx_CFG register.
	lis3dh_reg_intx_src_t	int_src;	//!< Data formatted as INTx_SRC register.
	lis3dh_reg_click_cfg_t	click_cfg;  //!< Data formatted as CLICK_CFG register.
	lis3dh_reg_click_src_t	click_src;  //!< Data formatted as CLICK_SRC register.
	lis3dh_reg_click_tht_t	click_tht;  //!< Data formatted as CLICK_THT register.
} lis3dh_reg_t;

/**
 * @union	lis3dh_adc_bits_t
 * @brief	ADC bits depending on power mode.
 */
typedef union {
	struct {							//!< Conversion on low power mode.
		int16_t	unused	: 8;			//!< 8bit unused.
		int16_t value	: 8;			//!< 8bit value.
	} ADC8;
	struct {							//!< Conversion on normal mode.
		int16_t	unused	: 6;			//!< 6bit unused.
		int16_t value	: 10;			//!< 10bit value.
	} ADC10;
} lis3dh_adc_bits_t;


/**
 * @union	lis3dh_accel_bits_t
 * @brief	Acceleration bits depending on power mode.
 */
typedef union {
	struct {							//!< Conversion on low power mode.
		int16_t	unused	: 8;			//!< 8bit unused.
		int16_t value	: 8;			//!< 8bit value.
	} low_power;
	struct {							//!< Conversion on normal mode.
		int16_t	unused	: 6;			//!< 6bit unused.
		int16_t value	: 10;			//!< 10bit value.
	} normal;
	struct {							//!< Conversion on high resolution mode.
		int16_t	unused	: 4;			//!< 4bit unused.
		int16_t value	: 12;			//!< 12bit value.
	} high_res;
} lis3dh_accel_bits_t;

/**
 * @union	lis3dh_data_t
 * @brief	General data reader handler.
 */
typedef union {
	bk_u16_u8_t 		raw;			//!< Raw data.
	lis3dh_adc_bits_t	adc;			//!< ADC.
	lis3dh_accel_bits_t	accel;			//!< Accelerometer.
} lis3dh_data_t;

/**
 * @struct	lis3dh_adc_t
 * @brief	ADC data sample.
 */
typedef struct {
	uint16_t l1;							//!< ADC line 1.
	uint16_t l2;							//!< ADC line 2.
	uint16_t l3;							//!< ADC line 3.
} lis3dh_adc_t;

/**
 * @struct	lis3dh_accel_t
 * @brief	Accelerometer data sample.
 */
typedef struct {
	int16_t	x;							//!< X-axis.
	int16_t	y;							//!< Y-axis.
	int16_t	z;							//!< Z-axis.
} lis3dh_accel_t;

typedef struct {
	uint8_t			mg_dig;
	lis3dh_scale_t	scale;
	lis3dh_res_t	resolution;
	lis3dh_fifo_t	fifo_mode;
} lis3dh_t;

/**
 * @struct 	lis3dh_int_src_t
 * @brief	Data interrupt source.
 */
typedef struct {
	bool data_ready;
	bool fifo_wtm;
	bool fifo_ovr;
} lis3dh_int_src_t;

/**
 * @union	lis3dh_int_evtcfg_t
 * @brief	Inertial event configuration handler.
 */
typedef union {
	bk_u16_u8_t raw;
	struct {
		lis3dh_reg_intx_cfg_t 	INTx;
		uint8_t					D4D		:1;
		uint8_t					unused	:7;
	} cfg;
} lis3dh_int_evtcfg_t;

//###############################################
#endif /* _DEVICES_LIS3DH_DEFS_H_ */
//###############################################

