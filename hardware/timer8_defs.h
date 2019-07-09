/**
 * @code 	#include "hardware/timer8_defs.h" @endcode
 * @file	libs/hardware/timer8_defs.h
 * @brief	AVR 8bit Timer library definitions.
 */
//###############################################
#ifndef _HARDWARE_TIMER8_DEFS_H_
#define _HARDWARE_TIMER8_DEFS_H_
//###############################################
#include <hardware/gpio.h>
//###############################################
#define TIMER8_RESOLUTION		256UL		//!< 8bit timer resolution.

//###############################################
#if defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__)
#define AVR_TIMER0							//!< TIMER0 defined on the board.
#define TIMER0_ISR_F	TIMER0_OVF_vect     //!< TIMER0 ISR vector (fixed mode).
#define TIMER0_ISR_C	TIMER0_COMPA_vect	//!< TIMER0 ISR vector (custom mode).
#define TIMER0_CTRL		TCCR0A              //!< TIMER0 control registers initial address.
#define TIMER0_TCNT		TCNT0               //!< TIMER0 counter configuration initial address.
#define TIMER0_TIMSK	TIMSK0              //!< TIMER0 interruption configuration register address.
#define TIMER0_PIN_A	PTB7                //!< TIMER0 channel A pin.
#define TIMER0_PIN_B	PTD0                //!< TIMER0 channel B pin.

#elif  defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__)
#define AVR_TIMER0							//!< TIMER0 defined on the board.
#define TIMER0_ISR_F	TIMER0_OVF_vect     //!< TIMER0 ISR vector (fixed mode).
#define TIMER0_ISR_C	TIMER0_COMPA_vect	//!< TIMER0 ISR vector (custom mode).
#define TIMER0_CTRL		TCCR0A              //!< TIMER0 control registers initial address.
#define TIMER0_TCNT		TCNT0               //!< TIMER0 counter configuration initial address.
#define TIMER0_TIMSK	TIMSK0              //!< TIMER0 interruption configuration register address.
#define TIMER0_PIN_A	PTD6                //!< TIMER0 channel A pin.
#define TIMER0_PIN_B	PTD5                //!< TIMER0 channel B pin.

#define AVR_TIMER2							//!< TIMER2 defined on the board.
#define TIMER2_ISR_F	TIMER2_OVF_vect     //!< TIMER2 ISR vector (fixed mode).
#define TIMER2_ISR_C	TIMER2_COMPA_vect	//!< TIMER2 ISR vector (custom mode).
#define TIMER2_CTRL		TCCR2A              //!< TIMER2 control registers initial address.
#define TIMER2_TCNT		TCNT2               //!< TIMER2 counter configuration initial address.
#define TIMER2_TIMSK	TIMSK2              //!< TIMER2 interruption configuration register address.
#define TIMER2_PIN_A	PTB3                //!< TIMER2 channel A pin.
#define TIMER2_PIN_B	PTD3                //!< TIMER2 channel B pin.
#endif

//###############################################
/**
 * @struct	timer8_tccra_t
 * @brief	8bit Timer Control A register fields.
 */
typedef struct {
	uint8_t wgm01	: 2;	//!< Waveform Generator Mode (LSB).
	uint8_t unused	: 2;    //!< Unused bit.
	uint8_t comb	: 2;    //!< Compare Output Mode for channel A.
	uint8_t coma	: 2;    //!< Compare Output Mode for channel B.
} timer8_tccra_t;

/**
 * @struct	timer8_tccrb_t
 * @brief	8bit Timer Control B register fields.
 */
typedef struct {
	uint8_t cs		: 3;	//!< Clock select.
	uint8_t wgm2	: 1;    //!< Waveform Generator Mode (LSB).
	uint8_t unused	: 2;    //!< Unused bit.
	uint8_t focb	: 1;    //!< Force Output Compare for channel A.
	uint8_t foca	: 1;    //!< Force Output Compare for channel B.
} timer8_tccrb_t;

/**
 * @struct	timer8_timsk_t
 * @brief	8bit Timer Interrupt Mask register fields.
 */
typedef struct {
	uint8_t toie	: 1;	//!< Timer Overflow Interrupt Enable.
	uint8_t ociea	: 1;    //!< Output Compare Interrupt Enable for channel A.
	uint8_t ocieb	: 1;    //!< Output Compare Interrupt Enable for channel B.
	uint8_t unused	: 5;    //!< Unused bit.
} timer8_timsk_t;

