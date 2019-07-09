/**
 * @code	#include "utils/stream_defs.h" @endcode
 * @file 	libs/utils/stream_defs.h
 * @brief 	Stream handling library definitions.
 */
//###############################################
#ifndef _UTILS_STRING_DEFS_H_
#define _UTILS_STRING_DEFS_H_
//###############################################
extern "C" {
#include <ctype.h>
#include <avr/pgmspace.h>
}
#include <base/base.h>
//###############################################
/**
 * @enum	num_base_t
 * @brief	Bases used to number conversion.
 */
typedef enum {
	NUM_BASE_BIN = 0x02,	//!< Binary base.
	NUM_BASE_OCT = 0x08, 	//!< Octal base.
	NUM_BASE_DEC = 0x0A,  	//!< Decimal base.
	NUM_BASE_HEX = 0x10  	//!< Hexadecimal base.
} num_base_t;

//###############################################
#endif /* _UTILS_STRING_DEFS_H_ */
//###############################################
