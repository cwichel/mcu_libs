/**
 * @code 	#include "hardware/uart_defs.h" @endcode
 * @file	libs/hardware/uart_defs.h
 * @brief	AVR interrupt UART library definitions.
 */
//###############################################
#ifndef _HARDWARE_UART_DEFS_H_
#define _HARDWARE_UART_DEFS_H_
//###############################################
#include <hardware/gpio.h>
#include <utils/stream.h>
//###############################################
#if defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__)
#define AVR_UART1								//!< UART1 defined on the board.
#define UART1_RX_VECT		USART1_RX_vect		//!< UART1 RX ISR vector.
#define UART1_TX_VECT		USART1_UDRE_vect	//!< UART1 TX ISR vector.
#define UART1_CTRL			UCSR1A				//!< UART1 control registers initial address.
#define UART1_UBRR			UBRR1				//!< UART1 baudrate register address.
#define UART1_DATA			UDR1				//!< UART1 data register address.

#elif  defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__)
#define AVR_UART0								//!< UART0 defined on the board.
#define UART0_RX_VECT		USART_RX_vect		//!< UART0 RX ISR vector.
#define UART0_TX_VECT		USART_UDRE_vect     //!< UART0 TX ISR vector.
#define UART0_CTRL			UCSR0A              //!< UART0 control registers initial address.
#define UART0_UBRR			UBRR0              	//!< UART0 baudrate register address.
#define UART0_DATA			UDR0                //!< UART0 data register address.

#endif

//###############################################
#define UART_BUFF_SIZE	64						//!< UARTx buffer size.
#define UART_BUFF_MASK	(UART_BUFF_SIZE - 1)	//!< UARTx buffer mask.

#if (UART_BUFF_SIZE & UART_BUFF_MASK)
#error UART buffers size is not a power of 2.
#endif

//###############################################
/**
 * @struct	uart_ucsra_t
 * @brief	UART status register fields.
 */
typedef struct {
	uint8_t mpcm	: 1;	//!< Multi-processor communication mode.
	uint8_t u2x		: 1;	//!< 2x transmission speed.
	uint8_t upe		: 1;    //!< Parity error.
	uint8_t dor		: 1;    //!< Data overrun.
	uint8_t fe		: 1;    //!< Frame error.
	uint8_t udre	: 1;    //!< Data register empty.
	uint8_t txc		: 1;    //!< Transmission complete.
	uint8_t rxc		: 1;    //!< Receive complete.
} uart_ucsra_t;

/**
 * @struct	uart_ucsrb_t
 * @brief	UART Control 1 register fields.
 */
typedef struct {
	uint8_t txb8	: 1;	//!< TX bit 9.
	uint8_t rxb8	: 1;    //!< RX bit 9.
	uint8_t ucsz2	: 1;    //!< Data size (MSB).
	uint8_t txen	: 1;    //!< TX enable.
	uint8_t rxen	: 1;    //!< RX enable.
	uint8_t udrie	: 1;    //!< Interrupt enable on data register empty.
	uint8_t txcie	: 1;    //!< Interrupt enable on TX complete.
	uint8_t rxcie	: 1;    //!< Interrupt enable on RX complete.
} uart_ucsrb_t;

/**
 * @struct	uart_ucsrc_t
 * @brief	UART Control 2 register fields.
 */
typedef struct {
	uint8_t ucpol	: 1;	//!< Clock polarity (UART only).
	uint8_t ucsz	: 2;   	//!< Data size (LSB).
	uint8_t usbs	: 1;    //!< Stop bits.
	uint8_t upm		: 2;    //!< Parity mode.
	uint8_t umsel	: 2;    //!< UART operation mode.
} uart_ucsrc_t;

/**
 * @struct	uart_ucsrd_t
 * @brief	UART Control 3 register fields.
 */
typedef struct {
	uint8_t rtsen	: 1;	//!< ~RTS signal enable.
	uint8_t ctsen	: 1;	//!< ~CTS signal enable.
	uint8_t unused	: 6;	//!< Unused bit.
} uart_ucsrd_t;

//###############################################
/**
 * @enum	uart_inst_t
 * @brief	UART Available instances.
 */
typedef enum {
	#if defined(AVR_UART0)
	UART_0 				= 0x00,
	#endif
	#if defined(AVR_UART1)
	UART_1 				= 0x01
	#endif
} uart_inst_t;

/**
 * @enum	uart_pmode_t
 * @brief	Parity mode.
 */
typedef enum {
	UART_PAR_DISABLED 	= 0x00,	//!< Parity disabled.
	UART_PAR_EVEN		= 0x02, //!< Parity enabled, even.
	UART_PAR_ODD		= 0x03  //!< Parity enabled, odd.
} uart_pmode_t;

/**
 * @enum	uart_stbit_t
 * @brief	Stop bits selection.
 */
typedef enum {
	UART_ST_1BIT		= 0x00,	//!< 1 stop bit.
	UART_ST_2BIT		= 0x01	//!< 2 stop bit.
} uart_stbit_t;

/**
 * @enum	uart_msgbit_t
 * @brief	Data size selection.
 */
typedef enum {
	UART_MSG_5BIT		= 0x00,	//!< 5bit data size.
	UART_MSG_6BIT		= 0x01, //!< 6bit data size.
	UART_MSG_7BIT		= 0x02, //!< 7bit data size.
	UART_MSG_8BIT		= 0x03, //!< 8bit data size.
} uart_msgbit_t;

//###############################################
/**
 * @union	uart_stat_t
 * @brief	Status register handler.
 */
typedef union {
	uint8_t		 raw;		//!< Raw data.
	uart_ucsra_t field;		//!< Data formatted as Status register.
} uart_stat_t;

/**
 * @union	uart_ctrl1_t
 * @brief	Control 1 register handler.
 */
typedef union {
	uint8_t		 raw;		//!< Raw data.
	uart_ucsrb_t field;     //!< Data formatted as Control 2 register.
} uart_ctrl1_t;

/**
 * @union	uart_ctrl2_t
 * @brief	Control 2 register handler.
 */
typedef union {
	uint8_t		 raw;		//!< Raw data.
	uart_ucsrc_t field;     //!< Data formatted as Control 2 register.
} uart_ctrl2_t;

/**
 * @union	uart_ctrl3_t
 * @brief	Control 3 register handler.
 */
typedef union {
	uint8_t		 raw;		//!< Raw data.
	uart_ucsrd_t field;     //!< Data formatted as Control 3 register.
} uart_ctrl3_t;

//###############################################
/**
 * @struct	uart_ctrl_t
 * @brief	Control registers.
 */
typedef struct {
	volatile uart_stat_t	stat;	//!< Status register (UCSRxA).
	volatile uart_ctrl1_t	ctrl1;  //!< Control 1 register (UCSRxB).
	volatile uart_ctrl2_t	ctrl2;  //!< Control 1 register (UCSRxC).
	volatile uart_ctrl3_t	ctrl3;  //!< Control 1 register (UCSRxD).
} uart_ctrl_t;

//###############################################
#endif /* _HARDWARE_UART_DEFS_H_ */
//###############################################