//###############################################
/**
 * @enum 	timer8_inst_t
 * @brief	8bit Timer available instances.
 */
typedef enum {
	#if defined(AVR_TIMER0)
	TIMER_0 		= 0x00,
	#endif
	#if defined(AVR_TIMER2)
	TIMER_2 		= 0x02
	#endif
} timer8_inst_t;

/**
 * @enum	timer8_cs_t
 * @brief	8bit Timer prescalers.
 */
typedef enum {
	T8_CS_OFF		= 0x00,	//!< Timer stopped.
	T8_CS_1			= 0x01,	//!< No prescaler.
	T8_CS_8			= 0x02, //!< F_CPU/8	-> 2^3.
	T8_CS_64		= 0x03, //!< F_CPU/64	-> 2^6.
	T8_CS_256		= 0x04, //!< F_CPU/256	-> 2^8.
	T8_CS_1024		= 0x05, //!< F_CPU/1024	-> 2^10.
} timer8_cs_t;

/**
 * @enum	timer8_com_t
 * @brief	8bit Timer Compare Output Modes (CM = Compare Match).
 */
typedef enum {
	T8_COM_OFF		= 0x00,	//!< Normal port operation, OCx disconnected.
	T8_COM_CTC_TGL	= 0x01, //!< CTC mode: Toggle on CM.
	T8_COM_CTC_CLR	= 0x02, //!< CTC mode: Clear on CM.
	T8_COM_CTC_SET	= 0x03, //!< CTC mode: Set on CM.
	T8_COM_PWM		= 0x02, //!< PWM mode: Normal.
	T8_COM_PWM_INV	= 0x03, //!< PWM mode: Inverted.
} timer8_com_t;

/**
 * @enum	timer8_om_t
 * @brief	8bit Timer Operation Modes. The timer can be controlled using the overflow
 * 			mode or the OCRA register.
 */
typedef enum {
	T8_OM_CTC			= 0x00,	//!< Normal mode.
	T8_OM_CTC_OCRA		= 0x02, //!< CTC mode (period given by OCRA).
	T8_OM_FPWM			= 0x03, //!< Fast PWM mode (period given by TCNT overflow).
	T8_OM_FPWM_OCRA		= 0x13, //!< Fast PWM mode (period given by OCRA).
	T8_OM_CPWM_P		= 0x01, //!< Phase Correct PWM mode (period given by TCNT overflow).
	T8_OM_CPWM_P_OCRA	= 0x11, //!< Phase Correct PWM mode (period given by OCRA).
} timer8_om_t;

//###############################################
/**
 * @union	timer8_ctrla_t
 * @brief	Control A register handler.
 */
typedef union {
	uint8_t			raw;	//!< Raw data.
	timer8_tccra_t	field;  //!< Control A register fields.
} timer8_ctrla_t;

/**
 * @union	timer8_ctrlb_t
 * @brief	Control B register handler.
 */
typedef union {
	uint8_t			raw;	//!< Raw data.
	timer8_tccrb_t	field;  //!< Control B register fields.
} timer8_ctrlb_t;

/**
 * @union	timer8_int_t
 * @brief	Interruption mask register handler.
 */
typedef union {
	uint8_t			raw;	//!< Raw data.
	timer8_timsk_t	field;  //!< Interruption Control register fields.
} timer8_int_t;

//###############################################
/**
 * @struct	timer8_ctrl_t
 * @brief	Control registers.
 */
typedef struct {
	volatile timer8_ctrla_t		ctrla;	//!< Control A register (TCCRxA).
	volatile timer8_ctrlb_t		ctrlb;  //!< Control B register (TCCRxB).
} timer8_ctrl_t;

/**
 * @struct	timer16_cfg_t
 * @brief	Counter configuration registers.
 */
typedef struct {
	volatile uint8_t			tcnt;	//!< Timer Counter (TCNTx).
	volatile uint8_t			ocra;	//!< Output Compare Register (OCRxA).
	volatile uint8_t			ocrb;   //!< Output Compare Register (OCRxB).
} timer8_cfg_t;

//###############################################
#endif /* _HARDWARE_TIMER8_DEFS_H_ */
//###############################################
