/**
 * @code 	#include "hardware/spi_defs.h" @endcode
 * @file	libs/hardware/spi_defs.h
 * @brief	AVR SPI library definitions.
 */
//###############################################
#ifndef _HARDWARE_SPI_DEFS_H_
#define _HARDWARE_SPI_DEFS_H_
//###############################################
#include <hardware/gpio.h>
#include <hardware/timing.h>
#include <hardware/int_ext.h>
#include <hardware/int_pc.h>

//###############################################
/** SPI maximum frequency */
#define SPI_MAX_FREQ		F_CPU / 2

//###############################################
/**
 * @struct	spi_spcr_t
 * @brief	SPI control register fields.
 */
typedef struct {
	uint8_t spr		: 2;	//!< Clock rate select.
	uint8_t cconf	: 2;	//!< Clock phase & polarity.
	uint8_t mstr	: 1;    //!< Master/slave select.
	uint8_t dord	: 1;    //!< Data order.
	uint8_t spe		: 1;    //!< SPI enable.
	uint8_t spie	: 1;    //!< SPI interrupt enable.
} spi_spcr_t;

/**
 * @struct	spi_spsr_t
 * @brief	SPI status register fields.
 */
typedef struct {
	uint8_t spi2x	: 1;	//!< SPI double speed.
	uint8_t unused	: 5;    //!< Unused bit.
	uint8_t wcol	: 1;    //!< Write collision flag.
	uint8_t spif	: 1;    //!< SPI interrupt flag.
} spi_spsr_t;

//###############################################
/**
 * @enum	spi_tx_mode_t
 * @brief	SPI transmission modes.
 */
typedef enum {
	SPI_MODE_0 = 0x00,	//!< CPOL(0), CPHA(0): Clock low when inactive, Data is valid on clock leading edge.
	SPI_MODE_1 = 0x01,	//!< CPOL(0), CPHA(1): Clock low when inactive, Data is valid on clock trailing edge.
	SPI_MODE_2 = 0x02,	//!< CPOL(1), CPHA(0): Clock high when inactive, Data is valid on clock leading edge.
	SPI_MODE_3 = 0x03,	//!< CPOL(1), CPHA(1): Clock high when inactive, Data is valid on clock trailing edge.
} spi_tx_mode_t;

/**
 * @enum	spi_ds_mode_t
 * @brief	SPI data shifting modes.
 */
typedef enum {
	LSB_FIRST = true,	//!< Send LSB bit first
	MSB_FIRST = false	//!< Send MSB bit first
} spi_ds_mode_t;

//###############################################
/**
 * @union	spi_reg_t
 * @brief	General register handler.
 */
typedef union {
	uint8_t		raw;		//!< Raw data.
	spi_spcr_t 	spcr;		//!< SPI control register.
	spi_spsr_t	spsr;		//!< SPI status register.
} spi_reg_t;

/**
 * @union	spi_ctrl_t
 * @brief	Control register handler.
 */
typedef union {
	uint8_t 	raw;		//!< Raw data.
	spi_spcr_t	field;      //!< Data formatted as control register.
} spi_ctrl_t;

/**
 * @union	spi_stat_t
 * @brief	Status register handler.
 */
typedef union {
	uint8_t 	raw;		//!< Raw data.
	spi_spsr_t	field;      //!< Data formatted as status register.
} spi_stat_t;

/**
 * @struct	spi_conf_t
 * @brief	SPI configuration.
 */
typedef struct {
	spi_ctrl_t	ctrl;		//!< Control register.
	spi_stat_t	stat;       //!< Status register.
} spi_conf_t;

//###############################################
#endif /* _HARDWARE_SPI_DEFS_H_ */
//###############################################
