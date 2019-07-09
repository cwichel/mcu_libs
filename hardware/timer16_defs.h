/**
 * @code 	#include "hardware/timer16_defs.h" @endcode
 * @file	libs/hardware/timer16_defs.h
 * @brief	AVR 16bit Timer library definitions.
 */
//###############################################
#ifndef _HARDWARE_TIMER16_DEFS_H_
#define _HARDWARE_TIMER16_DEFS_H_
//###############################################
#include <hardware/gpio.h>

//###############################################
#define TIMER16_RESOLUTION		65536UL		//!< 16bit timer resolution.

//###############################################
#if defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__)
#define AVR_TIMER1							//!< TIMER1 defined on the board.
#define TIMER1_ISR		TIMER1_CAPT_vect	//!< TIMER1 ISR vector.
#define TIMER1_CTRL		TCCR1A              //!< TIMER1 control registers initial address.
#define TIMER1_TCNT		TCNT1              	//!< TIMER1 counter configuration initial address.
#define TIMER1_TIMSK	TIMSK1              //!< TIMER1 interruption configuration register address.
#define TIMER1_PIN_A	PTB5                //!< TIMER1 channel A pin.
#define TIMER1_PIN_B	PTB6                //!< TIMER1 channel B pin.
#define TIMER1_PIN_C	PTB7                //!< TIMER1 channel C pin.

#define AVR_TIMER3							//!< TIMER3 defined on the board.
#define TIMER3_ISR		TIMER3_CAPT_vect    //!< TIMER3 ISR vector.
#define TIMER3_CTRL		TCCR3A              //!< TIMER3 control registers initial address.
#define TIMER3_TCNT		TCNT3              	//!< TIMER3 counter configuration initial address.
#define TIMER3_TIMSK	TIMSK3              //!< TIMER3 interruption configuration register.
#define TIMER3_PIN_A	PTC6                //!< TIMER3 channel A pin.

#elif  defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__)
#define AVR_TIMER1							//!< TIMER1 defined on the board.
#define TIMER1_ISR		TIMER1_CAPT_vect	//!< TIMER1 ISR vector.
#define TIMER1_CTRL		TCCR1A              //!< TIMER1 control registers initial address.
#define TIMER1_TCNT		TCNT1              	//!< TIMER1 counter configuration initial address.
#define TIMER1_TIMSK	TIMSK1              //!< TIMER1 interruption configuration register.
#define TIMER1_PIN_A	PTB1                //!< TIMER1 channel A pin.
#define TIMER1_PIN_B	PTB2                //!< TIMER1 channel B pin.

#else
#error Timer16.h: AVR MCU not supported
#endif

//###############################################
/**
 * @struct	timer16_ctrla_t
 * @brief	16bit Timer Control A register fields.
 */
typedef struct {
	uint8_t wgm01	: 2;	//!< Waveform Generator Mode (LSB).
	uint8_t comc	: 2;    //!< Compare Output Mode for channel C.
	uint8_t comb	: 2;    //!< Compare Output Mode for channel B.
	uint8_t coma	: 2;    //!< Compare Output Mode for channel A.
} timer16_tccra_t;

/**
 * @struct	timer16_ctrlb_t
 * @brief	16bit Timer Control B register fields.
 */
typedef struct {
	uint8_t cs		: 3;	//!< Clock select.
	uint8_t wgm23	: 2;    //!< Waveform Generator Mode (MSB).
	uint8_t unused	: 1;	//!< Unused bit.
	uint8_t ices	: 1;    //!< Input Capture Edge Select.
	uint8_t icnc	: 1;    //!< Input Capture Noise Canceler.
} timer16_tccrb_t;

/**
 * @struct	timer16_ctrlc_t
 * @brief	16bit Timer Control C register fields.
 */
typedef struct {
	uint8_t unused	: 5;	//!< Unused bit.
	uint8_t focc	: 1;    //!< Force Output Compare for channel C.
	uint8_t focb	: 1;	//!< Force Output Compare for channel B.
	uint8_t foca	: 1;    //!< Force Output Compare for channel A.
} timer16_tccrc_t;

/**
 * @struct	timer16_timsk_t
 * @brief	16bit Timer Interrupt Mask register fields.
 */
typedef struct {
	uint8_t toie	: 1;	//!< Timer Overflow Interrupt Enable.
	uint8_t ociea	: 1;    //!< Output Compare Interrupt Enable for channel A.
	uint8_t ocieb	: 1;	//!< Output Compare Interrupt Enable for channel B.
	uint8_t ociec	: 1;    //!< Output Compare Interrupt Enable for channel C.
	uint8_t unused_1: 1;    //!< Unused bit.
	uint8_t icie	: 1;    //!< Input Capture Interrupt Enable.
	uint8_t unused_2: 2;    //!< Unused bit.
} timer16_timsk_t;

//###############################################
/**
 * @enum 	timer16_inst_t
 * @brief	16bit Timer available instances.
 */
typedef enum {
	#if defined(AVR_TIMER1)
	TIMER_1 		= 0x01,
	#endif
	#if defined(AVR_TIMER3)
	TIMER_3 		= 0x03
	#endif
} timer16_inst_t;

/**
 * @enum	timer16_cs_t
 * @brief	16bit Timer prescalers.
 */
typedef enum {
	T16_CS_OFF		= 0x00,	//!< Timer stopped.
	T16_CS_1		= 0x01,	//!< No prescaler.
	T16_CS_8		= 0x02, //!< F_CPU/8	-> 2^3.
	T16_CS_64		= 0x03, //!< F_CPU/64	-> 2^6.
	T16_CS_256		= 0x04, //!< F_CPU/256	-> 2^8.
	T16_CS_1024		= 0x05, //!< F_CPU/1024	-> 2^10.
} timer16_cs_t;

/**
 * @enum	timer16_com_t
 * @brief	16bit Timer Compare Output Modes (CM = Compare Match).
 */
typedef enum {
	T16_COM_OFF		= 0x00,	//!< Normal port operation. OCx disconnected.
	T16_COM_CTC_TGL	= 0x01, //!< CTC mode: Toggle on CM.
	T16_COM_CTC_CLR	= 0x02, //!< CTC mode: Clear on CM.
	T16_COM_CTC_SET	= 0x03, //!< CTC mode: Set on CM.
	T16_COM_PWM		= 0x02, //!< PWM mode: Normal.
	T16_COM_PWM_INV	= 0x03, //!< PWM mode: Inverted.
} timer16_com_t;

/**
 * @enum	timer16_om_t
 * @brief	16bit Timer Operation Modes. The time period is controlled in all cases with ICR.
 */
typedef enum {
	T16_OM_CTC		= 0x30, //!< CTC mode.
	T16_OM_FPWM		= 0x32, //!< Fast PWM mode.
	T16_OM_CPWM_P	= 0x22, //!< Phase Correct PWM mode.
	T16_OM_CPWM_PF	= 0x20, //!< Phase and Frequency Correct PWM mode.
} timer16_om_t;

//###############################################
/**
 * @union	timer16_ctrla_t
 * @brief	Control A register handler.
 */
typedef union {
	uint8_t			raw;	//!< Raw data.
	timer16_tccra_t	field; 	//!< Control A register fields.
} timer16_ctrla_t;

/**
 * @union	timer16_ctrlb_t
 * @brief	Control B register handler.
 */
typedef union {
	uint8_t			raw;	//!< Raw data.
	timer16_tccrb_t	field; 	//!< Control B register fields.
} timer16_ctrlb_t;

/**
 * @union	timer16_ctrlc_t
 * @brief	Control C register handler.
 */
typedef union {
	uint8_t			raw;	//!< Raw data.
	timer16_tccrc_t	field; 	//!< Control C register fields.
} timer16_ctrlc_t;

/**
 * @union	timer16_int_t
 * @brief	Interruption mask register handler.
 */
typedef union {
	uint8_t			raw;	//!< Raw data.
	timer16_timsk_t	field; 	//!< Interruption Control register fields.
} timer16_int_t;

//###############################################
/**
 * @struct	timer16_ctrl_t
 * @brief	Control registers.
 */
typedef struct {
	volatile timer16_ctrla_t	ctrla;	//!< Control A register (TCCRxA).
	volatile timer16_ctrlb_t	ctrlb;  //!< Control B register (TCCRxB).
	volatile timer16_ctrlc_t	ctrlc;  //!< Control C register (TCCRxC).
} timer16_ctrl_t;

/**
 * @struct	timer16_cfg_t
 * @brief	Counter configuration registers.
 */
typedef struct {
	volatile uint16_t			tcnt;	//!< Timer Counter (TCNTx).
	volatile uint16_t			icr;    //!< Input Capture Register (ICRx).
	volatile uint16_t			ocra;   //!< Output Compare Register (OCRxA).
	volatile uint16_t			ocrb;   //!< Output Compare Register (OCRxB).
	volatile uint16_t			ocrc;   //!< Output Compare Register (OCRxC).
} timer16_cfg_t;

//###############################################
#endif /* _HARDWARE_TIMER16_DEFS_H_ */
//###############################################
